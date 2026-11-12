r"""Mecha samurai test body (no weapons): tools/blender/atlas_ai_body.py on the AI model, then HSDRawViewer --import over Ray 01's five
entries (chara 000-004, its colour variants; template skeleton, so no roles file).

  python -I mods/mecha_samurai_full/make_body.py D:\ai\Pictures\mecha_samurai_v1\samurai_3d_00001_.glb --tris=<n>

The AI model: a FLUX 3 Klein picture (D:\ai\Pictures\mecha_samurai_v1\klein_s1234.png, prompt.txt beside it) turned into a
textured 3D model by Pixal3D + TRELLIS.2 in ComfyUI (D:\ai\work\wf_pixal3d_trellis2_api.json). No sword or shield.
Big body: the entry may be up to 0x100000 bytes (code/big_body.txt moves the body slots to expanded RAM).
"""
import os
import subprocess
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
EXE = G / 'tools' / 'HSDLib' / 'HSDRawViewer' / 'bin' / 'Release' / 'net8.0-windows7.0' / 'HSDRawViewer.exe'
BLENDER = os.environ.get('BLENDER') or r'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe'
LIMIT = 0x200000     # the body slot of code/samurai.txt (Dolphin MEM1 64 MB)


def run(args):
    r = subprocess.run([str(a) for a in args], capture_output=True, text=True)
    if r.returncode != 0:
        sys.stdout.write(r.stdout[-3000:])
        sys.stderr.write(r.stderr[-3000:])
        raise SystemExit('failed: %s' % args[0])
    return r.stdout


def main():
    ai = Path(sys.argv[1]).resolve()
    # --work=<name>: build in build/mecha_samurai_full/<name>; --dst=<dir>: write the .dat files there instead of the
    # mod (trial looks side by side, docs/robo_look.md)
    own = {k[2:]: v for k, _, v in (x.partition('=') for x in sys.argv[2:] if x.startswith(('--work=', '--dst=')))}
    sys.argv = [x for x in sys.argv if not x.startswith(('--work=', '--dst='))]
    work = G / 'build' / 'mecha_samurai_full' / own.get('work', 'body')
    work.mkdir(parents=True, exist_ok=True)
    roles = work / 'roles_000.json'
    roles.write_text(run([sys.executable, '-I', G / 'tools' / 'robo_joints.py', 'roles', G / 'unpacked' / 'chara' / '000.bin']))
    ray = G / 'build' / 'model_loop' / 'ray_legionnaire' / 'export' / 'chara_000' / 'root0_joint.glb'
    if not ray.exists():
        ray.parent.mkdir(parents=True, exist_ok=True)
        run([EXE, '--export', ray.parent, 'glb', G / 'models_dat' / 'chara' / 'chara_000.dat'])
    guide = G / 'build' / 'model_loop' / 'ray_legionnaire' / 'chara_000.edited.glb'
    out = work / 'samurai.glb'
    log = run([BLENDER, '-b', '--factory-startup', '--python-exit-code', '1', '--python', G / 'tools' / 'blender' / 'atlas_ai_body.py', '--',
               ray, out, roles, ai, guide, guide.with_suffix('.pose.json')] + [x for x in sys.argv[2:] if x.startswith('--')])
    print('\n'.join(l for l in log.splitlines() if l.startswith('atlas_ai_body:')))
    dst = Path(own['dst']).resolve() if own.get('dst') else MOD / 'models' / 'chara'
    dst.mkdir(parents=True, exist_ok=True)
    for f in dst.glob('chara_*.roles.json'):
        f.unlink()                      # Ray 01's own skeleton: no roles file
    for n in range(5):
        o = dst / f'chara_{n:03d}.dat'
        run([EXE, '--import', G / 'models_dat' / 'chara' / f'chara_{n:03d}.dat', out, o, '--replace'])
        size = o.stat().st_size
        print(f'{o.name}: {size} bytes (limit {LIMIT})')
        if size > LIMIT:
            raise SystemExit('too big for the body buffer')


if __name__ == '__main__':
    main()
