"""plate_body.py: rebuild a skinned robo body the way the game's own robos are built (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/plate_body.py -- <body.glb> <roles json> <out.glb> <motion.glb>
      [--ball=1] [--ball_min=0.03] [--ball_max=0.10] [--ball_pct=60] [--cap=1] [--watch=<dir>]

What the game does (Ray 01, chara_000.dat, measured Nov 12): 18 meshes, 17 of them 100% rigid, one per joint (head,
chest, hips, each thigh / shin / ankle / foot / upper arm / forearm / hand); 96% of its vertices follow exactly one
joint. Only a 210-triangle connector is skinned (waist, neck, the hip and shoulder sockets, weights 0.4 / 0.6). Elbows,
knees, wrists and ankles have no skin: rigid shells overlap around mechanical hinge discs. On the deform sweep that
gives stretch 200-950 and squash ~0 per motion, where the fully skinned samurai had 760-7,900 and 400-21,000.

This tool turns a skinned body into that architecture:
1. pieces: every limb joint's piece (arm upper / fore / hand, leg thigh / shin / ankle / foot, per side) is the set of
   triangles whose corners mostly belong to it (strongest weight); the rest is the trunk (chest + hips + head, with
   the shoulder (clavicle) joints folded into the chest)
2. the mesh is cut along the piece boundaries (each side gets its own vertices)
3. weights: a limb piece is 100% rigid on its joint; the trunk keeps its own blend among chest / hips / head only
   (the game's soft connector: waist and neck)
4. caps (--cap): every cut end is closed with a fan to its centre, textured with the darkest spot of the body's own
   atlas, so no shell is see-through
5. ball joints (--ball): an icosphere at each shoulder, elbow, wrist, hip, knee and ankle pivot, rigid on the PARENT
   piece's joint, radius from the cut ends nearest the pivot (--ball_pct percentile of their distances, clamped: the
   limb's inner core; sized from the armour's outer rim they bulged like cannonballs); wrists / ankles fixed sizes. The
   pivot is where the game puts the child joint, measured in the parent piece's rest space from <motion.glb> (baked
   against this body): exactly the bone head for shoulders / elbows / hips / knees (drift 0.0000 m), a point of the
   forearm / shin for wrists and ankles (the game places hands and feet by its own offsets, not the bind's)
Rigid pieces cannot stretch or squash; what remains is the trunk connector.
"""
import json
import os
import sys
from collections import Counter, defaultdict

import bmesh
import bpy
import numpy as np
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
SRC, ROLES, DST, MOTION = [x for x in a if not x.startswith('--')][:4]
BALL = opts.get('ball', '1') != '0'
CAP = opts.get('cap', '0') != '0'      # off: the funnels close the shells
BMIN, BMAX = float(opts.get('ball_min') or 0.03), float(opts.get('ball_max') or 0.065)
BPCT = float(opts.get('ball_pct') or 15)      # the limb's inner core at the hinge, not the armour's outer rim
FIXED = {'wrist': float(opts.get('wrist') or 0.04), 'ankle': float(opts.get('ankle') or 0.05)}   # no cut there (hands / feet were pieces already)
roles = json.load(open(ROLES, encoding='utf-8'))['C']
rb = {f'JOBJ_{v}': k for k, v in roles.items() if v is not None}
TRUNK = {'upper_body', 'lower_body', 'head', 'arm_a_shoulder', 'arm_b_shoulder'}
JOINTS = []          # (name, parent role, child role)
for s in 'ab':
    JOINTS += [(f'shoulder_{s}', f'arm_{s}_shoulder', f'arm_{s}_upper'), (f'elbow_{s}', f'arm_{s}_upper', f'arm_{s}_fore'),
               (f'wrist_{s}', f'arm_{s}_fore', f'arm_{s}_hand'), (f'hip_{s}', 'lower_body', f'leg_{s}_thigh'),
               (f'knee_{s}', f'leg_{s}_thigh', f'leg_{s}_shin'), (f'ankle_{s}', f'leg_{s}_shin', f'leg_{s}_ankle')]

# ---- the pivots, from the motion (before the body is loaded for editing)
bpy.ops.wm.read_factory_settings(use_empty=True)
bpy.context.scene.render.fps = 30
bpy.ops.import_scene.gltf(filepath=MOTION)
act = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0].animation_data.action
act.use_fake_user = True
for o in list(bpy.data.objects):
    bpy.data.objects.remove(o, do_unlink=True)
bpy.ops.import_scene.gltf(filepath=SRC)
arm0 = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0]
arm0.animation_data_create()
arm0.animation_data.action = act
try:
    arm0.animation_data.action_slot = act.slots[0]
except (AttributeError, IndexError):
    pass
f0, f1 = (int(x) for x in act.frame_range)
PIVOT = {}
acc = defaultdict(list)
for fr in range(f0, f1 + 1, max(1, (f1 - f0) // 8)):
    bpy.context.scene.frame_set(fr)
    bpy.context.view_layer.update()
    for name, p, c in JOINTS:
        bp, bc = arm0.pose.bones[f'JOBJ_{roles[p]}'], arm0.pose.bones[f'JOBJ_{roles[c]}']
        Dp = bp.matrix @ bp.bone.matrix_local.inverted()
        acc[name].append(arm0.matrix_world @ (Dp.inverted() @ bc.matrix.translation))
for name, pts in acc.items():
    m = sum(pts, Vector()) / len(pts)
    PIVOT[name] = m
    print(f'plate_body: pivot {name:11s} {[round(x, 3) for x in m]} (spread {max((q - m).length for q in pts):.4f} m)')
bpy.data.actions.remove(act)

# ---- the body
arm = common.load(SRC)
ob = common.model_meshes()[0]
me = ob.data
names = [g.name for g in ob.vertex_groups]
for r, jn in roles.items():
    if jn is not None and f'JOBJ_{jn}' not in names and f'JOBJ_{jn}' in arm.data.bones:
        ob.vertex_groups.new(name=f'JOBJ_{jn}')
        names.append(f'JOBJ_{jn}')
role_of = []
for n in names:
    b = arm.data.bones.get(n)
    while b is not None and b.name not in rb:
        b = b.parent
    role_of.append(rb[b.name] if b is not None else 'upper_body')
gidx = {n: i for i, n in enumerate(names)}
role_joint = {r: gidx[f'JOBJ_{jn}'] for r, jn in roles.items() if jn is not None and f'JOBJ_{jn}' in gidx}


def piece_of_role(r):
    return 'trunk' if r in TRUNK else r


# the darkest spot of each material's image (caps and balls are textured with it)
DARK = {}
for mi, mat in enumerate(me.materials):
    uv = (0.5, 0.5)
    img = None
    if mat and mat.use_nodes:
        for nd in mat.node_tree.nodes:
            if nd.type == 'TEX_IMAGE' and nd.image:
                img = nd.image
                break
    if img is not None and img.size[0] > 0:
        w_, h_ = img.size
        px = np.array(img.pixels[:], np.float32).reshape(h_, w_, -1)
        lum = px[:, :, :3].mean(2)
        k = max(2, w_ // 64)
        hh, ww = (h_ // k) * k, (w_ // k) * k
        blk = lum[:hh, :ww].reshape(hh // k, k, ww // k, k)
        score = blk.mean((1, 3)) + blk.std((1, 3)) * 2          # dark and even
        by, bx = np.unravel_index(np.argmin(score), score.shape)
        uv = ((bx + 0.5) * k / w_, (by + 0.5) * k / h_)
    DARK[mi] = uv
    print(f'plate_body: material {mi}: dark texel at uv {uv[0]:.3f}, {uv[1]:.3f}')

bm = bmesh.new()
bm.from_mesh(me)
bm.verts.ensure_lookup_table()
bm.faces.ensure_lookup_table()
dl = bm.verts.layers.deform.verify()
uvl = bm.loops.layers.uv.verify()
fpiece = bm.faces.layers.string.new('piece')
fstrad = bm.faces.layers.int.new('straddle')      # 1: its corners belonged to more than one piece (a bridge)
eopen = bm.edges.layers.int.new('orig_open')
for e in bm.edges:
    e[eopen] = 1 if len(e.link_faces) < 2 else 0

# points (vertices at one position) and their strongest joint / piece
key = {}
vpid = {}
for v in bm.verts:
    vpid[v.index] = key.setdefault(tuple(np.round(np.array(v.co) * 1e5).astype(np.int64)), len(key))
PW = np.zeros((len(key), len(names)))
cnt = np.zeros(len(key))
for v in bm.verts:
    for g, w_ in v[dl].items():
        PW[vpid[v.index], g] += w_
    cnt[vpid[v.index]] += 1
PW /= np.maximum(cnt, 1)[:, None]
pdom = PW.argmax(1)
ppiece = [piece_of_role(role_of[j]) for j in pdom]
tri_count = Counter()
for f in bm.faces:
    c = Counter(ppiece[vpid[v.index]] for v in f.verts)
    f[fstrad] = 1 if len(c) > 1 else 0
    top = c.most_common()
    if len(top) > 1 and top[0][1] == top[1][1]:
        pc = ppiece[vpid[max(f.verts, key=lambda v: PW[vpid[v.index]].max()).index]]
    else:
        pc = top[0][0]
    f[fpiece] = pc.encode()
    tri_count[pc] += 1
# smooth cut lines (Nov 12: the first cut followed the old weight boundary triangle by triangle and zigzagged through
# the skirt plates - red spikes in the film): a triangle whose label disagrees with at least two of its edge neighbours
# takes theirs (--smooth_cut passes), then islands under --island triangles join the piece around them
SMOOTH_CUT = int(opts.get('smooth_cut') or 6)
ISLAND = int(opts.get('island') or 16)
pkey = lambda v: vpid[v.index]
edge_faces = defaultdict(list)
for f in bm.faces:
    ks = [pkey(v) for v in f.verts]
    for i in range(len(ks)):
        edge_faces[tuple(sorted((ks[i], ks[(i + 1) % len(ks)])))].append(f)
nbrs = {f.index: [g for ks in [[pkey(v) for v in f.verts]] for i in range(len(ks))
                  for g in edge_faces[tuple(sorted((ks[i], ks[(i + 1) % len(ks)])))] if g is not f] for f in bm.faces}
for _ in range(SMOOTH_CUT):
    flips = []
    for f in bm.faces:
        c = Counter(g[fpiece] for g in nbrs[f.index])
        if c:
            lab, n = c.most_common(1)[0]
            if lab != f[fpiece] and n >= 2:
                flips.append((f, lab))
    for f, lab in flips:
        f[fpiece] = lab
    if not flips:
        break
seen = set()
merged = 0
for f in bm.faces:
    if f.index in seen:
        continue
    lab = f[fpiece]
    comp, stack, border = [], [f], Counter()
    seen.add(f.index)
    while stack:
        x = stack.pop()
        comp.append(x)
        for g in nbrs[x.index]:
            if g[fpiece] == lab:
                if g.index not in seen:
                    seen.add(g.index)
                    stack.append(g)
            else:
                border[g[fpiece]] += 1
    if len(comp) < ISLAND and border:
        to = border.most_common(1)[0][0]
        for x in comp:
            x[fpiece] = to
        merged += 1
tri_count = Counter(f[fpiece].decode() for f in bm.faces)
print(f'plate_body: cut lines smoothed, {merged} small islands merged')
print('plate_body: pieces', dict(tri_count.most_common()))

# ---- cut: each limb piece gets its own vertices
for pc in [p for p in tri_count if p != 'trunk']:
    faces = [f for f in bm.faces if f[fpiece] == pc.encode()]
    bmesh.ops.split(bm, geom=faces)
bm.verts.ensure_lookup_table()
bm.edges.ensure_lookup_table()
bm.faces.ensure_lookup_table()

# ---- weights
mixed = 0
for v in bm.verts:
    pcs = {f[fpiece].decode() for f in v.link_faces}
    if not pcs:
        continue
    if len(pcs) > 1:
        mixed += 1
    pc = sorted(pcs)[0]
    old = dict(v[dl].items())
    v[dl].clear()
    if pc == 'trunk':
        keep = {g: w_ for g, w_ in old.items() if piece_of_role(role_of[g]) == 'trunk'}
        folded = defaultdict(float)
        for g, w_ in keep.items():
            r = role_of[g]
            folded[role_joint['upper_body'] if r.endswith('_shoulder') else g] += w_
        if not folded:
            hip = arm.data.bones[f'JOBJ_{roles["lower_body"]}'].head_local
            folded[role_joint['lower_body' if v.co.z < hip.z else 'upper_body']] = 1.0
        t = sum(folded.values())
        for g, w_ in folded.items():
            v[dl][g] = w_ / t
    else:
        own = {g: w_ for g, w_ in old.items() if role_of[g] == pc}
        v[dl][max(own, key=own.get) if own else role_joint[pc]] = 1.0
if mixed:
    print(f'plate_body: warning: {mixed} vertices still shared by two pieces')

# ---- funnels (Nov 12, close-up film: the triangles that used to bridge a joint stuck out as black shards once they
# were rigid on one side): every vertex on a cut edge moves onto its joint's ball, just under the surface, so each
# shell tapers into the ball. Both rims then ride on one sphere about the pivot: nothing can stick out and no gap
# can open, at any angle; the taper also closes the shell (no cap needed)
FUNNEL = opts.get('funnel', '1') != '0'
FUN_NEAR = float(opts.get('fun_near') or 0.11)      # metres from the pivot
FUN_K = float(opts.get('fun_k') or 2.4)             # or this many ball radii
wmi_ = ob.matrix_world.inverted()
PIV_L = {n: wmi_ @ p_ for n, p_ in PIVOT.items()}
RAD = {}
pre_cut = defaultdict(list)
for e in bm.edges:
    if len(e.link_faces) == 1 and e[eopen] == 0:
        pre_cut[e.link_faces[0][fpiece].decode()] += [e.verts[0], e.verts[1]]
for name, p_role, c_role in JOINTS:
    piv = PIV_L[name]
    ds = sorted((v.co - piv).length for pc in (c_role, piece_of_role(p_role)) for v in pre_cut.get(pc, [])
                if (v.co - piv).length < 0.2)
    kind = name.split('_')[0]
    if kind in FIXED:
        RAD[name] = FIXED[kind]
    elif len(ds) >= 6:
        RAD[name] = min(BMAX, max(BMIN, ds[int(len(ds) * BPCT / 100)]))
nfun = 0
if FUNNEL:
    done_v = set()
    far_v = set()
    for pc, vs in pre_cut.items():
        cands = [(n, PIV_L[n]) for n, p_role, c_role in JOINTS if n in RAD and pc in (c_role, piece_of_role(p_role))]
        for v in vs:
            if v.index in done_v or not cands:
                continue
            done_v.add(v.index)
            n, piv = min(cands, key=lambda c_: (v.co - c_[1]).length)
            d = v.co - piv
            # hips: only near the joint (further out the skirt plates part from the thigh plates: no joint there);
            # knees / elbows / shoulders: every cut vertex (their bridges all belong to the hinge)
            lim = max(FUN_NEAR, RAD[n] * FUN_K) if n.startswith('hip') else 0.3
            if 1e-6 < d.length < lim:
                v.co = piv + d.normalized() * RAD[n] * 0.9
                nfun += 1
            else:
                far_v.add(v)
    print(f'plate_body: {nfun} cut vertices pulled onto their joint balls')
    # the bridges left at far cuts (skirt <-> thigh plates): membranes between plates that are now separate; rigid
    # on one side they stood out as shards (close-up film), so they go
    dead = {f for v in far_v if v.is_valid for f in v.link_faces if f[fstrad]}
    if dead:
        bmesh.ops.delete(bm, geom=list(dead), context='FACES')
        bm.verts.ensure_lookup_table(); bm.edges.ensure_lookup_table(); bm.faces.ensure_lookup_table()
    print(f'plate_body: {len(dead)} bridge triangles between separate plates removed')

# ---- caps: every cut end closed with a fan to its centre
cut_by_piece = defaultdict(list)
for e in bm.edges:
    if len(e.link_faces) == 1 and e[eopen] == 0:
        cut_by_piece[e.link_faces[0][fpiece].decode()].append(e)
print('plate_body: cut edges', {k: len(v) for k, v in cut_by_piece.items()})
cut_pts = defaultdict(list)          # piece -> positions of its cut ends (for the ball radii)
ncap = 0
skipped = 0
for pc, edges in cut_by_piece.items():
    for e in edges:
        cut_pts[pc] += [e.verts[0].co.copy(), e.verts[1].co.copy()]
    if not CAP:
        continue
    par = {}

    def find(x):
        while par.setdefault(x, x) != x:
            par[x] = par[par[x]]
            x = par[x]
        return x
    kk = lambda v: tuple(np.round(np.array(v.co) * 1e4).astype(np.int64))
    for e in edges:
        par[find(kk(e.verts[0]))] = find(kk(e.verts[1]))
    comp = defaultdict(list)
    for e in edges:
        comp[find(kk(e.verts[0]))].append(e)
    for es in comp.values():
        if len(es) < 3:
            continue
        # only a clean closed ring gets a cap (Nov 12 film: fans over open / branching cuts made black starbursts at
        # the knees and hips): every point of it on exactly two cut edges, and the ring roughly flat and round
        deg = Counter()
        for e in es:
            deg[kk(e.verts[0])] += 1
            deg[kk(e.verts[1])] += 1
        if any(d != 2 for d in deg.values()):
            skipped += 1
            continue
        cen = sum((e.verts[0].co + e.verts[1].co for e in es), Vector()) / (2 * len(es))
        P_ = np.array([tuple(e.verts[0].co - cen) for e in es])
        sv = np.linalg.svd(P_, compute_uv=False)
        if sv[0] < 1e-6 or sv[2] / sv[0] > 0.45 or sv[1] / sv[0] < 0.25:
            skipped += 1
            continue
        cv = bm.verts.new(cen)
        src = es[0].verts[0]
        for g, w_ in src[dl].items():
            cv[dl][g] = w_
        for e in es:
            f0_ = e.link_faces[0]
            l = next(l_ for l_ in f0_.loops if l_.edge == e)
            v1, v2 = l.vert, l.link_loop_next.vert            # the piece's face runs v1 -> v2: the cap runs v2 -> v1
            try:
                nf = bm.faces.new((v2, v1, cv))
            except ValueError:
                continue
            nf.material_index = f0_.material_index
            nf[fpiece] = pc.encode()
            nf.smooth = False
            for l_ in nf.loops:
                l_[uvl].uv = DARK.get(f0_.material_index, (0.5, 0.5))
            ncap += 1
print(f'plate_body: {ncap} cap triangles ({skipped} cuts left open: not a clean ring)')

# ---- ball joints
nball = 0
if BALL:
    mat_default = Counter(f.material_index for f in bm.faces).most_common(1)[0][0]
    wm = ob.matrix_world
    wmi = wm.inverted()
    for name, p_role, c_role in JOINTS:
        piv = wmi @ PIVOT[name]
        ds = sorted((q - piv).length for pc in (c_role, piece_of_role(p_role)) for q in cut_pts.get(pc, [])
                    if (q - piv).length < 0.2)
        if name not in RAD:
            print(f'plate_body: {name}: no cut near the pivot, no ball')
            continue
        r = RAD[name]
        ds = ds or [r]
        res = bmesh.ops.create_icosphere(bm, subdivisions=2, radius=r, matrix=Matrix.Translation(piv), calc_uvs=False)
        bone = role_joint['upper_body'] if p_role.endswith('_shoulder') else role_joint[p_role]
        for v in res['verts']:
            v[dl].clear()
            v[dl][bone] = 1.0
        fs = {f for v in res['verts'] for f in v.link_faces}
        for f in fs:
            f.material_index = mat_default
            f[fpiece] = b'ball'
            f.smooth = True
            for l_ in f.loops:
                l_[uvl].uv = DARK.get(mat_default, (0.5, 0.5))
        nball += len(fs)
        print(f'plate_body: ball {name:11s} radius {r:.3f} ({len(ds)} cut points within 0.2 m, median {ds[len(ds) // 2]:.3f}) on {names[bone]}')
bm.normal_update()
bm.to_mesh(me)
bm.free()
me.update()
print(f'plate_body: {len(me.polygons)} triangles ({ncap} caps, {nball} ball), {len(me.vertices)} vertices')
if opts.get('watch'):
    import watch
    w = watch.Watcher(opts['watch'], size=420, textured=True)
    w.snap('plated_rest', [ob])
common.save(DST)
