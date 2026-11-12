"""A robo part (gun, bomb, pod, legs) from an AI-generated model, fitted onto one root of a stock part file.

usage: blender -b --factory-startup --python tools/blender/ai_part.py -- <stock root.glb> <ai model.glb> <out.glb>
       --joint=<n> [--tris=900] [--tex=256] [--fill=1.0] [--flip=<axes>] [--spin=<deg>]

<stock root.glb>  one root of a stock part file, exported by HSDRawViewer --export (rootN_joint.glb); a part file has
                  several roots (docs/formats/model_entry.md "Part files"), imported one at a time with --root N
<ai model.glb>    the AI part (TRELLIS output: front view = -Y, the picture's right = +X, up = +Z)
--joint=n         the joint (JOBJ_n of this root) that carries the new rigid mesh; root 0 of a part must keep a mesh on
                  the part's material joint (the file's header[4] path: JOBJ_4 for gun / bomb / pod, JOBJ_5 for legs),
                  the game sets up that joint's material for the whole part
--tris            triangle budget; --tex: texture size (square, CMP in the game); --fill: size vs the stock part's
                  longest side; --flip=xy: mirror the AI model on those axes before fitting; --spin: turn it about
                  the long axis (degrees)

How it fits: the AI model's axes sorted by length are matched to the stock part's (longest to longest); along the
long axis the end that is thicker goes where the stock part is thicker (the mount end), so a barrel points away from
the arm the way the stock barrel does; one uniform scale (the stock part's longest side x --fill); centred on the
stock part's box. Then: a decimated copy, smart UVs, the AI colour baked onto it (Cycles, selected to active,
emission), one material, every vertex 100% on --joint (the importer writes it rigid on that joint, in its space).
Writes <out.glb> and <out>.png (the texture) and <out>.preview.png.
"""
import math
import os
import sys

import bmesh
import bpy
import numpy as np
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

log = lambda *a: print('ai_part:', *a)
a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
a = [x for x in a if not x.startswith('--')]
STOCK, AI, DST = a[:3]
JOINT = f"JOBJ_{int(opts['joint'])}"
TRIS = int(opts.get('tris') or 900)
TEX = int(opts.get('tex') or 256)
FILL = float(opts.get('fill') or 1.0)
FLIP = opts.get('flip') or ''
SPIN = float(opts.get('spin') or 0)

arm = common.load(STOCK)
stock = common.model_meshes()
if JOINT not in arm.data.bones:
    sys.exit(f'{STOCK}: no bone {JOINT}')
spts = np.array([tuple(o.matrix_world @ v.co) for o in stock for v in o.data.vertices])
log(f'stock: {len(stock)} meshes, {len(spts)} vertices')


def frame(pts):
    """box centre, sorted axes (longest first) with extents, and the long axis' sign toward the thicker end"""
    lo, hi = pts.min(0), pts.max(0)
    ext = hi - lo
    order = list(np.argsort(-ext))
    c = (lo + hi) / 2
    ax = order[0]
    t = (pts[:, ax] - lo[ax]) / max(ext[ax], 1e-9)
    others = [i for i in range(3) if i != ax]

    def spread(mask):
        q = pts[mask][:, others]
        return float((q.max(0) - q.min(0)).prod()) if mask.sum() > 3 else 0.0
    thick = 1.0 if spread(t > 0.8) > spread(t < 0.2) else -1.0
    return c, order, ext, thick


# ---- the AI model, joined, flipped / spun as asked
for o in list(bpy.context.scene.objects):
    o.select_set(False)
before = set(bpy.context.scene.objects)
bpy.ops.import_scene.gltf(filepath=AI)
new = [o for o in bpy.context.scene.objects if o not in before]
hm = [o for o in new if o.type == 'MESH']
for o in new:
    if o.type != 'MESH':
        o.select_set(False)
bpy.ops.object.select_all(action='DESELECT')
for o in hm:
    o.select_set(True)
bpy.context.view_layer.objects.active = hm[0]
if len(hm) > 1:
    bpy.ops.object.join()
hi_ob = bpy.context.view_layer.objects.active
hi_ob.parent = None
bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
for o in new:
    if o is not hi_ob and o.name in bpy.data.objects:
        bpy.data.objects.remove(o, do_unlink=True)
hme = hi_ob.data
m0 = Matrix.Identity(4)
for ch in FLIP:
    i = 'xyz'.index(ch)
    m0[i][i] *= -1
hme.transform(m0)
if any(m0[i][i] < 0 for i in range(3)) and (sum(m0[i][i] < 0 for i in range(3)) % 2):
    hme.flip_normals()
hpts = np.array([tuple(v.co) for v in hme.vertices])
hc, hord, hext, hthick = frame(hpts)
sc, sord, sext, sthick = frame(spts)
log(f'stock box {np.round(sext, 3).tolist()} long axis {"xyz"[sord[0]]} thick end {"+" if sthick > 0 else "-"}; '
    f'AI box {np.round(hext, 3).tolist()} long axis {"xyz"[hord[0]]} thick end {"+" if hthick > 0 else "-"}')
# rotation: AI axis hord[k] -> stock axis sord[k]; the long axis signed by the thick ends, the third axis by handedness
R = np.zeros((3, 3))
R[sord[0], hord[0]] = sthick * hthick
R[sord[1], hord[1]] = 1.0
R[sord[2], hord[2]] = 1.0
if np.linalg.det(R) < 0:
    R[sord[2], hord[2]] = -1.0
if SPIN:
    ax = np.zeros(3)
    ax[sord[0]] = 1
    R = np.array(Matrix.Rotation(math.radians(SPIN), 3, Vector(ax))) @ R
s = FILL * sext[sord[0]] / max(hext[hord[0]], 1e-9)
side = min(sext[sord[i]] * 1.6 / max(hext[hord[i]] * s, 1e-9) for i in (1, 2))
if side < 1:
    s *= side                       # not much wider than the stock part across
M = Matrix.Identity(4)
for i in range(3):
    for j in range(3):
        M[i][j] = R[i, j] * s
M.translation = Vector(sc) - Vector(R @ hc * s)
hme.transform(M)
hme.update()
p2 = np.array([tuple(v.co) for v in hme.vertices])
log(f'fitted: scale {s:.4f}, box {np.round(p2.min(0), 3).tolist()} .. {np.round(p2.max(0), 3).tolist()}')

# ---- low copy
lo_ob = hi_ob.copy()
lo_ob.data = hme.copy()
lo_ob.name = 'Part'
bpy.context.scene.collection.objects.link(lo_ob)
bpy.ops.object.select_all(action='DESELECT')
lo_ob.select_set(True)
bpy.context.view_layer.objects.active = lo_ob
n0 = len(lo_ob.data.polygons)
# a voxel remesh first: the AI mesh is many loose, open pieces, which a collapse decimation shreds into slivers (as
# the close-up head in atlas_ai_body.py); the remesh makes one closed skin that decimates cleanly
VOX = float(opts.get('voxel') or 0) or float(max(p2.max(0) - p2.min(0))) / float(opts.get('vox_div') or 140)
rm = lo_ob.modifiers.new('rm', 'REMESH')
rm.mode = 'VOXEL'
rm.voxel_size = VOX
rm.adaptivity = 0.0
bpy.ops.object.modifier_apply(modifier=rm.name)
bm = bmesh.new()
bm.from_mesh(lo_ob.data)
shells = []
seen = set()
for f0 in bm.faces:
    if f0.index in seen:
        continue
    st, sh = [f0], []
    seen.add(f0.index)
    while st:
        f = st.pop()
        sh.append(f)
        for e in f.edges:
            for g in e.link_faces:
                if g.index not in seen:
                    seen.add(g.index)
                    st.append(g)
    shells.append(sh)
big = max(len(s_) for s_ in shells) if shells else 0
bmesh.ops.delete(bm, geom=[f for s_ in shells if len(s_) < big * 0.02 for f in s_], context='FACES')
bm.to_mesh(lo_ob.data)
bm.free()
log(f'voxel remesh {VOX:.4f}: {len(lo_ob.data.polygons)} faces, {len(shells)} shells')
for _ in range(6):
    n = len(lo_ob.data.polygons)
    if n <= TRIS:
        break
    d = lo_ob.modifiers.new('dec', 'DECIMATE')
    d.ratio = max(TRIS / n, 0.02)
    d.use_collapse_triangulate = True
    bpy.ops.object.modifier_apply(modifier=d.name)
bm = bmesh.new()
bm.from_mesh(lo_ob.data)
bmesh.ops.triangulate(bm, faces=bm.faces)
bm.to_mesh(lo_ob.data)
bm.free()
log(f'low: {n0} -> {len(lo_ob.data.polygons)} triangles')
lo_ob.data.materials.clear()
for uv in list(lo_ob.data.uv_layers):
    lo_ob.data.uv_layers.remove(uv)
lo_ob.data.uv_layers.new(name='UVMap')
bpy.ops.object.mode_set(mode='EDIT')
bpy.ops.mesh.select_all(action='SELECT')
bpy.ops.uv.smart_project(angle_limit=math.radians(66), island_margin=0.01, area_weight=0.0, scale_to_bounds=False)
bpy.ops.uv.pack_islands(rotate=True, scale=True, margin=0.01)
bpy.ops.object.mode_set(mode='OBJECT')
lo_ob.data.shade_smooth()

# ---- bake the AI colour (emission) onto the low copy
scene = bpy.context.scene
scene.render.engine = 'CYCLES'
scene.cycles.device = 'CPU'
scene.cycles.samples = 16
for m in hme.materials:
    if m is None or not m.use_nodes:
        continue
    nt = m.node_tree
    bsdf = next((n for n in nt.nodes if n.type == 'BSDF_PRINCIPLED'), None)
    out = next((n for n in nt.nodes if n.type == 'OUTPUT_MATERIAL'), None)
    if bsdf is None or out is None:
        continue
    em = nt.nodes.new('ShaderNodeEmission')
    src = bsdf.inputs['Base Color']
    if src.is_linked:
        nt.links.new(src.links[0].from_socket, em.inputs['Color'])
    else:
        em.inputs['Color'].default_value = tuple(src.default_value)
    nt.links.new(em.outputs[0], out.inputs['Surface'])
BK = TEX * 4
img = bpy.data.images.new('bake', BK, BK)
bm_ = bpy.data.materials.new('BakeTarget')
bm_.use_nodes = True
lo_ob.data.materials.append(bm_)
tn = bm_.node_tree.nodes.new('ShaderNodeTexImage')
tn.image = img
bm_.node_tree.nodes.active = tn
bpy.ops.object.select_all(action='DESELECT')
hi_ob.select_set(True)
lo_ob.select_set(True)
bpy.context.view_layer.objects.active = lo_ob
ext_ = float(max(sext)) * 0.01
scene.render.bake.use_selected_to_active = True
scene.render.bake.cage_extrusion = ext_
scene.render.bake.max_ray_distance = ext_ * 3
scene.render.bake.margin = 8
bpy.ops.object.bake(type='EMIT', use_clear=True)
px = np.array(img.pixels[:], dtype=np.float32).reshape(BK, BK, 4)
spec_small = None
if opts.get('look') == 'paint':
    # --look=paint: the stock parts' painted style (tools/blender/paint_look.py, docs/robo_look.md)
    import paint_look  # noqa: E402
    prgb, pspec, _ids, rep = paint_look.paint(np.clip(px[:, :, :3], 0, 1), 4, paint_look.palette(opts.get('paint')))
    rep.pop('palette')
    log(f'paint: materials (% of the bake) {rep}')
    px[:, :, :3] = prgb
    spec_small = pspec.reshape(TEX, 4, TEX, 4, 1).mean(axis=(1, 3))
small = px.reshape(TEX, 4, TEX, 4, 4).mean(axis=(1, 3))
rgb = small[:, :, :3]
if spec_small is None:
    lum = rgb.mean(axis=2)
    hi_ = float(np.percentile(lum[lum > 0.02], 98)) if (lum > 0.02).any() else 1.0
    rgb = np.clip(rgb / max(hi_ / 0.95, 1e-3), 0, 1) ** 0.9            # as the body's --look=game levels
small[:, :, :3] = rgb
small[:, :, 3] = 1
tex = bpy.data.images.new('Part_tex', TEX, TEX, alpha=False)
tex.pixels[:] = small.ravel()
base = os.path.splitext(DST)[0]
tex.filepath_raw = base + '.png'
tex.file_format = 'PNG'
tex.save()
tex.pack()
log(f'baked {BK}x{BK} -> {TEX}x{TEX}, colour mean {rgb.mean():.3f}')

mat = bpy.data.materials.new('Part')
mat.use_nodes = True
nt = mat.node_tree
bsdf = next(n for n in nt.nodes if n.type == 'BSDF_PRINCIPLED')
t_ = nt.nodes.new('ShaderNodeTexImage')
t_.image = tex
nt.links.new(t_.outputs['Color'], bsdf.inputs['Base Color'])
if spec_small is None:
    bsdf.inputs['Metallic'].default_value = 0.35
else:
    # the specular mask per material, as the body's (the importer turns metallic into the TEX1 specular map)
    mr_ = np.ones((TEX, TEX, 4), np.float32)
    mr_[:, :, 0] = 0
    mr_[:, :, 1] = 0.5
    mr_[:, :, 2] = spec_small[:, :, 0]
    mri_ = bpy.data.images.new('Part_mr', TEX, TEX, alpha=False)
    mri_.colorspace_settings.name = 'Non-Color'
    mri_.pixels[:] = mr_.ravel()
    mri_.pack()
    x2_ = nt.nodes.new('ShaderNodeTexImage')
    x2_.image = mri_
    s2_ = nt.nodes.new('ShaderNodeSeparateColor')
    nt.links.new(x2_.outputs['Color'], s2_.inputs['Color'])
    nt.links.new(s2_.outputs['Green'], bsdf.inputs['Roughness'])
    nt.links.new(s2_.outputs['Blue'], bsdf.inputs['Metallic'])
mat.use_backface_culling = True
lo_ob.data.materials.clear()
lo_ob.data.materials.append(mat)

# ---- rigid on the joint; the stock meshes and the AI model go
bpy.data.objects.remove(hi_ob, do_unlink=True)
for o in stock:
    bpy.data.objects.remove(o, do_unlink=True)
lo_ob.vertex_groups.new(name=JOINT).add(list(range(len(lo_ob.data.vertices))), 1.0, 'REPLACE')
lo_ob.parent = arm
lo_ob.matrix_parent_inverse = arm.matrix_world.inverted()
mod = lo_ob.modifiers.new('Armature', 'ARMATURE')
mod.object = arm
try:
    import render  # noqa: E402
    render.render(base + '.preview.png')
    for o in list(scene.objects):
        if o.name.startswith('PreviewCam'):
            bpy.data.objects.remove(o, do_unlink=True)
except Exception as e:  # the preview is a convenience
    log('preview failed:', e)
common.save(DST)
log(f'{len(lo_ob.data.polygons)} triangles on {JOINT}, texture {TEX}x{TEX}')
