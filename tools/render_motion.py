"""Render frames of a glTF animation (skinned, CPU) into one PNG contact sheet, to check motion exports by eye.

Usage: tools/pyenv/Scripts/python -I tools/render_motion.py <model.glb> <out.png> [--anim N] [--frames 0,10,20] [--view front|side|both]
Meshes with a skin are skinned (JOINTS_0 / WEIGHTS_0, inverse bind matrices); meshes without one follow their node.
"""
import json
import struct
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

CT = {5120: np.int8, 5121: np.uint8, 5122: np.int16, 5123: np.uint16, 5125: np.uint32, 5126: np.float32}
NC = {'SCALAR': 1, 'VEC2': 2, 'VEC3': 3, 'VEC4': 4, 'MAT4': 16}


def load(path):
    d = open(path, 'rb').read()
    jl = struct.unpack_from('<I', d, 12)[0]
    j = json.loads(d[20:20 + jl])
    o = 20 + jl
    b = d[o + 8:o + 8 + struct.unpack_from('<I', d, o)[0]] if o < len(d) else b''
    return j, b


def acc(j, b, i):
    a = j['accessors'][i]
    bv = j['bufferViews'][a['bufferView']]
    dt = CT[a['componentType']]
    n = NC[a['type']]
    off = bv.get('byteOffset', 0) + a.get('byteOffset', 0)
    stride = bv.get('byteStride', 0)
    size = np.dtype(dt).itemsize * n
    if stride and stride != size:
        raw = np.frombuffer(b, np.uint8, count=stride * (a['count'] - 1) + size, offset=off)
        rows = np.lib.stride_tricks.as_strided(raw, (a['count'], size), (stride, 1))
        out = np.frombuffer(rows.copy().tobytes(), dt).reshape(a['count'], n)
    else:
        out = np.frombuffer(b, dt, count=a['count'] * n, offset=off).reshape(a['count'], n)
    if a.get('normalized') and dt != np.float32:
        out = out / np.iinfo(dt).max
    return out.astype(np.float64)


def quat_mat(q):
    x, y, z, w = q
    return np.array([[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
                     [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
                     [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])


def local(node, ov):
    if 'matrix' in node and not ov:
        return np.array(node['matrix']).reshape(4, 4).T
    t = ov.get('translation', node.get('translation', [0, 0, 0]))
    r = ov.get('rotation', node.get('rotation', [0, 0, 0, 1]))
    s = ov.get('scale', node.get('scale', [1, 1, 1]))
    m = np.eye(4)
    m[:3, :3] = quat_mat(r) * np.array(s)[None, :]
    m[:3, 3] = t
    return m


def sample(j, b, anim, t):
    ov = {}
    if anim is None:
        return ov
    for ch in anim['channels']:
        s = anim['samplers'][ch['sampler']]
        ts = acc(j, b, s['input'])[:, 0]
        vs = acc(j, b, s['output'])
        k = int(np.clip(np.searchsorted(ts, t, side='right') - 1, 0, len(ts) - 1))
        if k + 1 < len(ts) and ts[k + 1] > ts[k]:
            f = (t - ts[k]) / (ts[k + 1] - ts[k])
            v = vs[k] * (1 - f) + vs[k + 1] * f
        else:
            v = vs[k]
        if ch['target']['path'] == 'rotation':
            v = v / np.linalg.norm(v)
        ov.setdefault(ch['target']['node'], {})[ch['target']['path']] = list(v)
    return ov


def worlds(j, ov):
    nodes = j['nodes']
    parent = {c: i for i, n in enumerate(nodes) for c in n.get('children', [])}
    W = {}

    def w(i):
        if i not in W:
            m = local(nodes[i], ov.get(i, {}))
            W[i] = w(parent[i]) @ m if i in parent else m
        return W[i]
    for i in range(len(nodes)):
        w(i)
    return W


def triangles(j, b, W):
    tris = []
    for ni, node in enumerate(j['nodes']):
        if 'mesh' not in node:
            continue
        skin = j['skins'][node['skin']] if 'skin' in node else None
        if skin:
            ibm = acc(j, b, skin['inverseBindMatrices']).reshape(-1, 4, 4).transpose(0, 2, 1)
            jm = np.array([W[jn] @ ibm[k] for k, jn in enumerate(skin['joints'])])
        for p in j['meshes'][node['mesh']]['primitives']:
            if p.get('mode', 4) != 4:
                continue
            v = acc(j, b, p['attributes']['POSITION'])
            vh = np.c_[v, np.ones(len(v))]
            if skin and 'JOINTS_0' in p['attributes']:
                J = acc(j, b, p['attributes']['JOINTS_0']).astype(int)
                Wt = acc(j, b, p['attributes']['WEIGHTS_0'])
                out = np.zeros((len(v), 4))
                for k in range(4):
                    out += Wt[:, k:k + 1] * np.einsum('nij,nj->ni', jm[J[:, k]], vh)
                v = out[:, :3]
            else:
                v = (W[ni] @ vh.T).T[:, :3]
            idx = acc(j, b, p['indices'])[:, 0].astype(int) if 'indices' in p else np.arange(len(v))
            tris.append(v[idx.reshape(-1, 3)])
    return np.concatenate(tris) if tris else np.zeros((0, 3, 3))


def main(argv):
    src, out = argv[1], argv[2]
    opts = dict(zip(argv[3::2], argv[4::2]))
    j, b = load(src)
    anims = j.get('animations', [])
    anim = anims[int(opts.get('--anim', len(anims) - 1))] if anims else None
    if anim:
        tmax = max(acc(j, b, s['input'])[-1, 0] for s in anim['samplers'])
    else:
        tmax = 0
    frames = [float(x) for x in opts['--frames'].split(',')] if '--frames' in opts else list(np.linspace(0, tmax * 30, 6))
    views = {'front': [(10, -90)], 'side': [(10, 0)], 'both': [(10, -90), (10, 0)]}[opts.get('--view', 'both')]
    all_tris = [triangles(j, b, worlds(j, sample(j, b, anim, f / 30.0))) for f in frames]
    pts = np.concatenate([t.reshape(-1, 3) for t in all_tris])
    lo, hi = pts.min(0), pts.max(0)
    c, r = (lo + hi) / 2, (hi - lo).max() / 2
    fig = plt.figure(figsize=(3 * len(frames), 3.2 * len(views)))
    light = np.array([0.3, -1.0, 0.6]); light /= np.linalg.norm(light)
    for vi, (elev, azim) in enumerate(views):
        for fi, (f, tris) in enumerate(zip(frames, all_tris)):
            ax = fig.add_subplot(len(views), len(frames), vi * len(frames) + fi + 1, projection='3d')
            n = np.cross(tris[:, 1] - tris[:, 0], tris[:, 2] - tris[:, 0])
            n /= np.linalg.norm(n, axis=1, keepdims=True) + 1e-9
            shade = np.clip(np.abs(n @ light), 0.15, 1)
            ax.add_collection3d(Poly3DCollection(tris[:, :, [0, 2, 1]], facecolors=plt.cm.Greys_r(shade * 0.85),
                                                 edgecolor='none'))
            ax.set_xlim(c[0] - r, c[0] + r); ax.set_ylim(c[2] - r, c[2] + r); ax.set_zlim(c[1] - r, c[1] + r)
            ax.view_init(elev, azim); ax.set_axis_off()
            ax.set_title('frame %g' % f, fontsize=8)
    fig.suptitle(anim.get('name', '') if anim else 'bind pose')
    plt.tight_layout()
    plt.savefig(out, dpi=80)
    print('wrote', out)


if __name__ == '__main__':
    main(sys.argv)
