"""slim_arms.py: pull the arm armour in where the motions push it into the chest or head (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/slim_arms.py -- <body.glb> <roles json> <out.glb> <motion.glb>...
      [--step=4] [--max=0.04] [--margin=0.01] [--smooth=4] [--near=0.12]

The deform sweep's remaining clipping is the upper arms and their shoulder plates (sode) going into the chest and the
helmet. Carving the chest stopped helping once the body was rebound (tools/blender/rebind.py): with the arms bound
hanging, the chest-side carve folded the armpit. This works on the arm instead: every --step-th frame of every motion,
each arm vertex (strongest role arm_*_shoulder / upper / fore) that is inside the posed chest, hips or head (ray parity,
as deform_check.py) records its depth (distance to that surface). Then in the rest pose each such point moves toward its
own bone's axis (the line from its role joint to the next joint down the arm) by depth + --margin, at most --max,
smoothed over --smooth rings so the plates thin without creases. Single-joint points stay rigid with their bone, so the
change carries over to every frame. UV seams move as one point; weights, UVs and textures are kept.
"""
import json
import sys
from collections import defaultdict

import bpy
import numpy as np
from mathutils import Vector
from mathutils.bvhtree import BVHTree

sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
import common  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
pos = [x for x in a if not x.startswith('--')]
BODY, ROLES, OUT, MOTIONS = pos[0], pos[1], pos[2], pos[3:]
STEP = int(opts.get('step') or 4)
MAXD = float(opts.get('max') or 0.04)
MARGIN = float(opts.get('margin') or 0.01)
SMOOTH = int(opts.get('smooth') or 4)
NEAR = float(opts.get('near') or 0.12)
roles = json.load(open(ROLES, encoding='utf-8'))['C']
role_of = {f'JOBJ_{v}': k for k, v in roles.items() if v is not None}
NEXT = {}
for s in 'ab':
    NEXT.update({f'arm_{s}_shoulder': f'arm_{s}_upper', f'arm_{s}_upper': f'arm_{s}_fore', f'arm_{s}_fore': f'arm_{s}_hand'})

bpy.ops.wm.read_factory_settings(use_empty=True)
bpy.context.scene.render.fps = 30
actions = []
for mp in MOTIONS:
    bpy.ops.import_scene.gltf(filepath=mp)
    act = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0].animation_data.action
    act.use_fake_user = True
    actions.append(act)
    for o in list(bpy.data.objects):
        bpy.data.objects.remove(o, do_unlink=True)
bpy.ops.import_scene.gltf(filepath=BODY)
arm = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0]
meshes = common.model_meshes()
assert len(meshes) == 1, 'one body mesh expected'
ob = meshes[0]
me = ob.data
gname = {g.index: g.name for g in ob.vertex_groups}


def vrole(v):
    if not v.groups:
        return ''
    g = max(v.groups, key=lambda x: x.weight)
    b = arm.data.bones.get(gname[g.group])
    while b is not None and b.name not in role_of:
        b = b.parent
    return role_of.get(b.name, '') if b is not None else ''


R = [vrole(v) for v in me.vertices]
is_arm = np.array([r in NEXT for r in R])
is_body = np.array([r in ('upper_body', 'lower_body', 'head') for r in R])
me.calc_loop_triangles()
tris = [tuple(t.vertices) for t in me.loop_triangles]
body_tris = [t for t in tris if all(is_body[i] for i in t)]
arm_idx = np.nonzero(is_arm)[0]
RAYS = [Vector((1, 0.013, 0.007)).normalized(), Vector((-0.011, 1, 0.017)).normalized(),
        Vector((0.009, -0.014, 1)).normalized()]


def inside(tree, p):
    odd = 0
    for dv in RAYS:
        hits, o_ = 0, p.copy()
        for _ in range(16):
            h = tree.ray_cast(o_, dv, 2.0)
            if h[0] is None:
                break
            hits += 1
            o_ = h[0] + dv * 1e-4
        odd += hits % 2
    return odd >= 2


depth = np.zeros(len(me.vertices))
mw = ob.matrix_world
arm.animation_data_create()
nfr = 0
for act in actions:
    arm.animation_data.action = act
    try:
        arm.animation_data.action_slot = act.slots[0]
    except (AttributeError, IndexError):
        pass
    f0, f1 = (int(x) for x in act.frame_range)
    for fr in range(f0, f1 + 1, STEP):
        bpy.context.scene.frame_set(fr)
        bpy.context.view_layer.update()
        dg = bpy.context.evaluated_depsgraph_get()
        pm = ob.evaluated_get(dg).to_mesh()
        P = [mw @ v.co for v in pm.vertices]
        ob.evaluated_get(dg).to_mesh_clear()
        tree = BVHTree.FromPolygons([tuple(p) for p in P], body_tris, all_triangles=True)
        nfr += 1
        for i in arm_idx:
            p = P[int(i)]
            loc, nor, fi, dist = tree.find_nearest(p, NEAR)
            if loc is None or dist <= depth[i]:
                continue
            if inside(tree, p):
                depth[i] = dist
print(f'slim_arms: {nfr} frames; {int((depth > 0).sum())} arm vertices went inside the body, deepest {depth.max():.3f} m')

# points (UV seams), smoothing of the wanted move over the arm
rest = np.array([tuple(mw @ v.co) for v in me.vertices])
key = {}
pid = np.zeros(len(rest), np.int64)
for i, p in enumerate(rest):
    pid[i] = key.setdefault(tuple(np.round(p * 1e5).astype(np.int64)), len(key))
NP = len(key)
want = np.zeros(NP)
np.maximum.at(want, pid, np.where(depth > 0, np.minimum(depth + MARGIN, MAXD), 0.0))
parm = np.zeros(NP, bool)
np.logical_or.at(parm, pid, is_arm)
nbr = defaultdict(set)
for e in me.edges:
    p, q = pid[e.vertices[0]], pid[e.vertices[1]]
    if p != q:
        nbr[p].add(q)
        nbr[q].add(p)
for _ in range(SMOOTH):
    nw = want.copy()
    for p in np.nonzero(parm)[0]:
        if nbr[p]:
            m = np.mean([want[q] for q in nbr[p]])
            nw[p] = max(want[p], m * 0.8)
    want = np.minimum(nw, MAXD)


def head(role):
    return Vector(tuple(arm.matrix_world @ arm.data.bones[f'JOBJ_{roles[role]}'].head_local)) if roles.get(role) is not None else None


moved = 0
inv = mw.inverted()
for v in me.vertices:
    p = pid[v.index]
    r = R[v.index]
    if want[p] <= 1e-4 or r not in NEXT:
        continue
    a0, a1 = head(r), head(NEXT[r])
    if a0 is None or a1 is None:
        continue
    x = Vector(tuple(rest[v.index]))
    ax = (a1 - a0)
    t = max(0.0, min(1.0, (x - a0).dot(ax) / max(ax.length_squared, 1e-9)))
    c = a0 + ax * t
    d = x - c
    if d.length < 1e-4:
        continue
    step = min(float(want[p]), d.length * 0.5)       # never past half way to the bone
    v.co = inv @ (x - d.normalized() * step)
    moved += 1
me.update()
print(f'slim_arms: moved {moved} arm vertices toward their bones (max {MAXD} m, smoothed {SMOOTH} rings)')
arm.animation_data.action = None
for act in actions:
    act.use_fake_user = False
    bpy.data.actions.remove(act)
common.save(OUT)
