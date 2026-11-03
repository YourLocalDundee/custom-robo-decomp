"""Atlas (the ray_legionnaire body) with a ninja run: every *_normal_run motion of motion.DBN gets its arms swept straight
back and its upper body leaned forward. Writes files/motion.DBN of this mod; then: python -I tools/make_mod.py mods/atlas_ninja_run
And every *_weapon_* motion (the gun stance, the shot, aiming left / right) turned to show its back to the target,
bent over, the gun hand at the butt (the "butt shot", a proof of concept).
usage: python -I mods/atlas_ninja_run/make_motion.py [LEAN_DEG]"""
import math, sys
from pathlib import Path
G = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(G / 'tools'))
import crmotion as c, crmotion_rig as rig

lean = float(sys.argv[1]) if len(sys.argv) > 1 else 45.0
src = G / 'Custom Robo extracted' / 'files' / 'motion.DBN'
data = c.parse(src.read_bytes())
dol = c.load_dol(None)
TOL = float(sys.argv[2]) if len(sys.argv) > 2 else 0.003
BACK, HAND_Y = 0.55, 0.05            # hands: this far behind the hips (+Z: the run goes toward -Z), near hip height
done = []
for g in data['groups']:
    pending = []
    for m in g['motions']:
        name = dol.name(m['id'])
        butt = name[:3] in ('sf_', 'mg_', 'ab_') and name[3:].startswith('weapon_')
        if 'normal_run' not in name and not butt:
            continue
        roles = c.track_roles(len(m['tracks']))
        last = max(t['times'][-1] for t in m['tracks'] if t)
        frames = [c.sample(m, g['floats'], f) for f in range(last + 1)]
        want = {}
        for ch in frames:
            p0 = [ch['point0.x'], ch['point0.y'], ch['point0.z']]
            r1 = [ch['point1.%s' % a] - p0[k] for k, a in enumerate('xyz')]
            d1 = math.hypot(r1[1], r1[2]) or 0.3
            a = math.radians(lean)
            n1 = [r1[0], d1 * math.cos(a), -d1 * math.sin(a)]        # the chest target tipped forward by `lean`
            delta = [n1[k] - r1[k] for k in range(3)]
            if butt:   # bent over 85 degrees, the back to the target, the gun hand (B, point 4) at the butt
                a = math.radians(85.0)
                n1 = [r1[0], d1 * math.cos(a), -d1 * math.sin(a)]
                delta = [n1[k] - r1[k] for k in range(3)]
                vals = {'root.roty': ch['root.roty'] + 180.0,
                        'point1.y': p0[1] + n1[1], 'point1.z': p0[2] + n1[2],
                        'point2.y': ch['point2.y'] + delta[1], 'point2.z': ch['point2.z'] + delta[2],
                        'point4.x': p0[0], 'point4.y': p0[1] - 0.05, 'point4.z': p0[2] + 0.30,
                        'point3.x': p0[0] - 0.15, 'point3.y': p0[1] - 0.35, 'point3.z': p0[2] - 0.10}
                for k, v in vals.items():
                    want.setdefault(k, []).append(v)
                continue
            vals = {
                'point1.y': p0[1] + n1[1], 'point1.z': p0[2] + n1[2],
                'point2.y': ch['point2.y'] + delta[1], 'point2.z': ch['point2.z'] + delta[2],
                'point3.y': p0[1] + HAND_Y, 'point3.z': p0[2] + BACK,
                'point4.y': p0[1] + HAND_Y, 'point4.z': p0[2] + BACK,
            }
            for k, v in vals.items():
                want.setdefault(k, []).append(v)
        for k, vals in want.items():
            ti = roles.index(k)
            pending.append((m, ti, rig.reduce_keys(vals, 0.2 if 'rot' in k else TOL)))
            m['tracks'][ti] = None
        done.append(name)
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
                raise SystemExit('the float pool of the group is full (%d floats, %d keys pending): use fewer motions or a larger tolerance' % (len(g['floats']), sum(len(k) for _, _, k in pending)))
            m['tracks'][ti] = {'times': [x['frame'] for x in keys], 'words': words}
        print('group: %d tracks rebuilt, %d floats' % (len(pending), len(g['floats'])))
out = Path(__file__).resolve().parent / 'files' / 'motion.DBN'
out.write_bytes(c.build(data))
print('wrote %s: %d motions (%s), lean %g degrees' % (out, len(done), ', '.join(done), lean))
