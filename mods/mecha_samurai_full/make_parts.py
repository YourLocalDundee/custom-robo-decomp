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
# Nov 12, 2026 (owner: make things "stand out better"): the parts were painted the same red as the armour and read
# as lumps of the body; they are black lacquer and gold (parts_palette.json: the AI's red / maroon painted gunmetal,
# its cream trim gold), against the red armour
LOOK = ['--look=paint', f'--paint={Path(__file__).resolve().parent / "parts_palette.json"}']
PARTS = {
    # Nov 12, 2026: no --shift any more. The Nov 11 shifts came from robo_preview.py placing parts by the role joint
    # and the mount offset alone; the game (fn_80039FAC) also turns each part root (robo +0x180C ...) and plays the
    # part's own animation, so those shifts pushed the parts further off (the victory gun floated above the fist). The
    # check is now robo_preview.py --gameparts (every part joint as the game drew it, from a Dolphin capture)
    # Nov 12 (Dolphin capture, tools: wincap.py + handspace.py): in the gun mesh joint's space the forearm runs along
    # +x from the elbow (0.2, 0.1, 0) to the wrist (1.9, 0.1, 0) (glTF axes, same in every battle frame); the fitted
    # matchlock's grip (the thin wrist of its stock, x 2.0-2.5) sat at y -2.3, z -0.33: moved up onto the wrist line
    # (+2.4 y, +0.33 z in glTF axes = Blender 0, -0.33, +2.4 for ai_part)
    'gun': ('weapon_gun', 'gun_3d_00001_.glb', [(0, 4, 450, 256, ['--shift=0,-0.33,2.4', '--main_only=1']),
                                                (1, 3, 450, 256, ['--shift=0,-0.33,2.4', '--main_only=1'])]),
    # Nov 12 (Dolphin capture): bomb root 1 (battle and Customize) - in its mesh joint's space the left forearm runs
    # along -x (elbow -0.2, wrist -1.9) and the fitted launcher sat at x +0.6..+3.7, off the far side of the elbow:
    # moved -3 x, -0.5 y (glTF) onto the forearm. Legs roots 4 / 5 (battle and Customize): the shin runs down y (knee
    # +0.5, ankle -3.1) at z 0.3 and the pack bulged 1-3.5 behind it and 1 to the side ("the balls on the back of his
    # calves"): 75% size, moved -1 x, -1.3 z (glTF) onto the calf
    # Nov 12 later (robo_preview.py rest-pose check; Dolphin captures: in battle the bomb's joints mostly sit in their
    # rest pose under the root): -3 x had pushed the drum off the far side of the elbow, behind the upper arm; the
    # check's grip shift, iterated twice (-3,0,-0.5 -> -0.05,0.155,-1.03), puts it on the middle of the forearm
    'bomb': ('weapon_bom', 'bomb_3d_00001_.glb', [(0, 4, 400, 256, []),
                                                  (1, 3, 400, 256, ['--shift=-0.05,0.155,-1.03'])]),
    # Nov 11 (robo_preview.py: posed robo + attachment check): the Sashimono poles stand up on the back (--long=x:
    # world up in the pod roots' drawn frame), seated 5.5 cm into the back (their bases only touched it in the stance and
    # floated 2-5 cm in the other motions as the back armour moves with the shoulders); the leg packs moved onto the shins by the measured gap x 1.1
    # Nov 12, 2026: the owner does not want the Sashimono poles on his back ("I don't really need those poles on his
    # back I told you already"): every pod root is left empty (the pods still fire; legs r1 / r3 are empty the same way)
    'pod': ('weapon_pod', 'pod_3d_00001_.glb', [(0, 4, 0, 0, []), (1, 4, 0, 0, []), (2, 3, 0, 0, []), (3, 3, 0, 0, [])]),
    # the stock legs are a calf thruster pack (roots 0 / 2 / 4 / 5) and an ankle clamp (1 / 3): klein_legs2 is the
    # samurai's calf thruster pack (the first try, a shin guard, was the wrong part and too thin to decimate)
    # Nov 12, 2026 (owner: "remove them like the back poles"): the calf packs swung out from the calves in every jump
    # (the leg part's thruster animation pivots at the knee); every leg root is left empty, the legs still work
    'legs': ('weapon_leg', 'legs2_3d_00001_.glb', [(0, 5, 0, 0, []), (2, 5, 0, 0, []), (4, 1, 0, 0, []),
                                                   (5, 1, 0, 0, []), (1, 4, 0, 0, []), (3, 4, 0, 0, [])]),
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
