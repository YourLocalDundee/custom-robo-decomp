"""Basic Arena with a golden pillar: the first edited stage (plan.md step 75, docs/MODDING.md "Stages").

usage: blender -b --factory-startup --python tools/blender/stage_pillar.py -- <in.glb> <out.glb> [<roles.json>, ignored]
   run by the loop:  pwsh tools\\model_loop.ps1 -Mod basic_arena_pillar -Script stage_pillar -Category bg_data -Entries 000

The loop calls a stage script once per scene model (scene_data_<n>.glb). Every model except 0 goes through unchanged.
Model 0 of Basic Arena (bg_data entry 000) holds the floor (bone JOBJ_6, mesh Joint_6_Object_1: the 18 x 18 tiled
floor, material "VERTEX, TEX0": the stage's 512x512 atlas times the vertex colour, unlit). This script adds a box
(1.2 x 1.2 wide, 2.6 high) standing on the floor at game x = 4.5, z = 0 (clear of the arena's obstacles: collision
groups 6-9 of entry 001), rigid on JOBJ_6, with the floor's own material (HSDRawViewer --import shares the template
MObj: same texture, same render mode, nothing re-encoded) and a golden vertex colour (the material multiplies the
texture by it). Its UVs are the floor's UV rectangle, so each side shows the floor's tile pattern.
The pillar has no collision (the collision is entry 001, not edited): robos walk through it.

Coordinates: HSDRawViewer exports the game's coordinates as glTF (+Y up); Blender's importer turns glTF (x, y, z)
into Blender (x, -z, y).
"""
import os
import sys

import bpy

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

FLOOR = 'Joint_6_Object_1'          # the floor mesh (exporter name prefix)
BONE = 'JOBJ_6'                     # its joint
GAME_X, GAME_Z = 4.5, 0.0           # where the pillar stands (game units; the floor spans -8..8)
WIDTH, HEIGHT = 1.2, 2.6
GOLD = (1.0, 0.78, 0.30, 1.0)       # vertex colour (multiplies the floor texture)


def floor_uv_rect(floor):
    uv = floor.data.uv_layers[0].data
    us = [d.uv[0] for d in uv]
    vs = [d.uv[1] for d in uv]
    return min(us), min(vs), max(us), max(vs)


def add_pillar(arm, floor):
    u0, v0, u1, v1 = floor_uv_rect(floor)
    x, y = GAME_X, -GAME_Z           # Blender x, y; Blender z = game height
    floor_top = max((floor.matrix_world @ v.co).z for v in floor.data.vertices)
    h, w = HEIGHT, WIDTH / 2
    verts, faces, uvs = [], [], []
    corners = [(x - w, y - w), (x + w, y - w), (x + w, y + w), (x - w, y + w)]
    # four sides (outward, counter-clockwise seen from outside) and the top; no bottom (it stands on the floor)
    for i in range(4):
        (ax, ay), (bx, by) = corners[i], corners[(i + 1) % 4]
        base = len(verts)
        verts += [(ax, ay, floor_top), (bx, by, floor_top), (bx, by, floor_top + h), (ax, ay, floor_top + h)]
        faces.append((base, base + 1, base + 2, base + 3))
        uvs += [(u0, v0), (u1, v0), (u1, v1), (u0, v1)]
    base = len(verts)
    top = floor_top + h
    verts += [(cx, cy, top) for cx, cy in corners]
    faces.append((base, base + 1, base + 2, base + 3))
    uvs += [(u0, v0), (u1, v0), (u1, v1), (u0, v1)]

    me = bpy.data.meshes.new('StagePillar')
    me.from_pydata(verts, [], faces)
    me.update()
    layer = me.uv_layers.new(name='UVMap')
    for poly in me.polygons:
        for li in poly.loop_indices:
            layer.data[li].uv = uvs[me.loops[li].vertex_index]
    col = me.color_attributes.new(name='Col', type='BYTE_COLOR', domain='CORNER')
    for d in col.data:
        d.color = GOLD
    me.color_attributes.active_color = col
    me.materials.append(floor.data.materials[0])     # the floor's own material (shared by the importer)
    ob = bpy.data.objects.new('StagePillar', me)
    bpy.context.scene.collection.objects.link(ob)
    ob.parent = arm
    vg = ob.vertex_groups.new(name=BONE)
    vg.add(list(range(len(verts))), 1.0, 'REPLACE')
    ob.modifiers.new('Armature', 'ARMATURE').object = arm
    print(f'stage_pillar: pillar {WIDTH} x {WIDTH} x {HEIGHT} at game ({GAME_X}, {floor_top:.2f}, {GAME_Z}) on {BONE}, '
          f'material {floor.data.materials[0].name}, floor UVs ({u0:.3f}, {v0:.3f})-({u1:.3f}, {v1:.3f})')


def main():
    src, dst, _ = common.args(2, __doc__, 3)
    arm = common.load(src)
    if os.path.basename(src) == 'scene_data_0.glb':
        floors = [o for o in common.model_meshes() if o.name.startswith(FLOOR)]
        if len(floors) != 1 or BONE not in arm.data.bones:
            sys.exit(f'{src}: not Basic Arena model 0 (no {FLOOR} mesh / {BONE} bone); run it on bg_data entry 000')
        add_pillar(arm, floors[0])
        common.save(dst, export_vertex_color='ACTIVE')
    else:
        common.save(dst)


if __name__ == '__main__':
    main()
