"""Legion Arena: Basic Arena (stage 0) rebuilt as a small Roman amphitheatre, for mods/atlas_legion_arena.

usage: blender -b --factory-startup --python tools/blender/legion_arena.py -- <export dir> <out dir> <cc0 dir> <collision.json>

<export dir>     HSDRawViewer --export of bg_data_000.dat (scene_data_<n>.glb)
<out dir>        writes scene_data_<n>.glb for n = 0..9 (the models to import, each over its own template model) and
                 textures_<name>.png (the prepared textures, for checking)
<cc0 dir>        ambientCG materials (CC0) unpacked: <id>/<id>_1K-JPG_Color.jpg (and _AmbientOcclusion.jpg)
<collision.json> the stage's collision groups (tools/stage_collision.py parse): obstacles are built on their boxes

Everything is built in one Blender scene in game coordinates (glTF +Y up; Blender (x, -z, y)) so the lighting can be
baked across models: every vertex gets a colour = sky light x ambient occlusion + sun x shadow (BVH ray casts, the sun
in the stage light's direction, game (10, 20, 5) toward the origin). The game draws these materials as texture x
vertex colour, unlit ("VERTEX, TEX0" copied from the floor with --tex-model 0), so the lighting is in the colours.

Model 0 (the arena, everything on JOBJ_6; the sky on JOBJ_2; every Basic Arena mesh of model 0 dropped with --replace):
  sand floor, a travertine-brick podium wall with a marble coping and two wooden gates, 14 tiers of travertine seats
  that round the square arena into an oval, a colonnade on the top tier, a terrace, upper seating and a three-storey
  arcade facade (arches painted into its texture), red banners, a sunset sky dome.
  Faces are single-sided and face the arena (or up): a camera outside the near stands looks through their backs.
Models 1, 5-9 (the obstacles; collision groups 1, 5-9 unchanged): stone blocks on their exact collision boxes with a
  marble coping; models 8 and 9 (the side walls) are bronze-banded wooden barricades (model 9 slides: make_stage.py).
Models 2-4 (the breakable pods): kept as they are, recoloured to bronze and gold (their meshes, animations and break
  shards stay; the importer turns a changed base colour into the diffuse colour and re-encodes an edited texture).
"""
import json
import math
import os
import random
import sys

import bmesh
import bpy
import numpy as np
from mathutils import Vector
from mathutils.bvhtree import BVHTree

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

EXPORT, OUT, CC0, COLL = common.args(4, __doc__)
bpy.ops.wm.read_factory_settings(use_empty=True)   # no startup cube / camera / light
os.makedirs(OUT, exist_ok=True)
random.seed(7)
log = lambda *a: print('legion_arena:', *a)


def B(x, y, z):
    """game (x, y up, z) -> Blender"""
    return Vector((x, -z, y))


SUN = B(10.0, 20.0, 5.0).normalized()
SUN_COL = np.array([1.00, 0.86, 0.66])
SKY_COL = np.array([0.50, 0.52, 0.62])

# ---------------------------------------------------------------- textures

def load_px(path, size):
    img = bpy.data.images.load(path)
    img.scale(size, size)
    px = np.array(img.pixels[:], dtype=np.float32).reshape(size, size, 4)
    bpy.data.images.remove(img)
    return px


def cc0(asset, size, grade=(1, 1, 1), ao_amount=0.6, contrast=1.0):
    col = load_px(os.path.join(CC0, asset, f'{asset}_1K-JPG_Color.jpg'), size)
    aop = os.path.join(CC0, asset, f'{asset}_1K-JPG_AmbientOcclusion.jpg')
    if os.path.exists(aop):
        ao = load_px(aop, size)[:, :, :1]
        col[:, :, :3] *= (1 - ao_amount) + ao_amount * ao
    rgb = col[:, :, :3]
    m = rgb.mean()
    rgb = (rgb - m) * contrast + m
    col[:, :, :3] = np.clip(rgb * np.array(grade), 0, 1)
    col[:, :, 3] = 1
    return col


def make_image(name, px):
    h, w = px.shape[:2]
    img = bpy.data.images.new(name, w, h, alpha=False)
    img.pixels[:] = px.astype(np.float32).ravel()
    img.pack()
    img.filepath_raw = os.path.join(OUT, f'texture_{name}.png')
    img.file_format = 'PNG'
    img.save()
    return img


def tileable_noise(w, h, octaves=5, seed=1, fx=1, fy=1):
    """smooth noise that wraps in x (and y): a sum of random-phase sine products with integer frequencies"""
    rng = np.random.default_rng(seed)
    u = np.linspace(0, 2 * np.pi, w, endpoint=False)[None, :]
    v = np.linspace(0, 2 * np.pi, h, endpoint=False)[:, None]
    out = np.zeros((h, w))
    amp = 1.0
    for o in range(octaves):
        f = 2 ** o
        for _ in range(4):
            kx, ky = rng.integers(1, 3) * f * fx, rng.integers(0, 3) * f * fy
            out += amp * np.sin(kx * u + rng.uniform(0, 6.3)) * np.cos(ky * v + rng.uniform(0, 6.3))
        amp *= 0.55
    out -= out.min()
    return out / out.max()


def arcade_texture(size, stone):
    """one bay of the facade: a travertine pier frame around an arch whose inside is in shadow, a cornice on top"""
    px = stone.copy()
    h, w = size, size
    y, x = np.mgrid[0:h, 0:w].astype(np.float32)
    yy = y / h            # 0 = bottom (Blender images start at the bottom row)
    xx = x / w
    half = 0.30
    spring = 0.55        # the arch springs here, its top is a half circle
    cx = 0.5
    inside_rect = (np.abs(xx - cx) < half) & (yy < spring)
    inside_round = ((xx - cx) ** 2 + ((yy - spring) * 1.0) ** 2 < half ** 2) & (yy >= spring)
    arch = inside_rect | inside_round
    depth = np.clip(1.0 - yy * 0.5, 0, 1)
    dark = np.stack([0.10 + 0.06 * depth, 0.08 + 0.05 * depth, 0.07 + 0.04 * depth], -1)
    px[:, :, :3] = np.where(arch[:, :, None], dark, px[:, :, :3])
    # the arch's rim (voussoirs): lighter stone ring
    r = np.sqrt((xx - cx) ** 2 + (yy - spring) ** 2)
    rim = ((r > half) & (r < half + 0.05) & (yy >= spring)) | ((np.abs(np.abs(xx - cx) - half - 0.025) < 0.025) & (yy < spring))
    px[:, :, :3] = np.where(rim[:, :, None], np.clip(px[:, :, :3] * 1.18, 0, 1), px[:, :, :3])
    # half-column pilasters at the bay edges
    pil = (xx < 0.07) | (xx > 0.93)
    px[:, :, :3] = np.where(pil[:, :, None], np.clip(px[:, :, :3] * 1.1, 0, 1), px[:, :, :3])
    edge = (np.abs(xx - 0.07) < 0.008) | (np.abs(xx - 0.93) < 0.008)
    px[:, :, :3] = np.where(edge[:, :, None], px[:, :, :3] * 0.6, px[:, :, :3])
    # the cornice band at the top of the storey
    corn = yy > 0.90
    px[:, :, :3] = np.where(corn[:, :, None], np.clip(px[:, :, :3] * 1.15, 0, 1), px[:, :, :3])
    line = np.abs(yy - 0.90) < 0.012
    px[:, :, :3] = np.where(line[:, :, None], px[:, :, :3] * 0.45, px[:, :, :3])
    return px


def sky_texture(w, h):
    """equirectangular upper half: v 0 = horizon, 1 = zenith; a warm sunset low in the sun's direction"""
    v = np.linspace(0, 1, h)[:, None]
    u = np.linspace(0, 1, w, endpoint=False)[None, :]
    horizon = np.array([1.00, 0.63, 0.36])
    mid = np.array([0.80, 0.56, 0.56])
    zen = np.array([0.22, 0.32, 0.58])
    t1 = np.clip(v / 0.35, 0, 1)[..., None]
    t2 = np.clip((v - 0.35) / 0.65, 0, 1)[..., None]
    col = np.where(v[..., None] < 0.35, horizon * (1 - t1) + mid * t1, mid * (1 - t2) + zen * t2)
    col = np.broadcast_to(col, (h, w, 3)).copy()
    # the sun's glow (azimuth of the stage light)
    az = (math.atan2(SUN.y, SUN.x) / (2 * math.pi)) % 1.0
    du = np.minimum(np.abs(u - az), 1 - np.abs(u - az))
    glow = np.exp(-(du / 0.08) ** 2) * np.exp(-(v / 0.25) ** 2)
    col += glow[..., None] * np.array([0.45, 0.30, 0.10])
    # clouds: stretched noise, thicker low in the sky, lit pink on the sun's side
    n = tileable_noise(w, h, 5, seed=3, fx=1, fy=2)
    dens = np.clip((n - 0.52) * 3.0, 0, 1) * np.clip(1.2 - v * 1.3, 0, 1)
    cloud = np.array([1.0, 0.78, 0.70]) * (0.75 + 0.35 * np.exp(-(du / 0.2) ** 2))[..., None]
    col = col * (1 - dens[..., None] * 0.75) + cloud * dens[..., None] * 0.75
    out = np.ones((h, w, 4), dtype=np.float32)
    out[:, :, :3] = np.clip(col, 0, 1)
    return out


log('textures')
TEX = {}
stone = cc0('Travertine009', 256, grade=(1.05, 0.97, 0.86), ao_amount=0.5)
TEX['travertine'] = make_image('travertine', stone)
TEX['bricks'] = make_image('bricks', cc0('Bricks076C', 256, grade=(1.08, 0.96, 0.84), ao_amount=0.7))
TEX['sand'] = make_image('sand', cc0('Ground080', 256, grade=(1.10, 0.98, 0.80), ao_amount=0.5, contrast=1.1))
def bronze_texture(size):
    m = cc0('Marble021', size)
    lum = m[:, :, :3].mean(2, keepdims=True)
    n = tileable_noise(size, size, 4, seed=9, fx=2, fy=2)[..., None]
    base = np.array([0.62, 0.42, 0.20]) * (0.75 + 0.45 * lum)
    patina = np.array([0.30, 0.42, 0.33])
    col = base * (1 - 0.35 * (n > 0.7)) + patina * 0.35 * (n > 0.7)
    out = np.ones((size, size, 4), dtype=np.float32)
    out[:, :, :3] = np.clip(col, 0, 1)
    return out


TEX['bronze'] = make_image('bronze', bronze_texture(128))
TEX['paving'] = make_image('paving', cc0('PavingStones151', 256, grade=(1.04, 0.95, 0.84), ao_amount=0.7))
TEX['marble'] = make_image('marble', cc0('Marble021', 128, grade=(1.0, 0.98, 0.95)))
TEX['planks'] = make_image('planks', cc0('Planks037A', 128, grade=(0.95, 0.80, 0.62), ao_amount=0.7))
TEX['fabric'] = make_image('fabric', cc0('Fabric085', 128, grade=(1.25, 0.55, 0.45), contrast=1.2))
TEX['arcade'] = make_image('arcade', arcade_texture(256, cc0('Travertine009', 256, grade=(1.02, 0.94, 0.84), ao_amount=0.5)))
TEX['sky'] = make_image('sky', sky_texture(256, 128))

MATS = {}


def material(name, double=False):
    if name in MATS:
        return MATS[name]
    m = bpy.data.materials.new('Legion_' + name)
    m.use_nodes = True
    nt = m.node_tree
    b = next(n for n in nt.nodes if n.type == 'BSDF_PRINCIPLED')
    t = nt.nodes.new('ShaderNodeTexImage')
    t.image = TEX[name]
    nt.links.new(t.outputs['Color'], b.inputs['Base Color'])
    b.inputs['Roughness'].default_value = 0.9
    m.use_backface_culling = not double
    MATS[name] = m
    return m


# ---------------------------------------------------------------- geometry

class Part:
    """a mesh being built: quads in game coordinates with UVs, for one (model, bone, material)"""

    def __init__(self, model, bone, mat, bake=True, double=False):
        self.model, self.bone, self.mat, self.bake, self.double = model, bone, mat, bake, double
        self.verts, self.uvs, self.faces = [], [], []

    def quad(self, ps, uvs, sub=(1, 1), want=None):
        """ps: 4 game points, uvs: 4 UVs; split into sub[0] x sub[1]. want: a game-space direction the face must
        face (the winding is reversed when it does not; the game culls the back of single-sided faces)"""
        if want is not None:
            e1 = B(*ps[1]) - B(*ps[0])
            e2 = B(*ps[2]) - B(*ps[0])
            if e1.cross(e2).dot(B(*want)) < 0:
                ps = [ps[1], ps[0], ps[3], ps[2]]
                uvs = [uvs[1], uvs[0], uvs[3], uvs[2]]
        su, sv = sub
        for i in range(su):
            for j in range(sv):
                a, b2 = i / su, (i + 1) / su
                c, d = j / sv, (j + 1) / sv

                def lerp(s, t):
                    p = [(1 - s) * (1 - t) * ps[0][k] + s * (1 - t) * ps[1][k] + s * t * ps[2][k] + (1 - s) * t * ps[3][k] for k in range(3)]
                    q = [(1 - s) * (1 - t) * uvs[0][k] + s * (1 - t) * uvs[1][k] + s * t * uvs[2][k] + (1 - s) * t * uvs[3][k] for k in range(2)]
                    return p, q
                base = len(self.verts)
                for s, t in ((a, c), (b2, c), (b2, d), (a, d)):
                    p, q = lerp(s, t)
                    self.verts.append(B(*p))
                    self.uvs.append(q)
                self.faces.append((base, base + 1, base + 2, base + 3))

    def build(self, name):
        me = bpy.data.meshes.new(name)
        me.from_pydata(self.verts, [], self.faces)
        me.update()
        uvl = me.uv_layers.new(name='UVMap')
        for poly in me.polygons:
            for li in poly.loop_indices:
                u, v = self.uvs[me.loops[li].vertex_index]
                uvl.data[li].uv = (u, v)
        me.materials.append(material(self.mat, self.double))
        ob = bpy.data.objects.new(name, me)
        bpy.context.scene.collection.objects.link(ob)
        ob['model'], ob['bone'], ob['bake'] = self.model, self.bone, self.bake
        return ob


PARTS = {}


def part(model, bone, mat, bake=True, double=False):
    k = (model, bone, mat, double)
    if k not in PARTS:
        PARTS[k] = Part(model, bone, mat, bake, double)
    return PARTS[k]


def ring_point(r, t, n):
    """a superellipse |x/r|^n + |z/r|^n = 1 at angle t: n large = rounded square, n 2 = circle"""
    c, s = math.cos(t), math.sin(t)
    x = r * math.copysign(abs(c) ** (2.0 / n), c)
    z = r * math.copysign(abs(s) ** (2.0 / n), s)
    return x, z


def shape_n(r):
    return max(2.4, 10.0 - (r - 8.6) * 0.62)


SEG = 48                       # segments around


def ring(r, k):
    t = 2 * math.pi * k / SEG
    return ring_point(r, t, shape_n(r))


def perimeter_u(r, tile):
    """u along a ring: arc length / tile, rounded to whole tiles so the texture closes"""
    pts = [ring(r, k) for k in range(SEG + 1)]
    L = sum(math.dist(pts[k], pts[k + 1]) for k in range(SEG))
    n = max(1, round(L / tile))
    us, acc = [0.0], 0.0
    for k in range(SEG):
        acc += math.dist(pts[k], pts[k + 1])
        us.append(acc / L * n)
    return us


def wall_ring(p, r, y0, y1, tile_u, tile_v, inward=True, vsub=1):
    us = perimeter_u(r, tile_u)
    for k in range(SEG):
        (x0, z0), (x1, z1) = ring(r, k), ring(r, k + 1)
        a, b2 = (x0, y0, z0), (x1, y0, z1)
        quad = [a, b2, (x1, y1, z1), (x0, y1, z0)]
        uv = [(us[k], y0 / tile_v), (us[k + 1], y0 / tile_v), (us[k + 1], y1 / tile_v), (us[k], y1 / tile_v)]
        mx, mz = (x0 + x1) / 2, (z0 + z1) / 2
        w = (-mx, 0, -mz) if inward else (mx, 0, mz)
        p.quad(quad, uv, (1, vsub), want=w)


def flat_ring(p, r0, r1, y, tile, rsub=1, uv_world=True):
    """a horizontal band between rings r0 < r1 at height y, facing up"""
    for k in range(SEG):
        (a0, c0), (a1, c1) = ring(r0, k), ring(r0, k + 1)
        (b0, d0), (b1, d1) = ring(r1, k), ring(r1, k + 1)
        quad = [(a1, y, c1), (a0, y, c0), (b0, y, d0), (b1, y, d1)]
        uv = [(q[0] / tile, q[2] / tile) for q in quad]
        p.quad(quad, uv, (1, rsub), want=(0, 1, 0))


def box(p, x0, x1, y0, y1, z0, z1, tile, sides=('n', 's', 'e', 'w', 't'), sub=1):
    """an axis-aligned box's outward faces (no bottom)"""
    def uvs(q, a, b2):
        return [(pt[a] / tile, pt[b2] / tile) for pt in q]
    if 's' in sides:   # -z face
        q = [(x1, y0, z0), (x0, y0, z0), (x0, y1, z0), (x1, y1, z0)]
        p.quad(q, [(-pt[0] / tile, pt[1] / tile) for pt in q], (sub, 1), want=(0, 0, -1))
    if 'n' in sides:   # +z face
        q = [(x0, y0, z1), (x1, y0, z1), (x1, y1, z1), (x0, y1, z1)]
        p.quad(q, uvs(q, 0, 1), (sub, 1), want=(0, 0, 1))
    if 'e' in sides:   # +x
        q = [(x1, y0, z1), (x1, y0, z0), (x1, y1, z0), (x1, y1, z1)]
        p.quad(q, [(-pt[2] / tile, pt[1] / tile) for pt in q], (sub, 1), want=(1, 0, 0))
    if 'w' in sides:   # -x
        q = [(x0, y0, z0), (x0, y0, z1), (x0, y1, z1), (x0, y1, z0)]
        p.quad(q, uvs(q, 2, 1), (sub, 1), want=(-1, 0, 0))
    if 't' in sides:
        q = [(x0, y1, z1), (x1, y1, z1), (x1, y1, z0), (x0, y1, z0)]
        p.quad(q, uvs(q, 0, 2), (sub, sub), want=(0, 1, 0))


def column(p, x, z, y0, y1, rad, sides=8, facing=None):
    """an n-sided column; facing: (dx, dz) to keep only the half that faces that way (seen from the arena)"""
    for k in range(sides):
        a0, a1 = 2 * math.pi * k / sides, 2 * math.pi * (k + 1) / sides
        mid = (a0 + a1) / 2
        if facing and math.cos(mid) * facing[0] + math.sin(mid) * facing[1] < -0.2:
            continue
        p0 = (x + rad * math.cos(a0), z + rad * math.sin(a0))
        p1 = (x + rad * math.cos(a1), z + rad * math.sin(a1))
        q = [(p1[0], y0, p1[1]), (p0[0], y0, p0[1]), (p0[0], y1, p0[1]), (p1[0], y1, p1[1])]
        p.quad(q, [((k + 1) / sides, y0 / 2), (k / sides, y0 / 2), (k / sides, y1 / 2), ((k + 1) / sides, y1 / 2)], (1, 1),
               want=(math.cos(mid), 0, math.sin(mid)))


# ---- model 0: the arena
log('geometry: arena')
A, SKY = 6, 2                  # bones of model 0: the floor joint, the old sky's joint
floor = part(0, A, 'sand')
F = 9.4
NF = 18
for i in range(NF):
    for j in range(NF):
        x0, x1 = -F + 2 * F * i / NF, -F + 2 * F * (i + 1) / NF
        z0, z1 = -F + 2 * F * j / NF, -F + 2 * F * (j + 1) / NF
        q = [(x0, 0.0, z1), (x1, 0.0, z1), (x1, 0.0, z0), (x0, 0.0, z0)]
        floor.quad(q, [(pt[0] / 5.4, pt[2] / 5.4) for pt in q], want=(0, 1, 0))

POD = 8.6                       # the podium wall's face (the collision walls are at +-8)
POD_H = 2.0
wall_ring(part(0, A, 'bricks'), POD, -0.05, POD_H, 2.4, 2.4, vsub=2)
cap = part(0, A, 'marble')
wall_ring(cap, POD - 0.12, POD_H, POD_H + 0.22, 1.5, 1.5)
flat_ring(cap, POD - 0.12, POD + 0.75, POD_H + 0.22, 1.5)

# the gates: two wooden doors in the podium wall (north and south), framed in marble
for zs in (-1, 1):
    zf = zs * (POD - 0.04)
    gate = part(0, A, 'planks')
    q = [(-1.5, 0.0, zf), (1.5, 0.0, zf), (1.5, 1.75, zf), (-1.5, 1.75, zf)]
    gate.quad(q, [(0, 0), (2.2, 0), (2.2, 1.3), (0, 1.3)], (4, 2), want=(0, 0, -zs))
    fr = part(0, A, 'marble')
    zi = zs * (POD - 0.10)
    for x0, x1, y0, y1 in ((-1.8, -1.5, 0.0, 1.95), (1.5, 1.8, 0.0, 1.95), (-1.8, 1.8, 1.75, 1.95)):
        if zs < 0:
            box(fr, x0, x1, y0, y1, zi - 0.0, zi + 0.12, 1.0, sides=('n', 'e', 'w', 't'))
        else:
            box(fr, x0, x1, y0, y1, zi - 0.12, zi, 1.0, sides=('s', 'e', 'w', 't'))

# red banners hanging over the podium wall, between the gates
pban = part(0, A, 'fabric')
for k in range(12):
    t = 2 * math.pi * (k + 0.5) / 12
    if abs(math.cos(t)) < 0.35:      # leave the gates (north / south) clear
        continue
    x, z = ring_point(POD - 0.05, t, shape_n(POD))
    ln = math.hypot(x, z)
    tx, tz = -z / ln, x / ln
    w2, y1, y0 = 0.45, POD_H + 0.15, POD_H - 1.45
    q = [(x - tx * w2, y0, z - tz * w2), (x + tx * w2, y0, z + tz * w2), (x + tx * w2, y1, z + tz * w2), (x - tx * w2, y1, z - tz * w2)]
    pban.quad(q, [(0, 0), (0.7, 0), (0.7, 1.3), (0, 1.3)], (1, 1), want=(-x, 0, -z))
    bz = part(0, A, 'bronze')    # a bronze rod across the banner's top
    box(bz, min(q[0][0], q[1][0]) - 0.05, max(q[0][0], q[1][0]) + 0.05, y1 - 0.06, y1 + 0.02,
        min(q[0][2], q[1][2]) - 0.05, max(q[0][2], q[1][2]) + 0.05, 0.5, sides=('n', 's', 'e', 'w', 't'))

# the tiers: risers (facing the arena) and treads (facing up)
log('geometry: stands')
seats = part(0, A, 'travertine')
risers = part(0, A, 'travertine')
TIERS, R0, DR, DH = 9, POD + 0.75, 1.3, 0.62
for k in range(TIERS):
    r = R0 + k * DR
    y = POD_H + 0.22 + k * DH
    wall_ring(risers, r, y, y + DH, 2.4, 2.4)
    flat_ring(seats, r, r + DR, y + DH, 3.5)
R_TOP = R0 + TIERS * DR
Y_TOP = POD_H + 0.22 + TIERS * DH
# the colonnade on the top tier: half columns facing the arena, an entablature band
col = part(0, A, 'marble')
NCOL = 24
for k in range(NCOL):
    t = 2 * math.pi * (k + 0.5) / NCOL
    x, z = ring_point(R_TOP + 0.4, t, shape_n(R_TOP + 0.4))
    ln = math.hypot(x, z)
    column(col, x, z, Y_TOP, Y_TOP + 3.2, 0.32, sides=6, facing=(-x / ln, -z / ln))
wall_ring(part(0, A, 'travertine'), R_TOP + 0.15, Y_TOP + 3.2, Y_TOP + 3.9, 2.0, 2.0)
# the terrace and the upper seating, the arcade facade
R_UP0, Y_TERR = R_TOP + 4.0, Y_TOP
flat_ring(part(0, A, 'paving'), R_TOP, R_UP0, Y_TERR, 2.5, rsub=1)
upper = part(0, A, 'travertine')
for k in range(5):
    r = R_UP0 + k * 3.0
    y = Y_TERR + k * 1.0
    wall_ring(risers, r, y, y + 1.0, 2.4, 2.4)
    flat_ring(upper, r, r + 3.0, y + 1.0, 3.5)
R_FAC = R_UP0 + 15.0
Y_FAC = Y_TERR + 5 * 1.0
fac = part(0, A, 'arcade')
us = perimeter_u(R_FAC, 4.5)
STOREY = 5.2
for s in range(3):
    y0, y1 = Y_FAC + s * STOREY, Y_FAC + (s + 1) * STOREY
    for k in range(SEG):
        (x0, z0), (x1, z1) = ring(R_FAC, k), ring(R_FAC, k + 1)
        fac.quad([(x0, y0, z0), (x1, y0, z1), (x1, y1, z1), (x0, y1, z0)],
                 [(us[k], 0.0), (us[k + 1], 0.0), (us[k + 1], 1.0), (us[k], 1.0)], (1, 1), want=(-(x0 + x1), 0, -(z0 + z1)))
attic = part(0, A, 'travertine')
wall_ring(attic, R_FAC, Y_FAC + 3 * STOREY, Y_FAC + 3 * STOREY + 2.5, 2.4, 2.4)
# red banners on the facade's first storey and down the podium cap between the gates
ban = part(0, A, 'fabric')
NB = 16
for k in range(NB):
    t = 2 * math.pi * (k + 0.25) / NB
    x, z = ring_point(R_FAC - 0.3, t, shape_n(R_FAC))
    ln = math.hypot(x, z)
    tx, tz = -z / ln, x / ln
    w2 = 1.2
    y1 = Y_FAC + 2 * STOREY - 0.6
    y0 = y1 - 6.0
    q = [(x - tx * w2, y0, z - tz * w2), (x + tx * w2, y0, z + tz * w2), (x + tx * w2, y1, z + tz * w2), (x - tx * w2, y1, z - tz * w2)]
    ban.quad(q, [(0, 0), (1.5, 0), (1.5, 3.5), (0, 3.5)], (1, 1), want=(-x, 0, -z))
# the sky dome (unlit, white vertex colour): an upper hemisphere, slightly below the horizon too
sky = part(0, SKY, 'sky', bake=False)
RS, NU, NV = 600.0, 24, 7
for i in range(NU):
    for j in range(-1, NV):
        a0, a1 = 2 * math.pi * i / NU, 2 * math.pi * (i + 1) / NU
        e0, e1 = (math.pi / 2) * j / NV, (math.pi / 2) * (j + 1) / NV

        def sp(a, e):
            return (RS * math.cos(e) * math.cos(a), RS * math.sin(e) - 40.0, RS * math.cos(e) * math.sin(a))
        q = [sp(a0, e0), sp(a1, e0), sp(a1, e1), sp(a0, e1)]
        cx = sum(pt[0] for pt in q) / 4
        cy = sum(pt[1] for pt in q) / 4 + 40.0
        cz = sum(pt[2] for pt in q) / 4
        sky.quad(q, [(i / NU, max(0.0, j / NV)), ((i + 1) / NU, max(0.0, j / NV)),
                     ((i + 1) / NU, max(0.0, (j + 1) / NV)), (i / NU, max(0.0, (j + 1) / NV))], want=(-cx, -cy, -cz))

# ---- the obstacles (models 1, 5-9) on their collision boxes
log('geometry: obstacles')
coll = json.load(open(COLL))
STYLE = {1: 'marble', 5: 'travertine', 6: 'travertine', 7: 'travertine', 8: 'planks', 9: 'planks'}
for m in (1, 5, 6, 7, 8, 9):
    g = coll['groups'][m]
    body = part(m, 0, 'bricks' if STYLE[m] == 'travertine' else STYLE[m])
    trim = part(m, 0, 'marble' if STYLE[m] != 'planks' else 'planks')
    for qd in g['quads']:
        c = qd['corners']
        n = qd['normal']
        span = max(math.dist(c[0], c[1]), math.dist(c[1], c[2]))
        sub = max(1, round(span / 0.6))
        # the collision quads are wound so (c1 - c0) x (c2 - c0) points out; Blender wants counter-clockwise = out too
        ys = [p[1] for p in c]
        if abs(n[1]) > 0.9:          # top: the coping (slightly larger, thicker)
            xs, zs = [p[0] for p in c], [p[2] for p in c]
            y = max(ys)
            box(trim, min(xs) - 0.06, max(xs) + 0.06, y - 0.02, y + 0.12, min(zs) - 0.06, max(zs) + 0.06, 0.8, sub=sub)
            continue
        horiz = [(p[0] if abs(n[0]) < 0.5 else p[2]) for p in c]
        uv = [(h / 1.2 * (1 if (n[2] > 0.5 or n[0] < -0.5) else -1), p[1] / 1.2) for h, p in zip(horiz, c)]
        body.quad([tuple(p) for p in c], uv, (sub, 2), want=tuple(n))
    if STYLE[m] == 'planks':        # bronze bands: thin boxes around the barricade
        xs = [p[0] for qd in g['quads'] for p in qd['corners']]
        zs = [p[2] for qd in g['quads'] for p in qd['corners']]
        band = part(m, 0, 'bronze')
        for yb in (0.25, 0.85):
            box(band, min(xs) - 0.03, max(xs) + 0.03, yb, yb + 0.1, min(zs) - 0.03, max(zs) + 0.03, 0.5, sides=('n', 's', 'e', 'w'))

for k, pt in PARTS.items():
    log('  part %s: %d triangles' % (k, 2 * len(pt.faces)))
objs = [pt.build('Legion_%d_%d_%s%s' % (k[0], k[1], k[2], '_2s' if k[3] else '')) for k, pt in PARTS.items()]
log('built %d meshes, %d faces' % (len(objs), sum(len(o.data.polygons) for o in objs)))

# ---------------------------------------------------------------- baked vertex lighting
log('lighting')
dg = bpy.context.evaluated_depsgraph_get()
occ_objs = [o for o in objs if o['bake']]
verts, polys = [], []
for o in occ_objs:
    base = len(verts)
    verts += [o.matrix_world @ v.co for v in o.data.vertices]
    polys += [[base + i for i in p.vertices] for p in o.data.polygons]
tree = BVHTree.FromPolygons(verts, polys)
rng = random.Random(5)
DIRS = []
for i in range(14):     # cosine-weighted hemisphere samples (about +Z), turned per vertex
    u1, u2 = (i + 0.5) / 14, rng.random()
    r = math.sqrt(u1)
    DIRS.append(Vector((r * math.cos(2 * math.pi * u2), r * math.sin(2 * math.pi * u2), math.sqrt(1 - u1))))


def basis(n):
    a = Vector((1, 0, 0)) if abs(n.x) < 0.9 else Vector((0, 1, 0))
    t = n.cross(a).normalized()
    return t, n.cross(t)


for o in objs:
    me = o.data
    ca = me.color_attributes.new(name='Col', type='FLOAT_COLOR', domain='POINT')
    if not o['bake']:
        for d in ca.data:
            d.color = (1, 1, 1, 1)
        me.color_attributes.active_color = ca
        continue
    me.update()
    nrm = [v.normal.copy() for v in me.vertices]
    for poly in me.polygons:     # the face normal: every vertex here belongs to one face
        for vi in poly.vertices:
            nrm[vi] = poly.normal.copy()
    for vi, v in enumerate(me.vertices):
        n = nrm[vi]
        p = o.matrix_world @ v.co + n * 0.03
        t, b = basis(n)
        free = 0
        for d in DIRS:
            w = t * d.x + b * d.y + n * d.z
            hit = tree.ray_cast(p, w, 3.5)
            if hit[0] is None:
                free += 1
        ao = free / len(DIRS)
        lam = max(0.0, n.dot(SUN))
        sh = 1.0
        if lam > 0 and tree.ray_cast(p, SUN, 400.0)[0] is not None:
            sh = 0.0
        c = SKY_COL * (0.30 + 0.70 * ao) * 0.95 + SUN_COL * lam * sh * 0.80
        c = np.clip(c * 1.05, 0, 1)
        ca.data[vi].color = (float(c[0]), float(c[1]), float(c[2]), 1.0)
    me.color_attributes.active_color = ca
log('lighting done')

# ---------------------------------------------------------------- export per model
keep = {}
for m in range(10):
    src = os.path.join(EXPORT, f'scene_data_{m}.glb')
    mine = [o for o in objs if o['model'] == m]
    before = set(bpy.data.objects)
    bpy.ops.import_scene.gltf(filepath=src)
    new = [o for o in bpy.data.objects if o not in before]
    arm = next(o for o in new if o.type == 'ARMATURE')
    tmeshes = [o for o in new if o.type == 'MESH' and o.visible_get()]
    if m in (2, 3, 4):
        # the breakable pods: bronze shells, gold shards (kept meshes; colour / texture edits only)
        for o in tmeshes:
            for mat in o.data.materials:
                if mat is None or not mat.use_nodes:
                    continue
                b = next((n for n in mat.node_tree.nodes if n.type == 'BSDF_PRINCIPLED'), None)
                timg = [n for n in mat.node_tree.nodes if n.type == 'TEX_IMAGE' and n.image]
                if timg:
                    img = timg[0].image
                    if img.get('legion_done'):
                        continue
                    px = np.array(img.pixels[:], dtype=np.float32).reshape(-1, 4)
                    lum = px[:, :3].mean(1, keepdims=True)
                    px[:, :3] = np.clip(lum * np.array([1.25, 0.95, 0.45]) + 0.05, 0, 1)
                    img.pixels[:] = px.ravel()
                    img['legion_done'] = True
                elif b is not None:
                    bc = list(b.inputs['Base Color'].default_value)
                    lum = (bc[0] + bc[1] + bc[2]) / 3
                    b.inputs['Base Color'].default_value = (0.55 + 0.4 * lum, 0.36 + 0.3 * lum, 0.14 + 0.15 * lum, bc[3])
    else:
        for o in tmeshes:            # --replace: the template meshes go
            o.hide_set(True)
            o.hide_render = True
    for o in mine:
        o.parent = arm
        vg = o.vertex_groups.new(name=f'JOBJ_{o["bone"]}')
        vg.add(list(range(len(o.data.vertices))), 1.0, 'REPLACE')
        mod = o.modifiers.new('Armature', 'ARMATURE')
        mod.object = arm
    others = [o for o in objs if o['model'] != m]
    for o in others:
        o.hide_set(True)
    for o in mine:
        o.hide_set(False)
    out = os.path.join(OUT, f'scene_data_{m}.glb')
    common.save(out, export_vertex_color='ACTIVE')
    keep[m] = len(mine)
    for o in mine:
        o.modifiers.clear()
        o.parent = None
        o.vertex_groups.clear()
    for o in new:
        bpy.data.objects.remove(o, do_unlink=True)
log('exported: ' + ', '.join(f'model {m}: {n} new meshes' for m, n in keep.items()))
