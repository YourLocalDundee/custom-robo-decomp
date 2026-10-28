"""Swap two robos completely (model, skeleton, joint roles, limb measurements; all 5 variants each).

usage: python -I tools/robo_swap.py <robo A> <robo B> [<clean.iso>] [--model-only]
  robo N = chara entries 5N..5N+4 (the game loads entry robo*5). Writes the mod folder mods/swap_robo<A>_robo<B>/
  and builds it with tools/make_mod.py: build/mods/swap_robo<A>_robo<B>/swap_robo<A>_robo<B>.iso + .crpatch.
  Also swaps the two per-robo tables cr_Robo_BuildModel reads by robo number (as main.dol patches in code/):
    0x8021B080 f32[robo]   size (scale)                 - without it a body keeps the slot's size
    0x8021B160 u16*[robo]  motion set (17 animation IDs) - without it a body keeps the slot's animations
  --model-only leaves those two with the robo number (the first community test did that: robo 7's body ran
  with robo 0's animations and size; only its own model animation, the charge, came along).
Needs models_dat/ and unpacked/ (from tools/from_scratch.ps1).
"""
import json, shutil, subprocess, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))  # python -I drops the script dir
import make_mod, robo_joints  # noqa: E402

G = Path(__file__).resolve().parent.parent
VARIANTS = 5
SCALE_TABLE, MOTION_TABLE = 0x8021B080, 0x8021B160  # cr_g_RoboScale, cr_g_RoboMotionSets


def main():
    if len(sys.argv) < 3: sys.exit(__doc__)
    a, b = int(sys.argv[1]), int(sys.argv[2])
    count = len(list((G / 'unpacked' / 'chara').glob('[0-9][0-9][0-9].bin')))
    if a == b or not all(0 <= r < count // VARIANTS for r in (a, b)):
        sys.exit(f'robo numbers must differ and be 0..{count // VARIANTS - 1}')
    name = f'swap_robo{a}_robo{b}'
    mod = G / 'mods' / name
    shutil.rmtree(mod, ignore_errors=True)
    (mod / 'models' / 'chara').mkdir(parents=True)
    (mod / 'mod.json').write_text(json.dumps({
        'name': name, 'author': 'robo_swap.py',
        'description': f'robo {a} (entries {a * 5:03d}-{a * 5 + 4:03d}) and robo {b} '
                       f'(entries {b * 5:03d}-{b * 5 + 4:03d}) swap models and skeletons, all variants'}, indent=2))
    for v in range(VARIANTS):
        for slot, src in ((a * 5 + v, b * 5 + v), (b * 5 + v, a * 5 + v)):
            shutil.copyfile(G / 'models_dat' / 'chara' / f'chara_{src:03d}.dat', mod / 'models' / 'chara' / f'chara_{slot:03d}.dat')
            spec = robo_joints.roles_json((G / 'unpacked' / 'chara' / f'{src:03d}.bin').read_bytes())
            spec['A_from'] = f'chara_{src:03d}'
            (mod / 'models' / 'chara' / f'chara_{slot:03d}.roles.json').write_text(json.dumps(spec, indent=2))
    print(f'{mod}: robo {a} <-> robo {b}, {2 * VARIANTS} models with roles')
    if '--model-only' not in sys.argv:
        dol = (G / 'Custom Robo extracted' / 'sys' / 'main.dol').read_bytes()
        lines = ['; per-robo tables read by robo number in cr_Robo_BuildModel (0x80074110), swapped with the bodies']
        for table, what in ((SCALE_TABLE, 'size (f32)'), (MOTION_TABLE, 'motion set pointer')):
            va, vb = (dol[make_mod.dol_offset(dol, table + 4 * r):][:4].hex() for r in (a, b))
            lines.append(f'{table + 4 * a:08X}: {vb} [was {va}] ; robo {a} {what} <- robo {b}')
            lines.append(f'{table + 4 * b:08X}: {va} [was {vb}] ; robo {b} {what} <- robo {a}')
        (mod / 'code').mkdir()
        (mod / 'code' / 'robo_tables.txt').write_text('\n'.join(lines) + '\n')
        print('  + size and motion set swapped (code/robo_tables.txt)')
    args = [x for x in sys.argv[3:] if not x.startswith('--')][:1]
    sys.exit(subprocess.call([sys.executable, '-I', str(G / 'tools' / 'make_mod.py'), str(mod)] + args))


if __name__ == '__main__':
    main()
