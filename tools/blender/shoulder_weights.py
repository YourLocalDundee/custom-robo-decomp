"""shoulder_weights.py: route the chest - upper arm blend through the shoulder joint (Nov 12, 2026).

  blender -b --factory-startup --python tools/blender/shoulder_weights.py -- <in.glb> <roles json> <out.glb> [--share=0.4]

make_motion.py's share_shoulders() turns the shoulder (clavicle) joint by --share of a big upper-arm swing. The body's
armpit / shoulder band was weighted chest <-> upper arm directly, so it gained nothing from that. Per point (vertices
at one position together) with chest weight wc and arm weight (shoulder ws + upper arm wu) on one side:
  t = (wu + share ws) / (wc + ws + wu)        how far the point followed the arm, 0 chest .. 1 upper arm
  t <= share:  chest (1 - t / share), shoulder t / share
  t >  share:  shoulder (1 - t) / (1 - share), upper arm (t - share) / (1 - share)
So with the shoulder at `share` of the swing the point still follows the arm by t, but along a two-step arc (chest ->
shoulder -> upper arm) instead of one straight blend, which is what collapses in linear skinning. Other weights (head,
hips, forearm) are kept; a point with both arms' weights is routed on its stronger side only.
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
K = float(opts.get('share') or 0.4)
roles = json.load(open(ROLES, encoding='utf-8'))['C']
rb = {f'JOBJ_{v}': k for k, v in roles.items() if v is not None}
arm = common.load(SRC)
ob = common.model_meshes()[0]
me = ob.data
for r in ('arm_a_shoulder', 'arm_b_shoulder', 'arm_a_upper', 'arm_b_upper', 'upper_body'):
    if f'JOBJ_{roles[r]}' not in ob.vertex_groups:
        ob.vertex_groups.new(name=f'JOBJ_{roles[r]}')
names = [g.name for g in ob.vertex_groups]
role_of = []
for n in names:
    b = arm.data.bones.get(n)
    while b is not None and b.name not in rb:
        b = b.parent
    role_of.append(rb[b.name] if b is not None else '')
role_of = np.array(role_of)
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
chest_cols = np.nonzero(role_of == 'upper_body')[0]
moved = 0
for side in 'ab':
    other = 'b' if side == 'a' else 'a'
    sh_cols = np.nonzero(role_of == f'arm_{side}_shoulder')[0]
    up_cols = np.nonzero(role_of == f'arm_{side}_upper')[0]
    o_cols = np.nonzero((role_of == f'arm_{other}_shoulder') | (role_of == f'arm_{other}_upper'))[0]
    j_sh = names.index(f'JOBJ_{roles[f"arm_{side}_shoulder"]}')
    for p in range(NP):
        wc = PW[p, chest_cols].sum()
        ws = PW[p, sh_cols].sum()
        wu = PW[p, up_cols].sum()
        if wc < 1e-3 or ws + wu < 1e-3 or PW[p, o_cols].sum() > ws + wu:
            continue
        T = wc + ws + wu
        t = (wu + K * ws) / T
        if t <= K:
            nc, ns, nu = 1 - t / K, t / K, 0.0
        else:
            nc, ns, nu = 0.0, (1 - t) / (1 - K), (t - K) / (1 - K)
        # keep each side's split among its own joints (chest: several joints carry the role)
        PW[p, chest_cols] *= (nc * T / wc)
        if wu > 1e-9:
            PW[p, up_cols] *= (nu * T / wu)
            PW[p, sh_cols] = 0.0
        else:
            PW[p, sh_cols] = 0.0
            nsu = nu * T
            PW[p, names.index(f'JOBJ_{roles[f"arm_{side}_upper"]}')] += nsu
        PW[p, j_sh] += ns * T
        moved += 1
PW /= np.maximum(PW.sum(1, keepdims=True), 1e-9)
# at most 4 joints
idx = np.argsort(-PW, 1)[:, 4:]
np.put_along_axis(PW, idx, 0.0, 1)
PW /= np.maximum(PW.sum(1, keepdims=True), 1e-9)
for j, nm in enumerate(names):
    ob.vertex_groups[nm].remove(list(range(nv)))
for v in me.vertices:
    row = PW[pid[v.index]]
    for j in np.nonzero(row > 1e-4)[0]:
        ob.vertex_groups[names[j]].add([v.index], float(row[j]), 'REPLACE')
print(f'shoulder_weights: {moved} points routed chest -> shoulder -> upper arm (share {K})')
common.save(DST)
