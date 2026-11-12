"""motion_strip.py: the whole robo (body + parts, as the game assembles it) in the game's own motions, a grid of
frames, before anything goes into an ISO (Nov 11, 2026: the T-pose preview hid a skin that tore in every game pose).

  python -I tools/motion_strip.py <body.glb> <parts export dir> <out.png> [--mod MOD] [--body 0]
         [--slots stance,walk,run,weapon_jump,gun_stance,bomb_l90,victory1,charge] [--frames 4]

<body.glb>        the body with its JOBJ_n armature (atlas_ai_body.py's glb, or an HSDRawViewer export of the .dat)
<parts dir>       HSDRawViewer --export folders weapon_gun_000/ ... (robo_preview.py)
--mod             take the motions the mod plays (its code/motion.txt set and files/motion.DBN); default: the disc's
Each row is a motion, each column a frame spread over its length; one three-quarter view per cell, the same camera
box for every cell so the sizes compare. Standard library + Blender (BLENDER env or the default install path).
"""
import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

G = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(G / 'tools'))
import anim  # noqa: E402
import crmotion as c  # noqa: E402
import crmotion_rig  # noqa: E402

BLENDER = os.environ.get('BLENDER') or r'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe'
SETS = 0x801C51B8


def mod_set(mod, body):
    """the mod's 38 motion IDs for the body: the disc's set with the mod's code/motion.txt writes applied"""
    dol = c.load_dol(None)
    ids = list(struct.unpack('>38H', dol.read(SETS + body * 0x4C, 0x4C)))
    code = G / 'mods' / mod / 'code' / 'motion.txt'
    if code.exists():
        for line in code.read_text().splitlines():
            m = re.match(r'([0-9A-Fa-f]{8}):\s*([0-9A-Fa-f]{8})', line)
            if not m:
                continue
            a, v = int(m.group(1), 16), int(m.group(2), 16)
            k = (a - SETS - body * 0x4C) // 2
            if 0 <= k < 38:
                ids[k] = v >> 16
                if k + 1 < 38:
                    ids[k + 1] = v & 0xFFFF
    return ids


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('body_glb')
    ap.add_argument('parts')
    ap.add_argument('out')
    ap.add_argument('--mod')
    ap.add_argument('--body', type=int, default=0)
    ap.add_argument('--slots', default='stance,walk,run,weapon_jump,gun_stance,bomb_l90,victory1,charge')
    ap.add_argument('--frames', type=int, default=4)
    ap.add_argument('--partpose', help="a battle capture's mats json: the parts' own joint poses (robo_preview.py)")
    a = ap.parse_args()
    dol = c.load_dol(None)
    ids = mod_set(a.mod, a.body) if a.mod else None
    src = anim.motion_file(a.mod)
    binp, glb = anim.body_paths(a.body)
    tmp = Path(tempfile.mkdtemp(prefix='strip_'))
    cells = []
    flags = []
    flagged = set()
    for name in a.slots.split(','):
        k = anim.slot_of(name)
        sets = None
        mid = (ids[k] if ids else struct.unpack('>38H', dol.read(SETS + a.body * 0x4C, 0x4C))[k]) & 0x1FFF
        mg = tmp / f'{name}.glb'
        crmotion_rig.cmd_gltf(str(src), mid, str(binp), str(glb), str(mg), dol)
        last = max(1, int(anim.last_frame(c.parse(c.read_motion(src)), mid)[2]))
        row = []
        for i in range(a.frames):
            f = round(last * i / max(1, a.frames - 1)) if a.frames > 1 else 0
            out = tmp / f'{name}_{f:03d}'
            r = subprocess.run([BLENDER, '-b', '--factory-startup', '--python', str(G / 'tools' / 'blender' / 'robo_preview.py'),
                                '--', a.body_glb, str(mg), a.parts, str(out), f'--frame={f}', '--views=1', '--w=300', '--h=340',
                                '--ortho=1.75', '--centre=0,0.05,0.85'] + ([f'--partpose={a.partpose}'] if a.partpose else []),
                               capture_output=True, text=True)
            if r.returncode or not (out.with_suffix('.png')).exists():
                sys.stderr.write(r.stdout[-2000:] + r.stderr[-2000:])
                raise SystemExit(f'render failed: {name} frame {f}')
            row.append(str(out.with_suffix('.png')))
            bad = [l.split('robo_preview: ', 1)[1] for l in r.stdout.splitlines()
                   if l.startswith('robo_preview: ') and 'FLOATING' in l and 'verdict' not in l]
            flags.append((name, f, bad))
            if bad:
                flagged.add(str(out.with_suffix('.png')))
        cells.append((name, row))
        print(f'{name}: motion {mid:#x}, frames 0..{last}')
    # tile with Blender's image API (standard library has no PNG writer)
    script = tmp / 'tile.py'
    script.write_text('''import bpy, sys, json
a = json.loads(sys.argv[sys.argv.index("--") + 1])
flagged = set(a["flagged"])
rows = a["rows"]
ims = [[bpy.data.images.load(p) for p in r] for r in rows]
W, H = ims[0][0].size
cols = max(len(r) for r in ims)
o = bpy.data.images.new("o", W * cols, H * len(ims))
buf = [0.0] * (W * cols * H * len(ims) * 4)
for ri, r in enumerate(ims):
    oy = (len(ims) - 1 - ri) * H
    for ci, im in enumerate(r):
        px = list(im.pixels)
        if bpy.path.abspath(im.filepath).replace(chr(92), "/") in {f.replace(chr(92), "/") for f in flagged}:
            for y in range(H):
                for x in range(W):
                    if x < 5 or x >= W - 5 or y < 5 or y >= H - 5:
                        px[(y * W + x) * 4:(y * W + x) * 4 + 4] = [0.9, 0.05, 0.05, 1.0]
        for y in range(H):
            s = ((oy + y) * W * cols + ci * W) * 4
            buf[s:s + W * 4] = px[y * W * 4:(y + 1) * W * 4]
o.pixels = buf
o.filepath_raw = a["out"]
o.file_format = "PNG"
o.save()
''')
    import json
    tr = subprocess.run([BLENDER, '-b', '--factory-startup', '--python', str(script), '--',
                    json.dumps({'rows': [r for _, r in cells], 'out': str(Path(a.out).resolve()), 'flagged': sorted(flagged)})],
                       capture_output=True, text=True)
    if not Path(a.out).exists():
        sys.stderr.write(tr.stdout[-3000:] + tr.stderr[-3000:])
    rep = Path(a.out).with_suffix('.check.txt')
    lines = [f'{n} frame {f}: ' + ('; '.join(b) if b else 'ok') for n, f, b in flags]
    rep.write_text('Attachment check (gap > 0.02 to the body = FLOATING; robo about 1.3 high)' + chr(10)
                   + chr(10).join(lines) + chr(10))
    nbad = sum(1 for _, _, b in flags if b)
    print(f'check: {nbad} of {len(flags)} frames with something floating -> {rep}')
    for l in lines:
        if not l.endswith('ok'):
            print('  ' + l)
    print('rows (top to bottom):', ', '.join(n for n, _ in cells))
    print('wrote', a.out)


if __name__ == '__main__':
    main()
