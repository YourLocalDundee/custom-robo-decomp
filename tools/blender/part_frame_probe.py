"""part_frame_probe.py: measure what HSDRawViewer's import + export does to a part mesh, root by root.

  blender -b --factory-startup --python tools/blender/part_frame_probe.py -- make <stock root.glb> <out.glb> <points.json> --joint=<n>
  blender -b --factory-startup --python tools/blender/part_frame_probe.py -- solve <exported root.glb> <points.json> <frame.json>

make: a small marker mesh (12 distinct points, 4 triangles) in ai_part.py's frame (the stock root as drawn), placed
      around the stock part, rigid on JOBJ_<n>, written the way ai_part.py writes a part.
solve: after `HSDRawViewer --import ... --root N --replace --relayout` and `--export`, the markers as drawn; the map
      F (one of the 24 axis turns, a uniform scale and an offset) from where ai_part put them to where they are drawn,
      with its error. ai_part.py --frame_fix=<frame.json> writes its mesh as F^-1 x fitted.

Why (Nov 11, 2026): guessing the importer's behaviour from part boxes went wrong twice (the joint's full transform
made the parts huge; the root joint's turn fixed the pods and moved the gun 8 cm off the hand). This measures it.
"""
import itertools
import json
import os
import sys

import bpy
import numpy as np
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
a = [x for x in a if not x.startswith('--')]
MARK = np.array([(0, 0, 0), (1, 0, 0), (0, 2, 0), (0, 0, 3), (1, 2, 0), (0, 2.5, 3), (1.5, 0, 3), (2, 1, 1),
                 (-1, 0.3, 0.7), (0.4, -1.2, 0.9), (0.2, 0.8, -1.6), (-0.7, -0.6, -0.4)], float)


def drawn_points(path):
    common.load(path)
    dg = bpy.context.evaluated_depsgraph_get()
    return np.array([tuple(o.matrix_world @ v.co) for o in common.model_meshes()
                     for v in o.evaluated_get(dg).to_mesh().vertices])


if a[0] == 'make':
    stock, out, pts_json = a[1:4]
    joint = f"JOBJ_{int(opts['joint'])}"
    sp = drawn_points(stock)
    arm = [o for o in bpy.context.scene.objects if o.type == 'ARMATURE'][0]
    c = (sp.min(0) + sp.max(0)) / 2
    k = float(np.max(sp.max(0) - sp.min(0))) / 8
    pts = c + MARK * k
    for o in common.model_meshes():
        bpy.data.objects.remove(o, do_unlink=True)
    me = bpy.data.meshes.new('Probe')
    faces = [(0, 1, 2), (3, 4, 5), (6, 7, 8), (9, 10, 11)]
    me.from_pydata([tuple(p) for p in pts], [], faces)
    me.update()
    uv = me.uv_layers.new(name='UVMap')
    for i, l in enumerate(uv.data):
        l.uv = ((i % 3) / 3, (i // 3) / 4)
    ob = bpy.data.objects.new('Probe', me)
    bpy.context.scene.collection.objects.link(ob)
    img = bpy.data.images.new('probe_tex', 8, 8)
    img.pixels = [0.8, 0.2, 0.2, 1.0] * 64
    img.filepath_raw = os.path.splitext(out)[0] + '.png'
    img.file_format = 'PNG'
    img.save()
    mat = bpy.data.materials.new('Probe')
    mat.use_nodes = True
    nt = mat.node_tree
    tx = nt.nodes.new('ShaderNodeTexImage')
    tx.image = img
    nt.links.new(tx.outputs['Color'], nt.nodes['Principled BSDF'].inputs['Base Color'])
    me.materials.append(mat)
    ob.vertex_groups.new(name=joint).add(list(range(len(me.vertices))), 1.0, 'REPLACE')
    ob.parent = arm
    ob.matrix_parent_inverse = arm.matrix_world.inverted()
    mod = ob.modifiers.new('Armature', 'ARMATURE')
    mod.object = arm
    # the armature would move the markers in the scene, but ai_part writes its mesh the same way: what is written is
    # the mesh data (in this frame); the importer decides the rest
    common.save(out)
    json.dump({'points': pts.tolist()}, open(pts_json, 'w'))
    print('probe: wrote', out, 'markers around', np.round(c, 3).tolist(), 'size', round(k, 4))

elif a[0] == 'solve':
    exported, pts_json, frame_json = a[1:4]
    P = np.array(json.load(open(pts_json))['points'])
    Q = drawn_points(exported)
    best = None
    for perm in itertools.permutations(range(3)):
        for signs in itertools.product((1, -1), repeat=3):
            R = np.zeros((3, 3))
            for i in range(3):
                R[i, perm[i]] = signs[i]
            if np.linalg.det(R) < 0:
                continue
            Pc, Qc = P - P.mean(0), Q - Q.mean(0)
            s = np.sqrt((Qc ** 2).sum() / max((Pc ** 2).sum(), 1e-12))
            T = Q.mean(0) - s * (R @ P.mean(0))
            mapped = (s * (R @ P.T)).T + T
            err = max(np.min(np.linalg.norm(Q - m, axis=1)) for m in mapped)
            if best is None or err < best[0]:
                best = (err, R, s, T)
    err, R, s, T = best
    F = np.eye(4)
    F[:3, :3] = s * R
    F[:3, 3] = T
    json.dump({'F': F.tolist(), 'error': err, 'markers_in': len(P), 'points_out': len(Q)}, open(frame_json, 'w'))
    print(f'probe: F turn {R.astype(int).tolist()} scale {s:.4f} offset {np.round(T, 3).tolist()} '
          f'error {err:.5f} ({len(Q)} points back)')
