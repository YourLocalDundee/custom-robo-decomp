"""scores.py OUTDIR: the match % of every compiled draft in OUTDIR/obj against OUTDIR/orig (draft_batch compare)"""
import os, sys
G = r"D:\emulators\gameTesting"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
out = os.path.abspath(sys.argv[1])
ver, flags = db.game_flags()
db.init(out, ver, flags)
orig = {}
for f in sorted(os.listdir(os.path.join(out, "orig"))):
    e = db.orig_elf(os.path.join(out, "orig", f))
    for name in getattr(e, "names", lambda: [])() if hasattr(e, "names") else []:
        orig[name] = e
    orig.setdefault("__elves__", []).append(e)
res = []
for f in sorted(os.listdir(os.path.join(out, "obj"))):
    name = f[:-2]
    try:
        mine = db.Elf(open(os.path.join(out, "obj", f), "rb").read()).func(name)
    except Exception:
        continue
    fo = None
    for e in orig["__elves__"]:
        try:
            fo = e.func(name)
        except Exception:
            fo = None
        if fo is not None:
            break
    if fo is None or mine is None:
        continue
    p, eq = db.compare(fo, mine)
    res.append((p, len(fo[0]) * 4, name))
for p, n, name in sorted(res, reverse=True):
    print(f"{p:3}  {n:6}  {name}")
