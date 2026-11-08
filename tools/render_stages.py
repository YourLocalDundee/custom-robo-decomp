"""Pictures of every stage, drawn from the game's own stage models (bg_data.BIN entry 2s of stage id s), for the docs.

  python -I tools/render_stages.py [stage ids ...]          (default: all 40)

For each stage: HSDRawViewer --export of models_dat/bg_data/bg_data_<2s>.dat (glb + textures) into
build/stage_views/<2s>/, then Blender (tools/blender/stage_preview.py ... auto) from Basic Arena's opening-camera
direction, pulled back to fit the stage, materials drawn as the game draws them (texture x vertex colour, unlit ones
unlit). Writes docs/images/stages/stage_<ss>.jpg (640 x 480). Stage 7 (RPG Test, a room with walls) is also drawn from
above (stage_07_top.jpg).
"""
import os
import subprocess
import sys
from pathlib import Path


G = Path(__file__).resolve().parents[1]
EXE = G / 'tools' / 'HSDLib' / 'HSDRawViewer' / 'bin' / 'Release' / 'net8.0-windows7.0' / 'HSDRawViewer.exe'
BLENDER = os.environ.get('BLENDER') or r'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe'
OUT = G / 'docs' / 'images' / 'stages'
EXTRA = {7: [('top', ['0', '34', '12', '0', '0', '2', '45', 'cull'])]}
# stages whose huge backdrop throws the auto framing off: Basic Arena's camera direction, pulled back 1.5x
CAMERA = {24: ['32', '20', '29', '0', '-1.4', '0', '41.54', 'cull']}


def render(src, out, args):
    r = subprocess.run([BLENDER, '-b', '--factory-startup', '--python', str(G / 'tools' / 'blender' / 'stage_preview.py'), '--',
                        str(src), str(out)] + args, capture_output=True, text=True)
    if r.returncode != 0 or not out.exists():
        sys.stderr.write(r.stdout[-2000:] + r.stderr[-2000:])
        raise SystemExit(f'render failed: {src}')


def main():
    ids = [int(x) for x in sys.argv[1:]] or list(range(40))
    OUT.mkdir(parents=True, exist_ok=True)
    for s in ids:
        e = f'{s * 2:03d}'
        d = G / 'build' / 'stage_views' / e
        glbs = d / f'bg_data_{e}'
        if not (glbs / 'scene_data_0.glb').exists():
            d.mkdir(parents=True, exist_ok=True)
            subprocess.run([str(EXE), '--export', str(d), 'glb', str(G / 'models_dat' / 'bg_data' / f'bg_data_{e}.dat')],
                           capture_output=True, check=True)
        views = [('', CAMERA.get(s, ['auto']))] + EXTRA.get(s, [])
        for tag, args in views:
            jpg = OUT / (f'stage_{s:02d}' + (f'_{tag}' if tag else '') + '.jpg')
            render(glbs, jpg, args)
            print(f'stage {s}: {jpg.relative_to(G)}')


if __name__ == '__main__':
    main()
