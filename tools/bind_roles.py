"""bind_roles.py: a body glb's bind (rest world) matrices of the role joints, for make_motion.py's untwist (Nov 12, 2026).

  python -I tools/bind_roles.py <body.glb> <roles json> <out json>

glTF space (Y up, the game's model space), 4x4 row-major, one per role name. The rest pose of a rebound body
(tools/blender/rebind.py) is the bind the game skins with (its inverse bind matrices), so a motion's joint turns
"from the bind" are measured against these.
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import crmotion_rig as rig  # noqa: E402

body, roles_p, out = sys.argv[1:4]
j, _ = rig.read_glb(body)
nodes = j['nodes']
parent = {c: i for i, n in enumerate(nodes) for c in n.get('children', [])}
local = {i: rig.trs_matrix(n) for i, n in enumerate(nodes)}
memo = {}


def world(i):
    if i not in memo:
        memo[i] = rig.mm(world(parent[i]), local[i]) if i in parent else local[i]
    return memo[i]


byname = {n.get('name'): i for i, n in enumerate(nodes)}
roles = json.load(open(roles_p, encoding='utf-8'))['C']
res = {r: world(byname[f'JOBJ_{jn}']) for r, jn in roles.items() if jn is not None and f'JOBJ_{jn}' in byname}
json.dump(res, open(out, 'w'), indent=0)
print(f'bind_roles: {len(res)} role joints from {body}')
