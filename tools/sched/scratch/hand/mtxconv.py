"""mtxconv.py FUNC SRC.c OUTSPEC.py: m2c's stack struct (f32 triples, f32[4] + pad = Mtx) -> Vec / Mtx locals;
writes an hdvar spec with the four declaration orders (Vec / Mtx groups forward or reversed)"""
import re, sys
func, src, out = sys.argv[1:4]
s = open(src).read()
m = re.search(r"    struct \{\n(.*?)\n    \} sp;\n", s, re.S)
fields = []
for l in m.group(1).split("\n"):
    t = re.match(r"\s*(f32|u8|s32) (sp|pad_)([0-9A-F]+)(\[(\d+)\])?;", l)
    fields.append((t.group(1), t.group(2), int(t.group(3), 16), t.group(5)))
vecs, mtxs = [], []
i = 0
while i < len(fields):
    ty, kind, off, n = fields[i]
    if kind == "sp" and n == "4":
        mtxs.append(off); i += 1
        if i < len(fields) and fields[i][1] == "pad_": i += 1
        continue
    if kind == "sp" and n is None:
        vecs.append(off); i += 3; continue
    i += 1
body = s[:m.start()] + "{D}" + s[m.end():]
for o in mtxs:
    body = body.replace(f"(f32 (*)[4]) &sp.sp{o:X}[0]", f"m{o:X}")
for o in vecs:
    body = body.replace(f"(Point3d *) &sp.sp{o:X}", f"&v{o:X}").replace(f"(Vec *) &sp.sp{o:X}", f"&v{o:X}").replace(f"&sp.sp{o:X}", f"&v{o:X}")
    for k, c in enumerate("xyz"):
        body = re.sub(rf"sp\.sp{o + 4 * k:X}\b", f"v{o:X}.{c}", body)
left = re.findall(r"sp\.sp\w+", body)
if left: print("left:", sorted(set(left)))
V = {}
for vn, vo in (("vf", vecs), ("vr", vecs[::-1])):
    for mn, mo in (("mf", mtxs), ("mr", mtxs[::-1])):
        V[vn + mn] = {"D": "".join(f"    Vec v{o:X};\n" for o in vo) + "".join(f"    Mtx m{o:X};\n" for o in mo)}
        V[mn + vn] = {"D": "".join(f"    Mtx m{o:X};\n" for o in mo) + "".join(f"    Vec v{o:X};\n" for o in vo)}
open(out + ".c", "w").write(body)
open(out, "w").write(f"BASE=open(r{(out + '.c')!r}).read()\nV={V!r}\n")
print(func, "vecs", [hex(o) for o in vecs], "mtxs", [hex(o) for o in mtxs])
