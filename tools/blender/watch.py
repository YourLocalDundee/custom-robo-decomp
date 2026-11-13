"""watch.py: quick 4-view snapshots for watching an edit while it runs (Nov 12, 2026).

Owner (Nov 12): "much higher recording rates than what we've been doing to watch what you doing so you could see
what's going on in live action instead of after the fact". Editing tools (rebind.py, carve_torso.py, reweight.py,
motion_film.py) call snap() at every stage / frame; each PNG is written the moment it is rendered.

  import watch
  w = watch.Watcher(out_dir, size=300)              # one camera rig, workbench, fixed framing
  w.snap('stage_name', objects, heat=None)          # -> out_dir/stage_name.png, 4 views side by side
  heat: optional per-face RGB list (len = polygons of the single object) painted as a face colour

Views: front, left side, back, three-quarter from above. The framing is fixed per Watcher (set from the first snap's
bounding box, or frame=(centre, half_size)), so successive snapshots line up and motion is visible between them.
"""
import math
import os

import bpy
import numpy as np
from mathutils import Vector

VIEWS = [('front', (0, -1, 0.12)), ('side', (1, 0, 0.12)), ('back', (0, 1, 0.12)), ('top34', (0.7, -0.7, 0.8))]


class Watcher:
    def __init__(self, out_dir, size=300, frame=None, textured=False, views=None):
        self.out = os.path.abspath(out_dir)      # Blender saves relative image paths elsewhere
        os.makedirs(self.out, exist_ok=True)
        self.size = size
        self.frame = frame
        sc = bpy.context.scene
        sc.render.engine = 'BLENDER_WORKBENCH'
        sc.display.shading.light = 'STUDIO'
        sc.display.shading.color_type = 'TEXTURE' if textured else 'VERTEX'      # VERTEX = colour attributes (5.2's name)
        self.textured = textured
        self.views = views or VIEWS
        sc.display.shading.show_shadows = False
        sc.display.shading.show_cavity = True
        sc.render.resolution_x = size
        sc.render.resolution_y = size
        sc.render.film_transparent = False
        sc.render.image_settings.file_format = 'PNG'
        cam_data = bpy.data.cameras.new('watch_cam')
        cam_data.type = 'ORTHO'
        self.cam = bpy.data.objects.new('watch_cam', cam_data)
        sc.collection.objects.link(self.cam)
        sc.camera = self.cam
        self.tmp = os.path.join(self.out, '_view.png')

    def _frame(self, objs):
        if self.frame:
            return self.frame
        pts = [o.matrix_world @ Vector(c) for o in objs for c in o.bound_box]
        lo = Vector((min(p.x for p in pts), min(p.y for p in pts), min(p.z for p in pts)))
        hi = Vector((max(p.x for p in pts), max(p.y for p in pts), max(p.z for p in pts)))
        self.frame = ((lo + hi) / 2, max(hi - lo) * 0.62)
        return self.frame

    def snap(self, name, objs, heat=None):
        centre, half = self._frame(objs)
        added = []
        if heat is not None:
            o = objs[0]
            me = o.data
            attr = me.color_attributes.get('watch_heat') or me.color_attributes.new('watch_heat', 'BYTE_COLOR', 'CORNER')
            cols = np.ones((len(me.loops), 4), np.float32)
            for p in me.polygons:
                c = heat[p.index]
                cols[p.loop_start:p.loop_start + p.loop_total, :3] = c
            attr.data.foreach_set('color', cols.ravel())
            me.color_attributes.active_color = attr
        elif not self.textured:
            for o in objs:
                if o.type == 'MESH' and not o.data.color_attributes:
                    a = o.data.color_attributes.new('watch_heat', 'BYTE_COLOR', 'CORNER')
                    a.data.foreach_set('color', np.full(len(o.data.loops) * 4, 0.8, np.float32))
                    added.append(o)
        tiles = []
        sc = bpy.context.scene
        self.cam.data.ortho_scale = half * 2 * getattr(self, 'pad', 1.0)
        for vn, d in self.views:
            dv = Vector(d).normalized()
            self.cam.location = centre + dv * (half * 6)
            self.cam.rotation_euler = (-dv).to_track_quat('-Z', 'Y').to_euler()     # a camera's up is its local Y
            sc.render.filepath = self.tmp
            bpy.ops.render.render(write_still=True)
            img = bpy.data.images.load(self.tmp, check_existing=False)
            px = np.array(img.pixels[:], np.float32).reshape(self.size, self.size, 4)
            bpy.data.images.remove(img)
            tiles.append(px)
        strip = np.concatenate(tiles, axis=1)
        out = bpy.data.images.new(name, strip.shape[1], strip.shape[0], alpha=True)
        out.pixels = strip.ravel()
        path = os.path.join(self.out, name + '.png')
        out.filepath_raw = path
        out.file_format = 'PNG'
        out.save()
        bpy.data.images.remove(out)
        print(f'watch: {path}', flush=True)
        return path


def heat_from_ratio(ratio_max, ratio_min):
    """per-face colour: grey ok, yellow stretch 1.3-1.6, red > 1.6, light blue squash 0.6-0.75, blue < 0.6"""
    out = []
    for hi, lo in zip(ratio_max, ratio_min):
        if hi > 1.6:
            out.append((0.9, 0.1, 0.05))
        elif hi > 1.3:
            out.append((0.95, 0.8, 0.1))
        elif lo < 0.6:
            out.append((0.1, 0.25, 0.9))
        elif lo < 0.75:
            out.append((0.45, 0.65, 0.95))
        else:
            out.append((0.75, 0.75, 0.75))
    return out
