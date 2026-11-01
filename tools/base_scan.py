"""The base-register scan of the game code (plan.md steps 79, 81 and 90): which data objects the game addresses, how often
and at which offsets, from the asm of the last batch 9 build (decomp/build/GXCE01/asm).

An access is a load or store `op rX, off(rB)` where rB holds the address of a data symbol: set by `addi rB, rA, SYM@l`
(after `lis rA, SYM@ha`), or the symbol itself `op rX, SYM@l(rA)` / `SYM@sda21(r13)` (offset 0, direct). The register
stays the symbol's base (`lwz rB, SYM@sda21(r13)` / `SYM@l(rA)`: the base of *SYM, the object a pointer variable
points at) until an instruction writes it (first operand of anything but a store, compare or branch) or
the function ends; a branch target does not reset it (the scan is a count, not a proof: the fields it lists are then
read in the code). Only functions of the game's code range are scanned (below the first library object, 0x80131000).

Usage: python -I tools/base_scan.py top [N] [--default]       objects by access count (--default: dtk default names only)
       python -I tools/base_scan.py fields SYM [SYM ...]      per offset: width / sign / load-store and the functions
"""
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

G = Path(__file__).resolve().parent.parent
ASM = G / "decomp/build/GXCE01/asm"
SYMS = G / "decomp/config/GXCE01/symbols.txt"
GAME_END = 0x80131000
INS = re.compile(r"^/\* ([0-9A-F]{8}) [0-9A-F]{8}  (?:[0-9A-F]{2} ){4}\*/\t(\S+)\s*(.*)$")
MEM = re.compile(r"^(r\d+|f\d+), (-?(?:0x[0-9a-fA-F]+|\d+)|(\w+)@(?:l|sda21))\((r\d+)\)$")
SYM_L = re.compile(r"^(r\d+), (r\d+), (\w+)@l$")
WIDTH = {"lbz": (1, "u", "L"), "lbzu": (1, "u", "L"), "lbzx": (1, "u", "L"), "stb": (1, "", "S"), "stbu": (1, "", "S"),
         "lhz": (2, "u", "L"), "lhzu": (2, "u", "L"), "lha": (2, "s", "L"), "lhau": (2, "s", "L"), "sth": (2, "", "S"),
         "sthu": (2, "", "S"), "lwz": (4, "", "L"), "lwzu": (4, "", "L"), "stw": (4, "", "S"), "stwu": (4, "", "S"),
         "lfs": (4, "f", "L"), "lfsu": (4, "f", "L"), "stfs": (4, "f", "S"), "stfsu": (4, "f", "S"), "lfd": (8, "d", "L"),
         "lfdu": (8, "d", "L"), "stfd": (8, "d", "S"), "stfdu": (8, "d", "S"), "lmw": (4, "m", "L"), "stmw": (4, "m", "S")}
NOWRITE = re.compile(r"^(st|cmp|b|mt|tw|dcb|icb|sync|isync|crxor|cror|mtcrf)")


def data_symbols():
    out = {}
    for ln in SYMS.read_text(encoding="utf-8").splitlines():
        m = re.match(r"^(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // type:object size:0x([0-9A-Fa-f]+)", ln)
        if m and m.group(2) in (".data", ".bss", ".sdata", ".sbss", ".rodata", ".sdata2", ".sbss2"):
            out[m.group(1)] = (m.group(2), int(m.group(3), 16), int(m.group(4), 16))
    return out


def scan():
    """-> [(symbol, offset, mnemonic, function, address)]"""
    syms = data_symbols()
    acc = []
    for p in sorted(ASM.glob("*.s")):
        fn, base = None, {}
        for ln in p.read_text(encoding="utf-8", errors="replace").splitlines():
            if ln.startswith(".fn "):
                fn, base = ln.split()[1].rstrip(","), {}
                continue
            m = INS.match(ln)
            if not m or fn is None:
                continue
            addr, op, args = int(m.group(1), 16), m.group(2), m.group(3).strip()
            if addr >= GAME_END:
                continue
            s = SYM_L.match(args)
            if op == "addi" and s and s.group(3) in syms:
                base[s.group(1)] = s.group(3)
                continue
            mm = MEM.match(args)
            if op in WIDTH and mm:
                rb, sym = mm.group(4), mm.group(3)
                if sym in syms:
                    acc.append((sym, 0, op, fn, addr))
                    if op == "lwz" and mm.group(1).startswith("r"):
                        base[mm.group(1)] = "*" + sym   # a pointer variable: its target's fields (fields *SYM)
                        continue
                elif rb in base and not sym:
                    acc.append((base[rb], int(mm.group(2), 0), op, fn, addr))
                if op.endswith("u"):
                    base.pop(rb, None)
            if not NOWRITE.match(op):
                d = args.split(",")[0].strip()
                if d in base and not (op in WIDTH and WIDTH[op][2] == "S"):
                    base.pop(d, None)
    return syms, acc


def main(argv):
    if not argv:
        sys.exit(__doc__)
    syms, acc = scan()
    if argv[0] == "top":
        n = int(argv[1]) if len(argv) > 1 and argv[1].isdigit() else 40
        dflt = "--default" in argv
        cnt, fns = Counter(), defaultdict(set)
        for sym, off, op, fn, a in acc:
            cnt[sym] += 1
            fns[sym].add(fn)
        k = 0
        for sym, c in cnt.most_common():
            if dflt and not sym.startswith("lbl_"):
                continue
            sec, a, sz = syms.get(sym.lstrip("*"), ("?", 0, 0))
            print(f"{sym:<28} {sec:<7} 0x{a:08X} size 0x{sz:<6X} {c:>5} accesses {len(fns[sym]):>4} functions")
            k += 1
            if k >= n:
                break
        return 0
    if argv[0] == "fields":
        for want in argv[1:]:
            by = defaultdict(lambda: [Counter(), set()])
            for sym, off, op, fn, a in acc:
                if sym == want:
                    by[off][0][op] += 1
                    by[off][1].add(fn)
            sec, a, sz = syms.get(want.lstrip("*"), ("?", 0, 0))
            print(f"== {want} {sec} 0x{a:08X} size 0x{sz:X}: {sum(sum(v[0].values()) for v in by.values())} accesses")
            for off in sorted(by):
                ops, fs = by[off]
                print(f"  +0x{off:<5X} {' '.join(f'{o}x{c}' for o, c in sorted(ops.items())):<30} "
                      f"{len(fs):>3} fn: {', '.join(sorted(fs)[:6])}{' ...' if len(fs) > 6 else ''}")
        return 0
    sys.exit(__doc__)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
