"""Generate decomp/config/GXCE01/symbols.txt: decomp-toolkit's own analysis of main.dol, with our verified names.

Inputs (all committed or made by tools/setup.ps1):
  tools/dtk/proj/symbols.txt   dtk v1.8.4's analysis of the retail main.dol (every symbol, section, size, scope, type)
  symbols/symbols.txt          library names (hand, layout-proof and lib-candidate lines: all passed the cross-check)
  symbols/descriptive.txt      our cr_ names for game functions (hand + "// game-real" lines)
  symbols/cr_globals.txt       our cr_g_ names for game globals
  symbols/data_splits.txt      cuts in dtk's data objects (plan.md step 90): where dtk saw one object, the code shows two

Rule: only the NAME of an existing dtk symbol changes. Section, address, size, type, scope and data attributes stay
exactly dtk's, and no symbol is removed, so dtk's analysis (and the linked main.dol) cannot change. The one exception
(step 90): a cut of symbols/data_splits.txt (section, address, the code that shows it) inside a dtk data object ends that
object there and starts a new one, lbl_<address>, with the rest of its bytes (a name of ours at that address then
applies as to any dtk symbol); the bytes, and so main.dol, are unchanged.
(Afterwards tools/gen_dtk_splits.py cuts a dtk symbol that runs on over the zero padding after a C-proven data piece
where that piece ends: its trailing-padding rule, step 61 iteration 8; the bytes are unchanged.)
A name is applied to the dtk symbol at the same address and of the same kind (function <-> function,
object <-> object / label). It is NOT applied, and the dtk default name is kept, when:
  - it is not a valid linker identifier (a C name, or CodeWarrior's local-static form name$123);
  - two of our files give the same address different names, or the same name to different addresses;
  - another dtk symbol (one we do not rename) already has that name;
  (except for statics: a name every one of our lines gives as scope:local may be on several symbols, as dtk's own
  AlarmHandler is; the renamed symbols are then written scope:local, so the all-asm link does not define it twice:
  step 68, reverb_hi.c's and reverb_std.c's HandleReverb)
  - dtk has no symbol of that kind at that address (e.g. a name for a field inside a larger dtk object).
Every such case is listed in decomp/reports/symbols_report.txt. At one address, labels are written after the
sized symbol (see the comment at the end: mwld hangs otherwise). Output is byte-identical on every run (LF, as dtk
writes its own files; dtk rewrites this file on a build only if its analysis differs, which batch 9 checks).

Usage: python -I tools/gen_dtk_symbols.py
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

G = Path(__file__).resolve().parent.parent
DTK = G / "tools/dtk/proj/symbols.txt"
OURS = [G / "symbols/symbols.txt", G / "symbols/descriptive.txt", G / "symbols/cr_globals.txt"]
SPLITS = G / "symbols/data_splits.txt"
CUT = re.compile(r"^\s*(\.[\w.]+):0x([0-9A-Fa-f]+)\s*//\s*why:\s*\S")
OUT = G / "decomp/config/GXCE01/symbols.txt"
REPORT = G / "decomp/reports/symbols_report.txt"

LINE = re.compile(r"^\s*([^\s=]+)\s*=\s*(\.[\w.]+):0x([0-9A-Fa-f]+);\s*//(.*)$")
# C identifiers, plus CodeWarrior's own names for function-local statics (name$123), which mwld and dtk accept
VALID = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*(\$[0-9]+)?$")
DEFAULT = re.compile(r"^(fn|lbl|jumptable|gap|switch|float|double|string|wstring)_[0-9A-Fa-f_]+$|^@")


def kind(comment: str) -> str:
    m = re.search(r"type:(\w+)", comment)
    t = m.group(1) if m else "object"
    return "function" if t == "function" else "object"


def apply_cuts(dtk):
    """symbols/data_splits.txt: `.section:0xADDR // why: ...` -> the dtk object of that section around ADDR ends there and
    a new object lbl_ADDR takes the rest of its bytes (in place in dtk's list). A cut that is not strictly inside a dtk
    object of its section stops the run (the file is out of date)."""
    if not SPLITS.exists():
        return []
    cuts = []
    for line in SPLITS.read_text(encoding="utf-8").splitlines():
        if line.lstrip().startswith("//") or not line.strip():
            continue
        m = CUT.match(line)
        if not m:
            sys.exit(f"{SPLITS.name}: unparsed line (`.section:0xADDR // why: ...`): {line}")
        cuts.append((m.group(1), int(m.group(2), 16)))
    for sec, x in sorted(cuts, key=lambda c: c[1]):
        hit = None
        for i, s in enumerate(dtk):
            m = re.search(r"size:0x([0-9A-Fa-f]+)", s[4])
            if s[1] == sec and s[3] == "object" and m and s[2] < x < s[2] + int(m.group(1), 16):
                hit = (i, int(m.group(1), 16))
        if hit is None:
            sys.exit(f"{SPLITS.name}: {sec}:0x{x:08X} is not inside a dtk object of {sec}")
        i, size = hit
        a = dtk[i][2]
        dtk[i][4] = re.sub(r"size:0x[0-9A-Fa-f]+", f"size:0x{x - a:X}", dtk[i][4])
        n = f"lbl_{x:08X}"
        dtk.insert(i + 1, [n, sec, x, "object", f"{n} = {sec}:0x{x:08X}; // type:object size:0x{a + size - x:X}"])
    return cuts


def main() -> int:
    dtk = []  # [name, section, addr, kind, line]
    for line in DTK.read_text(encoding="utf-8").splitlines():
        m = LINE.match(line)
        if not m:
            sys.exit(f"unparsed dtk line: {line}")
        dtk.append([m.group(1), m.group(2), int(m.group(3), 16), kind(m.group(4)), line])
    cuts = apply_cuts(dtk)
    by_addr = defaultdict(list)
    for i, s in enumerate(dtk):
        by_addr[(s[2], s[3])].append(i)

    report = []
    # our names: (addr, kind) -> {name: [source files]}
    want = defaultdict(lambda: defaultdict(list))
    local = set()  # (addr, name) given as scope:local: a static, which several files may name alike (step 68)
    for f in OURS:
        for line in f.read_text(encoding="utf-8").splitlines():
            if line.lstrip().startswith("//") or not line.strip():
                continue
            m = LINE.match(line)
            if not m:
                sys.exit(f"{f.name}: unparsed line: {line}")
            want[(int(m.group(3), 16), kind(m.group(4)))][m.group(1)].append(f.name)
            if "scope:local" in m.group(4):
                local.add((int(m.group(3), 16), m.group(1)))

    rename = {}  # dtk index -> new name
    for (addr, k), names in sorted(want.items()):
        where = f"0x{addr:08X} ({k})"
        if len(names) > 1:
            report.append(f"CONFLICT  {where}: different names {sorted(names)}; dtk name kept")
            continue
        name = next(iter(names))
        if not VALID.match(name):
            report.append(f"INVALID   {where}: '{name}' is not a valid linker identifier; dtk name kept")
            continue
        idx = by_addr.get((addr, k), [])
        if not idx:
            report.append(f"NO-SYMBOL {where}: {name} ({', '.join(sorted(set(names[name])))}): dtk has no {k} here; not applied")
            continue
        if len(idx) > 1:
            report.append(f"AMBIGUOUS {where}: {name}: dtk has {len(idx)} {k} symbols here; not applied")
            continue
        if dtk[idx[0]][0] != name:
            rename[idx[0]] = name

    # the same name for different addresses -> keep dtk names for all of them
    users = defaultdict(list)
    for i, n in rename.items():
        users[n].append(i)
    for n, idx in sorted(users.items()):
        if len(idx) > 1 and all((dtk[i][2], n) in local for i in idx):
            continue  # statics of different files (reverb_hi.c's and reverb_std.c's HandleReverb): allowed, as dtk's own
        if len(idx) > 1:
            report.append(f"DUPLICATE {n}: given to {', '.join(f'0x{dtk[i][2]:08X}' for i in sorted(idx))}; dtk names kept")
            for i in idx:
                del rename[i]
    # a name already held by a dtk symbol that keeps its name (dtk's default names are unique, so dropping a rename
    # cannot cause a new clash)
    held = {s[0]: i for i, s in enumerate(dtk) if i not in rename}
    for i in sorted(rename):
        n = rename[i]
        if n in held:
            j = held[n]
            if (dtk[i][2], n) in local and "scope:local" in dtk[j][4]:
                continue  # a static of another file that dtk already names alike (step 68)
            report.append(f"COLLISION 0x{dtk[i][2]:08X}: {n} is already dtk's name for {dtk[j][1]}:0x{dtk[j][2]:08X}; dtk name {dtk[i][0]} kept")
            del rename[i]
    final = {i: rename.get(i, s[0]) for i, s in enumerate(dtk)}

    # a static name given to several symbols (ours scope:local) must be local in dtk's file too, or the all-asm link
    # defines it twice (step 68: the two HandleReverb)
    count = defaultdict(int)
    for n in final.values():
        count[n] += 1
    out = []
    for i, s in enumerate(dtk):
        line = s[4]
        if i in rename:
            assert line.startswith(s[0] + " = ")
            line = rename[i] + line[len(s[0]):]
            if count[rename[i]] > 1 and (s[2], rename[i]) in local and "scope:" not in line:
                line += " scope:local"
        out.append(line)
    # At one address, labels (size 0) go after the function / object that starts there. dtk writes the split objects'
    # ELF symbols in this order, and mwld (GC/1.3.2 and 2.6) hangs linking an object whose first symbol is a label in
    # front of a function (seen at 0x80137F98: __OSSystemCallVectorStart / SystemCallVector, when OSSync.c starts there).
    # dtk keeps this order when it rewrites the file.
    sec_order = []
    for s_ in dtk:
        if s_[1] not in sec_order:
            sec_order.append(s_[1])
    keyed = sorted(range(len(out)), key=lambda i: (sec_order.index(dtk[i][1]), dtk[i][2], "type:label" in dtk[i][4], i))
    out = [out[i] for i in keyed]
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_bytes(("\n".join(out) + "\n").encode("utf-8"))

    named = sum(1 for s in final.values() if not DEFAULT.match(s))
    kinds = defaultdict(int)
    for r in report:
        kinds[r.split()[0]] += 1
    head = [
        "decomp/config/GXCE01/symbols.txt: generated by tools/gen_dtk_symbols.py (method: its docstring).",
        f"dtk symbols: {len(dtk)} ({len(cuts)} of them from the cuts of symbols/data_splits.txt; otherwise dtk's count, "
        f"sections, sizes, scopes); renamed from our files: {len(rename)}",
        f"symbols with a non-default name now: {named} (dtk's own signature names included)",
        f"our names not applied: {len(report)} " + (str(dict(sorted(kinds.items()))) if kinds else ""),
        "",
    ]
    REPORT.parent.mkdir(parents=True, exist_ok=True)
    REPORT.write_bytes(("\r\n".join(head + sorted(report)) + "\r\n").encode("utf-8"))
    print("\n".join(head[:4]))
    for r in sorted(report):
        print("  " + r)
    return 0


if __name__ == "__main__":
    sys.exit(main())
