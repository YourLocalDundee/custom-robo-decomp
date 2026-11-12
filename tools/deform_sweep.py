"""deform_sweep.py: tools/blender/deform_check.py over a mod body's motions (Nov 12, 2026).

  python -I tools/deform_sweep.py <body.glb> <roles json> <out dir> --mod mecha_samurai_full [--slots a,b,...] [--step 2]

For every slot (default: all of body 0's slots the mod restyles plus the four of its own): the motion as a glb
(crmotion_rig.cmd_gltf, as motion_strip.py), then deform_check.py. Writes <out dir>/<slot>.txt / .json /
_worst.png and <out dir>/summary.txt (one line per slot: stretched / squashed triangles, clipping vertices, the worst
frame).
"""
import argparse
import os
import struct
import subprocess
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(G / 'tools'))
import anim  # noqa: E402
import crmotion as c  # noqa: E402
import crmotion_rig  # noqa: E402
from motion_strip import mod_set, SETS  # noqa: E402

BLENDER = os.environ.get('BLENDER') or r'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe'
ALL = ('stance,walk,run,walk_weapon_r180,weapon_jump,air_dash,double_jump,landing,bomb_l90,pod,gun_stance,gun_l90,'
       'gun_stiff,fall_back,fall_front,getup,lose,victory1,victory2,charge')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('body_glb')
    ap.add_argument('roles')
    ap.add_argument('out')
    ap.add_argument('--mod')
    ap.add_argument('--body', type=int, default=0)
    ap.add_argument('--slots', default=ALL)
    ap.add_argument('--step', type=int, default=2)
    a = ap.parse_args()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    dol = c.load_dol(None)
    ids = mod_set(a.mod, a.body) if a.mod else None
    src = anim.motion_file(a.mod)
    binp, glb = anim.body_paths(a.body)
    summary = []
    for name in a.slots.split(','):
        k = anim.slot_of(name)
        mid = (ids[k] if ids else struct.unpack('>38H', dol.read(SETS + a.body * 0x4C, 0x4C))[k]) & 0x1FFF
        mg = out / f'{name}.motion.glb'
        # baked against the body under test (Nov 12, 2026): Blender keeps an action relative to the rest pose of the
        # skeleton it was imported with, so a body bound in another pose (tools/blender/rebind.py) needs its own rest
        crmotion_rig.cmd_gltf(str(src), mid, str(binp), str(a.body_glb), str(mg), dol)
        r = subprocess.run([BLENDER, '-b', '--factory-startup', '--python', str(G / 'tools' / 'blender' / 'deform_check.py'),
                            '--', a.body_glb, str(mg), a.roles, str(out / name), f'--step={a.step}'],
                           capture_output=True, text=True)
        txt = out / f'{name}.txt'
        if r.returncode or not txt.exists():
            sys.stderr.write(r.stdout[-2500:] + r.stderr[-2500:])
            summary.append(f'{name:22s} FAILED')
            continue
        t = txt.read_text().splitlines()
        st = [l for l in t[2:] if l.startswith('  ') and 'CLIPPING' not in l]
        ci = [i for i, l in enumerate(t) if l.startswith('CLIPPING')][0]
        stretch_lines = t[2:ci]
        clip_lines = [l for l in t[ci + 1:] if l.startswith('  ')]
        ns = sum(int(l.split()[1]) for l in stretch_lines)
        nq = sum(int(l.split()[2]) for l in stretch_lines)
        nc = sum(int(l.split()[-5]) for l in clip_lines) if clip_lines else 0
        top = stretch_lines[0].split()[0] if stretch_lines else '-'
        topc = clip_lines[0].strip().split('  ')[0] if clip_lines else '-'
        summary.append(f'{name:22s} stretched {ns:6d} squashed {nq:6d} (most: {top:20s}) clipping {nc:6d} '
                       f'(most: {topc}); {t[-1]}')
        print(summary[-1], flush=True)
    (out / 'summary.txt').write_text('\n'.join(summary) + '\n')


if __name__ == '__main__':
    main()
