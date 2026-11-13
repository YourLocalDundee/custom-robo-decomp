"""rebind.py: bind a robo body in the pose its motions actually use (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/rebind.py -- <body.glb> <roles json> <out.glb> <motion.glb>...
      [--amount=0.7] [--step=3] [--relax=60] [--roles_only=...] [--report=<json>]

Why: linear skinning stretches the outside of a bend and squashes the inside, more the further a joint has turned from
the bind pose. The AI bodies are bound arms-out with straight elbows and knees, but in the game's motions the upper arms
are never within ~50 degrees of that (typically 63-76), the elbows sit ~95 degrees bent and the knees 70-86
(tools/deform_sweep.py: all the stretched triangles are the blended bands at those joints). Binding in the motions'
typical pose leaves every frame only a small turn from the rest.

Safe in the game: cr_Battle_ApplyRoleMatrices (0x800392B0) copies each computed role matrix straight into the
joint's world matrix (jobj+0x44, flagged user-defined); a role joint's rest rotation only reaches the screen through
its inverse bind matrix (HSDRawViewer --import --skeleton glb --bind glb writes it). Vertices on one joint end up
exactly where they were; only the blended bands change.

Steps
1. every role joint's turn from its parent (deform rotation, parent's inverse x child's) over every --step-th frame
   of the motions; the target is their quaternion mean, --amount of the way from the current bind (0.7: a little
   short of the mean, so the rarer straight poses do not swing far the other way)
2. the armature posed to the targets (each joint turning about its own rest head, children following) and the mesh
   deformed by it (the existing weights)
3. relax: the blended points (strongest weight < 0.98) move to give every edge its original rest length back
   (--relax iterations, Jacobi, half the correction per step, plus a little neighbour averaging); single-joint points
   stay fixed. Vertices that share a position (UV seams) move as one point
4. that pose becomes the rest (the armature's bind), the deformed + relaxed mesh the rest mesh; weights unchanged
"""
import json
import math
import sys
from collections import defaultdict

import bpy
import numpy as np
from mathutils import Matrix, Quaternion, Vector

sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
import common  # noqa: E402
import watch  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
pos = [x for x in a if not x.startswith('--')]
BODY, ROLES, OUT, MOTIONS = pos[0], pos[1], pos[2], pos[3:]
AMOUNT = float(opts.get('amount') or 0.7)
STEP = int(opts.get('step') or 3)
RELAX = int(opts.get('relax') or 100)
LAP = float(opts.get('lap') or 0.01)
roles = json.load(open(ROLES, encoding='utf-8'))['C']
PAR = {'head': 'upper_body', 'lower_body': 'upper_body'}
for s in 'ab':
    PAR.update({f'arm_{s}_shoulder': 'upper_body', f'arm_{s}_upper': f'arm_{s}_shoulder', f'arm_{s}_fore': f'arm_{s}_upper',
                f'arm_{s}_hand': f'arm_{s}_fore', f'leg_{s}_thigh': 'lower_body', f'leg_{s}_shin': f'leg_{s}_thigh',
                f'leg_{s}_ankle': f'leg_{s}_shin', f'leg_{s}_foot': f'leg_{s}_ankle'})
ONLY = set((opts.get('roles_only') or '').split(',')) - {''}
ORDER = ['upper_body', 'head', 'lower_body'] + [f'{p}_{s}_{q}' for s in 'ab' for p, qs in
                                                (('arm', ('shoulder', 'upper', 'fore', 'hand')),
                                                 ('leg', ('thigh', 'shin', 'ankle', 'foot'))) for q in qs]
ORDER = [r for r in ORDER if r in roles and roles[r] is not None]

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
B = lambda r: arm.pose.bones[f'JOBJ_{roles[r]}']


def deform(pb):
    return pb.matrix @ pb.bone.matrix_local.inverted()


# 1. the turns
samples = defaultdict(list)
arm.animation_data_create()
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
        for r in ORDER:
            if r not in PAR:
                continue
            q = (deform(B(PAR[r])).to_quaternion().inverted() @ deform(B(r)).to_quaternion()).normalized()
            samples[r].append(q)
arm.animation_data.action = None
for pb in arm.pose.bones:
    pb.matrix_basis = Matrix.Identity(4)
bpy.context.view_layer.update()


def qmean(qs):
    ref = qs[0]
    acc = np.zeros(4)
    for q in qs:
        v = np.array(tuple(q))
        acc += v if np.dot(v, np.array(tuple(ref))) >= 0 else -v
    acc /= np.linalg.norm(acc)
    return Quaternion(tuple(acc))


target = {}
report = {}
for r in ORDER:
    if r not in PAR or not samples[r] or (ONLY and r not in ONLY):
        continue
    m = qmean(samples[r])
    t = Quaternion().slerp(m, AMOUNT)
    ang = [math.degrees(m.rotation_difference(q).angle) for q in samples[r]]
    ang0 = [math.degrees(q.angle) for q in samples[r]]
    ang.sort(); ang0.sort()
    tt = [math.degrees(t.rotation_difference(q).angle) for q in samples[r]]
    tt.sort()
    target[r] = t
    report[r] = {'bind_median': ang0[len(ang0) // 2], 'bind_p90': ang0[int(len(ang0) * .9)],
                 'new_median': tt[len(tt) // 2], 'new_p90': tt[int(len(tt) * .9)], 'turn': math.degrees(t.angle)}
    print(f'rebind: {r:16s} bind: median {ang0[len(ang0)//2]:6.1f} p90 {ang0[int(len(ang0)*.9)]:6.1f}   '
          f'new: median {tt[len(tt)//2]:6.1f} p90 {tt[int(len(tt)*.9)]:6.1f}   (rest turned {math.degrees(t.angle):5.1f})')

# --watch=<dir> (owner, Nov 12: watch edits live): a 4-view snapshot at every stage, written as it happens; the relax
# every 20 iterations, coloured by its stretch against the rest edge lengths
WATCH = None
if opts.get('watch'):
    WATCH = watch.Watcher(opts['watch'], size=int(opts.get('watch_size') or 260))
    WATCH.snap('00_bind_before', [ob])


def snap_points(name, Xp, pid_, faces_):
    if WATCH is None:
        return
    V = Xp[pid_]
    me_ = bpy.data.meshes.new(name)
    me_.from_pydata([tuple(v) for v in V], [], faces_)
    me_.update()
    o_ = bpy.data.objects.new(name, me_)
    bpy.context.scene.collection.objects.link(o_)
    hid = ob.hide_render
    ob.hide_render = True
    L_ = [[np.linalg.norm(V[f[(i + 1) % len(f)]] - V[f[i]]) / max(np.linalg.norm(REST_V[f[(i + 1) % len(f)]] - REST_V[f[i]]), 1e-6)
           for i in range(len(f))] for f in faces_]
    WATCH.snap(name, [o_], heat=watch.heat_from_ratio([max(x) for x in L_], [min(x) for x in L_]))
    ob.hide_render = hid
    bpy.data.objects.remove(o_, do_unlink=True)
    bpy.data.meshes.remove(me_)


# 2. pose: deform(child) = deform(parent) x (the turn about the child's rest head)
D = {}
for r in ORDER:
    pb = B(r)
    if r in PAR and PAR[r] in D:
        Dp = D[PAR[r]]
    else:
        Dp = Matrix.Identity(4)
    h = arm.matrix_world @ pb.bone.head_local
    q = target.get(r, Quaternion())
    Dl = Matrix.Translation(h) @ q.to_matrix().to_4x4() @ Matrix.Translation(-h)
    D[r] = Dp @ Dl if (r in PAR) else Dl
    pb.matrix = arm.matrix_world.inverted() @ D[r] @ arm.matrix_world @ pb.bone.matrix_local
    bpy.context.view_layer.update()
# non-role bones keep their local pose (identity basis): they follow their role parent
mw = ob.matrix_world.copy()
rest = np.array([tuple(mw @ v.co) for v in me.vertices])
dg = bpy.context.evaluated_depsgraph_get()
pm = ob.evaluated_get(dg).to_mesh()
posed = np.array([tuple(mw @ v.co) for v in pm.vertices])
ob.evaluated_get(dg).to_mesh_clear()

# 3. relax the blended bands to their rest edge lengths
gname = {g.index: g.name for g in ob.vertex_groups}
wmax = np.array([max((g.weight for g in v.groups), default=1.0) for v in me.vertices])
key = {}
pid = np.zeros(len(rest), np.int64)
for i, p in enumerate(rest):
    pid[i] = key.setdefault(tuple(np.round(p * 1e5).astype(np.int64)), len(key))
P = len(key)
X = np.zeros((P, 3)); X0 = np.zeros((P, 3)); cnt = np.zeros(P)
np.add.at(X, pid, posed); np.add.at(X0, pid, rest); np.add.at(cnt, pid, 1)
X /= cnt[:, None]; X0 /= cnt[:, None]
pw = np.ones(P)
np.minimum.at(pw, pid, wmax)
free = pw < 0.98
edges = set()
for e in me.edges:
    p, q = pid[e.vertices[0]], pid[e.vertices[1]]
    if p != q:
        edges.add((min(p, q), max(p, q)))
E = np.array(sorted(edges))
L0 = np.linalg.norm(X0[E[:, 0]] - X0[E[:, 1]], axis=1)


def stretch_stats(Y):
    L = np.linalg.norm(Y[E[:, 0]] - Y[E[:, 1]], axis=1) / np.maximum(L0, 1e-6)
    ok = L0 > 0.003
    return float(((L > 1.6) | (L < 0.6))[ok].mean() * 100), float(np.abs(np.log(L[ok])).mean())


before = stretch_stats(X)
REST_V = rest
FACES = [tuple(p.vertices) for p in me.polygons]
snap_points('01_posed_before_relax', X, pid, FACES)
deg = np.zeros(P); np.add.at(deg, E[:, 0], 1); np.add.at(deg, E[:, 1], 1)
for it in range(RELAX):
    d = X[E[:, 1]] - X[E[:, 0]]
    L = np.maximum(np.linalg.norm(d, axis=1), 1e-9)
    corr = (d / L[:, None]) * ((L - L0) * 0.5)[:, None]
    acc = np.zeros_like(X)
    np.add.at(acc, E[:, 0], corr)
    np.add.at(acc, E[:, 1], -corr)
    nb = np.zeros_like(X)
    np.add.at(nb, E[:, 0], X[E[:, 1]]); np.add.at(nb, E[:, 1], X[E[:, 0]])
    lap = nb / np.maximum(deg, 1)[:, None] - X
    step = acc / np.maximum(deg, 1)[:, None] * 0.9 + lap * LAP
    X[free] += step[free]
    if WATCH is not None and (it + 1) % 20 == 0:
        snap_points(f'02_relax_{it + 1:03d}', X, pid, FACES)
after = stretch_stats(X)
print(f'rebind: {int(free.sum())} blended points relaxed ({RELAX} iterations); in the new rest pose '
      f'bad edges {before[0]:.2f}% -> {after[0]:.2f}%, mean |log stretch| {before[1]:.4f} -> {after[1]:.4f}')
report['_relax'] = {'bad_before': before[0], 'bad_after': after[0]}

# 4. the pose becomes the rest
new_co = X[pid]
inv = mw.inverted()
for m_ in list(ob.modifiers):
    if m_.type == 'ARMATURE':
        ob.modifiers.remove(m_)
for v in me.vertices:
    v.co = inv @ Vector(tuple(new_co[v.index]))
me.update()
bpy.context.view_layer.objects.active = arm
bpy.ops.object.mode_set(mode='POSE')
bpy.ops.pose.select_all(action='SELECT')
bpy.ops.pose.armature_apply(selected=False)
bpy.ops.object.mode_set(mode='OBJECT')
mod = ob.modifiers.new('Armature', 'ARMATURE')
mod.object = arm
if ob.parent is None:
    ob.parent = arm
for act in actions:
    act.use_fake_user = False
    bpy.data.actions.remove(act)
if opts.get('report'):
    json.dump(report, open(opts['report'], 'w'), indent=1)
common.save(OUT)
