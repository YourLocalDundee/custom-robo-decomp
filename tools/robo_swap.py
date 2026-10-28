"""Swap two robos completely (model, skeleton, joint roles, limb measurements; all 5 variants each).

usage: python -I tools/robo_swap.py <robo A> <robo B> [<clean.iso>]
  robo N = chara entries 5N..5N+4 (the game loads entry robo*5). Writes the mod folder mods/swap_robo<A>_robo<B>/
  and builds it with tools/make_mod.py: build/mods/swap_robo<A>_robo<B>/swap_robo<A>_robo<B>.iso + .crpatch.
Needs models_dat/ and unpacked/ (from tools/from_scratch.ps1).
"""
import json, shutil, subprocess, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))  # python -I drops the script dir
import robo_joints  # noqa: E402

G = Path(__file__).resolve().parent.parent
VARIANTS = 5


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
    sys.exit(subprocess.call([sys.executable, '-I', str(G / 'tools' / 'make_mod.py'), str(mod)] + sys.argv[3:4]))


if __name__ == '__main__':
    main()
