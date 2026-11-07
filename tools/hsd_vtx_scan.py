"""hsd_vtx_scan.py: summarise the vertex attribute formats (GX vertex descriptors) of standard HSD .dat files.

usage: python -I tools/hsd_vtx_scan.py FILE_OR_DIR...   -> counts of (attribute, index type, component count,
       component type, frac) over every PObj; GameCube fixed-point components are value / 2**frac.

PObj desc (HSDLib HSD_POBJ): +0x04 next, +0x08 vertex attribute list, +0x0C flags (u16), +0x0E display list size
(u16, x 0x20 bytes), +0x10 display list, +0x14 envelope list / joint.
Vertex attribute (0x18 bytes, list ended by attribute 0xFF): +0x00 attribute (GXAttr), +0x04 index type (GXAttrType:
1 direct, 2 index8, 3 index16), +0x08 component count (GXCompCnt), +0x0C component type (GXCompType: 0 u8, 1 s8,
2 u16, 3 s16, 4 f32 for positions / normals / UVs; colours use their own enum), +0x10 frac (u8), +0x12 stride (u16),
+0x14 data.
"""
import collections
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hsd_tex_scan import Dat, joints   # noqa: E402

ATTR = {0: 'PNMTXIDX', 9: 'POS', 10: 'NRM', 11: 'CLR0', 12: 'CLR1', 13: 'TEX0', 14: 'TEX1', 15: 'TEX2',
        25: 'NBT'}
ITYPE = {0: 'none', 1: 'direct', 2: 'index8', 3: 'index16'}
CTYPE = {0: 'u8', 1: 's8', 2: 'u16', 3: 's16', 4: 'f32'}


def pobjs(dat, top):
    for j in joints(dat, top, [], set()):
        flags = dat.u32(j + 4)
        if flags & 0x20 or flags & 0x4000:
            continue
        d = dat.u32(j + 0x10)
        while d:
            p = dat.u32(d + 0xC)
            while p:
                yield p
                p = dat.u32(p + 4)
            d = dat.u32(d + 4)


def scan(path, cnt):
    dat = Dat(open(path, 'rb').read())
    for name, roff in dat.roots:
        tops = []
        if name == 'scene_data':
            arr = dat.u32(roff)
            k = 0
            while arr and dat.u32(arr + 4 * k):
                tops.append(dat.u32(dat.u32(arr + 4 * k)))
                k += 1
        elif name.endswith('_joint'):
            tops.append(roff)
        for top in tops:
            for p in pobjs(dat, top):
                v = dat.u32(p + 8)
                while v:
                    a = dat.u32(v)
                    if a == 0xFF:
                        break
                    it, cc, ct = dat.u32(v + 4), dat.u32(v + 8), dat.u32(v + 0xC)
                    frac = dat.b[dat.d + v + 0x10]
                    key = (ATTR.get(a, a), ITYPE.get(it, it), cc,
                           CTYPE.get(ct, ct) if a in (9, 10, 13, 14, 15) else 'clr%d' % ct, frac)
                    cnt[key] += 1
                    v += 0x18


if __name__ == '__main__':
    for a in sys.argv[1:]:
        cnt = collections.Counter()
        files = [os.path.join(a, f) for f in sorted(os.listdir(a)) if f.endswith('.dat')] if os.path.isdir(a) else [a]
        for f in files:
            try:
                scan(f, cnt)
            except Exception as e:
                print('  %s: %s' % (f, e))
        print('== %s (%d files)' % (a, len(files)))
        for k, n in sorted(cnt.items(), key=lambda x: -x[1]):
            print('  %6d  %-8s %-7s cnt=%d %-5s frac=%d' % (n, *k))
