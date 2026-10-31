"""Ray 01 as a Roman legionnaire: a tall red crest on the head bone, a big red / gold scutum on the left forearm bone,
the body texture recoloured to bronze. The model-loop test edit (docs/MODDING.md "Blender loop").

usage: blender -b --factory-startup --python tools/blender/legionnaire.py -- <in.glb> <out.glb> <roles.json>

<in.glb>      a robo exported by HSDRawViewer --export (tools/export_models.py), e.g.
              models_export/chara/chara_000/root0_joint.glb
<roles.json>  `python -I tools/robo_joints.py roles unpacked/chara/000.bin` (which bone is the head, which the forearms)

The skeleton is not touched (same bones, order, names JOBJ_n). Every new part is weighted 100% to one existing
bone (a vertex group named after it + an Armature modifier), so HSDRawViewer --import makes it a rigid DObj on
that joint and no roles file is needed.

Which forearm is the left one: the roles name the two arms a / b by the -X / +X side, not by left / right. The
script takes the robo's forward direction from the foot (ankle -> toe) and puts the shield on the arm on the
robo's own left (up x forward). It prints its choice.
"""
import json
import os
import sys

import bmesh
import bpy
import numpy as np
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

RED = (0.75, 0.02, 0.02, 1.0)
GOLD = (1.0, 0.72, 0.12, 1.0)
BRONZE = np.array([1.0, 0.68, 0.30])   # texture tint: luminance x this


def material(name, rgba):
    m = bpy.data.materials.get(name) or bpy.data.materials.new(name)
    m.diffuse_color = rgba
    if m.node_tree is None:
        m.use_nodes = True
    bsdf = next((n for n in m.node_tree.nodes if n.type == 'BSDF_PRINCIPLED'), None)
    if bsdf is None:
        bsdf = m.node_tree.nodes.new('ShaderNodeBsdfPrincipled')
        out = next(n for n in m.node_tree.nodes if n.type == 'OUTPUT_MATERIAL')
        m.node_tree.links.new(bsdf.outputs[0], out.inputs[0])
    bsdf.inputs['Base Color'].default_value = rgba
    bsdf.inputs['Metallic'].default_value = 0.0
    return m


def box(name, arm, bone, mat, center, axes, size):
    """A box centred at `center` (world), edges along the unit vectors `axes` (3), full lengths `size` (3),
    weighted 100% to `bone`."""
    bm = bmesh.new()
    bmesh.ops.create_cube(bm, size=1.0)
    m = Matrix.Identity(4)
    for i in range(3):
        for r in range(3):
            m[r][i] = axes[i][r] * size[i]
        m[i][3] = center[i]
    bmesh.ops.transform(bm, matrix=m, verts=bm.verts)
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
    me = bpy.data.meshes.new(name)
    bm.to_mesh(me)
    bm.free()
    me.materials.append(mat)
    ob = bpy.data.objects.new(name, me)
    bpy.context.scene.collection.objects.link(ob)
    ob.parent = arm
    ob.matrix_parent_inverse = arm.matrix_world.inverted()
    vg = ob.vertex_groups.new(name=bone)
    vg.add(list(range(len(me.vertices))), 1.0, 'REPLACE')
    mod = ob.modifiers.new('Armature', 'ARMATURE')
    mod.object = arm
    return ob


def world_bbox(objs):
    pts = [o.matrix_world @ v.co for o in objs for v in o.data.vertices]
    lo = Vector((min(p.x for p in pts), min(p.y for p in pts), min(p.z for p in pts)))
    hi = Vector((max(p.x for p in pts), max(p.y for p in pts), max(p.z for p in pts)))
    return lo, hi


def recolour_textures(meshes):
    """Bronze body: every image used by the robo's own materials becomes luminance x BRONZE (alpha kept)."""
    images = set()
    for o in meshes:
        for slot in o.material_slots:
            if slot.material and slot.material.node_tree:
                images |= {n.image for n in slot.material.node_tree.nodes if n.type == 'TEX_IMAGE' and n.image}
    for img in images:
        px = np.array(img.pixels[:], dtype=np.float32).reshape(-1, 4)
        lum = px[:, :3] @ np.array([0.299, 0.587, 0.114], dtype=np.float32)
        px[:, :3] = np.clip((0.25 + 1.05 * lum)[:, None] * BRONZE, 0, 1)
        img.pixels[:] = px.ravel()
        img.pack()   # the glTF exporter writes the packed (edited) pixels
        print(f'legionnaire: recoloured image {img.name} ({img.size[0]}x{img.size[1]}) to bronze')
    return len(images)


def main():
    src, dst, roles_path = common.args(3, __doc__)
    roles = json.load(open(roles_path, encoding='utf-8'))['C']
    arm = common.load(src)
    meshes = common.model_meshes()
    bones = arm.data.bones
    pos = lambda j: arm.matrix_world @ bones[f'JOBJ_{j}'].head_local

    def own_meshes(joint, stop=()):
        """Meshes on `joint` and its child joints (not below `stop`): the exporter names them Joint_<j>_Object_<d>."""
        js, todo = set(), [bones[f'JOBJ_{joint}']]
        while todo:
            bn = todo.pop()
            j = int(bn.name.split('_')[1])
            if j in stop:
                continue
            js.add(j)
            todo += list(bn.children)
        return [o for o in meshes if o.name.startswith('Joint_') and int(o.name.split('_')[1]) in js]

    lo, hi = world_bbox(meshes)
    height = hi.z - lo.z
    up = Vector((0, 0, 1))
    # forward: hips -> the two toes, snapped to the nearest horizontal axis (the bind pose's feet stand forward)
    toes = (pos(roles['leg_a_foot']) + pos(roles['leg_b_foot'])) / 2 - pos(roles['lower_body'])
    fwd = Vector((1 if toes.x > 0 else -1, 0, 0)) if abs(toes.x) > abs(toes.y) else Vector((0, 1 if toes.y > 0 else -1, 0))
    left = up.cross(fwd)
    centre = (lo + hi) / 2
    a, b = roles['arm_a_fore'], roles['arm_b_fore']
    fore_role = 'arm_a' if (pos(a) - centre).dot(left) > (pos(b) - centre).dot(left) else 'arm_b'
    fore, hand = roles[fore_role + '_fore'], roles[fore_role + '_hand']
    print(f'legionnaire: model box {tuple(round(x, 3) for x in lo)} .. {tuple(round(x, 3) for x in hi)}, height {height:.3f}, '
          f'forward {tuple(fwd)}, left forearm = {fore_role}_fore JOBJ_{fore} (hand JOBJ_{hand}), head JOBJ_{roles["head"]}')

    red, gold = material('Legion_Red', RED), material('Legion_Gold', GOLD)

    # crest: a tall red ridge front-to-back on top of the head, on a gold stand
    head = roles['head']
    head_objs = own_meshes(head)
    hlo, hhi = world_bbox(head_objs) if head_objs else (pos(head) - Vector((0.1,) * 3) * height, pos(head) + Vector((0.1,) * 3) * height)
    hsize = hhi - hlo
    hc = (hlo + hhi) / 2
    hd = max(abs(hsize.dot(fwd)), 0.12 * height)    # head depth along forward
    hw = max(abs(hsize.dot(left)), 0.12 * height)   # head width
    top = hhi.z
    crest_h = 0.30 * height
    box('Legion_CrestStand', arm, f'JOBJ_{head}', gold, Vector((hc.x, hc.y, top + 0.02 * height)),
        (fwd, left, up), (hd * 1.0, hw * 0.30, 0.06 * height))
    box('Legion_Crest', arm, f'JOBJ_{head}', red, Vector((hc.x, hc.y, top + 0.04 * height + crest_h / 2)) - fwd * hd * 0.15,
        (fwd, left, up), (hd * 1.6, hw * 0.16, crest_h))

    # scutum: a big red rectangle with a gold rim, spine and boss in front of the left forearm, long side up
    flo, fhi = world_bbox(own_meshes(fore, stop={hand}))
    fext = fhi - flo
    across = Vector((1, 0, 0)) if abs(fext.x) >= abs(fext.y) else Vector((0, 1, 0))   # the forearm's long horizontal axis
    sw, sh, st = 0.30 * height, 0.50 * height, 0.025 * height
    sc = (flo + fhi) / 2 + fwd * (abs(fext.dot(fwd)) / 2 + st)
    sfwd = up.cross(across).normalized()
    if sfwd.dot(fwd) < 0:
        sfwd = -sfwd
    bone = f'JOBJ_{fore}'
    box('Legion_Scutum', arm, bone, red, sc, (across, sfwd, up), (sw, st, sh))
    r = 0.025 * height   # rim width
    front = sc + sfwd * st * 0.6
    for n, off, ax_size in (('Top', up * (sh / 2 - r / 2), (sw, st, r)), ('Bottom', -up * (sh / 2 - r / 2), (sw, st, r)),
                            ('Left', across * (sw / 2 - r / 2), (r, st, sh)), ('Right', -across * (sw / 2 - r / 2), (r, st, sh))):
        box(f'Legion_ScutumRim{n}', arm, bone, gold, front + off, (across, sfwd, up), ax_size)
    box('Legion_ScutumSpine', arm, bone, gold, front, (across, sfwd, up), (r * 0.8, st, sh * 0.9))
    box('Legion_ScutumBoss', arm, bone, gold, sc + sfwd * st * 1.4, (across, sfwd, up), (sw * 0.30, st * 1.6, sw * 0.30))

    n = recolour_textures(meshes)
    if not n:
        print('legionnaire: warning: no textures found to recolour')
    common.save(dst)


if __name__ == '__main__':
    main()
