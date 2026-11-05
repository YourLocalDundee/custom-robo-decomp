"""hsdparts.py FILE.dat ...: per root joint of a standard HSD .dat (our models_dat/), the joints, DObjs and PObjs, the
PObj kinds (flags & 0x3000: 0 plain / rigid to its joint, 0x1000 shape animation, 0x2000 envelope = weighted to
several joints), hidden joints (flag 0x10) and joints whose transform is not identity (Nov 5, 2026: to explain a
community parser's garbled multi-PObj parts)."""
import struct
import sys


def rd(path):
    d = open(path, 'rb').read()
    fsz, dsz, nrel, nroot, nref = struct.unpack_from('>5I', d, 0)
    data = d[0x20:0x20 + dsz]
    rt = 0x20 + dsz + nrel * 4
    strt = rt + (nroot + nref) * 8
    roots = []
    for i in range(nroot):
        off, so = struct.unpack_from('>II', d, rt + i * 8)
        nm = d[strt + so:d.index(b'\0', strt + so)].decode('latin-1')
        roots.append((nm, off))
    return data, roots


def u32(b, o): return struct.unpack_from('>I', b, o)[0]


def walk(data, j, depth, st):
    while j:
        st['joints'] += 1
        flags = u32(data, j + 4)
        if flags & 0x10: st['hidden'] += 1
        rot = struct.unpack_from('>3f', data, j + 0x14); sc = struct.unpack_from('>3f', data, j + 0x20); tr = struct.unpack_from('>3f', data, j + 0x2C)
        if any(abs(x) > 1e-6 for x in rot + tr) or any(abs(x - 1) > 1e-6 for x in sc): st['moved'] += 1
        if not flags & 0x4000 and not flags & 0x200000:   # not SPLINE / PTCL: +0x10 is a DObj
            dob = u32(data, j + 0x10)
            while dob:
                st['dobjs'] += 1
                pob = u32(data, dob + 0xC)
                while pob:
                    pf = struct.unpack_from('>H', data, pob + 0xC)[0]
                    st['pobj_' + {0: 'plain', 0x1000: 'shapeanim', 0x2000: 'envelope', 0x3000: 'other'}[pf & 0x3000]] += 1
                    pob = u32(data, pob + 4)
                dob = u32(data, dob + 4)
        walk(data, u32(data, j + 8), depth + 1, st)
        j = u32(data, j + 0xC)


for p in sys.argv[1:]:
    data, roots = rd(p)
    print(p.split('/')[-1])
    for nm, off in roots:
        st = dict(joints=0, hidden=0, moved=0, dobjs=0, pobj_plain=0, pobj_shapeanim=0, pobj_envelope=0, pobj_other=0)
        if nm.endswith('_joint'):
            walk(data, off, 0, st)
        print('   %-14s' % nm, ' '.join(f'{k}={v}' for k, v in st.items() if v))
