"""Verify every function name in symbols/symbols.txt against an independent source.

Checks, per name:
  - the name exists in a reference decomp's verified symbols (Melee, plus any further references given)
  - our function size (from dtk's analysis of main.dol) vs that reference's size for the name;
    the closest reference wins, and the report names it
Prints a table and a verdict per name: VERIFIED (exact size), CLOSE (within 10%),
MISMATCH, or UNSOURCED (no independent source has this name).

Usage: python -I verify_symbols.py <symbols.txt> <dtk symbols.txt> <reference symbols.txt> [more references ...]
"""
import os
import re
import sys

LINE = re.compile(r"^(\S+) = \S*:0x([0-9A-Fa-f]{8});.*?type:function(?:.*?size:0x([0-9A-Fa-f]+))?")


def load(path):
    out = {}
    for line in open(path, encoding="utf-8"):
        m = LINE.match(line)
        if m:
            out[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16) if m.group(3) else None)
    return out


def main():
    ours, dtk = load(sys.argv[1]), load(sys.argv[2])
    refs = [(p.replace(os.sep, '/').split('/config/')[0].split('/')[-1], load(p)) for p in sys.argv[3:]]
    size_at = {addr: size for addr, size in dtk.values()}
    counts = {}
    for name, (addr, _) in sorted(ours.items(), key=lambda kv: kv[1][0]):
        size = size_at.get(addr)
        cands = [(t, r[name][1]) for t, r in refs if name in r and r[name][1]]
        if not cands:
            verdict = "UNSOURCED" if not any(name in r for _, r in refs) else "NO-SIZE"
            msize, rtag = None, ""
        else:
            rtag, msize = min(cands, key=lambda c: abs(c[1] - size) if size else 0)
            if size is None:
                verdict = "NO-SIZE"
            elif size == msize:
                verdict = "VERIFIED"
            elif abs(size - msize) <= 0.1 * msize:
                verdict = "CLOSE"
            else:
                verdict = "MISMATCH"
        counts[verdict] = counts.get(verdict, 0) + 1
        print(f"{verdict:9} {addr:08X} {name:40} ours={size and hex(size)} ref={msize and hex(msize)} {rtag}")
    print(counts)


if __name__ == "__main__":
    main()
