"""Atlas (the ray_legionnaire body on robo 0): the Roman march, the off-arm flail, the hip-thrust dash and the butt shot.
Writes files/motion.DBN of this mod (make_mod.py puts it into logo_read_us.dbn entry 8, the copy the game plays);
then: python -I tools/make_mod.py mods/atlas_ninja_run

In a battle the game aims the gun arm (hand B, point4) at the target and keeps the robos facing each other, so a
pose that moves the gun arm or turns the body is fought by the game. What this script changes instead:
- the 8 walk / run motions of robo 0's battle motion set (slots 1-6, 8, 9; every sf_ body shares them): a Roman
  march. The body upright with the chest out, each swinging foot kicked forward on a straight leg. The off arm
  (hand A, point3) flails: a fast windmill beside the body. The gun hand is left alone.
- sf_jump_dash (slot 10, the air dash): the hip thrust. The body leaned back, the groin first, both arms thrown
  back and up, the legs trailing.
- every sf_weapon_* motion: the butt shot on all fours. Turned 180 degrees so the butt faces the target, bent
  over with the butt up, the off hand on the ground, the gun arm reaching back under the body between the legs so
  the gun points back and up through the butt.
usage: python -I mods/atlas_ninja_run/make_motion.py [KICK_DEG] [TOL] [--stock-dash] [--stock-gun] [--out FILE]"""
import math, sys
from pathlib import Path
G = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(G / 'tools'))
import crmotion as c, crmotion_rig as rig

# --stock-dash / --stock-gun: leave sf_jump_dash / the sf_weapon_* motions as the game has them; --out FILE: write there instead of files/motion.DBN
STOCK_DASH = '--stock-dash' in sys.argv
STOCK_GUN = '--stock-gun' in sys.argv   # leave the sf_weapon_* motions as the game has them
OUT = sys.argv[sys.argv.index('--out') + 1] if '--out' in sys.argv else None
# gun-motion diagnostics (Nov 2, 2026: the butt shot blacked the screen at battle start): --gun-turn DEG (default 160;
# 0 = no turn) and --gun-turn-only (the stock gun pose, only turned)
GUN_TURN = float(sys.argv[sys.argv.index('--gun-turn') + 1]) if '--gun-turn' in sys.argv else 160.0
GUN_TURN_ONLY = '--gun-turn-only' in sys.argv
_VALUED = ('--out', '--gun-turn')
sys.argv = [x for i, x in enumerate(sys.argv) if x not in ('--stock-dash', '--stock-gun', '--gun-turn-only') + _VALUED and (i == 0 or sys.argv[i - 1] not in _VALUED)]

KICK = float(sys.argv[1]) if len(sys.argv) > 1 else 70.0   # the march: the kicking leg's angle forward at the top
TOL = float(sys.argv[2]) if len(sys.argv) > 2 else 0.006
LEG = 0.86                   # a straight leg's IK target distance from the hip joint (the IK solves on the default skeleton)
CHEST_BACK = 20.0            # march: chest tipped back (degrees)
FLAIL_R, FLAIL_FRAMES = 0.45, 8.0   # off-arm windmill: radius, frames per turn (about 4 turns a second)
THRUST = 50.0                # dash: the upper body arched back on top of the tilt (degrees)
TRAIL = 60.0                 # dash: the legs swung back from straight down, in the body's frame (the tilt takes 20 off)
WRIST = -60.0                # gun motions: the gun hand's wrist pitch (joint14.rotz)
BEND = 150.0                 # gun motions: the chest tipped forward from upright, past horizontal (all fours)
TILT = 20.0                  # dash: the whole body tilted back (root rotation x), so the groin leads
src = G / 'Custom Robo extracted' / 'files' / c.BOOT_BUNDLE   # the copy the game plays (entry 8); motion.DBN is never loaded
data = c.parse(c.read_motion(src))
dol = c.load_dol(None)
# the points' frame: +Y is up; the robo faces -Z, except in the *_r180 motions, whose root rotation y is 180 (with
# joint 0's 180 the body still faces the same world way): there it faces +Z and its sides swap. Hand A (point3) is
# the off hand, hand B (point4) the gun hand.


def facing(ch):
    """-1 when the robo faces -Z in the points' frame, +1 when it faces +Z (root rotation y 180)."""
    return -1.0 if math.cos(math.radians(ch.get('root.roty', 0.0))) >= 0 else 1.0


def tip(rel, deg, fz=-1.0):
    """A target relative to the hips, tipped about X by deg (positive: forward, toward fz * Z)."""
    d = math.hypot(rel[1], rel[2]) or 0.3
    a = math.atan2(fz * rel[2], rel[1]) + math.radians(deg)
    return [rel[0], d * math.cos(a), fz * d * math.sin(a)]


def march(frames, last):
    ground = min(min(ch['point5.y'], ch['point6.y']) for ch in frames)
    lift_max = {f: max(ch['point%d.y' % f] for ch in frames) - ground for f in (5, 6)}
    turns = max(1, round(last / FLAIL_FRAMES))
    side = -1.0 if sum(ch['point3.x'] - ch['point0.x'] for ch in frames) < 0 else 1.0   # the off hand's side (x)
    want = {}
    for t, ch in enumerate(frames):
        p0 = [ch['point0.%s' % a] for a in 'xyz']
        fz = facing(ch)
        vals = {}
        for k in (1, 2):   # chest and head targets tipped back: upright, chest out
            rel = [ch['point%d.%s' % (k, a)] - p0[i] for i, a in enumerate('xyz')]
            n = tip(rel, -CHEST_BACK, fz)
            vals.update({'point%d.y' % k: p0[1] + n[1], 'point%d.z' % k: p0[2] + n[2]})
        for f in (5, 6):   # the swinging foot: a straight leg kicked forward
            foot = [ch['point%d.%s' % (f, a)] for a in 'xyz']
            if lift_max[f] < 0.02:
                continue
            s = max(0.0, (foot[1] - ground) / lift_max[f])
            th = math.radians(KICK) * s ** 0.7
            kick = [foot[0], p0[1] - 0.08 - LEG * math.cos(th), p0[2] + fz * LEG * math.sin(th)]   # from the hip joint
            w = min(1.0, s * 2.5)
            vals['point%d.y' % f] = (1 - w) * foot[1] + w * kick[1]
            vals['point%d.z' % f] = (1 - w) * foot[2] + w * kick[2]
        # the off arm: a windmill beside the shoulder, with a sideways wobble; whole turns per loop so it loops
        ph = 2 * math.pi * turns * t / last if last else 0.0
        sh = [ch['point1.x'] + side * 0.22, vals['point1.y'], vals['point1.z']]
        dx = side * (0.35 - 0.3 * math.sin(2 * ph + 0.7))
        d = [dx, math.cos(ph), math.sin(ph)]
        n = math.sqrt(sum(x * x for x in d))
        for i, a in enumerate('xyz'):
            vals['point3.%s' % a] = sh[i] + FLAIL_R * d[i] / n
        for k, v in vals.items():
            want.setdefault(k, []).append(v)
    return want


def thrust(frames):
    want = {}
    for ch in frames:
        p0 = [ch['point0.%s' % a] for a in 'xyz']
        L = LEG   # the leg IK solves on the game's default skeleton (0x801F9B00): 0.9 from the hip joint is straight there, 0.6 a 96-degree bend
        # neither the root tracks nor joint 0 are touched: in a battle, root rotation x +20 here blacked the screen on
        # the dash (Nov 2 22:48) and joint 0 edits blacked it at the start (23:0x); the lean comes from the targets,
        # turned about point 0 below
        vals = {}
        for k in (1, 2):   # the upper body and head leaned far back: the groin leads
            rel = [ch['point%d.%s' % (k, a)] - p0[i] for i, a in enumerate('xyz')]
            n = tip(rel, -THRUST)
            vals.update({'point%d.y' % k: p0[1] + n[1], 'point%d.z' % k: p0[2] + n[2]})
        for k, sx in ((3, -1), (4, 1)):   # both arms thrown back and up, past the shoulders
            vals.update({'point%d.x' % k: p0[0] + sx * 0.30, 'point%d.y' % k: p0[1] + 0.20, 'point%d.z' % k: p0[2] + 0.65})
        th = math.radians(TRAIL)
        for f, sx in ((5, -1), (6, 1)):   # the legs trail behind the hips, straight
            vals.update({'point%d.x' % f: p0[0] + sx * 0.10, 'point%d.y' % f: p0[1] - 0.08 - L * math.cos(th),
                         'point%d.z' % f: p0[2] + L * math.sin(th)})   # from the hip joint, 0.08 below point 0
        tl = math.radians(TILT)   # the whole pose turned back about X through point 0 (+Y toward +Z)
        for k in range(1, 7):
            y, z = vals['point%d.y' % k] - p0[1], vals['point%d.z' % k] - p0[2]
            vals['point%d.y' % k] = p0[1] + y * math.cos(tl) - z * math.sin(tl)
            vals['point%d.z' % k] = p0[2] + y * math.sin(tl) + z * math.cos(tl)
        for k, v in vals.items():
            want.setdefault(k, []).append(v)
    return want


def butt_shot(frames):
    """On all fours, the butt up and toward the target (the body turned 180 degrees about Y), the off hand on the
    ground ahead, the gun arm reaching back under the body between the legs, so the gun points back and up through
    the butt: at the target."""
    ground = min(min(ch['point5.y'], ch['point6.y']) for ch in frames)
    want = {}
    for ch in frames:
        p0 = [ch['point0.%s' % a] for a in 'xyz']
        fz = facing(ch)
        # the turn to the target: root rotation y + GUN_TURN. Exactly 180 (and the pose with it) blacked the screen at
        # battle start (Nov 2, 2026): the game aims the gun arm itself, and a target exactly behind may be a 0 / 0 there
        vals = {'root.roty': ch['root.roty'] + GUN_TURN} if GUN_TURN else {}
        if GUN_TURN_ONLY:
            for k, v in vals.items():
                want.setdefault(k, []).append(v)
            continue
        for k in (1, 2):   # chest and head tipped forward past horizontal: bent over, the butt the highest point
            rel = [ch['point%d.%s' % (k, a)] - p0[i] for i, a in enumerate('xyz')]
            n = tip(rel, BEND, fz)
            vals.update({'point%d.y' % k: p0[1] + n[1], 'point%d.z' % k: p0[2] + n[2]})
        for f, sx in ((5, -1), (6, 1)):   # straight legs, spread, the feet a little behind the hips
            vals.update({'point%d.x' % f: p0[0] + sx * 0.18, 'point%d.y' % f: p0[1] - 0.08 - LEG * math.cos(math.radians(8)),
                         'point%d.z' % f: p0[2] - fz * LEG * math.sin(math.radians(8))})
        side = -1.0 if ch['point3.x'] - p0[0] < 0 else 1.0
        # the off hand flat on the ground ahead of the chest (the fourth leg)
        vals.update({'point3.x': p0[0] + side * 0.22, 'point3.y': ground, 'point3.z': p0[2] + fz * 0.70})
        # the gun hand: under the hips, between the legs, a little behind them; the forearm from the low shoulder
        # ahead runs back and up through the legs
        vals.update({'point4.x': p0[0], 'point4.y': p0[1] - 0.22, 'point4.z': p0[2] - fz * 0.18})
        # the gun barrel is the hand's X axis (along the forearm at wrist 0); the forearm points at the target 24
        # degrees down, the wrist (arm B's hand joint, joint 14) at z -60 tips it 33 degrees up: out through the butt
        vals['joint14.rotz'] = ch.get('joint14.rotz', 0.0) + WRIST
        for k, v in vals.items():
            want.setdefault(k, []).append(v)
    return want


# the motions robo 0 plays to move: its battle motion set (cr_g_BattleMotionSets 0x801C51B8, 38 u16 per body), slots
# 1-6, 8, 9 (walk / run: normal, back, weapon_r180, guard_r180); only these get the march, to fit the float pool
import struct
MOVE_IDS = {x & 0x7FFF for i, x in enumerate(struct.unpack('>38H', dol.read(0x801C51B8, 0x4C))) if i in (1, 2, 3, 4, 5, 6, 8, 9)}
done = []
for g in data['groups']:
    pending = []
    for m in g['motions']:
        name = dol.name(m['id'])
        fam, rest = name[:3], name[3:]
        if m['id'] & 0x7FFF in MOVE_IDS:
            kind = march
        elif name == 'sf_jump_dash' and not STOCK_DASH:
            kind = thrust
        elif fam == 'sf_' and rest.startswith('weapon_') and not STOCK_GUN:   # sf_ only: the group's float pool (16,384) is the limit
            kind = butt_shot
        else:
            continue
        roles = c.track_roles(len(m['tracks']))
        last = max(t['times'][-1] for t in m['tracks'] if t)
        frames = [c.sample(m, g['floats'], f) for f in range(last + 1)]
        want = kind(frames, last) if kind is march else kind(frames)
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
                print('\n  '.join(done))
                raise SystemExit('the float pool of the group is full (%d floats, %d keys pending): use fewer motions or a larger tolerance' % (len(g['floats']), sum(len(k) for _, _, k in pending)))
            m['tracks'][ti] = {'times': [x['frame'] for x in keys], 'words': words}
        print('group: %d tracks rebuilt, %d floats' % (len(pending), len(g['floats'])))
out = Path(OUT) if OUT else Path(__file__).resolve().parent / 'files' / 'motion.DBN'
out.write_bytes(c.build(data))
print('wrote %s: %d motions:\n  %s' % (out, len(done), '\n  '.join(done)))
