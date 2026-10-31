"""Render preview images of a robo glb (or of the current Blender scene) in headless Blender: textured, three views
(front, three-quarter, side) in one PNG. Used for docs/images previews (docs/MODDING.md "Blender loop").

usage: blender -b --factory-startup --python tools/blender/render.py -- <in.glb> <out.png> [pose.json] [joints.json]

<pose.json>    optional: {"<bone name>": 4x4 world matrix rows} - meshes weighted 100% to that bone (or exported as
               Joint_<j>_Object_<d> of it) are moved by the matrix before rendering. The legionnaire scripts write one
               (<out>.pose.json) that puts the hands and feet, which sit at the IK effectors' odd bind positions in the
               game data, where they are drawn in the game.
<joints.json>  optional: the roles file HSDRawViewer --import --roles wrote (its "joints" list maps the output joint
               numbers back to the Blender bone names, for a re-exported glb whose bones are renumbered JOBJ_n).
The front is Blender +Y (glTF -Z): the robos face that way (checked on Ray 01: face and chest gem). Engine: Workbench (flat studio light, texture colours);
falls back to Cycles (CPU, 16 samples) if Workbench cannot run.
"""
import json
import math
import os
import sys

import bpy
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402


def apply_pose(pose, names):
    """Move rigid meshes of the bones in `pose` by their matrices (world space)."""
    moved = 0
    for o in common.model_meshes():
        bone = None
        if o.name.startswith('Joint_'):
            j = int(o.name.split('_')[1])
            bone = names[j] if names and j < len(names) else f'JOBJ_{j}'
        else:
            groups = {o.vertex_groups[g.group].name for v in o.data.vertices for g in v.groups if g.weight > 1e-3}
            if len(groups) == 1:
                bone = groups.pop()
        if bone in pose:
            m = Matrix(pose[bone])
            o.data.transform(o.matrix_world.inverted() @ m @ o.matrix_world)
            o.data.update()
            moved += 1
    print(f'render: moved {moved} meshes to their in-game hand / foot positions')


def frame(objs):
    pts = [o.matrix_world @ Vector(c) for o in objs for c in o.bound_box]
    lo = Vector((min(p.x for p in pts), min(p.y for p in pts), min(p.z for p in pts)))
    hi = Vector((max(p.x for p in pts), max(p.y for p in pts), max(p.z for p in pts)))
    return lo, hi


def render(out, title=None):
    out = os.path.abspath(out)
    scene = bpy.context.scene
    meshes = common.model_meshes()
    for o in meshes:   # no armature deformation surprises: render the bind pose
        for m in o.modifiers:
            if m.type == 'ARMATURE':
                m.show_render = m.show_viewport = False
    for o in bpy.context.scene.objects:
        if o.type == 'ARMATURE':
            o.hide_render = True
    lo, hi = frame(meshes)
    c = (lo + hi) / 2
    h = max(hi.z - lo.z, hi.x - lo.x, hi.y - lo.y)
    cam_data = bpy.data.cameras.new('PreviewCam')
    cam_data.type = 'ORTHO'
    cam_data.ortho_scale = h * 1.08
    cam = bpy.data.objects.new('PreviewCam', cam_data)
    scene.collection.objects.link(cam)
    scene.camera = cam
    res = 512
    scene.render.resolution_x, scene.render.resolution_y = res, res
    scene.render.film_transparent = False
    world = bpy.data.worlds.new('PreviewWorld')
    scene.world = world
    world.color = (0.16, 0.17, 0.2)
    try:
        scene.render.engine = 'BLENDER_WORKBENCH'
        sh = scene.display.shading
        sh.light = 'STUDIO'
        sh.color_type = 'TEXTURE'
        sh.show_cavity = True
        sh.cavity_type = 'WORLD'
        sh.show_specular_highlight = True
        sh.background_type = 'WORLD'
        scene.display.render_aa = '8'
    except Exception as e:  # pragma: no cover
        print(f'render: workbench settings failed ({e}); using Cycles')
        scene.render.engine = 'CYCLES'
        scene.cycles.samples = 16
        scene.cycles.device = 'CPU'
    views = [('front', 0), ('three-quarter', 35), ('side', 90), ('back', 180)]
    tiles = []
    base = os.path.splitext(out)[0]
    for name, az in views:
        a = math.radians(az)
        d = Vector((-math.sin(a), math.cos(a), 0.12)).normalized()   # front = +Y: Ray 01 faces Blender +Y
        cam.location = c + d * h * 3
        cam.rotation_euler = (-d).to_track_quat('-Z', 'Y').to_euler()
        scene.render.filepath = f'{base}.{name}.png'
        try:
            bpy.ops.render.render(write_still=True)
        except RuntimeError as e:
            if scene.render.engine != 'BLENDER_WORKBENCH':
                raise
            print(f'render: workbench failed ({e}); using Cycles')
            scene.render.engine = 'CYCLES'
            scene.cycles.samples = 16
            scene.cycles.device = 'CPU'
            bpy.ops.render.render(write_still=True)
        tiles.append(scene.render.filepath)
    # stitch the views side by side into one image
    imgs = [bpy.data.images.load(t) for t in tiles]
    w = sum(i.size[0] for i in imgs)
    hh = max(i.size[1] for i in imgs)
    sheet = bpy.data.images.new('sheet', w, hh)
    import numpy as np
    px = np.zeros((hh, w, 4), dtype=np.float32)
    x = 0
    for i in imgs:
        a = np.array(i.pixels[:], dtype=np.float32).reshape(i.size[1], i.size[0], 4)
        px[:i.size[1], x:x + i.size[0]] = a
        x += i.size[0]
    sheet.pixels[:] = px.ravel()
    sheet.filepath_raw = out
    sheet.file_format = 'PNG'
    sheet.save()
    for t in tiles:
        os.remove(t)
    for i in imgs + [sheet]:
        bpy.data.images.remove(i)
    # leave the scene exportable: armature modifiers and the armature back on, the camera gone
    for o in meshes:
        for m in o.modifiers:
            if m.type == 'ARMATURE':
                m.show_render = m.show_viewport = True
    for o in bpy.context.scene.objects:
        if o.type == 'ARMATURE':
            o.hide_render = False
    bpy.data.objects.remove(cam, do_unlink=True)
    print(f'render: wrote {out} ({w}x{hh}, views {", ".join(v for v, _ in views)}, {scene.render.engine})')


def main():
    src, out, pose_path, joints_path = common.args(2, __doc__, most=4)
    common.load(src)
    if pose_path:
        pose = json.load(open(pose_path, encoding='utf-8'))
        names = json.load(open(joints_path, encoding='utf-8')).get('joints') if joints_path else None
        apply_pose(pose, names)
    render(out)


if __name__ == '__main__':
    main()
