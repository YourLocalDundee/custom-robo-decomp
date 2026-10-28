"""Swap two robos completely (model, skeleton, joint roles, limb measurements; all 5 variants each).

usage: python -I tools/robo_swap.py <robo A> <robo B> [<clean.iso>] [--model-only] [--keep-motion] [--size X]
  robo N = chara entries 5N..5N+4 (the game loads entry robo*5). Writes the mod folder mods/swap_robo<A>_robo<B>/
  and builds it with tools/make_mod.py: build/mods/swap_robo<A>_robo<B>/swap_robo<A>_robo<B>.iso + .crpatch.
  Also swaps the two per-robo tables cr_Chara_BuildModel reads by robo number (as main.dol patches in code/):
    0x8021B080 f32[number] Y offset (cr_g_CharaYOffset -> chara+0xE14; added to the character's Y position in FUN_8002fefc)
    0x8021B160 *[number]   motion set (cr_g_CharaMotionSets: 17 motions wait/walk/run/talk_*/battle_*; field characters)
  --model-only   leaves both tables with the robo number (first community test: robo 7's body ran with robo 0's
                 animations; only its own model animation, the charge, came along)
  --keep-motion  swaps the Y offset but leaves the motion sets with the robo number
  --size X       draws the swapped bodies X times bigger: sets the model's root joint scale to X. The game has no
                 per-robo size of its own (robo+0x3C0 is 1.0 for every robo, FUN_800ba488; every root joint is 1.0),
                 so a robo's size is in its vertices; scaling the root joint scales the whole model. Experimental.
Needs models_dat/ and unpacked/ (from tools/from_scratch.ps1).
"""
import json, shutil, struct, subprocess, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))  # python -I drops the script dir
import make_mod, robo_joints  # noqa: E402

G = Path(__file__).resolve().parent.parent
VARIANTS = 5
YOFFSET_TABLE, MOTION_TABLE = 0x8021B080, 0x8021B160  # cr_g_CharaYOffset, cr_g_CharaMotionSets (field characters, plan.md step 40)
DAT_DATA = 0x20       # a standard .dat's data block starts at 0x20; the entry's at 0x40 (crmodel.to_dat)
JOINT_SCALE = 0x20    # HSD_Joint: rotation +0x14, scale +0x20, translation +0x2C


def set_root_scale(dat, entry, size):
    """Root joint scale of a .dat made by crmodel.to_dat from entry, set to (size, size, size)."""
    root = robo_joints.joint_tree(entry)[0][0][0]           # root joint's offset in the entry
    at = DAT_DATA + (root - 0x40) + JOINT_SCALE
    if struct.unpack_from('>3f', dat, at) != struct.unpack_from('>3f', entry, root + JOINT_SCALE):
        sys.exit('root joint not where expected in the .dat')
    dat = bytearray(dat)
    struct.pack_into('>3f', dat, at, size, size, size)
    return bytes(dat)


def main():
    if len(sys.argv) < 3: sys.exit(__doc__)
    a, b = int(sys.argv[1]), int(sys.argv[2])
    count = len(list((G / 'unpacked' / 'chara').glob('[0-9][0-9][0-9].bin')))
    if a == b or not all(0 <= r < count // VARIANTS for r in (a, b)):
        sys.exit(f'robo numbers must differ and be 0..{count // VARIANTS - 1}')
    keep_motion = '--keep-motion' in sys.argv
    size = float(sys.argv[sys.argv.index('--size') + 1]) if '--size' in sys.argv else 1.0
    name = f'swap_robo{a}_robo{b}' + ('_keepmotion' if keep_motion else '') + (f'_size{size:g}' if size != 1.0 else '')
    mod = G / 'mods' / name
    shutil.rmtree(mod, ignore_errors=True)
    (mod / 'models' / 'chara').mkdir(parents=True)
    (mod / 'mod.json').write_text(json.dumps({
        'name': name, 'author': 'robo_swap.py',
        'description': f'robo {a} (entries {a * 5:03d}-{a * 5 + 4:03d}) and robo {b} '
                       f'(entries {b * 5:03d}-{b * 5 + 4:03d}) swap models and skeletons, all variants'
                       + (f', drawn {size:g}x bigger' if size != 1.0 else '')}, indent=2))
    for v in range(VARIANTS):
        for slot, src in ((a * 5 + v, b * 5 + v), (b * 5 + v, a * 5 + v)):
            entry = (G / 'unpacked' / 'chara' / f'{src:03d}.bin').read_bytes()
            dat = (G / 'models_dat' / 'chara' / f'chara_{src:03d}.dat').read_bytes()
            if size != 1.0: dat = set_root_scale(dat, entry, size)
            (mod / 'models' / 'chara' / f'chara_{slot:03d}.dat').write_bytes(dat)
            spec = robo_joints.roles_json(entry)
            spec['A_from'] = f'chara_{src:03d}'
            (mod / 'models' / 'chara' / f'chara_{slot:03d}.roles.json').write_text(json.dumps(spec, indent=2))
    print(f'{mod}: robo {a} <-> robo {b}, {2 * VARIANTS} models with roles' + (f', root scale {size:g}' if size != 1.0 else ''))
    if '--model-only' not in sys.argv:
        dol = (G / 'Custom Robo extracted' / 'sys' / 'main.dol').read_bytes()
        word = lambda t, r: dol[make_mod.dol_offset(dol, t + 4 * r):][:4]
        tables = [(YOFFSET_TABLE, 'Y offset')] + ([] if keep_motion else [(MOTION_TABLE, 'motion set pointer')])
        lines = ['; per-robo tables read by robo number in cr_Chara_BuildModel (0x80074110), swapped with the bodies']
        for t, what in tables:
            for r, src in ((a, b), (b, a)):
                lines.append(f'{t + 4 * r:08X}: {word(t, src).hex()} [was {word(t, r).hex()}] ; robo {r} {what} <- robo {src}')
        (mod / 'code').mkdir()
        (mod / 'code' / 'robo_tables.txt').write_text('\n'.join(lines) + '\n')
        print('  + Y offset' + ('' if keep_motion else ' and motion set') + ' swapped (code/robo_tables.txt)')
    args = [x for x in sys.argv[3:] if not x.startswith('--') and x != (sys.argv[sys.argv.index('--size') + 1] if '--size' in sys.argv else None)][:1]
    sys.exit(subprocess.call([sys.executable, '-I', str(G / 'tools' / 'make_mod.py'), str(mod)] + args))


if __name__ == '__main__':
    main()
