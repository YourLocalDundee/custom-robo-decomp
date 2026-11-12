"""floor_lift.py: per-frame body lifts that keep a mod body above the floor (Nov 12, 2026).

  python -I tools/floor_lift.py <deform sweep dir> <out json> [--slots fall_back,fall_front,lose,charge] [--prev old.json]

Reads deform_check.py's <slot>.json (the lowest body point per checked frame), takes the floor as the stance's lowest
point, and for every frame where a slot goes below it writes the lift that puts it back (interpolated between the
checked frames, plus --prev's lift when iterating). Sweeps made before deform_check.py set the scene to 30 fps
(Nov 12) counted Blender's 24 fps frames: pass --scale=1.25 for those. make_motion.py adds it to every IK point's height of that frame.

Why: the samurai's armour is much thicker than Ray 01's body, so lying / kneeling poses sank shins, ankles and the back
through the floor (owner: "look at the ankles").
"""
import json
import sys
from pathlib import Path

a = [x for x in sys.argv[1:] if not x.startswith('--')]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in sys.argv[1:] if x.startswith('--'))}
src, out = Path(a[0]), Path(a[1])
slots = (opts.get('slots') or 'fall_back,fall_front,lose,charge').split(',')
SCALE = float(opts.get('scale') or 1.0)   # sweeps made before deform_check.py set 30 fps: --scale=1.25
prev = json.load(open(opts['prev'])) if opts.get('prev') else {}
stance = json.load(open(src / 'stance.json'))
floor = sorted(r['low_z'] for r in stance)[len(stance) // 2]
res = {}
for s in slots:
    p = src / f'{s}.json'
    if not p.exists():
        continue
    rows = sorted(json.load(open(p)), key=lambda r: r['frame'])
    fr = [r['frame'] for r in rows]
    need = [max(0.0, floor - r['low_z']) for r in rows]
    last = round(fr[-1] * SCALE)
    lift = {}
    for f in range(last + 1):           # motion frames; the sweep's are Blender's (24 fps scene, 30 fps glb)
        x = min(f / SCALE, fr[-1])
        i = max(k for k in range(len(fr)) if fr[k] <= x)
        j = min(i + 1, len(fr) - 1)
        t = 0.0 if fr[j] == fr[i] else (x - fr[i]) / (fr[j] - fr[i])
        v = need[i] + (need[j] - need[i]) * t
        lift[str(f)] = round(v + prev.get(s, {}).get(str(f), 0.0), 4)
    res[s] = lift
    print(f'{s}: deepest {max(need):.3f} below the floor ({floor:.3f}); lift up to {max(lift.values()):.3f}')
json.dump(res, open(out, 'w'), indent=0)
