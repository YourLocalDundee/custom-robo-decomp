"""stage_collision.py: export / import a stage's collision (bg_data.BIN entry 2s+1) as an OBJ for Blender.

Standard library only. Format (docs/stage_tables.md "Entry 2s+1", docs/WHERE_IS_EVERYTHING.md section 10; read by fn_80072A58):
  +0x00 u32 N groups, +0x04 offset of N group-header offsets, +0x08 offset of N quad-array offsets,
  +0x0C offset of section C (0x284-byte waypoint records) or 0
  group header (0x20): bounding sphere x, y, z, r; u32 quad count; u32 model flags (scene model n's +0xF8); 2 x u32 0
  quad (0x44): 4 corners (12 floats), unit normal (3 floats), plane d (n . p + d = 0), u32 attribute (surface kind)
  layout: 0x10 header, the two offset tables, zero padding to 0x10, then per group its header and quads padded to
  0x10, then section C (to the end of the file; the entry itself is padded to 0x20 in bg_data.BIN).
Group n belongs to scene model n of entry 2s (the stage model).

  python -I tools/stage_collision.py export 0 [--out DIR]
      stage id 0 (unpacked/bg_data/001.bin) -> DIR/stage_000_collision.obj (+ .json: group flags, spheres, the
      waypoint block); one OBJ object per group (`group_<n>`), one face per quad, the attribute as its material
      (`attr_<value>`). Blender: File > Import > Wavefront (.obj), keep the default axes (as for the model glb).
  python -I tools/stage_collision.py import 0 IN.obj (OUT.bin | --mod MOD) [--template T.bin]
      with --mod the entry goes to mods/MOD/models/bg_data/bg_data_<2s+1>.dat, which tools/make_mod.py packs into
      bg_data.BIN (refusing more than 0x8000 bytes); rebuilds the entry: a quad whose 4 corners are a template quad's keeps that quad's bytes (normal, d,
      attribute unless its material says another); a new or moved quad gets its normal from the corners (the
      game's winding: (c1 - c0) x (c2 - c0), normalised) and d = -n . c0. A triangle is stored as a quad with its
      last corner repeated. Spheres are rebuilt for changed groups (centre = the corners' box centre, radius = the
      farthest corner, as the retail files). Objects `group_<n>` with n past the template's last group add groups
      (model flags from `--flags`, default 0): the stage model needs a scene model for each group.
  python -I tools/stage_collision.py roundtrip
      every collision entry of unpacked/bg_data: export, import with no edit, compare bytes
  python -I tools/stage_collision.py info 0
      groups, quad counts, flags, attributes, the waypoint record count
"""
import argparse
import json
import math
import os
import re
import struct
import sys

R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BG = os.path.join(R, 'unpacked', 'bg_data')
WAYPOINT = 0x284


def entry_path(stage):
    return os.path.join(BG, '%03d.bin' % (2 * stage + 1))


def is_collision(b):
    if len(b) < 0x10:
        return False
    n, oh, oq, oc = struct.unpack_from('>4I', b, 0)
    if not (0 < n <= 64 and oh == 0x10 and oq == 0x10 + 4 * n and oc <= len(b)):
        return False
    for k in range(n):
        h, q = struct.unpack_from('>I', b, oh + 4 * k)[0], struct.unpack_from('>I', b, oq + 4 * k)[0]
        if h + 0x20 > len(b) or q + 0x44 * struct.unpack_from('>I', b, h + 16)[0] > len(b):
            return False
    return True


def parse(b):
    n, oh, oq, oc = struct.unpack_from('>4I', b, 0)
    hdrs = struct.unpack_from('>%dI' % n, b, oh)
    qofs = struct.unpack_from('>%dI' % n, b, oq)
    groups = []
    for h, q in zip(hdrs, qofs):
        x, y, z, r, cnt, flags, z0, z1 = struct.unpack_from('>4f4I', b, h)
        quads = []
        for k in range(cnt):
            o = q + k * 0x44
            f = struct.unpack_from('>16f', b, o)
            quads.append({'corners': [f[i * 3:i * 3 + 3] for i in range(4)], 'normal': f[12:15], 'd': f[15],
                          'attr': struct.unpack_from('>I', b, o + 0x40)[0], 'raw': b[o:o + 0x44]})
        groups.append({'sphere': (x, y, z, r), 'flags': flags, 'pad': (z0, z1), 'quads': quads,
                       'raw_header': b[h:h + 0x20]})
    wp = b[oc:] if oc else b''
    return {'groups': groups, 'waypoints': wp, 'has_c': oc != 0}


def build(groups, waypoints, has_c):
    n = len(groups)
    out = bytearray(0x10 + 8 * n)
    pos = (len(out) + 0xF) & ~0xF
    out += bytes(pos - len(out))
    hdr_ofs, q_ofs = [], []
    for g in groups:
        hdr_ofs.append(len(out))
        if 'raw_header' in g and g.get('keep_header'):
            out += g['raw_header']
        else:
            x, y, z, r = g['sphere']
            out += struct.pack('>4f4I', x, y, z, r, len(g['quads']), g['flags'], *g.get('pad', (0, 0)))
        q_ofs.append(len(out))
        for q in g['quads']:
            if q.get('raw') is not None and q.get('keep'):
                out += q['raw']
            else:
                c = [v for corner in q['corners'] for v in corner]
                out += struct.pack('>16fI', *(c + list(q['normal']) + [q['d']]), q['attr'])
        out += bytes(((len(out) + 0xF) & ~0xF) - len(out))
    oc = len(out) if has_c else 0
    out += waypoints
    struct.pack_into('>4I', out, 0, n, 0x10, 0x10 + 4 * n, oc)
    struct.pack_into('>%dI' % n, out, 0x10, *hdr_ofs)
    struct.pack_into('>%dI' % n, out, 0x10 + 4 * n, *q_ofs)
    return bytes(out)


def fmt(v):
    s = repr(float(v))
    return s


def export(stage, out_dir, src=None):
    src = src or entry_path(stage)
    b = open(src, 'rb').read()
    if not is_collision(b):
        raise SystemExit('%s is not a collision entry of this format' % src)
    c = parse(b)
    os.makedirs(out_dir, exist_ok=True)
    base = os.path.join(out_dir, 'stage_%03d_collision' % stage)
    lines = ['# Custom Robo stage %d collision (bg_data entry %03d): tools/stage_collision.py' % (stage, 2 * stage + 1),
             'mtllib %s.mtl' % os.path.basename(base)]
    vi = 1
    attrs = set()
    for gi, g in enumerate(c['groups']):
        lines.append('o group_%d' % gi)
        for q in g['quads']:
            for corner in q['corners']:
                lines.append('v %s %s %s' % tuple(fmt(x) for x in corner))
            attrs.add(q['attr'])
            lines.append('usemtl attr_%d' % q['attr'])
            lines.append('f %d %d %d %d' % (vi, vi + 1, vi + 2, vi + 3))
            vi += 4
    open(base + '.obj', 'w', newline='\n').write('\n'.join(lines) + '\n')
    mtl = []
    for a in sorted(attrs):
        h = (a * 2654435761) & 0xFFFFFF
        mtl += ['newmtl attr_%d' % a, 'Kd %.3f %.3f %.3f' % (((h >> 16) & 255) / 255, ((h >> 8) & 255) / 255, (h & 255) / 255), '']
    open(base + '.mtl', 'w', newline='\n').write('\n'.join(mtl))
    meta = {'stage': stage, 'entry': 2 * stage + 1,
            'groups': [{'flags': g['flags'], 'sphere': g['sphere'], 'quads': len(g['quads'])} for g in c['groups']],
            'waypoint_records': len(c['waypoints']) // WAYPOINT if c['has_c'] else None}
    open(base + '.json', 'w', newline='\n').write(json.dumps(meta, indent=1) + '\n')
    return base + '.obj'


def read_obj(path):
    verts, groups, cur, mat = [], {}, None, 0
    for line in open(path, encoding='utf-8', errors='replace'):
        p = line.split()
        if not p:
            continue
        if p[0] == 'v':
            verts.append(tuple(float(x) for x in p[1:4]))
        elif p[0] in ('o', 'g'):
            m = re.match(r'group_(\d+)', p[1] if len(p) > 1 else '')
            cur = int(m.group(1)) if m else None
            if cur is not None:
                groups.setdefault(cur, [])
        elif p[0] == 'usemtl':
            m = re.match(r'attr_(\d+)', p[1] if len(p) > 1 else '')
            mat = int(m.group(1)) if m else 0
        elif p[0] == 'f' and cur is not None:
            idx = [int(x.split('/')[0]) for x in p[1:]]
            idx = [i - 1 if i > 0 else len(verts) + i for i in idx]
            corners = [verts[i] for i in idx]
            if len(corners) == 3:
                corners.append(corners[2])
            if len(corners) != 4:
                raise SystemExit('%s: a face with %d corners in group_%d (use quads or triangles)' % (path, len(corners), cur))
            groups[cur].append((corners, mat))
    return groups


def f32(x):
    return struct.unpack('>f', struct.pack('>f', x))[0]


def plane(corners):
    a, b2, c = corners[0], corners[1], corners[2]
    u = [b2[i] - a[i] for i in range(3)]
    v = [c[i] - a[i] for i in range(3)]
    n = [u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]]
    ln = math.sqrt(sum(x * x for x in n)) or 1.0
    n = [x / ln for x in n]
    return n, -(n[0] * a[0] + n[1] * a[1] + n[2] * a[2])


def sphere(quads):
    pts = [p for q in quads for p in q['corners']]
    if not pts:
        return (0.0, 0.0, 0.0, 0.0)
    lo = [min(p[i] for p in pts) for i in range(3)]
    hi = [max(p[i] for p in pts) for i in range(3)]
    c = [(lo[i] + hi[i]) / 2 for i in range(3)]
    r = max(math.sqrt(sum((p[i] - c[i]) ** 2 for i in range(3))) for p in pts)
    return (c[0], c[1], c[2], r)


def import_obj(stage, obj, out, template=None, new_flags=0, quiet=False):
    template = template or entry_path(stage)
    t = parse(open(template, 'rb').read())
    og = read_obj(obj)
    groups = []
    changed = 0
    n = max(len(t['groups']), (max(og) + 1) if og else 0)
    for gi in range(n):
        tg = t['groups'][gi] if gi < len(t['groups']) else None
        faces = og.get(gi, [])
        tq = {}
        if tg:
            for q in tg['quads']:
                tq.setdefault(tuple(tuple(f32(x) for x in c) for c in q['corners']), q)
        quads = []
        same = tg is not None and len(faces) == len(tg['quads'])
        for k, (corners, attr) in enumerate(faces):
            key = tuple(tuple(f32(x) for x in c) for c in corners)
            old = tq.get(key)
            if old is not None:
                q = dict(old)
                q['keep'] = attr == old['attr']
                q['attr'] = attr
                if same and tg['quads'][k] is not old:
                    same = False
            else:
                nrm, d = plane(key)
                q = {'corners': key, 'normal': nrm, 'd': d, 'attr': attr, 'raw': None, 'keep': False}
                same = False
                changed += 1
            quads.append(q)
        if tg:
            g = {'flags': tg['flags'], 'pad': tg['pad'], 'quads': quads, 'raw_header': tg['raw_header'],
                 'keep_header': same, 'sphere': tg['sphere'] if same else sphere(quads)}
        else:
            g = {'flags': new_flags, 'pad': (0, 0), 'quads': quads, 'sphere': sphere(quads)}
        groups.append(g)
    data = build(groups, t['waypoints'], t['has_c'])
    open(out, 'wb').write(data)
    if not quiet:
        print('%s: %d groups, %d quads, %d new or moved; %d bytes (collision entries may be at most 0x8000)'
              % (out, len(groups), sum(len(g['quads']) for g in groups), changed, len(data)))
        if len(data) > 0x8000:
            print('warning: over the 0x8000-byte load region of a collision entry')
    return data


def cmd_info(stage):
    b = open(entry_path(stage), 'rb').read()
    c = parse(b)
    print('stage %d: entry %03d, %d bytes, %d groups, waypoints: %s' % (
        stage, 2 * stage + 1, len(b), len(c['groups']),
        ('%d records' % (len(c['waypoints']) // WAYPOINT)) if c['has_c'] else 'none'))
    for gi, g in enumerate(c['groups']):
        attrs = sorted({q['attr'] for q in g['quads']})
        print('  group %2d: %3d quads, flags %-4d sphere (%.2f, %.2f, %.2f) r %.2f, attributes %s' % (
            gi, len(g['quads']), g['flags'], *g['sphere'], attrs))


def cmd_roundtrip(tmp):
    ok = bad = skip = 0
    for fn in sorted(os.listdir(BG)):
        e = int(fn[:3])
        if e % 2 == 0:
            continue
        b = open(os.path.join(BG, fn), 'rb').read()
        if not is_collision(b):
            skip += 1
            continue
        s = e // 2
        obj = export(s, tmp)
        out = os.path.join(tmp, 'rt_%03d.bin' % e)
        data = import_obj(s, obj, out, quiet=True)
        if data == b:
            ok += 1
        else:
            bad += 1
            print('DIFF entry %03d: %d vs %d bytes' % (e, len(data), len(b)))
    print('round trip: %d byte-identical, %d differ, %d entries of another format skipped' % (ok, bad, skip))
    return bad == 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('export')
    p.add_argument('stage', type=int)
    p.add_argument('--out', default=os.path.join(R, 'build', 'collision'))
    p = sub.add_parser('import')
    p.add_argument('stage', type=int)
    p.add_argument('obj')
    p.add_argument('out', nargs='?')
    p.add_argument('--mod')
    p.add_argument('--template')
    p.add_argument('--flags', type=int, default=0)
    p = sub.add_parser('roundtrip')
    p.add_argument('--tmp', default=os.path.join(R, 'build', 'collision', 'roundtrip'))
    p = sub.add_parser('info')
    p.add_argument('stage', type=int)
    a = ap.parse_args()
    if a.cmd == 'export':
        print(export(a.stage, a.out))
    elif a.cmd == 'import':
        out = a.out
        if a.mod:
            d = os.path.join(R, 'mods', a.mod, 'models', 'bg_data')
            os.makedirs(d, exist_ok=True)
            out = os.path.join(d, 'bg_data_%03d.dat' % (2 * a.stage + 1))
        if not out:
            raise SystemExit('give OUT.bin or --mod MOD')
        import_obj(a.stage, a.obj, out, a.template, a.flags)
        if a.mod:
            print('next: python -I tools/make_mod.py mods/%s' % a.mod)
    elif a.cmd == 'roundtrip':
        sys.exit(0 if cmd_roundtrip(a.tmp) else 1)
    else:
        cmd_info(a.stage)


if __name__ == '__main__':
    main()
