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

TOL = float(sys.argv[1]) if len(sys.argv) > 1 else 0.008
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


# ---- slot -> the motion the samurai plays (a copy for every shared one)
free = {g: [u for u in unused if group_of.get(u) == g] for g in range(3)}
target, code = {}, []
for slot, x in enumerate(mine):
    mid = x & 0x1FFF
    if mid in target:
        continue
    if mid in used_by_others:
        g = group_of[mid]
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
    return -1.0 if sum(ch['point3.x'] - ch['point0.x'] for ch in frames) < 0 else 1.0


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
        put(v, 3, lerp(P(ch, 3), [n0[0] + side * 0.22, n0[1] + 0.42, n0[2] + fz * 0.72], k))   # off the chest plate
        # the tucked gun out of the bulky chest plate (deform_check: arm b inside the torso)
        put(v, 4, lerp(P(ch, 4), [n0[0] - side * 0.32, n0[1] + 0.26, n0[2] + fz * 0.32], k))
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
            shoulder = [c1[0] + sh_sign * 0.22 + sh_sign * 0.06 * max(sw, 0), c1[1] + 0.08, c1[2] + fz * 0.30]
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
        n0 = [p0[0], p0[1] - 0.26 * k, p0[2]]
        v = {}
        put(v, 0, n0)
        for kk in (1, 2):
            rel = [a - b for a, b in zip(P(ch, kk), p0)]
            n = tip(rel, (12 if kk == 1 else 40) * k, fz)
            put(v, kk, [n0[i] + n[i] for i in range(3)])
        for f in (5, 6):
            q = P(ch, f)
            back = (q[0] - p0[0]) * side > 0         # the off-hand side's foot goes back: that knee goes down
            want = [q[0], ground, p0[2] - fz * 0.45] if back else [q[0], ground, p0[2] + fz * 0.30]
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
        lean = 10 * smooth(ph) + 22 * lunge * (1 - 0.6 * smooth((t - 44) / 14))
        for kk in (1, 2):
            rel = [a - b for a, b in zip(P(ch, kk), p0)]
            n = tip(rel, lean, fz)
            put(v, kk, [n0[i] + n[i] for i in range(3)])
        hilt = [n0[0] + side * 0.26, n0[1] + 0.05, n0[2] + fz * 0.26]   # in front of the hip plates (deform_check)
        # the sweep: from the hilt across the front to full reach on the other side, at chest height
        sw = smooth((t - 20) / 14)
        ang = math.radians(-70 + 160 * sw)
        reach = [n0[0] - side * 0.75 * math.sin(ang), n0[1] + 0.32, n0[2] + fz * 0.75 * math.cos(ang)]
        hand = lerp(hilt, reach, smooth((t - 18) / 6))
        put(v, 3, lerp(P(ch, 3), hand, smooth(ph * 1.5)))
        put(v, 4, lerp(P(ch, 4), [hand[0] + side * 0.06, hand[1] - 0.03, hand[2]], smooth(ph * 1.5)))
        for f in (5, 6):
            q = P(ch, f)
            sx = 1.0 if q[0] - p0[0] > 0 else -1.0
            front = (sx == -side)
            dz = (0.35 if front else -0.25) * lunge
            put(v, f, [q[0] + sx * 0.08 * smooth(ph), q[1], q[2] + fz * dz])
        out.append(v)
    return out


CLEAR = 0.28        # a hand's least distance from the body's centre line (hips -> chest)


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


ALIAS = ['stance', 'walk', 'run', 'walk_weapon_r180', 'run_weapon_r180', 'back_walk_weapon_r180', 'back_run_weapon_r180',
         'weapon_jump', 'back_run_guard_r180', 'run_guard_r180', 'air_dash', 'double_jump', 'landing', 'landing2',
         'bomb_l90', 'bomb_r90', 'bomb_stance', 'bomb_jump', 'pod', 'pod_jump', 'gun_stance', 'gun_jump',
         'gun_jump_stiff', 'gun_l90', 'gun_r90', 'gun_stiff', 'fall_back', 'fall_back_wall', 'fall_front',
         'fall_front_wall', 'fall_magma', 'getup', 'lose', 'victory1', 'victory2', 'charge', 'change', 'change2']


def style_for(slot):
    if slot in (0, 1, 2, 3, 4, 5, 6, 8, 9):
        return lambda fr: stance(fr, gun_out=0.07)
    if slot == 10:
        return air_dash
    if slot in (7, 11, 12, 13, 21, 22):
        return lambda fr: stance(fr, low=0.03, spread=0.03, hilt=0.6)
    if slot in (14, 15, 16, 17):        # bomb: the bomb is on the off arm; the hand stays where the game puts it
        return lambda fr: stance(fr, low=0.05, hands=False)
    if slot in (18, 19):
        return lambda fr: stance(fr, low=0.04, hilt=0.5)
    if slot in (20, 23, 24, 25):
        return lambda fr: stance(fr, low=0.06)
    if slot in (26, 27, 28, 29, 30):
        return lambda fr: stance(fr, low=0.0, spread=0.04, lean=0.0, hands=False)
    if slot == 31:
        return head_check
    if slot == 32:
        return kneel
    if slot == 33:
        return heisman
    if slot == 34:
        return dust_off
    if slot == 35:
        return iai
    return lambda fr: stance(fr, low=0.03, hilt=0.4)       # change


done, pending_by_group = [], {}
styled = set()
for slot, x in enumerate(new_set):
    mid = x & 0x1FFF
    if mid in styled:
        continue
    styled.add(mid)
    m = entry[mid]
    gi = group_of[mid]
    roles = c.track_roles(len(m['tracks']))
    last = max(t['times'][-1] for t in m['tracks'] if t)
    frames = [c.sample(m, data['groups'][gi]['floats'], f) for f in range(last + 1)]
    per = clear_body(style_for(slot)(frames), frames)
    want = {}
    for v in per:
        for k, val in v.items():
            want.setdefault(k, []).append(val)
    for k, vals in want.items():
        if len(vals) != last + 1:
            continue
        ti = roles.index(k)
        if max(abs(a - fr[k]) for a, fr in zip(vals, frames)) < TOL:
            continue                    # the style left this track as it was: keep the stock keys (no new floats)
        keys = rig.reduce_keys(vals, TOL)
        pending_by_group.setdefault(gi, []).append((m, ti, keys))
        m['tracks'][ti] = None
    done.append(f'slot {slot:2d} {ALIAS[slot]:22s} {dol.name(mine[slot] & 0x1FFF):28s} -> {dol.name(m["id"]):28s} ({last + 1} frames)')
for gi, pending in pending_by_group.items():
    g = data['groups'][gi]
    rig.compact_pool(g)
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
