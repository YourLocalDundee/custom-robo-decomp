"""reweight.py: smooth a built body's skin weights where joints share it (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/reweight.py -- <in.glb> <out.glb> [--iters=8] [--factor=0.5]
      [--ring=2] [--keep=<joint>,<joint>]

Only the weights change (the mesh, UVs and textures are written back as they were), so a weight idea can be tried in
seconds instead of a full atlas_ai_body.py build. Vertices that share a position (glTF splits vertices along UV
seams) are one point: they get the same weights, so the smoothing never opens a seam.
- the zone: every point whose strongest weight is below 0.98, grown by --ring edge rings
- --keep: joints whose single-weight points stay as they are (default: the hand / ankle joints, which
  atlas_ai_body.py moved to the game's bind positions and must stay rigid)
- per iteration each point in the zone takes (1 - factor) of its own weights + factor x its neighbours' mean;
  at most 4 joints, normalised
- --roles=<roles json> (Nov 12, 2026): weights only along the skeleton. A point keeps only joints whose role is its
  strongest role or a direct neighbour of it (chest - head / shoulders / hips, chest - upper arm (the armpit: Ray's shoulder joint is a short clavicle), shoulder - upper arm - forearm - hand,
  hips - thighs, thigh - shin - ankle - foot); applied every iteration, and once more with --iters=0. The plain
  smoothing spread weights over mesh edges, so the skirt between the legs took both legs and the collar both arms
  and the head (deform check: those triangles tear when the limbs part)

Measure with tools/deform_sweep.py (stretch / clipping per motion) before and after.
"""
import sys
from collections import defaultdict

import bpy
import numpy as np

sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
import common  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
SRC, DST = [x for x in a if not x.startswith('--')][:2]
ITERS = int(opts.get('iters') or 8)
FACTOR = float(opts.get('factor') or 0.5)
RING = int(opts.get('ring') or 2)

arm = common.load(SRC)
meshes = common.model_meshes()
ADJ = {}
role_of_bone = {}
if opts.get('roles'):
    import json
    _roles = json.load(open(opts['roles'], encoding='utf-8'))['C']
    _rb = {f'JOBJ_{v}': k for k, v in _roles.items()}
    for b in arm.data.bones:
        x = b
        while x is not None and x.name not in _rb:
            x = x.parent
        role_of_bone[b.name] = _rb[x.name] if x is not None else ''
    links = [('upper_body', 'head'), ('upper_body', 'lower_body')]
    for s_ in 'ab':
        links += [('upper_body', f'arm_{s_}_shoulder'), ('upper_body', f'arm_{s_}_upper'), (f'arm_{s_}_shoulder', f'arm_{s_}_upper'),
                  (f'arm_{s_}_upper', f'arm_{s_}_fore'), (f'arm_{s_}_fore', f'arm_{s_}_hand'),
                  ('lower_body', f'leg_{s_}_thigh'), (f'leg_{s_}_thigh', f'leg_{s_}_shin'),
                  (f'leg_{s_}_shin', f'leg_{s_}_ankle'), (f'leg_{s_}_ankle', f'leg_{s_}_foot')]
    for r in set(_rb.values()) | {''}:
        ADJ[r] = {r}
    for x, y in links:
        ADJ.setdefault(x, {x}).add(y)
        ADJ.setdefault(y, {y}).add(x)


def prune(rows, names):
    """keep only joints whose role is the row's strongest role or its skeleton neighbour; normalised"""
    if not ADJ:
        return rows, 0
    jr = [role_of_bone.get(n, '') for n in names]
    roles_u = sorted(set(jr))
    ri = {r: i for i, r in enumerate(roles_u)}
    R = np.zeros((rows.shape[0], len(roles_u)))
    for j, r in enumerate(jr):
        R[:, ri[r]] += rows[:, j]
    dom = [roles_u[i] for i in R.argmax(1)]
    mask = np.array([[jr[j] in ADJ.get(d, {d}) for j in range(len(names))] for d in dom])
    off = (rows > 1e-4) & ~mask
    changed = int(off.any(1).sum())
    if opts.get('debug'):
        from collections import Counter
        c = Counter()
        wsum = Counter()
        for i in np.nonzero(off.any(1))[0]:
            for j in np.nonzero(off[i])[0]:
                c[(dom[i], jr[j])] += 1
                wsum[(dom[i], jr[j])] += rows[i, j]
        for k, n in c.most_common(25):
            print(f'reweight: prune {n:5d} x  {k[0]} -/- {k[1]}  mean weight {wsum[k] / n:.3f}')
    rows = rows * mask
    rows /= np.maximum(rows.sum(1, keepdims=True), 1e-9)
    return rows, changed
for o in meshes:
    me = o.data
    names = [g.name for g in o.vertex_groups]
    nv = len(me.vertices)
    W = np.zeros((nv, len(names)))
    for v in me.vertices:
        for g in v.groups:
            W[v.index, g.group] = g.weight
    # points: vertices at one position
    key = {}
    pid = np.zeros(nv, np.int64)
    for v in me.vertices:
        k = tuple(np.round(np.array(v.co) * 1e5).astype(np.int64))
        pid[v.index] = key.setdefault(k, len(key))
    npnt = len(key)
    PW = np.zeros((npnt, len(names)))
    cnt = np.zeros(npnt)
    np.add.at(PW, pid, W)
    np.add.at(cnt, pid, 1)
    PW /= np.maximum(cnt, 1)[:, None]
    nbr = defaultdict(set)
    for e in me.edges:
        p, q = pid[e.vertices[0]], pid[e.vertices[1]]
        if p != q:
            nbr[p].add(q)
            nbr[q].add(p)
    keep_names = set((opts.get('keep') or '').split(',')) - {''}
    if not keep_names:
        # the hand / ankle joints: the joints of points with weight 1.0 that atlas_ai_body moved (rigid pieces);
        # found as joints whose points are all single-weight
        for j, nm in enumerate(names):
            pts = PW[:, j] > 1e-3
            if pts.any() and np.all(PW[pts].max(1) > 0.999) and np.all(np.count_nonzero(PW[pts] > 1e-3, 1) == 1):
                keep_names.add(nm)
    keep_cols = [names.index(n) for n in keep_names if n in names]
    rigid = np.zeros(npnt, bool)
    if keep_cols:
        rigid = PW[:, keep_cols].max(1) > 0.999
    zone = set(np.nonzero(PW.max(1) < 0.98)[0].tolist())
    for _ in range(RING):
        zone |= {q for p in list(zone) for q in nbr[p]}
    zone = np.array(sorted(p for p in zone if not rigid[p]), np.int64)
    print(f'reweight: {o.name}: {nv} vertices, {npnt} points, zone {len(zone)}, kept rigid {int(rigid.sum())} '
          f'({sorted(keep_names)})')
    nb = [np.array(sorted(nbr[p]), np.int64) for p in zone]
    for _ in range(ITERS):
        new = PW.copy()
        for i, p in enumerate(zone):
            if len(nb[i]):
                new[p] = (1 - FACTOR) * PW[p] + FACTOR * PW[nb[i]].mean(0)
        # at most 4 joints, normalised
        sub = new[zone]
        if sub.shape[1] > 4:
            idx = np.argsort(-sub, 1)[:, 4:]
            np.put_along_axis(sub, idx, 0.0, 1)
        sub /= np.maximum(sub.sum(1, keepdims=True), 1e-9)
        new[zone], _ = prune(sub, names)
        PW = new
    PW, n_pruned = prune(PW, names)
    if ADJ:
        print(f'reweight: {o.name}: {n_pruned} points had weights off the skeleton (pruned)')
    # write back: every vertex takes its point's weights
    for j, nm in enumerate(names):
        o.vertex_groups[nm].remove(list(range(nv)))
    for v in me.vertices:
        row = PW[pid[v.index]]
        for j in np.nonzero(row > 1e-4)[0]:
            o.vertex_groups[names[j]].add([v.index], float(row[j]), 'REPLACE')
common.save(DST)
