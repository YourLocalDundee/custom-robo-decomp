"""robo_look_scan.py: what a stock robo is made of, for matching a new robo's look to the game's own (the mecha
samurai research, docs/robo_look.md).

usage: python -I tools/robo_look_scan.py FILE.dat... [--png DIR]
  per file: triangles (from the display lists), PObjs, vertices drawn, distinct images (size, format, which UV set,
  TObj flags = light map kind + colour blend), MObj render modes and material colours (ambient / diffuse /
  specular RGBA, alpha, shininess), vertex attributes in use. With --png every distinct image is written as a PNG.

Layouts: tools/hsd_tex_scan.py (JObj / DObj / MObj / TObj / image) and tools/hsd_vtx_scan.py (PObj, vertex
attributes). Material (MObj +0x0C): ambient u32 RGBA, diffuse u32, specular u32, alpha f32, shininess f32.
Display list: primitives 0x80 quads, 0x90 triangles, 0x98 strip, 0xA0 fan (low 3 bits = vertex format), u16 vertex
count, then the vertices; 0 ends the list.
TObj flags (HSD): bits 0-3 coord kind, 0x10 LIGHTMAP_DIFFUSE, 0x20 SPECULAR, 0x40 AMBIENT, 0x80 EXT, 0x100 SHADOW;
bits 16-19 COLORMAP (1 alpha mask, 2 rgb mask, 3 blend, 4 modulate, 5 replace, 6 pass, 7 add, 8 sub).
MObj render mode (HSD RENDER_*): 0x1 CONSTANT, 0x2 VERTEX, 0x4 DIFFUSE, 0x8 SPECULAR, 0x10..0x80 TEX0..TEX3,
0x4000000 / 0x2000 XLU, 0x20000000 SHADOW, 0x40000000 ZMODE ALWAYS.
"""
import collections
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hsd_tex_scan import Dat, joints, FMT, SRC   # noqa: E402

CMAP = {0: 'none', 1: 'alpha_mask', 2: 'rgb_mask', 3: 'blend', 4: 'modulate', 5: 'replace', 6: 'pass', 7: 'add',
        8: 'sub'}
LMAP = {0x10: 'diffuse', 0x20: 'specular', 0x40: 'ambient', 0x80: 'ext', 0x100: 'shadow'}
CSIZE = {0: 1, 1: 1, 2: 2, 3: 2, 4: 4}


def attr_list(dat, a):
    out = []
    while True:
        at, it, cc, ct = struct.unpack_from('>4I', dat.b, dat.d + a)
        if at == 0xFF:
            return out
        out.append((at, it, cc, ct))
        a += 0x18


def vsize(attrs):
    n = 0
    for at, it, cc, ct in attrs:
        if it == 2:
            n += 1
        elif it == 3:
            n += 2
        elif it == 1:
            if at <= 8:              # matrix indices
                n += 1
            elif at in (11, 12):     # direct colour: GXCompType for colours 0 rgb565 2, 1 rgb8 3, 2 rgbx8 4, 3 rgba4 2, 4 rgba6 3, 5 rgba8 4
                n += {0: 2, 1: 3, 2: 4, 3: 2, 4: 3, 5: 4}[ct]
            else:
                comps = {9: (2, 3), 10: (3, 9), 13: (1, 2), 14: (1, 2)}.get(at, (1, 1))[cc] if at in (9, 10, 13, 14) else 1
                n += comps * CSIZE.get(ct, 4)
    return n


def tris_of(dat, dl, size, vs):
    p, end, t, v = dat.d + dl, dat.d + dl + size * 0x20, 0, 0
    b = dat.b
    while p < end:
        op = b[p]
        if op == 0:
            break
        n = struct.unpack_from('>H', b, p + 1)[0]
        kind = op & 0xF8
        if kind == 0x90:
            t += n // 3
        elif kind in (0x98, 0xA0):
            t += max(0, n - 2)
        elif kind == 0x80:
            t += n // 4 * 2
        v += n
        p += 3 + n * vs
    return t, v


def scan(path, pngdir):
    dat = Dat(open(path, 'rb').read())
    imgs, mats, attrs_seen = {}, collections.Counter(), collections.Counter()
    tris = verts = pobjs = 0
    for name, roff in dat.roots:
        if not name.endswith('_joint'):
            continue
        for j in joints(dat, roff, [], set()):
            if dat.u32(j + 4) & 0x4020:
                continue
            d = dat.u32(j + 0x10)
            while d:
                m = dat.u32(d + 8)
                if m:
                    rm = dat.u32(m + 4)
                    mat = dat.u32(m + 0xC)
                    if mat:
                        amb, dif, spe = struct.unpack_from('>3I', dat.b, dat.d + mat)
                        alpha, shin = dat.f(mat + 0xC, 2)
                        mats[(rm, amb, dif, spe, round(alpha, 3), round(shin, 2))] += 1
                    t = dat.u32(m + 8)
                    while t:
                        img = dat.u32(t + 0x4C)
                        fl = dat.u32(t + 0x40)
                        if img and img not in imgs:
                            w, h = struct.unpack_from('>2H', dat.b, dat.d + img + 4)
                            imgs[img] = (w, h, dat.u32(img + 8), SRC.get(dat.u32(t + 0xC)), fl, dat.u32(img))
                        t = dat.u32(t + 4)
                p = dat.u32(d + 0xC)
                while p:
                    al = attr_list(dat, dat.u32(p + 8))
                    for a in al:
                        attrs_seen[(a[0], a[1])] += 1
                    size = struct.unpack_from('>H', dat.b, dat.d + p + 0xE)[0]
                    tt, vv = tris_of(dat, dat.u32(p + 0x10), size, vsize(al))
                    tris += tt
                    verts += vv
                    pobjs += 1
                    p = dat.u32(p + 4)
                d = dat.u32(d + 4)
    base = os.path.basename(path)
    print(f'== {base}: {tris} triangles, {pobjs} PObjs, {verts} vertices drawn')
    for k, (w, h, fm, src, fl, data) in imgs.items():
        lm = '+'.join(v for b_, v in LMAP.items() if fl & b_) or 'colour'
        print(f'   image {w}x{h} {FMT.get(fm, fm)} on {src}: {lm}, colormap {CMAP.get((fl >> 16) & 0xF)} (flags {fl:08X})')
        if pngdir:
            write_png(dat, w, h, fm, data, os.path.join(pngdir, f'{base[:-4]}_{w}x{h}_{lm}.png'))
    for (rm, amb, dif, spe, al, sh), n in mats.most_common():
        print(f'   material x{n}: render {rm:08X} ambient {amb:08X} diffuse {dif:08X} specular {spe:08X} alpha {al} shininess {sh}')
    names = {0: 'PNMTXIDX', 9: 'POS', 10: 'NRM', 11: 'CLR0', 12: 'CLR1', 13: 'TEX0', 14: 'TEX1', 25: 'NBT'}
    print('   attributes: ' + ', '.join(f'{names.get(a, a)}({ {1: "direct", 2: "idx8", 3: "idx16"}.get(i, i)}) x{n}'
                                      for (a, i), n in sorted(attrs_seen.items())))


def write_png(dat, w, h, fm, data, out):
    import tpl
    sz = {14: w * h // 2, 4: w * h * 2, 5: w * h * 2, 6: w * h * 4, 0: w * h // 2, 1: w * h, 3: w * h * 2}.get(fm)
    if sz is None or fm not in (14, 4, 5):
        return
    hdr = struct.pack('>III', 0x0020AF30, 1, 12) + struct.pack('>II', 20, 0)
    ih = struct.pack('>HHII', h, w, fm, 64) + b'\0' * 28
    buf = hdr + ih
    buf += b'\0' * (64 - len(buf)) + dat.b[dat.d + data:dat.d + data + sz]
    tpl.decode(buf).save(out)


if __name__ == '__main__':
    a = sys.argv[1:]
    pngdir = None
    if '--png' in a:
        i = a.index('--png')
        pngdir = a[i + 1]
        os.makedirs(pngdir, exist_ok=True)
        del a[i:i + 2]
    for f in a:
        scan(f, pngdir)
