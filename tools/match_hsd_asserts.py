"""Name sysdolphin (HSD) functions in Custom Robo by matching __assert calls against the Melee decomp.

Each HSD assert carries (source file, line, expression). Line numbers drift between library
versions, but the expression strings do not, so a function is matched when its set of
(file, expression) pairs best fits exactly one Melee function with a consistent line offset.

Usage: python -I match_hsd_asserts.py <main_dol.c> <main.dol> <melee/src/sysdolphin> <out_symbols.txt>
"""
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

ASSERT_FN = "FUN_801aea4c"  # HSD __assert(file, line, expr)


class Dol:
    def __init__(self, path):
        self.d = Path(path).read_bytes()
        self.off = struct.unpack_from(">18I", self.d, 0)
        self.adr = struct.unpack_from(">18I", self.d, 0x48)
        self.sz = struct.unpack_from(">18I", self.d, 0x90)

    def cstr(self, addr):
        for o, a, s in zip(self.off, self.adr, self.sz):
            if s and a <= addr < a + s:
                f = o + addr - a
                return self.d[f:self.d.index(b"\0", f)].decode("ascii", "replace")
        return None


def norm(expr):
    return re.sub(r"\s+", "", expr)


def ours(c_path, dol):
    """address -> (name, [(file, line, expr)])"""
    funcs = {}
    cur = None
    call = re.compile(ASSERT_FN + r"\(s_(\w+?)_c_[0-9a-f]{8},(0x[0-9a-f]+|\d+),[&]?(?:s_\w*?_|DAT_|PTR_\w*?_)?([0-9a-f]{8})\)")
    head = re.compile(r"^// ([0-9a-f]{8})  (\S+)")
    for line in open(c_path, encoding="utf-8", errors="replace"):
        m = head.match(line)
        if m:
            cur = int(m.group(1), 16)
            funcs[cur] = (m.group(2), [])
            continue
        if cur is None or ASSERT_FN not in line:
            continue
        for m in call.finditer(line):
            expr = dol.cstr(int(m.group(3), 16))
            if expr:
                funcs[cur][1].append((m.group(1), int(m.group(2), 0), norm(expr)))
    return {a: v for a, v in funcs.items() if v[1]}


def melee(src_dir):
    """(file, expr) -> [(func_name, line)] plus func -> [(file, line, expr)]"""
    by_func = defaultdict(list)
    fdef = re.compile(r"^[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;]*$")
    amac = re.compile(r"HSD_ASSERT(?:MSG|REPORT)?\s*\(\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*(.+?)\s*\)\s*;")
    araw = re.compile(r"__assert\s*\(\s*\w+\s*,\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*\"(.*?)\"\s*\)")
    for path in sorted(Path(src_dir).rglob("*.c"), key=lambda p: str(p).replace("/", "\\")):   # Windows path order on every OS
        fname = path.stem
        depth, cur = 0, None
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            if depth == 0:
                m = fdef.match(line)
                if m and not line.startswith(("static const", "typedef", "struct", "#")):
                    cur = m.group(1)
            for m in list(amac.finditer(line)) + list(araw.finditer(line)):
                expr = m.group(2)
                if m.re is amac and "," in expr:  # HSD_ASSERTMSG(line, cond, msg): keep cond
                    expr = expr.split(",")[0]
                if cur:
                    by_func[cur].append((fname, int(m.group(1), 0), norm(expr.strip('"'))))
            depth += line.count("{") - line.count("}")
            if depth <= 0:
                depth = 0
    return by_func


def match(our_funcs, mel_funcs):
    index = defaultdict(set)
    for name, items in mel_funcs.items():
        for f, _, e in items:
            index[(f, e)].add(name)
    results = {}
    for addr, (oname, items) in our_funcs.items():
        if not oname.startswith("FUN_"):
            continue
        scores = defaultdict(int)
        for f, line, e in items:
            for cand in index.get((f, e), ()):
                # count hits whose line offset is small and consistent
                for mf, ml, me in mel_funcs[cand]:
                    if mf == f and me == e and abs(ml - line) <= 0x60:
                        scores[cand] += 1
                        break
        if not scores:
            continue
        ranked = sorted(scores.items(), key=lambda kv: -kv[1])
        best, hits = ranked[0]
        if len(ranked) > 1 and ranked[1][1] == hits:
            continue  # ambiguous
        mel_files = {mf for mf, _, _ in mel_funcs[best]}
        if hits * 2 >= len(items) and {f for f, _, _ in items} <= mel_files:
            results[addr] = (best, hits, len(items))
    # a Melee name must map to one address only
    seen = defaultdict(list)
    for addr, (name, _, _) in results.items():
        seen[name].append(addr)
    return {a: r for a, r in results.items() if len(seen[r[0]]) == 1}


def main():
    c_path, dol_path, src_dir, out = sys.argv[1:5]
    dol = Dol(dol_path)
    our_funcs = ours(c_path, dol)
    mel_funcs = melee(src_dir)
    res = match(our_funcs, mel_funcs)
    with open(out, "w", encoding="utf-8") as w:
        for addr in sorted(res):
            name, hits, total = res[addr]
            w.write(f"{name} = .text:0x{addr:08X}; // type:function scope:global "
                    f"// melee-assert-match {hits}/{total}\n")
    print(f"our funcs with asserts: {len(our_funcs)}, melee funcs with asserts: {len(mel_funcs)}, matched: {len(res)}")


if __name__ == "__main__":
    main()
