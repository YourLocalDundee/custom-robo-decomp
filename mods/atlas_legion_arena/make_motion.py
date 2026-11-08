"""Atlas (the legionnaire body on robo 0): every motion of the body (all 38 slots) - gun, air dash, charge, victories,
lose, and the shield-up style for standing, moving, jumping, landing, falling, getting up and the transform.
Writes files/motion.DBN of this mod (make_mod.py puts it into logo_read_us.dbn entry 8, the copy the game plays);
then: python -I tools/make_mod.py mods/atlas_legion_arena

Every pose is written relative to the robo's hips (point 0) in the robo's own frame (forward / up / left, from the
motion's root rotation y), so it turns with the game's turns. Root and joint 0 tracks are never touched (Nov 2, 2026
rule: those blacked the screen); every rebuilt track keys the motion's last frame (crmotion.build checks it).

- the 6 sf_weapon_* motions (stance, turn left / right, recoil, jump, jump recoil; every sf_ body shares them): the
  shield brace. The off hand (hand A, the scutum) held across the chest, the chest leaned 6 degrees into the shot.
  The gun hand is left to the game (it aims it), except on the two recoil motions (sf_weapon_stiff,
  sf_weapon_jump_stiff): there the stock single kick becomes the cadence, a triple tap "da-da-DUM" (kicks on frames
  0, 6 and 12, the last one the strongest) with the chest rocking back on each beat.
- sf_jump_dash (the air dash): the spear dive. Leaned 40 degrees forward, the shield thrust ahead, the gun arm swept
  back along the side, both legs straight and trailing.
- cr_atk_sf011_tackl_n (robo 0's charge attack, 60 frames = three 20-frame phases): the shield bash. Wind-up (chest
  back, shield drawn back), the rush (leaned 35 degrees in, shield punched straight ahead at shoulder height, gun arm
  back), recovery into the stock end pose. The stock forward travel of the hips is kept.
- sf_win_sf011_01 (victory 1): the gladiator salute. Standing tall, the shield planted at the side, the gun raised
  straight up and pumped three times.
- sf_win_sf011_02 (victory 2): the shield raise. The shield lifted overhead, the gun arm pointed up and out, then
  three strikes of the gun on the face of the shield.
- sf_lose_sf011_01 (lose): down on one knee. The hips sink, one foot goes back, the chest and head slump, the gun
  arm hangs to the ground, the shield leans in front; a slow last sag.
- the other 25 (styled()): the shield held up in front (GUARD) standing / walking / running with a lean into the run
  (legs and the gun-arm swing stock), bomb and pod motions braced like the gun, overhead on the double jump (OVER),
  low on landings (LOW), over the face on the 5 knock-downs (FACE, in body_frame(): the axes tip with the torso),
  low-to-guard getting up, a raised-shield salute on sf_change.
usage: python -I mods/atlas_legion_arena/make_motion.py [--out FILE]"""
import math
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(G / 'tools'))
import crmotion as c          # noqa: E402
import crmotion_rig as rig    # noqa: E402

OUT = sys.argv[sys.argv.index('--out') + 1] if '--out' in sys.argv else None
TOL = 0.006
LEG = 0.86        # a straight leg's IK target distance from the hip joint (the IK solves on the default skeleton)
HIP = 0.08        # the hip joint sits this far below point 0
src = G / 'Custom Robo extracted' / 'files' / c.BOOT_BUNDLE
data = c.parse(c.read_motion(src))
dol = c.load_dol(None)


# ---- the robo's frame ----------------------------------------------------------------------------------------

def frame(ch):
    """(forward, up, left) unit vectors in the points' frame: root rotation y 0 faces -Z with the off hand at -X."""
    r = math.radians(ch.get('root.roty', 0.0))
    return (math.sin(r), 0.0, -math.cos(r)), (0.0, 1.0, 0.0), (-math.cos(r), 0.0, -math.sin(r))


def at(base, fr, f=0.0, u=0.0, l=0.0):
    fw, up, lf = fr
    return [base[i] + f * fw[i] + u * up[i] + l * lf[i] for i in range(3)]


def rotate(v, axis, deg):
    """v turned about the unit axis by deg (Rodrigues)."""
    a = math.radians(deg)
    ca, sa = math.cos(a), math.sin(a)
    d = sum(v[i] * axis[i] for i in range(3))
    cr = [axis[1] * v[2] - axis[2] * v[1], axis[2] * v[0] - axis[0] * v[2], axis[0] * v[1] - axis[1] * v[0]]
    return [v[i] * ca + cr[i] * sa + axis[i] * d * (1 - ca) for i in range(3)]


def lean(ch, p0, fr, deg, keys=(1, 2)):
    """chest / head targets tipped about the robo's left axis through point 0: positive leans forward."""
    out = {}
    for k in keys:
        rel = [ch['point%d.%s' % (k, a)] - p0[i] for i, a in enumerate('xyz')]
        n = rotate(rel, fr[2], -deg)   # about +left, a negative turn takes +up toward +forward
        out[k] = [p0[i] + n[i] for i in range(3)]
    return out


def pt(ch, k):
    return [ch['point%d.%s' % (k, a)] for a in 'xyz']


def mix(a, b, w):
    return [a[i] + (b[i] - a[i]) * w for i in range(3)]


def smooth(x):
    x = min(1.0, max(0.0, x))
    return x * x * (3 - 2 * x)


def ramp(t, t0, t1):
    return smooth((t - t0) / float(t1 - t0)) if t1 > t0 else float(t >= t0)


def put(vals, k, p):
    for i, a in enumerate('xyz'):
        vals['point%d.%s' % (k, a)] = p[i]


def straight_leg(p0, fr, back_deg, side):
    """a foot target on a straight leg swung back_deg behind straight down, side = +left / -left offset"""
    hip = at(p0, fr, u=-HIP)
    th = math.radians(back_deg)
    return at(hip, fr, f=-LEG * math.sin(th), u=-LEG * math.cos(th), l=side)


# ---- the motions ---------------------------------------------------------------------------------------------

def pulse(t, t0, amp):
    """a recoil kick: up in 2 frames, back down over the next 4"""
    x = t - t0
    if x < 0 or x > 6:
        return 0.0
    return amp * (x / 2.0 if x < 2 else 1.0 - (x - 2) / 4.0)


def triple(t):
    return pulse(t, 0, 0.6) + pulse(t, 6, 0.6) + pulse(t, 12, 1.0)


def gun(frames, last, name):
    recoil = name in ('sf_weapon_stiff', 'sf_weapon_jump_stiff')
    want = {}
    for t, ch in enumerate(frames):
        p0, fr = pt(ch, 0), frame(ch)
        k = triple(t) if recoil else 0.0
        vals = {}
        for key, p in lean(ch, p0, fr, 6.0 - 7.0 * k).items():     # leaned into the shot, rocked back on a beat
            put(vals, key, p)
        put(vals, 3, at(p0, fr, f=0.40 - 0.04 * k, u=0.20, l=0.10))  # the shield across the chest
        if recoil:   # the gun hand: the aim pose of frame 0 with the triple kick (up and back), instead of the stock kick
            aim = pt(frames[0], 4) if t < last else pt(ch, 4)
            put(vals, 4, at(aim, fr, f=-0.10 * k, u=0.14 * k))
        for kk, v in vals.items():
            want.setdefault(kk, []).append(v)
    return want


def dash(frames, last, name):
    want = {}
    for t, ch in enumerate(frames):
        p0, fr = pt(ch, 0), frame(ch)
        w = ramp(t, 0, 4)
        vals = {}
        for key, p in lean(ch, p0, fr, 40.0 * w).items():
            put(vals, key, p)
        put(vals, 3, mix(pt(ch, 3), at(p0, fr, f=0.68, u=0.28, l=0.12), w))   # the shield thrust ahead
        put(vals, 4, mix(pt(ch, 4), at(p0, fr, f=-0.42, u=-0.02, l=-0.30), w))  # the gun arm swept back
        put(vals, 5, mix(pt(ch, 5), straight_leg(p0, fr, 35.0, 0.09), w))
        put(vals, 6, mix(pt(ch, 6), straight_leg(p0, fr, 30.0, -0.09), w))
        for kk, v in vals.items():
            want.setdefault(kk, []).append(v)
    return want


def charge(frames, last, name):
    want = {}
    for t, ch in enumerate(frames):
        p0, fr = pt(ch, 0), frame(ch)
        wind = ramp(t, 0, 12) * (1 - ramp(t, 18, 24))      # phase 1: the wind-up
        rush = ramp(t, 18, 24) * (1 - ramp(t, 42, 58))     # phase 2: the rush (held into phase 3, then let go)
        vals = {}
        for key, p in lean(ch, p0, fr, -15.0 * wind + 35.0 * rush).items():
            put(vals, key, p)
        drawn = at(p0, fr, f=-0.10, u=0.16, l=0.42)
        punch = at(p0, fr, f=0.72, u=0.34, l=0.06)
        s = pt(ch, 3)
        s = mix(s, drawn, wind)
        put(vals, 3, mix(s, punch, rush))
        g = mix(pt(ch, 4), at(p0, fr, f=-0.40, u=-0.06, l=-0.32), rush)
        put(vals, 4, g)
        for kk, v in vals.items():
            want.setdefault(kk, []).append(v)
    return want


def win_salute(frames, last, name):
    want = {}
    p0s, f5, f6 = pt(frames[0], 0), pt(frames[0], 5), pt(frames[0], 6)
    for t, ch in enumerate(frames):
        fr = frame(ch)
        w = ramp(t, 0, 30) * (1 - ramp(t, last - 2, last))
        pump = sum(math.sin(math.pi * min(1.0, max(0.0, (t - t0) / 16.0))) for t0 in (50, 70, 90))
        p0 = at(p0s, fr, u=0.03 * w + 0.015 * pump)          # standing tall, a little lift on each pump
        vals = {}
        put(vals, 0, mix(pt(ch, 0), p0, w))
        for key, p in lean(ch, p0, fr, -8.0).items():
            put(vals, key, mix(pt(ch, key), p, w))
        put(vals, 3, mix(pt(ch, 3), at(p0, fr, f=0.24, u=-0.28, l=0.36), w))           # the shield planted
        put(vals, 4, mix(pt(ch, 4), at(p0, fr, f=0.08, u=0.82 - 0.14 * pump, l=-0.16), w))   # the gun raised, pumped
        put(vals, 5, mix(pt(ch, 5), f5, w))
        put(vals, 6, mix(pt(ch, 6), f6, w))
        for kk, v in vals.items():
            want.setdefault(kk, []).append(v)
    return want


def win_shield(frames, last, name):
    want = {}
    p0s, f5, f6 = pt(frames[0], 0), pt(frames[0], 5), pt(frames[0], 6)
    for t, ch in enumerate(frames):
        fr = frame(ch)
        w = ramp(t, 0, 36) * (1 - ramp(t, last - 2, last))
        lower = ramp(t, 70, 84)                                        # the shield comes down to the chest
        bang = sum(math.sin(math.pi * min(1.0, max(0.0, (t - t0) / 10.0))) for t0 in (88, 102, 116))
        p0 = at(p0s, fr, u=0.02 * w)
        vals = {}
        put(vals, 0, mix(pt(ch, 0), p0, w))
        for key, p in lean(ch, p0, fr, -6.0 + 4.0 * lower).items():
            put(vals, key, mix(pt(ch, key), p, w))
        up_shield = at(p0, fr, f=0.14, u=0.86, l=0.14)
        chest_shield = at(p0, fr, f=0.42, u=0.30, l=0.12)
        sh = mix(up_shield, chest_shield, lower)
        put(vals, 3, mix(pt(ch, 3), sh, w))
        sky = at(p0, fr, f=0.46, u=0.62, l=-0.24)
        ready = at(p0, fr, f=0.30, u=0.38, l=-0.30)
        hit = at(p0, fr, f=0.52, u=0.32, l=-0.02)                       # the gun on the shield's face
        g = mix(mix(sky, ready, lower), hit, bang)
        put(vals, 4, mix(pt(ch, 4), g, w))
        put(vals, 5, mix(pt(ch, 5), f5, w))
        put(vals, 6, mix(pt(ch, 6), f6, w))
        for kk, v in vals.items():
            want.setdefault(kk, []).append(v)
    return want


def lose(frames, last, name):
    want = {}
    p0s, f5, f6 = pt(frames[0], 0), pt(frames[0], 5), pt(frames[0], 6)
    for t, ch in enumerate(frames):
        fr = frame(ch)
        w = ramp(t, 8, 50)                       # down onto one knee
        sag = ramp(t, 90, 140)                   # the last slow sag
        p0 = at(p0s, fr, u=-0.40 * w - 0.03 * sag)
        vals = {}
        put(vals, 0, p0)
        for key, p in lean(ch, p0, fr, 0.0).items():   # start from the upright chest and head of the stock frame 0
            pass
        rel1 = [frames[0]['point1.%s' % a] - p0s[i] for i, a in enumerate('xyz')]
        rel2 = [frames[0]['point2.%s' % a] - p0s[i] for i, a in enumerate('xyz')]
        n1 = rotate(rel1, fr[2], -(30.0 * w + 8.0 * sag))
        n2 = rotate(rel2, fr[2], -(45.0 * w + 14.0 * sag))
        put(vals, 1, [p0[i] + n1[i] for i in range(3)])
        put(vals, 2, [p0[i] + n2[i] for i in range(3)])
        knee_foot = at(p0s, fr, f=-0.46, l=0.12)                  # the left foot back: that knee goes down
        knee_foot[1] = f5[1]
        front_foot = at(p0s, fr, f=0.26, l=-0.16)
        front_foot[1] = f6[1]
        put(vals, 5, mix(f5, knee_foot, w))
        put(vals, 6, mix(f6, front_foot, w))
        put(vals, 3, mix(pt(frames[0], 3), at(p0, fr, f=0.36, u=-0.30 - 0.04 * sag, l=0.28), w))   # the shield leaned in front
        put(vals, 4, mix(pt(frames[0], 4), at(p0, fr, f=0.08, u=-0.46, l=-0.36), w))              # the gun arm hanging
        for kk, v in vals.items():
            want.setdefault(kk, []).append(v)
    return want


# ---- the rest of the body's motions: the shield always up -------------------------------------------------------

def norm(v):
    n = math.sqrt(sum(x * x for x in v)) or 1.0
    return [x / n for x in v]


def cross(a, b):
    return [a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]]


def body_frame(ch):
    """(forward, up, left) of the torso: up = hips -> chest, left = the robo's left (turns about it stay exact),
    forward = left x up. Upright this is frame(ch); when the body tips over (falls) the axes tip with it."""
    left = frame(ch)[2]
    up = [ch['point1.%s' % a] - ch['point0.%s' % a] for a in 'xyz']
    d = sum(up[i] * left[i] for i in range(3))
    up = norm([up[i] - d * left[i] for i in range(3)])
    return cross(left, up), up, left


def styled(lean_deg, shield, gun_hand=None, ramp_in=0, ramp_out=0, tipped=False, extra=None):
    """a motion kind: the chest leaned lean_deg (a number or f(t, last)), the shield (hand A) at shield = (f, u, l)
    from the hips in the body's frame (or f(t, last) -> (f, u, l)), the gun hand optionally the same way; blended in
    over ramp_in frames from the stock pose and out over the last ramp_out frames (0 = held to the end)."""
    def kind(frames, last, name):
        want = {}
        for t, ch in enumerate(frames):
            p0 = pt(ch, 0)
            fr = body_frame(ch) if tipped else frame(ch)
            w = (ramp(t, 0, ramp_in) if ramp_in else 1.0) * ((1 - ramp(t, last - ramp_out, last)) if ramp_out else 1.0)
            vals = {}
            deg = lean_deg(t, last) if callable(lean_deg) else lean_deg
            if not tipped:
                for key, p in lean(ch, p0, fr, deg * w).items():
                    put(vals, key, p)
            s = shield(t, last) if callable(shield) else shield
            put(vals, 3, mix(pt(ch, 3), at(p0, fr, *s), w))
            if gun_hand is not None:
                g = gun_hand(t, last) if callable(gun_hand) else gun_hand
                put(vals, 4, mix(pt(ch, 4), at(p0, fr, *g), w))
            if extra:
                extra(t, last, ch, p0, fr, w, vals)
            for kk, v in vals.items():
                want.setdefault(kk, []).append(v)
        return want
    kind.__name__ = 'shield_' + ('tipped' if tipped else 'up')
    return kind


GUARD = (0.40, 0.22, 0.10)          # the shield held up in front of the chest
LOW = (0.42, 0.02, 0.12)            # braced low (landings)
OVER = (0.16, 0.62, 0.08)           # held over the head (double jump)
FACE = (0.30, 0.46, 0.04)           # covering the face (falls: in the tipped body frame)


def stance_shield(t, last):         # a slow breath: the shield rises and settles 1.5 cm
    return (GUARD[0], GUARD[1] + 0.015 * math.sin(2 * math.pi * t / max(1, last)), GUARD[2])


def getup_shield(t, last):          # rising behind the shield: low to guard
    w = ramp(t, 0, last)
    return tuple(LOW[i] + (GUARD[i] - LOW[i]) * w for i in range(3))


def change_shield(t, last):         # the transform: a quick salute, shield raised then back to guard
    w = math.sin(math.pi * min(1.0, t / max(1.0, last * 0.8)))
    return tuple(GUARD[i] + (OVER[i] - GUARD[i]) * w for i in range(3))


KINDS = {'sf_weapon_ta_kamae': gun, 'sf_weapon_l90': gun, 'sf_weapon_r90': gun, 'sf_weapon_stiff': gun,
         'sf_weapon_jump': gun, 'sf_weapon_jump_stiff': gun, 'sf_jump_dash': dash, 'cr_atk_sf011_tackl_n': charge,
         'sf_win_sf011_01': win_salute, 'sf_win_sf011_02': win_shield, 'sf_lose_sf011_01': lose,
         # standing and moving: the shield up, leaning into the run (the legs and the gun-arm swing stay stock)
         'sf_normal_ta_kamae': styled(4.0, stance_shield),
         'sf_normal_walk': styled(6.0, GUARD), 'sf_normal_run': styled(14.0, GUARD),
         'sf_normal_walk_weapon_r180': styled(6.0, GUARD), 'sf_normal_run_weapon_r180': styled(14.0, GUARD),
         'sf_back_walk_weapon_r180': styled(2.0, GUARD), 'sf_back_run_weapon_r180': styled(4.0, GUARD),
         'sf_back_run_guard_r180': styled(4.0, GUARD), 'sf_normal_run_guard_r180': styled(14.0, GUARD),
         # air: over the head on the double jump, braced low on landing
         'sf_double_jump': styled(10.0, OVER, ramp_in=3), 'sf_landing': styled(10.0, LOW),
         'sf_landing_2': styled(10.0, LOW),
         # bombs and pods (launched from the back): the shield brace of the gun motions
         'sf_bomb_l90': styled(6.0, GUARD), 'sf_bomb_r90': styled(6.0, GUARD), 'sf_bomb_ta_kamae': styled(6.0, GUARD),
         'sf_bomb_jump': styled(6.0, GUARD), 'sf_pod': styled(6.0, GUARD), 'sf_pod_jump': styled(6.0, GUARD),
         # knocked down: the shield arm covers the face, in a frame that tips with the body
         'sf_fall_back': styled(0, FACE, ramp_in=6, tipped=True), 'sf_fall_back_wall': styled(0, FACE, ramp_in=6, tipped=True),
         'sf_fall_front': styled(0, FACE, ramp_in=6, tipped=True), 'sf_fall_front_wall': styled(0, FACE, ramp_in=6, tipped=True),
         'sf_fall_magma': styled(0, FACE, ramp_in=6, tipped=True),
         # getting up behind the shield; the transform salute
         'sf_sf011_powerup': styled(8.0, getup_shield), 'sf_change': styled(0.0, change_shield)}

done = []
for g in data['groups']:
    pending = []
    for m in g['motions']:
        name = dol.name(m['id'])
        kind = KINDS.get(name)
        if not kind:
            continue
        roles = c.track_roles(len(m['tracks']))
        last = max(t['times'][-1] for t in m['tracks'] if t)
        frames = [c.sample(m, g['floats'], f) for f in range(last + 1)]
        want = kind(frames, last, name)
        nkeys = 0
        for k, vals in want.items():
            ti = roles.index(k)
            keys = rig.reduce_keys(vals, 0.2 if 'rot' in k else TOL)
            pending.append((m, ti, keys))
            nkeys += len(keys)
            m['tracks'][ti] = None
        done.append('%s (%s, %d frames, %d keys)' % (name, kind.__name__, last + 1, nkeys))
    if pending:   # the replaced tracks' floats are dropped (rig.compact_pool), then the new keys go into the pool
        rig.compact_pool(g)
        pool = {}
        for w in (2, 1, 3):
            for i in range(len(g['floats']) - w + 1):
                pool.setdefault(tuple(g['floats'][i:i + w]), i)
        for m, ti, keys in pending:
            if not keys:
                continue
            try:
                words = [c.encode_key(x, g['floats'], pool) for x in keys]
            except ValueError:
                raise SystemExit('the float pool of the group is full (%d floats): use a larger tolerance' % len(g['floats']))
            m['tracks'][ti] = {'times': [x['frame'] for x in keys], 'words': words}
        print('group: %d tracks rebuilt, %d floats' % (len(pending), len(g['floats'])))
missing = set(KINDS) - {d.split(' ')[0] for d in done}
if missing:
    raise SystemExit('motions not found: %s' % ', '.join(sorted(missing)))
out = Path(OUT) if OUT else Path(__file__).resolve().parent / 'files' / 'motion.DBN'
out.parent.mkdir(parents=True, exist_ok=True)
out.write_bytes(c.build(data))
print('wrote %s: %d motions:\n  %s' % (out, len(done), '\n  '.join(done)))
