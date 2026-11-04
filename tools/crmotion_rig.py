"""Custom Robo (GC): pose a robo from a motion, as the game does, and export motions as glTF animations.

The game's per-frame chain (main.dol, USA), ported here function by function:
  fn_80056104   root: tracks 72-74 (degrees) -> rotation Rz.Ry.Rx; translation = tracks 69-71 + R (tracks 75-77);
                the root point = R (tracks 0-2) + translation
  fn_80054238   the 6 other IK points (tracks 9, 15, 24, 36, 48, 60; x, y, z) through the root matrix; the angles of
                the 16 rotation joints (degrees, joint 0 plus the root rotation)
  fn_80053DA8   one rotation matrix Rz.Ry.Rx per rotation joint (the "joint matrices" below, chara +0x11CC)
  cr_Chara_PoseLimbs 0x8005AEE8 with fn_8005AAA8 / fn_8005A668 (two-bone IK on the game's default skeleton, table
                0x801F9B00) and cr_Chara_PoseLimbA / B (the limbs rebuilt with the robo's own lengths, model block A)
                -> 20 body matrices (chara +0x154C): matrix k (k = 1..19) is the WORLD matrix of the model joint with
                role k - 1 (fn_800392B0 copies them into the role joints, docs/formats/model_entry.md "Joint roles")
Not modelled (they are off for a robo standing at the origin, facing +Z): the facing angle and world position, the
mirrored side (flag 2), motion blending (flag 4), the y offset scale, the per-chara point offset (+0x9A0).

Usage:
  python -I tools/crmotion_rig.py pose <motion file> <motion ID> <frame> <robo entry.bin>   role joint world positions
  python -I tools/crmotion_rig.py gltf <motion file> <motion ID> <robo entry.bin> <robo .glb> <out.glb> [--fps 30]
      the robo model (HSDRawViewer export, nodes JOBJ_n) with the motion as a glTF animation on its role joints
  python -I tools/crmotion_rig.py import-gltf <in.glb> <animation name or index> <robo entry.bin> <motion file> <motion ID>
      <out file> [--no-template] [--tolerance DEG] [--fps 30]
      the role joints' animation solved back into motion tracks (root and joint 0 kept from the motion replaced unless
      --no-template); keys reduced to DEG (default 0.1); a group's float pool that would pass 14 bits is compacted
"""
import json
import math
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import crmotion   # noqa: E402
import robo_joints   # noqa: E402

JOINT_TABLE = 0x801F9B00       # 26 entries of 0x14: flags, ..., floats at +4, +8, +0xC (the game's default skeleton)
COMPACT_TRACKS = 0x801F9D74    # rotation joint k (0..15) -> its first track
TO16 = 65536.0 / 360.0         # FLOAT_8045083c / FLOAT_80450840: degrees -> 16-bit angle
ANGLE_TOL = 0.1                # import-gltf key reduction: degrees per rotation channel (--tolerance); a limb's IK
                               # target is kept so its hinge stays within this; other points within ANGLE_TOL / 100 units


def sincos16(a):
    a = int(a) & 0xFFFF
    r = a * (2 * math.pi / 65536.0)
    return math.sin(r), math.cos(r)


class M:
    """cr_g_CurrentMtx: 3x4 row-major (m[4r+c]); column 3 is the translation."""
    def __init__(self, m=None):
        self.m = list(m) if m else [1.0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1.0, 0]

    def copy(self):
        return M(self.m)

    def t(self):
        return [self.m[3], self.m[7], self.m[11]]

    def mul(self, o):                  # cr_Mtx_Mul: rotation part only, translation unchanged
        a, b = self.m, o.m
        r = list(a)
        for i in range(3):
            for j in range(3):
                r[4 * i + j] = sum(a[4 * i + k] * b[4 * k + j] for k in range(3))
        self.m = r

    def tx(self, d):                   # cr_Mtx_TranslateX
        m = self.m
        m[3] += m[0] * d; m[7] += m[4] * d; m[11] += m[8] * d

    def tz(self, d):                   # fn_8001DC84
        m = self.m
        m[3] += m[2] * d; m[7] += m[6] * d; m[11] += m[10] * d

    def tzx(self, z, x):               # cr_Mtx_TranslateZX
        self.tx(x); self.tz(z)

    def tyz(self, y, z):               # fn_8001DBF0
        m = self.m
        m[3] += m[1] * y; m[7] += m[5] * y; m[11] += m[9] * y
        self.tz(z)

    def rotz(self, s, c):              # col0 / col1 mixed: M . Rz
        m = self.m
        for r in range(3):
            c0, c1 = m[4 * r], m[4 * r + 1]
            m[4 * r], m[4 * r + 1] = c0 * c + c1 * s, c1 * c - c0 * s

    def roty(self, s, c):              # col0 / col2: M . Ry
        m = self.m
        for r in range(3):
            c0, c2 = m[4 * r], m[4 * r + 2]
            m[4 * r], m[4 * r + 2] = c0 * c - c2 * s, c2 * c + c0 * s

    def rotx(self, s, c):              # col1 / col2: M . Rx
        m = self.m
        for r in range(3):
            c1, c2 = m[4 * r + 1], m[4 * r + 2]
            m[4 * r + 1], m[4 * r + 2] = c1 * c + c2 * s, c2 * c - c1 * s

    def local(self, p):                # R^T (p - t): a world point in this frame
        m = self.m
        d = [p[0] - m[3], p[1] - m[7], p[2] - m[11]]
        return [m[0] * d[0] + m[4] * d[1] + m[8] * d[2], m[1] * d[0] + m[5] * d[1] + m[9] * d[2],
                m[2] * d[0] + m[6] * d[1] + m[10] * d[2]]

    def apply(self, v):                # fn_8001DE6C: R v + t
        m = self.m
        return [m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3], m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7],
                m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11]]

    def aim(self, v):
        """Turn the X axis toward the local direction v (the pattern of cr_Chara_PoseLimbs / LimbA / LimbB)."""
        x, y, z = v
        xy = x * x + y * y
        inv = 1.0 / math.sqrt(xy) if xy != 0 else 0.0
        ln = math.sqrt(z * z + xy)
        s = y * inv
        c = x * inv if inv != 0 else 0.0
        self.rotz(s, c)
        k = (1.0 / ln) / inv if inv != 0 else 0.0
        t = -z / ln if ln != 0 else 0.0
        m = self.m
        for r in range(3):
            c0, c2 = m[4 * r], m[4 * r + 2]
            m[4 * r], m[4 * r + 2] = c0 * k - c2 * t, c2 * k + c0 * t


def rot_zyx(ax, ay, az):
    """fn_80053DA8 / fn_80056104: Rz . Ry . Rx from 16-bit angles."""
    r = M()
    if az:
        r.rotz(*sincos16(az))
    if ay:
        r.roty(*sincos16(ay))
    if ax:
        r.rotx(*sincos16(ax))
    return r


class Tables:
    def __init__(self, dol):
        self.joint = []
        for k in range(26):
            e = dol.read(JOINT_TABLE + 0x14 * k, 0x14)
            self.joint.append(struct.unpack('>B3x3f4x', e)[0:4])   # flags, f4, f8, fC
        self.compact = list(dol.read(COMPACT_TRACKS, 16))

    def f(self, addr):
        """A float of the joint table by address (the solvers read it as a flat array)."""
        k, o = divmod(addr - JOINT_TABLE, 0x14)
        return self.joint[k][{4: 1, 8: 2, 0xC: 3}[o]]


def ik_angles(a, b, ln, sign0, sign1):
    """The law-of-cosines pair of fn_8005AAA8 / fn_8005A668 (cosines clamped to [-1, 1])."""
    def cosine(p, q, r):          # angle at the side p between p and the target side q, opposite r
        den = p * q + p * q
        v = 1.0 if den == 0 else -(r * r - (q * q + p * p)) / den
        return max(-1.0, min(1.0, v))
    c1 = cosine(a, ln, b)
    c2 = cosine(a, b, ln)
    return [sign0 * math.sqrt(1 - c1 * c1), c1, sign1 * math.sqrt(1 - c2 * c2), -c2]


def pose(ch, limb, tab, scale=1.0):
    """Channels of one frame (crmotion.sample) -> the 20 body matrices (chara +0x154C)."""
    g = lambda name: ch.get(name, 0.0)
    # fn_80056104: the root matrix and the root point
    rx, ry, rz = g('root.rotx') * TO16, g('root.roty') * TO16, g('root.rotz') * TO16
    if 'root.posx' in ch:
        root = rot_zyx(rx, ry, rz)
        root.m[3], root.m[7], root.m[11] = g('root.posx'), g('root.posy'), g('root.posz')
        t = root.apply([g('root.vecx'), g('root.vecy'), g('root.vecz')])
        root.m[3], root.m[7], root.m[11] = t
        p0 = root.apply([g('point0.x'), g('point0.y'), g('point0.z')])
    else:   # 69 tracks: no root channels, the root point as it is
        root = M()
        p0 = [g('point0.x'), g('point0.y'), g('point0.z')]
    # fn_80054238: the points (world = model space here) and the joint angles
    P = [p0] + [root.apply([g('point%d.x' % i), g('point%d.y' % i), g('point%d.z' % i)]) for i in range(1, 7)]
    slots = sorted(crmotion.JOINT_TRACKS)
    R = []
    for k, slot in enumerate(slots):
        a = [g('joint%d.rot%s' % (slot, ax)) * TO16 for ax in 'xyz']
        if k == 0:
            a = [a[0] + rx, a[1] + ry, a[2] + rz]
        R.append(rot_zyx(a[0], a[1], a[2]))      # fn_80053DA8
    S = [None] * 20
    J = tab.f
    st = []
    cur = M()
    cur.m[3], cur.m[7], cur.m[11] = P[0]
    # cr_Chara_PoseLimbs
    cur.mul(R[0]); S[0] = cur.copy(); st.append(cur.copy())
    cur.mul(R[1]); v234 = cur.local(P[1]); cur.aim(v234); S[1] = cur.copy(); st.append(cur.copy())
    cur.tx(J(0x801F9B2C)); st.append(cur.copy())
    cur.tx(J(0x801F9B40)); cur.mul(R[2]); v244 = cur.local(P[2])
    cur = st.pop(); st.append(cur.copy())

    def pre_arm(rk, target, base):          # fn_8005AAA8
        c = cur.copy()
        c.tx(J(base + 4)); c.mul(R[rk]); c.tzx(J(base + 0x20), J(base + 0x18)); c.mul(R[rk + 1])
        loc = c.local(target)
        return loc, ik_angles(J(base + 0x2C), J(base + 0x40), math.sqrt(sum(x * x for x in loc)), -1, 1)

    tgtA, angA = pre_arm(3, P[3], 0x801F9B64)
    cur = st.pop()
    tgtB, angB = pre_arm(6, P[4], 0x801F9BC8)
    cur = st.pop()
    cur.tx(limb[0] * scale); st.append(cur.copy())
    cur.tx(limb[1] * scale); cur.mul(R[2]); cur.aim(v244); S[2] = cur.copy()
    cur = st.pop(); st.append(cur.copy())

    def limb_a(c, tgt, ang, out, rk, lp):   # cr_Chara_PoseLimbA
        c.tx(lp[0] * scale); c.mul(R[rk]); S[out] = c.copy()
        c.tzx(lp[2] * scale, lp[1] * scale); c.mul(R[rk + 1]); c.aim(tgt); c.rotz(ang[0], ang[1]); S[out + 1] = c.copy()
        c.tx(lp[3] * scale); c.rotz(ang[2], ang[3]); S[out + 2] = c.copy()
        c.tx(lp[4] * scale); c.tz(lp[5] * scale); c.mul(R[rk + 2]); S[out + 3] = c.copy()

    limb_a(cur, tgtA, angA, 3, 3, limb[2:8])
    cur = st.pop()
    limb_a(cur, tgtB, angB, 7, 6, limb[8:14])
    cur = st.pop()
    cur.m[0:3], cur.m[4:7], cur.m[8:11] = [1.0, 0, 0], [0, 1.0, 0], [0, 0, 1.0]   # the facing (none here)
    cur.mul(R[0])
    cur.mul(R[9])
    cur.rotz(-1.0, 0.0)
    S[11] = cur.copy(); st.append(cur.copy())
    cur.tx(J(0x801F9C30)); st.append(cur.copy())

    def pre_leg(rk, target, base):          # fn_8005A668
        c = cur.copy()
        c.tzx(J(base + 0xC), J(base + 4)); c.mul(R[rk])
        loc = c.local(target)
        return loc, ik_angles(J(base + 0x18), J(base + 0x2C), math.sqrt(sum(x * x for x in loc)), 1, -1)

    tgtC, angC = pre_leg(10, P[5], 0x801F9C40)
    cur = st.pop()
    tgtD, angD = pre_leg(13, P[6], 0x801F9CA4)
    cur = st.pop()
    cur.tx(limb[0xE] * scale); st.append(cur.copy())

    def limb_b(c, tgt, ang, out, rk, lp):   # cr_Chara_PoseLimbB
        c.tzx(lp[1] * scale, lp[0] * scale); c.mul(R[rk]); c.aim(tgt); c.rotz(ang[0], ang[1]); S[out] = c.copy()
        c.tx(lp[2] * scale); c.rotz(ang[2], ang[3]); S[out + 1] = c.copy()
        c.tx(lp[3] * scale)
        c.m[0:3], c.m[4:7], c.m[8:11] = [1.0, 0, 0], [0, 1.0, 0], [0, 0, 1.0]
        if rz:
            c.rotz(*sincos16(rz))
        if ry:
            c.roty(*sincos16(ry))
        if rx:
            c.rotx(*sincos16(rx))
        c.mul(R[rk + 1]); S[out + 2] = c.copy()
        c.tyz(lp[4] * scale, lp[5] * scale); c.mul(R[rk + 2]); S[out + 3] = c.copy()

    limb_b(cur, tgtC, angC, 12, 10, limb[0xF:0x15])
    cur = st.pop()
    limb_b(cur, tgtD, angD, 16, 13, limb[0x15:0x1B])
    return S, P


# ---- the robo model: block A, roles, joint tree

def robo_inputs(entry):
    h = struct.unpack_from('>16I', entry)
    limb = list(struct.unpack_from('>32f', entry, h[3]))
    njoints, _, roles = robo_joints.decode(entry)
    return limb, roles


def load_motion(path, mid):
    m = crmotion.parse(crmotion.read_motion(path))
    return crmotion.find_motion(m, mid)


def cmd_pose(path, mid, frame, entry_path, dol):
    grp, e = load_motion(path, mid)
    limb, roles = robo_inputs(Path(entry_path).read_bytes())
    S, P = pose(crmotion.sample(e, grp['floats'], frame), limb, Tables(dol))
    for i, p in enumerate(P):
        print('point%d %8.3f %8.3f %8.3f' % (i, *p))
    for r in range(19):
        print('role %2d joint %-4s %8.3f %8.3f %8.3f' % (r, roles[r], *S[r + 1].t()))


# ---- glTF

def mat4(m3x4):
    m = m3x4.m
    return [[m[0], m[1], m[2], m[3]], [m[4], m[5], m[6], m[7]], [m[8], m[9], m[10], m[11]], [0, 0, 0, 1]]


def mm(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


def inv_rigid(a):
    """Inverse of an affine 4x4 (general 3x3 part)."""
    r = [row[:3] for row in a[:3]]
    det = (r[0][0] * (r[1][1] * r[2][2] - r[1][2] * r[2][1]) - r[0][1] * (r[1][0] * r[2][2] - r[1][2] * r[2][0])
           + r[0][2] * (r[1][0] * r[2][1] - r[1][1] * r[2][0]))
    ri = [[(r[(j + 1) % 3][(i + 1) % 3] * r[(j + 2) % 3][(i + 2) % 3] - r[(j + 1) % 3][(i + 2) % 3] * r[(j + 2) % 3][(i + 1) % 3]) / det
           for j in range(3)] for i in range(3)]
    t = [a[0][3], a[1][3], a[2][3]]
    ti = [-sum(ri[i][k] * t[k] for k in range(3)) for i in range(3)]
    return [ri[0] + [ti[0]], ri[1] + [ti[1]], ri[2] + [ti[2]], [0, 0, 0, 1]]


def trs_matrix(node):
    if 'matrix' in node:
        c = node['matrix']
        return [[c[0], c[4], c[8], c[12]], [c[1], c[5], c[9], c[13]], [c[2], c[6], c[10], c[14]], [0, 0, 0, 1]]
    t = node.get('translation', [0, 0, 0]); q = node.get('rotation', [0, 0, 0, 1]); s = node.get('scale', [1, 1, 1])
    x, y, z, w = q
    r = [[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
         [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
         [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]]
    return [[r[i][0] * s[0], r[i][1] * s[1], r[i][2] * s[2], t[i]] for i in range(3)] + [[0, 0, 0, 1]]


def decompose(a):
    t = [a[0][3], a[1][3], a[2][3]]
    cols = [[a[0][j], a[1][j], a[2][j]] for j in range(3)]
    s = [math.sqrt(sum(x * x for x in c)) for c in cols]
    r = [[cols[j][i] / s[j] for j in range(3)] for i in range(3)]
    tr = r[0][0] + r[1][1] + r[2][2]
    if tr > 0:
        k = math.sqrt(tr + 1.0) * 2
        q = [(r[2][1] - r[1][2]) / k, (r[0][2] - r[2][0]) / k, (r[1][0] - r[0][1]) / k, 0.25 * k]
    elif r[0][0] > r[1][1] and r[0][0] > r[2][2]:
        k = math.sqrt(1.0 + r[0][0] - r[1][1] - r[2][2]) * 2
        q = [0.25 * k, (r[0][1] + r[1][0]) / k, (r[0][2] + r[2][0]) / k, (r[2][1] - r[1][2]) / k]
    elif r[1][1] > r[2][2]:
        k = math.sqrt(1.0 + r[1][1] - r[0][0] - r[2][2]) * 2
        q = [(r[0][1] + r[1][0]) / k, 0.25 * k, (r[1][2] + r[2][1]) / k, (r[0][2] - r[2][0]) / k]
    else:
        k = math.sqrt(1.0 + r[2][2] - r[0][0] - r[1][1]) * 2
        q = [(r[0][2] + r[2][0]) / k, (r[1][2] + r[2][1]) / k, 0.25 * k, (r[1][0] - r[0][1]) / k]
    n = math.sqrt(sum(x * x for x in q))
    return t, [x / n for x in q], s


def read_glb(path):
    d = Path(path).read_bytes()
    jl = struct.unpack_from('<I', d, 12)[0]
    j = json.loads(d[20:20 + jl])
    o = 20 + jl
    binc = b''
    if o < len(d):
        bl = struct.unpack_from('<I', d, o)[0]
        binc = d[o + 8:o + 8 + bl]
    return j, bytearray(binc)


def write_glb(path, j, binc):
    while len(binc) % 4:
        binc.append(0)
    js = json.dumps(j, separators=(',', ':')).encode()
    js += b' ' * (-len(js) % 4)
    out = struct.pack('<III', 0x46546C67, 2, 12 + 8 + len(js) + 8 + len(binc))
    out += struct.pack('<II', len(js), 0x4E4F534A) + js + struct.pack('<II', len(binc), 0x004E4942) + bytes(binc)
    Path(path).write_bytes(out)


def add_accessor(j, binc, data, ctype, fmt):
    while len(binc) % 4:
        binc.append(0)
    off = len(binc)
    flat = [x for v in data for x in (v if isinstance(v, (list, tuple)) else [v])]
    binc += struct.pack('<%df' % len(flat), *flat)
    j['bufferViews'].append({'buffer': 0, 'byteOffset': off, 'byteLength': 4 * len(flat)})
    acc = {'bufferView': len(j['bufferViews']) - 1, 'componentType': 5126, 'count': len(data), 'type': ctype}
    if ctype == 'SCALAR':
        acc['min'] = [min(data)]; acc['max'] = [max(data)]
    j['accessors'].append(acc)
    return len(j['accessors']) - 1


def cmd_gltf(path, mid, entry_path, glb_in, glb_out, dol, fps=30.0):
    grp, e = load_motion(path, mid)
    limb, roles = robo_inputs(Path(entry_path).read_bytes())
    tab = Tables(dol)
    j, binc = read_glb(glb_in)
    nodes = j['nodes']
    byname = {n.get('name'): i for i, n in enumerate(nodes)}
    parent = {}
    for i, n in enumerate(nodes):
        for c in n.get('children', []):
            parent[c] = i
    bind_local = {i: trs_matrix(n) for i, n in enumerate(nodes)}
    role_node = {r: byname.get('JOBJ_%d' % jn) for r, jn in enumerate(roles) if jn is not None}
    driven = set(role_node.values()) - {None}
    # the nodes whose world matrix changes: role joints and everything under them
    last = max((t['times'][-1] for t in e['tracks'] if t), default=0)
    frames = list(range(0, last + 1))
    tracks = {i: [] for i in driven}

    def world_of(i, cache, roleworld):
        if i in cache:
            return cache[i]
        if i in roleworld:
            w = roleworld[i]
        elif i in parent:
            w = mm(world_of(parent[i], cache, roleworld), bind_local[i])
        else:
            w = bind_local[i]
        cache[i] = w
        return w

    for f in frames:
        S, _ = pose(crmotion.sample(e, grp['floats'], f), limb, tab)
        roleworld = {role_node[r]: mat4(S[r + 1]) for r in role_node if role_node[r] is not None}
        cache = {}
        for i in driven:
            pw = world_of(parent[i], cache, roleworld) if i in parent else [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]
            tracks[i].append(decompose(mm(inv_rigid(pw), roleworld[i])))
    j.setdefault('animations', [])
    j.setdefault('bufferViews', []); j.setdefault('accessors', [])
    times = add_accessor(j, binc, [f / fps for f in frames], 'SCALAR', 'f')
    samplers, channels = [], []
    for i in sorted(driven):
        for k, path_ in enumerate(('translation', 'rotation', 'scale')):
            vals = [tr[k] for tr in tracks[i]]
            acc = add_accessor(j, binc, vals, 'VEC4' if k == 1 else 'VEC3', 'f')
            samplers.append({'input': times, 'output': acc, 'interpolation': 'LINEAR'})
            channels.append({'sampler': len(samplers) - 1, 'target': {'node': i, 'path': path_}})
    name = (dol.name(e['id']) if dol else '') or 'motion_%04x' % e['id']
    j['animations'].append({'name': name, 'samplers': samplers, 'channels': channels})
    if j.get('buffers'):
        j['buffers'][0]['byteLength'] = len(binc) + (-len(binc) % 4)
    else:
        j['buffers'] = [{'byteLength': len(binc) + (-len(binc) % 4)}]
    write_glb(glb_out, j, binc)
    print('wrote %s: %s, %d frames, %d joints animated' % (glb_out, name, len(frames), len(driven)))


# ---- glTF -> motion (the rig solved backwards)

def gltf_sample(j, binc, anim, t):
    """Node overrides {node: {path: value}} of a glTF animation at time t (LINEAR / STEP; rotations nlerp)."""
    def acc(i):
        a = j['accessors'][i]
        bv = j['bufferViews'][a['bufferView']]
        n = {'SCALAR': 1, 'VEC3': 3, 'VEC4': 4}[a['type']]
        if a['componentType'] != 5126:
            raise ValueError('only float animation data is read')
        off = bv.get('byteOffset', 0) + a.get('byteOffset', 0)
        st = bv.get('byteStride', 4 * n)
        return [list(struct.unpack_from('<%df' % n, binc, off + st * k)) for k in range(a['count'])]
    ov = {}
    for ch in anim['channels']:
        s = anim['samplers'][ch['sampler']]
        ts = [x[0] for x in acc(s['input'])]
        vs = acc(s['output'])
        k = max(0, min(len(ts) - 1, sum(1 for x in ts if x <= t + 1e-9) - 1))
        if s.get('interpolation', 'LINEAR') == 'LINEAR' and k + 1 < len(ts) and ts[k + 1] > ts[k] and t > ts[k]:
            f = (t - ts[k]) / (ts[k + 1] - ts[k])
            a, b = vs[k], vs[k + 1]
            if ch['target']['path'] == 'rotation' and sum(x * y for x, y in zip(a, b)) < 0:
                b = [-x for x in b]
            v = [x * (1 - f) + y * f for x, y in zip(a, b)]
        else:
            v = vs[k]
        if ch['target']['path'] == 'rotation':
            n = math.sqrt(sum(x * x for x in v)); v = [x / n for x in v]
        ov.setdefault(ch['target']['node'], {})[ch['target']['path']] = v
    return ov


def node_worlds(j, ov, want):
    nodes = j['nodes']
    parent = {c: i for i, n in enumerate(nodes) for c in n.get('children', [])}
    cache = {}

    def w(i):
        if i not in cache:
            n = dict(nodes[i])
            if i in ov:
                n.pop('matrix', None); n.update(ov[i])
            m = trs_matrix(n)
            cache[i] = mm(w(parent[i]), m) if i in parent else m
        return cache[i]
    return {i: w(i) for i in want}


def m3(a):
    """4x4 -> M (rotation orthonormalised by Gram-Schmidt on X then Y; translation kept)."""
    x = [a[0][0], a[1][0], a[2][0]]; y = [a[0][1], a[1][1], a[2][1]]
    nx = math.sqrt(sum(v * v for v in x)); x = [v / nx for v in x]
    d = sum(p * q for p, q in zip(x, y)); y = [p - d * q for p, q in zip(y, x)]
    ny = math.sqrt(sum(v * v for v in y)); y = [v / ny for v in y]
    z = [x[1] * y[2] - x[2] * y[1], x[2] * y[0] - x[0] * y[2], x[0] * y[1] - x[1] * y[0]]
    return M([x[0], y[0], z[0], a[0][3], x[1], y[1], z[1], a[1][3], x[2], y[2], z[2], a[2][3]])


def rel(a, b):
    """The rotation r with a.R . r = b.R (a M with only its rotation used)."""
    r = M()
    A, B = a.m, b.m
    for i in range(3):
        for k in range(3):
            r.m[4 * i + k] = sum(A[4 * p + i] * B[4 * p + k] for p in range(3))
    return r


def euler_zyx(r, near=None):
    """R = Rz . Ry . Rx -> (x, y, z) in degrees: of the two solutions (and, in gimbal lock, every z) the one nearest
    `near` (the previous frame), each angle moved by whole turns toward it."""
    m = r.m
    near = near or [0.0, 0.0, 0.0]
    z1 = math.degrees(math.atan2(m[4], m[0]))
    zs = [z1, z1 + 180.0]
    if abs(m[8]) > 0.9999:                     # cos y ~ 0: z is free, keep the previous frame's
        zs.append(near[2])
    best = None
    for z in zs:
        c, s_ = math.cos(math.radians(z)), math.sin(math.radians(z))
        # Rz(-z) . R = Ry . Rx = [[cy, sy sx, sy cx], [0, cx, -sx], [-sy, cy sx, cy cx]]
        a00 = c * m[0] + s_ * m[4]
        a11 = -s_ * m[1] + c * m[5]
        a12 = -s_ * m[2] + c * m[6]
        a20 = m[8]
        x = math.degrees(math.atan2(-a12, a11))
        y = math.degrees(math.atan2(-a20, a00))
        e = [x, y, z]
        e = [v + 360.0 * round((n - v) / 360.0) for v, n in zip(e, near)]
        d = sum((v - n) ** 2 for v, n in zip(e, near))
        if best is None or d < best[0]:
            best = (d, e)
    return best[1]


def solve_frame(S, limb, tab, root_ch, joint0, near=None, scale=1.0):
    """20 body matrices (S[1..19] used) -> the channels that pose them (root channels and joint 0 given)."""
    J = tab.f
    g = lambda k: root_ch.get(k, 0.0)
    rx, ry, rz = g('root.rotx') * TO16, g('root.roty') * TO16, g('root.rotz') * TO16
    root = rot_zyx(rx, ry, rz)
    root.m[3], root.m[7], root.m[11] = g('root.posx'), g('root.posy'), g('root.posz')
    t = root.apply([g('root.vecx'), g('root.vecy'), g('root.vecz')])
    root.m[3], root.m[7], root.m[11] = t
    a0 = [joint0[i] * TO16 + (rx, ry, rz)[i] for i in range(3)]
    R0 = rot_zyx(*a0)
    R = [None] * 16
    R[0] = R0
    P = [None] * 7
    P[0] = S[1].t()
    base = M(); base.m[3], base.m[7], base.m[11] = P[0]; base.mul(R0)
    # upper body and head: the target along the bone's X axis, so the aim is no turn
    R[1] = rel(base, S[1])
    d1 = 0.3
    x1 = [S[1].m[0], S[1].m[4], S[1].m[8]]
    P[1] = [P[0][i] + x1[i] * d1 for i in range(3)]
    ta = S[1].copy(); ta.tx(J(0x801F9B2C)); ta.tx(J(0x801F9B40))
    R[2] = rel(S[1], S[2])
    x2 = [S[2].m[0], S[2].m[4], S[2].m[8]]
    P[2] = [ta.t()[i] + x2[i] * 0.2 for i in range(3)]

    def hinge(a, b):           # the angle of b about a's Z axis (a . Rz(s, c) = b)
        r = rel(a, b)
        return math.atan2(r.m[4], r.m[0])

    def arm(out, rk, base_tab, sign0, sign1, lp, legs=False):
        if not legs:
            c = S[1].copy(); c.tx(limb[0] * scale)
            R[rk] = rel(c, S[out])                     # shoulder: S = c (tx) . R
            frame_rot = M(S[out].m)                   # rotation S_out.R
            th = hinge(S[out + 1], S[out + 2])         # elbow: S2 = S1 (tx) . Rz
            a, b = J(base_tab + 0x2C), J(base_tab + 0x40)
        else:
            c = S[11].copy(); c.tx(limb[0xE] * scale)
            frame_rot = M(c.m)
            th = hinge(S[out], S[out + 1])
            a, b = J(base_tab + 0x18), J(base_tab + 0x2C)
        # the hinge: sin = sign1 * sqrt(1 - c2^2), cos = -c2 (c2: cosine of the inner angle, law of cosines)
        s2, cth = math.sin(th), math.cos(th)
        if sign1 * s2 < 0:
            cth = math.copysign(1.0, cth) if abs(cth) > 0.999 else cth   # bent the wrong way: the game cannot; keep straight-ish
        c2 = max(-1.0, min(1.0, -cth))
        L = math.sqrt(max(1e-12, a * a + b * b - 2 * a * b * c2))
        c1 = max(-1.0, min(1.0, (a * a + L * L - b * b) / (2 * a * L)))
        phi = math.atan2(sign0 * math.sqrt(1 - c1 * c1), c1)
        upper = S[out + 1] if not legs else S[out]
        # upper = frame . R_upper . Rz(phi)  ->  R_upper = frame^T . upper . Rz(-phi)
        r = rel(frame_rot, upper); r.rotz(-math.sin(phi), math.cos(phi))
        if not legs:
            R[rk + 1] = r
            g_ = S[1].copy(); g_.tx(J(0x801F9B2C)); g_.tx(J(base_tab + 4)); g_.mul(R[rk])
            g_.tzx(J(base_tab + 0x20), J(base_tab + 0x18)); g_.mul(R[rk + 1])
            fore = M(S[out + 1].m)                     # the forearm as the game builds it: the upper arm bent by the
            fore.rotz(sign1 * math.sqrt(1 - c2 * c2), -c2)   # hinge actually stored (a bend off the Z axis is dropped)
            R[rk + 2] = rel(fore, S[out + 3])
        else:
            R[rk] = r
            g_ = S[11].copy(); g_.tx(J(0x801F9C30)); g_.tzx(J(base_tab + 0xC), J(base_tab + 4)); g_.mul(R[rk])
            ankle = M(); ankle.rotz(*sincos16(rz)) if rz else None
            if ry:
                ankle.roty(*sincos16(ry))
            if rx:
                ankle.rotx(*sincos16(rx))
            R[rk + 1] = rel(ankle, S[out + 2])
            R[rk + 2] = rel(S[out + 2], S[out + 3])
        # how far the target may move for 0.05 degree at the hinge: dL / dtheta = a b sin(theta) / L, tight near straight
        tol = min(0.0003, max(1e-6, a * b * math.sqrt(max(0.0, 1 - c2 * c2)) / L * math.radians(ANGLE_TOL)))
        return g_.apply([L, 0.0, 0.0]), tol

    tol = {}
    P[3], tol[3] = arm(3, 3, 0x801F9B64, -1, 1, limb[2:8])
    P[4], tol[4] = arm(7, 6, 0x801F9BC8, -1, 1, limb[8:14])
    lower = M(); lower.mul(R0)
    R[9] = rel(lower, S[11]); R[9].rotz(1.0, 0.0)          # S11 = R0 . R9 . Rz(-90)
    P[5], tol[5] = arm(12, 10, 0x801F9C40, 1, -1, limb[0xF:0x15], legs=True)
    P[6], tol[6] = arm(16, 13, 0x801F9CA4, 1, -1, limb[0x15:0x1B], legs=True)
    # back to tracks: points through the root matrix, angles as Rz.Ry.Rx in degrees
    inv = M(); inv.m = list(root.m)
    ch = dict(root_ch)
    for i, v in tol.items():
        ch['tol.point%d' % i] = v
    for i in range(7):
        p = inv.local(P[i])
        for a, v in zip('xyz', p):
            ch['point%d.%s' % (i, a)] = v
    for k, slot in enumerate(sorted(crmotion.JOINT_TRACKS)):
        e = euler_zyx(R[k], [near.get('joint%d.rot%s' % (slot, a), 0.0) for a in 'xyz'] if near else None) if k else list(joint0)
        for a, v in zip('xyz', e):
            ch['joint%d.rot%s' % (slot, a)] = v
    return ch


def fit_tangents(keys, vals, prior):
    """Least-squares tangents (per second, one per key) of the game's Hermite curve through vals at the key frames."""
    K = len(keys)
    A = [[0.0] * K for _ in range(K)]
    b = [0.0] * K
    lam = 1e-6
    for k in range(K):
        A[k][k] += lam; b[k] += lam * prior[keys[k]]
    for j in range(K - 1):
        t0, t1 = keys[j], keys[j + 1]
        dt = (t1 - t0) * crmotion.TIME_UNIT
        p0, p1 = vals[t0], vals[t1]
        for i in range(t0 + 1, t1):
            s_ = (i - t0) / (t1 - t0); u = 1 - s_
            r = vals[i] - (p0 * u * u * (1 + 2 * s_) + p1 * s_ * s_ * (3 - 2 * s_))
            a, c = dt * s_ * u * u, -dt * s_ * s_ * u
            A[j][j] += a * a; A[j][j + 1] += a * c; A[j + 1][j] += a * c; A[j + 1][j + 1] += c * c
            b[j] += a * r; b[j + 1] += c * r
    for k in range(K):                      # tridiagonal: Gaussian elimination without pivoting is fine (diagonal > 0)
        piv = A[k][k]
        if k + 1 < K:
            f = A[k + 1][k] / piv
            A[k + 1][k + 1] -= f * A[k][k + 1]; b[k + 1] -= f * b[k]; A[k + 1][k] = 0.0
    m = [0.0] * K
    for k in range(K - 1, -1, -1):
        m[k] = (b[k] - (A[k][k + 1] * m[k + 1] if k + 1 < K else 0.0)) / A[k][k]
    return m


def reduce_keys(vals, eps):
    """Per-frame values -> keys (frame, kind, value, tangent) whose Hermite replay stays within eps of every frame:
    keys added greedily at the worst frame, tangents fitted by least squares after each addition."""
    eps = list(eps) if isinstance(eps, (list, tuple)) else [eps] * len(vals)
    n = len(vals)
    if max(vals) - min(vals) <= min(eps):
        v = sum(vals) / n
        if abs(v) <= min(eps):
            return []
        # a held value still needs a key on the last frame: past a track's last key the game reads the words after
        # its list as more keys (cr_Motion_SampleTrack), and every track on the disc ends on its motion's last frame
        # (found Nov 2, 2026: one key at frame 0 broke the Atlas dash and gun motions in a battle)
        return [{'frame': f, 'kind': 'flat', 'value': v} for f in sorted({0, n - 1})]
    prior = []
    for i in range(n):
        d = (vals[min(n - 1, i + 1)] - vals[max(0, i - 1)]) / (min(n - 1, i + 1) - max(0, i - 1))
        prior.append(d / crmotion.TIME_UNIT)
    keep = [0, n - 1]
    while True:
        keep.sort()
        tans = fit_tangents(keep, vals, prior)
        floats, tr = [], {'times': [], 'words': []}
        for f, t in zip(keep, tans):
            tr['times'].append(f); tr['words'].append(len(floats) | 0xC000)
            floats += [crmotion.f32(vals[f]), crmotion.f32(t)]
        errs = [abs(crmotion.sample_track(tr, floats, i) - vals[i]) / eps[i] for i in range(n)]
        worst = max(range(n), key=lambda i: errs[i])
        if errs[worst] <= 1.0 or len(keep) >= 255:
            break
        keep.append(worst)
    return [{'frame': f, 'kind': 'flat', 'value': vals[f]} if abs(t) < 1e-6 else
            {'frame': f, 'kind': 'smooth', 'value': vals[f], 'in': t, 'out': t} for f, t in zip(keep, tans)]


def compact_pool(grp):
    """Rebuild a group's float pool from the keys still used (when an import would overflow 14 bits)."""
    keys = []
    for m in grp['motions']:
        for tr in m['tracks']:
            if tr:
                tr['keys'] = [crmotion.decode_key(w, grp['floats']) for w in tr['words']]
    grp['floats'] = []
    pool = {}
    for m in grp['motions']:
        for tr in m['tracks']:
            if tr:
                tr['words'] = [crmotion.encode_key({'kind': k, 'value': v, 'in': i, 'out': o}, grp['floats'], pool)
                               for k, v, i, o in tr.pop('keys')]


def cmd_import_gltf(glb, anim_sel, entry_path, motion_path, mid, out_path, dol, fps=30.0, template=True, eps=None):
    j, binc = read_glb(glb)
    anims = j.get('animations', [])
    anim = next((a for a in anims if a.get('name') == anim_sel), None)
    if anim is None:
        anim = anims[int(anim_sel)]
    limb, roles = robo_inputs(Path(entry_path).read_bytes())
    tab = Tables(dol)
    byname = {n.get('name'): i for i, n in enumerate(j['nodes'])}
    role_node = [byname['JOBJ_%d' % jn] for jn in roles]
    data = crmotion.parse(crmotion.read_motion(motion_path))
    grp, e = crmotion.find_motion(data, mid)
    tmax = 0.0
    for s in anim['samplers']:
        a = j['accessors'][s['input']]
        tmax = max(tmax, a['max'][0] if 'max' in a else 0.0)
    nframes = int(round(tmax * fps)) + 1
    roles_tr = crmotion.track_roles(len(e['tracks']))
    chans = []
    for f in range(nframes):
        W = node_worlds(j, gltf_sample(j, binc, anim, f / fps), role_node)
        S = [None] + [m3(W[n]) for n in role_node]
        ref = crmotion.sample(e, grp['floats'], f) if template else {}
        root_ch = {k: v for k, v in ref.items() if k.startswith('root.')}
        j0 = [ref.get('joint0.rot%s' % a, 0.0) for a in 'xyz']
        chans.append(solve_frame(S, limb, tab, root_ch, j0, chans[-1] if chans else ref))
    # unwrap angles, then keys per track
    new_tracks = []
    for k, role in enumerate(roles_tr):
        vals = [c.get(role, 0.0) for c in chans]
        if 'rot' in role:
            for i in range(1, len(vals)):
                while vals[i] - vals[i - 1] > 180: vals[i] -= 360
                while vals[i] - vals[i - 1] < -180: vals[i] += 360
        if eps is not None:
            e_ = eps
        elif 'rot' in role:
            e_ = ANGLE_TOL
        elif role[:6] in ('point3', 'point4', 'point5', 'point6'):
            e_ = [c['tol.' + role[:6]] for c in chans]
        else:
            e_ = ANGLE_TOL / 100.0
        keys = reduce_keys(vals, e_)
        new_tracks.append(keys)
    pool = {}
    for w in (2, 1, 3):
        for i in range(len(grp['floats']) - w + 1):
            pool.setdefault(tuple(grp['floats'][i:i + w]), i)
    try:
        floats = list(grp['floats'])
        enc = [None if not ks else {'times': [x['frame'] for x in ks], 'words': [crmotion.encode_key(x, floats, pool) for x in ks]}
               for ks in new_tracks]
        grp['floats'] = floats
    except ValueError:
        e['tracks'] = [None] * len(e['tracks'])
        compact_pool(grp)
        floats = grp['floats']
        pool = {}
        for w in (2, 1, 3):
            for i in range(len(floats) - w + 1):
                pool.setdefault(tuple(floats[i:i + w]), i)
        free = 0x4000 - len(floats)
        try:
            enc = [None if not ks else {'times': [x['frame'] for x in ks], 'words': [crmotion.encode_key(x, floats, pool) for x in ks]}
                   for ks in new_tracks]
        except ValueError:
            need = sum({'flat': 1, 'smooth': 2, 'full': 3}.get(x['kind'], 0) for ks in new_tracks for x in ks)
            gi = data['groups'].index(grp)
            raise SystemExit('motion %#x does not fit: group %d of %s has %d free floats (of 16,384, after dropping unused '
                             'ones) and this motion needs up to %d. Use a larger --tolerance (now %g degrees), a shorter '
                             'animation, or a motion in a group with room (crmotion.py list shows the floats of each group).'
                             % (e['id'], gi, motion_path, free, need, ANGLE_TOL))
    e['tracks'] = enc
    Path(out_path).write_bytes(crmotion.build(data))
    nkeys = sum(len(t['times']) for t in enc if t)
    print('wrote %s: motion %#x from %s, %d frames, %d keys, group floats %d' % (
        out_path, e['id'], anim.get('name', anim_sel), nframes, nkeys, len(grp['floats'])))


def main(argv):
    dolp = None
    if '--dol' in argv:
        i = argv.index('--dol'); dolp = argv[i + 1]; del argv[i:i + 2]
    global ANGLE_TOL
    if '--tolerance' in argv:
        i = argv.index('--tolerance'); ANGLE_TOL = float(argv[i + 1]); del argv[i:i + 2]
    fps = 30.0
    if '--fps' in argv:
        i = argv.index('--fps'); fps = float(argv[i + 1]); del argv[i:i + 2]
    dol = crmotion.load_dol(dolp)
    if dol is None:
        raise SystemExit('main.dol not found (--dol PATH)')
    if len(argv) >= 6 and argv[1] == 'pose':
        cmd_pose(argv[2], int(argv[3], 0), float(argv[4]), argv[5], dol)
    elif len(argv) >= 7 and argv[1] == 'gltf':
        cmd_gltf(argv[2], int(argv[3], 0), argv[4], argv[5], argv[6], dol, fps)
    elif len(argv) >= 8 and argv[1] == 'import-gltf':
        template = '--no-template' not in argv
        argv = [a for a in argv if a != '--no-template']
        cmd_import_gltf(argv[2], argv[3], argv[4], argv[5], int(argv[6], 0), argv[7], dol, fps, template)
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main(sys.argv)
