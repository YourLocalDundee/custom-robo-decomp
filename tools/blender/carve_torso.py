"""carve_torso.py: make room in a bulky chest for the arms (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/carve_torso.py -- <body.glb> <roles json> <out.glb> <motion.glb>...
      [--step=4] [--max=0.05] [--margin=0.012] [--smooth=6]

The samurai's arms hang from Ray 01's narrow shoulder joints inside a much wider chest, so in the motions the upper
arms and their shoulder plates slide into the chest armour (tools/deform_sweep.py: almost all remaining clipping is
arm_*_upper in the torso). This takes every arm vertex at every --step-th frame of every motion, maps it into the
chest's rest space (the inverse of the upper_body joint's pose), and where such a point is inside the torso
(ray parity, as deform_check.py) pushes the nearest torso vertices inward along their normal by its depth + --margin
(at most --max), smoothed over --smooth rings so no dents have edges. Only the mesh moves (UVs, weights and
textures are kept); vertices sharing a position (UV seams) move together.
"""
import math
import sys
from collections import defaultdict

import bpy
import numpy as np
from mathutils import Matrix, Vector
from mathutils.bvhtree import BVHTree
from mathutils.kdtree import KDTree

sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
import common  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
pos = [x for x in a if not x.startswith('--')]
BODY, ROLES, OUT, MOTIONS = pos[0], pos[1], pos[2], pos[3:]
STEP = int(opts.get('step') or 4)
MAXD = float(opts.get('max') or 0.035)
MARGIN = float(opts.get('margin') or 0.012)
SMOOTH = int(opts.get('smooth') or 6)
import json  # noqa: E402
roles = json.load(open(ROLES, encoding='utf-8'))['C']
role_of = {f'JOBJ_{v}': k for k, v in roles.items()}

bpy.ops.wm.read_factory_settings(use_empty=True)
bpy.context.scene.render.fps = 30     # the motion glbs are 30 fps: Blender frame = game frame (default 24 scaled them 0.8)
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


def role(vi):
    v = me.vertices[vi]
    if not v.groups:
        return ''
    g = max(v.groups, key=lambda x: x.weight)
    j = gname[g.group]
    b = arm.data.bones.get(j)
    while b is not None and b.name not in role_of:
        b = b.parent
    return role_of.get(b.name, '') if b is not None else ''


vrole = [role(i) for i in range(len(me.vertices))]
is_arm = np.array([r.startswith('arm_') and not r.endswith('hand') for r in vrole])
is_torso = np.array([r in ('upper_body',) for r in vrole])
me.calc_loop_triangles()
tris = [tuple(t.vertices) for t in me.loop_triangles]
torso_tris = [t for t in tris if all(is_torso[i] for i in t)]
mw = ob.matrix_world
rest = np.array([tuple(mw @ v.co) for v in me.vertices])
tree = BVHTree.FromPolygons([tuple(p) for p in rest], torso_tris, all_triangles=True)
RAYS = [Vector((1, 0.013, 0.007)).normalized(), Vector((-0.011, 1, 0.017)).normalized(),
        Vector((0.009, -0.014, 1)).normalized()]


def inside(p):
    odd = 0
    for dv in RAYS:
        hits, o_ = 0, p.copy()
        for _ in range(16):
            h = tree.ray_cast(o_, dv, 1.0)
            if h[0] is None:
                break
            hits += 1
            o_ = h[0] + dv * 1e-4
        odd += hits % 2
    return odd >= 2


ub = arm.pose.bones[f'JOBJ_{roles["upper_body"]}']
arm.animation_data_create()
cloud = {}
arm_idx = np.nonzero(is_arm)[0]
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
        M = arm.matrix_world @ ub.matrix @ ub.bone.matrix_local.inverted() @ arm.matrix_world.inverted()
        Mi = M.inverted()
        for i in arm_idx:
            q = Mi @ (mw @ pm.vertices[int(i)].co)
            k = (round(q.x / 0.008), round(q.y / 0.008), round(q.z / 0.008))
            cloud[k] = q
        ob.evaluated_get(dg).to_mesh_clear()
print(f'carve_torso: {len(cloud)} arm points (8 mm grid) over {len(actions)} motions')

# depth wanted per torso vertex
kd = KDTree(len(rest))
for i in range(len(rest)):
    if is_torso[i]:
        kd.insert(Vector(rest[i]), i)
kd.balance()
want = np.zeros(len(rest))
n_in = 0
for q in cloud.values():
    loc, nor, fi, dist = tree.find_nearest(q, 0.12)
    if loc is None or not inside(q):
        continue
    n_in += 1
    d = min(MAXD, dist + MARGIN)
    for co, i, r in kd.find_range(loc, 0.035):
        want[i] = max(want[i], d * (1 - r / 0.035) ** 0.5)
print(f'carve_torso: {n_in} arm points inside the chest; {int((want > 0).sum())} torso vertices to move, '
      f'deepest {want.max():.3f} m')

# points (shared positions) and smoothing of the depth
key = {}
pid = np.zeros(len(rest), np.int64)
for i, p in enumerate(rest):
    pid[i] = key.setdefault(tuple(np.round(p * 1e5).astype(np.int64)), len(key))
P = len(key)
pw = np.zeros(P)
np.maximum.at(pw, pid, want)
nbr = defaultdict(set)
for e in me.edges:
    p, q = pid[e.vertices[0]], pid[e.vertices[1]]
    if p != q:
        nbr[p].add(q)
        nbr[q].add(p)
ptorso = np.zeros(P, bool)
np.logical_or.at(ptorso, pid, is_torso)
for _ in range(SMOOTH):
    nw = pw.copy()
    for p in np.nonzero(ptorso)[0]:
        if nbr[p]:
            m = np.mean([pw[q] for q in nbr[p]])
            nw[p] = max(pw[p] * 0.6 + m * 0.4, m * 0.85)
    pw = np.minimum(nw, MAXD)
# move along the point's mean normal, in the mesh's own space
pn = np.zeros((P, 3))
for v in me.vertices:
    pn[pid[v.index]] += np.array(tuple(v.normal))
pn /= np.maximum(np.linalg.norm(pn, axis=1, keepdims=True), 1e-9)
# --sides (default on): only the chest's sides and armpits give way (surfaces facing left / right), never the front,
# the collar or the back: carving those crumpled the collar under the visor (Nov 12 render)
if opts.get('sides', '1') != '0':
    nx = np.abs(pn[:, 0])
    pw *= np.clip((nx - 0.35) / 0.3, 0.0, 1.0)
moved = 0
for v in me.vertices:
    p = pid[v.index]
    if pw[p] > 1e-4 and ptorso[p]:
        v.co = v.co - Vector(tuple(pn[p])) * float(pw[p]) / max(mw.to_scale().x, 1e-9)
        moved += 1
me.update()
print(f'carve_torso: moved {moved} vertices (smoothed {SMOOTH} rings, max {MAXD} m)')
arm.animation_data.action = None
for act in actions:
    act.use_fake_user = False
    bpy.data.actions.remove(act)
common.save(OUT)
