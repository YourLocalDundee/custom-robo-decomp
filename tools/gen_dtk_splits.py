"""Generate decomp/config/GXCE01/splits.txt: object (.c file) boundaries in main.dol, only where they are proven.

Method (library code: Dolphin SDK, MSL / Runtime, MetroTRK, HSD sysdolphin):
  Every pinned reference decomp with a dtk config (tools/melee, tools/refs/*/config/*/{splits,symbols}.txt) gives, per
  object, the functions it contains in its own address order. For an object O of a reference R let F = R's functions
  in O. O's .text range in OUR main.dol is proven when:
    1. R's first and last function of O both have a real name in R (not fn_/lbl_), and that exact name is on exactly
       one function in our decomp/config/GXCE01/symbols.txt (our names are the cross-checked ones, gen_dtk_symbols.py);
    2. first <= last in our address order, in the same section;
    3. every NAMED function of ours from first to last is one of R's functions of O (unnamed fn_ ones may be in
       between: they are enclosed by O's own functions, so they belong to O);
    4. the named function right before first and right after last (if any, same section) is NOT one of O's
       functions in R (else the object would extend further: the reference's first / last is not ours);
    5. our function count from first to last is within max(2, 25%) of R's count for O (catches a wrong anchor).
  The range is [first's address, start of the next function / symbol after last) - alignment padding after an
  object's last function belongs to that object, as the linker puts it there.
  Several references can prove the same object; their ranges must agree, or the object is dropped (reported).
  Proven ranges that overlap each other are all dropped (reported). The object's path is Melee's path for a file of
  the same name (lower-case match), else the first reference's file name in Melee's folder layout (dolphin/<lib>/,
  MSL/, Runtime/, MetroTRK/, sysdolphin/baselib/).
Game code: no reference exists. tools/match_game_names.py's G3 module notes (assert .c names) say which file a
  function is in, but not where a file starts or ends; a game object is split only where both ends are proven: its
  first and last function have a G3 note for it, and the functions right outside have G3 notes for OTHER files.
  (On the current data this proves none; everything else stays in dtk's automatic objects, listed in the report.)
Data sections (.data/.rodata/.bss/.sdata/.sdata2/.sbss, .ctors/.dtors) are not split yet: dtk puts them in its
  automatic per-gap objects, which keeps the link byte-exact. Splitting them needs the per-object data order
  (Milestone 1, next step).

Output is byte-identical on every run (LF, as dtk writes it; the batch checks dtk leaves it unchanged).
Report: decomp/reports/splits_report.txt (every object with its proof, every rejected candidate, unproven ranges).
Usage: python -I tools/gen_dtk_splits.py
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

G = Path(__file__).resolve().parent.parent
OURS = G / "decomp/config/GXCE01/symbols.txt"
BASE = G / "tools/dtk/proj/splits.txt"  # the Sections: header (dtk's)
OUT = G / "decomp/config/GXCE01/splits.txt"
REPORT = G / "decomp/reports/splits_report.txt"
GAME_REAL = G / "symbols/game_real.txt"
# reference configs, in path-preference order after Melee
REFS = [
    "melee/config/GALE01",
    "refs/tww/config/GZLE01", "refs/pikmin2/config/GPVE01", "refs/mkdd/config/MarioClub_us",
    "refs/sms/config/GMSE01", "refs/tp/config/GZ2E01", "refs/kar_wowjinxy/config/GKYE01",
    "refs/xd/config/GXXE01", "refs/ac-decomp/config/GAFE01_00", "refs/marioparty4/config/GMPE01_00",
    "refs/pikmin/config/GPIE01_01", "refs/bfbb/config/GQPE78", "refs/ttyd/config/G8MJ01",
]
LIB_DIRS = ("dolphin", "sysdolphin", "MSL", "Runtime", "MetroTRK", "TRK_MINNOW_DOLPHIN", "Dolphin", "PowerPC_EABI_Support",
            "TRK", "libc", "musyx", "SDK", "dolsdk", "amcstubs", "odenotstub", "OdemuExi2", "debugger", "Runtime.PPCEABI.H",
            "MSL_C", "MSL_C.PPCEABI.bare.H", "MetroTRK", "os", "dvd", "gx", "vi", "pad", "si", "exi", "card", "ax",
            "ai", "ar", "dsp", "mtx", "gba", "thp", "hio", "db", "base", "mcc", "axfx", "OdemuExi", "libs")
SYM = re.compile(r"^\s*([^\s=]+)\s*=\s*(\.[\w.]+):0x([0-9A-Fa-f]+);\s*//(.*)$")
DEFAULT = re.compile(r"^(fn|lbl|func|FUN|zz|jumptable|gap|switch)_[0-9A-Fa-f_]+|^@|^\.L|^\$")
CODE = (".init", ".text")
ADDRNAME = re.compile(r"_[0-9A-Fa-f]{8}$|^hsd_[0-9A-Fa-f]{8}")  # a reference's address-based placeholder names


def family(path: str) -> str:
    p = path.lower()
    if p.startswith("sysdolphin/"):
        return "hsd"
    if "trk" in p.split("/")[0] or "metrotrk" in p or "trk_minnow" in p:
        return "trk"
    if "msl" in p or "runtime" in p or "powerpc_eabi_support" in p or p.startswith("libc/"):
        return "msl"
    return "sdk"


def objkey(path: str):
    # the same file in different references: same library family and file name (.c / .cpp / .s alike)
    return (family(path), Path(path).stem.lower())


SDK_LIBS = {"ai", "amcstubs", "ar", "ax", "axfx", "base", "card", "db", "dsp", "dvd", "exi", "gx", "hio", "mcc", "mtx",
            "odenotstub", "odemuexi2", "os", "pad", "si", "thp", "vi", "gba"}


def canonical(path: str) -> str:
    # a file Melee does not have: Melee's folder layout (dolphin/<lib>/, MSL/, Runtime/, MetroTRK/,
    # sysdolphin/baselib/), the file name as the reference writes it
    parts = path.split("/")
    f, fam = parts[-1], family(path)
    if fam == "hsd":
        return "sysdolphin/baselib/" + f
    if fam == "trk":
        return "MetroTRK/" + f
    if fam == "msl":
        return ("Runtime/" if "runtime" in path.lower() else "MSL/") + f
    lib = next((x for x in reversed(parts[:-1]) if x.lower() in SDK_LIBS), None)
    return f"dolphin/{lib}/{f}" if lib else f"dolphin/{f}"


def read_syms(path: Path):
    out = []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = SYM.match(line)
        if not m:
            continue
        c = m.group(4)
        sz = re.search(r"size:0x([0-9A-Fa-f]+)", c)
        out.append((m.group(1), m.group(2), int(m.group(3), 16), int(sz.group(1), 16) if sz else 0,
                    "type:function" in c))
    return out


def read_splits(path: Path):
    objs = []  # (name, [(section, start, end)])
    cur = None
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        if not line.strip() or line.startswith("Sections:") or line.lstrip().startswith("//"):
            if line.startswith("Sections:"):
                cur = None
            continue
        if not line[0].isspace():
            cur = (line.split(":")[0].strip(), [])
            objs.append(cur)
            continue
        if cur is None:
            continue
        m = re.match(r"\s+(\S+)\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)", line)
        if m:
            cur[1].append((m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return objs


def main() -> int:
    ours = read_syms(OURS)
    funcs = sorted([s for s in ours if s[4] and s[1] in CODE], key=lambda s: s[2])
    pos = {}  # name -> index, only for names on exactly one function
    seen = defaultdict(int)
    for s in funcs:
        seen[s[0]] += 1
    for i, s in enumerate(funcs):
        if seen[s[0]] == 1 and not DEFAULT.match(s[0]):
            pos[s[0]] = i
    # next symbol start after an address, per section (any symbol, for the end of an object's range)
    starts = defaultdict(list)
    for s in ours:
        starts[s[1]].append(s[2])
    for k in starts:
        starts[k] = sorted(set(starts[k]))

    def next_start(sec, addr):
        import bisect
        lst = starts[sec]
        j = bisect.bisect_right(lst, addr)
        return lst[j] if j < len(lst) else None

    melee_paths = {}
    cands = defaultdict(list)  # object path key (lower basename) -> [(ref, path, sec, start, end, nfun_ours, nref)]
    rejected = []
    for ref in REFS:
        d = G / "tools" / ref
        if not (d / "splits.txt").exists() or not (d / "symbols.txt").exists():
            rejected.append(f"reference {ref}: no config (not fetched?)")
            continue
        rsyms = read_syms(d / "symbols.txt")
        rf = sorted([s for s in rsyms if s[4]], key=lambda s: s[2])
        robjs = [(name, secs) for name, secs in read_splits(d / "splits.txt")]
        # which object each reference function name is in; a name in several objects tells nothing
        where = defaultdict(set)
        members = {}
        for name, secs in robjs:
            for sec, a, b in secs:
                if sec in CODE:
                    F = [s for s in rf if a <= s[2] < b and s[1] == sec]
                    members[(name, sec)] = F
                    for s in F:
                        where[s[0]].add(name)
        for name, secs in robjs:
            top = name.split("/")[0]
            if top not in LIB_DIRS:
                continue
            if ref.startswith("melee/"):
                melee_paths[objkey(name)] = name
            for sec, a, b in secs:
                if sec not in CODE:
                    continue
                F = members[(name, sec)]
                if not F:
                    continue
                tag = f"{ref.split('/')[-1]}:{name} {sec}"
                names = {s[0] for s in F if not DEFAULT.match(s[0]) and not ADDRNAME.search(s[0]) and len(where[s[0]]) == 1}
                hit = sorted(pos[n] for n in names if n in pos and funcs[pos[n]][1] == sec)
                if not hit:
                    rejected.append(f"{tag}: none of its functions is (uniquely) named in ours")
                    continue
                i, j = hit[0], hit[-1]
                bad = [funcs[k][0] for k in range(i, j + 1) if not DEFAULT.match(funcs[k][0]) and funcs[k][0] not in names]
                if bad:
                    rejected.append(f"{tag}: our range {funcs[i][0]} .. {funcs[j][0]} contains other functions: {', '.join(bad[:4])}")
                    continue

                def edge(k, ref_end):
                    # proven when the neighbour is the section edge, or a named function the reference puts in
                    # another object, or unnamed while our end function is the reference's own first / last
                    if k < 0 or k >= len(funcs) or funcs[k][1] != sec:
                        return "section edge"
                    nb = funcs[k][0]
                    if DEFAULT.match(nb):
                        return f"reference's own {'first' if ref_end is F[0] else 'last'} function" if funcs[i if ref_end is F[0] else j][0] == ref_end[0] else None
                    if where.get(nb) and name not in where[nb]:
                        return f"{nb} is in {sorted(where[nb])[0]}"
                    return None
                e1, e2 = edge(i - 1, F[0]), edge(j + 1, F[-1])
                if not e1 or not e2:
                    rejected.append(f"{tag}: {'start' if not e1 else 'end'} not proven ({funcs[i][0]} .. {funcs[j][0]}; "
                                    f"neighbour {funcs[i - 1][0] if not e1 else funcs[j + 1][0]})")
                    continue
                n = j - i + 1
                if abs(n - len(F)) > max(2, len(F) // 4):
                    rejected.append(f"{tag}: {n} functions in ours vs {len(F)} in the reference")
                    continue
                start = funcs[i][2]
                end = next_start(funcs[j][1], funcs[j][2]) or (funcs[j][2] + funcs[j][3])
                cands[objkey(name)].append((ref.split('/')[-1], name, sec, start, end, n, len(F),
                                                       funcs[i][0], funcs[j][0], e1, e2))

    # agree per object (per section), then drop overlaps
    chosen = {}  # key -> (path, {sec: (start, end)}, proofs)
    for key, lst in sorted(cands.items()):
        path = melee_paths.get(key) or canonical(lst[0][1])
        per_sec = defaultdict(set)
        for c in lst:
            per_sec[c[2]].add((c[3], c[4]))
        if any(len(v) > 1 for v in per_sec.values()):
            rejected.append(f"{path}: references disagree on the range: " +
                            "; ".join(f"{c[0]} {c[2]} 0x{c[3]:08X}-0x{c[4]:08X}" for c in lst))
            continue
        chosen[key] = (path, {s: next(iter(v)) for s, v in per_sec.items()}, lst)
    same = defaultdict(list)
    for k, (_, secs, lst) in chosen.items():
        same[tuple(sorted(secs.items()))].append(k)
    for r, ks in same.items():
        if len(ks) > 1:
            ks.sort(key=lambda k: (-len({c[0] for c in chosen[k][2]}), k not in melee_paths, chosen[k][0]))
            rejected.append(f"same range under other names, kept {chosen[ks[0]][0]}: " + ", ".join(chosen[k][0] for k in ks[1:]))
            for k in ks[1:]:
                chosen.pop(k)
    ranges = sorted(((s, a, b, k) for k, (_, secs, _) in chosen.items() for s, (a, b) in secs.items()), key=lambda r: r[1])
    drop = set()
    def weight(k):
        return (len({c[0] for c in chosen[k][2]}), k in melee_paths)
    for x, y in zip(ranges, ranges[1:]):
        if x[0] == y[0] and y[1] < x[2]:
            if (x[1], x[2]) == (y[1], y[2]) and weight(x[3]) != weight(y[3]):
                lo, hi = sorted((x[3], y[3]), key=weight)
                drop.add(lo)
                rejected.append(f"same {x[0]} range 0x{x[1]:08X}-0x{x[2]:08X} under another name, kept {chosen[hi][0]} "
                                f"(more references): {chosen[lo][0]}")
                continue
            drop.update((x[3], y[3]))
            rejected.append(f"overlap: {chosen[x[3]][0]} 0x{x[1]:08X}-0x{x[2]:08X} and {chosen[y[3]][0]} 0x{y[1]:08X}-0x{y[2]:08X}")
    for k in drop:
        chosen.pop(k, None)
    paths = defaultdict(list)
    for k, v in chosen.items():
        paths[v[0]].append(k)
    for p, ks in paths.items():
        if len(ks) > 1:
            rejected.append(f"path clash {p}: {ks}")
            for k in ks:
                chosen.pop(k)

    # game code: G3 notes (file per function), split only when both ends are proven by neighbours
    g3 = {}
    if GAME_REAL.exists():
        for line in GAME_REAL.read_text(encoding="utf-8").splitlines():
            m = re.match(r"// G3 0x([0-9A-Fa-f]{8}) (\S+\.c) ", line)
            if m:
                g3[int(m.group(1), 16)] = m.group(2)
    idx_of = {s[2]: i for i, s in enumerate(funcs)}
    game_objs = []
    by_file = defaultdict(list)
    for a, f in g3.items():
        if a in idx_of:
            by_file[f].append(idx_of[a])
    lib_ranges = [(s, a, b) for (s, a, b, k) in ranges if k in chosen]
    for f, idx in sorted(by_file.items()):
        i, j = min(idx), max(idx)
        if any(a <= funcs[i][2] < b for _, a, b in lib_ranges):
            continue  # a library file (axdriver.c, dvd.c, ...): handled above
        others = [k for k in range(i, j + 1) if funcs[k][2] in g3 and g3[funcs[k][2]] != f]
        before = funcs[i - 1][2] if i > 0 else None
        after = funcs[j + 1][2] if j + 1 < len(funcs) else None
        if not others and before in g3 and g3[before] != f and after in g3 and g3[after] != f:
            game_objs.append((f, funcs[i][1], funcs[i][2], funcs[j + 1][2]))
        else:
            why = "another file's note inside" if others else "ends not next to another file's function"
            rejected.append(f"game {f}: G3 functions 0x{funcs[i][2]:08X}..0x{funcs[j][2]:08X} ({len(idx)}), {why}")

    # write: objects in address order of their first range
    objs = [(v[0], v[1]) for v in chosen.values()] + [(f"game/{f}", {s: (a, b)}) for f, s, a, b in game_objs]
    objs.sort(key=lambda o: min(a for a, _ in o[1].values()))
    order = {".init": 0, ".text": 1}
    lines = BASE.read_text(encoding="utf-8").replace("\r\n", "\n").rstrip("\n").split("\n")
    for path, secs in objs:
        lines.append("")
        lines.append(f"{path}:")
        for s in sorted(secs, key=lambda s: order[s]):
            a, b = secs[s]
            lines.append(f"\t{s:<11} start:0x{a:08X} end:0x{b:08X}")
    OUT.write_bytes(("\n".join(lines) + "\n").encode("utf-8"))

    # unproven code ranges: the parts of .init / .text not inside an object
    covered = sorted((a, b) for _, secs in objs for (a, b) in secs.values())
    gaps = []
    for sec in CODE:
        fs = [s for s in funcs if s[1] == sec]
        if not fs:
            continue
        lo, hi = fs[0][2], fs[-1][2] + fs[-1][3]
        cur = lo
        for a, b in covered:
            if b <= lo or a >= hi:
                continue
            if a > cur:
                gaps.append((sec, cur, a))
            cur = max(cur, b)
        if cur < hi:
            gaps.append((sec, cur, hi))

    def nf(a, b):
        return sum(1 for s in funcs if a <= s[2] < b)

    lib = [o for o in objs if not o[0].startswith("game/")]
    head = [
        "decomp/config/GXCE01/splits.txt: generated by tools/gen_dtk_splits.py (method: its docstring).",
        f"objects split: {len(objs)} ({len(lib)} library, {len(objs) - len(lib)} game); code only, data in dtk's automatic objects",
        f"functions inside split objects: {sum(nf(a, b) for _, secs in objs for (a, b) in secs.values())} of {len(funcs)}",
        f"unproven code ranges (dtk automatic objects): {len(gaps)}, {sum(nf(a, b) for _, a, b in gaps)} functions",
        "",
        "== objects (path, range, functions, proving references: ref first..last ours/ref count)",
    ]
    for path, secs in objs:
        for s, (a, b) in secs.items():
            proofs = [c for v in chosen.values() if v[0] == path for c in v[2] if c[2] == s]
            pr = "; ".join(f"{c[0]} {c[7]}..{c[8]} {c[5]}/{c[6]} [{c[9]} | {c[10]}]" for c in proofs) or "G3 neighbours"
            head.append(f"{path:<48} {s:<6} 0x{a:08X}-0x{b:08X} {nf(a, b):>3} fn  {pr}")
    head += ["", "== unproven code ranges (one dtk automatic object each)"]
    for s, a, b in gaps:
        head.append(f"{s:<6} 0x{a:08X}-0x{b:08X} {nf(a, b):>4} functions")
    head += ["", f"== rejected candidates ({len(rejected)})"] + rejected
    REPORT.parent.mkdir(parents=True, exist_ok=True)
    REPORT.write_bytes(("\r\n".join(head) + "\r\n").encode("utf-8"))
    print("\n".join(head[:4]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
