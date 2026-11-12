"""robo_preview.py: a whole robo the way the game assembles it, posed by a game motion, rendered from three sides.

usage: blender -b --factory-startup --python tools/blender/robo_preview.py -- <body.glb> <motion.glb> <parts dir> <out>
       [--frame=10] [--roles=<roles json>] [--mount=gun:x,y,z;bomb:...;pod:...;shin:...] [--scale=gun:s;...]

<body.glb>    the body (an HSDRawViewer export of a chara .dat, or atlas_ai_body.py's glb): JOBJ_n armature + meshes
<motion.glb>  a motion on the same JOBJ_n joints (tools/anim.py export, or crmotion_rig.cmd_gltf)
<parts dir>   HSDRawViewer --export folders weapon_gun_000/ weapon_bom_000/ weapon_pod_000/ weapon_leg_000/
              (rootN_joint.glb each); a missing folder leaves that part out
Writes <out>.png (front, three-quarter, side).

Why (Nov 11, 2026): the first in-game samurai (Customize) came apart in ways the T-pose previews hid: the skin
sheared under the game's poses, and the parts were the wrong size and place. This puts together what the game draws
in battle (docs/formats/model_entry.md "Part files"): gun root 0, bomb root 1, pod roots 0 + 1, leg roots 4 / 1 on
leg_b shin / ankle and 5 / 3 on leg_a shin / ankle; each root placed like fn_80039FAC: the role joint's posed matrix,
translated by the body's mount offset (cr_g_BodyData; only the shins take the leg offset), scaled by 0.1 and by the
body's part scale. The part roots are shown in their rest pose (the game sets each part animation's frame from a
state value; frame 0 is not applied here). All the matrix work is in glTF space (Y up), so Blender's bone axes do not
enter: posed joint = (pose delta of the skinning) x (the joint's rest world matrix from the glb's nodes).
"""
import json
import math
import struct
import sys

import bpy
import numpy as np
from mathutils import Matrix, Quaternion, Vector

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
BODY, MOTION, PARTS, OUT = [x for x in a if not x.startswith('--')][:4]
FRAME = int(opts.get('frame') or 10)
ROLES_ALL = json.load(open(opts['roles'], encoding='utf-8'))['C'] if opts.get('roles') else None
ROLES = ROLES_ALL if ROLES_ALL else {
    'upper_body': 28, 'arm_a_fore': 39, 'arm_b_fore': 49, 'arm_a_hand': 41, 'arm_b_hand': 51, 'leg_a_shin': 10, 'leg_a_ankle': 13,
    'leg_b_shin': 20, 'leg_b_ankle': 23}
# Ray 01's mounts (docs/robo_tables.md "Part mounts per body"): offsets before the 0.1 scale, in the role joint's space


def vec_opt(s, default):
    out = dict(default)
    for item in (s or '').split(';'):
        if item:
            k, _, v = item.partition(':')
            out[k] = tuple(float(x) for x in v.split(','))
    return out


MOUNT = vec_opt(opts.get('mount'), {'gun': (-0.02, 0, -0.01), 'bomb': (-0.02, 0, 0.01), 'pod': (-0.025, -0.015, 0),
                                    'shin': (0.05, 0.03, 0)})
SCALE = {k: v[0] for k, v in vec_opt(opts.get('scale'), {'gun': (1,), 'bomb': (1,), 'pod': (1,), 'leg': (1,)}).items()}
# (part folder, root, role, mount key, scale key)
# --form0: the Customize screen's set (FUN_801076D8 puts every part in form 0): bomb root 0, legs roots 0 + 2 on the
# shins (docs/formats/model_entry.md "Two forms per part")
FORM0 = bool(opts.get('form0'))
BATTLE = [('weapon_gun_000', 0, 'arm_b_fore', 'gun', 'gun'), ('weapon_bom_000', 0 if FORM0 else 1, 'arm_a_fore', 'bomb', 'bomb'),
          ('weapon_pod_000', 0, 'upper_body', 'pod', 'pod'), ('weapon_pod_000', 1, 'upper_body', 'pod', 'pod'),
          ('weapon_leg_000', 0 if FORM0 else 4, 'leg_b_shin', 'shin', 'leg'), ('weapon_leg_000', 1, 'leg_b_ankle', None, 'leg'),
          ('weapon_leg_000', 2 if FORM0 else 5, 'leg_a_shin', 'shin', 'leg'), ('weapon_leg_000', 3, 'leg_a_ankle', None, 'leg')]
# --gameparts=<capture mats json>[:robo]: the part roots in BATTLE order are the battle robo's JObj pointers
# +0x944 gun, +0x948 bomb, +0x94C / +0x950 pods, +0x954 / +0x958 / +0x95C / +0x960 legs (fn_80039FAC); the capture
# (wincap.py) stores each root's joints depth-first = JOBJ_n
GAMEPARTS = None
if opts.get('gameparts'):
    _gp, _, _gr = opts['gameparts'].rpartition(':') if opts['gameparts'].count(':') > 1 else (opts['gameparts'], '', '0')
    _d = json.load(open(_gp))[f'parts{_gr or "0"}']
    GAMEPARTS = {i: _d.get(hex(0x944 + 4 * i)) for i in range(8)}
C = Matrix.Rotation(math.radians(90), 4, 'X')          # glTF (Y up) -> Blender (Z up), as the glTF importer does
Ci = C.inverted()


def gltf_nodes(path):
    b = open(path, 'rb').read()
    n = struct.unpack_from('<I', b, 12)[0]
    return json.loads(b[20:20 + n])['nodes']


def rest_world(path):
    """JOBJ_n -> the joint's rest world matrix in glTF space, from the glb's node TRS"""
    nodes = gltf_nodes(path)
    parent = {c: i for i, nd in enumerate(nodes) for c in nd.get('children', [])}

    def local(nd):
        if 'matrix' in nd:
            m = nd['matrix']
            return Matrix([m[0:4], m[4:8], m[8:12], m[12:16]]).transposed()
        t = Vector(nd.get('translation', (0, 0, 0)))
        q = nd.get('rotation', (0, 0, 0, 1))
        s = nd.get('scale', (1, 1, 1))
        return (Matrix.Translation(t) @ Quaternion((q[3], q[0], q[1], q[2])).to_matrix().to_4x4()
                @ Matrix.Diagonal((s[0], s[1], s[2], 1)))
    memo = {}

    def world(i):
        if i not in memo:
            memo[i] = (world(parent[i]) if i in parent else Matrix.Identity(4)) @ local(nodes[i])
        return memo[i]
    return {nd['name']: world(i) for i, nd in enumerate(nodes) if nd.get('name', '').startswith('JOBJ_')}


bpy.ops.wm.read_factory_settings(use_empty=True)
sc = bpy.context.scene
# the motion: its action, put on the body's armature
bpy.ops.import_scene.gltf(filepath=MOTION)
act = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0].animation_data.action
for o in list(bpy.data.objects):
    bpy.data.objects.remove(o, do_unlink=True)
bpy.ops.import_scene.gltf(filepath=BODY)
arm = [o for o in bpy.data.objects if o.type == 'ARMATURE'][0]
arm.animation_data_create()
arm.animation_data.action = act
try:
    arm.animation_data.action_slot = act.slots[0]
except (AttributeError, IndexError):
    pass
sc.frame_set(FRAME)
bpy.context.view_layer.update()
rest = rest_world(BODY)
# --gamemats=<json>[:robo] (Nov 11, 2026): pose the body from the bone matrices the game computed (battle robo +0x1434,
# 20 x 3x4, read from a running Dolphin): role joint i gets matrix i+1 (fn_80039FAC). This is the exact pose of the
# screen being looked at (Customize plays its own motion), not a motion file's
if opts.get('gamemats'):
    gpath, _, gr = opts['gamemats'].rpartition(':') if opts['gamemats'].count(':') > 1 else (opts['gamemats'], '', '0')
    mats = json.load(open(gpath))[gr or '0']
    order = list(ROLES_ALL.keys())
    arm.animation_data.action = None
    for pb in arm.pose.bones:
        pb.matrix_basis = Matrix.Identity(4)
    bpy.context.view_layer.update()
    by_joint = {f'JOBJ_{ROLES_ALL[name]}': mats[i + 1] for i, name in enumerate(order)}

    def depth(b):
        n = 0
        while b.parent:
            n, b = n + 1, b.parent
        return n
    for b in sorted(arm.data.bones, key=depth):
        if b.name in by_joint:
            v = by_joint[b.name]
            M = Matrix((v[0:4], v[4:8], v[8:12], (0, 0, 0, 1)))
            pb = arm.pose.bones[b.name]
            pb.matrix = C @ M @ rest[b.name].inverted() @ Ci @ b.matrix_local
            bpy.context.view_layer.update()
    print('robo_preview: posed from the game matrices', gpath, 'robo', gr or '0')


def posed(jname):
    pb = arm.pose.bones[jname]
    d_bl = arm.matrix_world @ pb.matrix @ pb.bone.matrix_local.inverted() @ arm.matrix_world.inverted()
    return (Ci @ d_bl @ C) @ rest[jname]


# the body, posed: baked into a static copy so the parts can be added beside it
dg = bpy.context.evaluated_depsgraph_get()
body_objs = [o for o in bpy.data.objects if o.type == 'MESH' and 'Icosphere' not in o.name]
for o in [o for o in bpy.data.objects if 'Icosphere' in o.name]:
    bpy.data.objects.remove(o, do_unlink=True)
placed = []
place_of = {}
role_of = {}
dom_joint = []          # per vertex of the merged posed body (same order as body_bm below): its strongest joint
for o in body_objs:
    gname = {g.index: g.name for g in o.vertex_groups}
    for v in o.data.vertices:
        dom_joint.append(gname[max(v.groups, key=lambda x: x.weight).group] if v.groups else '')
for o in body_objs:
    me = bpy.data.meshes.new_from_object(o.evaluated_get(dg))
    me.transform(o.matrix_world)
    ob = bpy.data.objects.new(o.name + '_posed', me)
    sc.collection.objects.link(ob)
    placed.append(ob)
for o in body_objs:
    o.hide_render = True
    o.hide_set(True)
# the parts
import os  # noqa: E402
used_roots = {}
for bi, (folder, root, role, mkey, skey) in enumerate(BATTLE):
    gj = GAMEPARTS.get(bi) if GAMEPARTS else None
    if gj:
        # the game swaps a part's root by state (Nov 12, 2026 capture: battle gun = root 1 (6 joints), pods = roots
        # 2 / 3 (4 joints) or 0 / 1 (13) while firing, Customize gun = root 0 (21)): pick the file root with the
        # captured joint count, the listed one when it fits
        fits = []
        for rr in range(8):
            pth = os.path.join(PARTS, folder, f'root{rr}_joint.glb')
            if os.path.exists(pth) and len(rest_world(pth)) == len(gj):
                fits.append(rr)
        if fits and root not in fits:
            # two roots of a pair (pods 2 / 3, legs 1 / 3) have the same count: keep the pair's order
            root = next((rr for rr in fits if rr not in used_roots.get(folder, [])), fits[0])
        used_roots.setdefault(folder, []).append(root)
    path = os.path.join(PARTS, folder, f'root{root}_joint.glb')
    if not os.path.exists(path):
        continue
    before = set(bpy.data.objects)
    bpy.ops.import_scene.gltf(filepath=path)
    new = [o for o in bpy.data.objects if o not in before]
    dg = bpy.context.evaluated_depsgraph_get()
    if gj:
        # --gameparts: every joint of this part where the game drew it (its world matrix, read from Dolphin), so the
        # part's own animation (gun opening, legs' thrusters) and the per-part turn are exactly the game's
        rw = rest_world(path)
        G = {f'JOBJ_{k}': Matrix((v[0:4], v[4:8], v[8:12], (0, 0, 0, 1))) for k, v in enumerate(gj)}
        for o in new:
            if o.type != 'MESH' or 'Icosphere' in o.name:
                continue
            me = bpy.data.meshes.new_from_object(o.evaluated_get(dg))
            me.transform(o.matrix_world)
            gname = {g_.index: g_.name for g_ in o.vertex_groups}
            for v, v0 in zip(me.vertices, o.data.vertices):
                p = Ci @ v.co
                acc, wsum = Vector((0, 0, 0)), 0.0
                for ge in v0.groups:
                    jn = gname.get(ge.group)
                    if jn in G and jn in rw and ge.weight > 0:
                        acc += ge.weight * (G[jn] @ rw[jn].inverted() @ p)
                        wsum += ge.weight
                if wsum > 0:
                    v.co = C @ (acc / wsum)
            ob = bpy.data.objects.new(f'{folder}_r{root}', me)
            sc.collection.objects.link(ob)
            placed.append(ob)
            # "part file space" for the shift / gripshift prints: the glb's own (rest) space of the mesh's main joint
            cnt = {}
            for v0 in o.data.vertices:
                if v0.groups:
                    jn = gname.get(max(v0.groups, key=lambda x: x.weight).group)
                    cnt[jn] = cnt.get(jn, 0) + 1
            jm = max(cnt, key=cnt.get) if cnt else 'JOBJ_0'
            place_of[ob.name] = C @ G[jm] @ rw[jm].inverted()
            print(f'robo_preview: {folder}_r{root} mesh joint {jm}; shifts below are in the glb rest space (Y up)')
            role_of[ob.name] = role
        print(f'robo_preview: {folder} root {root} placed from the game ({len(gj)} joints, file {len(rw)})')
        for o in new:
            bpy.data.objects.remove(o, do_unlink=True)
        continue
    role_m = posed(f'JOBJ_{ROLES[role]}')
    off = Vector(MOUNT[mkey]) if mkey else Vector((0, 0, 0))
    s = 0.1 * SCALE.get(skey, 1.0)
    place = C @ role_m @ Matrix.Translation(off) @ Matrix.Diagonal((s, s, s, 1)) @ Ci
    for o in new:
        if o.type == 'MESH' and 'Icosphere' not in o.name:
            me = bpy.data.meshes.new_from_object(o.evaluated_get(dg))
            me.transform(place @ o.matrix_world)
            ob = bpy.data.objects.new(f'{folder}_r{root}', me)
            sc.collection.objects.link(ob)
            placed.append(ob)
            place_of[ob.name] = place
            role_of[ob.name] = role
    for o in new:
        bpy.data.objects.remove(o, do_unlink=True)
# ---- attachment check (Nov 11, 2026, owner: "if he has something in his hand it needs to be held in his hand,
# not floating above or beside it; the pods not floating off his back"): each part's nearest distance to the posed
# body surface, and loose pieces of the body (connected pieces whose nearest distance to the rest is a gap)
from mathutils.bvhtree import BVHTree  # noqa: E402
import bmesh as _bm  # noqa: E402
GAP = float(opts.get('gap') or 0.02)
body_bm = _bm.new()
for ob in placed:
    if ob.name.endswith('_posed'):
        body_bm.from_mesh(ob.data)
body_bm.verts.ensure_lookup_table()
body_tree = BVHTree.FromBMesh(body_bm)
report = []
for ob in placed:
    if ob.name.endswith('_posed'):
        continue
    # the region the part must touch (owner, Nov 11: a gun in the hand, not beside it): the gun the right hand, the
    # bomb the left forearm or hand, the pods the chest / back joint, the leg packs the shin or below
    role = role_of.get(ob.name, '')
    region_role = {'arm_b_fore': 'arm_b_hand'}.get(role, role)
    want = {f'JOBJ_{ROLES[region_role]}'}
    if region_role != 'upper_body':
        todo = [arm.data.bones[f'JOBJ_{ROLES[region_role]}']]
        while todo:
            b_ = todo.pop()
            want.add(b_.name)
            todo += list(b_.children)
    idx = [i for i, j in enumerate(dom_joint) if j in want]
    from mathutils.kdtree import KDTree as _KD
    kd_r = _KD(max(1, len(idx)))
    for k_, i in enumerate(idx):
        kd_r.insert(body_bm.verts[i].co, k_)
    kd_r.balance()
    best, bv, bp = 99.0, None, None
    for v in ob.data.vertices:
        if not idx:
            break
        co_, _, d_ = kd_r.find(v.co)
        hit = (co_, None, None, d_)
        if hit[0] is not None and hit[3] < best:
            best, bv, bp = hit[3], v.co.copy(), hit[0].copy()
    # a held part (gun in the right hand, bomb on the left forearm): the hand / forearm must sit ON it, at its grip, not
    # touch its edge. Grip = 30% along the part's long axis from the end nearer the elbow; the check is the distance
    # from the region's centre to the grip, and the shift that puts the grip there
    if role in ('arm_b_fore', 'arm_a_fore') and idx:
        pts_ = np.array([tuple(v.co) for v in ob.data.vertices])
        cen_ = pts_.mean(0)
        u_, s_, vt_ = np.linalg.svd(pts_ - cen_, full_matrices=False)
        ax_ = vt_[0]
        proj_ = (pts_ - cen_) @ ax_
        e1_, e2_ = cen_ + ax_ * proj_.min(), cen_ + ax_ * proj_.max()
        elbow_ = np.array(tuple(posed(f'JOBJ_{ROLES[role]}').translation))
        elbow_ = np.array(tuple(C @ Vector(elbow_)))
        if np.linalg.norm(e2_ - elbow_) < np.linalg.norm(e1_ - elbow_):
            e1_, e2_ = e2_, e1_
        grip_ = Vector(tuple(e1_ + 0.3 * (e2_ - e1_)))
        rc_ = sum((body_bm.verts[i].co for i in idx), Vector()) / len(idx)
        gd_ = (rc_ - grip_).length
        print(f'robo_preview: grip {ob.name} hand-to-grip {gd_:.3f} {"FLOATING" if gd_ > 0.03 else "ok"}')
        pinv_ = place_of[ob.name].inverted()
        dg_ = (pinv_ @ rc_) - (pinv_ @ grip_)
        print(f'robo_preview: gripshift {ob.name} {dg_.x:.3f},{dg_.y:.3f},{dg_.z:.3f} (part file space, grip into the hand)')
        if gd_ > 0.03:
            best = max(best, gd_)
    report.append((ob.name, best))
    if opts.get('debug') and idx:
        rc = sum((body_bm.verts[i].co for i in idx), Vector()) / len(idx)
        print(f'robo_preview: debug region {sorted(want)} {len(idx)} verts, centre {tuple(round(x, 3) for x in rc)}; '
              f'nearest part vertex {bv} to region point {bp}')
    if opts.get('debug'):
        cs = [v.co for v in ob.data.vertices]
        lo_ = Vector([min(c_[i] for c_ in cs) for i in range(3)]); hi_ = Vector([max(c_[i] for c_ in cs) for i in range(3)])
        print(f'robo_preview: debug {ob.name} verts {len(cs)} box {tuple(round(x, 3) for x in lo_)} {tuple(round(x, 3) for x in hi_)} nearest at {bp} from {bv}')
    if best > GAP and bv is not None and ob.name in place_of:
        pinv = place_of[ob.name].inverted()
        d = (pinv @ bp) - (pinv @ bv)
        print(f'robo_preview: shift {ob.name} {d.x:.3f},{d.y:.3f},{d.z:.3f} (part file space, to touch the body)')
    if ob.name in place_of:
        up = (place_of[ob.name].inverted().to_3x3() @ Vector((0, 0, 1))).normalized()
        print(f'robo_preview: up {ob.name} {up.x:.2f},{up.y:.2f},{up.z:.2f} (world up in part file space)')
        # inward: from the part's centre toward the body's centre line at the same height, without the up component
        pinv = place_of[ob.name].inverted()
        cen = sum((v.co for v in ob.data.vertices), Vector()) / max(1, len(ob.data.vertices))
        axis_pt = Vector((0.0, 0.0, cen.z))
        bodyc = sum((v.co for o2 in placed if o2.name.endswith('_posed') for v in o2.data.vertices), Vector())
        nb = sum(len(o2.data.vertices) for o2 in placed if o2.name.endswith('_posed'))
        axis_pt.x, axis_pt.y = bodyc.x / nb, bodyc.y / nb
        din = (pinv @ axis_pt) - (pinv @ cen)
        din -= up * din.dot(up)
        din.normalize()
        print(f'robo_preview: inward {ob.name} {din.x:.2f},{din.y:.2f},{din.z:.2f} (toward the body, part file space)')
# loose body pieces: connected components (by shared vertices) with no vertex within GAP of another component
comp = {}
seen = set()
pieces = []
for v0 in body_bm.verts:
    if v0.index in seen:
        continue
    stack, cur = [v0], []
    seen.add(v0.index)
    while stack:
        v = stack.pop()
        cur.append(v)
        for e in v.link_edges:
            o = e.other_vert(v)
            if o.index not in seen:
                seen.add(o.index)
                stack.append(o)
    pieces.append(cur)
loose = []
if len(pieces) > 1:
    from mathutils.kdtree import KDTree
    cid = {}
    for i, pc in enumerate(pieces):
        for v in pc:
            cid[v.index] = i
    kd = KDTree(len(body_bm.verts))
    for v in body_bm.verts:
        kd.insert(v.co, v.index)
    kd.balance()
    for i, pc in enumerate(pieces):
        if any(cid[j] != i for v in pc for (_, j, _) in kd.find_range(v.co, GAP)):
            continue
        # nothing of another piece within GAP: how far is the nearest (for the report)
        d = min(min((dd for (_, j, dd) in kd.find_n(v.co, 64) if cid[j] != i), default=1.0) for v in pc)
        loose.append((len(pc), d))
body_bm.free()
for name, d in report:
    flag = 'FLOATING' if d > GAP else 'ok'
    print(f'robo_preview: attach {name} gap {d:.3f} to {role_of.get(name, "")} region {flag}')
for n, d in loose:
    print(f'robo_preview: loose body piece {n} vertices, {d:.3f} from the rest FLOATING')
print(f'robo_preview: verdict {"FLOATING" if any(d > GAP for _, d in report) or loose else "ok"}')
pts = [ob.matrix_world @ v.co for ob in placed for v in ob.data.vertices]
lo = Vector([min(p[i] for p in pts) for i in range(3)])
hi = Vector([max(p[i] for p in pts) for i in range(3)])
c, size = (lo + hi) / 2, max(hi - lo)
if opts.get('centre'):
    c = Vector([float(x) for x in opts['centre'].split(',')])
print('robo_preview: box', tuple(round(x, 3) for x in lo), tuple(round(x, 3) for x in hi))
sc.render.engine = 'BLENDER_EEVEE'
w = bpy.data.worlds.new('w')
sc.world = w
w.use_nodes = True
w.node_tree.nodes['Background'].inputs[0].default_value = (0.35, 0.37, 0.4, 1)
sun = bpy.data.objects.new('sun', bpy.data.lights.new('sun', 'SUN'))
sc.collection.objects.link(sun)
sun.rotation_euler = (0.8, 0.2, 0.6)
sun.data.energy = 3
cam = bpy.data.objects.new('cam', bpy.data.cameras.new('cam'))
sc.collection.objects.link(cam)
cam.data.type = 'ORTHO'
cam.data.ortho_scale = float(opts.get('ortho') or size * 1.1)
sc.camera = cam
sc.render.resolution_x, sc.render.resolution_y = int(opts.get('w') or 520), int(opts.get('h') or 600)
shots = []
VIEWS = [Vector((0.8, 1.0, 0.3)), Vector((0, 1, 0.15)), Vector((1, 0, 0.1))][:int(opts.get('views') or 3)]
if opts.get('back'):
    VIEWS = [Vector((-d.x, -d.y, d.z)) for d in VIEWS]
if int(opts.get('views') or 3) == 3:
    VIEWS = [VIEWS[1], VIEWS[0], VIEWS[2]]
for i, d in enumerate(VIEWS):
    d.normalize()
    cam.location = c + d * size * 3
    cam.rotation_euler = (c - cam.location).to_track_quat('-Z', 'Y').to_euler()
    sc.render.filepath = f'{OUT}_{i}.png'
    bpy.ops.render.render(write_still=True)
    shots.append(bpy.data.images.load(sc.render.filepath))
W, H = shots[0].size
strip = bpy.data.images.new('strip', W * len(shots), H)
pp = [list(i.pixels) for i in shots]
px = []
for y in range(H):
    for p in pp:
        px += p[y * W * 4:(y + 1) * W * 4]
strip.pixels = px
strip.filepath_raw = OUT + '.png'
strip.file_format = 'PNG'
strip.save()
for i in range(len(shots)):
    os.remove(f'{OUT}_{i}.png')
print('robo_preview: wrote', OUT + '.png')
