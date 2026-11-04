"""quick hand-draft check: python hd.py OUTDIR FUNC DRAFT.c [-v]
OUTDIR = a draft_batch.py --out folder that has drafted FUNC (ctx.h, orig/ copies, asm/FUNC.s); compiles DRAFT.c the way
draft_batch.py's build() does (game compiler, flags, the context) and compares with the original: percent + a diff."""
import difflib
import os
import re
import struct
import subprocess
import sys
import tempfile

G = r"D:\emulators\gameTesting"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db  # noqa: E402

out, name, src = os.path.abspath(sys.argv[1]), sys.argv[2], sys.argv[3]
ver, flags = db.game_flags()
db.init(out, ver, flags)
# the original object: the draft_batch job list is not stored; find the orig copy that defines the function
orig = None
for f in sorted(os.listdir(os.path.join(out, "orig"))):
    e = db.orig_elf(os.path.join(out, "orig", f))
    try:
        fn = e.func(name)
    except Exception:
        fn = None
    if fn is not None:
        orig = fn
        break
if orig is None:
    sys.exit("function not in any orig/ object")
body = open(src, encoding="utf-8").read().replace("\r\n", "\n")
pre = ""
if body.startswith(db.NO_RO):
    body, pre = body[len(db.NO_RO):], "#define CR_NO_RO\n"
text = ('// hand draft check of %s\n%s#define %s %s__proto\n#include "ctx.h"\n#undef %s\n\n%s'
        % (name, pre, name, name, name, body))
tmp = tempfile.mkdtemp()
c = os.path.join(tmp, name + ".c")
o = os.path.join(tmp, name + ".o")
open(c, "w", encoding="utf-8", newline="\n").write(text)
cmd = db.mwcc_cmd(ver) + ["-c"] + flags + db.CTX_DEFS + ["-DCR_DRAFT_CC", "-i", out.replace(os.sep, "/")] + \
    sum([["-i", i] for i in db.CTX_INC], []) + ["-o", o, c.replace("\\", "/")]
rc, so, se = db.run(cmd, 120, cwd=db.D)
if rc or not os.path.exists(o):
    print("compile failed:\n" + (so + se)[-2500:])
    sys.exit(1)
mine = db.Elf(open(o, "rb").read()).func(name)
p, eq = db.compare(orig, mine)
print(f"{name}: {p}%  ({eq} equal words)")
if "-r" in sys.argv:
    # our object with its relocation symbols, to compare with the original's .s (hd.py masks relocated fields)
    _od = os.path.join(db.D, "build", "binutils", "powerpc-eabi-objdump.exe")
    print(subprocess.run([_od, "-dr", o], capture_output=True, text=True).stdout.split("<%s>:" % name, 1)[-1])
if "-v" in sys.argv or p < 100:
    OD = os.path.join(db.D, "build", "binutils", "powerpc-eabi-objdump.exe")

    def words(fo):
        ws, rel = fo
        return b"".join(struct.pack(">I", w & ~(db.MASK.get(rel[k], 0xFFFF) if k in rel else 0)) for k, w in enumerate(ws))
    A, B = words(mine), words(orig)

    def objd(b):
        pth = os.path.join(tmp, "x.bin")
        open(pth, "wb").write(bytes(b))
        r = subprocess.run([OD, "-D", "-b", "binary", "-m", "powerpc", "-EB", pth], capture_output=True, text=True).stdout
        return [re.sub(r"\s+", " ", ln.split("\t", 2)[-1]).strip() for ln in r.splitlines() if re.match(r"\s+[0-9a-f]+:\t", ln)]
    a, b = objd(A), objd(B)
    if "-w" in sys.argv:
        lo, hi = int(sys.argv[sys.argv.index("-w") + 1]), int(sys.argv[sys.argv.index("-w") + 2])
        for k in range(lo, hi):
            print(f"{k:3d}  {a[k] if k < len(a) else '':40s} | {b[k] if k < len(b) else ''}")
        sys.exit(0)
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if op == "equal":
            continue
        print(f"--- {op} mine[{i1}:{i2}] orig[{j1}:{j2}]")
        for k in range(max(i2 - i1, j2 - j1)):
            x = a[i1 + k] if i1 + k < i2 else ""
            y = b[j1 + k] if j1 + k < j2 else ""
            print(f"   {x:42s} | {y}")
