"""motion_film.py: a body through a whole motion, EVERY frame, 4 views, stretch heat, written live (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/motion_film.py -- <body.glb> <motion.glb> <out dir>
      [--every=1] [--size=260] [--fps=30] [--mp4=1] [--sheet=12]

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
SHEET = int(opts.get('sheet') or 12)
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
    o.hide_render = True
f0, f1 = (int(x) for x in act.frame_range)
w = None
paths = []
for fr in range(f0, f1 + 1, EVERY):
    bpy.context.scene.frame_set(fr)
    bpy.context.view_layer.update()
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
pick = paths[::max(1, SHEET // EVERY)] if SHEET > 1 else paths
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
    strips = getattr(ed, 'strips', None) or ed.sequences
    st = strips.new_image('film', paths[0], 1, 1)
    for p in paths[1:]:
        st.elements.append(os.path.basename(p))
    im = bpy.data.images.load(paths[0])
    sc.render.resolution_x, sc.render.resolution_y = im.size[0] - im.size[0] % 2, im.size[1] - im.size[1] % 2
    bpy.data.images.remove(im)
    sc.render.resolution_percentage = 100
    sc.frame_start, sc.frame_end = 1, len(paths)
    sc.render.fps = max(1, FPS // EVERY)
    sc.render.image_settings.file_format = 'FFMPEG'
    sc.render.ffmpeg.format = 'MPEG4'
    sc.render.ffmpeg.codec = 'H264'
    sc.render.ffmpeg.constant_rate_factor = 'HIGH'
    sc.render.filepath = os.path.join(OUT, 'film.mp4')
    sc.render.use_sequencer = True
    bpy.ops.render.render(animation=True)
    print('motion_film:', os.path.join(OUT, 'film.mp4'))
