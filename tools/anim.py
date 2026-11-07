"""anim.py: export / import a robo's battle animations by body and slot name (victory, charge, ...).

A front end over tools/crmotion.py and tools/crmotion_rig.py (the motion format, the rig solve and the glTF
conversion; docs/formats/motion.md, motion_playback.md) and tools/battle_motions.py (which motion a body plays in
each slot; docs/battle_animations.md). Standard library only.

  python -I tools/anim.py slots --body 20
      the body's 38 slots: alias, motion ID and name, length, motion group, flags, other bodies sharing the motion

  python -I tools/anim.py export --body 20 --slot victory1 [--out DIR] [--mod MOD]
      DIR/<motion name>.glb: the body's model (models_export/chara/chara_<5N>/root0_joint.glb) with that motion as a
      glTF animation, 30 frames per second; open it in Blender. With --mod, the motion comes from the mod's
      files/motion.DBN (an earlier import) instead of the clean disc.

  python -I tools/anim.py import --body 20 --slot victory1 IN.glb --mod MOD [--anim NAME] [--tolerance DEG] [--keep-length]
      solves IN.glb's animation (on the body's JOBJ_n joints) back into the motion the slot plays and writes
      mods/MOD/files/motion.DBN (created from the clean disc's motion file the first time). The animation is stretched
      to the motion's fixed length (the game's timers map onto it: docs/battle_animations.md); --keep-length refuses
      a different length instead. Then build with `python -I tools/make_mod.py mods/MOD`.

Slot aliases: stance walk run walk_weapon_r180 run_weapon_r180 back_walk_weapon_r180 back_run_weapon_r180 weapon_jump
back_run_guard_r180 run_guard_r180 air_dash double_jump landing landing2 bomb_l90 bomb_r90 bomb_stance bomb_jump pod
pod_jump gun_stance gun_jump gun_jump_stiff gun_l90 gun_r90 gun_stiff fall_back fall_back_wall fall_front
fall_front_wall fall_magma getup lose victory1 victory2 charge change change2 (= slots 0..37), or a slot number.

Rules the tool keeps (docs/battle_animations.md, docs/formats/motion_playback.md "Authoring notes"):
- A motion's length is fixed by main.dol (cr_g_MotionInfo): win / lose 139 frames (last key 140), charge attack 60
  (three 20-frame phases), get-up 20. The import stretches to it.
- One motion ID can serve several bodies (the three sf bodies share sf_sf011_powerup; the Rahu share rr111's): the
  import changes it for all of them, and says so.
- The root tracks and joint 0 are kept from the motion being replaced (the game uses them beyond the pose).
- Every track ends on the motion's last frame (crmotion.build checks it).
- A motion group's float pool holds 16,384 floats: a long, detailed motion may need a larger --tolerance.
"""
import argparse
import os
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import battle_motions   # noqa: E402
import crmotion         # noqa: E402
import crmotion_rig     # noqa: E402

R = Path(__file__).resolve().parent.parent
CLEAN_BUNDLE = R / 'Custom Robo extracted' / 'files' / 'logo_read_us.dbn'
DOL = R / 'decomp' / 'orig' / 'GXCE01' / 'sys' / 'main.dol'
ALIASES = ('stance walk run walk_weapon_r180 run_weapon_r180 back_walk_weapon_r180 back_run_weapon_r180 weapon_jump '
           'back_run_guard_r180 run_guard_r180 air_dash double_jump landing landing2 bomb_l90 bomb_r90 bomb_stance '
           'bomb_jump pod pod_jump gun_stance gun_jump gun_jump_stiff gun_l90 gun_r90 gun_stiff fall_back '
           'fall_back_wall fall_front fall_front_wall fall_magma getup lose victory1 victory2 charge change '
           'change2').split()
assert len(ALIASES) == 38


def slot_of(s):
    if s.isdigit():
        n = int(s)
        if 0 <= n < 38:
            return n
    if s in ALIASES:
        return ALIASES.index(s)
    raise SystemExit('unknown slot %r: use 0..37 or one of %s' % (s, ', '.join(ALIASES)))


def motion_file(mod):
    """the motion file to read: the mod's (after an import) or the clean disc's entry 8 of logo_read_us.dbn"""
    if mod:
        p = R / 'mods' / mod / 'files' / 'motion.DBN'
        if p.exists():
            return p
    if not CLEAN_BUNDLE.exists():
        raise SystemExit('%s not found: run the one-time build (tools/from_scratch) first' % CLEAN_BUNDLE)
    return CLEAN_BUNDLE


def last_frame(data, mid):
    grp, e = crmotion.find_motion(data, mid)
    return grp, e, max((t['times'][-1] for t in e['tracks'] if t), default=0)


def body_paths(body):
    entry = 5 * body
    glb = R / 'models_export' / 'chara' / ('chara_%03d' % entry) / 'root0_joint.glb'
    binp = R / 'unpacked' / 'chara' / ('%03d.bin' % entry)
    for p in (glb, binp):
        if not p.exists():
            raise SystemExit('%s not found: run the one-time build (tools/from_scratch) first' % p)
    return binp, glb


def sharing(sets, mid, body):
    return [b for b, s in enumerate(sets) if b != body and any((r & 0x1FFF) == mid for r in s)]


def cmd_slots(a, sets, dol, bn):
    data = crmotion.parse(crmotion.read_motion(motion_file(a.mod)))
    print('body %d %s (model chara_%03d)' % (a.body, bn.get(a.body, ''), 5 * a.body))
    print('slot alias                  motion                         id      frames  group  flags  also used by bodies')
    for k, req in enumerate(sets[a.body]):
        mid = req & 0x1FFF
        try:
            grp, e, last = last_frame(data, mid)
            gi = data['groups'].index(grp)
        except SystemExit:
            last, gi = '?', '?'
        fl = battle_motions.flags(req)
        sh = sharing(sets, mid, a.body)
        print('%4d %-22s %-30s %#06x  %-6s  %-5s  %-5s  %s' % (k, ALIASES[k], dol.name(mid), mid, last, gi, fl,
                                                              ' '.join(map(str, sh[:12])) + (' ...' if len(sh) > 12 else '')))
    print('flags: T = also runs the body\'s own joint animation (transforming bodies); frames = the last key frame')


def cmd_export(a, sets, dol, bn):
    k = slot_of(a.slot)
    mid = sets[a.body][k] & 0x1FFF
    src = motion_file(a.mod)
    binp, glb = body_paths(a.body)
    out = Path(a.out or (R / 'build' / 'anim'))
    out.mkdir(parents=True, exist_ok=True)
    dst = out / ('%s_body%02d_%s.glb' % (dol.name(mid), a.body, ALIASES[k]))
    crmotion_rig.cmd_gltf(str(src), mid, str(binp), str(glb), str(dst), dol)
    print('slot %d (%s) of body %d %s: motion %#x %s from %s' % (k, ALIASES[k], a.body, bn.get(a.body, ''), mid,
                                                                dol.name(mid), src))


def cmd_import(a, sets, dol, bn):
    k = slot_of(a.slot)
    mid = sets[a.body][k] & 0x1FFF
    src = motion_file(a.mod)
    data = crmotion.parse(crmotion.read_motion(src))
    _, _, last = last_frame(data, mid)
    j, _ = crmotion_rig.read_glb(a.glb)
    anims = j.get('animations', [])
    if not anims:
        raise SystemExit('%s has no animation' % a.glb)
    sel = a.anim if a.anim is not None else '0'
    anim = next((x for x in anims if x.get('name') == sel), None) or anims[int(sel)]
    tmax = max(j['accessors'][s['input']].get('max', [0.0])[0] for s in anim['samplers'])
    if tmax <= 0:
        raise SystemExit('the animation has no length')
    own = int(round(tmax * 30.0))
    if own != last:
        if a.keep_length:
            raise SystemExit('the animation is %d frames at 30 fps; motion %s needs %d (drop --keep-length to stretch it)'
                             % (own, dol.name(mid), last))
        print('stretching: %d frames at 30 fps -> %d (the length %s has in the game)' % (own, last, dol.name(mid)))
    fps = last / tmax
    binp, _ = body_paths(a.body)
    dst = R / 'mods' / a.mod / 'files' / 'motion.DBN'
    dst.parent.mkdir(parents=True, exist_ok=True)
    if src == CLEAN_BUNDLE:
        tmp = dst.with_suffix('.clean.tmp')
        tmp.write_bytes(crmotion.boot_motion(CLEAN_BUNDLE.read_bytes()))
        src = tmp
    if a.tolerance is not None:
        crmotion_rig.ANGLE_TOL = a.tolerance
    crmotion_rig.cmd_import_gltf(a.glb, sel if a.anim is not None else 0, str(binp), str(src), mid, str(dst), dol, fps)
    if src.name.endswith('.clean.tmp'):
        src.unlink()
    sh = sharing(sets, mid, a.body)
    if sh:
        print('note: motion %s is also played by bodies %s (%s): the change shows on them too'
              % (dol.name(mid), ', '.join(map(str, sh)), ', '.join(bn.get(b, '') for b in sh)))
    mj = R / 'mods' / a.mod / 'mod.json'
    if not mj.exists():
        mj.write_text('{"name": "%s", "author": "", "description": "animation: %s"}\n' % (a.mod, dol.name(mid)))
    print('next: python -I tools/make_mod.py mods/%s' % a.mod)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    for name in ('slots', 'export', 'import'):
        p = sub.add_parser(name)
        p.add_argument('--body', type=int, required=True)
        p.add_argument('--mod')
        if name != 'slots':
            p.add_argument('--slot', required=True)
        if name == 'export':
            p.add_argument('--out')
        if name == 'import':
            p.add_argument('glb')
            p.add_argument('--anim')
            p.add_argument('--tolerance', type=float)
            p.add_argument('--keep-length', action='store_true')
    a = ap.parse_args()
    if a.cmd == 'import' and not a.mod:
        raise SystemExit('import needs --mod NAME (the mod folder to write)')
    if not 0 <= a.body < 34:
        raise SystemExit('body 0..33')
    dol = crmotion.load_dol(str(DOL))
    sets = battle_motions.sets(str(DOL))
    bn = battle_motions.body_names()
    {'slots': cmd_slots, 'export': cmd_export, 'import': cmd_import}[a.cmd](a, sets, dol, bn)


if __name__ == '__main__':
    main()
