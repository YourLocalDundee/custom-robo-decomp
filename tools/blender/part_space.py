import bpy, sys, json, glob, statistics
sys.path.insert(0, r'D:\emulators\gameTesting\tools\blender')
from mathutils import Matrix, Vector, Quaternion
a = sys.argv[sys.argv.index('--') + 1:]
glb, caps, off, ji, hand_i, fore_i = a[0], a[1], a[2], int(a[3]), int(a[4]), int(a[5])
import struct
b = open(glb, 'rb').read(); n = struct.unpack_from('<I', b, 12)[0]; nodes = json.loads(b[20:20 + n])['nodes']
par = {c: i for i, nd in enumerate(nodes) for c in nd.get('children', [])}
def loc(nd):
    t = Vector(nd.get('translation', (0, 0, 0))); q = nd.get('rotation', (0, 0, 0, 1)); s = nd.get('scale', (1, 1, 1))
    return Matrix.Translation(t) @ Quaternion((q[3], q[0], q[1], q[2])).to_matrix().to_4x4() @ Matrix.Diagonal((s[0], s[1], s[2], 1))
def world(i): return (world(par[i]) if i in par else Matrix.Identity(4)) @ loc(nodes[i])
idx = [i for i, nd in enumerate(nodes) if nd.get('name') == f'JOBJ_{ji}'][0]
R = world(idx)
M4 = lambda v: Matrix((v[0:4], v[4:8], v[8:12], (0, 0, 0, 1)))
hs, fs = [], []
for f in sorted(glob.glob(caps)):
    d = json.load(open(f)); js = d.get('parts0', {}).get(off) or []
    if len(js) != int(a[6]): continue
    T = R @ M4(js[ji]).inverted()
    h_ = T @ M4(d["0"][hand_i]).translation; f_ = T @ M4(d["0"][fore_i]).translation; hs.append(h_); fs.append(f_); print("HF", f[-11:-5], [round(x, 2) for x in h_], [round(x, 2) for x in f_])
for name, L in (('wrist', hs), ('elbow', fs)):
    print('HS', name, len(L), [round(statistics.median(p[k] for p in L), 2) for k in range(3)],
          'spread', [round(statistics.pstdev(p[k] for p in L), 2) for k in range(3)])
