"""The mecha samurai's own battle motions: all 38 slots of body 0's set, without changing any other robo.

  python -I mods/mecha_samurai_full/make_motion.py [TOL]

Writes files/motion.DBN (make_mod.py puts it into logo_read_us.dbn entry 8, the copy the game plays) and
code/motion.txt (body 0's motion set and the motion lengths in main.dol). How (docs/full_custom_robo.md, finding 3):
- Body 0 (Ray 01) shares 32 of its 38 slots' motions with other bodies (sf_ walk / run / jumps / bomb / pod / gun /
  falls / change, get-up with Splendor and Glory). Each shared motion is copied into a battle motion that no body
  plays (docs/data_unused_battle_motions.txt) in the same group (so its keys' floats stay valid), the copy's
  cr_g_MotionInfo entry (0x80205C94) points at the stock motion's tag list (length + frame events), and body 0's set
  (cr_g_BattleMotionSets 0x801C51B8, 38 u16) is pointed at the copy. Then the copy is restyled. The other bodies keep
  the stock motions.
- Body 0's own four (lose, both victories, the charge attack) are rewritten in place.
Style (IK points of the motion rig, docs/formats/motion_playback.md: point0 hips, 1 chest, 2 head, 3 off hand (A),
4 gun hand (B), 5 / 6 feet; +Y up, the robo faces -Z except in *_r180 motions): a low, wide samurai stance, the
chest upright and a little forward, the off hand on the hilt at the left hip. The gun hand is left to the game
(it aims it) and the root / joint 0 tracks are never touched (Atlas mod lessons, docs/MODDING.md).
Victories: (1) the fist raised high overhead, (2) a deep bow. Lose: down on one knee, head bowed. Charge attack
(three 20-frame phases): draw (crouch, both hands at the left hip), the iai lunge (a wide horizontal sweep of both
arms across the body while lunging), the follow-through held.
"""
import copy
import math
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
sys.path.insert(0, str(G / 'tools'))
import crmotion as c  # noqa: E402
import crmotion_rig as rig  # noqa: E402
sys.path.insert(0, str(MOD))
import samurai_moves as sm  # noqa: E402
FLOOR_JSON = MOD / 'floor_lift.json'
FLOOR = __import__('json').load(open(FLOOR_JSON)) if FLOOR_JSON.exists() else {}

TOL = float(sys.argv[1]) if len(sys.argv) > 1 else 0.008
TOL_ROT = 1.0     # degrees (Nov 12: our own rotation tracks)
SETS, BODY = 0x801C51B8, 0
# Nov 11, 2026: no extra crouch (was 0.07 lower, 0.05 wider): on the bulky samurai Ray 01's own stance already reads
# as a crouch, and in Customize (the game's matrices, robo_preview.py --gamemats) the extra drop and spread made a squat
# that flattened the front skirt plate into a sheet. Nov 12: lean 8 -> 4 degrees (the collar pulled to a point, owner:
# "like the bow of a ship")
LOW, SPREAD, LEAN = 0.0, 0.0, 4.0     # stance: hips lower, feet wider (each side), chest forward (degrees)
LEG = 0.86                               # a straight leg's IK target distance from the hip joint (Atlas notes)

src = G / 'Custom Robo extracted' / 'files' / c.BOOT_BUNDLE
data = c.parse(c.read_motion(src))
dol = c.load_dol(None)
group_of, entry = {}, {}
for gi, g in enumerate(data['groups']):
    for m in g['motions']:
        group_of[m['id'] & 0x7FFF] = gi
        entry[m['id'] & 0x7FFF] = m
nsets = 34
sets = [list(struct.unpack('>38H', dol.read(SETS + s * 0x4C, 0x4C))) for s in range(nsets)]
mine = sets[BODY]
used_by_others = {x & 0x1FFF for s in range(nsets) if s != BODY for x in sets[s]}
unused = []
for line in (G / 'docs' / 'data_unused_battle_motions.txt').read_text().splitlines():
    if line.startswith('0x'):
        unused.append(int(line.split()[0], 16))


def header_addr(mid):
    p = struct.unpack('>I', dol.read(c.INFO_ADDR + 4 * mid, 4))[0]
    return struct.unpack('>I', dol.read(p + 4, 4))[0]


ALIAS_OF_SLOT = ['stance', 'walk', 'run', 'walk_weapon_r180', 'run_weapon_r180', 'back_walk_weapon_r180',
                 'back_run_weapon_r180', 'weapon_jump', 'back_run_guard_r180', 'run_guard_r180', 'air_dash', 'double_jump',
                 'landing', 'landing2', 'bomb_l90', 'bomb_r90', 'bomb_stance', 'bomb_jump', 'pod', 'pod_jump', 'gun_stance',
                 'gun_jump', 'gun_jump_stiff', 'gun_l90', 'gun_r90', 'gun_stiff', 'fall_back', 'fall_back_wall',
                 'fall_front', 'fall_front_wall', 'fall_magma', 'getup', 'lose', 'victory1', 'victory2', 'charge',
                 'change', 'change2']

# ---- slot -> the motion the samurai plays (a copy for every shared one)
free = {g: [u for u in unused if group_of.get(u) == g] for g in range(3)}
target, code = {}, []
# Nov 12, 2026: group 0's float pool (14-bit index, shared with every robo's stock battle motions) has no room left
# for the per-frame arm roll (untwist); every group of the boot motion file is registered at boot
# (cr_MotionFile_Register, per motion ID), so these slots' copies go to unused 78-track IDs of groups 1 / 2 instead.
# A moved copy is sampled from its source group's pool and every track is re-encoded into its new group's pool
MOVE_SLOTS = set()   # {'fall_back', 'fall_front', ...} when group 0 runs out (untwist needed it; off with it)
SRC_GROUP = {}
for slot, x in enumerate(mine):
    mid = x & 0x1FFF
    if mid in target:
        continue
    if mid in used_by_others:
        g = group_of[mid]
        alt = [h for h in (1, 2) if h != g and free[h]
               and len(entry[free[h][0]]['tracks']) == len(entry[mid]['tracks'])]
        if ALIAS_OF_SLOT[slot] in MOVE_SLOTS and alt:
            t = free[alt[0]].pop(0)
            SRC_GROUP[t] = g
            target[mid] = t
            entry[t]['tracks'] = copy.deepcopy(entry[mid]['tracks'])
            continue
        if not free[g]:
            raise SystemExit(f'no unused motion left in group {g} for slot {slot}')
        t = free[g].pop(0)
        target[mid] = t
        entry[t]['tracks'] = copy.deepcopy(entry[mid]['tracks'])
    else:
        target[mid] = mid
new_set = [(x & ~0x1FFF) | target[x & 0x1FFF] for x in mine]
# the set and the copies' lengths as main.dol words
words = {}
for i, v in enumerate(new_set):
    a = SETS + BODY * 0x4C + i * 2
    wa = a & ~3
    words.setdefault(wa, bytearray(dol.read(wa, 4)))
    words[wa][a - wa:a - wa + 2] = struct.pack('>H', v)
# Nov 11, 2026: each copy's cr_g_MotionInfo entry now points at the STOCK motion's tag list (its header = length, and
# its frame events: effects, sounds). Patching only the copy's length kept the unused motion's own events: slot 0
# (idle, played in Customize) became ID 2 sf_back_run_guard, whose tag 6 at frame 25 spawns a battle-only effect
# (fn_8005C7F4 -> fn_80079214 with robo+0x333C0, never loaded in Customize) -> NULL jobj assert, the Customize freeze.
# The tag lists' headers are also shared between IDs, so patching them changed other bodies' motion lengths.
for mid, t in target.items():
    if t == mid:
        continue
    wa = c.INFO_ADDR + 4 * t
    words[wa] = bytearray(dol.read(c.INFO_ADDR + 4 * mid, 4))
for wa in sorted(words):
    old = struct.unpack('>I', dol.read(wa, 4))[0]
    new = struct.unpack('>I', bytes(words[wa]))[0]
    if new != old:
        code.append(f'{wa:08X}: {new:08X} [was {old:08X}]')


# ---- the styles
def facing(ch):
    return -1.0 if math.cos(math.radians(ch.get('root.roty', 0.0))) >= 0 else 1.0


def P(ch, k):
    return [ch[f'point{k}.{a}'] for a in 'xyz']


def tip(rel, deg, fz):
    """rel (from the hips) turned about X by deg, positive = forward (toward fz * Z)"""
    d = math.hypot(rel[1], rel[2]) or 0.3
    a = math.atan2(fz * rel[2], rel[1]) + math.radians(deg)
    return [rel[0], d * math.cos(a), fz * d * math.sin(a)]


def off_side(frames):
    return facing(frames[0])            # the off hand's side is the facing's sign (samurai_moves.side_of)


def lerp(a, b, t):
    return [x + (y - x) * t for x, y in zip(a, b)]


def smooth(t):
    t = max(0.0, min(1.0, t))
    return t * t * (3 - 2 * t)


def put(vals, k, p):
    for i, a in enumerate('xyz'):
        vals[f'point{k}.{a}'] = p[i]


def stance(frames, low=LOW, spread=SPREAD, lean=LEAN, hilt=0.75, hands=True, feet=True, gun_out=0.0):
    """the samurai stance over a stock motion"""
    side = off_side(frames)
    out = []
    for ch in frames:
        fz = facing(ch)
        p0 = P(ch, 0)
        n0 = [p0[0], p0[1] - low, p0[2]]
        v = {}
        put(v, 0, n0)
        for k in (1, 2):
            rel = [a - b for a, b in zip(P(ch, k), p0)]
            n = tip(rel, lean, fz)
            put(v, k, [n0[i] + n[i] for i in range(3)])
        if feet:
            for f in (5, 6):
                q = P(ch, f)
                sx = 1.0 if q[0] - p0[0] > 0 else -1.0
                put(v, f, [q[0] + sx * spread, q[1], q[2]])
        if hands and hilt > 0:
            h = [n0[0] + side * 0.20, n0[1] + 0.04, n0[2] + fz * 0.10]
            put(v, 3, lerp(P(ch, 3), h, hilt))
        if gun_out:
            # Nov 12 (owner: "his right arm is consistently clipping through his right leg"): the gun hand hangs
            # beside the bulky skirt; held out from it
            q = P(ch, 4)
            put(v, 4, [q[0] - side * gun_out, q[1] + gun_out * 0.3, q[2]])
        out.append(v)
    return out


def frozen(frames):
    """the stock motion held at its first frame (all IK points): the victories are driven entirely by our pose, so
    the stock win motion's own crouch / jump does not fight it (Nov 12)"""
    keys = [f'point{k}.{a}' for k in range(7) for a in 'xyz']
    return [dict(ch, **{k_: frames[0][k_] for k_ in keys if k_ in frames[0]}) for ch in frames]


def heisman(frames):
    frames = frozen(frames)
    """victory 1 (Nov 12, owner: football celebrations): the Heisman - the gun-side knee up, the off arm stiff-armed
    straight out in front, the gun tucked at the chest like the ball; held, eased out at the end"""
    side = off_side(frames)
    st = stance(frames, low=0.02, spread=0.0, lean=6.0, hands=False)
    last = len(frames) - 1
    for t, (ch, v) in enumerate(zip(frames, st)):
        fz = facing(ch)
        k = smooth((t - 4) / 14)                    # held to the end (the game stops on the last frame)
        n0 = [v['point0.x'], v['point0.y'], v['point0.z']]
        put(v, 3, lerp(P(ch, 3), [n0[0] + side * 0.32, n0[1] + 0.38, n0[2] + fz * 0.70], k))   # off the chest plate (B10: wider)
        # the tucked gun out of the bulky chest plate (deform_check: arm b inside the torso)
        put(v, 4, lerp(P(ch, 4), [n0[0] - side * 0.40, n0[1] + 0.22, n0[2] + fz * 0.40], k))   # B10: further out
        for f in (5, 6):
            q = P(ch, f)
            gun_side = (q[0] - P(ch, 0)[0]) * side < 0
            if gun_side:
                put(v, f, lerp(q, [q[0], q[1] + 0.24, q[2] + fz * 0.16], k))   # knee a bit lower: the side skirt tore
    return st


def dust_off(frames):
    frames = frozen(frames)
    """victory 2 (Nov 12, owner: "looking around and dusting his dirt off his shoulders like it ain't nothing"):
    relaxed stance; looks left, then right; the off hand brushes the gun-side shoulder twice, then the gun hand the
    off-side shoulder twice; chin up at the end"""
    side = off_side(frames)
    st = stance(frames, low=0.0, spread=0.02, lean=0.0, hands=False)
    for t, (ch, v) in enumerate(zip(frames, st)):
        fz = facing(ch)
        n0 = [v['point0.x'], v['point0.y'], v['point0.z']]
        c1 = [v['point1.x'], v['point1.y'], v['point1.z']]
        h2 = [v['point2.x'], v['point2.y'], v['point2.z']]
        look = math.sin(math.pi * 2 * smooth(t / 40.0)) * (1 - smooth((t - 34) / 8)) * 0.07
        chin = 0.025 * smooth((t - 110) / 14)
        put(v, 2, [h2[0] + look, h2[1] + chin, h2[2] - fz * chin * 0.5])
        rest3 = [n0[0] + side * 0.20, n0[1] + 0.02, n0[2] + fz * 0.06]
        rest4 = [n0[0] - side * 0.21, n0[1] + 0.02, n0[2] + fz * 0.06]
        def brush(t0, sh_sign, hand_rest):
            k = smooth((t - t0) / 8) * (1 - smooth((t - (t0 + 34)) / 8))
            sw = math.sin(math.pi * 2 * max(0.0, (t - t0 - 8)) / 13.0) if t > t0 + 8 else 0.0
            # outside the shoulder plate (deform_check: the hands went into the chest / shoulder armour)
            shoulder = [c1[0] + sh_sign * 0.22 + sh_sign * 0.06 * max(sw, 0), c1[1] + 0.08, c1[2] + fz * 0.46]
            return lerp(hand_rest, shoulder, k)
        put(v, 3, lerp(P(ch, 3), brush(44, -side, rest3), smooth(t / 10)))
        put(v, 4, lerp(P(ch, 4), brush(86, side, rest4), smooth(t / 10)))
    return st


def head_check(frames):
    """get-up (21 frames, the game's own length): a quick look left and right while rising"""
    st = stance(frames, low=0.04, hilt=0.5)
    n = max(len(frames) - 1, 1)
    for t, v in enumerate(st):
        if 'point2.x' in v:
            v['point2.x'] += 0.06 * math.sin(math.pi * 2 * t / n)
        else:
            q = P(frames[t], 2)
            put(v, 2, [q[0] + 0.06 * math.sin(math.pi * 2 * t / n), q[1], q[2]])
    return st


def air_dash(frames):
    """air dash (Nov 12, owner: its own animation): a hard forward lean, both arms swept back, legs trailing"""
    st = stance(frames, low=0.0, spread=0.0, lean=24.0, hands=False)
    n = max(len(frames) - 1, 1)
    for t, (ch, v) in enumerate(zip(frames, st)):
        fz = facing(ch)
        k = smooth(t / max(n * 0.3, 1))
        n0 = [v['point0.x'], v['point0.y'], v['point0.z']]
        for kk, sx in ((3, 1), (4, -1)):
            q = P(ch, kk)
            sx = 1.0 if q[0] - n0[0] > 0 else -1.0
            put(v, kk, lerp(q, [n0[0] + sx * 0.24, n0[1] + 0.12, n0[2] - fz * 0.30], k))
        for f in (5, 6):
            q = P(ch, f)
            put(v, f, lerp(q, [q[0], q[1] + 0.10, q[2] - fz * 0.22], k))
    return st


def fist_high(frames):
    """victory 1: wide stance, the gun fist driven up overhead and held, the off hand on the hilt"""
    st = stance(frames, low=0.09, spread=0.08, lean=0.0)
    side = off_side(frames)
    for t, (ch, v) in enumerate(zip(frames, st)):
        k = smooth((t - 6) / 24)
        n0 = [v['point0.x'], v['point0.y'], v['point0.z']]
        up = [n0[0] - side * 0.10, n0[1] + 1.05, n0[2]]
        put(v, 4, lerp(P(ch, 4), up, k))
        put(v, 3, lerp(P(ch, 3), [n0[0] + side * 0.20, n0[1] + 0.04, n0[2] + facing(ch) * 0.10], k))
    return st


def bow(frames):
    """victory 2: feet together, a deep bow (chest and head 45 degrees forward), hands down the thighs; up again at
    the end"""
    last = len(frames) - 1
    out = []
    for t, ch in enumerate(frames):
        fz = facing(ch)
        p0 = P(ch, 0)
        k = smooth((t - 10) / 30) * (1 - smooth((t - (last - 30)) / 25))
        v = {}
        for kk in (1, 2):
            rel = [a - b for a, b in zip(P(ch, kk), p0)]
            n = tip(rel, 32 * k + (12 * k if kk == 2 else 0), fz)
            put(v, kk, [p0[i] + n[i] for i in range(3)])
        for kk, sx in ((3, -1), (4, 1)):
            down = [p0[0] + sx * 0.17, p0[1] - 0.30, p0[2] + fz * 0.10 * k]
            put(v, kk, lerp(P(ch, kk), down, smooth((t - 4) / 16)))
        for f, sx in ((5, -1), (6, 1)):
            q = P(ch, f)
            put(v, f, lerp(q, [p0[0] + sx * 0.10, q[1], p0[2]], smooth((t - 4) / 16)))
        out.append(v)
    return out


def kneel(frames):
    """lose: down on one knee (hips low, the off-side foot back), head bowed, hands on the thighs"""
    side = off_side(frames)
    ground = min(min(ch['point5.y'], ch['point6.y']) for ch in frames)
    out = []
    for t, ch in enumerate(frames):
        fz = facing(ch)
        p0 = P(ch, 0)
        k = smooth((t - 8) / 30)
        n0 = [p0[0], p0[1] - 0.14 * k, p0[2]]          # 0.26 put the knee 6 cm into the floor (deform_check)
        v = {}
        put(v, 0, n0)
        for kk in (1, 2):
            rel = [a - b for a, b in zip(P(ch, kk), p0)]
            n = tip(rel, (12 if kk == 1 else 40) * k, fz)
            put(v, kk, [n0[i] + n[i] for i in range(3)])
        for f in (5, 6):
            q = P(ch, f)
            back = (q[0] - p0[0]) * side > 0         # the off-hand side's foot goes back: that knee goes down
            want = [q[0], ground + 0.05, p0[2] - fz * 0.45] if back else [q[0], ground, p0[2] + fz * 0.30]   # back foot up: the shin on the floor, not in it
            put(v, f, lerp(q, want, k))
        for kk, sx in ((3, side), (4, -side)):
            put(v, kk, lerp(P(ch, kk), [n0[0] + sx * 0.24, n0[1] + 0.04, n0[2] + fz * 0.34], k))   # off the thighs (deform_check)
        out.append(v)
    return out


def iai(frames):
    """charge attack, three 20-frame blocks (the game stretches each over its phase): 0 draw, 1 lunge and sweep,
    2 follow-through"""
    side = off_side(frames)
    out = []
    for t, ch in enumerate(frames):
        fz = facing(ch)
        p0 = P(ch, 0)
        ph = t / 20.0
        crouch = smooth(ph) * (1 - smooth(ph - 2.2))
        lunge = smooth((t - 20) / 8)
        n0 = [p0[0], p0[1] - 0.16 * crouch, p0[2]]
        v = {}
        put(v, 0, n0)
        lean = 6 * smooth(ph) + 12 * lunge * (1 - 0.6 * smooth((t - 44) / 14))   # less lean: the chest folded over the arms (deform_check)
        for kk in (1, 2):
            rel = [a - b for a, b in zip(P(ch, kk), p0)]
            n = tip(rel, lean, fz)
            put(v, kk, [n0[i] + n[i] for i in range(3)])
        hilt = [n0[0] + side * 0.26, n0[1] + 0.02, n0[2] + fz * 0.46]   # well in front of the hip plates: the gun arm reaches in
        # front of the chest, not across it (B10 sweep: arm b in the torso at the grab)
        # Nov 12 (deform_check: the two-handed sweep crossed the off arm through the chest): a real draw (nukitsuke) -
        # the gun hand reaches across to the hilt at the off hip and draws out in a forward arc to its own side at
        # chest height; the off hand stays at the hip, pulling the scabbard back (saya-biki)
        sw = smooth((t - 20) / 14)
        grab = smooth(ph * 1.5)
        end = [n0[0] - side * 0.62, n0[1] + 0.36, n0[2] + fz * 0.40]
        arc = lerp(hilt, end, sw)
        arc[2] += fz * 0.32 * math.sin(math.pi * sw)          # the blade's path bulges forward
        put(v, 4, lerp(P(ch, 4), arc, grab))
        put(v, 3, lerp(P(ch, 3), [n0[0] + side * 0.34, n0[1] + 0.02, n0[2] + fz * (0.20 - 0.14 * sw)], grab))
        for f in (5, 6):
            q = P(ch, f)
            sx = 1.0 if q[0] - p0[0] > 0 else -1.0
            front = (sx == -side)
            dz = (0.35 if front else -0.25) * lunge
            put(v, f, [q[0] + sx * 0.08 * smooth(ph), q[1], q[2] + fz * dz])
        out.append(v)
    return out


CLEAR = 0.32        # a hand's least distance from the body's centre line (hips -> chest)


def clear_body(per, frames):
    """Nov 12, 2026 (tools/deform_sweep.py: arms inside the torso in the turns, the falls, the charge, the victories;
    the stock motions were made for Ray 01's slim chest): each hand (points 3 / 4) is pushed out to CLEAR from the
    hips -> chest line at its height, sideways and forward only (its height is kept)"""
    for v, ch in zip(per, frames):
        def pt(k):
            return [v[f'point{k}.{a}'] if f'point{k}.{a}' in v else ch[f'point{k}.{a}'] for a in 'xyz']
        h0, c1 = pt(0), pt(1)
        for k in (3, 4):
            q = pt(k)
            t = 0.0 if c1[1] == h0[1] else max(0.0, min(1.0, (q[1] - h0[1]) / (c1[1] - h0[1])))
            ax = [h0[0] + (c1[0] - h0[0]) * t, h0[2] + (c1[2] - h0[2]) * t]
            dx, dz = q[0] - ax[0], q[2] - ax[1]
            d = math.hypot(dx, dz)
            if d < CLEAR and q[1] > h0[1] - 0.35:
                if d < 1e-4:
                    dx, dz, d = 0.0, facing(ch), 1.0
                put(v, k, [ax[0] + dx / d * CLEAR, q[1], ax[1] + dz / d * CLEAR])
    return per


# ---- untwist (Nov 12, 2026): the upper arms' roll, solved per frame against the body's bind
# (tools/blender/rebind.py binds the body in the motions' own pose; deform sweep: the Heisman's stiff arm twisted
# 73 degrees about its own axis on top of a 124 degree raise, which wrung the shoulder band flat - 25k squashed
# triangles). In the game's pose code (crmotion_rig.pose = cr_Chara_PoseLimbs / LimbA) the upper arm frame is
# shoulder x R(joint6 / joint11) and then aimed at the hand target by a yaw and a pitch, so the roll left about the
# arm comes from that joint's rotation; the hand reaches its target whatever the roll (the IK bends in the aimed
# plane), only the elbow's direction and the twist change. Per frame the x rotation (Rz Ry Rx: the last, the arm's
# own axis before the aim) is solved for zero twist of the upper arm against the shoulder, measured from the bind
# (mods/mecha_samurai_full/bind_roles.json, tools/bind_roles.py), then smoothed over time.
BIND_JSON = MOD / 'bind_roles.json'
# OFF (Nov 12 evening, measured): untwisting worked (the Heisman's off arm 73 -> 1 degree) but its squash stayed (the
# 124 degree raise is the cause), and the freed roll swung the gun arm's elbow into worse places: stretch +13%,
# clipping +38% over 20 motions. Kept for reference; UNTWIST = True turns it back on
UNTWIST = False
BIND = __import__('json').load(open(BIND_JSON)) if (UNTWIST and BIND_JSON.exists()) else None
_ENTRY = G / 'unpacked' / 'chara' / '000.bin'
LIMB = rig.robo_inputs(_ENTRY.read_bytes())[0] if BIND else None
TAB = rig.Tables(dol) if BIND else None
ARMS = (('joint6', 3, 4, 'arm_a_shoulder', 'arm_a_upper'), ('joint11', 7, 8, 'arm_b_shoulder', 'arm_b_upper'))


def _r3(m):                  # crmotion_rig.M (3x4 row-major) or a 4x4 list -> 3x3 rows
    if isinstance(m, list):
        return [m[0][:3], m[1][:3], m[2][:3]]
    a = m.m
    return [a[0:3], a[4:7], a[8:11]]


def _mul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(3)) for j in range(3)] for i in range(3)]


def _T(a):
    return [[a[j][i] for j in range(3)] for i in range(3)]


def _mv(a, v):
    return [sum(a[i][k] * v[k] for k in range(3)) for i in range(3)]


def _cross(a, b):
    return [a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]]


def _dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def _unit(a):
    n = math.sqrt(_dot(a, a)) or 1.0
    return [x / n for x in a]


def _rotate(v, axis, ang):   # Rodrigues
    c, s_ = math.cos(ang), math.sin(ang)
    k = _unit(axis)
    kv = _cross(k, v)
    kd = _dot(k, v)
    return [v[i] * c + kv[i] * s_ + k[i] * kd * (1 - c) for i in range(3)]


def twist_deg(S, si_sh, si_up, r_sh, r_up):
    """signed twist (degrees) of the upper arm about its own axis, relative to the shoulder, from the bind"""
    Bs, Bu = _r3(BIND[r_sh]), _r3(BIND[r_up])
    Ds = _mul(_r3(S[si_sh]), _T(Bs))
    Du = _mul(_r3(S[si_up]), _T(Bu))
    Q = _mul(_T(Ds), Du)                       # the upper arm's turn in the shoulder's bind frame
    a = _unit([Bu[0][0], Bu[1][0], Bu[2][0]])  # the arm's axis: the bind frame's x (the aim axis)
    a2 = _unit(_mv(Q, a))
    u = _unit(_cross(a, [0.0, 0.0, 1.0] if abs(a[2]) < 0.9 else [0.0, 1.0, 0.0]))
    ax = _cross(a, a2)
    sw = _rotate(u, ax, math.atan2(math.sqrt(_dot(ax, ax)), _dot(a, a2))) if _dot(ax, ax) > 1e-12 else u
    qu = _mv(Q, u)
    return math.degrees(math.atan2(_dot(_cross(sw, qu), a2), _dot(sw, qu)))


UNTWIST_RANGE = 120.0       # degrees either side of the motion's own roll
UNTWIST_MAX = 1.0           # 1.0: the whole twist; less leaves some
UNTWIST_MIN = 15.0          # degrees: motions twisting less are left as they are


UNTWISTED = []


def untwist(per, frames):
    if not BIND:
        return per
    for jn, si_sh, si_up, r_sh, r_up in ARMS:
        key = f'{jn}.rotx'
        # only where it matters (the float pool is nearly full): a motion whose arm twists past UNTWIST_MIN somewhere
        worst = 0.0
        for v, ch in list(zip(per, frames))[::3]:
            full = dict(ch)
            full.update(v)
            S, _ = rig.pose(full, LIMB, TAB)
            worst = max(worst, abs(twist_deg(S, si_sh, si_up, r_sh, r_up)))
        if worst < UNTWIST_MIN:
            continue
        print(f'untwist: {jn} twisted up to {worst:.0f} degrees: solved per frame')
        sol, prev = [], None
        for v, ch in zip(per, frames):
            full = dict(ch)
            full.update(v)
            x0 = full.get(key, 0.0)

            def f(x):
                full[key] = x
                S, _ = rig.pose(full, LIMB, TAB)
                return twist_deg(S, si_sh, si_up, r_sh, r_up)
            # zero crossings on a 10-degree scan where the twist is continuous (the aim's yaw / pitch jumps near
            # its poles), refined by bisection; the root nearest the last frame's (else the motion's own roll)
            xs = [x0 + d for d in range(-int(UNTWIST_RANGE), int(UNTWIST_RANGE) + 1, 10)]
            ts = [f(x) for x in xs]
            roots = []
            for k in range(len(xs) - 1):
                a_, b_ = ts[k], ts[k + 1]
                if a_ == 0 or (a_ < 0) != (b_ < 0):
                    if abs(a_ - b_) > 60:
                        continue
                    lo, hi, tl = xs[k], xs[k + 1], a_
                    for _ in range(14):
                        mid = 0.5 * (lo + hi)
                        tm = f(mid)
                        if (tm < 0) == (tl < 0):
                            lo, tl = mid, tm
                        else:
                            hi = mid
                    roots.append(0.5 * (lo + hi))
            ref = prev if prev is not None else x0
            if roots:
                x = min(roots, key=lambda r: abs(r - ref))
            else:
                x = min(zip(xs, ts), key=lambda p_: abs(p_[1]))[0]
            x = x0 + (x - x0) * UNTWIST_MAX
            sol.append(x)
            prev = x
        sm_ = [sum(sol[max(0, k - 2):k + 3]) / len(sol[max(0, k - 2):k + 3]) for k in range(len(sol))]
        for v, x in zip(per, sm_):
            v[key] = x
    return per


ALIAS = ['stance', 'walk', 'run', 'walk_weapon_r180', 'run_weapon_r180', 'back_walk_weapon_r180', 'back_run_weapon_r180',
         'weapon_jump', 'back_run_guard_r180', 'run_guard_r180', 'air_dash', 'double_jump', 'landing', 'landing2',
         'bomb_l90', 'bomb_r90', 'bomb_stance', 'bomb_jump', 'pod', 'pod_jump', 'gun_stance', 'gun_jump',
         'gun_jump_stiff', 'gun_l90', 'gun_r90', 'gun_stiff', 'fall_back', 'fall_back_wall', 'fall_front',
         'fall_front_wall', 'fall_magma', 'getup', 'lose', 'victory1', 'victory2', 'charge', 'change', 'change2']


def style_for(slot):
    """Nov 12, 2026 (owner: "we're not using any of Ray's ... we need all of his"): every slot is generated by
    samurai_moves.py from scratch; the stock motion only gives its length, root tracks, the hips' travel and (in
    locomotion) where the feet are planted. The point-only styles above (victories, lose, charge, get-up) run on
    our own standing pose instead of a stock frame, and every slot gets our rotation joints."""
    name = ALIAS[slot]

    def on_stand(fn, height=0.98):
        def f(fr):
            base = sm.stand(fr, height)
            merged = [dict(ch, **v) for ch, v in zip(fr, base)]
            pts = fn(merged)
            return [dict(b, **p) for b, p in zip(base, pts)]
        return f
    if slot == 0:
        return lambda fr: sm.stand(fr, 0.98)
    if name in ('walk', 'walk_weapon_r180', 'back_walk_weapon_r180'):
        return lambda fr: sm.locomotion(fr, 1.06, lift=0.16, lean=6.0, arm_swing=0.10, bob=0.035, twist=8.0)
    if name in ('run', 'run_weapon_r180', 'back_run_weapon_r180', 'back_run_guard_r180', 'run_guard_r180'):
        return lambda fr: sm.locomotion(fr, 1.00, lift=0.30, lean=14.0, arm_swing=0.18, bob=0.05, twist=16.0, run=True)
    if name == 'air_dash':
        return lambda fr: sm.air(fr, 1.02, tuck=0.10, trail=0.30, lean=24.0, arms_back=0.32, spread=0.10)
    if name in ('weapon_jump', 'double_jump', 'bomb_jump', 'pod_jump', 'gun_jump', 'gun_jump_stiff'):
        return lambda fr: sm.air(fr, 1.02, tuck=0.24, trail=0.12, lean=6.0)
    if name in ('landing', 'landing2'):
        return lambda fr: sm.action(fr, 0.98, low=0.10, lean=10.0)
    if name in ('bomb_l90', 'bomb_r90', 'bomb_stance'):
        return lambda fr: sm.action(fr, 0.98, off_reach=(0.30, 0.30, 0.50), lean=6.0)
    if name == 'pod':
        return lambda fr: sm.action(fr, 0.98, off_reach=(0.34, 0.14, 0.26), gun_reach=(0.38, 0.10, 0.20), lean=2.0)
    if name in ('gun_stance', 'gun_l90', 'gun_r90', 'gun_stiff'):
        return lambda fr: sm.action(fr, 0.98, gun_reach=(0.22, 0.32, 0.56), lean=6.0, low=0.03)
    if name in ('fall_back', 'fall_back_wall'):
        return lambda fr: sm.fall(fr, 0.98, back=True)
    if name in ('fall_front', 'fall_front_wall', 'fall_magma'):
        return lambda fr: sm.fall(fr, 0.98, back=False)
    if name == 'getup':
        return on_stand(head_check)
    if name == 'lose':
        return on_stand(kneel)
    if name == 'victory1':
        return on_stand(heisman)
    if name == 'victory2':
        return on_stand(dust_off)
    if name == 'charge':
        return on_stand(iai)
    return lambda fr: sm.stand(fr, 0.98, guard=False)       # change

done, pending_by_group = [], {}
styled = set()
for slot, x in enumerate(new_set):
    mid = x & 0x1FFF
    if mid in styled:
        continue
    styled.add(mid)
    m = entry[mid]
    gi = group_of[mid]
    moved = mid in SRC_GROUP
    roles = c.track_roles(len(m['tracks']))
    last = max(t['times'][-1] for t in m['tracks'] if t)
    frames = [c.sample(m, data['groups'][SRC_GROUP.get(mid, gi)]['floats'], f) for f in range(last + 1)]
    per = untwist(clear_body(style_for(slot)(frames), frames), frames)
    if BIND:
        UNTWISTED.append(ALIAS[slot])
    # the floor (Nov 12, tools/floor_lift.py from a deform sweep): the whole body lifted where its armour sank through
    lift = FLOOR.get(ALIAS[slot], {})
    # whole-body lift: the root's translation (root.posy, world) where the motion has root channels (78 tracks), set
    # on every frame since a track is only taken whole; otherwise every IK point's height on the lifted frames
    for f_, v in enumerate(per):
        dy = lift.get(str(f_), 0.0)
        if lift and 'root.posy' in roles:
            v['root.posy'] = v.get('root.posy', frames[f_].get('root.posy', 0.0)) + dy
        elif dy:
            for k in range(7):
                key = f'point{k}.y'
                v[key] = (v[key] if key in v else frames[f_][key]) + dy
    want = {}
    if moved:                           # every track that has keys, re-encoded into the new group's pool
        for ti_, tr_ in enumerate(m['tracks']):
            if tr_:
                want[roles[ti_]] = [v.get(roles[ti_], ch[roles[ti_]]) for v, ch in zip(per, frames)]
    for v in per:
        for k, val in v.items():
            if moved:
                continue
            want.setdefault(k, []).append(val)
    for v_i, v in enumerate(per) if moved else ():
        for k, val in v.items():
            if k not in want:
                want.setdefault(k, [None] * (last + 1))[v_i] = val
    for k, vals in want.items():
        if len(vals) != last + 1:
            continue
        ti = roles.index(k)
        # rotation tracks are degrees; per point (Nov 12, the float pool): the feet tight (a planted foot must not
        # wobble), the hips middling, chest / head / hands loose
        tol = TOL_ROT if '.rot' in k else {'point5': 0.008, 'point6': 0.008, 'point0': 0.015}.get(k.split('.')[0], 0.03)
        if k in ('joint6.rotx', 'joint11.rotx'):
            tol = 2.5                   # the solved arm roll (untwist): smooth, coarse keys (the float pool)
        if any(x_ is None for x_ in vals):
            continue
        if not moved and max(abs(a - fr.get(k, 0.0)) for a, fr in zip(vals, frames)) < tol:
            continue                    # the style left this track as it was: keep the stock keys (no new floats)
        keys = rig.reduce_keys(vals, tol)
        # Nov 12: our own motions replace every track, and group 0's float pool (14-bit index, shared with every other
        # robo's stock motions) overflowed; values and tangents on a coarse grid repeat, and the pool keeps one copy
        qv, qt = (1.0, 2.0) if '.rot' in k else (0.004, 0.04)   # rotations in 1-degree steps (Nov 12: room for untwist)
        if k in ('joint6.rotx', 'joint11.rotx'):
            qv, qt = 2.0, 4.0
        for key in keys:
            key['value'] = round(key['value'] / qv) * qv
            if key['kind'] == 'smooth':
                tq = round(key['in'] / qt) * qt
                if abs(tq) < 1e-9:
                    key['kind'] = 'flat'
                    key.pop('in', None)
                    key.pop('out', None)
                else:
                    key['in'] = key['out'] = tq
        pending_by_group.setdefault(gi, []).append((m, ti, keys))
        m['tracks'][ti] = None
    done.append(f'slot {slot:2d} {ALIAS[slot]:22s} {dol.name(mine[slot] & 0x1FFF):28s} -> {dol.name(m["id"]):28s} ({last + 1} frames)')
for gi, pending in pending_by_group.items():
    g = data['groups'][gi]
    rig.compact_pool(g)
    base_n = len(g['floats'])
    need = sum(len(keys) for _, _, keys in pending)
    print(f'group {gi}: {base_n} floats kept after the compaction, {len(pending)} new tracks, {need} new keys')
    pool = {}
    for w in (2, 1, 3):
        for i in range(len(g['floats']) - w + 1):
            pool.setdefault(tuple(g['floats'][i:i + w]), i)
    for m, ti, keys in pending:
        if not keys:
            continue
        try:
            wds = [c.encode_key(k, g['floats'], pool) for k in keys]
        except ValueError:
            raise SystemExit(f'group {gi}: float pool full ({len(g["floats"])}); raise TOL (now {TOL})')
        m['tracks'][ti] = {'times': [k['frame'] for k in keys], 'words': wds}
    print(f'group {gi}: {len(pending)} tracks rebuilt, {len(g["floats"])} floats')
(MOD / 'files').mkdir(exist_ok=True)
(MOD / 'files' / 'motion.DBN').write_bytes(c.build(data))
(MOD / 'code').mkdir(exist_ok=True)
(MOD / 'code' / 'motion.txt').write_text(
    '; The mecha samurai\'s own motions (make_motion.py): body 0\'s motion set pointed at copies in unused motion\n'
    '; IDs, and those IDs given the stock motions\' tag lists (cr_g_MotionInfo: length + frame events). Other bodies keep the stock motions.\n'
    + '\n'.join(code) + '\n')
print('\n'.join(done))
print(f'files/motion.DBN, code/motion.txt ({len(code)} words)')
