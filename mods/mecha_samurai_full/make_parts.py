"""The mecha samurai's gun, bomb, pods and legs: AI models (D:\\ai\\Pictures\\mecha_samurai_parts, FLUX 3 Klein pictures
made from the samurai picture, then Pixal3D + TRELLIS.2) fitted onto every root of the stock part Ray 01 starts with
(Basic Gun 0, Standard Bomb 0, Standard Pod 0, Standard Legs 0) by tools/blender/ai_part.py, then imported root by
root over the stock file (HSDRawViewer --import --root N --replace).

  python -I mods/mecha_samurai_full/make_parts.py [gun bomb pod legs]

Roots and their mesh joints (docs/formats/model_entry.md "Part files"; root 0's material joint is the file's header[4]
path, the joint whose material the game sets up for the whole part): gun r0 JOBJ_4 (battle form 0) / r1 JOBJ_3 (form 1);
bomb r0 JOBJ_4 (Customize, story) / r1 JOBJ_3 (battle); pod r0 / r1 JOBJ_4 (battle, the two pods) / r2 / r3 JOBJ_3
(the other form); legs r0 / r2 JOBJ_5 (Customize) / r4 / r5 JOBJ_1 (battle shins); legs r1 / r3 (the small ankle
pieces) are left empty: the samurai's body has its own armoured feet.
"""
import os
import subprocess
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
EXE = G / 'tools' / 'HSDLib' / 'HSDRawViewer' / 'bin' / 'Release' / 'net8.0-windows7.0' / 'HSDRawViewer.exe'
BLENDER = os.environ.get('BLENDER') or r'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe'
AI = Path(r'D:\ai\Pictures\mecha_samurai_parts')
WORK = G / 'build' / 'samurai_full' / 'parts'
LIMIT = 0x40000      # the part slots of code/samurai.txt
# part: (category, AI model, [(root, joint, triangles, texture, extra ai_part options)])
# Nov 10 (docs/robo_look.md): stock-sized budgets (stock gun ~450 triangles per form, bomb ~400, the pod file ~900 over
# its four roots, the legs file ~530-690 over six) and the painted look; the first, detailed set (1,000 / 800 / 700 /
# 500 triangles, photo bake) is kept in build/mecha_samurai_full/models_detailed_nov9
LOOK = ['--look=paint']
PARTS = {
    'gun': ('weapon_gun', 'gun_3d_00001_.glb', [(0, 4, 450, 256, []), (1, 3, 450, 256, [])]),
    'bomb': ('weapon_bom', 'bomb_3d_00001_.glb', [(0, 4, 400, 256, []), (1, 3, 400, 256, [])]),
    'pod': ('weapon_pod', 'pod_3d_00001_.glb', [(0, 4, 300, 256, []), (1, 4, 300, 256, []), (2, 3, 200, 128, []),
                                                (3, 3, 200, 128, [])]),
    # the stock legs are a calf thruster pack (roots 0 / 2 / 4 / 5) and an ankle clamp (1 / 3): klein_legs2 is the
    # samurai's calf thruster pack (the first try, a shin guard, was the wrong part and too thin to decimate)
    'legs': ('weapon_leg', 'legs2_3d_00001_.glb', [(0, 5, 280, 128, []), (2, 5, 280, 128, []), (4, 1, 280, 128, []),
                                                   (5, 1, 280, 128, []), (1, 4, 0, 0, []), (3, 4, 0, 0, [])]),
}


def run(args):
    r = subprocess.run([str(a) for a in args], capture_output=True, text=True)
    if r.returncode != 0:
        sys.stdout.write(r.stdout[-3000:])
        sys.stderr.write(r.stderr[-3000:])
        raise SystemExit('failed: %s' % args[:3])
    return r.stdout


def empty_root(stock_glb, out_glb):
    """the stock root with every mesh removed (the importer's --replace then drops the template's meshes)"""
    script = WORK / 'empty_root.py'
    script.write_text('import bpy, sys\nsys.path.insert(0, r"%s")\nimport common\na = sys.argv[sys.argv.index("--") + 1:]\n'
                      'common.load(a[0])\nfor o in common.model_meshes():\n    bpy.data.objects.remove(o, do_unlink=True)\n'
                      'common.save(a[1])\n' % (G / 'tools' / 'blender'))
    run([BLENDER, '-b', '--factory-startup', '--python-exit-code', '1', '--python', script, '--', stock_glb, out_glb])


def main():
    want = sys.argv[1:] or list(PARTS)
    dst = MOD / 'models'
    for part in want:
        cat, model, roots = PARTS[part]
        stock_dat = G / 'models_dat' / cat / f'{cat}_000.dat'
        stock = WORK / 'stock' / f'{cat}_000'
        if not stock.exists():
            run([EXE, '--export', WORK / 'stock', 'glb', stock_dat])
        cur = stock_dat
        for root, joint, tris, tex, extra in roots:
            out = WORK / f'{part}_root{root}.glb'
            if tris:
                log = run([BLENDER, '-b', '--factory-startup', '--python-exit-code', '1', '--python',
                           G / 'tools' / 'blender' / 'ai_part.py', '--', stock / f'root{root}_joint.glb', AI / model, out,
                           f'--joint={joint}', f'--tris={tris}', f'--tex={tex}'] + LOOK + extra)
                print('\n'.join(l for l in log.splitlines() if l.startswith('ai_part:') and 'fitted' in l or 'triangles on' in l))
            else:
                empty_root(stock / f'root{root}_joint.glb', out)
            nxt = WORK / f'{part}_after_root{root}.dat'
            log = run([EXE, '--import', cur, out, nxt, '--root', root, '--replace', '--relayout'])
            print(f'{part} root {root}:', log.strip().splitlines()[-1][:160])
            cur = nxt
        (dst / cat).mkdir(parents=True, exist_ok=True)
        final = dst / cat / f'{cat}_000.dat'
        final.write_bytes(Path(cur).read_bytes())
        print(f'{final.relative_to(MOD)}: {final.stat().st_size} bytes (limit {LIMIT})')


if __name__ == '__main__':
    main()
