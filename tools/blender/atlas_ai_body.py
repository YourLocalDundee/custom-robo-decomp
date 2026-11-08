"""Atlas from an AI-generated model: a textured high-poly character (Hunyuan3D 2.1 output, made from a picture drawn on
Atlas's own T-pose silhouette) turned into a Custom Robo body on Ray 01's skeleton.

usage: blender -b --factory-startup --python tools/blender/atlas_ai_body.py -- <in.glb> <out.glb> <roles.json>
       <ai model .glb/.obj> <guide.glb> <guide.pose.json> [triangles 3300] [--scutum=<ai model>] [--gladius=<ai model>]

<in.glb>       Ray 01 (chara_000) exported by HSDRawViewer --export (its skeleton is kept exactly: template mode)
<roles.json>   python -I tools/robo_joints.py roles unpacked/chara/000.bin
<ai model>     the generated character, any scale / facing (the facing is found from the feet)
<guide.glb>    the body the AI picture was drawn on (the legionnaire's edited glb) and its pose.json: the AI model is
               fitted onto the guide's bounding box (hands and feet as drawn in the game)
--scutum / --gladius: AI models of the props (Hunyuan3D from product shots), rigid on the left forearm / in the right
               fist as the legionnaire's; they take 300 / 200 of the triangles
Writes <out.glb>, <out>.args (--replace), <out>.pose.json, <out>.preview.png, <out>.tex_atlas.png.

Steps: fit and face +Y; a copy decimated to the triangle budget (the 0x3A000 body entry: 512x256 atlas + ~3,300
triangles, as the legionnaire); weights from the skeleton's measurements (the legionnaire's chains: torso, neck,
arms, legs; hands / feet rigid on their IK effector joints and moved to the game data's bind positions); the AI
model's colour baked onto the low copy (selected-to-active, emission) times its ambient occlusion, its metallic map
into the specular mask; one material, culled.
"""
import json
import math
import os
import sys

import bmesh
import bpy
import numpy as np
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402
from legionnaire_v2 import chain, smoothstep  # noqa: E402

log = lambda *a: print('atlas_ai_body:', *a)
a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
a = [x for x in a if not x.startswith('--')]
SRC, DST, ROLES, AI, GUIDE, GUIDE_POSE = a[:6]
TRIS = int(opts.get('tris') or (a[6] if len(a) > 6 else 3300))   # --tris=<total>: lower it if the entry is too big
PROP_TRIS = {'scutum': 300, 'gladius': 200}
PROPS = {k: opts[k] for k in PROP_TRIS if opts.get(k)}      # --scutum=<ai model> --gladius=<ai model>
BODY_TRIS = TRIS - sum(PROP_TRIS[k] for k in PROPS)
ATLAS = (512, 256)
BAKE = 1024
roles = json.load(open(ROLES, encoding='utf-8'))['C']

arm = common.load(SRC)
bones = arm.data.bones
old = common.model_meshes()


def bpos(j):
    return arm.matrix_world @ bones[f'JOBJ_{j}'].head_local


mesh_joints = {int(o.name.split('_')[1]) for o in old if o.name.startswith('Joint_')}


def holder(j):
    todo = [bones[f'JOBJ_{j}']]
    while todo:
        b = todo.pop(0)
        k = int(b.name.split('_')[1])
        if k in mesh_joints:
            return k
        todo += sorted((c for c in b.children if c.name.startswith('JOBJ_')), key=lambda c: int(c.name.split('_')[1]))
    return j


boxes = {}
for k in mesh_joints:
    pts = [o.matrix_world @ v.co for o in old if o.name.startswith(f'Joint_{k}_') for v in o.data.vertices]
    boxes[k] = (Vector((min(p.x for p in pts), min(p.y for p in pts), min(p.z for p in pts))),
                Vector((max(p.x for p in pts), max(p.y for p in pts), max(p.z for p in pts))))
J = lambda role: f'JOBJ_{roles[role]}'
H = lambda role: f'JOBJ_{holder(roles[role])}'
side_of = {'a': -1.0, 'b': 1.0}
head_lo, head_hi = boxes[holder(roles['head'])]
hip = {s: bpos(roles[f'leg_{s}_thigh']) for s in 'ab'}
knee = {s: bpos(roles[f'leg_{s}_shin']) for s in 'ab'}
shoulder = {s: bpos(roles[f'arm_{s}_upper']) for s in 'ab'}
elbow = {s: bpos(roles[f'arm_{s}_fore']) for s in 'ab'}
fore = {s: boxes[holder(roles[f'arm_{s}_fore'])] for s in 'ab'}
shin_bottom = min(boxes[holder(roles[f'leg_{s}_shin'])][0].z for s in 'ab')
ground = shin_bottom - 0.08
wrist = {s: Vector((side_of[s] * max(abs(fore[s][0].x), abs(fore[s][1].x)), elbow[s].y, elbow[s].z)) for s in 'ab'}
ankle_z = ground + 0.085
upper_z = bpos(roles['upper_body']).z
log(f'ground {ground:.3f}, ankle {ankle_z:.3f}, hips {hip["a"].z:.3f}, shoulders {shoulder["a"].z:.3f}, head {head_lo.z:.3f}..{head_hi.z:.3f}, wrists {wrist["a"].x:.3f}/{wrist["b"].x:.3f}')

# the IK effectors (as legionnaire_v2): drawn hand / foot -> where the game data keeps them
effector = {}
for s in 'ab':
    hl, hh = boxes[holder(roles[f'arm_{s}_hand'])]
    drawn = wrist[s] + Vector((side_of[s] * 0.045, 0, 0))
    effector[f'arm_{s}'] = Matrix.Translation((hl + hh) / 2 - drawn)
    al, ah = boxes[holder(roles[f'leg_{s}_ankle'])]
    tl, th = boxes[holder(roles[f'leg_{s}_foot'])]
    lo = Vector((min(al.x, tl.x), min(al.y, tl.y), min(al.z, tl.z)))
    hi = Vector((max(ah.x, th.x), max(ah.y, th.y), max(ah.z, th.z)))
    d = (tl + th) / 2 - (al + ah) / 2
    i = max(range(3), key=lambda k: abs(d[k]))
    toe_b = Vector([0, 0, 0]); toe_b[i] = 1 if d[i] > 0 else -1
    up_b = Vector((0, 1, 0))
    x_b = toe_b.cross(up_b)
    heel_b = Vector([((hi[k] if toe_b[k] < 0 else lo[k]) if toe_b[k] else
                      (lo[k] if up_b[k] > 0 else hi[k]) if up_b[k] else (lo[k] + hi[k]) / 2) for k in range(3)])
    heel_d = Vector((hip[s].x, 0.04 - 0.105, ground))
    rot = Matrix.Identity(4)
    for c, ax in enumerate((x_b, toe_b, up_b)):
        for r in range(3):
            rot[r][c] = ax[r]
    effector[f'leg_{s}'] = Matrix.Translation(heel_b) @ rot @ Matrix.Translation(-heel_d)
for o in old:
    bpy.data.objects.remove(o, do_unlink=True)
for m in list(bpy.data.materials):
    bpy.data.materials.remove(m)

# ---- the guide's box (hands / feet where the game draws them)
before = set(bpy.data.objects)
bpy.ops.import_scene.gltf(filepath=GUIDE)
gnew = [o for o in bpy.data.objects if o not in before]
gpose = json.load(open(GUIDE_POSE))
gpts = []
for o in gnew:
    if o.type != 'MESH' or not o.visible_get():
        continue
    names = {g.index: g.name for g in o.vertex_groups}
    for v in o.data.vertices:
        p = o.matrix_world @ v.co
        if v.groups:
            g = max(v.groups, key=lambda x: x.weight)
            bn = names.get(g.group)
            if bn in ('Scutum', 'Gladius'):
                continue
            if bn in gpose:
                p = Matrix(gpose[bn]) @ p
        gpts.append(p)
glo = Vector((min(p.x for p in gpts), min(p.y for p in gpts), min(p.z for p in gpts)))
ghi = Vector((max(p.x for p in gpts), max(p.y for p in gpts), max(p.z for p in gpts)))
for o in gnew:
    bpy.data.objects.remove(o, do_unlink=True)
log(f'guide box {tuple(round(x, 3) for x in glo)} .. {tuple(round(x, 3) for x in ghi)}')

# ---- the AI model: join, face +Y, fit onto the guide box
before = set(bpy.data.objects)
if AI.lower().endswith('.obj'):
    bpy.ops.wm.obj_import(filepath=AI)
else:
    bpy.ops.import_scene.gltf(filepath=AI)
new = [o for o in bpy.data.objects if o not in before and o.type == 'MESH']
for o in bpy.data.objects:
    o.select_set(o in new)
bpy.context.view_layer.objects.active = new[0]
if len(new) > 1:
    bpy.ops.object.join()
hi_ob = bpy.context.view_layer.objects.active
bpy.ops.object.parent_clear(type='CLEAR_KEEP_TRANSFORM')
bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
hi_ob.name = 'AI_high'
me = hi_ob.data


def box(ob):
    ps = [v.co for v in ob.data.vertices]
    return (Vector((min(p.x for p in ps), min(p.y for p in ps), min(p.z for p in ps))),
            Vector((max(p.x for p in ps), max(p.y for p in ps), max(p.z for p in ps))))


lo, hi = box(hi_ob)
ext = hi - lo
# up: the tallest axis must be Z; the arms (the widest of the other two) along X
order = sorted(range(3), key=lambda k: -ext[k])
up_ax = 2 if ext[2] >= max(ext[0], ext[1]) * 0.8 else order[0]
if up_ax != 2:
    m = Matrix.Rotation(math.radians(90), 4, 'X' if up_ax == 1 else 'Y')
    me.transform(m)
    lo, hi = box(hi_ob)
    ext = hi - lo
if ext[1] > ext[0]:
    me.transform(Matrix.Rotation(math.radians(90), 4, 'Z'))
    lo, hi = box(hi_ob)
# front: the toes stick out forward; compare the lowest 4% of the model with its middle
c = (lo + hi) / 2
low = [v.co for v in me.vertices if v.co.z < lo.z + 0.04 * (hi.z - lo.z)]
toe_y = sum(p.y for p in low) / max(1, len(low))
if toe_y < c.y:
    me.transform(Matrix.Rotation(math.radians(180), 4, 'Z'))
    lo, hi = box(hi_ob)
# fit x and z to the guide box, y by the mean scale
sx = (ghi.x - glo.x) / (hi.x - lo.x)
sz = (ghi.z - glo.z) / (hi.z - lo.z)
sy = (sx + sz) / 2
gc = (glo + ghi) / 2
c = (lo + hi) / 2
fit = Matrix.Translation(Vector((gc.x, gc.y, glo.z))) @ Matrix.Diagonal((sx, sy, sz, 1)) @ Matrix.Translation(-Vector((c.x, c.y, lo.z)))
me.transform(fit)
me.update()
log(f'AI model: {len(me.polygons)} faces, scale x {sx:.4f} z {sz:.4f} ({"turned" if toe_y < c.y else "kept"} facing)')

# ---- the props (AI models of their own): placed as the legionnaire's, scutum on the left forearm, gladius in the
# right fist; Ray 01 faces +Y with its left arm (arm_a) at -X
FWD, UP, RIGHT = Vector((0, 1, 0)), Vector((0, 0, 1)), Vector((1, 0, 0))


def load_prop(path, name):
    before = set(bpy.data.objects)
    if path.lower().endswith('.obj'):
        bpy.ops.wm.obj_import(filepath=path)
    else:
        bpy.ops.import_scene.gltf(filepath=path)
    new = [o for o in bpy.data.objects if o not in before and o.type == 'MESH']
    for o in bpy.data.objects:
        o.select_set(o in new)
    bpy.context.view_layer.objects.active = new[0]
    if len(new) > 1:
        bpy.ops.object.join()
    ob = bpy.context.view_layer.objects.active
    bpy.ops.object.parent_clear(type='CLEAR_KEEP_TRANSFORM')
    bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
    ob.name = name
    return ob


def pca(ob):
    """the prop's axes, longest first, and its centre (bounding box in that frame)"""
    P = np.array([v.co[:] for v in ob.data.vertices])
    c = P.mean(axis=0)
    _, _, vt = np.linalg.svd(P - c, full_matrices=False)
    ax = [Vector(v) for v in vt]
    Q = (P - c) @ vt.T
    lo_, hi_ = Q.min(axis=0), Q.max(axis=0)
    c = Vector(c) + sum((ax[k] * float((lo_[k] + hi_[k]) / 2) for k in range(3)), Vector())
    return ax, c, hi_ - lo_, (P - np.array(c[:])) @ vt.T


def place(ob, src_axes, src_c, dst_axes, dst_c, scale):
    R = Matrix.Identity(4)
    S = Matrix.Identity(4)
    for k in range(3):
        for r in range(3):
            R[r][k] = dst_axes[k][r]
            S[k][r] = src_axes[k][r]      # rows: source axes (world -> prop frame)
    ob.data.transform(Matrix.Translation(dst_c) @ R @ Matrix.Diagonal((scale, scale, scale, 1)) @ S @ Matrix.Translation(-src_c))
    ob.data.update()


prop_hi = {}
if 'scutum' in PROPS:
    ob = load_prop(PROPS['scutum'], 'Scutum_high')
    ax, c, ext, Q = pca(ob)
    # axes: height, width, thickness; the boss side (more bulk out of the plane near the centre) is the front
    centre = (np.abs(Q[:, 0]) < ext[0] * 0.12) & (np.abs(Q[:, 1]) < ext[1] * 0.12)
    front = ax[2] if Q[centre, 2].max() > -Q[centre, 2].min() else -ax[2]
    up_ = ax[0]
    right_ = front.cross(up_)            # right-handed: (right, front, up)
    s = 'a'
    sc = (elbow[s] + wrist[s]) / 2 + FWD * 0.075
    place(ob, (right_, front, up_), c, (RIGHT, FWD, UP), sc, 0.56 / ext[0])
    prop_hi['scutum'] = (ob, H('arm_a_fore'), None)
    log(f'scutum: {len(ob.data.polygons)} faces, {ext[0]:.3f} tall -> 0.56, centre {tuple(round(x, 3) for x in sc)}')
if 'gladius' in PROPS:
    ob = load_prop(PROPS['gladius'], 'Gladius_high')
    ax, c, ext, Q = pca(ob)
    # the long axis; the point is the narrower end (the hilt end carries the guard and the pommel)
    q0 = Q[:, 0]
    wide = lambda m: (Q[m, 1].max() - Q[m, 1].min()) if m.any() else 0
    tip = ax[0] if wide(q0 > ext[0] * 0.35) < wide(q0 < -ext[0] * 0.35) else -ax[0]
    flat = ax[1]                          # the blade's width (the guard's length)
    s = 'b'
    g0 = wrist[s] + Vector((0.045, 0, 0))
    bd = (FWD * 0.9 + UP * 0.45).normalized()
    L = 0.42
    k = L / ext[0]
    # the grip centre: 13% of the length from the pommel end
    gc_ = c - tip * (ext[0] / 2) + tip * (ext[0] * 0.13)
    place(ob, (tip, flat, tip.cross(flat)), gc_, (bd, RIGHT, bd.cross(RIGHT)), g0, k)
    prop_hi['gladius'] = (ob, H('arm_b_hand'), 'arm_b')
    log(f'gladius: {len(ob.data.polygons)} faces, {ext[0]:.3f} long -> {L}, grip at {tuple(round(x, 3) for x in g0)}')
prop_lo = {}
for name, (ob, bone, eff_) in prop_hi.items():
    lo = ob.copy()
    lo.data = ob.data.copy()
    lo.name = name + '_low'
    bpy.context.scene.collection.objects.link(lo)
    bm = bmesh.new()
    bm.from_mesh(lo.data)
    bmesh.ops.remove_doubles(bm, verts=bm.verts, dist=1e-5)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(lo.data)
    bm.free()
    d = lo.modifiers.new('Dec', 'DECIMATE')
    d.ratio = min(1.0, PROP_TRIS[name] / max(1, len(lo.data.polygons)))
    d.use_collapse_triangulate = True
    with bpy.context.temp_override(object=lo, active_object=lo):
        bpy.ops.object.modifier_apply(modifier='Dec')
    lo.data.materials.clear()
    lo.data.shade_smooth()
    try:
        lo.data.set_sharp_from_angle(angle=math.radians(40))
    except AttributeError:
        pass
    lo.vertex_groups.new(name=bone).add(list(range(len(lo.data.vertices))), 1.0, 'REPLACE')
    prop_lo[name] = lo
    log(f'{name}: decimated to {len(lo.data.polygons)} triangles, rigid on {bone}')

# ---- the low copy
lo_ob = hi_ob.copy()
lo_ob.data = hi_ob.data.copy()
lo_ob.name = 'Atlas_low'
bpy.context.scene.collection.objects.link(lo_ob)
bm = bmesh.new()
bm.from_mesh(lo_ob.data)
bmesh.ops.remove_doubles(bm, verts=bm.verts, dist=1e-5)
bmesh.ops.triangulate(bm, faces=bm.faces)
bm.to_mesh(lo_ob.data)
bm.free()
n0 = len(lo_ob.data.polygons)
dec = lo_ob.modifiers.new('Dec', 'DECIMATE')
dec.ratio = min(1.0, BODY_TRIS / n0)
dec.use_collapse_triangulate = True
with bpy.context.temp_override(object=lo_ob, active_object=lo_ob):
    bpy.ops.object.modifier_apply(modifier='Dec')
lo_ob.data.materials.clear()
lo_ob.data.shade_smooth()
try:
    lo_ob.data.set_sharp_from_angle(angle=math.radians(50))
except AttributeError:
    pass
log(f'decimated {n0} -> {len(lo_ob.data.polygons)} triangles')

# ---- the cut: hands and feet become separate pieces (rigid on their effector joints)
bm = bmesh.new()
bm.from_mesh(lo_ob.data)
region = {}
for f in bm.faces:
    p = f.calc_center_median()
    if abs(p.x) > abs(wrist['a' if p.x < 0 else 'b'].x) + 0.005 and p.z > ground + 0.3:
        region[f.index] = 'hand_' + ('a' if p.x < 0 else 'b')
    elif p.z < ankle_z:
        region[f.index] = 'foot_' + ('a' if p.x < 0 else 'b')
    else:
        region[f.index] = 'body'
loose = [v for v in bm.verts if not v.link_faces]
bmesh.ops.delete(bm, geom=loose, context='VERTS')
bm.faces.ensure_lookup_table()
region = {f.index: region[f.index] for f in bm.faces}
edges = [e for e in bm.edges if len({region[f.index] for f in e.link_faces}) > 1]
bmesh.ops.split_edges(bm, edges=edges)
bm.to_mesh(lo_ob.data)
bm.free()
me = lo_ob.data
fregion = {}
for f in me.polygons:
    p = f.center
    if abs(p.x) > abs(wrist['a' if p.x < 0 else 'b'].x) + 0.005 and p.z > ground + 0.3:
        fregion[f.index] = 'hand_' + ('a' if p.x < 0 else 'b')
    elif p.z < ankle_z:
        fregion[f.index] = 'foot_' + ('a' if p.x < 0 else 'b')
    else:
        fregion[f.index] = 'body'
vregion = {}
for f in me.polygons:
    for vi in f.vertices:
        vregion[vi] = fregion[f.index]

# ---- weights
neck_w = chain([J('upper_body'), J('head')], [head_lo.z + 0.01], 0.025)
torso_w = chain([J('lower_body'), J('upper_body')], [upper_z], 0.05)
arm_w = {s: chain([J(f'arm_{s}_shoulder'), J(f'arm_{s}_upper'), J(f'arm_{s}_fore')], [abs(shoulder[s].x) - 0.005, abs(elbow[s].x)], 0.03) for s in 'ab'}
leg_w = {s: chain([J('lower_body'), J(f'leg_{s}_thigh'), J(f'leg_{s}_shin')], [-(hip[s].z - 0.02), -knee[s].z], 0.035) for s in 'ab'}
groups = {}


def add(vi, b, w):
    if w <= 1e-3:
        return
    if b not in groups:
        groups[b] = lo_ob.vertex_groups.new(name=b)
    groups[b].add([vi], w, 'REPLACE')


arm_start = min(abs(shoulder['a'].x), abs(shoulder['b'].x)) - 0.06
for v in me.vertices:
    p = v.co
    r = vregion.get(v.index, 'body')
    s = 'a' if p.x < 0 else 'b'
    if r == 'hand_a' or r == 'hand_b':
        add(v.index, H(f'arm_{r[-1]}_hand'), 1.0)
        continue
    if r.startswith('foot'):
        add(v.index, H(f'leg_{r[-1]}_ankle'), 1.0)
        continue
    w = {}
    if p.z > head_lo.z - 0.04:
        w = neck_w(p.z)
    else:
        t_arm = smoothstep((abs(p.x) - arm_start) / 0.08) * smoothstep((p.z - (shoulder[s].z - 0.16)) / 0.06)
        t_leg = smoothstep(((hip[s].z + 0.02) - p.z) / 0.10) * smoothstep((abs(p.x) - 0.015) / 0.03)
        base = torso_w(p.z)
        if t_arm > 0:
            aw = arm_w[s](abs(p.x))
            w = {b: x * (1 - t_arm) for b, x in base.items()}
            for b, x in aw.items():
                w[b] = w.get(b, 0) + x * t_arm
        elif t_leg > 0:
            lw = leg_w[s](-p.z)
            w = {b: x * (1 - t_leg) for b, x in base.items()}
            for b, x in lw.items():
                w[b] = w.get(b, 0) + x * t_leg
        else:
            w = base
    tot = sum(w.values())
    for b, x in w.items():
        add(v.index, b, x / tot)
lo_ob.parent = arm
lo_ob.matrix_parent_inverse = arm.matrix_world.inverted()
mod = lo_ob.modifiers.new('Armature', 'ARMATURE')
mod.object = arm

# the props join the body: their low copies (rigid groups) into the low mesh, their high models into the bake source
for name, lo in prop_lo.items():
    for o in bpy.context.view_layer.objects:
        o.select_set(o in (lo, lo_ob))
    bpy.context.view_layer.objects.active = lo_ob
    bpy.ops.object.join()
    ob = prop_hi[name][0]
    for o in bpy.context.view_layer.objects:
        o.select_set(o in (ob, hi_ob))
    bpy.context.view_layer.objects.active = hi_ob
    bpy.ops.object.join()
if prop_lo:
    log(f'with props: {len(lo_ob.data.polygons)} triangles')

# ---- UVs (the face gets 2.5x the texels) and the bakes
bpy.ops.object.select_all(action='DESELECT')
lo_ob.select_set(True)
bpy.context.view_layer.objects.active = lo_ob
bpy.ops.object.mode_set(mode='EDIT')
bpy.ops.mesh.select_all(action='SELECT')
bpy.ops.uv.smart_project(angle_limit=math.radians(60), island_margin=0.004, area_weight=0.0, scale_to_bounds=False)
bme = bmesh.from_edit_mesh(lo_ob.data)
uvl = bme.loops.layers.uv.active
for f in bme.faces:
    if f.calc_center_median().z > head_lo.z - 0.02 and abs(f.calc_center_median().x) < 0.15:
        for lp in f.loops:
            lp[uvl].uv *= 2.5
bmesh.update_edit_mesh(lo_ob.data)
bpy.ops.uv.select_all(action='SELECT')
bpy.ops.uv.pack_islands(rotate=True, scale=True, margin=0.004)
bpy.ops.object.mode_set(mode='OBJECT')

scene = bpy.context.scene
scene.render.engine = 'CYCLES'
scene.cycles.device = 'GPU' if bpy.context.preferences.addons.get('cycles') else 'CPU'
scene.cycles.samples = 32
world = bpy.data.worlds.new('BakeWorld')
world.color = (1, 1, 1)
scene.world = world


def hi_material_emit(channel):
    """the AI model's material made to emit its base colour (or its metallic value) for an EMIT bake"""
    for m in hi_ob.data.materials:
        if m is None or not m.use_nodes:
            continue
        nt = m.node_tree
        bsdf = next((n for n in nt.nodes if n.type == 'BSDF_PRINCIPLED'), None)
        out = next((n for n in nt.nodes if n.type == 'OUTPUT_MATERIAL'), None)
        if bsdf is None or out is None:
            continue
        em = nt.nodes.get('BakeEmit') or nt.nodes.new('ShaderNodeEmission')
        em.name = 'BakeEmit'
        src = bsdf.inputs['Base Color' if channel == 'color' else 'Metallic']
        for l in list(nt.links):
            if l.to_node == em:
                nt.links.remove(l)
        cimg = next((n.image for n in nt.nodes if n.type == 'TEX_IMAGE' and n.image and n.image.filepath), None)
        side = os.path.splitext(bpy.path.abspath(cimg.filepath) if cimg else AI)[0] + '_metallic.jpg'
        if channel == 'metal' and not src.is_linked and os.path.exists(side):
            # Hunyuan3D's OBJ: the metallic map sits next to it (<name>_metallic.jpg), not in the .mtl
            ti = nt.nodes.new('ShaderNodeTexImage')
            ti.image = bpy.data.images.load(side)
            ti.image.colorspace_settings.name = 'Non-Color'
            nt.links.new(ti.outputs['Color'], em.inputs['Color'])
        elif src.is_linked:
            nt.links.new(src.links[0].from_socket, em.inputs['Color'])
        else:
            v = src.default_value
            em.inputs['Color'].default_value = tuple(v) if channel == 'color' else (v, v, v, 1)
        nt.links.new(em.outputs[0], out.inputs['Surface'])


bake_mat = bpy.data.materials.new('BakeTarget')
bake_mat.use_nodes = True
lo_ob.data.materials.append(bake_mat)
tex = bake_mat.node_tree.nodes.new('ShaderNodeTexImage')
bake_mat.node_tree.nodes.active = tex
# the armature must not move the low copy during the bake
mod.show_render = mod.show_viewport = False


def bake(kind, img, selected=True):
    tex.image = img
    for o in bpy.context.view_layer.objects:
        o.select_set(False)
    if selected:
        hi_ob.select_set(True)
    lo_ob.select_set(True)
    bpy.context.view_layer.objects.active = lo_ob
    scene.render.bake.use_selected_to_active = selected
    scene.render.bake.cage_extrusion = 0.015
    scene.render.bake.max_ray_distance = 0.05
    scene.render.bake.margin = 6
    bpy.ops.object.bake(type=kind, use_clear=True)
    return np.array(img.pixels[:], dtype=np.float32).reshape(BAKE, BAKE, 4)


img_c = bpy.data.images.new('bake_colour', BAKE, BAKE)
img_m = bpy.data.images.new('bake_metal', BAKE, BAKE)
img_a = bpy.data.images.new('bake_ao', BAKE, BAKE)
hi_material_emit('color')
col = bake('EMIT', img_c)
hi_material_emit('metal')
met = bake('EMIT', img_m)
ao = bake('AO', img_a)
log('baked colour, metallic, ambient occlusion')
m_ = np.clip((met[:, :, :1] - 0.25) / 0.5, 0, 1)            # metal areas
occ = ao[:, :, :1]
rgb = col[:, :, :3] * (0.50 + 0.50 * occ) * 1.08
# PBR metal albedo is pale and flat (its look comes from reflections): metal goes darker and cooler with deeper
# occlusion, so it reads as iron / bronze; the game's specular mask (from the metallic map) adds the shine
iron = rgb * np.array([0.80, 0.82, 0.88]) * (0.65 + 0.35 * occ) + 0.07     # (lifted: chainmail read as flat black)
rgb = rgb * (1 - m_) + iron * m_
lum = rgb.mean(axis=2, keepdims=True)
rgb = np.clip(lum + (rgb - lum) * 1.15, 0, 1)                # a little more colour


def down(px, w, h):
    """area-average BAKE x BAKE -> w x h"""
    return px.reshape(h, BAKE // h, w, BAKE // w, -1).mean(axis=(1, 3))


W, Hh = ATLAS
atlas = np.ones((Hh, W, 4), dtype=np.float32)
atlas[:, :, :3] = down(rgb, W, Hh)
mr = np.ones((Hh, W, 4), dtype=np.float32)
mr[:, :, 0] = 0
mr[:, :, 1] = 0.5
mr[:, :, 2] = down(met[:, :, :1], W, Hh)[:, :, 0]
base = os.path.splitext(DST)[0]
img = bpy.data.images.new('Atlas_Body', W, Hh, alpha=False)
img.pixels[:] = atlas.ravel()
img.filepath_raw = base + '.tex_atlas.png'
img.file_format = 'PNG'
img.save()
img.pack()
mri = bpy.data.images.new('Atlas_Body_mr', W, Hh, alpha=False)
mri.colorspace_settings.name = 'Non-Color'
mri.pixels[:] = mr.ravel()
mri.pack()

# the final material: atlas -> base colour, metallic from the mr image (the importer's specular mask)
mat = bpy.data.materials.new('Atlas_Body')
mat.use_nodes = True
nt = mat.node_tree
nt.nodes.clear()
o_ = nt.nodes.new('ShaderNodeOutputMaterial')
b_ = nt.nodes.new('ShaderNodeBsdfPrincipled')
nt.links.new(b_.outputs[0], o_.inputs[0])
t_ = nt.nodes.new('ShaderNodeTexImage')
t_.image = img
nt.links.new(t_.outputs['Color'], b_.inputs['Base Color'])
t2 = nt.nodes.new('ShaderNodeTexImage')
t2.image = mri
sep = nt.nodes.new('ShaderNodeSeparateColor')
nt.links.new(t2.outputs['Color'], sep.inputs['Color'])
nt.links.new(sep.outputs['Green'], b_.inputs['Roughness'])
nt.links.new(sep.outputs['Blue'], b_.inputs['Metallic'])
mat.use_backface_culling = True
lo_ob.data.materials.clear()
lo_ob.data.materials.append(mat)
mod.show_render = mod.show_viewport = True
bpy.data.objects.remove(hi_ob, do_unlink=True)
log(f'{len(lo_ob.data.polygons)} triangles, atlas {W}x{Hh}')

# ---- preview (drawn as in the game), then hands / feet to the game data's bind positions
import render  # noqa: E402
state = scene.render.engine
render.render(base + '.preview.png')
scene.render.engine = state
for o in list(scene.objects):
    if o.name.startswith('PreviewCam'):
        bpy.data.objects.remove(o, do_unlink=True)
eff = {}
for s in 'ab':
    eff[H(f'arm_{s}_hand')] = f'arm_{s}'
    eff[H(f'leg_{s}_ankle')] = f'leg_{s}'
moved = 0
for v in lo_ob.data.vertices:
    gs = [(lo_ob.vertex_groups[g.group].name, g.weight) for g in v.groups if g.weight > 1e-3]
    if len(gs) == 1 and gs[0][0] in eff:
        m = effector[eff[gs[0][0]]]
        v.co = lo_ob.matrix_world.inverted() @ (m @ (lo_ob.matrix_world @ v.co))
        moved += 1
lo_ob.data.update()
log(f'moved {moved} hand / foot vertices to the effector bind positions')
json.dump({b: [list(r) for r in effector[k].inverted()] for b, k in eff.items()}, open(base + '.pose.json', 'w'), indent=1)
open(base + '.args', 'w', newline='\n').write('--replace\n')
common.save(DST)
