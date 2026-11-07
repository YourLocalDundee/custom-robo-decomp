"""scene_data.py: read the game's standard HSD .dat files (stages, effects, story objects) without HSDLib.

Standard library only. The layout is sysdolphin's (as Super Smash Bros. Melee; HSDLib reads and writes it): a 0x20
header (file size, data size, relocation count, root count, reference count), the data block, the relocation table
(offsets of every pointer word in the data block), the roots (data offset, name offset), the references, then the
name strings. A root named `scene_data` is an HSD scene (HSDLib HSD_SOBJ): docs/WHERE_IS_EVERYTHING.md section 10.

  python -I tools/scene_data.py info models_dat/bg_data/bg_data_000.dat [--joints]
      roots, then the scene: every model (joints, DObjs, PObjs, materials, textures; its joint / material / shape
      animations with end frames and animated channels), cameras, lights, fog
  python -I tools/scene_data.py scan models_dat/gun_effect [--tsv]
      one line per file: models, joints, DObjs, textures, animations (end frames), cameras, lights, fog, externs
  python -I tools/scene_data.py roots FILE.dat
      the roots and references (names + offsets) only
"""
import argparse
import os
import struct
import sys

# FObj track types (HSDLib JointTrackType / MatTrackType; the joint ones are what the game's stages and effects use)
JOINT_TRACKS = {40: 'ptcl', 1: 'rot_x', 2: 'rot_y', 3: 'rot_z', 4: 'path', 5: 'trans_x', 6: 'trans_y', 7: 'trans_z', 8: 'scale_x',
                9: 'scale_y', 10: 'scale_z', 11: 'node', 12: 'branch', 20: 'set_byte0', 21: 'set_byte1',
                22: 'set_byte2', 23: 'set_byte3', 24: 'set_byte4', 25: 'set_byte5', 26: 'set_byte6', 27: 'set_byte7',
                28: 'set_byte8', 29: 'set_byte9', 30: 'set_float0', 31: 'set_float1', 32: 'set_float2',
                33: 'set_float3', 34: 'set_float4', 35: 'set_float5', 36: 'set_float6', 37: 'set_float7',
                38: 'set_float8', 39: 'set_float9'}
MAT_TRACKS = {1: 'ambient_r', 2: 'ambient_g', 3: 'ambient_b', 4: 'diffuse_r', 5: 'diffuse_g', 6: 'diffuse_b',
              7: 'specular_r', 8: 'specular_g', 9: 'specular_b', 10: 'alpha', 11: 'pixel_process'}
TEX_TRACKS = {1: 'timg', 2: 'trans_u', 3: 'trans_v', 4: 'scale_u', 5: 'scale_v', 6: 'rot_x', 7: 'rot_y', 8: 'rot_z',
              9: 'blend', 10: 'tclt'}


class Dat:
    def __init__(self, path):
        self.path = path
        b = open(path, 'rb').read()
        self.raw = b
        self.size, self.data_size, self.nreloc, self.nroot, self.nref = struct.unpack_from('>5I', b, 0)
        self.data = b[0x20:0x20 + self.data_size]
        p = 0x20 + self.data_size
        self.relocs = set(struct.unpack_from('>%dI' % self.nreloc, b, p))
        p += 4 * self.nreloc
        roots = [struct.unpack_from('>II', b, p + 8 * i) for i in range(self.nroot)]
        p += 8 * self.nroot
        refs = [struct.unpack_from('>II', b, p + 8 * i) for i in range(self.nref)]
        p += 8 * self.nref
        self.strings_at = p

        def name(o):
            e = b.index(b'\0', p + o)
            return b[p + o:e].decode('latin1')
        self.roots = [(name(n), off) for off, n in roots]
        self.refs = [(name(n), off) for off, n in refs]

    def u32(self, o):
        return struct.unpack_from('>I', self.data, o)[0]

    def u16(self, o):
        return struct.unpack_from('>H', self.data, o)[0]

    def s16(self, o):
        return struct.unpack_from('>h', self.data, o)[0]

    def f32(self, o):
        return struct.unpack_from('>f', self.data, o)[0]

    def u8(self, o):
        return self.data[o]

    def ptr(self, o):
        """the pointer word at data offset o (None when the word is not relocated or 0)"""
        if o not in self.relocs:
            return None
        return self.u32(o)

    def ptr_array(self, o):
        """a null-terminated array of pointers at o"""
        out = []
        while o is not None and o + 4 <= len(self.data):
            p = self.ptr(o)
            if p is None:
                break
            out.append(p)
            o += 4
        return out

    def cstr(self, o):
        if o is None:
            return None
        e = self.data.index(b'\0', o)
        return self.data[o:e].decode('latin1')


# ---- joints, DObjs, materials ----------------------------------------------------------------------------------

def walk_joints(d, j, depth=0, out=None):
    """every joint of the tree at j: (offset, depth, flags, dobj offset)"""
    if out is None:
        out = []
    while j is not None:
        out.append((j, depth, d.u32(j + 4), d.ptr(j + 0x10)))
        if not d.u32(j + 4) & 0x1000:          # JOBJ_INSTANCE: the child is a joint shared by reference
            walk_joints(d, d.ptr(j + 8), depth + 1, out)
        j = d.ptr(j + 0xC)
    return out


def dobj_info(d, dobj):
    """(DObjs, PObjs, MObjs, TObjs) along a DObj list"""
    n_d = n_p = n_t = 0
    mobjs = set()
    while dobj is not None:
        n_d += 1
        m = d.ptr(dobj + 8)
        if m is not None:
            mobjs.add(m)
            t = d.ptr(m + 8)
            while t is not None:
                n_t += 1
                t = d.ptr(t + 4)
        p = d.ptr(dobj + 0xC)
        while p is not None:
            n_p += 1
            p = d.ptr(p + 4)
        dobj = d.ptr(dobj + 4)
    return n_d, n_p, mobjs, n_t


# ---- animations ------------------------------------------------------------------------------------------------

PTCL_CODES = []   # (bank, generator) of every PTCL key read by fobj_tracks, in order


def fobj_tracks(d, f, names):
    out = []
    while f is not None:
        t = d.u8(f + 0xC)
        out.append(names.get(t, 'type%d' % t))
        if names is JOINT_TRACKS and t == 40:
            # PTCL: the key's bytes 1..3 little-endian; bank = bits 0-5, generator number = bits 6-23 (HSDLib FOBJ_Player)
            dp, ln = d.ptr(f + 0x10), d.u32(f + 4)
            if dp is not None and ln >= 4:
                code = d.data[dp + 1] | (d.data[dp + 2] << 8) | (d.data[dp + 3] << 16)
                PTCL_CODES.append((code & 0x3F, code >> 6))
        f = d.ptr(f)
    return out


def aobj(d, a, names):
    """(end frame, flags, [tracks])"""
    if a is None:
        return None
    return d.f32(a + 4), d.u32(a), fobj_tracks(d, d.ptr(a + 8), names)


def walk_animjoint(d, aj, out):
    """the AObjs of an AnimJoint tree (child +0, next +4, aobj +8)"""
    while aj is not None:
        a = aobj(d, d.ptr(aj + 8), JOINT_TRACKS)
        if a:
            out.append(a)
        walk_animjoint(d, d.ptr(aj), out)
        aj = d.ptr(aj + 4)
    return out


def walk_matanimjoint(d, mj, out):
    """MatAnimJoint (child +0, next +4, matanim +8); MatAnim (next +0, aobj +4, texanim +8, renderanim +0xC);
    TexAnim (next +0, tex id +4, aobj +8, ...)"""
    while mj is not None:
        ma = d.ptr(mj + 8)
        while ma is not None:
            a = aobj(d, d.ptr(ma + 4), MAT_TRACKS)
            if a:
                out.append(('mat',) + a)
            ta = d.ptr(ma + 8)
            while ta is not None:
                a = aobj(d, d.ptr(ta + 8), TEX_TRACKS)
                if a:
                    out.append(('tex',) + a)
                ta = d.ptr(ta)
            ma = d.ptr(ma)
        walk_matanimjoint(d, d.ptr(mj), out)
        mj = d.ptr(mj + 4)
    return out


def anim_summary(items, kind_first=False):
    if not items:
        return '-'
    ends = sorted({round(x[1] if kind_first else x[0], 2) for x in items})
    tracks = {}
    for x in items:
        for t in (x[3] if kind_first else x[2]):
            tracks[t] = tracks.get(t, 0) + 1
    return '%d AObjs, end %s, %s' % (len(items), '/'.join('%g' % e for e in ends[:6]) + ('...' if len(ends) > 6 else ''),
                                    ' '.join('%s x%d' % kv for kv in sorted(tracks.items())))


# ---- the scene -------------------------------------------------------------------------------------------------

def wobj_pos(d, w):
    if w is None:
        return None
    return tuple(round(d.f32(w + 4 + 4 * i), 3) for i in range(3))


def scene(d, s):
    """HSD_SOBJ at s: +0 JOBJDesc array, +4 camera array, +8 light array, +0xC fog (FogDesc pointer + AObj)"""
    out = {'models': [], 'cameras': [], 'lights': [], 'fog': None}
    for jd in d.ptr_array(d.ptr(s)) if d.ptr(s) is not None else []:
        root = d.ptr(jd)
        joints = walk_joints(d, root)
        n_d = n_p = n_t = 0
        mobjs = set()
        for _, _, _, dobj in joints:
            a, b2, m, t = dobj_info(d, dobj)
            n_d += a
            n_p += b2
            mobjs |= m
            n_t += t
        ja, ma, sa = [], [], []
        del PTCL_CODES[:]
        for aj in d.ptr_array(d.ptr(jd + 4)) if d.ptr(jd + 4) is not None else []:
            walk_animjoint(d, aj, ja)
        n_janim = len(d.ptr_array(d.ptr(jd + 4))) if d.ptr(jd + 4) is not None else 0
        for mj in d.ptr_array(d.ptr(jd + 8)) if d.ptr(jd + 8) is not None else []:
            walk_matanimjoint(d, mj, ma)
        n_manim = len(d.ptr_array(d.ptr(jd + 8))) if d.ptr(jd + 8) is not None else 0
        n_sanim = len(d.ptr_array(d.ptr(jd + 0xC))) if d.ptr(jd + 0xC) is not None else 0
        ptcl = list(PTCL_CODES)
        out['models'].append({'ptcl': ptcl, 'offset': jd, 'joints': len(joints), 'root_dobjs': dobj_info(d, d.ptr(root + 0x10))[0] if root is not None else 0,
                              'dobjs': n_d, 'pobjs': n_p, 'mobjs': len(mobjs), 'tobjs': n_t,
                              'joint_anims': n_janim, 'joint_anim': ja, 'mat_anims': n_manim, 'mat_anim': ma,
                              'shape_anims': n_sanim, 'joint_list': joints})
    for c in d.ptr_array(d.ptr(s + 4)) if d.ptr(s + 4) is not None else []:
        out['cameras'].append({'flags': d.u16(c + 4), 'projection': d.u16(c + 6),
                               'viewport': tuple(d.s16(c + 8 + 2 * i) for i in range(4)),
                               'eye': wobj_pos(d, d.ptr(c + 0x18)), 'target': wobj_pos(d, d.ptr(c + 0x1C)),
                               'roll': d.f32(c + 0x20), 'near': d.f32(c + 0x28), 'far': d.f32(c + 0x2C),
                               'fov_or_top': d.f32(c + 0x30), 'aspect_or_bottom': d.f32(c + 0x34)})
    for l in d.ptr_array(d.ptr(s + 8)) if d.ptr(s + 8) is not None else []:
        lo = d.ptr(l)
        if lo is None:
            continue
        fl = d.u16(lo + 8)
        kind = {0: 'ambient', 1: 'infinite', 2: 'point', 3: 'spot'}.get(fl & 3, '?')
        out['lights'].append({'kind': kind, 'flags': fl, 'color': tuple(d.data[lo + 0xC:lo + 0x10]),
                              'position': wobj_pos(d, d.ptr(lo + 0x10)), 'interest': wobj_pos(d, d.ptr(lo + 0x14)),
                              'animated': d.ptr(l + 4) is not None})
    fa = d.ptr(s + 0xC)
    if fa is not None:
        f = d.ptr(fa)
        if f is not None:
            out['fog'] = {'type': d.u32(f), 'start': d.f32(f + 8), 'end': d.f32(f + 0xC),
                          'color': tuple(d.data[f + 0x10:f + 0x14]), 'animated': d.ptr(fa + 4) is not None}
    return out


def info(path, show_joints=False):
    d = Dat(path)
    print('%s: %d bytes, data 0x%X, %d relocations' % (path, d.size, d.data_size, d.nreloc))
    for n, o in d.roots:
        print('  root %-24s +0x%X' % (n, o))
    for n, o in d.refs:
        print('  extern %-22s +0x%X' % (n, o))
    for n, o in d.roots:
        if n != 'scene_data':
            continue
        sc = scene(d, o)
        for i, m in enumerate(sc['models']):
            print('  model %d: %d joints (root has %d DObjs), %d DObjs, %d PObjs, %d materials, %d textures'
                  % (i, m['joints'], m['root_dobjs'], m['dobjs'], m['pobjs'], m['mobjs'], m['tobjs']))
            if m['joint_anims'] or m['mat_anims'] or m['shape_anims']:
                print('     joint anims %d: %s' % (m['joint_anims'], anim_summary(m['joint_anim'])))
                print('     material anims %d: %s' % (m['mat_anims'], anim_summary(m['mat_anim'], True)))
                if m['shape_anims']:
                    print('     shape anims %d' % m['shape_anims'])
                if m['ptcl']:
                    print('     particles fired: %s' % ', '.join('%d (bank %d)' % (g, b) for b, g in m['ptcl']))
            if show_joints:
                for j, dep, fl, dobj in m['joint_list']:
                    tr = tuple(round(d.f32(j + 0x2C + 4 * k), 3) for k in range(3))
                    print('       %s+0x%X flags 0x%08X %s t=%s' % ('  ' * dep, j, fl, 'dobj' if dobj is not None else '', tr))
        for c in sc['cameras']:
            print('  camera: projection %d, eye %s, target %s, near %g, far %g, fov %g, aspect %g'
                  % (c['projection'], c['eye'], c['target'], c['near'], c['far'], c['fov_or_top'], c['aspect_or_bottom']))
        for l in sc['lights']:
            print('  light: %s, colour %s, position %s%s' % (l['kind'], l['color'], l['position'], ', animated' if l['animated'] else ''))
        if sc['fog']:
            f = sc['fog']
            print('  fog: type %d, %g .. %g, colour %s%s' % (f['type'], f['start'], f['end'], f['color'], ', animated' if f['animated'] else ''))
    return d


def scan(folder, tsv=False):
    files = sorted(x for x in os.listdir(folder) if x.endswith('.dat'))
    hdr = 'file\troots\tmodels\tjoints\tdobjs\ttextures\tjoint_anim_end\tmat_anims\tcameras\tlights\tfog\texterns'
    print(hdr if tsv else hdr.replace('\t', '  '))
    for fn in files:
        d = Dat(os.path.join(folder, fn))
        roots = ','.join(n for n, _ in d.roots)
        row = [fn, roots]
        sc = None
        for n, o in d.roots:
            if n == 'scene_data':
                sc = scene(d, o)
        if sc:
            ms = sc['models']
            ends = sorted({round(a[0], 2) for m in ms for a in m['joint_anim']})
            row += [str(len(ms)), str(sum(m['joints'] for m in ms)), str(sum(m['dobjs'] for m in ms)),
                    str(sum(m['tobjs'] for m in ms)), '/'.join('%g' % e for e in ends) or '-',
                    str(sum(len(m['mat_anim']) for m in ms)), str(len(sc['cameras'])), str(len(sc['lights'])),
                    'yes' if sc['fog'] else '-']
        else:
            row += ['-'] * 9
        row.append(','.join(n for n, _ in d.refs) or '-')
        print('\t'.join(row) if tsv else '  '.join(row))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('info')
    p.add_argument('file')
    p.add_argument('--joints', action='store_true')
    p = sub.add_parser('scan')
    p.add_argument('folder')
    p.add_argument('--tsv', action='store_true')
    p = sub.add_parser('roots')
    p.add_argument('file')
    a = ap.parse_args()
    sys.stdout.reconfigure(errors='replace')
    if a.cmd == 'info':
        info(a.file, a.joints)
    elif a.cmd == 'scan':
        scan(a.folder, a.tsv)
    else:
        d = Dat(a.file)
        for n, o in d.roots:
            print('root', n, hex(o))
        for n, o in d.refs:
            print('extern', n, hex(o))


if __name__ == '__main__':
    main()
