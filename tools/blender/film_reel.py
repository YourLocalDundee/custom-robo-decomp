"""film_reel.py: join motion_film.py frame folders into one MP4, in the order given (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/film_reel.py -- <out.mp4> <frames dir> [<frames dir> ...]
      [--fps=30] [--hold=12]

Each folder's f_*.png play in order; the last frame of each motion is held --hold frames so the cuts read.
"""
import os
import sys

import bpy

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
pos = [x for x in a if not x.startswith('--')]
OUT, DIRS = os.path.abspath(pos[0]), [os.path.abspath(d) for d in pos[1:]]
FPS = int(opts.get('fps') or 30)
HOLD = int(opts.get('hold') or 12)
bpy.ops.wm.read_factory_settings(use_empty=True)
sc = bpy.context.scene
sc.sequence_editor_create()
ed = sc.sequence_editor
strips = ed.strips if hasattr(ed, 'strips') else ed.sequences
t = 1
size = None
for d in DIRS:
    fs = sorted(f for f in os.listdir(d) if f.startswith('f_') and f.endswith('.png'))
    if not fs:
        continue
    fs += [fs[-1]] * HOLD
    st = strips.new_image(os.path.basename(d), os.path.join(d, fs[0]), 1, t)
    for f in fs[1:]:
        st.elements.append(f)
    if size is None:
        im = bpy.data.images.load(os.path.join(d, fs[0]))
        size = (im.size[0] - im.size[0] % 2, im.size[1] - im.size[1] % 2)
        bpy.data.images.remove(im)
    t += len(fs)
    print(f'film_reel: {os.path.basename(d)}: {len(fs) - HOLD} frames')
sc.render.resolution_x, sc.render.resolution_y = size
sc.render.resolution_percentage = 100
sc.frame_start, sc.frame_end = 1, t - 1
sc.render.fps = FPS
if hasattr(sc.render.image_settings, 'media_type'):
    sc.render.image_settings.media_type = 'VIDEO'
sc.render.image_settings.file_format = 'FFMPEG'
sc.render.ffmpeg.format = 'MPEG4'
sc.render.ffmpeg.codec = 'H264'
sc.render.ffmpeg.constant_rate_factor = 'HIGH'
sc.render.filepath = OUT
sc.render.use_sequencer = True
bpy.ops.render.render(animation=True)
print('film_reel:', OUT, f'{(t - 1) / FPS:.1f} s')
