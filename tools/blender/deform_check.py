"""deform_check.py: where a skinned robo body stretches or clips in a motion (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/deform_check.py -- <body.glb> <motion.glb> <roles json> <out prefix>
      [--step=2] [--render=1] [--stretch=1.6] [--squash=0.6] [--depth=0.02]

Per frame (every --step frames of the motion's action):
- stretch: each triangle's posed edge lengths against its rest lengths; a triangle is STRETCHED when an edge grows
  past --stretch x or shrinks below --squash x. Counted per region: the role of the joint that moves the triangle
  most (neck / chest / collar / shoulders / face ... are the regions where two joints share the weight).
- clipping: a vertex of one limb (arm a / arm b / leg a / leg b) deeper than --depth behind the surface of another
  limb or of the torso (nearest point on that group's triangles, behind its normal).
Writes <out>.json (every frame) and <out>.txt (summary); with --render=1 also <out>_worst.png: the worst frame from
front, back, left, right, top and bottom, stretch as colour (white ok -> yellow -> red stretched; blue squashed;
magenta = clipping vertices).

Why: the owner (Nov 12): "look around every angle ... for clipping and for stretching, primarily around high contour
areas neck chest collarbone face shoulders sides hips waist legs".
"""
import json
import math
import os
import sys

import bpy
import bmesh
import numpy as np
from mathutils import Matrix, Vector
from mathutils.bvhtree import BVHTree

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
BODY, MOTION, ROLES, OUT = [x for x in a if not x.startswith('--')][:4]
OUT = os.path.abspath(OUT)
STEP = int(opts.get('step') or 2)
STRETCH = float(opts.get('stretch') or 1.6)
SQUASH = float(opts.get('squash') or 0.6)
DEPTH = float(opts.get('depth') or 0.02)
roles = json.load(open(ROLES, encoding='utf-8'))['C']
role_of = {f'JOBJ_{v}': k for k, v in roles.items()}

bpy.ops.wm.read_factory_settings(use_empty=True)
bpy.context.scene.render.fps = 30     # the motion glbs are 30 fps: Blender frame = game frame (default 24 scaled them 0.8)
sc = bpy.context.scene
bpy.ops.import_scene.gltf(filepath=MOTION)
act = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0].animation_data.action
for o in list(bpy.data.objects):
    bpy.data.objects.remove(o, do_unlink=True)
bpy.ops.import_scene.gltf(filepath=BODY)
arm = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0]
meshes = [o for o in bpy.data.objects if o.type == 'MESH' and 'Icosphere' not in o.name]
for o in [o for o in bpy.data.objects if 'Icosphere' in o.name]:
    bpy.data.objects.remove(o, do_unlink=True)

# one merged index space over every body mesh
vinfo = []      # (object index, dominant joint, strongest weight)
tris = []
for oi, o in enumerate(meshes):
    base = len(vinfo)
    gname = {g.index: g.name for g in o.vertex_groups}
    for v in o.data.vertices:
        if v.groups:
            g = max(v.groups, key=lambda x: x.weight)
            vinfo.append((oi, gname[g.group], g.weight))
        else:
            vinfo.append((oi, '', 1.0))
    o.data.calc_loop_triangles()
    for t in o.data.loop_triangles:
        tris.append(tuple(base + i for i in t.vertices))
tris = np.array(tris, dtype=np.int64)


def region(vi):
    j, w = vinfo[vi][1], vinfo[vi][2]
    r = role_of.get(j)
    if r is None:
        # a joint below a role joint (fingers, toes ...): the nearest role ancestor
        b = arm.data.bones.get(j)
        while b is not None and role_of.get(b.name) is None:
            b = b.parent
        r = role_of.get(b.name, 'other') if b is not None else 'other'
    if w < 0.7:
        r += '+blend'
    return r


vreg = [region(i) for i in range(len(vinfo))]
GROUP = {}
for i, r in enumerate(vreg):
    base = r.replace('+blend', '')
    if base.startswith('arm_a'):
        GROUP[i] = 'arm_a'
    elif base.startswith('arm_b'):
        GROUP[i] = 'arm_b'
    elif base.startswith('leg_a'):
        GROUP[i] = 'leg_a'
    elif base.startswith('leg_b'):
        GROUP[i] = 'leg_b'
    elif base in ('head',):
        GROUP[i] = 'head'
    else:
        GROUP[i] = 'torso'
RAYS = [Vector((1, 0.013, 0.007)).normalized(), Vector((-0.011, 1, 0.017)).normalized(),
        Vector((0.009, -0.014, 1)).normalized()]
PAIRS = [('arm_a', 'leg_a'), ('arm_a', 'leg_b'), ('arm_a', 'torso'), ('arm_a', 'head'), ('arm_b', 'leg_a'),
         ('arm_b', 'leg_b'), ('arm_b', 'torso'), ('arm_b', 'head'), ('leg_a', 'leg_b'), ('arm_a', 'arm_b')]


def positions():
    dg = bpy.context.evaluated_depsgraph_get()
    out = []
    for o in meshes:
        me = o.evaluated_get(dg).to_mesh()
        mw = o.matrix_world
        out += [tuple(mw @ v.co) for v in me.vertices]
        o.evaluated_get(dg).to_mesh_clear()
    return np.array(out)


def edge_lengths(P):
    A, B, Cc = P[tris[:, 0]], P[tris[:, 1]], P[tris[:, 2]]
    return np.stack([np.linalg.norm(B - A, axis=1), np.linalg.norm(Cc - B, axis=1), np.linalg.norm(A - Cc, axis=1)], 1)


arm.animation_data_create()
arm.animation_data.action = None
for pb in arm.pose.bones:
    pb.matrix_basis = Matrix.Identity(4)
bpy.context.view_layer.update()
P0 = positions()
L0 = np.maximum(edge_lengths(P0), 1e-6)
VALID = L0.min(1) > 0.003         # slivers (a rest edge under 3 mm) make meaningless ratios
arm.animation_data.action = act
try:
    arm.animation_data.action_slot = act.slots[0]
except (AttributeError, IndexError):
    pass
f0, f1 = (int(x) for x in act.frame_range)
tri_reg = [vreg[t[np.argmin([vinfo[i][2] for i in t])]] for t in tris]     # the triangle's most shared corner


def check(frame):
    sc.frame_set(frame)
    bpy.context.view_layer.update()
    P = positions()
    r = edge_lengths(P) / L0
    mx, mn = r.max(1), r.min(1)
    bad_s = (mx > STRETCH) & VALID
    bad_q = (mn < SQUASH) & VALID
    per = {}
    for i in np.nonzero(bad_s | bad_q)[0]:
        k = tri_reg[i]
        d = per.setdefault(k, [0, 0, 1.0])
        if bad_s[i]:
            d[0] += 1
        if bad_q[i]:
            d[1] += 1
        d[2] = max(d[2], float(mx[i]), float(1 / max(mn[i], 1e-3)))
    # clipping
    clip = {}
    clip_v = set()
    creg = {}
    by_group = {}
    for ti, t in enumerate(tris):
        gs = {GROUP[i] for i in t}
        if len(gs) == 1:
            by_group.setdefault(gs.pop(), []).append(ti)
    trees = {}
    for g, tl in by_group.items():
        trees[g] = BVHTree.FromPolygons([tuple(p) for p in P], [tuple(tris[i]) for i in tl], all_triangles=True)
    for ga, gb in PAIRS:
        if ga not in trees or gb not in trees:
            continue
        n = 0
        worst = 0.0
        for vi in [i for i in range(len(P)) if GROUP[i] == ga]:
            pv = Vector(P[vi])
            loc, nor, idx, dist = trees[gb].find_nearest(pv, 0.08)
            if loc is None or dist < DEPTH:
                continue
            # inside the other group's volume: an odd number of crossings along a ray, in at least 2 of 3 directions
            # (the AI armour is many open plates, so "behind the nearest normal" alone counted plates layered over a
            # limb as clipping)
            odd = 0
            for dv in RAYS:
                hits, o_ = 0, pv.copy()
                for _ in range(16):
                    h = trees[gb].ray_cast(o_, dv, 0.6)
                    if h[0] is None:
                        break
                    hits += 1
                    o_ = h[0] + dv * 1e-4
                odd += hits % 2
            if odd >= 2:
                n += 1
                worst = max(worst, dist)
                clip_v.add(vi)
                creg[vreg[vi].replace('+blend', '')] = creg.get(vreg[vi].replace('+blend', ''), 0) + 1
        if n:
            clip[f'{ga} in {gb}'] = [n, round(worst, 3)]
    # the ground (Nov 12, owner: "look at the ankles"): the lowest body point; a standing frame's lowest point is the
    # sole on the floor (z of frame 0 in a standing motion is the reference, set by the caller as GROUND_Z)
    low_z = float(P[:, 2].min())
    low_part = vreg[int(P[:, 2].argmin())]
    foot = [i for i in range(len(P)) if vreg[i].replace('+blend', '') in ('leg_a_foot', 'leg_b_foot', 'leg_a_ankle', 'leg_b_ankle')]
    feet_z = {s_: float(min(P[i, 2] for i in foot if vreg[i].startswith(f'leg_{s_}'))) for s_ in 'ab' if any(vreg[i].startswith(f'leg_{s_}') for i in foot)}
    score = sum(v[0] + v[1] for v in per.values()) + sum(v[0] for v in clip.values()) * 3
    return {'frame': frame, 'stretch': {k: [v[0], v[1], round(v[2], 2)] for k, v in per.items()},
            'clip': clip, 'clip_regions': creg, 'score': score, 'low_z': round(low_z, 3), 'low_part': low_part,
            'feet_z': {k_: round(z_, 3) for k_, z_ in feet_z.items()}}, mx, mn, clip_v


results = []
worst = None
for fr in range(f0, f1 + 1, STEP):
    res, mx, mn, cv = check(fr)
    results.append(res)
    if worst is None or res['score'] > worst[0]['score']:
        worst = (res, mx, mn, cv)
json.dump(results, open(OUT + '.json', 'w'), indent=0)

# summary: per region, the frames and the worst ratio
tot = {}
ctot = {}
for res in results:
    for k, (ns, nq, w) in res['stretch'].items():
        d = tot.setdefault(k, [0, 0, 1.0, []])
        d[0] += ns
        d[1] += nq
        d[2] = max(d[2], w)
        d[3].append(res['frame'])
    for k, (n, dp) in res['clip'].items():
        d = ctot.setdefault(k, [0, 0.0, []])
        d[0] += n
        d[1] = max(d[1], dp)
        d[2].append(res['frame'])
lines = [f'deform_check {os.path.basename(MOTION)} frames {f0}..{f1} step {STEP}; stretch > {STRETCH} / squash < {SQUASH}; '
         f'clip depth > {DEPTH}', 'STRETCH by region (stretched tris, squashed tris, worst ratio, frames):']
for k, (ns, nq, w, frs) in sorted(tot.items(), key=lambda kv: -(kv[1][0] + kv[1][1])):
    lines.append(f'  {k:24s} {ns:6d} {nq:6d}  x{w:.2f}  frames {frs[0]}..{frs[-1]} ({len(frs)})')
lines.append('CLIPPING (vertices inside another limb, deepest m, frames):')
for k, (n, dp, frs) in sorted(ctot.items(), key=lambda kv: -kv[1][0]):
    lines.append(f'  {k:24s} {n:6d}  {dp:.3f}  frames {frs[0]}..{frs[-1]} ({len(frs)})')
crt = {}
for res in results:
    for k, n in res.get('clip_regions', {}).items():
        crt[k] = crt.get(k, 0) + n
lz = [r['low_z'] for r in results]
lines.append(f'GROUND: lowest body point z from {min(lz):.3f} to {max(lz):.3f} (frames {results[lz.index(min(lz))]["frame"]} / '
             f'{results[lz.index(max(lz))]["frame"]}); feet per frame in the .json')
lines.append('CLIPPING by part: ' + ', '.join(f'{k} {n}' for k, n in sorted(crt.items(), key=lambda kv: -kv[1])))
lines.append(f'worst frame {worst[0]["frame"]} (score {worst[0]["score"]})')
open(OUT + '.txt', 'w').write('\n'.join(lines) + '\n')
print('\n'.join('deform_check: ' + l for l in lines))

if opts.get('render', '1') != '0':
    res, mx, mn, cv = worst
    sc.frame_set(res['frame'])
    bpy.context.view_layer.update()
    # bake the posed mesh with a colour per corner
    dg = bpy.context.evaluated_depsgraph_get()
    bm = bmesh.new()
    for o in meshes:
        me = bpy.data.meshes.new_from_object(o.evaluated_get(dg))
        me.transform(o.matrix_world)
        bm.from_mesh(me)
    bm.verts.ensure_lookup_table()
    bm.faces.ensure_lookup_table()
    me = bpy.data.meshes.new('heat')
    bm.to_mesh(me)
    ob = bpy.data.objects.new('heat', me)
    sc.collection.objects.link(ob)
    for o in meshes:
        o.hide_render = True
    col = me.color_attributes.new('heat', 'FLOAT_COLOR', 'CORNER')
    me.calc_loop_triangles()
    # faces keep the triangle order only when every source face was a triangle (HSD meshes are); colour per face
    nt = len(tris)
    for poly in me.polygons:
        i = poly.index if poly.index < nt else None
        if i is None:
            c = (0.8, 0.8, 0.8, 1)
        else:
            s, q = float(mx[i]), float(mn[i])
            if s > STRETCH:
                k = min(1.0, (s - STRETCH) / STRETCH)
                c = (1.0, 0.9 * (1 - k), 0.0, 1)
            elif s > 1.25:
                k = (s - 1.25) / (STRETCH - 1.25)
                c = (1.0, 1.0, 1.0 - k, 1)
            elif q < SQUASH:
                c = (0.15, 0.35, 1.0, 1)
            else:
                c = (0.85, 0.85, 0.85, 1)
            if any(v in cv for v in tris[i]):
                c = (1.0, 0.0, 1.0, 1)
        for li in poly.loop_indices:
            col.data[li].color = c
    sc.render.engine = 'BLENDER_WORKBENCH'
    sc.display.shading.light = 'STUDIO'
    sc.display.shading.color_type = 'VERTEX'
    sc.render.resolution_x = sc.render.resolution_y = 420
    sc.render.film_transparent = False
    pts = np.array([tuple(v.co) for v in me.vertices])
    c0 = Vector(tuple((pts.min(0) + pts.max(0)) / 2))
    size = float(np.max(pts.max(0) - pts.min(0)))
    cam = bpy.data.objects.new('cam', bpy.data.cameras.new('cam'))
    sc.collection.objects.link(cam)
    cam.data.type = 'ORTHO'
    cam.data.ortho_scale = size * 1.15
    sc.camera = cam
    # Blender axes (Z up); the robo faces -Y after the glTF import (glTF -Z)
    views = [('front', Vector((0, -1, 0.05))), ('back', Vector((0, 1, 0.05))), ('left', Vector((-1, 0, 0.05))),
             ('right', Vector((1, 0, 0.05))), ('top', Vector((0.001, -0.05, 1))), ('bottom', Vector((0.001, -0.05, -1))),
             ('front34', Vector((-0.8, -1, 0.35))), ('back34', Vector((0.8, 1, 0.35))), ('low', Vector((0.3, -1, -0.35)))]
    shots = []
    for name, d in views:
        d.normalize()
        cam.location = c0 + d * size * 3
        cam.rotation_euler = (c0 - cam.location).to_track_quat('-Z', 'Y').to_euler()
        sc.render.filepath = f'{OUT}_{name}.png'
        bpy.ops.render.render(write_still=True)
        shots.append(bpy.data.images.load(sc.render.filepath))
    W, H = shots[0].size
    rows = (len(shots) + 2) // 3
    o2 = bpy.data.images.new('o', W * 3, H * rows)
    buf = [0.0] * (W * 3 * H * rows * 4)
    for k, im in enumerate(shots):
        px = list(im.pixels)
        cx, cy = k % 3, rows - 1 - k // 3
        for y in range(H):
            s0 = ((cy * H + y) * W * 3 + cx * W) * 4
            buf[s0:s0 + W * 4] = px[y * W * 4:(y + 1) * W * 4]
    o2.pixels = buf
    o2.filepath_raw = OUT + '_worst.png'
    o2.file_format = 'PNG'
    o2.save()
    for name, _ in views:
        try:
            os.remove(f'{OUT}_{name}.png')
        except OSError:
            pass
    print('deform_check: wrote', OUT + '_worst.png', 'frame', res['frame'])
