"""motion_film.py: a body through a whole motion, EVERY frame, 4 views, stretch heat, written live (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/motion_film.py -- <body.glb> <motion.glb> <out dir>
      [--every=1] [--size=260] [--fps=30] [--mp4=1] [--sheet=12]
      [--textured=1 --parts=<parts export dir> --roles=<roles json>]

--textured=1: the body with its own textures instead of the stretch colours (use the game file's export, e.g.
build/deform/<body>_export/chara_000/root0_joint.glb), and with --parts the gun and the bomb on the forearms the way
the game mounts them in battle (role joint x mount offset x 0.1, robo_preview.py), 3 views.

The motion glb must be baked against this body (tools/deform_sweep.py / motion_strip.py do that: crmotion_rig
cmd_gltf with the body as the skeleton). Writes, as it goes:
  <out>/f_0000.png ...   one 4-view tile per frame (front, side, back, 3/4 top), each the moment it is rendered;
                         faces coloured by stretch against the rest pose (red > 1.6, yellow > 1.3, blue squash)
  <out>/progress.txt     the last frame done and its bad-triangle count (to follow a long run)
  <out>/sheet.png        every --sheet-th frame in a grid (end of run)
  <out>/film.mp4         all frames at --fps (Blender's own FFmpeg; end of run)
Owner, Nov 12: watch the edits at a much higher rate, live, not a few frames after the fact.
"""
import os
import sys

import bpy
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import watch  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
BODY, MOTION, OUT = [x for x in a if not x.startswith('--')][:3]
OUT = os.path.abspath(OUT)
EVERY = int(opts.get('every') or 1)
SIZE = int(opts.get('size') or 260)
FPS = int(opts.get('fps') or 30)
SHEET = int(opts['sheet']) if opts.get('sheet') not in (None, '') else 12
os.makedirs(OUT, exist_ok=True)

bpy.ops.wm.read_factory_settings(use_empty=True)
bpy.context.scene.render.fps = 30
bpy.ops.import_scene.gltf(filepath=MOTION)
act = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0].animation_data.action
act.use_fake_user = True
for o in list(bpy.data.objects):
    bpy.data.objects.remove(o, do_unlink=True)
bpy.ops.import_scene.gltf(filepath=BODY)
arm = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0]
meshes = [o for o in bpy.data.objects if o.type == 'MESH' and 'Icosphere' not in o.name]
for o in [o for o in bpy.data.objects if 'Icosphere' in o.name]:
    bpy.data.objects.remove(o, do_unlink=True)
TEXTURED = opts.get('textured') == '1'
PARTS = []       # (static part object, role pose bone, its glTF rest world, mount offset)
if TEXTURED and opts.get('parts') and opts.get('roles'):
    import json
    import math
    import struct
    from mathutils import Matrix, Quaternion, Vector
    C = Matrix.Rotation(math.radians(90), 4, 'X')
    Ci = C.inverted()
    roles = json.load(open(opts['roles'], encoding='utf-8'))['C']

    def rest_world(path):
        d = open(path, 'rb').read()
        n = struct.unpack_from('<I', d, 12)[0]
        nodes = json.loads(d[20:20 + n])['nodes']
        parent = {c: i for i, nd in enumerate(nodes) for c in nd.get('children', [])}

        def local(nd):
            if 'matrix' in nd:
                m = nd['matrix']
                return Matrix([m[0:4], m[4:8], m[8:12], m[12:16]]).transposed()
            q = nd.get('rotation', (0, 0, 0, 1))
            s_ = nd.get('scale', (1, 1, 1))
            return (Matrix.Translation(Vector(nd.get('translation', (0, 0, 0))))
                    @ Quaternion((q[3], q[0], q[1], q[2])).to_matrix().to_4x4() @ Matrix.Diagonal((s_[0], s_[1], s_[2], 1)))
        memo = {}

        def world(i):
            if i not in memo:
                memo[i] = (world(parent[i]) if i in parent else Matrix.Identity(4)) @ local(nodes[i])
            return memo[i]
        return {nd['name']: world(i) for i, nd in enumerate(nodes) if nd.get('name', '').startswith('JOBJ_')}
    REST = rest_world(BODY)
    for folder, root, role, off in (('weapon_gun_000', 1, 'arm_b_fore', (-0.02, 0, -0.01)),
                                    ('weapon_bom_000', 1, 'arm_a_fore', (-0.02, 0, 0.01))):
        pth = os.path.join(opts['parts'], folder, f'root{root}_joint.glb')
        if not os.path.exists(pth):
            continue
        before = set(bpy.data.objects)
        bpy.ops.import_scene.gltf(filepath=pth)
        new = [o for o in bpy.data.objects if o not in before]
        dg_ = bpy.context.evaluated_depsgraph_get()
        for o in new:
            if o.type == 'MESH' and 'Icosphere' not in o.name:
                me_ = bpy.data.meshes.new_from_object(o.evaluated_get(dg_))
                me_.transform(o.matrix_world)
                po = bpy.data.objects.new(f'{folder}_part', me_)
                bpy.context.scene.collection.objects.link(po)
                jn = f'JOBJ_{roles[role]}'
                PARTS.append((po, arm.pose.bones[jn], REST[jn], Vector(off)))
        for o in new:
            bpy.data.objects.remove(o, do_unlink=True)


def place_parts():
    for po, pb, rest, off in PARTS:
        d_bl = arm.matrix_world @ pb.matrix @ pb.bone.matrix_local.inverted() @ arm.matrix_world.inverted()
        po.matrix_world = d_bl @ C @ rest @ Matrix.Translation(off) @ Matrix.Diagonal((0.1, 0.1, 0.1, 1)) @ Ci


def baked():
    """the posed body as one static mesh object (all meshes joined), and its triangles' edge lengths"""
    dg = bpy.context.evaluated_depsgraph_get()
    vs, fs, base = [], [], 0
    for o in meshes:
        me = o.evaluated_get(dg).to_mesh()
        vs += [tuple(o.matrix_world @ v.co) for v in me.vertices]
        fs += [tuple(base + i for i in p.vertices) for p in me.polygons]
        base += len(me.vertices)
        o.evaluated_get(dg).to_mesh_clear()
    return np.array(vs), fs


def lengths(V, F):
    out = []
    for f in F:
        n = len(f)
        out.append([np.linalg.norm(V[f[(i + 1) % n]] - V[f[i]]) for i in range(n)])
    return out


arm.animation_data_create()
arm.animation_data.action = None
for pb in arm.pose.bones:
    pb.matrix_basis.identity()
bpy.context.view_layer.update()
V0, F = baked()
L0 = lengths(V0, F)
arm.animation_data.action = act
try:
    arm.animation_data.action_slot = act.slots[0]
except (AttributeError, IndexError):
    pass
for o in meshes:
    o.hide_render = not TEXTURED
f0, f1 = (int(x) for x in act.frame_range)
w = None
paths = []
ONLY_MP4 = opts.get('only_mp4') == '1'      # assemble the film from the frames already written
if ONLY_MP4:
    paths = sorted(os.path.join(OUT, f) for f in os.listdir(OUT) if f.startswith('f_') and f.endswith('.png'))
for fr in ([] if ONLY_MP4 else range(f0, f1 + 1, EVERY)):
    bpy.context.scene.frame_set(fr)
    bpy.context.view_layer.update()
    if TEXTURED:
        place_parts()
        if w is None:
            fr_box = None
            if opts.get('box'):
                bx = [float(x) for x in opts['box'].split(',')]
                from mathutils import Vector as _V
                fr_box = (_V(bx[:3]), bx[3])
            w = watch.Watcher(OUT, size=SIZE, textured=True, frame=fr_box,
                              views=[('front34', (-0.55, 1, 0.18)), ('side', (1, 0.1, 0.1)), ('back34', (0.6, -1, 0.25))])
            w.pad = float(opts.get('pad') or 1.0)
        paths.append(w.snap(f'f_{fr:04d}', meshes + [p[0] for p in PARTS]))
        with open(os.path.join(OUT, 'progress.txt'), 'w') as fh:
            fh.write(f'frame {fr} of {f0}..{f1}' + chr(10))
        continue
    V, _ = baked()
    me = bpy.data.meshes.new('film')
    me.from_pydata([tuple(v) for v in V], [], F)
    me.update()
    ob = bpy.data.objects.new('film', me)
    bpy.context.scene.collection.objects.link(ob)
    L = lengths(V, F)
    rmax, rmin, bad = [], [], 0
    for a_, b_ in zip(L, L0):
        r = [x / max(y, 1e-6) for x, y in zip(a_, b_) if y > 0.003] or [1.0]
        rmax.append(max(r))
        rmin.append(min(r))
        bad += max(r) > 1.6 or min(r) < 0.6
    if w is None:
        w = watch.Watcher(OUT, size=SIZE)
    paths.append(w.snap(f'f_{fr:04d}', [ob], heat=watch.heat_from_ratio(rmax, rmin)))
    with open(os.path.join(OUT, 'progress.txt'), 'w') as fh:
        fh.write(f'frame {fr} of {f0}..{f1}: {bad} bad triangles of {len(F)}\n')
    print(f'motion_film: frame {fr}/{f1} bad {bad}', flush=True)
    bpy.data.objects.remove(ob, do_unlink=True)
    bpy.data.meshes.remove(me)

# contact sheet
pick = paths[::max(1, SHEET // EVERY)] if SHEET > 0 else []      # --sheet=0: no contact sheet
imgs = []
for p in pick:
    im = bpy.data.images.load(p)
    imgs.append(np.array(im.pixels[:], np.float32).reshape(im.size[1], im.size[0], 4))
    bpy.data.images.remove(im)
if imgs:
    cols = 3
    h, wd = imgs[0].shape[:2]
    rows = (len(imgs) + cols - 1) // cols
    grid = np.ones((rows * h, cols * wd, 4), np.float32)
    for i, im in enumerate(imgs):
        r, c = divmod(i, cols)
        grid[(rows - 1 - r) * h:(rows - r) * h, c * wd:(c + 1) * wd] = im
    out = bpy.data.images.new('sheet', grid.shape[1], grid.shape[0], alpha=True)
    out.pixels = grid.ravel()
    out.filepath_raw = os.path.join(OUT, 'sheet.png')
    out.file_format = 'PNG'
    out.save()
    print('motion_film:', os.path.join(OUT, 'sheet.png'))

# the film: the frame tiles through the sequencer into an MP4 (Blender's own FFmpeg)
if opts.get('mp4', '1') != '0' and paths:
    sc = bpy.context.scene
    sc.sequence_editor_create()
    ed = sc.sequence_editor
    strips = ed.strips if hasattr(ed, 'strips') else ed.sequences      # Blender 5: strips (an empty one is falsy)
    st = strips.new_image('film', paths[0], 1, 1)
    for p in paths[1:]:
        st.elements.append(os.path.basename(p))
    im = bpy.data.images.load(paths[0])
    sc.render.resolution_x, sc.render.resolution_y = im.size[0] - im.size[0] % 2, im.size[1] - im.size[1] % 2
    bpy.data.images.remove(im)
    sc.render.resolution_percentage = 100
    sc.frame_start, sc.frame_end = 1, len(paths)
    sc.render.fps = max(1, FPS // EVERY)
    if hasattr(sc.render.image_settings, 'media_type'):
        sc.render.image_settings.media_type = 'VIDEO'     # Blender 5: video output is a media type
    sc.render.image_settings.file_format = 'FFMPEG'
    sc.render.ffmpeg.format = 'MPEG4'
    sc.render.ffmpeg.codec = 'H264'
    sc.render.ffmpeg.constant_rate_factor = 'HIGH'
    sc.render.filepath = os.path.join(OUT, 'film.mp4')
    sc.render.use_sequencer = True
    bpy.ops.render.render(animation=True)
    print('motion_film:', os.path.join(OUT, 'film.mp4'))
