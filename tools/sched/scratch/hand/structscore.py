"""structscore.py [OUTDIR]: three match scores of every compiled draft (default decomp/drafts) against the original:
exact (draft_batch.compare: words, relocation fields masked, aligned by difflib), noreg (the register fields
masked too: rD/rS, rA, rB, rC of every form, so a different register allocation does not count) and ops (opcode and
extended opcode only: the instruction sequence). Writes OUTDIR/structscore.txt (name, bytes, exact, noreg, ops) and
prints the byte buckets. A function far apart in exact but close in noreg needs register work (permuter-shaped); one
close in ops but not in noreg has other operands (offsets, constants: types / fields); far in ops is structure."""
import difflib
import os
import sys
G = r"D:\emulators\gameTesting"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
out = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else os.path.join(G, "decomp", "drafts"))
ver, flags = db.game_flags()
db.init(out, ver, flags)
elves = [db.orig_elf(os.path.join(out, "orig", f)) for f in sorted(os.listdir(os.path.join(out, "orig")))]


def masked(w, r):
    return [(x & ~db.MASK.get(r[i], 0xFFFF) & 0xFFFFFFFF) if i in r else x for i, x in enumerate(w)]


def noreg(x):
    op = x >> 26
    if op == 18 or op == 16:          # b / bc: keep (targets are relative)
        return x
    return x & ~0x03FFF800 if op in (19, 31, 59, 63, 4) else x & ~0x03FF0000   # X/A-forms: rD rA rB; D-forms: rD rA


def ops(x):
    op = x >> 26
    return (op << 16) | (((x >> 1) & 0x3FF) if op in (19, 31, 59, 63, 4) else 0)


def ratio(a, b):
    n = max(len(a), len(b))
    if not n:
        return 100
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return sum(bl.size for bl in sm.get_matching_blocks()) * 100 // n


rows = []
for f in sorted(os.listdir(os.path.join(out, "obj"))):
    name = f[:-2]
    try:
        mine = db.Elf(open(os.path.join(out, "obj", f), "rb").read()).func(name)
    except Exception:
        continue
    fo = None
    for e in elves:
        try:
            fo = e.func(name)
        except Exception:
            fo = None
        if fo is not None:
            break
    if fo is None or mine is None:
        continue
    p, _ = db.compare(fo, mine)
    a, b = masked(*fo), masked(*mine)
    rows.append((name, len(fo[0]) * 4, p, ratio([noreg(x) for x in a], [noreg(x) for x in b]),
                 ratio([ops(x) for x in a], [ops(x) for x in b])))
with open(os.path.join(out, "structscore.txt"), "w", encoding="utf-8") as fh:
    for r in sorted(rows, key=lambda r: -r[1]):
        fh.write("%-44s %6d  exact %3d  noreg %3d  ops %3d\n" % r)
tot = sum(r[1] for r in rows)
for col, label in ((2, "exact"), (3, "noreg"), (4, "ops")):
    print(label, " ".join(f"{lo}-{hi}%: {100 * sum(r[1] for r in rows if lo <= r[col] <= hi) / tot:4.1f}%"
                          for lo, hi in ((100, 100), (90, 99), (80, 89), (60, 79), (0, 59))))
