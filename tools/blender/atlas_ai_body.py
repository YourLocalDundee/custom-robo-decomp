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
ATLAS = tuple(int(x) for x in (opts.get('atlas') or '512x256').split('x'))   # --atlas=1024x512 for a big body
BAKE = 1024
# --regions: the texture split by body section (owner's request, Nov 9, 2026). Each section is unwrapped and packed on
# its own into a square cell of the 512x256 atlas (square in pixels, so texels stay square: the old path packs into the
# unit square, which the 512x256 image stretches 2:1), baked at 4x (2048x1024), shrunk with a Lanczos-3 filter and a
# light unsharp mask; 2 px gaps that the bake dilates into, so no CMP (DXT1) 4x4 block mixes a piece with black.
# Seams follow the armour's hard creases; each region tries several crease angles and keeps the unwrap with the best
# cell use x evenness (smart_project shredded the decimated AI mesh into hundreds of near-single-triangle pieces).
# Mecha samurai (Nov 9): 3,600 triangles fit (230,015 bytes; the fewer UV seams free bytes vs v1's 3,300).
# Cells: (x, y, size) in pixels.
REGIONS = bool(opts.get('regions') or opts.get('mirror'))   # --mirror implies --regions
CELLS = {'torso_arms': (0, 0, 256), 'head': (256, 128, 128), 'leg_a': (384, 128, 128), 'leg_b': (384, 0, 128),
         'hands_feet': (256, 0, 128)}
SS = int(opts.get('ss') or 4)   # bake supersampling with --regions (--ss=2 for a 1024x512 atlas: a 2048x1024 bake)
GAP_PX = float(opts.get('gap') or 2)        # px between pieces in the final atlas (the bake dilates into it)
UV_MODE = opts.get('uv') or 'seams'         # --uv=seams (hard-crease seams, angle-based unwrap) or --uv=smart
# --uv_regions=head:smart (Nov 10): a UV method per region. Crease seams on the head left the horns joined to the bowl
# in one folded island that baked the bowl's red onto the horns; smart_project keeps the head clean
UV_REGION = {k: v for k, _, v in (x.partition(':') for x in (opts.get('uv_regions') or '').split(',') if x)}
HEAD = opts.get('head')                     # --head=<close-up head model>: replaces the body model's head
HEAD_SHARE = float(opts.get('head_share') or 0.22)   # --head: the head's share of the triangles
MIRROR = bool(opts.get('mirror'))           # --mirror: symmetric low mesh, the right side shares the left's texels
DETAIL = float(opts.get('detail') or 0)     # --detail=<k>: high-poly relief baked into the colour as light / shade
# --sections (Nov 9, 2026, docs/full_custom_robo.md): one texture PER SECTION instead of one atlas: torso + arms
# 1024x1024, legs 512x1024, head 512x512, hands / feet 512x512 (a GX texture is at most 1024 on a side, so one atlas
# caps at 1024x1024; four textures give 2.25x the pixels). Needs the per-mesh material patch (the game otherwise draws
# the whole body with the block B joint's material) and a body slot big enough (code/samurai.txt). Implies --mirror.
SECTIONS = bool(opts.get('sections'))
SECTION_CELLS = {'torso_arms': (0, 0, 1024, 1024), 'leg': (1024, 0, 512, 1024), 'head': (1536, 512, 512, 512),
                 'hands_feet': (1536, 0, 512, 512)}
if SECTIONS:
    ATLAS = (2048, 1024)
    MIRROR = True
    REGIONS = True
# --plates: armour plates that hang free (skirt plates, shoulder plates) follow the hips / the shoulder, not the thigh /
# upper arm: position-only weights stretched them like skin and they flared when the legs and arms moved (first big
# samurai, Customize screenshots Nov 9)
PLATES = bool(opts.get('plates'))
# --rigid (Nov 11, 2026): every face rigid on ONE joint, as the stock bodies are built (Ray 01: 42 of 52 PObjs rigid,
# docs/robo_look.md "Seams"). The smooth weights above only pick the joint (the face's largest summed weight); the mesh
# is split where neighbouring faces chose different joints. The first in-game look (Customize, Nov 11) showed why: a
# T-pose skin dragged between bones by the game's poses (arms ~70 degrees down, legs spread) sheared the shoulder
# plates into flat sheets, folded the upper arms and smeared the skirt over the thighs. Rigid plates move like armour
RIGID = bool(opts.get('rigid'))
# --rigid=plates: only the armour plates rigid (shoulder plates on the upper arm, skirt plates on the hips), cut free
# from the skin; the rest stays skinned. --rigid=1: everything rigid but the joint bands
RIGID_PLATES_ONLY = opts.get('rigid') == 'plates'
RIGID_BAND = float(opts.get('rigid_band') or 0.7)   # --rigid_band: below this strongest weight a face stays skinned
# --look=game: lighter bake for the game's lighting (the game lights the body itself: ambient 70% + diffuse): softer
# ambient occlusion, metal not darkened as much, levels so the brightest 2% sit near white
LOOK = opts.get('look') or ''
# --regions with --mirror: section weights (texel density relative to each other); the head is what you look at
WEIGHTS = {'head': 2.0, 'torso_arms': 1.15, 'leg': 0.85, 'hands_feet': 0.8}
# crease angles tried per region (--seams=60,80,100,120); each region keeps the unwrap that fills its cell best
SEAM_DEGS = [float(x) for x in (opts.get('seams') or '60,80,100,120').split(',')]
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
# the same turns the body needed, for a close-up head made by the same picture-to-3D pipeline (same conventions)
_turns = []
if up_ax != 2:
    _turns.append(Matrix.Rotation(math.radians(90), 4, 'X' if up_ax == 1 else 'Y'))

def drop_specks(bm_, frac):
    """delete loose pieces smaller than frac of the mesh's surface (rivets, studs, crumbs modelled as separate shells):
    a decimation cannot shrink a loose piece below a few triangles, so many of them eat the budget. Only for the low
    copy: the bake still reads them from the full model. Returns how many pieces went."""
    bm_.faces.ensure_lookup_table()
    seen, pieces = set(), []
    for f0 in bm_.faces:
        if f0.index in seen:
            continue
        stack, comp = [f0], []
        seen.add(f0.index)
        while stack:
            f = stack.pop()
            comp.append(f)
            for e in f.edges:
                for g in e.link_faces:
                    if g.index not in seen:
                        seen.add(g.index)
                        stack.append(g)
        pieces.append(comp)
    total = sum(f.calc_area() for f in bm_.faces)
    gone = [c for c in pieces if sum(f.calc_area() for f in c) < frac * total]
    bmesh.ops.delete(bm_, geom=[f for c in gone for f in c], context='FACES')
    return len(gone)


if HEAD:
    # --head=<model>: a close-up of the head turned into 3D on its own (far more texture detail for the face): it
    # replaces the body model's head. Fitted to the body's head box (uniform scale, top and centre aligned); the body's
    # own head faces are deleted. The bake then reads the close-up's 2K texture for the whole head.
    before = set(bpy.data.objects)
    if HEAD.lower().endswith('.obj'):
        bpy.ops.wm.obj_import(filepath=HEAD)
    else:
        bpy.ops.import_scene.gltf(filepath=HEAD)
    hnew = [o for o in bpy.data.objects if o not in before and o.type == 'MESH']
    for o in bpy.data.objects:
        o.select_set(o in hnew)
    bpy.context.view_layer.objects.active = hnew[0]
    if len(hnew) > 1:
        bpy.ops.object.join()
    head_ob = bpy.context.view_layer.objects.active
    bpy.ops.object.parent_clear(type='CLEAR_KEEP_TRANSFORM')
    bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
    hm = head_ob.data
    for m_ in _turns:
        hm.transform(m_)
    hl, hh = box(head_ob)
    if (hh - hl)[1] > (hh - hl)[0] * 1.3:          # same arm-axis rule as the body (a head is roughly round: mild)
        hm.transform(Matrix.Rotation(math.radians(90), 4, 'Z'))
    if toe_y < c.y:
        hm.transform(Matrix.Rotation(math.radians(180), 4, 'Z'))
    hl, hh = box(head_ob)
    cut_z = head_lo.z - 0.02
    region = [v.co for v in me.vertices if v.co.z > cut_z and abs(v.co.x) < 0.2]
    rl = Vector((min(p.x for p in region), min(p.y for p in region), min(p.z for p in region)))
    rh = Vector((max(p.x for p in region), max(p.y for p in region), max(p.z for p in region)))
    # scale from the upper 60% of each (helmet, flaps, crest): a close-up picture often keeps bits of the shoulders
    # at its bottom corners, which would widen the head model's box
    def upper_width(pts, z0, z1):
        u_ = [p.x for p in pts if p.z > z0 + 0.4 * (z1 - z0)]
        return max(u_) - min(u_)
    s_ = upper_width(region, rl.z, rh.z) / upper_width([v.co for v in hm.vertices], hl.z, hh.z)
    hc, rc = (hl + hh) / 2, (rl + rh) / 2
    hm.transform(Matrix.Translation(Vector((rc.x, rc.y, rh.z))) @ Matrix.Diagonal((s_, s_, s_, 1))
                 @ Matrix.Translation(-Vector((hc.x, hc.y, hh.z))))
    hm.update()
    # trim the close-up below the neck (shoulder bits, the top of the chest): the body's own shoulders stay
    _hl2, _hh2 = box(head_ob)
    log(f'head fit: close-up z {_hl2.z:.3f}..{_hh2.z:.3f} (height {_hh2.z - _hl2.z:.3f}), body head region z '
        f'{rl.z:.3f}..{rh.z:.3f}, cut {cut_z:.3f}, head bone box {head_lo.z:.3f}..{head_hi.z:.3f}')
    bm_ = bmesh.new()
    bm_.from_mesh(hm)
    _trim = [f for f in bm_.faces if f.calc_center_median().z < cut_z - 0.03]
    log(f'head fit: {len(_trim)} of {len(bm_.faces)} close-up faces below the neck trimmed')
    bmesh.ops.delete(bm_, geom=_trim, context='FACES')
    bm_.to_mesh(hm)
    bm_.free()
    # the close-up's neck reaches below the cut: keep it (it hides the join), drop the body's head above the cut
    bm_ = bmesh.new()
    bm_.from_mesh(me)
    bmesh.ops.delete(bm_, geom=[f for f in bm_.faces if f.calc_center_median().z > cut_z
                                and abs(f.calc_center_median().x) < 0.2], context='FACES')
    bm_.to_mesh(me)
    bm_.free()
    # the low mesh starts from its own copy where the close-up head is thinned to the body's faces per area: a dense
    # close-up would otherwise soak up the triangle budget (seen: 1,029 head triangles per side, 25 for the hands);
    # the bake source (hi_ob) keeps the full-detail head
    def surf(m):
        return sum(p.area for p in m.polygons)
    head_area, body_area = surf(hm), surf(me)
    head_target = len(me.polygons) * head_area / max(body_area, 1e-9)
    lo_src = hi_ob.copy()
    lo_src.data = me.copy()
    bpy.context.scene.collection.objects.link(lo_src)
    head_lo_ob = head_ob.copy()
    head_lo_ob.data = hm.copy()
    bpy.context.scene.collection.objects.link(head_lo_ob)
    # weld first, as the body's low copy is: a glTF splits vertices along its texture seams, and a decimation of the
    # unwelded mesh cannot collapse across them (seen: 17,764 triangles left for a 3,600 budget)
    bm_ = bmesh.new()
    bm_.from_mesh(head_lo_ob.data)
    bmesh.ops.remove_doubles(bm_, verts=bm_.verts, dist=1e-5)
    bmesh.ops.triangulate(bm_, faces=bm_.faces)
    log(f'head: dropped {drop_specks(bm_, 0.003)} loose specks from the low copy')
    bm_.to_mesh(head_lo_ob.data)
    bm_.free()
    # a voxel remesh turns the close-up's many separate shells into one closed surface (a decimation stalled at
    # ~2,000 triangles on the shells for a 792 budget); voxel 0.004 keeps the crest (thinnest part) solid
    # the picture-to-3D close-up is open shells (helmet bowl, faceplate, visor: not watertight), and a voxel remesh of
    # open shells loses them: seen the bowl gone and the face in holes, the body shown with an empty helmet. A thin
    # solidify first makes every shell a closed solid that the remesh keeps (--head_solid, 0 = off)
    _solid = float(opts.get('head_solid') or 0.005)
    if _solid > 0:
        so_ = head_lo_ob.modifiers.new('HeadSolid', 'SOLIDIFY')
        so_.thickness = _solid
        so_.offset = -1.0
        with bpy.context.temp_override(object=head_lo_ob, active_object=head_lo_ob):
            bpy.ops.object.modifier_apply(modifier='HeadSolid')
    rm = head_lo_ob.modifiers.new('HeadRemesh', 'REMESH')
    rm.mode = 'VOXEL'
    rm.voxel_size = float(opts.get('head_voxel') or 0.004)
    rm.adaptivity = 0.0
    with bpy.context.temp_override(object=head_lo_ob, active_object=head_lo_ob):
        bpy.ops.object.modifier_apply(modifier='HeadRemesh')
    log(f'head: low copy remeshed to {len(head_lo_ob.data.polygons)} faces (one closed surface)')
    ih = head_lo_ob.data.attributes.new('is_head', 'INT', 'FACE')     # the body's faces get 0 when they are joined
    ih.data.foreach_set('value', [1] * len(head_lo_ob.data.polygons))
    d_ = head_lo_ob.modifiers.new('HeadDec', 'DECIMATE')
    d_.ratio = min(1.0, head_target / max(1, len(hm.polygons)))
    with bpy.context.temp_override(object=head_lo_ob, active_object=head_lo_ob):
        bpy.ops.object.modifier_apply(modifier='HeadDec')
    for o in bpy.context.view_layer.objects:
        o.select_set(o in (head_lo_ob, lo_src))
    bpy.context.view_layer.objects.active = lo_src
    bpy.ops.object.join()
    lo_src.name = 'AI_low_source'
    for o in bpy.context.view_layer.objects:
        o.select_set(o in (head_ob, hi_ob))
    bpy.context.view_layer.objects.active = hi_ob
    bpy.ops.object.join()
    me = hi_ob.data
    log(f'head: close-up model fitted (scale {s_:.4f}) into the box {tuple(round(x, 3) for x in rl)} .. '
        f'{tuple(round(x, 3) for x in rh)}, body head removed above z {cut_z:.3f}; {len(me.polygons)} faces now')

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
_src = bpy.data.objects.get('AI_low_source')          # --head: the copy with the close-up head thinned
lo_ob.data = (_src.data if _src else hi_ob.data).copy()
if _src:
    bpy.data.objects.remove(_src, do_unlink=True)
lo_ob.name = 'Atlas_low'
bpy.context.scene.collection.objects.link(lo_ob)
bm = bmesh.new()
bm.from_mesh(lo_ob.data)
bmesh.ops.remove_doubles(bm, verts=bm.verts, dist=1e-5)
bmesh.ops.triangulate(bm, faces=bm.faces)
if REGIONS:
    log(f'dropped {drop_specks(bm, 0.0005)} loose specks from the low copy')
bm.to_mesh(lo_ob.data)
bm.free()
n0 = len(lo_ob.data.polygons)


# --feature=<k> (Nov 10, docs/robo_look.md "Feature-protected decimation"): plain quadric collapse rounds off creases
# and plate edges at a stock budget (the stock robos are hard-surface: sharp plate edges, clean silhouettes). Each pass
# weights the vertices by the sharpest dihedral angle around them (and open borders: plate rims) and hands the
# weights to the Decimate modifier's vertex group (Factor k), so collapses happen in the flat areas first
FEATURE = float(opts.get('feature') or 0)


def feature_weights(ob):
    me_ = ob.data
    nv, ne, nl, npo = len(me_.vertices), len(me_.edges), len(me_.loops), len(me_.polygons)
    if not ne or not npo:
        return
    me_.update()
    pn = np.zeros(npo * 3)
    me_.polygons.foreach_get('normal', pn)
    pn = pn.reshape(-1, 3)
    le = np.zeros(nl, np.int64)
    me_.loops.foreach_get('edge_index', le)
    ls = np.zeros(npo, np.int64)
    lt = np.zeros(npo, np.int64)
    me_.polygons.foreach_get('loop_start', ls)
    me_.polygons.foreach_get('loop_total', lt)
    lp = np.repeat(np.arange(npo), lt)
    order = np.argsort(le, kind='stable')
    se, sp = le[order], lp[order]
    cnt = np.bincount(se, minlength=ne)
    start = np.concatenate([[0], np.cumsum(cnt)[:-1]])
    # open borders (plate rims) 0.5, creases up to 0.8: full weights on an AI shell (rims everywhere) stalled the
    # collapse (seen 140,236 faces left for a 2,100 budget)
    sharp = np.full(ne, 0.5)
    two = cnt >= 2
    a = pn[sp[start[two]]]
    b = pn[sp[start[two] + 1]]
    ang = np.degrees(np.arccos(np.clip((a * b).sum(1), -1, 1)))
    sharp[two] = 0.8 * np.clip((ang - 15) / 45, 0, 1)    # 15 deg and below: flat; 60 and up: a crease
    ev = np.zeros(ne * 2, np.int64)
    me_.edges.foreach_get('vertices', ev)
    ev = ev.reshape(-1, 2)
    w = np.zeros(nv)
    np.maximum.at(w, ev[:, 0], sharp)
    np.maximum.at(w, ev[:, 1], sharp)
    vg = ob.vertex_groups.get('Feature') or ob.vertex_groups.new(name='Feature')
    q = np.round(w * 10).astype(np.int64)                # added in 11 weight levels (one call per level)
    for lev in range(11):
        idx = np.nonzero(q == lev)[0]
        if len(idx):
            vg.add(idx.tolist(), lev / 10, 'REPLACE')


def weld_centre(ob):
    """the centre line back on x = 0 and welded (a symmetric decimation may leave it a hair off)"""
    bm_ = bmesh.new()
    bm_.from_mesh(ob.data)
    for v in bm_.verts:
        if abs(v.co.x) < 1e-4:
            v.co.x = 0.0
    bmesh.ops.remove_doubles(bm_, verts=bm_.verts, dist=1e-5)
    bm_.to_mesh(ob.data)
    bm_.free()
    ob.data.update()


def decimate_to(ob, n, sym=False):
    # one collapse pass can stop well short of the ratio on a busy, open AI mesh (seen: 2,080 left for 792): repeat
    weighted = FEATURE > 0
    for _ in range(10):
        have = len(ob.data.polygons)
        if have <= n * 1.03:
            break
        d_ = ob.modifiers.new('Dec', 'DECIMATE')
        d_.ratio = min(1.0, n / max(1, have))
        d_.use_collapse_triangulate = True
        if sym:
            d_.use_symmetry = True
            d_.symmetry_axis = 'X'
        if weighted:
            feature_weights(ob)
            d_.vertex_group = 'Feature'
            d_.vertex_group_factor = FEATURE
        with bpy.context.temp_override(object=ob, active_object=ob):
            bpy.ops.object.modifier_apply(modifier='Dec')
        log(f'decimate {ob.name}: {have} -> {len(ob.data.polygons)} (target {n}{", feature-weighted" if weighted else ""})')
        if len(ob.data.polygons) > have * 0.97:
            if weighted:
                weighted = False             # the weights hold it up: finish unweighted
                continue
            break
    if ob.vertex_groups.get('Feature'):
        ob.vertex_groups.remove(ob.vertex_groups['Feature'])     # not a bone: never reaches the export


# --region_tris=head:3000,torso:3000,arms:3000,legs:3000 (Nov 10, owner's request: a full budget per body region,
# outside only) and --cull (docs/robo_look.md "Hidden faces"): the AI meshes are shells with insides (skirt and helmet
# insides, the backs of plates, double walls) that the stock robos never spend triangles or texels on. Each region is
# decimated on its own to CULL_PAD x its budget, the faces no ray from outside reaches are deleted, then each region
# goes to its budget: every triangle that is left can be seen
REGION_TRIS = {k: int(v) for k, _, v in (x.partition(':') for x in opts['region_tris'].split(','))} \
    if opts.get('region_tris') else None
CULL = bool(opts.get('cull'))
CULL_PAD = float(opts.get('cull_pad') or 2.0)
if REGION_TRIS:
    BODY_TRIS = sum(REGION_TRIS.values())


def fib_dirs(n):
    i = np.arange(n) + 0.5
    phi = np.arccos(1 - 2 * i / n)
    th = np.pi * (1 + 5 ** 0.5) * i
    return [Vector((math.cos(t) * math.sin(p), math.sin(t) * math.sin(p), math.cos(p))) for t, p in zip(th, phi)]


def cull_hidden(ob, delete=True, ndirs=80):
    """ray-cast visibility: a face is visible when, from some direction it faces, a ray from one of its sample points
    (centre and three points halfway to the corners) leaves the mesh without a hit. Also writes the face attribute
    'imp' (Nov 10, docs/robo_look.md "Texel importance"): 0.6 + 2 x the share of front directions (+Y, the side the
    camera mostly sees) the face is seen from, capped at 1.6. Returns (deleted, kept)."""
    from mathutils.bvhtree import BVHTree
    me_ = ob.data
    vs = [v.co.copy() for v in me_.vertices]
    bvh = BVHTree.FromPolygons(vs, [tuple(p.vertices) for p in me_.polygons], epsilon=0.0)
    dirs = fib_dirs(ndirs)
    front = [d for d in dirs if d.y > 0.25]
    size = max((max(v[i] for v in vs) - min(v[i] for v in vs)) for i in range(3))
    eps = size * 2e-4
    vis, imp = [], []
    for p in me_.polygons:
        n_ = p.normal
        c_ = p.center
        pts = [c_ + n_ * eps] + [c_ + (vs[k] - c_) * 0.5 + n_ * eps for k in p.vertices]
        seen = False
        fs = 0
        for d in dirs:
            if n_.dot(d) <= 0.02:
                continue
            if delete and seen:
                break                       # deleting: one free ray is enough
            if any(bvh.ray_cast(q, d)[0] is None for q in pts):
                seen = True
                if d.y > 0.25:
                    fs += 1
        vis.append(seen)
        imp.append(min(1.6, 0.6 + 2.0 * fs / max(1, len(front))))
    attr = me_.attributes.get('imp') or me_.attributes.new('imp', 'FLOAT', 'FACE')
    attr.data.foreach_set('value', imp)
    gone = 0
    if delete:
        bm_ = bmesh.new()
        bm_.from_mesh(me_)
        bm_.faces.ensure_lookup_table()
        dead = [bm_.faces[i] for i, s in enumerate(vis) if not s]
        gone = len(dead)
        bmesh.ops.delete(bm_, geom=dead, context='FACES')
        bmesh.ops.delete(bm_, geom=[v for v in bm_.verts if not v.link_faces], context='VERTS')
        bm_.to_mesh(me_)
        bm_.free()
        me_.update()
    return gone, len(me_.polygons)


def split_regions(ob, attr):
    """one object per value of the int face attribute (names from reg_names); ob is kept for the first"""
    out = {}
    vals = np.zeros(len(ob.data.polygons), np.int64)
    ob.data.attributes[attr].data.foreach_get('value', vals)
    for v in sorted(set(vals.tolist())):
        part = ob.copy()
        part.data = ob.data.copy()
        bpy.context.scene.collection.objects.link(part)
        bm_ = bmesh.new()
        bm_.from_mesh(part.data)
        lay = bm_.faces.layers.int.get(attr)
        bmesh.ops.delete(bm_, geom=[f for f in bm_.faces if f[lay] != v], context='FACES')
        bm_.to_mesh(part.data)
        bm_.free()
        out[v] = part
    return out


def join_into(target_name, parts):
    for o in bpy.context.view_layer.objects:
        o.select_set(o in parts)
    bpy.context.view_layer.objects.active = parts[0]
    bpy.ops.object.join()
    j = bpy.context.view_layer.objects.active
    j.name = target_name
    return j


def mirror_mesh(ob, target):
    """--mirror: keep the left half (-X: arm_a, leg_a) and mirror it over x = 0, so both sides are the same shape and
    the right side can reuse the left side's texels (twice the texture detail per side). The seam on x = 0 is welded."""
    bm_ = bmesh.new()
    bm_.from_mesh(ob.data)
    geom = bm_.verts[:] + bm_.edges[:] + bm_.faces[:]
    bmesh.ops.bisect_plane(bm_, geom=geom, dist=1e-6, plane_co=(0, 0, 0), plane_no=(1, 0, 0), clear_outer=True)
    for v in bm_.verts:
        if abs(v.co.x) < 1e-5:
            v.co.x = 0.0
    bmesh.ops.mirror(bm_, geom=bm_.verts[:] + bm_.edges[:] + bm_.faces[:], axis='X', merge_dist=1e-5)
    # bmesh.ops.mirror keeps the winding, so the mirrored half came out inside-out (normals inward: checked on a cube,
    # Nov 10). The game culls back faces: every mirrored body before this fix showed its right side's insides, and the
    # visibility cull deleted the whole right side. The new faces (x > 0) are reversed
    bm_.normal_update()
    bmesh.ops.reverse_faces(bm_, faces=[f for f in bm_.faces if f.calc_center_median().x > 1e-6])
    bmesh.ops.triangulate(bm_, faces=bm_.faces)
    bm_.to_mesh(ob.data)
    bm_.free()
    ob.data.update()
    n1_ = len(ob.data.polygons)
    # the cut along x = 0 adds triangles: decimate again to the budget, with the modifier's symmetry so it stays mirrored
    if n1_ > target:
        dec = ob.modifiers.new('DecSym', 'DECIMATE')
        dec.ratio = target / n1_
        dec.use_collapse_triangulate = True
        dec.use_symmetry = True
        dec.symmetry_axis = 'X'
        with bpy.context.temp_override(object=ob, active_object=ob):
            bpy.ops.object.modifier_apply(modifier='DecSym')
        # weld the centre line again (the decimation may leave it a hair off x = 0)
        bm_ = bmesh.new()
        bm_.from_mesh(ob.data)
        for v in bm_.verts:
            if abs(v.co.x) < 1e-4:
                v.co.x = 0.0
        bmesh.ops.remove_doubles(bm_, verts=bm_.verts, dist=1e-5)
        bm_.to_mesh(ob.data)
        bm_.free()
        ob.data.update()
    log(f'mirrored: {n1_} -> {len(ob.data.polygons)} triangles, symmetric about x = 0')


def finish_low(ob):
    ob.data.materials.clear()
    ob.data.shade_smooth()
    try:
        ob.data.set_sharp_from_angle(angle=math.radians(50))
    except AttributeError:
        pass


if REGION_TRIS or CULL:
    reg_names = list(REGION_TRIS) if REGION_TRIS else (['head', 'body'] if 'is_head' in lo_ob.data.attributes else ['body'])
    if REGION_TRIS:
        budget = dict(REGION_TRIS)
    elif 'is_head' in lo_ob.data.attributes:
        budget = {'head': int(round(BODY_TRIS * HEAD_SHARE)), 'body': BODY_TRIS - int(round(BODY_TRIS * HEAD_SHARE))}
    else:
        budget = {'body': BODY_TRIS}
    ih_ = np.zeros(len(lo_ob.data.polygons), np.int64)
    has_head = 'is_head' in lo_ob.data.attributes
    if has_head:
        lo_ob.data.attributes['is_head'].data.foreach_get('value', ih_)
    sx_ = abs(shoulder['a'].x)

    def reg_of(i, c_):
        if ih_[i] or (not has_head and c_.z > head_lo.z - 0.04 and abs(c_.x) < 0.15):
            return 'head'
        if not REGION_TRIS:
            return 'body'
        if c_.z < hip['a'].z + 0.02 and abs(c_.x) > 0.015:
            return 'legs'
        if abs(c_.x) > sx_ + 0.02:
            return 'arms'
        return 'torso'
    lab_ = [reg_names.index(reg_of(i, p.center)) for i, p in enumerate(lo_ob.data.polygons)]
    ra = lo_ob.data.attributes.new('reg', 'INT', 'FACE')
    ra.data.foreach_set('value', lab_)
    if has_head:
        lo_ob.data.attributes.remove(lo_ob.data.attributes['is_head'])
    def decimate_regions(ob, scale, sym=False):
        """each region decimated IN PLACE (edit-mode decimate of its faces only): the borders between regions stay
        connected. Splitting the regions into objects and decimating them apart left their borders unmatched: 2,044
        crack edges in the first split build (the stock body: 0; Nov 10, docs/robo_look.md "Seams")"""
        bpy.ops.object.select_all(action='DESELECT')
        ob.select_set(True)
        bpy.context.view_layer.objects.active = ob
        for v, name in enumerate(reg_names):
            n = int(budget[name] * scale)
            for _ in range(10):
                vals = np.zeros(len(ob.data.polygons), np.int64)
                ob.data.attributes['reg'].data.foreach_get('value', vals)
                have = int((vals == v).sum())
                if have <= n * 1.03 or have == 0:
                    break
                bpy.ops.object.mode_set(mode='EDIT')
                bpy.ops.mesh.select_mode(type='FACE')
                bm_ = bmesh.from_edit_mesh(ob.data)
                lay = bm_.faces.layers.int.get('reg')
                for f in bm_.faces:
                    f.select_set(False)
                for f in bm_.faces:
                    if f[lay] == v:
                        f.select_set(True)
                bm_.select_flush_mode()
                bmesh.update_edit_mesh(ob.data)
                bpy.ops.mesh.decimate(ratio=min(1.0, n / have), use_symmetry=sym, symmetry_axis='X')
                bpy.ops.object.mode_set(mode='OBJECT')
                vals = np.zeros(len(ob.data.polygons), np.int64)
                ob.data.attributes['reg'].data.foreach_get('value', vals)
                now = int((vals == v).sum())
                log(f'decimate {name} in place: {have} -> {now} (target {n})')
                if now > have * 0.97:
                    break

    # stage 1: each region to CULL_PAD x its budget, then the hidden faces out (the source of every try below)
    decimate_regions(lo_ob, CULL_PAD if CULL else 1.0)
    cull_src = lo_ob
    cull_src.name = 'Cull_source'
    if CULL:
        gone, kept = cull_hidden(cull_src)
        log(f'cull: {gone} hidden faces deleted, {kept} left (decimated to {CULL_PAD}x the budgets first)')

    if MIRROR:
        # mirrored before the last decimation (the source is still dense), so each region is then decimated
        # symmetrically straight to its own budget (a global trim after the mirror took the head from 900 to 512)
        mirror_mesh(cull_src, 10 ** 9)

    def build_low(scale):
        src_ = cull_src.copy()
        src_.data = cull_src.data.copy()
        bpy.context.scene.collection.objects.link(src_)
        ob = src_
        ob.name = 'Atlas_low'
        decimate_regions(ob, scale, sym=MIRROR)
        finish_low(ob)
        if MIRROR:
            weld_centre(ob)
        kept_ = len(ob.data.polygons)
        if CULL:
            # what the last decimation hid goes too, and every face gets its importance
            gone_, kept_ = cull_hidden(ob)
            cull_hidden(ob, delete=False)
            log(f'cull (final, budget x{scale:.2f}): {gone_} more hidden faces deleted, {kept_} triangles')
        return ob, kept_

    scale = 1.0
    lo_ob, kept = build_low(scale)
    for _try in range(2):
        # decimating hides faces again (seen 36% of the final mesh): raise the budgets by what the cull took, so the
        # visible triangles land on the budget
        if not CULL or kept >= BODY_TRIS * 0.93:
            break
        scale *= BODY_TRIS / max(kept, 1)
        bpy.data.objects.remove(lo_ob, do_unlink=True)
        lo_ob, kept = build_low(scale)
    bpy.data.objects.remove(cull_src, do_unlink=True)
    cnt_ = np.zeros(len(lo_ob.data.polygons), np.int64)
    lo_ob.data.attributes['reg'].data.foreach_get('value', cnt_)
    log('regions: ' + ', '.join(f'{n} {int((cnt_ == i).sum())} (budget {budget[n]})' for i, n in enumerate(reg_names))
        + f'; {len(lo_ob.data.polygons)} triangles, all visible')
    lo_ob.data.attributes.remove(lo_ob.data.attributes['reg'])
    if CULL:
        _imp = np.zeros(len(lo_ob.data.polygons))
        lo_ob.data.attributes['imp'].data.foreach_get('value', _imp)
        log(f'importance {_imp.min():.2f}..{_imp.max():.2f} (mean {_imp.mean():.2f})')
else:
    if 'is_head' in lo_ob.data.attributes:
        # --head: the close-up head and the body get their own budgets (one decimation spends triangles where the
        # surface is busiest, and the close-up is busier: seen 967 head triangles per side against 367 for the torso)
        hb = int(round(BODY_TRIS * HEAD_SHARE))
        head_part = lo_ob.copy()
        head_part.data = lo_ob.data.copy()
        bpy.context.scene.collection.objects.link(head_part)
        for ob_, keep in ((head_part, 1), (lo_ob, 0)):
            bm = bmesh.new()
            bm.from_mesh(ob_.data)
            lay = bm.faces.layers.int.get('is_head')
            bmesh.ops.delete(bm, geom=[f for f in bm.faces if f[lay] != keep], context='FACES')
            bm.to_mesh(ob_.data)
            bm.free()
        decimate_to(head_part, hb)
        decimate_to(lo_ob, BODY_TRIS - hb)
        for o in bpy.context.view_layer.objects:
            o.select_set(o in (head_part, lo_ob))
        bpy.context.view_layer.objects.active = lo_ob
        bpy.ops.object.join()
        lo_ob.data.attributes.remove(lo_ob.data.attributes['is_head'])
        log(f'head budget {hb} triangles, body {BODY_TRIS - hb}')
    else:
        decimate_to(lo_ob, BODY_TRIS)
    finish_low(lo_ob)
    log(f'decimated {n0} -> {len(lo_ob.data.polygons)} triangles')
    if MIRROR:
        mirror_mesh(lo_ob, BODY_TRIS)
lo_ob.name = 'Atlas_low'

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


def seg_dist(p, a_, b_):
    ab = b_ - a_
    t = max(0.0, min(1.0, (p - a_).dot(ab) / max(ab.length_squared, 1e-12)))
    return (p - (a_ + ab * t)).length, t


def core_radius(a_, b_, lo_t, hi_t, q=35):
    """a limb's core radius: the q-th percentile distance of the vertices along the middle of a bone to its axis
    (a low percentile: plates hanging outside the limb do not count)"""
    ds = []
    for v_ in me.vertices:
        d_, t_ = seg_dist(v_.co, a_, b_)
        if lo_t < t_ < hi_t and d_ < 0.25:
            ds.append(d_)
    return float(np.percentile(ds, q)) if ds else 0.05


if PLATES:
    thigh_r = {s: core_radius(hip[s], knee[s], 0.3, 0.7) for s in 'ab'}
    uarm_r = {s: core_radius(shoulder[s], elbow[s], 0.3, 0.7) for s in 'ab'}
    log('plates: thigh core radius', {s: round(r, 3) for s, r in thigh_r.items()},
        'upper arm', {s: round(r, 3) for s, r in uarm_r.items()})


def plate_fix(p, s, w):
    """skirt plates (around the thigh, outside its core) keep most of the hips' weight; shoulder plates (around the
    upper arm, outside and above its core) follow the shoulder joint more than the upper arm"""
    d_, t_ = seg_dist(p, hip[s], knee[s])
    if p.z < hip[s].z + 0.03 and t_ < 0.85:
        k = smoothstep((d_ - thigh_r[s] * 1.2) / (thigh_r[s] * 0.6)) * (1 - smoothstep((t_ - 0.6) / 0.25))
        if k > 0:
            tgt = {J('lower_body'): 0.7 - 0.35 * t_, J(f'leg_{s}_thigh'): 0.3 + 0.35 * t_}
            w = {b: x * (1 - k) for b, x in w.items()}
            for b, x in tgt.items():
                w[b] = w.get(b, 0) + x * k
    d_, t_ = seg_dist(p, shoulder[s], elbow[s])
    if t_ < 0.7 and p.z > shoulder[s].z - 0.02:
        k = smoothstep((d_ - uarm_r[s] * 1.3) / (uarm_r[s] * 0.6)) * (1 - smoothstep((t_ - 0.45) / 0.25))
        if k > 0:
            tgt = {J(f'arm_{s}_shoulder'): 0.6, J(f'arm_{s}_upper'): 0.4}
            w = {b: x * (1 - k) for b, x in w.items()}
            for b, x in tgt.items():
                w[b] = w.get(b, 0) + x * k
    return w
vweights = {}
vplate = set()
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
    # Nov 11, 2026 (bone-coloured posed render): the head test had no side limit, so the tops of the T-posed arms
    # (at helmet height) followed the chest and tore into flat sheets when the arms dropped; and the leg test wanted
    # every vertex 1.5 cm off the centre line at every height, so the inner faces of legs standing close together
    # followed the hips down to the ankles and tore between the knees when the legs spread
    # (the helmet's horns and side flaps reach wider than the shoulders: above shoulder height they are the head's)
    if p.z > head_lo.z - 0.04 and (abs(p.x) < arm_start or p.z > shoulder[s].z + 0.08):
        w = neck_w(p.z)
    else:
        t_arm = smoothstep((abs(p.x) - arm_start) / 0.08) * smoothstep((p.z - (shoulder[s].z - 0.16)) / 0.06)
        x_leg = max(smoothstep((abs(p.x) - 0.015) / 0.03), smoothstep(((hip[s].z - 0.05) - p.z) / 0.06))
        t_leg = smoothstep(((hip[s].z + 0.02) - p.z) / 0.10) * x_leg
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
        if PLATES:
            w0 = w
            w = plate_fix(p, s, w)
            if RIGID:
                # a plate vertex: plate_fix moved its weights. In --rigid the shoulder plates (sode) hang along the
                # T-posed arm, so they follow the upper arm (rigid on the shoulder joint they stuck out sideways when
                # the arm dropped); the skirt keeps the hips
                moved = sum(abs(w.get(b, 0) - w0.get(b, 0)) for b in set(w) | set(w0))
                if moved > 0.3:
                    vplate.add(v.index)
                    sj, uj = J(f'arm_{s}_shoulder'), J(f'arm_{s}_upper')
                    if sj in w and p.z > hip[s].z + 0.1:
                        w[uj] = w.get(uj, 0) + w.pop(sj)
    tot = sum(w.values())
    if RIGID:
        vweights[v.index] = {b: x / tot for b, x in w.items()}
        continue
    for b, x in w.items():
        add(v.index, b, x / tot)
if RIGID:
    # Each face picks one joint (its corners' summed weights), the picks are smoothed by a majority vote of the
    # neighbouring faces (single faces of another joint were confetti in the first rigid try), and a face stays SKINNED
    # only where it is really between two joints (its corners' strongest weight below RIGID_BAND) and is not a plate:
    # the bands at the elbows, knees and waist that keep the limbs closed, as Ray 01's two envelope PObjs among 42
    # rigid ones. Rigid faces of different joints are split apart; a band vertex touching a rigid face takes that
    # face's joint, so the band stretches between rigid pieces
    for vi in range(len(me.vertices)):
        if vi not in vweights:
            g = max(me.vertices[vi].groups, key=lambda x: x.weight)
            vweights[vi] = {lo_ob.vertex_groups[g.group].name: 1.0}
    bone_names = sorted({b for d in vweights.values() for b in d})
    bidx = {b: i for i, b in enumerate(bone_names)}
    nf = len(me.polygons)
    fbone = np.zeros(nf, np.int64)
    fband = np.zeros(nf, bool)
    for f in me.polygons:
        acc = {}
        for vi in f.vertices:
            for b, x in vweights[vi].items():
                acc[b] = acc.get(b, 0.0) + x
        fbone[f.index] = bidx[max(acc, key=acc.get)]
        strongest = np.mean([max(vweights[vi].values()) for vi in f.vertices])
        plate = any(vi in vplate for vi in f.vertices)
        fband[f.index] = (not plate) if RIGID_PLATES_ONLY else (strongest < RIGID_BAND and not plate)
    # face neighbours by shared edge
    bm = bmesh.new()
    bm.from_mesh(me)
    bm.faces.ensure_lookup_table()
    nbr = [[g.index for e in f.edges for g in e.link_faces if g.index != f.index] for f in bm.faces]
    for _ in range(3):
        nxt = fbone.copy()
        for i in range(nf):
            if len(nbr[i]) >= 2:
                cnt = {}
                for j in nbr[i]:
                    cnt[fbone[j]] = cnt.get(fbone[j], 0) + 1
                top = max(cnt, key=cnt.get)
                if cnt[top] >= 2 and cnt.get(fbone[i], 0) == 0:
                    nxt[i] = top
        fbone = nxt
    for g in list(lo_ob.vertex_groups):
        lo_ob.vertex_groups.remove(g)
    groups.clear()
    lay = bm.faces.layers.int.new('rigid_bone')
    lay_b = bm.faces.layers.int.new('rigid_band')
    for f in bm.faces:
        f[lay] = int(fbone[f.index])
        f[lay_b] = int(fband[f.index])
    old_index = bm.verts.layers.int.new('old_index')
    for v_ in bm.verts:
        v_[old_index] = v_.index
    cut = [e for e in bm.edges
           if len({f[lay] for f in e.link_faces if not f[lay_b]}) > 1]
    bmesh.ops.split_edges(bm, edges=cut)
    bm.verts.ensure_lookup_table()
    vw_new = {}
    for v_ in bm.verts:
        rig = [f[lay] for f in v_.link_faces if not f[lay_b]]
        if rig:
            vw_new[v_.index] = {bone_names[max(set(rig), key=rig.count)]: 1.0}
        elif v_.link_faces:
            vw_new[v_.index] = vweights[v_[old_index]]
    bm.faces.layers.int.remove(lay)
    bm.faces.layers.int.remove(lay_b)
    bm.verts.layers.int.remove(old_index)
    bm.to_mesh(me)
    bm.free()
    me.update()
    for vi, d in vw_new.items():
        for b, x in d.items():
            add(vi, b, x)
    per = {}
    for bi in fbone:
        per[bone_names[bi]] = per.get(bone_names[bi], 0) + 1
    log(f'rigid: {len(cut)} edges split, {int(fband.sum())} of {nf} faces skinned (joint bands), '
        f'{len(vplate)} plate vertices; faces per joint {dict(sorted(per.items(), key=lambda kv: -kv[1]))}')
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


# --teximp (Nov 10, docs/robo_look.md "Texel importance"): pieces seen from the front get more texels, the backs of
# plates and undersides fewer (the old games gave the face and the chest the most). Each UV island is scaled about its
# centre by (its area-weighted 'imp', 0.6..1.6) ^ 0.75 before the pack, which keeps the islands' relative sizes
TEXIMP = bool(opts.get('teximp'))


def scale_islands(tm):
    bm_ = bmesh.new()
    bm_.from_mesh(tm)
    uvl = bm_.loops.layers.uv.active
    il = bm_.faces.layers.float.get('imp')
    bm_.faces.ensure_lookup_table()
    par = list(range(len(bm_.faces)))

    def find(a_):
        while par[a_] != a_:
            par[a_] = par[par[a_]]
            a_ = par[a_]
        return a_

    for e in bm_.edges:
        if len(e.link_faces) != 2:
            continue
        f1, f2 = e.link_faces

        def uv_at(f, v):
            return next(l[uvl].uv for l in f.loops if l.vert == v)
        if all((uv_at(f1, v) - uv_at(f2, v)).length < 1e-6 for v in e.verts):
            par[find(f1.index)] = find(f2.index)
    isl = {}
    for f in bm_.faces:
        isl.setdefault(find(f.index), []).append(f)
    for fs in isl.values():
        ar = np.array([f.calc_area() for f in fs])
        im = np.array([f[il] for f in fs])
        s = float(np.clip((ar * im).sum() / max(ar.sum(), 1e-12), 0.6, 1.6)) ** 0.75
        cen = sum((l[uvl].uv for f in fs for l in f.loops), Vector((0, 0))) / sum(len(f.loops) for f in fs)
        for f in fs:
            for l in f.loops:
                l[uvl].uv = cen + (l[uvl].uv - cen) * s
    bm_.to_mesh(tm)
    bm_.free()


def uv_region(f):
    """the texture cell of a face (--regions), from the same landmarks as the weights"""
    r = fregion.get(f.index, 'body')
    if r != 'body':
        return 'hands_feet'
    p = f.center
    if p.z > head_lo.z - 0.04 and abs(p.x) < 0.15:     # the T-posed arms and shoulder plates reach neck height
        return 'head'
    s = 'a' if p.x < 0 else 'b'
    if p.z < hip[s].z + 0.02 and abs(p.x) > 0.015:
        return 'leg_' + s
    return 'torso_arms'


if not REGIONS:
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
else:
    me = lo_ob.data
    if not me.uv_layers:
        me.uv_layers.new(name='UVMap')
    freg = {f.index: uv_region(f) for f in me.polygons}
    if MIRROR:
        # only the left half (x <= 0) is unwrapped; each right-side face gets its mirror twin's UVs afterwards
        freg = {i: (None if me.polygons[i].center.x > 1e-6 else ('leg' if r.startswith('leg') else r))
                for i, r in freg.items()}
    names = sorted(set(r for r in freg.values() if r))
    area = {k: 0.0 for k in names}
    for f in me.polygons:
        if freg[f.index]:
            area[freg[f.index]] += f.area
    W_, H_ = ATLAS
    if SECTIONS:
        cells = dict(SECTION_CELLS)
    elif MIRROR:
        # weighted layout: pixels per section ~ weight^2 x surface; full-height columns for the torso + arms and the
        # leg, the head over the hands / feet in the third column (cells: x, y, w, h in pixels, multiples of 4)
        want = {k: WEIGHTS[k] ** 2 * area[k] for k in names}
        tot_w = sum(want.values())
        wt = int(round(W_ * want['torso_arms'] / tot_w / 4)) * 4
        wl = int(round(W_ * want['leg'] / tot_w / 4)) * 4
        wc = W_ - wt - wl
        hh_ = min(H_ - H_ // 4, int(round(H_ * want['head'] / (want['head'] + want['hands_feet']) / 4)) * 4)  # hands / feet >= a quarter of the height
        cells = {'torso_arms': (0, 0, wt, H_), 'leg': (wt, 0, wl, H_),
                 'head': (wt + wl, H_ - hh_, wc, hh_), 'hands_feet': (wt + wl, 0, wc, H_ - hh_)}
    else:
        cells = {k: (x, y, s, s) for k, (x, y, s) in CELLS.items()}
    # one region at a time, each on a temporary copy that holds only its faces: without a UV editor, smart_project /
    # pack_islands touch every face of the mesh (hiding the others did not stop pack_islands moving the regions already
    # placed: the torso ended at u, v ~2), so the region is unwrapped alone and its UVs copied back by face index
    fidx = me.attributes.new('orig_face', 'INT', 'FACE')
    fidx.data.foreach_set('value', list(range(len(me.polygons))))
    uvd = me.uv_layers.active.data

    def unwrap_region(ids, cw, ch, deg, mode=UV_MODE):
        """the region's faces alone on a temporary copy, unwrapped (crease seams at deg, or smart_project) and packed
        for a cw x ch pixel cell (islands pre-squashed by the cell's shape, so texels end up square); returns (copy,
        fill, even): fill = used share of the cell, even = share of the surface whose texel density is within
        0.6x..1.7x of the region's median (little stretch)"""
        tmp = lo_ob.copy()
        tmp.data = me.copy()
        tmp.modifiers.clear()
        bpy.context.scene.collection.objects.link(tmp)
        tb = bmesh.new()
        tb.from_mesh(tmp.data)
        tb.faces.ensure_lookup_table()
        bmesh.ops.delete(tb, geom=[f for f in tb.faces if f.index not in ids], context='FACES')
        tb.to_mesh(tmp.data)
        tb.free()
        bpy.ops.object.select_all(action='DESELECT')
        tmp.select_set(True)
        bpy.context.view_layer.objects.active = tmp
        bpy.ops.object.mode_set(mode='EDIT')
        if mode == 'seams':
            # few big pieces: seams only along the armour's hard creases, then an angle-based unwrap of whole plates
            # (smart_project shreds a decimated scan-like mesh into hundreds of near-single-triangle pieces)
            bpy.ops.mesh.select_mode(type='EDGE')
            bpy.ops.mesh.select_all(action='DESELECT')
            bpy.ops.mesh.edges_select_sharp(sharpness=math.radians(deg))
            bpy.ops.mesh.mark_seam(clear=False)
            bpy.ops.mesh.select_mode(type='FACE')
            bpy.ops.mesh.select_all(action='SELECT')
            bpy.ops.uv.unwrap(method='ANGLE_BASED', margin=0.0)
        else:
            bpy.ops.mesh.select_all(action='SELECT')
            bpy.ops.uv.smart_project(angle_limit=math.radians(66), island_margin=0.0, area_weight=0.0, scale_to_bounds=False)
        if cw != ch:
            bpy.ops.object.mode_set(mode='OBJECT')
            for d_ in tmp.data.uv_layers.active.data:
                d_.uv = (d_.uv[0] / cw * ch, d_.uv[1])
            bpy.ops.object.mode_set(mode='EDIT')
        if TEXIMP and 'imp' in tmp.data.attributes:
            bpy.ops.object.mode_set(mode='OBJECT')
            scale_islands(tmp.data)
            bpy.ops.object.mode_set(mode='EDIT')
        bpy.ops.uv.select_all(action='SELECT')
        bpy.ops.uv.pack_islands(rotate=(cw == ch), scale=True, margin_method='FRACTION', margin=GAP_PX / min(cw, ch))
        bpy.ops.object.mode_set(mode='OBJECT')
        tm = tmp.data
        tuv = tm.uv_layers.active.data
        uva = np.zeros(len(tm.polygons))
        for f in tm.polygons:
            a_, b_, c_ = (tuv[li].uv for li in f.loop_indices[:3])
            uva[f.index] = abs((b_[0] - a_[0]) * (c_[1] - a_[1]) - (c_[0] - a_[0]) * (b_[1] - a_[1])) / 2
        a3 = np.array([f.area for f in tm.polygons])
        _p = np.array([tuv[li].uv[:] for li in range(len(tm.loops))])
        sp = _p.max(axis=0) - _p.min(axis=0)
        k_ = min(1 / max(sp[0], 1e-9), 1 / max(sp[1], 1e-9))
        d = uva / np.maximum(a3, 1e-12)
        med = np.median(d[a3 > 0])
        even = float(a3[(d > 0.36 * med) & (d < 2.9 * med)].sum() / max(a3.sum(), 1e-12))   # density^2 ratios
        return tmp, float(uva.sum() * k_ * k_), even

    for name in names:
        cx, cy, cw, ch = cells[name]
        ids = set(i for i, r in freg.items() if r == name)
        if not ids:
            log(f'region {name}: no faces')
            continue
        # try several crease angles; keep the unwrap that uses the cell best without stretching (fill x even)
        best = None
        mode = UV_REGION.get(name, UV_MODE)
        for deg in (SEAM_DEGS if mode == 'seams' else (0,)):
            tmp, fill, even = unwrap_region(ids, cw, ch, deg, mode)
            score = fill * even
            log(f'region {name}: {"seams " + str(deg) + " deg" if mode == "seams" else "smart"}: cell used '
                f'{100 * fill:.0f}%, evenly textured {100 * even:.0f}%')
            if best is None or score > best[0]:
                if best:
                    old_me = best[1].data
                    bpy.data.objects.remove(best[1], do_unlink=True)
                    bpy.data.meshes.remove(old_me)
                best = (score, tmp, deg)
            else:
                t_me = tmp.data
                bpy.data.objects.remove(tmp, do_unlink=True)
                bpy.data.meshes.remove(t_me)
        _, tmp, deg = best
        tm = tmp.data
        tuv = tm.uv_layers.active.data
        # fit the packed result into the cell uniformly (texels stay square), half a gap from the cell edges
        _p = np.array([tuv[li].uv[:] for li in range(len(tm.loops))])
        lo_uv = _p.min(axis=0)
        sp = _p.max(axis=0) - lo_uv
        eu, ev = 0.5 * GAP_PX / cw, 0.5 * GAP_PX / ch
        k = min((1 - 2 * eu) / max(sp[0], 1e-9), (1 - 2 * ev) / max(sp[1], 1e-9))
        orig = tm.attributes['orig_face'].data
        _o = [orig[f.index].value for f in tm.polygons]
        assert set(_o) <= ids and len(_o) == len(ids)
        for f in tm.polygons:
            of = me.polygons[orig[f.index].value]
            assert len(of.loop_indices) == len(f.loop_indices)
            for li_t, li_o in zip(f.loop_indices, of.loop_indices):
                u, v = (tuv[li_t].uv[0] - lo_uv[0]) * k + eu, (tuv[li_t].uv[1] - lo_uv[1]) * k + ev
                uvd[li_o].uv = ((cx + u * cw) / W_, (cy + v * ch) / H_)
        tmp_me = tmp.data
        bpy.data.objects.remove(tmp, do_unlink=True)
        bpy.data.meshes.remove(tmp_me)
        log(f'region {name}: kept {"seams at " + str(deg) + " deg" if mode == "seams" else "smart"}, '
            f'{len(ids)} faces, {area[name]:.4f} units2, cell {cw}x{ch} at ({cx}, {cy})')
    if opts.get('pack') == 'one' and not SECTIONS:
        # --pack=one (Nov 10, docs/robo_look.md): the cells left most of a 512x512 atlas black (the stock robos fill
        # theirs edge to edge); every region's pieces packed again together over the whole atlas, keeping their
        # relative sizes (the head's extra density stays). Only the unwrapped (left) faces, on a copy (pack_islands
        # without a UV editor moves every face of the mesh)
        ids_all = set(i for i, r in freg.items() if r)
        tmp = lo_ob.copy()
        tmp.data = me.copy()
        tmp.modifiers.clear()
        bpy.context.scene.collection.objects.link(tmp)
        tb = bmesh.new()
        tb.from_mesh(tmp.data)
        bmesh.ops.delete(tb, geom=[f for f in tb.faces if f.index not in ids_all], context='FACES')
        tb.to_mesh(tmp.data)
        tb.free()
        # texels square on the atlas: work in pixel-proportional UVs
        for d_ in tmp.data.uv_layers.active.data:
            d_.uv = (d_.uv[0] * W_ / max(W_, H_), d_.uv[1] * H_ / max(W_, H_))
        bpy.ops.object.select_all(action='DESELECT')
        tmp.select_set(True)
        bpy.context.view_layer.objects.active = tmp
        bpy.ops.object.mode_set(mode='EDIT')
        bpy.ops.mesh.select_all(action='SELECT')
        bpy.ops.uv.select_all(action='SELECT')
        bpy.ops.uv.pack_islands(rotate=True, scale=True, margin_method='FRACTION', margin=GAP_PX / max(W_, H_),
                                shape_method='CONCAVE', rotate_method='ANY')
        bpy.ops.object.mode_set(mode='OBJECT')
        tuv = tmp.data.uv_layers.active.data
        orig = tmp.data.attributes['orig_face'].data
        used = 0.0
        for f in tmp.data.polygons:
            of = me.polygons[orig[f.index].value]
            for li_t, li_o in zip(f.loop_indices, of.loop_indices):
                u, v = tuv[li_t].uv
                uvd[li_o].uv = (u * max(W_, H_) / W_, v * max(W_, H_) / H_)
            a_, b_, c_ = (tuv[li].uv for li in f.loop_indices[:3])
            used += abs((b_[0] - a_[0]) * (c_[1] - a_[1]) - (c_[0] - a_[0]) * (b_[1] - a_[1])) / 2
        tmp_me = tmp.data
        bpy.data.objects.remove(tmp, do_unlink=True)
        bpy.data.meshes.remove(tmp_me)
        log(f'pack=one: all regions packed together, {100 * used:.0f}% of the atlas covered')
    if MIRROR:
        # right-side faces: the UVs of their mirror twin (corner by corner, matched by the mirrored vertex position)
        from mathutils.kdtree import KDTree
        left = [f for f in me.polygons if freg[f.index]]
        kd = KDTree(len(left))
        for n_, f in enumerate(left):
            kd.insert(f.center, n_)
        kd.balance()
        worst = 0.0
        for f in me.polygons:
            if freg[f.index]:
                continue
            c_ = f.center
            _, n_, dist = kd.find(Vector((-c_.x, c_.y, c_.z)))
            t_ = left[n_]
            worst = max(worst, dist)
            for li in f.loop_indices:
                p_ = me.vertices[me.loops[li].vertex_index].co
                q_ = Vector((-p_.x, p_.y, p_.z))
                lt = min(t_.loop_indices, key=lambda l_: (me.vertices[me.loops[l_].vertex_index].co - q_).length)
                uvd[li].uv = uvd[lt].uv[:]
        log(f'mirror: right side shares the left side\'s texels (worst twin distance {worst:.6f})')
    me.attributes.remove(me.attributes['orig_face'])
    if 'imp' in me.attributes:
        me.attributes.remove(me.attributes['imp'])
    bpy.ops.object.select_all(action='DESELECT')
    lo_ob.select_set(True)
    bpy.context.view_layer.objects.active = lo_ob
    # texel density per section, relative to spreading the whole atlas evenly over the unwrapped surface
    uni = math.sqrt(W_ * H_ / sum(area.values()))
    log('texel density vs a uniform spread: ' + ', '.join(
        f'{k} {math.sqrt(cells[k][2] * cells[k][3] / max(area[k], 1e-9)) / uni:.2f}x' for k in names if area[k])
        + (' (each side; the mirror doubles the surface per texel)' if MIRROR else ''))

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
        if channel == 'point':
            # --edges: Cycles' pointiness (vertex curvature of the high model: 0.5 flat, above convex, below concave)
            geo = nt.nodes.get('BakeGeo') or nt.nodes.new('ShaderNodeNewGeometry')
            geo.name = 'BakeGeo'
            nt.links.new(geo.outputs['Pointiness'], em.inputs['Color'])
            nt.links.new(em.outputs[0], out.inputs['Surface'])
            continue
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
if MIRROR:
    # the right side shares the left side's texels: during the bakes its faces go to a throwaway image, so only the
    # left side writes into the atlas (two faces baking into the same pixels would fight)
    dummy_mat = bpy.data.materials.new('BakeMirrorSide')
    dummy_mat.use_nodes = True
    dummy_tex = dummy_mat.node_tree.nodes.new('ShaderNodeTexImage')
    dummy_tex.image = bpy.data.images.new('bake_mirror_side', 64, 64)
    dummy_mat.node_tree.nodes.active = dummy_tex
    lo_ob.data.materials.append(dummy_mat)
    for f in lo_ob.data.polygons:
        f.material_index = 1 if f.center.x > 1e-6 else 0
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
    # dilation: with --regions it fills the whole gap, so no CMP block mixes a piece with black
    scene.render.bake.margin = int(round(GAP_PX * SS)) if REGIONS else 6
    bpy.ops.object.bake(type=kind, use_clear=True)
    return np.array(img.pixels[:], dtype=np.float32).reshape(BH, BW, 4)


BW, BH = (ATLAS[0] * SS, ATLAS[1] * SS) if REGIONS else (BAKE, BAKE)
img_c = bpy.data.images.new('bake_colour', BW, BH)
img_m = bpy.data.images.new('bake_metal', BW, BH)
img_a = bpy.data.images.new('bake_ao', BW, BH)
hi_material_emit('color')
col = bake('EMIT', img_c)
hi_material_emit('metal')
met = bake('EMIT', img_m)
ao = bake('AO', img_a)
log('baked colour, metallic, ambient occlusion')
curv = None
if LOOK == 'paint' and opts.get('edges'):
    img_p = bpy.data.images.new('bake_point', BW, BH, float_buffer=True)
    img_p.colorspace_settings.name = 'Non-Color'
    hi_material_emit('point')
    curv = bake('EMIT', img_p)[:, :, :1]
    log(f'edges: curvature baked, {float(curv.min()):.2f}..{float(curv.max()):.2f} (0.5 flat)')
shade = None
if DETAIL:
    # --detail: the high-poly relief (rivets, plate edges, panel lines) the 3,600 triangles cannot carry, baked into
    # the colour as fixed light and shade (the game body has no normal map). Object-space normals of the high model
    # minus the low model's own; lit from front-top (x = 0, so both mirrored sides match)
    scene.render.bake.normal_space = 'OBJECT'
    img_n1 = bpy.data.images.new('bake_n_hi', BW, BH, float_buffer=True)
    img_n0 = bpy.data.images.new('bake_n_lo', BW, BH, float_buffer=True)
    for im_ in (img_n1, img_n0):
        im_.colorspace_settings.name = 'Non-Color'
    n1 = bake('NORMAL', img_n1)[:, :, :3] * 2 - 1
    n0 = bake('NORMAL', img_n0, selected=False)[:, :, :3] * 2 - 1
    L = np.array([0.0, 0.55, 0.83])
    L /= np.linalg.norm(L)
    dn = n1 - n0
    shade = np.clip(1 + DETAIL * (dn @ L), 0.55, 1.45)[:, :, None]
    log(f'detail: relief baked (strength {DETAIL}), shade {float(shade.min()):.2f}..{float(shade.max()):.2f}')
_uv = np.array([lp.uv[:] for lp in lo_ob.data.uv_layers.active.data]) if lo_ob.data.uv_layers else np.zeros((1, 2))
log(f'bake check: {BW}x{BH}, colour mean {col[:, :, :3].mean():.3f}, ao mean {ao[:, :, :1].mean():.3f}, '
    f'uv layers {len(lo_ob.data.uv_layers)} (active {lo_ob.data.uv_layers.active.name if lo_ob.data.uv_layers else "-"}), '
    f'uv range u {_uv[:, 0].min():.3f}..{_uv[:, 0].max():.3f} v {_uv[:, 1].min():.3f}..{_uv[:, 1].max():.3f}')
m_ = np.clip((met[:, :, :1] - 0.25) / 0.5, 0, 1)            # metal areas
occ = ao[:, :, :1]
rgb = col[:, :, :3] * ((0.72 + 0.28 * occ) if LOOK == 'game' else (0.50 + 0.50 * occ)) * 1.08
if shade is not None:
    rgb = rgb * shade
# PBR metal albedo is pale and flat (its look comes from reflections): metal goes darker and cooler with deeper
# occlusion, so it reads as iron / bronze; the game's specular mask (from the metallic map) adds the shine
iron = rgb * np.array([0.80, 0.82, 0.88]) * (0.65 + 0.35 * occ) + 0.07     # (lifted: chainmail read as flat black)
if LOOK == 'game':
    m_ = m_ * 0.45                                          # keep the lacquer red and the gold, only a hint of iron
rgb = rgb * (1 - m_) + iron * m_
lum = rgb.mean(axis=2, keepdims=True)
rgb = np.clip(lum + (rgb - lum) * 1.15, 0, 1)                # a little more colour
if LOOK == 'game':
    _l = rgb.mean(axis=2)
    _hi = float(np.percentile(_l[_l > 0.02], 98)) if (_l > 0.02).any() else 1.0
    rgb = np.clip(rgb / max(_hi / 0.95, 1e-3), 0, 1) ** 0.85   # levels: the brightest 2% near white, mid-tones lifted
    log(f'look=game: levels gain {0.95 / max(_hi, 1e-3):.2f}, gamma 0.85, colour mean now {rgb.mean():.3f}')


# --look=paint (Nov 10, 2026, docs/robo_look.md): the bake repainted in the stock robos' painted style
# (tools/blender/paint_look.py): flat materials, one light ramp each, panel lines, a specular mask per material
def island_raster(ob, W_, H_):
    """the UV island number of every bake texel (-1 = gutter), from the baked (left-side) faces: the game samples the
    textures bilinearly with no mipmaps (HSD_TObjSetup: no mipmap -> min filter LINEAR) and stores them CMPR (4x4
    blocks of two colours), so what sits in a gutter shows on the island's edge (docs/robo_look.md "Seams")"""
    me_ = ob.data
    uvd_ = me_.uv_layers.active.data
    faces = [p for p in me_.polygons if not (MIRROR and p.center.x > 1e-6)]
    idx = {p.index: k for k, p in enumerate(faces)}
    par_ = list(range(len(faces)))

    def find(a_):
        while par_[a_] != a_:
            par_[a_] = par_[par_[a_]]
            a_ = par_[a_]
        return a_
    edge_uv = {}
    for p in faces:
        li = list(p.loop_indices)
        for j in range(len(li)):
            a_, b_ = li[j], li[(j + 1) % len(li)]
            va, vb = me_.loops[a_].vertex_index, me_.loops[b_].vertex_index
            key = (min(va, vb), max(va, vb))
            uva = tuple(round(x, 6) for x in uvd_[a_].uv)
            uvb = tuple(round(x, 6) for x in uvd_[b_].uv)
            sig = (uva, uvb) if va < vb else (uvb, uva)
            if key in edge_uv:
                q, s = edge_uv[key]
                if s == sig:
                    par_[find(idx[p.index])] = find(q)
            else:
                edge_uv[key] = (idx[p.index], sig)
    out = np.full((H_, W_), -1, np.int32)
    for p in faces:
        isl = find(idx[p.index])
        pts = np.array([uvd_[li].uv[:] for li in p.loop_indices]) * np.array([W_, H_])
        for t in range(1, len(pts) - 1):
            a_, b_, c_ = pts[0], pts[t], pts[t + 1]
            x0, x1 = int(max(0, np.floor(min(a_[0], b_[0], c_[0])))), int(min(W_ - 1, np.ceil(max(a_[0], b_[0], c_[0]))))
            y0, y1 = int(max(0, np.floor(min(a_[1], b_[1], c_[1])))), int(min(H_ - 1, np.ceil(max(a_[1], b_[1], c_[1]))))
            if x1 < x0 or y1 < y0:
                continue
            xs, ys = np.meshgrid(np.arange(x0, x1 + 1) + 0.5, np.arange(y0, y1 + 1) + 0.5)
            d = (b_[1] - c_[1]) * (a_[0] - c_[0]) + (c_[0] - b_[0]) * (a_[1] - c_[1])
            if abs(d) < 1e-12:
                continue
            l1 = ((b_[1] - c_[1]) * (xs - c_[0]) + (c_[0] - b_[0]) * (ys - c_[1])) / d
            l2 = ((c_[1] - a_[1]) * (xs - c_[0]) + (a_[0] - c_[0]) * (ys - c_[1])) / d
            inside = (l1 >= -1e-3) & (l2 >= -1e-3) & (1 - l1 - l2 >= -1e-3)
            sub = out[y0:y1 + 1, x0:x1 + 1]
            sub[inside] = isl
    return out


if LOOK == 'paint':
    import paint_look  # noqa: E402
    isl_map = island_raster(lo_ob, BW, BH)
    log(f'islands: {len(np.unique(isl_map)) - 1} UV islands, {float((isl_map >= 0).mean()) * 100:.0f}% of the bake covered')
    rgb, met, lab_id, rep = paint_look.paint(np.clip(col[:, :, :3], 0, 1), SS, paint_look.palette(opts.get('paint')),
                                             occ=occ, shade=shade, nrm=n1 if DETAIL else None, curv=curv,
                                             edges=float(opts.get('edges') or 0), isl=isl_map,
                                             gutter=int(GAP_PX * SS * 2))
    _pal = rep.pop('palette')
    log(f'paint: materials (% of the bake) {rep}')
    lab_vis = np.array([p[3] for p in _pal], np.float32)[lab_id]
    _lv = bpy.data.images.new('paint_labels', BW, BH, alpha=False)
    _lv.pixels[:] = np.dstack([lab_vis, np.ones((BH, BW, 1), np.float32)]).ravel()
    _lv.filepath_raw = os.path.splitext(DST)[0] + '.paint_labels.png'
    _lv.file_format = 'PNG'
    _lv.save()


def lanczos_matrix(n_in, n_out, a=3):
    """n_out x n_in Lanczos-a resampling matrix (downscale, kernel widened by the scale factor)"""
    s = n_in / n_out
    x = (np.arange(n_out) + 0.5) * s - 0.5
    j = np.arange(n_in)
    t = (j[None, :] - x[:, None]) / s
    k = np.sinc(t) * np.sinc(t / a)
    k[np.abs(t) >= a] = 0
    return k / k.sum(axis=1, keepdims=True)


def unsharp(px, amount=0.35, sigma=0.8):
    """light unsharp mask on the final-size image (separable Gaussian, edges clamped)"""
    r = 2
    g = np.exp(-0.5 * (np.arange(-r, r + 1) / sigma) ** 2)
    g /= g.sum()
    pad = np.pad(px, ((r, r), (r, r), (0, 0)), mode='edge')
    bl = sum(g[i] * pad[:, i:i + px.shape[1]] for i in range(2 * r + 1))
    bl = sum(g[i] * bl[i:i + px.shape[0]] for i in range(2 * r + 1))
    return np.clip(px + amount * (px - bl), 0, 1)


def down(px, w, h):
    """bake -> w x h: area average (old path), or Lanczos-3 + light sharpening (--regions)"""
    if not REGIONS:
        return px.reshape(h, BAKE // h, w, BAKE // w, -1).mean(axis=(1, 3))
    my, mx = lanczos_matrix(px.shape[0], h), lanczos_matrix(px.shape[1], w)
    out = np.einsum('ij,jkc->ikc', my, px)
    out = np.einsum('ij,kjc->kic', mx, out)
    return unsharp(np.clip(out, 0, 1))


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

def make_mat(name, img_, mri_):
    m = bpy.data.materials.new(name)
    m.use_nodes = True
    t = m.node_tree
    t.nodes.clear()
    o1 = t.nodes.new('ShaderNodeOutputMaterial')
    b1 = t.nodes.new('ShaderNodeBsdfPrincipled')
    t.links.new(b1.outputs[0], o1.inputs[0])
    x1 = t.nodes.new('ShaderNodeTexImage')
    x1.image = img_
    t.links.new(x1.outputs['Color'], b1.inputs['Base Color'])
    x2 = t.nodes.new('ShaderNodeTexImage')
    x2.image = mri_
    s1 = t.nodes.new('ShaderNodeSeparateColor')
    t.links.new(x2.outputs['Color'], s1.inputs['Color'])
    t.links.new(s1.outputs['Green'], b1.inputs['Roughness'])
    t.links.new(s1.outputs['Blue'], b1.inputs['Metallic'])
    m.use_backface_culling = True
    return m


if SECTIONS:
    # one image + material per section; each face's UVs moved from its cell of the virtual atlas to 0..1 of its image
    order = ['torso_arms', 'head', 'leg', 'hands_feet']      # torso first: material 0 (the block B joint's)
    uvd_ = lo_ob.data.uv_layers.active.data
    sec_of = {}
    for f in lo_ob.data.polygons:
        cu = sum(uvd_[li].uv[0] for li in f.loop_indices) / len(f.loop_indices) * W
        cv = sum(uvd_[li].uv[1] for li in f.loop_indices) / len(f.loop_indices) * Hh
        sec_of[f.index] = next((k for k in order if cells[k][0] <= cu < cells[k][0] + cells[k][2]
                                and cells[k][1] <= cv < cells[k][1] + cells[k][3]), 'torso_arms')
    lo_ob.data.materials.clear()
    for k in order:
        cx, cy, cw, ch = cells[k]
        im = bpy.data.images.new(f'Body_{k}', cw, ch, alpha=False)
        im.pixels[:] = np.ascontiguousarray(atlas[cy:cy + ch, cx:cx + cw]).ravel()
        im.filepath_raw = base + f'.tex_{k}.png'
        im.file_format = 'PNG'
        im.save()
        im.pack()
        mi = bpy.data.images.new(f'Body_{k}_mr', cw, ch, alpha=False)
        mi.colorspace_settings.name = 'Non-Color'
        mi.pixels[:] = np.ascontiguousarray(mr[cy:cy + ch, cx:cx + cw]).ravel()
        mi.pack()
        lo_ob.data.materials.append(make_mat(f'Body_{k}', im, mi))
    for f in lo_ob.data.polygons:
        k = sec_of[f.index]
        cx, cy, cw, ch = cells[k]
        f.material_index = order.index(k)
        for li in f.loop_indices:
            u, v = uvd_[li].uv
            uvd_[li].uv = ((u * W - cx) / cw, (v * Hh - cy) / ch)
    log('sections: ' + ', '.join(f'{k} {cells[k][2]}x{cells[k][3]} ({sum(1 for x in sec_of.values() if x == k)} tris)'
                                 for k in order))
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
if not SECTIONS:
    lo_ob.data.materials.clear()
    lo_ob.data.materials.append(mat)
    for f in lo_ob.data.polygons:
        f.material_index = 0            # (--mirror put the right side on a bake-only second slot)
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
