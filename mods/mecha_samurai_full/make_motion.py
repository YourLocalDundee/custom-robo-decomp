"""The mecha samurai's own battle motions: all 38 slots of body 0's set, without changing any other robo.

  python -I mods/mecha_samurai_full/make_motion.py [TOL]

Writes files/motion.DBN (make_mod.py puts it into logo_read_us.dbn entry 8, the copy the game plays) and
code/motion.txt (body 0's motion set and the motion lengths in main.dol). How (docs/full_custom_robo.md, finding 3):
- Body 0 (Ray 01) shares 32 of its 38 slots' motions with other bodies (sf_ walk / run / jumps / bomb / pod / gun /
  falls / change, get-up with Splendor and Glory). Each shared motion is copied into a battle motion that no body
  plays (docs/data_unused_battle_motions.txt) in the same group (so its keys' floats stay valid), the copy gets the
  stock motion's length in cr_g_MotionInfo (0x80205C94 -> record +4 -> u16, low 12 bits), and body 0's set
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
LOW, SPREAD, LEAN = 0.07, 0.05, 8.0     # stance: hips lower, feet wider (each side), chest forward (degrees)
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
for mid, t in target.items():
    if t == mid:
        continue
    hs, ht = header_addr(mid), header_addr(t)
    n_src = struct.unpack('>H', dol.read(hs, 2))[0] & 0xFFF
    wa = ht & ~3
    words.setdefault(wa, bytearray(dol.read(wa, 4)))
    old = struct.unpack('>H', words[wa][ht - wa:ht - wa + 2])[0]
    words[wa][ht - wa:ht - wa + 2] = struct.pack('>H', (old & 0xF000) | n_src)
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


def stance(frames, low=LOW, spread=SPREAD, lean=LEAN, hilt=0.75, hands=True, feet=True):
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
        out.append(v)
    return out


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
            put(v, kk, lerp(P(ch, kk), [n0[0] + sx * 0.16, n0[1] + 0.02, n0[2] + fz * 0.22], k))
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
        hilt = [n0[0] + side * 0.22, n0[1] + 0.05, n0[2] + fz * 0.12]
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


ALIAS = ['stance', 'walk', 'run', 'walk_weapon_r180', 'run_weapon_r180', 'back_walk_weapon_r180', 'back_run_weapon_r180',
         'weapon_jump', 'back_run_guard_r180', 'run_guard_r180', 'air_dash', 'double_jump', 'landing', 'landing2',
         'bomb_l90', 'bomb_r90', 'bomb_stance', 'bomb_jump', 'pod', 'pod_jump', 'gun_stance', 'gun_jump',
         'gun_jump_stiff', 'gun_l90', 'gun_r90', 'gun_stiff', 'fall_back', 'fall_back_wall', 'fall_front',
         'fall_front_wall', 'fall_magma', 'getup', 'lose', 'victory1', 'victory2', 'charge', 'change', 'change2']


def style_for(slot):
    if slot in (0, 1, 2, 3, 4, 5, 6, 8, 9):
        return lambda fr: stance(fr)
    if slot in (7, 10, 11, 12, 13, 21, 22):
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
        return lambda fr: stance(fr, low=0.04, hilt=0.5)
    if slot == 32:
        return kneel
    if slot == 33:
        return fist_high
    if slot == 34:
        return bow
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
    per = style_for(slot)(frames)
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
    '; IDs, and those IDs given the stock motions\' lengths (cr_g_MotionInfo). Other bodies keep the stock motions.\n'
    + '\n'.join(code) + '\n')
print('\n'.join(done))
print(f'files/motion.DBN, code/motion.txt ({len(code)} words)')
