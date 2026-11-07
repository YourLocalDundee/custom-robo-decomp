"""hsd_tex_scan.py: list every texture (TObj) of standard HSD .dat files with the fields an exporter needs.

usage: python -I tools/hsd_tex_scan.py FILE_OR_DIR... [--odd]
  prints, per TObj: file, root, joint number, image size / GX format, texture coordinate source, wrap / repeat,
  and the TObj's own scale / rotation / translation (the texture matrix). With --odd only TObjs whose matrix is not
  identity, whose repeat is not 1 or whose wrap is not CLAMP are printed.

Layouts (HSDLib HSDRaw/Common, checked against the game's loaders, docs/formats/model_entry.md):
  .dat header 0x20: file size, data size, reloc count, root count, ref count; data at 0x20, then reloc table,
  root table (offset, name offset), names.
  JObj desc: +0x04 flags, +0x08 child, +0x0C next, +0x10 DObj, +0x14 rot xyz, +0x20 scale xyz, +0x2C trans xyz.
  DObj desc: +0x04 next, +0x08 MObj, +0x0C PObj.   MObj desc: +0x04 render mode, +0x08 TObj.
  TObj desc: +0x04 next, +0x08 tex map id, +0x0C coord source (GXTexGenSrc), +0x10 rot xyz, +0x1C scale xyz,
             +0x28 trans xyz, +0x34 wrap S, +0x38 wrap T, +0x3C repeat S (u8), +0x3D repeat T (u8), +0x40 flags,
             +0x44 blending, +0x4C image.   Image: +0x00 data, +0x04 width u16, +0x06 height u16, +0x08 format.
  Scene (root scene_data): +0x00 null-terminated array of model sets, each +0x00 the root JObj desc.
"""
import os
import struct
import sys

FMT = {0: 'I4', 1: 'I8', 2: 'IA4', 3: 'IA8', 4: 'RGB565', 5: 'RGB5A3', 6: 'RGBA8', 8: 'C4', 9: 'C8', 10: 'C14X2',
       14: 'CMPR'}
WRAP = {0: 'CLAMP', 1: 'REPEAT', 2: 'MIRROR'}
SRC = {4: 'TEX0', 5: 'TEX1', 6: 'TEX2', 7: 'TEX3', 1: 'NRM', 0: 'POS', 2: 'BINRM', 3: 'TANGENT'}


class Dat:
    def __init__(self, b):
        self.b = b
        fs, ds, rc, nroot, nref = struct.unpack_from('>5I', b, 0)
        self.d = 0x20
        rt = 0x20 + ds + rc * 4
        names = rt + (nroot + nref) * 8
        self.roots = []
        for i in range(nroot):
            off, no = struct.unpack_from('>2I', b, rt + i * 8)
            e = b.index(b'\0', names + no)
            self.roots.append((b[names + no:e].decode('ascii', 'replace'), off))

    def u32(self, o):
        return struct.unpack_from('>I', self.b, self.d + o)[0]

    def f(self, o, n):
        return struct.unpack_from('>%df' % n, self.b, self.d + o)


def joints(dat, off, out, seen):
    while off and off not in seen:
        seen.add(off)
        out.append(off)
        flags = dat.u32(off + 4)
        child = dat.u32(off + 8)
        if child:
            joints(dat, child, out, seen)
        off = dat.u32(off + 0xC)
    return out


def scan(path, odd):
    b = open(path, 'rb').read()
    try:
        dat = Dat(b)
    except Exception as e:
        print('%s: not a standard .dat (%s)' % (path, e))
        return
    for name, roff in dat.roots:
        tops = []
        if name == 'scene_data':
            arr = dat.u32(roff)
            k = 0
            while arr and dat.u32(arr + 4 * k):
                tops.append(('model%d' % k, dat.u32(dat.u32(arr + 4 * k))))
                k += 1
        elif name.endswith('_joint'):
            tops.append((name, roff))
        else:
            continue
        for tname, top in tops:
            js = joints(dat, top, [], set())
            for jn, j in enumerate(js):
                flags = dat.u32(j + 4)
                if flags & 0x20 or flags & 0x4000:   # PTCL (0x20) / SPLINE (0x4000): no DObj at +0x10
                    continue
                sc = dat.f(j + 0x20, 3)
                dobj = dat.u32(j + 0x10)
                dn = 0
                while dobj:
                    mobj = dat.u32(dobj + 8)
                    t = dat.u32(mobj + 8) if mobj else 0
                    tn = 0
                    while t:
                        rot, tsc, tr = dat.f(t + 0x10, 3), dat.f(t + 0x1C, 3), dat.f(t + 0x28, 3)
                        ws, wt = dat.u32(t + 0x34), dat.u32(t + 0x38)
                        rs, rtt = dat.b[dat.d + t + 0x3C], dat.b[dat.d + t + 0x3D]
                        src = dat.u32(t + 0xC)
                        tflags = dat.u32(t + 0x40)
                        img = dat.u32(t + 0x4C)
                        w = h = fm = 0
                        if img:
                            w, h = struct.unpack_from('>2H', b, dat.d + img + 4)
                            fm = dat.u32(img + 8)
                        ident = (all(abs(x) < 1e-6 for x in rot) and all(abs(x - 1) < 1e-6 for x in tsc)
                                 and all(abs(x) < 1e-6 for x in tr))
                        unusual = not ident or rs != 1 or rtt != 1 or ws != 0 or wt != 0
                        if not odd or unusual:
                            print('%s %s J%d D%d T%d %dx%d %s src=%s wrap=%s/%s rep=%d/%d flags=%08X'
                                  ' scale=(%.4g,%.4g,%.4g) rot=(%.4g,%.4g,%.4g) trans=(%.4g,%.4g,%.4g) joint_scale=(%.3g,%.3g,%.3g)'
                                  % (os.path.basename(path), tname, jn, dn, tn, w, h, FMT.get(fm, fm), SRC.get(src, src),
                                     WRAP.get(ws, ws), WRAP.get(wt, wt), rs, rtt, tflags, *tsc, *rot, *tr, *sc))
                        t = dat.u32(t + 4)
                        tn += 1
                    dobj = dat.u32(dobj + 4)
                    dn += 1


if __name__ == '__main__':
    odd = '--odd' in sys.argv
    for a in [x for x in sys.argv[1:] if x != '--odd']:
        if os.path.isdir(a):
            for fn in sorted(os.listdir(a)):
                if fn.endswith('.dat'):
                    scan(os.path.join(a, fn), odd)
        else:
            scan(a, odd)
