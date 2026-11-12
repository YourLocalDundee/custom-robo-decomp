"""The mecha samurai's own motions, generated from scratch (Nov 12, 2026; owner: "we're not using any of Ray's [motions]
... we need all of his").

What is still taken from the stock motion a slot replaces, and why:
- its length (the game's tag list) and its root tracks / joint 0 (the game moves the robo with them; editing them
  blacked the screen in the Atlas tests, docs/formats/motion_playback.md);
- the hips' travel over the ground (point 0 x / z: the walk / run carry the points forward with the root) and, in
  locomotion, WHERE and WHEN each foot is planted (else the feet would skate against the game's movement).
Everything else is ours: every IK point's shape (hips height and sway, chest, head, both hands, the feet's swing
arcs) and every rotation joint 1..25 (from one neutral pose + our own swing / twist / roll), so no stock pose data
is played.

Conventions (docs/formats/motion_playback.md): +Y up; the robo faces -Z (fz = -1) except in *_r180 motions (fz = +1,
sides swapped); point 0 hips, 1 chest, 2 head, 3 off hand (A), 4 gun hand (B), 5 / 6 feet; a foot on the ground has
its target at y 0.14.
"""
import math

GROUND = 0.14
# the neutral rotation pose (degrees): our standing samurai. Joint 3 (head) is 180 / -90 / 0 in every stock motion
# (its frame), joint 6 / 11 (upper arms) z -90 and the thighs (16 / 21) z 90 likewise: those are frame constants.
NEUTRAL = {
    'joint1': (0.0, 100.0, 0.0),     # upper body: a little side-on (guard), 90 = square to the front
    'joint3': (180.0, -90.0, 0.0),   # head
    'joint5': (0.0, 0.0, 0.0),       # arm A shoulder
    'joint6': (15.0, -30.0, -90.0),    # arm A upper arm
    'joint9': (0.0, 0.0, 0.0),       # arm A hand
    'joint10': (0.0, 0.0, 0.0),      # arm B shoulder
    'joint11': (-15.0, 30.0, -90.0),  # arm B upper arm
    'joint14': (0.0, 0.0, 0.0),      # arm B hand (the gun)
    'joint15': (0.0, 100.0, 0.0),    # lower body: with the upper body
    'joint16': (0.0, 15.0, 90.0),    # leg A thigh: toes a little out
    'joint19': (0.0, 25.0, 0.0),     # leg A ankle: x = heel lift / toe down, y = toe out, z = roll
    'joint20': (0.0, 0.0, 0.0),      # leg A foot: x = toe bend
    'joint21': (0.0, -15.0, 90.0),   # leg B thigh
    'joint24': (0.0, -25.0, 0.0),    # leg B ankle
    'joint25': (0.0, 0.0, 0.0),      # leg B foot
}


LIE_Y = 0.62        # the hips' height lying down (the samurai's thickness)
REACH = 0.84        # hip joint -> foot target at a nearly straight leg (docs: about 0.9 is straight on the default skeleton)
HIP_DROP = 0.08     # the hip joints sit this far below point 0
HIP_X = 0.07        # and this far to each side


def reach_height(hips, feet_pts, base):
    """the hips' height: `base`, lowered where a planted foot would be out of the leg's reach (else the knee folds
    and the other leg kneels: the first run test)"""
    y = base
    for f in feet_pts:
        if f is None or f[1] > GROUND + 0.02:
            continue                    # only planted feet hold the hips down; a swinging foot is pulled in instead
        dx = abs(f[0] - hips[0]) - HIP_X
        dz = f[2] - hips[2]
        h2 = REACH ** 2 - dx * dx - dz * dz
        if h2 > 0:
            y = min(y, f[1] + math.sqrt(h2) + HIP_DROP)
        else:
            y = min(y, f[1] + HIP_DROP + 0.25)
    return y


def pull_in(hips, f):
    """a swinging foot target beyond the leg's reach: pulled toward the hip joint to REACH (else the solver folds
    the hips down after it)"""
    if f is None:
        return f
    hx = hips[0] + (HIP_X if f[0] > hips[0] else -HIP_X)
    d = [f[0] - hx, f[1] - (hips[1] - HIP_DROP), f[2] - hips[2]]
    L = math.sqrt(sum(x * x for x in d))
    if L <= REACH or f[1] <= GROUND + 0.02:
        return f
    k = REACH / L
    return [hx + d[0] * k, max(GROUND, hips[1] - HIP_DROP + d[1] * k), hips[2] + d[2] * k]


def smooth(t):
    t = max(0.0, min(1.0, t))
    return t * t * (3 - 2 * t)


def lerp(a, b, t):
    return [x + (y - x) * t for x, y in zip(a, b)]


def P(ch, k):
    return [ch[f'point{k}.{a}'] for a in 'xyz']


def facing(ch):
    return -1.0 if math.cos(math.radians(ch.get('root.roty', 0.0))) >= 0 else 1.0


def dirs(ch, ch0):
    """(F, O): the body's forward and off-hand-side directions in the points' frame (x, z). Stock gun / bomb turns
    (root rotation y -90 -> 0) show the points turning with the root: at frame 0 a turn's feet are the stance's,
    at the end they are turned by (x, z) -> (z, -x). So F = R(d) F0, O = R(d) O0 with d = root y now - root y at
    frame 0, F0 = (0, fz), O0 = (fz, 0)."""
    fz = facing(ch0)
    d = -math.radians(ch.get('root.roty', 0.0) - ch0.get('root.roty', 0.0))   # sign: the turn tests (Nov 12)
    c, s_ = math.cos(d), math.sin(d)

    def R(x, z):
        return (x * c + z * s_, -x * s_ + z * c)
    return R(0.0, fz), R(fz, 0.0)


def at(hips, F, O, side, up, fwd):
    """hips + side x O + up + fwd x F"""
    return [hips[0] + O[0] * side + F[0] * fwd, hips[1] + up, hips[2] + O[1] * side + F[1] * fwd]


def put(v, k, p):
    for i, a in enumerate('xyz'):
        v[f'point{k}.{a}'] = p[i]


def rot(v, j, x=None, y=None, z=None, add=(0.0, 0.0, 0.0)):
    base = NEUTRAL[j]
    for i, (a, val) in enumerate(zip('xyz', (x, y, z))):
        v[f'{j}.rot{a}'] = (base[i] if val is None else val) + add[i]


def neutral(v):
    for j in NEUTRAL:
        rot(v, j)


def side_of(frames):
    """the off hand's side (-1 / +1 in x) from the stock motion's first frame"""
    # Nov 12: the stock run's off hand is on the centre line at frame 0, so its x said the wrong side and the arms
    # crossed through the chest; in every stock motion the off hand's side is the facing's sign (-x facing -Z, +x in
    # the *_r180 motions)
    return facing(frames[0])


def foot_sides(frames):
    """+1 for the foot point (5 / 6) on the off-hand side, -1 for the gun side (at frame 0)"""
    ch = frames[0]
    fz = facing(ch)
    return {k: (1.0 if (ch[f'point{k}.x'] - ch['point0.x']) * fz > 0 else -1.0) for k in (5, 6)}


def ankle_joints(frames):
    """foot point -> (ankle joint, toe joint): points 5 / 6 swap legs in the mirrored *_r180 motions"""
    if facing(frames[0]) < 0:
        return {5: ('joint19', 'joint20'), 6: ('joint24', 'joint25')}
    return {5: ('joint24', 'joint25'), 6: ('joint19', 'joint20')}


def travel(frames):
    """the hips' path over the ground (x, z): the stock's, smoothed (a 9-frame mean) so no stock sway is kept"""
    n = len(frames)
    out = []
    for i in range(n):
        lo, hi = max(0, i - 4), min(n, i + 5)
        out.append([sum(frames[k]['point0.x'] for k in range(lo, hi)) / (hi - lo),
                    sum(frames[k]['point0.z'] for k in range(lo, hi)) / (hi - lo)])
    return out


def contacts(frames, k):
    """the frames foot k is planted in the stock motion (its target at its lowest, within 1.5 cm) and where"""
    ys = [ch[f'point{k}.y'] for ch in frames]
    low = min(ys)
    planted = [y <= max(low, GROUND) + 0.015 for y in ys]
    return planted


def foot_path(frames, k, lift, ahead=0.0):
    """our own foot k: planted where and when the stock foot is planted (the mean of its planted positions, so the
    foot does not creep), and between plants our own arc: an eased glide with a sine lift of `lift`, its peak a
    little early (a samurai picks the foot up quickly, sets it down softly). -> (positions, swing phase 0..1 or None)"""
    n = len(frames)
    planted = contacts(frames, k)
    pos = [P(ch, k) for ch in frames]
    if not any(planted):
        return None, None
    # the planted runs and their mean positions
    runs = []
    i = 0
    while i < n:
        if planted[i]:
            j = i
            while j + 1 < n and planted[j + 1]:
                j += 1
            m = [sum(pos[t][a] for t in range(i, j + 1)) / (j - i + 1) for a in range(3)]
            runs.append((i, j, [m[0], GROUND, m[2]]))
            i = j + 1
        else:
            i += 1
    out = [None] * n
    phase = [None] * n
    for a_, b_, m in runs:
        for t in range(a_, b_ + 1):
            out[t] = m
    # swings between runs; before the first / after the last run: toward / from the stock's own end positions
    bounds = [(-1, None)] + [(r[1], r[2]) for r in runs]
    nxt = [(r[0], r[2]) for r in runs] + [(n, None)]
    for (e, src), (s, dst) in zip(bounds, nxt):
        if s - e <= 1:
            continue
        p_src = src if src is not None else [pos[0][0], pos[0][1], pos[0][2]]
        p_dst = dst if dst is not None else [pos[-1][0], pos[-1][1], pos[-1][2]]
        L = s - e
        for t in range(e + 1, s):
            u = (t - e) / L
            g = smooth(u)
            h = math.sin(math.pi * min(1.0, u * 1.15)) if u < 1 / 1.15 else 0.0
            base_y = p_src[1] + (p_dst[1] - p_src[1]) * g
            out[t] = [p_src[0] + (p_dst[0] - p_src[0]) * g, max(GROUND, base_y) + lift * h,
                      p_src[2] + (p_dst[2] - p_src[2]) * g + ahead * h]
            phase[t] = u
    return out, phase


def chest_head(v, hips, F, O, lean_deg, sway=0.0, look=0.0, nod=0.0):
    """our chest and head targets over the hips. Both are aim targets, not positions (stock data: the chest target
    sits about 0.37 up and a little forward of the hips, the head target 0.32 up and 0.30 forward - where he looks):
    tipped forward by the lean, the head's further by a nod; a sway toward the off side; a look to the side"""
    a = math.radians(lean_deg)
    cu, cf = 0.37 * math.cos(a) - 0.08 * math.sin(a), 0.37 * math.sin(a) + 0.08 * math.cos(a)
    b = math.radians(lean_deg + nod)
    hu, hf = 0.32 * math.cos(b) - 0.30 * math.sin(b), 0.32 * math.sin(b) + 0.30 * math.cos(b)
    put(v, 1, at(hips, F, O, sway * 0.6, cu, cf))
    put(v, 2, at(hips, F, O, sway * 0.4 + look, hu, hf))


def stand(frames, height, guard=True, breathe=0.008):
    """idle / guard: feet planted wide, knees soft, the off hand at the hilt, the gun held low and out; breathing"""
    fs = foot_sides(frames)
    tr = travel(frames)
    n = len(frames)
    out = []
    for t, ch in enumerate(frames):
        F, O = dirs(ch, frames[0])
        br = math.sin(2 * math.pi * t / max(n - 1, 1))
        hips = [tr[t][0], height + breathe * br * 0.5, tr[t][1]]
        v = {}
        put(v, 0, hips)
        chest_head(v, hips, F, O, 4.0 + 1.0 * br)
        v['point1.y'] += breathe * br
        for k in (5, 6):
            off = fs[k] > 0                     # the off-hand side's foot leads
            put(v, k, at([hips[0], GROUND, hips[2]], F, O, fs[k] * 0.24, 0.0, 0.20 if off else -0.18))
        put(v, 3, at(hips, F, O, 0.26, 0.06, 0.22))                    # hand on the hilt
        put(v, 4, at(hips, F, O, -0.34, -0.10 + 0.01 * br, 0.12))      # the gun, low and out
        neutral(v)
        rot(v, 'joint1', y=NEUTRAL['joint1'][1] + (10.0 if guard else 0.0))
        rot(v, 'joint15', y=NEUTRAL['joint15'][1] + (6.0 if guard else 0.0))
        out.append(v)
    return out


def locomotion(frames, height, lift, lean, arm_swing, bob, twist, run=False):
    """walk / run / back / strafe cycles: feet on the stock plants with our arcs; our hips bob, sway over the
    planted foot, the torso counter-twists, the arms swing against the legs (the gun arm half as much); heel lift
    and toe point through each swing"""
    fs = foot_sides(frames)
    aj = ankle_joints(frames)
    tr = travel(frames)
    feet = {k: foot_path(frames, k, lift) for k in (5, 6)}
    if feet[5][0] is None or feet[6][0] is None:
        return air(frames, height, tuck=0.15, trail=0.1)
    off_k = 5 if fs[5] > 0 else 6
    out = []
    for t, ch in enumerate(frames):
        F, O = dirs(ch, frames[0])
        sw = {k: (math.sin(math.pi * feet[k][1][t]) if feet[k][1][t] is not None else 0.0) for k in (5, 6)}
        stride = sw[off_k] - sw[11 - off_k]       # +1: the off-side foot swings, -1: the gun-side foot
        up = max(sw.values())
        hips = at([tr[t][0], height + bob * up - (0.02 if run else 0.0), tr[t][1]], F, O, 0.03 * stride, 0.0, 0.0)
        hips[1] = reach_height(hips, [feet[5][0][t], feet[6][0][t]], hips[1] + 0.25)
        hips[1] = min(hips[1], height + 0.12)
        v = {}
        put(v, 0, hips)
        chest_head(v, hips, F, O, lean, sway=0.015 * stride)
        for k in (5, 6):
            put(v, k, pull_in(hips, feet[k][0][t]))
        ah = 0.16 if run else -0.08
        wide = 0.06 if run else 0.0
        put(v, 3, at(hips, F, O, 0.29 + wide, ah + 0.03 * abs(stride), 0.10 - arm_swing * stride))
        put(v, 4, at(hips, F, O, -(0.32 + wide), ah - 0.04, 0.08 + 0.5 * arm_swing * stride))
        neutral(v)
        rot(v, 'joint1', y=90.0 + twist * stride)
        rot(v, 'joint15', y=90.0 - 0.6 * twist * stride)
        for k in (5, 6):
            ank, toe = aj[k]
            rot(v, ank, x=-(55.0 if run else 40.0) * sw[k])
            rot(v, toe, x=(25.0 if run else 15.0) * sw[k])
        out.append(v)
    return out


def air(frames, height, tuck=0.20, trail=0.15, lean=8.0, arms_back=0.0, spread=0.12):
    """jumps / the double jump / the air dash: our own tuck - the off-side knee drawn up, the gun-side leg trailing -
    eased in over the first frames; the hips mostly ours, a little of the stock height (some jumps move point 0)"""
    fs = foot_sides(frames)
    aj = ankle_joints(frames)
    tr = travel(frames)
    n = len(frames)
    out = []
    for t, ch in enumerate(frames):
        F, O = dirs(ch, frames[0])
        k_ = smooth(t / max(n * 0.35, 1))
        hips = [tr[t][0], ch['point0.y'] * 0.3 + height * 0.7, tr[t][1]]
        v = {}
        put(v, 0, hips)
        chest_head(v, hips, F, O, lean * k_)
        for kk in (5, 6):
            if fs[kk] > 0:
                put(v, kk, at(hips, F, O, spread, -0.78 + tuck, 0.18))
            else:
                put(v, kk, at(hips, F, O, -spread * 0.8, -0.86 + tuck * 0.4, -trail))
        put(v, 3, at(hips, F, O, 0.30, 0.12, 0.16 - arms_back))
        put(v, 4, at(hips, F, O, -0.34, 0.08, 0.10 - arms_back))
        neutral(v)
        rot(v, 'joint1', y=95.0)
        rot(v, 'joint15', y=95.0)
        for kk in (5, 6):
            ank, toe = aj[kk]
            rot(v, ank, x=-30.0 * k_)              # toes pointed in the air
            rot(v, toe, x=10.0 * k_)
        out.append(v)
    return out


def fall(frames, height, back=True):
    """knocked down: the stock hips' height says how far down he is (the game's timing); our body tilts with it -
    backwards (or forwards), legs out along the ground the other way, arms out to the sides breaking the fall"""
    fs = foot_sides(frames)
    aj = ankle_joints(frames)
    tr = travel(frames)
    out = []
    lowest = min(ch['point0.y'] for ch in frames)
    for t, ch in enumerate(frames):
        F, O = dirs(ch, frames[0])
        down = max(0.0, min(1.0, (height - ch['point0.y']) / max(height - lowest, 1e-3)))
        # the stock lying height (0.14) is Ray 01's slim body: the samurai's back and helmet went 0.4 through the
        # floor (deform_check ground); his hips stay at LIE_Y or above
        y = max(ch['point0.y'], LIE_Y * down + ch['point0.y'] * (1 - down))
        tilt = (-1.0 if back else 1.0) * 50.0 * smooth(down * 1.2)
        hips = [tr[t][0], y, tr[t][1]]
        v = {}
        put(v, 0, hips)
        chest_head(v, hips, F, O, tilt)
        away = -1.0 if tilt > 0 else 1.0            # the legs go the other way from the chest
        for kk in (5, 6):
            p = at(hips, F, O, fs[kk] * (0.16 + 0.08 * down), 0.0, away * 0.92 * down)   # straight: a bent knee points at the floor lying down
            # a lying leg's shin guard hangs below the foot line: the feet rise as he goes down (deform_check: the
            # shins / ankles went 9-14 cm through the floor)
            # lying, the legs lie out at the hips' height (feet drawn in folded the knees down through the floor)
            p[1] = max(GROUND, hips[1] - 0.80 * (1 - down) - 0.04 * down)
            put(v, kk, p)
        a = math.radians(tilt)
        for kk, sd in ((3, 1.0), (4, -1.0)):
            p = at(hips, F, O, sd * (0.36 + 0.14 * down), 0.25 * (1 - down), 0.42 * math.sin(a) * 0.6)
            p[1] = max(0.24, p[1])          # hands on the floor, not through it
            put(v, kk, p)
        neutral(v)
        rot(v, 'joint1', y=90.0)
        rot(v, 'joint15', y=90.0)
        rot(v, 'joint3', y=-90.0 + (8.0 if back else -8.0) * down)
        for kk in (5, 6):
            rot(v, aj[kk][0], x=-35.0 * down)
        out.append(v)
    return out


def action(frames, height, off_reach=None, gun_reach=None, lean=6.0, low=0.0):
    """bomb / pod / gun moves standing (turns included): feet on the stock plants with our short steps, hips low
    and steady, our arm poses (off arm thrown out for the bomb, the gun arm up and out for gun moves); everything
    placed along the body's own forward / side, which turn with the root in the turns"""
    fs = foot_sides(frames)
    aj = ankle_joints(frames)
    tr = travel(frames)
    n = len(frames)
    feet = {k: foot_path(frames, k, 0.10) for k in (5, 6)}
    out = []
    for t, ch in enumerate(frames):
        F, O = dirs(ch, frames[0])
        u = t / max(n - 1, 1)
        hips = [tr[t][0], height - low, tr[t][1]]
        hips[1] = reach_height(hips, [feet[k][0][t] if feet[k][0] else None for k in (5, 6)], hips[1])
        v = {}
        put(v, 0, hips)
        chest_head(v, hips, F, O, lean)
        for kk in (5, 6):
            if feet[kk][0] is not None:
                put(v, kk, feet[kk][0][t])
            else:
                put(v, kk, at([hips[0], GROUND, hips[2]], F, O, fs[kk] * 0.24, 0.0, 0.0))
        k_ = smooth(u * 3) * (1 - smooth((u - 0.75) * 4))
        rest3 = at(hips, F, O, 0.26, 0.06, 0.22)
        rest4 = at(hips, F, O, -0.34, -0.06, 0.14)
        put(v, 3, lerp(rest3, at(hips, F, O, off_reach[0], off_reach[1], off_reach[2]), k_) if off_reach else rest3)
        put(v, 4, lerp(rest4, at(hips, F, O, -gun_reach[0], gun_reach[1], gun_reach[2]), k_) if gun_reach else rest4)
        neutral(v)
        rot(v, 'joint1', y=100.0 + 10.0 * k_)
        rot(v, 'joint15', y=100.0)
        for kk in (5, 6):
            ph = feet[kk][1][t] if feet[kk][1] else None
            s_ = math.sin(math.pi * ph) if ph is not None else 0.0
            rot(v, aj[kk][0], x=-30.0 * s_)
            rot(v, aj[kk][1], x=10.0 * s_)
        out.append(v)
    return out
