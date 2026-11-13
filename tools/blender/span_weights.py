"""span_weights.py: no triangle may be pulled by joints that are not neighbours (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/span_weights.py -- <in.glb> <roles json> <out.glb> [--passes=8]

reweight.py --roles keeps each VERTEX on its strongest joint and that joint's neighbours. A triangle can still span
further: the thigh guards hang from the belt with their top corners on the hips and their bottom corners on the shin
(motion film, stance: the big red triangles at the inner thighs from frame 1), and the collar has triangles with one
corner on each shoulder. Per triangle (vertices at one position = one point, so UV seams stay closed):
- both sides: weights of arm_a and arm_b (or leg_a and leg_b) in one triangle -> the weaker side's weights on its
  corners go to the trunk joint (upper_body for arms, lower_body for legs)
- a skipped joint: along trunk -> limb (upper_body, lower_body, thigh, shin, ankle, foot / upper_body, [shoulder,]
  upper arm, forearm, hand) the triangle keeps its strongest joint and that joint's two neighbours; weights further
  along move to the nearer kept joint (hips + shin: the shin weights become thigh weights). The shoulder counts as
  one step with the upper arm (the chest - upper arm armpit blend is normal on Ray's clavicle joint)
Repeated until nothing changes (a corner is shared by several triangles). Only weights change.
"""
import json
import sys

import bpy
import numpy as np

sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
import common  # noqa: E402

a = sys.argv[sys.argv.index('--') + 1:]
opts = {k[2:]: v for k, _, v in (x.partition('=') for x in a if x.startswith('--'))}
SRC, ROLES, DST = [x for x in a if not x.startswith('--')][:3]
PASSES = int(opts.get('passes') or 8)
SIDES = opts.get('sides', '1') != '0'      # --sides=0: only the skipped-joint rule
roles = json.load(open(ROLES, encoding='utf-8'))['C']
rb = {f'JOBJ_{v}': k for k, v in roles.items() if v is not None}
arm = common.load(SRC)
ob = common.model_meshes()[0]
me = ob.data
names = [g.name for g in ob.vertex_groups]
role_of = []
for n in names:
    b = arm.data.bones.get(n)
    while b is not None and b.name not in rb:
        b = b.parent
    role_of.append(rb[b.name] if b is not None else '')
RN = sorted(set(role_of) | set(rb.values()))
ri = {r: i for i, r in enumerate(RN)}
role_joint = {}
for r in RN:
    jn = f'JOBJ_{roles[r]}' if r in roles and roles[r] is not None else None
    if jn and jn not in names:
        ob.vertex_groups.new(name=jn)
        names.append(jn)
        role_of.append(r)
    role_joint[r] = names.index(jn) if jn else None

nv = len(me.vertices)
W = np.zeros((nv, len(names)))
for v in me.vertices:
    for g in v.groups:
        W[v.index, g.group] = g.weight
key = {}
pid = np.zeros(nv, np.int64)
for v in me.vertices:
    pid[v.index] = key.setdefault(tuple(np.round(np.array(v.co) * 1e5).astype(np.int64)), len(key))
NP = len(key)
PW = np.zeros((NP, len(names)))
cnt = np.zeros(NP)
np.add.at(PW, pid, W)
np.add.at(cnt, pid, 1)
PW /= np.maximum(cnt, 1)[:, None]
jr = np.array([ri[r] for r in role_of])
tris = sorted({tuple(sorted({int(pid[i]) for i in p.vertices})) for p in me.polygons})
tris = [t for t in tris if len(t) == 3]


def chain(side, limb):
    if limb == 'arm':
        return ['lower_body', 'upper_body', f'arm_{side}_upper', f'arm_{side}_fore', f'arm_{side}_hand']
    return ['upper_body', 'lower_body', f'leg_{side}_thigh', f'leg_{side}_shin', f'leg_{side}_ankle', f'leg_{side}_foot']


ALIAS = {'arm_a_shoulder': 'arm_a_upper', 'arm_b_shoulder': 'arm_b_upper'}       # one step with the upper arm


def move(p, frm_cols, to_role):
    """point p: the weights of the joints in frm_cols go to to_role's joint"""
    amt = PW[p, frm_cols].sum()
    if amt <= 0:
        return 0
    PW[p, frm_cols] = 0.0
    PW[p, role_joint[to_role]] += amt
    return 1


def role_w(p):
    out = np.zeros(len(RN))
    np.add.at(out, jr, PW[p])
    return out


total = {'sides': 0, 'skip': 0}
for it in range(PASSES):
    changed = 0
    for t in tris:
        rw = [role_w(p) for p in t]
        tot = sum(rw)
        for limb, trunk in (('arm', 'upper_body'), ('leg', 'lower_body')):
            sa = sum(tot[ri[r]] for r in RN if r.startswith(f'{limb}_a'))
            sb = sum(tot[ri[r]] for r in RN if r.startswith(f'{limb}_b'))
            if SIDES and sa > 0.02 and sb > 0.02:
                weak = 'b' if sa >= sb else 'a'
                cols = [j for j, r in enumerate(role_of) if r.startswith(f'{limb}_{weak}')]
                for p in t:
                    c_ = move(p, cols, trunk)
                    changed += c_
                    total['sides'] += c_
        rw = [role_w(p) for p in t]
        tot = sum(rw)
        for side in 'ab':
            for limb in ('arm', 'leg'):
                ch = chain(side, limb)
                amt = [sum(tot[ri[r]] for r in RN if ALIAS.get(r, r) == c_) for c_ in ch]
                if sum(1 for x in amt if x > 0.02) < 2:
                    continue
                # only if this limb is in the triangle at all
                if sum(amt[2:]) <= 0.02:
                    continue
                d = int(np.argmax(amt))
                for k, c_ in enumerate(ch):
                    if amt[k] <= 0.02 or abs(k - d) <= 1:
                        continue
                    to = ch[d + 1] if k > d else ch[d - 1]
                    cols = [j for j, r in enumerate(role_of) if ALIAS.get(r, r) == c_]
                    for p in t:
                        c2 = move(p, cols, to)
                        changed += c2
                        total['skip'] += c2
    print(f'span_weights: pass {it + 1}: {changed} point weights moved', flush=True)
    if not changed:
        break
PW /= np.maximum(PW.sum(1, keepdims=True), 1e-9)
for j, nm in enumerate(names):
    ob.vertex_groups[nm].remove(list(range(nv)))
for v in me.vertices:
    row = PW[pid[v.index]]
    for j in np.nonzero(row > 1e-4)[0]:
        ob.vertex_groups[names[j]].add([v.index], float(row[j]), 'REPLACE')
print(f'span_weights: {total["sides"]} moves off the weaker side, {total["skip"]} moves off a skipped joint')
common.save(DST)
