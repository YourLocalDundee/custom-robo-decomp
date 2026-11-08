"""Render a stage's scene models (HSDRawViewer --export glbs) from the battle's opening camera, to check a stage edit.

usage: blender -b --factory-startup --python tools/blender/stage_preview.py -- <dir with scene_data_<n>.glb> <out.png>
       [eye_x eye_y eye_z target_x target_y target_z fov]   (game units, +Y up; default: Basic Arena's scene camera)
       or "auto": that direction, pulled back to fit the stage (sky / backdrop meshes left out), dark void behind
       add "cull" for one-sided faces as the game draws them

Unlit materials show their texture x vertex colour, as the game draws "VERTEX, TEX0" materials; lit ones get one sun
and a soft sky, close to the stage's ambient + infinite light. Eevee, 1280 x 960.
"""
import glob
import math
import os
import sys

import bpy
from mathutils import Vector

a = sys.argv[sys.argv.index('--') + 1:]
src, out = a[0], a[1]
cam = [float(x) for x in a[2:9]] if len(a) >= 9 else [21.512, 13.165, 19.531, 0.0, -1.399, 0.0, 41.54]

bpy.ops.wm.read_factory_settings(use_empty=True)
for f in sorted(glob.glob(os.path.join(src, 'scene_data_*.glb'))):
    bpy.ops.import_scene.gltf(filepath=f)
for o in list(bpy.context.scene.objects):
    if o.type == 'MESH' and not o.visible_get():
        bpy.data.objects.remove(o)
    elif o.type == 'MESH' and not o.data.color_attributes:
        ca = o.data.color_attributes.new('Col', 'FLOAT_COLOR', 'POINT')   # no vertex colours: white, as the game
        for d in ca.data:
            d.color = (1, 1, 1, 1)
# vertex colours into the base colour, as the game's VERTEX materials
for m in bpy.data.materials:
    if 'cull' in a:
        m.use_backface_culling = True     # as the game: one-sided faces (a room's walls seen from outside vanish)
    if not m.use_nodes:
        continue
    nt = m.node_tree
    bsdf = next((n for n in nt.nodes if n.type == 'BSDF_PRINCIPLED'), None)
    if bsdf is None:
        continue
    tex_link = next((l for l in nt.links if l.to_socket == bsdf.inputs['Base Color']), None)
    attr = nt.nodes.new('ShaderNodeVertexColor')
    mul = nt.nodes.new('ShaderNodeMix')
    mul.data_type = 'RGBA'
    mul.blend_type = 'MULTIPLY'
    mul.inputs['Factor'].default_value = 1.0
    if tex_link:
        nt.links.new(tex_link.from_socket, mul.inputs[6])
    else:
        mul.inputs[6].default_value = bsdf.inputs['Base Color'].default_value
    nt.links.new(attr.outputs['Color'], mul.inputs[7])
    nt.links.new(mul.outputs[2], bsdf.inputs['Base Color'])
    bsdf.inputs['Specular IOR Level'].default_value = 0.1
    if m.get('unlit', True) and nt.nodes.get('Emission') is None:
        # the game draws these unlit: show base colour as emission so the baked vertex lighting is what you see
        em = nt.nodes.new('ShaderNodeEmission')
        em.name = 'Emission'
        nt.links.new(mul.outputs[2], em.inputs['Color'])
        mout = next(n for n in nt.nodes if n.type == 'OUTPUT_MATERIAL')
        nt.links.new(em.outputs[0], mout.inputs['Surface'])

scene = bpy.context.scene
scene.render.engine = 'BLENDER_EEVEE' if 'BLENDER_EEVEE' in bpy.types.RenderSettings.bl_rna.properties['engine'].enum_items.keys() else 'BLENDER_EEVEE_NEXT'
scene.render.resolution_x, scene.render.resolution_y = 1280, 960
scene.view_settings.view_transform = 'Standard'
world = bpy.data.worlds.new('W')
world.use_nodes = True
world.node_tree.nodes['Background'].inputs[0].default_value = (0.03, 0.03, 0.04, 1) if 'auto' in a else (0.55, 0.6, 0.7, 1)
world.node_tree.nodes['Background'].inputs[1].default_value = 0.6
scene.world = world
sun = bpy.data.objects.new('Sun', bpy.data.lights.new('Sun', 'SUN'))
sun.data.energy = 2.5
sun.rotation_euler = (math.radians(40), math.radians(15), math.radians(30))
scene.collection.objects.link(sun)


def g2b(x, y, z):
    return Vector((x, -z, y))


c = bpy.data.objects.new('Cam', bpy.data.cameras.new('Cam'))
scene.collection.objects.link(c)
c.location = g2b(*cam[0:3])
if 'auto' in a:
    # frame the playfield: Basic Arena's camera direction, pulled back to fit the stage's models, leaving out the
    # sky / backdrop meshes (several times bigger than the rest)
    boxes = []
    for o in scene.objects:
        if o.type == 'MESH':
            ws = [o.matrix_world @ Vector(v) for v in o.bound_box]
            lo = Vector([min(w[i] for w in ws) for i in range(3)])
            hi = Vector([max(w[i] for w in ws) for i in range(3)])
            boxes.append((lo, hi, (hi - lo).length))
    sizes = sorted(b[2] for b in boxes)
    med = sizes[len(sizes) // 2] if sizes else 1
    core = [b for b in boxes if b[2] <= max(6 * med, 40)] or boxes
    for o in [o for o in scene.objects if o.type == 'MESH']:
        ws = [o.matrix_world @ Vector(v) for v in o.bound_box]
        big = (Vector([max(w[i] for w in ws) for i in range(3)]) - Vector([min(w[i] for w in ws) for i in range(3)])).length > max(6 * med, 40)
        textured = any(s.material and s.material.use_nodes and any(n.type == 'TEX_IMAGE' for n in s.material.node_tree.nodes)
                       for s in o.material_slots)
        if big and not textured:
            o.hide_render = True        # oversize untextured helper meshes (effect / shadow planes) the game does not show plainly
    lo = Vector([min(b[0][i] for b in core) for i in range(3)])
    hi = Vector([max(b[1][i] for b in core) for i in range(3)])
    mid, rad = (lo + hi) / 2, max((hi - lo).length / 2, 1)
    dirv = (g2b(*cam[0:3]) - g2b(*cam[3:6])).normalized()
    c.location = mid + dirv * rad / math.tan(math.radians(cam[6]) / 2) * 0.95
    cam[3:6] = [mid.x, mid.z, -mid.y]
d = g2b(*cam[3:6]) - c.location
c.rotation_euler = d.to_track_quat('-Z', 'Y').to_euler()
c.data.sensor_fit = 'VERTICAL'
c.data.angle = math.radians(cam[6])
c.data.clip_end = 2000
scene.camera = c
scene.render.filepath = out
if out.lower().endswith('.jpg'):
    scene.render.image_settings.file_format = 'JPEG'      # docs pictures: 640 x 480
    scene.render.image_settings.quality = 82
    scene.render.resolution_percentage = 50
bpy.ops.render.render(write_still=True)
print('stage_preview: wrote', out)
