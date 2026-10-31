"""Ray 01 as a Roman legionnaire, version 2: a complete replacement body built procedurally on Ray 01's skeleton
(plan.md step 73, docs/MODDING.md "Blender loop"). Nothing of Ray 01's meshes, materials or textures is kept.

usage: blender -b --factory-startup --python tools/blender/legionnaire_v2.py -- <in.glb> <out.glb> <roles.json> [template]

<in.glb>      the robo exported by HSDRawViewer --export (bones JOBJ_n; Ray 01's meshes give the sizes and the hand /
              foot placement, then they are deleted)
<roles.json>  `python -I tools/robo_joints.py roles unpacked/chara/<entry>.bin` (the 19 role bones)
template      optional: keep the template skeleton exactly (no new bones; the step 70 path, no roles file needed)

Writes next to <out.glb>:
  <out>.args               the HSDRawViewer --import options (model_loop passes them): --replace, and without
                           "template" --skeleton glb --roles <out>.roles_by_name.json
  <out>.roles_by_name.json the roles by bone name (the importer turns them into chara_NNN.roles.json numbers)
  <out>.pose.json          for tools/blender/render.py: moves the hand / foot parts from the IK effectors' bind
                           positions (where the game data keeps them) to where the game draws them
  <out>.preview.png        a render of the model as built (hands and feet in place)

Skeleton ("glb" mode, the default): Ray 01's 55 joints with their transforms (the IK chains, their lengths and the
role bones stay as the game's motions expect) plus 3 new bones: Crest (on the head), Scutum (on the left forearm) and
Gladius (in the right hand). A roles file is emitted for the new numbering.

Parts (bone in brackets; "env" = smooth skinned over several bones):
  head: skin head with painted eyes, brows and mouth, nose [head]; bronze helmet: dome, brow band, neck guard,
        two hinged cheek guards, crest holder [head]; red horse-hair crest [Crest]
  neck: skin neck + red scarf (focale) [env upper body / head]
  torso: red tunic core [env lower / upper body]; iron lorica segmentata: 6 girth hoops, 2 breast plates,
        shoulder lames (3 per side) [upper body, upper arms]
  waist: leather belt with gold studs and a 4-strap apron [lower body]; red tunic skirt [env hips / thighs];
        leather pteruges strips [env hips / thighs]
  arms: skin arms [env shoulder / upper / fore]; red sleeves [env]; leather bracers [forearms]; fists [hands]
  legs: skin legs [env hips / thigh / shin]; bronze greaves with knee caps [shins]; sandals (caligae): soles,
        uppers, ankle straps [feet, toes, shins]
  scutum: curved red shield with gold rim, gold boss and painted gold wings / lightning [Scutum]
  gladius: steel blade, gold guard and pommel, leather grip [Gladius]
Materials (each one baked image, power of two): Iron 128, Bronze 128, Gold 64, Red 128, Leather 128, Skin 256,
Scutum 128 - procedural shaders baked with Cycles (base colour x ambient occlusion).
"""
import json
import math
import os
import sys

import bmesh
import bpy
import numpy as np
from mathutils import Matrix, Vector

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

UP = Vector((0, 0, 1))
FWD = Vector((0, 1, 0))     # Ray 01 faces Blender +Y (glTF -Z); its right arm (arm_b) is +X
RIGHT = Vector((1, 0, 0))

# material: (image size, metallic, roughness, double-sided)
MATS = {
    'Iron': (128, 0.85, 0.35, False),
    'Bronze': (128, 0.9, 0.3, False),
    'Gold': (64, 1.0, 0.25, False),
    'Red': (128, 0.0, 0.8, True),
    'Leather': (128, 0.0, 0.7, True),
    'Skin': (256, 0.0, 0.6, False),
    'Scutum': (128, 0.2, 0.5, False),
}

parts = []          # (object, material name)
UV_BOOST = {}       # material -> (test(object, bmesh face, deform layer), scale): set by main()
TEX_DUMP = None     # set by main(): <out>, the baked images go to <out>.tex_<material>.png
log = lambda *a: print('legionnaire_v2:', *a)


# ---------------------------------------------------------------- geometry helpers

def obj_from_bm(name, bm, mat, arm, outward=None):
    """A mesh object from a bmesh, parented to the armature with an Armature modifier. Normals: recomputed outside
    (closed or nearly closed shapes) or, for flat strips, turned to face `outward`."""
    if outward is None:
        bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
    else:
        bm.normal_update()
        for f in bm.faces:
            if f.normal.dot(outward) < 0:
                f.normal_flip()
    me = bpy.data.meshes.new(name)
    bm.to_mesh(me)
    bm.free()
    ob = bpy.data.objects.new(name, me)
    bpy.context.scene.collection.objects.link(ob)
    ob.parent = arm
    ob.matrix_parent_inverse = arm.matrix_world.inverted()
    ob.matrix_world = Matrix.Identity(4)
    mod = ob.modifiers.new('Armature', 'ARMATURE')
    mod.object = arm
    parts.append((ob, mat))
    return ob


def rigid(ob, bone):
    vg = ob.vertex_groups.new(name=bone)
    vg.add(list(range(len(ob.data.vertices))), 1.0, 'REPLACE')
    return ob


def weighted(ob, fn):
    """fn(world position) -> {bone: weight}"""
    groups = {}
    for v in ob.data.vertices:
        for b, w in fn(ob.matrix_world @ v.co).items():
            if w <= 1e-3:
                continue
            if b not in groups:
                groups[b] = ob.vertex_groups.new(name=b)
            groups[b].add([v.index], w, 'REPLACE')
    return ob


def frame_matrix(center, x, y, z):
    m = Matrix.Identity(4)
    for i, ax in enumerate((x, y, z)):
        for r in range(3):
            m[r][i] = ax[r]
    m.translation = center
    return m


def bm_box(center, axes, size, bevel=0.0):
    bm = bmesh.new()
    bmesh.ops.create_cube(bm, size=1.0)
    m = Matrix.Identity(4)
    for i in range(3):
        for r in range(3):
            m[r][i] = axes[i][r] * size[i]
    m.translation = center
    bmesh.ops.transform(bm, matrix=m, verts=bm.verts)
    if bevel > 0:
        bmesh.ops.bevel(bm, geom=list(bm.edges), offset=bevel, segments=1, affect='EDGES', profile=0.5)
    return bm


def bm_ellipsoid(center, radii, segs=12, rings=8, frame=None, zmin=-1.0, zmax=1.0):
    """Ellipsoid (radii along the frame's x, y, z); zmin / zmax cut it (fractions of the z radius) without caps."""
    bm = bmesh.new()
    rows = []
    for i in range(rings + 1):
        t = zmin + (zmax - zmin) * i / rings
        phi = math.asin(max(-1, min(1, t)))
        row = []
        for s in range(segs):
            a = 2 * math.pi * s / segs
            p = Vector((math.cos(phi) * math.cos(a), math.cos(phi) * math.sin(a), math.sin(phi)))
            row.append(p)
        rows.append(row)
    f = frame or Matrix.Identity(4)
    vrows = []
    for i, row in enumerate(rows):
        pole = abs(abs(zmin + (zmax - zmin) * i / rings) - 1) < 1e-6
        if pole:
            p = row[0]
            v = bm.verts.new(f @ Vector((p.x * radii[0], p.y * radii[1], p.z * radii[2])) + center)
            vrows.append([v] * segs)
        else:
            vrows.append([bm.verts.new(f @ Vector((p.x * radii[0], p.y * radii[1], p.z * radii[2])) + center) for p in row])
    for i in range(rings):
        for s in range(segs):
            a, b = vrows[i][s], vrows[i][(s + 1) % segs]
            c, d = vrows[i + 1][(s + 1) % segs], vrows[i + 1][s]
            vs = []
            for v in (a, b, c, d):
                if v not in vs:
                    vs.append(v)
            if len(vs) >= 3:
                bm.faces.new(vs)
    return bm


def bm_tube(points, radii, segs=10, caps=(True, True), squash=(1.0, 1.0), ref=None):
    """A tube along a polyline; radii per point (r or (rx, ry)); squash scales the ring's (side, ref) axes."""
    bm = bmesh.new()
    rings = []
    n = len(points)
    for i, p in enumerate(points):
        d = (points[min(i + 1, n - 1)] - points[max(i - 1, 0)]).normalized()
        r_ref = (ref or (FWD if abs(d.dot(FWD)) < 0.9 else UP))
        side = d.cross(r_ref).normalized()
        up = side.cross(d).normalized()
        r = radii[i]
        rx, ry = (r, r) if not isinstance(r, tuple) else r
        ring = []
        for s in range(segs):
            a = 2 * math.pi * s / segs
            ring.append(bm.verts.new(p + side * math.cos(a) * rx * squash[0] + up * math.sin(a) * ry * squash[1]))
        rings.append(ring)
    for i in range(n - 1):
        for s in range(segs):
            bm.faces.new((rings[i][s], rings[i][(s + 1) % segs], rings[i + 1][(s + 1) % segs], rings[i + 1][s]))
    if caps[0]:
        bm.faces.new(list(reversed(rings[0])))
    if caps[1]:
        bm.faces.new(rings[-1])
    return bm


def bm_band(center, rx, ry, z0, z1, segs=16, flare=1.0, thick=0.0, front_only=None):
    """A girth hoop (open tube, elliptical) from z0 to z1, its bottom flared by `flare`; with thick > 0 the rims get
    a lip. front_only=(a0, a1): only the arc between the angles (radians, 0 = +X, counter-clockwise)."""
    bm = bmesh.new()
    if front_only:
        a0, a1 = front_only
        angs = [a0 + (a1 - a0) * s / segs for s in range(segs + 1)]
        closed = False
    else:
        angs = [2 * math.pi * s / segs for s in range(segs)]
        closed = True
    zs = [(z1, 1.0), (z0, flare)]
    if thick > 0:   # a lip turned in at the bottom edge (the visible one)
        zs = [(z1, 1.0), (z0, flare), (z0, flare - thick)]
    rows = []
    for z, k in zs:
        rows.append([bm.verts.new(Vector((center.x + math.cos(a) * rx * k, center.y + math.sin(a) * ry * k, z))) for a in angs])
    m = len(angs)
    for i in range(len(rows) - 1):
        for s in range(m if closed else m - 1):
            t = (s + 1) % m
            bm.faces.new((rows[i][s], rows[i][t], rows[i + 1][t], rows[i + 1][s]))
    return bm


def bm_strip(p0, p1, width, normal, segs=1, bend=0.0):
    """A flat strip from p0 to p1 (width across, facing `normal`), `segs` lengthwise, bent along `normal` by `bend`."""
    bm = bmesh.new()
    d = (p1 - p0)
    across = d.cross(normal).normalized() * width / 2
    rows = []
    for i in range(segs + 1):
        t = i / segs
        c = p0 + d * t + normal * (bend * math.sin(math.pi * t))
        rows.append((bm.verts.new(c - across), bm.verts.new(c + across)))
    for i in range(segs):
        bm.faces.new((rows[i][0], rows[i][1], rows[i + 1][1], rows[i + 1][0]))
    return bm


def merge(*bms):
    out = bmesh.new()
    for b in bms:
        me = bpy.data.meshes.new('tmp')
        b.to_mesh(me)
        b.free()
        out.from_mesh(me)
        bpy.data.meshes.remove(me)
    return out


def smoothstep(x):
    x = max(0.0, min(1.0, x))
    return x * x * (3 - 2 * x)


def chain(bones, bounds, blend):
    """Weights along a chain: bones[i] between bounds[i-1] and bounds[i] (scalar coordinates along the chain);
    blended over +-blend around each bound."""
    def w(s):
        out = {bones[0]: 1.0}
        for k, b in enumerate(bounds):
            t = smoothstep((s - b) / (2 * blend) + 0.5)
            if t <= 0:
                break
            prev = bones[k]
            out[prev] = out.get(prev, 0) * (1 - t)
            out[bones[k + 1]] = t
            if t >= 1:
                out = {bones[k + 1]: 1.0}
        tot = sum(out.values())
        return {b: x / tot for b, x in out.items() if x / tot > 0.01}
    return w


# ---------------------------------------------------------------- shading / baking

def node(nt, kind, **kw):
    n = nt.nodes.new(kind)
    for k, v in kw.items():
        if k.startswith('in_'):
            n.inputs[int(k[3:])].default_value = v
        else:
            setattr(n, k, v)
    return n


class Shader:
    """Tiny helper to build procedural base-colour node trees: colours mixed by masks computed from world position."""

    def __init__(self, mat):
        self.mat = mat
        mat.use_nodes = True
        nt = self.nt = mat.node_tree
        nt.nodes.clear()
        self.out = node(nt, 'ShaderNodeOutputMaterial')
        self.bsdf = node(nt, 'ShaderNodeBsdfPrincipled')
        nt.links.new(self.bsdf.outputs[0], self.out.inputs[0])
        self.geo = node(nt, 'ShaderNodeNewGeometry')

    def link(self, a, b):
        self.nt.links.new(a, b)

    def pos(self, frame_obj=None):
        if frame_obj is None:
            return self.geo.outputs['Position']
        tc = node(self.nt, 'ShaderNodeTexCoord', object=frame_obj)
        return tc.outputs['Object']

    def comp(self, vec, i):
        sep = node(self.nt, 'ShaderNodeSeparateXYZ')
        self.link(vec, sep.inputs[0])
        return sep.outputs[i]

    def math(self, op, a, b=None, clamp=False):
        n = node(self.nt, 'ShaderNodeMath', operation=op, use_clamp=clamp)
        for i, x in enumerate((a, b)):
            if x is None:
                continue
            if isinstance(x, (int, float)):
                n.inputs[i].default_value = x
            else:
                self.link(x, n.inputs[i])
        return n.outputs[0]

    def vmath(self, op, a, b):
        n = node(self.nt, 'ShaderNodeVectorMath', operation=op)
        for i, x in enumerate((a, b)):
            if isinstance(x, (tuple, list, Vector)):
                n.inputs[i].default_value = tuple(x)
            else:
                self.link(x, n.inputs[i])
        return n.outputs[1] if op in ('DOT_PRODUCT', 'DISTANCE', 'LENGTH') else n.outputs[0]

    def noise(self, scale, detail=4.0, vec=None):
        n = node(self.nt, 'ShaderNodeTexNoise')
        n.inputs['Scale'].default_value = scale
        n.inputs['Detail'].default_value = detail
        self.link(vec or self.pos(), n.inputs['Vector'])
        return n.outputs['Fac']

    def mix(self, fac, a, b):
        n = node(self.nt, 'ShaderNodeMix', data_type='RGBA')
        if isinstance(fac, (int, float)):
            n.inputs['Factor'].default_value = fac
        else:
            self.link(fac, n.inputs['Factor'])
        for sock, c in ((n.inputs[6], a), (n.inputs[7], b)):
            if isinstance(c, tuple):
                sock.default_value = c if len(c) == 4 else c + (1.0,)
            else:
                self.link(c, sock)
        return n.outputs[2]

    def band(self, x, lo, hi, soft=0.002):
        a = self.math('GREATER_THAN', x, lo)
        b = self.math('LESS_THAN', x, hi)
        return self.math('MULTIPLY', a, b)

    def final(self, colour, metallic, roughness):
        self.link(colour, self.bsdf.inputs['Base Color'])
        self.bsdf.inputs['Metallic'].default_value = 0.0      # baked colour only
        self.bsdf.inputs['Roughness'].default_value = roughness


def grime(sh, base, dark, scale=30.0, amount=0.35):
    return sh.mix(sh.math('MULTIPLY', sh.noise(scale), amount), base, dark)


def build_shaders(frames):
    """Procedural base-colour shaders, one per material."""
    out = {}
    for name in MATS:
        m = bpy.data.materials.new('Legion_' + name)
        sh = Shader(m)
        p = sh.pos()
        if name == 'Iron':
            c = grime(sh, (0.50, 0.52, 0.56), (0.24, 0.25, 0.28), 45.0, 0.6)
            scratch = sh.math('GREATER_THAN', sh.noise(140.0, 2.0), 0.68)
            c = sh.mix(sh.math('MULTIPLY', scratch, 0.5), c, (0.78, 0.80, 0.84))
        elif name == 'Bronze':
            c = grime(sh, (0.80, 0.52, 0.22), (0.45, 0.27, 0.10), 35.0, 0.6)
            c = sh.mix(sh.math('MULTIPLY', sh.math('GREATER_THAN', sh.noise(18.0), 0.62), 0.45), c, (0.30, 0.42, 0.30))
        elif name == 'Gold':
            c = grime(sh, (1.0, 0.78, 0.30), (0.70, 0.48, 0.12), 60.0, 0.5)
        elif name == 'Red':
            wave = node(sh.nt, 'ShaderNodeTexWave', wave_type='BANDS')
            wave.inputs['Scale'].default_value = 180.0
            sh.link(p, wave.inputs['Vector'])
            c = sh.mix(sh.math('MULTIPLY', wave.outputs['Fac'], 0.25), (0.66, 0.06, 0.05), (0.40, 0.02, 0.02))
            c = sh.mix(sh.math('MULTIPLY', sh.noise(25.0), 0.3), c, (0.35, 0.02, 0.02))
        elif name == 'Leather':
            c = grime(sh, (0.42, 0.25, 0.12), (0.20, 0.11, 0.05), 50.0, 0.7)
        elif name == 'Skin':
            c = grime(sh, (0.84, 0.60, 0.44), (0.66, 0.42, 0.30), 25.0, 0.35)
            f = frames['face']
            fp = sh.pos(f)
            x, y, z = sh.comp(fp, 0), sh.comp(fp, 1), sh.comp(fp, 2)
            ax = sh.math('ABSOLUTE', x)
            front = sh.math('GREATER_THAN', y, 0.0)
            # eyes: white almond, dark iris
            ex = sh.math('DIVIDE', sh.math('SUBTRACT', ax, 0.030), 0.017)
            ez = sh.math('DIVIDE', sh.math('SUBTRACT', z, 0.012), 0.009)
            eye = sh.math('LESS_THAN', sh.math('ADD', sh.math('POWER', ex, 2.0), sh.math('POWER', ez, 2.0)), 1.0)
            eye = sh.math('MULTIPLY', eye, front)
            c = sh.mix(eye, c, (0.92, 0.90, 0.86))
            ix = sh.math('DIVIDE', sh.math('SUBTRACT', ax, 0.028), 0.008)
            iz = sh.math('DIVIDE', sh.math('SUBTRACT', z, 0.011), 0.008)
            iris = sh.math('MULTIPLY', sh.math('LESS_THAN', sh.math('ADD', sh.math('POWER', ix, 2.0), sh.math('POWER', iz, 2.0)), 1.0), front)
            c = sh.mix(iris, c, (0.18, 0.10, 0.05))
            brow = sh.math('MULTIPLY', sh.math('MULTIPLY', sh.band(z, 0.027, 0.036), sh.band(ax, 0.012, 0.050)), front)
            c = sh.mix(brow, c, (0.20, 0.12, 0.07))
            mouth = sh.math('MULTIPLY', sh.math('MULTIPLY', sh.band(z, -0.034, -0.028), sh.band(ax, -1.0, 0.020)), front)
            c = sh.mix(mouth, c, (0.50, 0.20, 0.18))
            stubble = sh.math('MULTIPLY', sh.math('MULTIPLY', sh.band(z, -0.07, -0.02), front), 0.25)
            c = sh.mix(stubble, c, (0.45, 0.32, 0.26))
        elif name == 'Scutum':
            f = frames['scutum']
            fp = sh.pos(f)
            u, n_, v = sh.comp(fp, 0), sh.comp(fp, 1), sh.comp(fp, 2)
            au, av = sh.math('ABSOLUTE', u), sh.math('ABSOLUTE', v)
            c = grime(sh, (0.70, 0.06, 0.05), (0.42, 0.03, 0.03), 20.0, 0.45)
            gold = (1.0, 0.76, 0.26)
            hw, hh = frames['scutum_size']
            border = sh.math('MAXIMUM', sh.band(au, hw - 0.030, hw - 0.018), sh.band(av, hh - 0.030, hh - 0.018))
            inner = sh.math('MULTIPLY', sh.math('LESS_THAN', au, hw - 0.018), sh.math('LESS_THAN', av, hh - 0.018))
            c = sh.mix(sh.math('MULTIPLY', border, inner), c, gold)
            r = sh.math('SQRT', sh.math('ADD', sh.math('POWER', u, 2.0), sh.math('POWER', v, 2.0)))
            ring = sh.band(r, 0.075, 0.085)
            c = sh.mix(ring, c, gold)
            # wings: two feathered ellipses left and right of the boss
            wx = sh.math('DIVIDE', sh.math('SUBTRACT', au, 0.095), 0.055)
            wz = sh.math('DIVIDE', sh.math('SUBTRACT', v, sh.math('MULTIPLY', au, 0.35)), 0.030)
            wing = sh.math('LESS_THAN', sh.math('ADD', sh.math('POWER', wx, 2.0), sh.math('POWER', wz, 2.0)), 1.0)
            feathers = sh.math('GREATER_THAN', sh.math('SINE', sh.math('MULTIPLY', au, 160.0)), -0.55)
            c = sh.mix(sh.math('MULTIPLY', wing, feathers), c, gold)
            # lightning bolts up and down from the boss (zig-zag bars)
            zig = sh.math('MULTIPLY', sh.math('SINE', sh.math('MULTIPLY', v, 60.0)), 0.012)
            bolt = sh.math('MULTIPLY', sh.math('LESS_THAN', sh.math('ABSOLUTE', sh.math('SUBTRACT', u, zig)), 0.007),
                           sh.band(av, 0.095, hh - 0.045))
            c = sh.mix(bolt, c, gold)
            # back: leather-covered wood
            back = sh.math('LESS_THAN', sh.vmath('DOT_PRODUCT', sh.geo.outputs['Normal'], tuple(frames['scutum_normal'])), -0.2)
            c = sh.mix(back, c, (0.36, 0.22, 0.11))
        sh.final(c, MATS[name][1], MATS[name][2])
        out[name] = m
    return out


def select_only(objs, active):
    for o in bpy.context.view_layer.objects:
        o.select_set(False)
    for o in objs:
        o.select_set(True)
    bpy.context.view_layer.objects.active = active


def bake_all(mats):
    """Per material: join its parts, unwrap, bake base colour and AO, and swap in an image-textured material."""
    scene = bpy.context.scene
    scene.render.engine = 'CYCLES'
    scene.cycles.device = 'CPU'
    scene.cycles.samples = 16
    scene.render.bake.margin = 4
    world = bpy.data.worlds.new('BakeWorld')
    world.color = (1, 1, 1)
    scene.world = world
    joined = {}
    for name in MATS:
        objs = [o for o, m in parts if m == name]
        if not objs:
            continue
        mat = mats[name]
        for o in objs:
            o.data.materials.clear()
            o.data.materials.append(mat)
        select_only(objs, objs[0])
        if len(objs) > 1:
            bpy.ops.object.join()
        ob = bpy.context.view_layer.objects.active
        ob.name = f'Legion_{name}'
        ob.data.name = f'Legion_{name}'
        # merge coincident vertices of each part (bmesh built faces from separate rings)
        bm = bmesh.new()
        bm.from_mesh(ob.data)
        bmesh.ops.remove_doubles(bm, verts=bm.verts, dist=1e-5)
        bm.to_mesh(ob.data)
        bm.free()
        # shading: smooth, sharp over 40 degrees
        ob.data.shade_smooth()
        try:
            ob.data.set_sharp_from_angle(angle=math.radians(40))
        except AttributeError:
            pass
        bpy.ops.object.mode_set(mode='EDIT')
        bpy.ops.mesh.select_all(action='SELECT')
        bpy.ops.uv.smart_project(angle_limit=math.radians(55), island_margin=0.02, area_weight=0.0, scale_to_bounds=False)
        boost = UV_BOOST.get(name)
        if boost:
            # more texels where they show: scale the chosen faces' islands up, then pack all islands again
            test, k = boost
            bm = bmesh.from_edit_mesh(ob.data)
            uvl = bm.loops.layers.uv.active
            dl = bm.verts.layers.deform.active
            n = 0
            for f in bm.faces:
                if test(ob, f, dl):
                    n += 1
                    for lp in f.loops:
                        lp[uvl].uv *= k
            bmesh.update_edit_mesh(ob.data)
            bpy.ops.uv.select_all(action='SELECT')
            bpy.ops.uv.pack_islands(rotate=True, scale=True, margin=0.01)
            log(f'material {name}: {n} faces get {k}x the texels')
        bpy.ops.object.mode_set(mode='OBJECT')
        size, metallic, rough, two = MATS[name]
        img = bpy.data.images.new(f'Legion_{name}', size, size, alpha=False)
        ao = bpy.data.images.new(f'Legion_{name}_ao', size, size, alpha=False)
        nt = mat.node_tree
        tex = node(nt, 'ShaderNodeTexImage', image=img)
        nt.nodes.active = tex
        bpy.ops.object.bake(type='DIFFUSE', pass_filter={'COLOR'}, margin=4, use_clear=True)
        tex.image = ao
        bpy.ops.object.bake(type='AO', margin=4, use_clear=True)
        px = np.array(img.pixels[:], dtype=np.float32).reshape(-1, 4)
        aop = np.array(ao.pixels[:], dtype=np.float32).reshape(-1, 4)[:, :1]
        px[:, :3] = np.clip(px[:, :3] * (0.45 + 0.55 * aop), 0, 1)
        px[:, 3] = 1.0
        img.pixels[:] = px.ravel()
        img.pack()
        bpy.data.images.remove(ao)
        if TEX_DUMP:
            img.save(filepath=f'{TEX_DUMP}.tex_{name}.png')
        # the exported material: image -> base colour
        nt.nodes.clear()
        out = node(nt, 'ShaderNodeOutputMaterial')
        bsdf = node(nt, 'ShaderNodeBsdfPrincipled')
        nt.links.new(bsdf.outputs[0], out.inputs[0])
        t = node(nt, 'ShaderNodeTexImage', image=img)
        nt.links.new(t.outputs['Color'], bsdf.inputs['Base Color'])
        bsdf.inputs['Metallic'].default_value = metallic
        bsdf.inputs['Roughness'].default_value = rough
        mat.use_backface_culling = not two
        joined[name] = ob
        log(f'material {name}: {len(objs)} parts joined, {len(ob.data.polygons)} faces, {size}x{size} baked')
    return joined


def triangulate(ob):
    bm = bmesh.new()
    bm.from_mesh(ob.data)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(ob.data)
    bm.free()
    return len(ob.data.polygons)


# ---------------------------------------------------------------- the model

def main(template_skeleton=None):
    a = common.args(3, __doc__, most=4)
    src, dst, roles_path = a[:3]
    if template_skeleton is None:
        template_skeleton = a[3] == 'template'
    roles = json.load(open(roles_path, encoding='utf-8'))['C']
    arm = common.load(src)
    bones = arm.data.bones
    old = common.model_meshes()

    def bpos(j):
        return arm.matrix_world @ bones[f'JOBJ_{j}'].head_local

    mesh_joints = {int(o.name.split('_')[1]) for o in old if o.name.startswith('Joint_')}

    def holder(j):
        """The joint carrying Ray 01's mesh for role joint j (itself or the first descendant with one)."""
        todo = [bones[f'JOBJ_{j}']]
        while todo:
            b = todo.pop(0)
            k = int(b.name.split('_')[1])
            if k in mesh_joints:
                return k
            todo += sorted((c for c in b.children if c.name.startswith('JOBJ_')), key=lambda c: int(c.name.split('_')[1]))
        return j

    boxes = {}
    for k in mesh_joints:
        pts = [o.matrix_world @ v.co for o in old if o.name.startswith(f'Joint_{k}_') for v in o.data.vertices]
        boxes[k] = (Vector((min(p.x for p in pts), min(p.y for p in pts), min(p.z for p in pts))),
                    Vector((max(p.x for p in pts), max(p.y for p in pts), max(p.z for p in pts))))

    def bbox(j):
        """World bounding box of Ray 01's meshes on joint j (measured before they are deleted)."""
        return boxes[j]

    J = lambda role: f'JOBJ_{roles[role]}'
    H = lambda role: f'JOBJ_{holder(roles[role])}'
    # sides: arm_a / leg_a are on -X = the robo's left (it faces +Y; checked in game: step 73)
    side_of = {'a': -1.0, 'b': 1.0}
    left_arm = 'arm_a' if bpos(roles['arm_a_fore']).x < 0 else 'arm_b'
    right_arm = 'arm_b' if left_arm == 'arm_a' else 'arm_a'
    log(f'faces +Y; left arm (scutum) = {left_arm}, right arm (gladius) = {right_arm}; '
        f'skeleton: {"template" if template_skeleton else "glb (+ Crest, Scutum, Gladius bones)"}')

    # ---- measurements from Ray 01 (Blender world, Z up)
    head_lo, head_hi = bbox(holder(roles['head']))
    hip = {s: bpos(roles[f'leg_{s}_thigh']) for s in 'ab'}
    knee = {s: bpos(roles[f'leg_{s}_shin']) for s in 'ab'}
    shoulder = {s: bpos(roles[f'arm_{s}_upper']) for s in 'ab'}
    elbow = {s: bpos(roles[f'arm_{s}_fore']) for s in 'ab'}
    fore_lo, fore_hi = {}, {}
    for s in 'ab':
        fore_lo[s], fore_hi[s] = bbox(holder(roles[f'arm_{s}_fore']))
    shin_bottom = min(bbox(holder(roles[f'leg_{s}_shin']))[0].z for s in 'ab')
    ground = shin_bottom - 0.08          # the shins end in the feet's ankle sockets
    shin_lo = {s: bbox(holder(roles[f'leg_{s}_shin']))[0] for s in 'ab'}
    wrist = {s: Vector((side_of[s] * max(abs(fore_lo[s].x), abs(fore_hi[s].x)), elbow[s].y, elbow[s].z)) for s in 'ab'}
    head_c = Vector((0, (head_lo.y + head_hi.y) / 2 + 0.01, head_lo.z + 0.10))
    ankle_z = ground + 0.085
    log(f'head centre {tuple(round(x, 3) for x in head_c)}, ground {ground:.3f}, wrists x {wrist["a"].x:.3f} / {wrist["b"].x:.3f}')

    # ---- delete Ray 01's meshes, materials and images: nothing of it is exported
    for o in old:
        bpy.data.objects.remove(o, do_unlink=True)
    for m in list(bpy.data.materials):
        bpy.data.materials.remove(m)
    for i in list(bpy.data.images):
        bpy.data.images.remove(i)

    # ---- new bones (glb skeleton)
    new_bones = {}
    if not template_skeleton:
        bpy.context.view_layer.objects.active = arm
        bpy.ops.object.mode_set(mode='EDIT')
        eb = arm.data.edit_bones
        inv = arm.matrix_world.inverted()

        def add_bone(name, parent, at):
            b = eb.new(name)
            b.head = inv @ at
            b.tail = inv @ (at + Vector((0, 0, 0.05)))
            b.parent = eb[parent]
            b.use_connect = False
            b.use_deform = True
            new_bones[name] = parent
        add_bone('Crest', J('head'), Vector((0, head_c.y, head_hi.z)))
        add_bone('Scutum', J(f'{left_arm}_fore'), (elbow[left_arm[-1]] + wrist[left_arm[-1]]) / 2 + FWD * 0.07)
        # the gladius bone sits in the right hand's bind (effector) frame, at the hand's game-data position
        add_bone('Gladius', f'JOBJ_{holder(roles[right_arm + "_hand"]) - 1}', Vector((0, 0, 0)))  # placed below
        bpy.ops.object.mode_set(mode='OBJECT')
    crest_bone = 'Crest' if not template_skeleton else H('head')
    scutum_bone = 'Scutum' if not template_skeleton else H(f'{left_arm}_fore')

    # ---- hands and feet live on IK effector chains: their game data keeps them at odd bind positions (Ray 01's
    # hands float beside the body, its feet stand turned 90 degrees). Build them where they belong, then move them
    # the way Ray 01's own hand / foot meshes are moved (`effector` maps "drawn" -> "game data").
    effector = {}
    for s in 'ab':
        hl, hh_ = bbox(holder(roles[f'arm_{s}_hand']))
        ray_hand = (hl + hh_) / 2
        drawn = wrist[s] + Vector((side_of[s] * 0.045, 0, 0))
        effector[f'arm_{s}'] = Matrix.Translation(ray_hand - drawn)
        al, ah = bbox(holder(roles[f'leg_{s}_ankle']))
        tl, th = bbox(holder(roles[f'leg_{s}_foot']))
        lo = Vector((min(al.x, tl.x), min(al.y, tl.y), min(al.z, tl.z)))
        hi = Vector((max(ah.x, th.x), max(ah.y, th.y), max(ah.z, th.z)))
        # Ray 01's feet in the game data (seen in a render of its export): toe towards the toe joint's mesh, sole
        # facing -Y (the foot lies on its back, toe sideways)
        d = (tl + th) / 2 - (al + ah) / 2
        i = max(range(3), key=lambda k: abs(d[k]))
        toe_b = Vector([0, 0, 0]); toe_b[i] = 1 if d[i] > 0 else -1
        up_b = Vector((0, 1, 0))
        x_b = toe_b.cross(up_b)
        heel_b = Vector([((hi[k] if toe_b[k] < 0 else lo[k]) if toe_b[k] else
                          (lo[k] if up_b[k] > 0 else hi[k]) if up_b[k] else (lo[k] + hi[k]) / 2) for k in range(3)])
        heel_d = Vector((hip[s].x, 0.04 - 0.105, ground))
        rot = Matrix.Identity(4)
        for c, ax in enumerate((x_b, toe_b, up_b)):
            for r in range(3):
                rot[r][c] = ax[r]
        effector[f'leg_{s}'] = Matrix.Translation(heel_b) @ rot @ Matrix.Translation(-heel_d)
        log(f'leg_{s} foot in the game data: toe {tuple(toe_b)}, sole -Y, heel at {tuple(round(x, 3) for x in heel_b)}')
    if not template_skeleton:
        # place the Gladius bone now that the hand mapping is known
        bpy.context.view_layer.objects.active = arm
        bpy.ops.object.mode_set(mode='EDIT')
        eb = arm.data.edit_bones
        s = right_arm[-1]
        grip = effector[right_arm] @ (wrist[s] + Vector((side_of[s] * 0.045, 0, 0)))
        eb['Gladius'].head = arm.matrix_world.inverted() @ grip
        eb['Gladius'].tail = arm.matrix_world.inverted() @ (grip + Vector((0, 0, 0.05)))
        eb['Gladius'].parent = eb[f'JOBJ_{holder(roles[right_arm + "_hand"])}']
        new_bones['Gladius'] = eb['Gladius'].parent.name
        bpy.ops.object.mode_set(mode='OBJECT')
    gladius_bone = 'Gladius' if not template_skeleton else H(f'{right_arm}_hand')

    effector_parts = []    # (object, effector key): moved after the preview render

    # ================================================================ HEAD
    hc = head_c
    face = bpy.data.objects.new('FaceFrame', None)
    bpy.context.scene.collection.objects.link(face)
    face.matrix_world = Matrix.Translation(hc + Vector((0, 0.0, 0)))
    head_r = (0.062, 0.070, 0.080)
    ob = obj_from_bm('Head', bm_ellipsoid(hc, head_r, 14, 8), 'Skin', arm)
    rigid(ob, H('head'))
    # nose and ears
    nose = bm_tube([hc + Vector((0, head_r[1] - 0.01, 0.012)), hc + Vector((0, head_r[1] + 0.014, -0.018))], [0.008, 0.011], 6)
    ob = obj_from_bm('Nose', nose, 'Skin', arm)
    rigid(ob, H('head'))
    for sx in (-1, 1):
        ear = bm_ellipsoid(hc + Vector((sx * head_r[0], 0.0, 0.0)), (0.008, 0.016, 0.022), 5, 3)
        rigid(obj_from_bm(f'Ear{sx}', ear, 'Skin', arm), H('head'))
    # chin / jaw shape
    jaw = bm_ellipsoid(hc + Vector((0, 0.025, -0.048)), (0.040, 0.040, 0.035), 8, 4)
    rigid(obj_from_bm('Jaw', jaw, 'Skin', arm), H('head'))

    # helmet (Imperial Gallic): dome, brow band, neck guard, cheek guards, crest holder
    dome_c = hc + Vector((0, -0.004, 0.006))
    rz = head_r[2] + 0.010
    dome = bm_ellipsoid(dome_c, (head_r[0] + 0.010, head_r[1] + 0.012, rz), 16, 6, zmin=0.40, zmax=1.0)
    rigid(obj_from_bm('HelmetDome', dome, 'Bronze', arm), H('head'))
    rim_z = dome_c.z + 0.40 * rz          # the helmet's lower edge, above the brows
    k = math.cos(math.asin(0.40))
    brow = bm_band(dome_c, (head_r[0] + 0.014) * k, (head_r[1] + 0.016) * k, rim_z - 0.004, rim_z + 0.010, segs=12, thick=0.06)
    rigid(obj_from_bm('HelmetBrow', brow, 'Gold', arm), H('head'))
    # neck guard: a flared skirt at the back
    ng = bm_band(dome_c, (head_r[0] + 0.012) * k, (head_r[1] + 0.014) * k, dome_c.z - 0.050, rim_z + 0.002, segs=10, flare=1.45,
                 front_only=(math.radians(195), math.radians(345)))
    rigid(obj_from_bm('HelmetNeckGuard', ng, 'Bronze', arm), H('head'))
    # cheek guards: curved plates on the sides, the face stays open
    for sx in (-1, 1):
        a0, a1 = (math.radians(-25), math.radians(48)) if sx > 0 else (math.radians(132), math.radians(205))
        cg = bm_band(dome_c + Vector((0, 0.004, 0)), head_r[0] + 0.013, head_r[1] + 0.012, dome_c.z - 0.075, rim_z + 0.002,
                     segs=4, flare=0.88, front_only=(a0, a1))
        rigid(obj_from_bm(f'HelmetCheek{sx}', cg, 'Bronze', arm), H('head'))
    top = Vector((0, dome_c.y, dome_c.z + head_r[2] + 0.010))
    holder_bm = bm_box(top + Vector((0, 0, 0.008)), (RIGHT, FWD, UP), (0.016, 0.10, 0.016))
    rigid(obj_from_bm('CrestHolder', holder_bm, 'Gold', arm), H('head'))
    # crest: a fan of horse hair, front to back, as a thick curved blade
    crest = bmesh.new()
    rows = []
    n = 9
    for i in range(n + 1):
        t = i / n
        a = math.radians(-70 + 150 * t)        # front (+Y) to back, over the top
        r0, r1 = 0.010, 0.085 * (0.75 + 0.25 * math.sin(math.pi * t))
        base = top + Vector((0, math.sin(a) * 0.055 - 0.004, 0.010 + 0.004 * math.cos(a)))
        tipd = Vector((0, math.sin(a), math.cos(a) * 0.85 + 0.25)).normalized()
        w = 0.016 * (0.6 + 0.4 * math.sin(math.pi * t))
        p0, p1 = base, base + tipd * r1
        rows.append([crest.verts.new(p0 + RIGHT * w * 0.6), crest.verts.new(p1 + RIGHT * w),
                     crest.verts.new(p1 - RIGHT * w), crest.verts.new(p0 - RIGHT * w * 0.6)])
    for i in range(n):
        for k in range(4):
            crest.faces.new((rows[i][k], rows[i][(k + 1) % 4], rows[i + 1][(k + 1) % 4], rows[i + 1][k]))
    crest.faces.new(rows[0])
    crest.faces.new(list(reversed(rows[-1])))
    rigid(obj_from_bm('Crest', crest, 'Red', arm), crest_bone)

    # ================================================================ NECK + TORSO
    neck_bot, neck_top = Vector((0, 0.0, 0.215)), Vector((0, 0.008, hc.z - 0.045))
    neck = bm_tube([neck_bot, (neck_bot + neck_top) / 2, neck_top], [0.040, 0.036, 0.034], 10, caps=(False, False))
    nw = chain([J('upper_body'), J('head')], [neck_bot.z + 0.6 * (neck_top.z - neck_bot.z)], 0.025)
    weighted(obj_from_bm('Neck', neck, 'Skin', arm), lambda p: nw(p.z))
    scarf = bm_tube([neck_bot + Vector((0, 0, 0.008)), neck_bot + Vector((0, 0.004, 0.036))], [(0.058, 0.052), (0.047, 0.043)], 12, caps=(False, False))
    weighted(obj_from_bm('Scarf', scarf, 'Red', arm), lambda p: nw(p.z))
    # tunic core: hips to chest, smooth over lower / upper body
    waist_z = 0.035
    tw = chain([J('lower_body'), J('upper_body')], [waist_z], 0.05)
    core_pts = [Vector((0, 0, z)) for z in (-0.10, -0.03, 0.04, 0.12, 0.20, 0.235)]
    core_r = [(0.125, 0.085), (0.120, 0.080), (0.112, 0.074), (0.125, 0.082), (0.118, 0.078), (0.075, 0.060)]
    core = bm_tube(core_pts, core_r, 14, caps=(True, True), ref=FWD)
    weighted(obj_from_bm('TunicCore', core, 'Red', arm), lambda p: tw(p.z))
    # lorica segmentata: girth hoops from the waist up (overlapping, each flaring a little at the bottom)
    hoops = [(0.020, 0.052, 0.132, 0.090), (0.046, 0.078, 0.131, 0.088), (0.072, 0.104, 0.134, 0.090),
             (0.098, 0.130, 0.138, 0.093), (0.124, 0.156, 0.141, 0.095), (0.150, 0.184, 0.140, 0.094)]
    for i, (z0, z1, rx, ry) in enumerate(hoops):
        hb_ = bm_band(Vector((0, 0.002, 0)), rx, ry, z0, z1, segs=14, flare=1.04, thick=0.05)
        rigid(obj_from_bm(f'LoricaHoop{i}', hb_, 'Iron', arm), H('upper_body'))
    # breast and back plates over the chest, with a gap for the neck
    for fy, nm in ((1, 'Breast'), (-1, 'Back')):
        pl = bm_band(Vector((0, 0.002, 0)), 0.128, 0.088, 0.180, 0.228, segs=8, flare=1.05, thick=0.05,
                     front_only=((math.radians(15), math.radians(165)) if fy > 0 else (math.radians(195), math.radians(345))))
        rigid(obj_from_bm(f'Lorica{nm}', pl, 'Iron', arm), H('upper_body'))
    # brass hinges / buckles on the breast plates
    for sx in (-1, 1):
        bk = bm_box(Vector((sx * 0.045, 0.093, 0.205)), (RIGHT, FWD, UP), (0.018, 0.008, 0.014))
        rigid(obj_from_bm(f'Buckle{sx}', bk, 'Gold', arm), H('upper_body'))
    # shoulder lames: 3 curved plates over each shoulder, following the upper arm
    for s in 'ab':
        sx = side_of[s]
        sh0 = shoulder[s]
        for i in range(3):
            # each lame wraps the top of the arm (front, top, back), stepping out along the arm and down
            c = Vector((sx * (0.125 + 0.040 * i), 0.0, sh0.z + 0.004 - 0.004 * i))
            r_in, r_out = 0.060 - 0.004 * i, 0.066 - 0.004 * i
            arc = bmesh.new()
            rows = []
            segs = 5
            for k in range(segs + 1):
                a = math.radians(-115 + 230 * k / segs)
                d = Vector((0, math.sin(a), math.cos(a)))
                rows.append([arc.verts.new(c + Vector((-sx * 0.026, 0, 0)) + d * r_in),
                             arc.verts.new(c + Vector((sx * 0.026, 0, 0)) + d * r_out)])
            for k in range(segs):
                arc.faces.new((rows[k][0], rows[k][1], rows[k + 1][1], rows[k + 1][0]))
            ob = obj_from_bm(f'ShoulderLame_{s}{i}', arc, 'Iron', arm, outward=UP)
            ob.modifiers.new('Solid', 'SOLIDIFY').thickness = 0.007
            rigid(ob, H(f'arm_{s}_upper'))

    # ================================================================ WAIST: belt, apron, skirt, pteruges
    belt = bm_band(Vector((0, 0.002, 0)), 0.130, 0.090, 0.000, 0.024, segs=16, flare=1.0, thick=0.05)
    rigid(obj_from_bm('Belt', belt, 'Leather', arm), H('lower_body'))
    for k in range(7):
        a = math.radians(30 + 20 * k)
        p = Vector((math.cos(a) * 0.134, 0.002 + math.sin(a) * 0.094, 0.012))
        stud = bm_ellipsoid(p, (0.008, 0.008, 0.008), 4, 2)
        rigid(obj_from_bm(f'BeltStud{k}', stud, 'Gold', arm), H('lower_body'))
    for k, x in enumerate((-0.036, -0.012, 0.012, 0.036)):
        st = bm_strip(Vector((x, 0.098, -0.002)), Vector((x, 0.108, -0.175)), 0.016, FWD, segs=2, bend=0.004)
        rigid(obj_from_bm(f'Apron{k}', st, 'Leather', arm, outward=FWD), H('lower_body'))
        tip = bm_box(Vector((x, 0.106, -0.170)), (RIGHT, FWD, UP), (0.018, 0.006, 0.016))
        rigid(obj_from_bm(f'ApronTip{k}', tip, 'Gold', arm), H('lower_body'))

    def hip_weights(p):
        """Skirt: hips at the top, the nearer thigh towards the hem (front / side), hips at the back."""
        t = smoothstep((0.0 - p.z) / 0.20)
        s = 'a' if p.x * side_of['a'] > 0 else 'b'
        side = min(1.0, abs(p.x) / 0.08) * (0.4 + 0.6 * max(0.0, p.y + 0.06) / 0.16)
        k = t * min(0.8, side)
        return {J('lower_body'): 1 - k, J(f'leg_{s}_thigh'): k} if k > 0.01 else {J('lower_body'): 1.0}
    skirt = bm_band(Vector((0, 0.0, 0)), 0.128, 0.088, -0.215, 0.005, segs=18, flare=1.42)
    # three rings: subdivide the skirt vertically for smooth skinning
    bmesh.ops.subdivide_edges(skirt, edges=[e for e in skirt.edges if abs(e.verts[0].co.z - e.verts[1].co.z) > 0.01], cuts=2)
    weighted(obj_from_bm('TunicSkirt', skirt, 'Red', arm), hip_weights)
    # pteruges: leather strips over the skirt, in two rows
    for row, (z0, z1, rr, n_) in enumerate(((-0.005, -0.150, 1.03, 16), (0.010, -0.105, 1.10, 16))):
        for k in range(n_):
            a = 2 * math.pi * (k + 0.5 * row) / n_
            d = Vector((math.cos(a), math.sin(a), 0))
            r0 = Vector((math.cos(a) * 0.132 * rr, math.sin(a) * 0.092 * rr, 0))
            flare = 1.0 + 0.32 * ((z0 - z1) / 0.215)
            p0 = r0 + Vector((0, 0.0, z0))
            p1 = Vector((r0.x * flare, r0.y * flare, z1))
            st = bm_strip(p0, p1, 0.026, d, segs=2, bend=0.003)
            weighted(obj_from_bm(f'Pteruges{row}_{k}', st, 'Leather', arm, outward=d), hip_weights)

    # ================================================================ ARMS
    for s in 'ab':
        sx = side_of[s]
        sh0, el, wr = shoulder[s], elbow[s], wrist[s]
        start = Vector((sx * 0.105, 0, sh0.z + 0.005))
        pts = [start, sh0 + Vector((sx * 0.02, 0, 0)), (sh0 + el) / 2, el, (el + wr) / 2, wr]
        rad = [0.046, 0.044, 0.040, 0.034, 0.033, 0.027]
        aw = chain([J(f'arm_{s}_shoulder'), J(f'arm_{s}_upper'), J(f'arm_{s}_fore')], [abs(sh0.x) - 0.005, abs(el.x)], 0.03)
        ob = obj_from_bm(f'Arm_{s}', bm_tube(pts, rad, 10, caps=(False, True), ref=UP), 'Skin', arm)
        weighted(ob, lambda p, aw=aw: aw(abs(p.x)))
        sleeve = bm_tube([start + Vector((sx * 0.01, 0, 0)), sh0 + Vector((sx * 0.06, 0, 0))], [0.054, 0.050], 10, caps=(False, False), ref=UP)
        weighted(obj_from_bm(f'Sleeve_{s}', sleeve, 'Red', arm), lambda p, aw=aw: aw(abs(p.x)))
        # bracer: a leather cuff on the forearm with a gold rim
        b0, b1 = el + (wr - el) * 0.45, el + (wr - el) * 0.95
        brc = bm_tube([b0, b1], [0.039, 0.032], 10, caps=(False, False), ref=UP)
        rigid(obj_from_bm(f'Bracer_{s}', brc, 'Leather', arm), H(f'arm_{s}_fore'))
        rim = bm_tube([b0 - (wr - el) * 0.01, b0 + (wr - el) * 0.05], [0.041, 0.041], 10, caps=(False, False), ref=UP)
        rigid(obj_from_bm(f'BracerRim_{s}', rim, 'Gold', arm), H(f'arm_{s}_fore'))
        # fist
        hc_ = wr + Vector((sx * 0.045, 0, -0.002))
        fist = bm_box(hc_, (RIGHT, FWD, UP), (0.060, 0.050, 0.058), bevel=0.012)
        thumb = bm_tube([hc_ + Vector((-sx * 0.012, 0.022, 0.012)), hc_ + Vector((sx * 0.012, 0.030, 0.022))], [0.010, 0.009], 6)
        ob = obj_from_bm(f'Fist_{s}', merge(fist, thumb), 'Skin', arm)
        rigid(ob, H(f'arm_{s}_hand'))
        effector_parts.append((ob, f'arm_{s}'))

    # ================================================================ LEGS
    for s in 'ab':
        x = hip[s].x
        top = Vector((x, 0.0, -0.03))
        kn = Vector((x, 0.005, knee[s].z))
        an = Vector((x, 0.0, ankle_z))
        pts = [top, Vector((x, 0.0, hip[s].z)), (hip[s] + kn) / 2 + Vector((0, 0.005, 0)), kn, (kn + an) / 2, an]
        rad = [0.060, 0.062, 0.055, 0.044, 0.040, 0.030]
        lw = chain([J('lower_body'), J(f'leg_{s}_thigh'), J(f'leg_{s}_shin')], [-(hip[s].z - 0.02), -kn.z], 0.035)
        ob = obj_from_bm(f'Leg_{s}', bm_tube(pts, rad, 10, caps=(False, False)), 'Skin', arm)
        weighted(ob, lambda p, lw=lw: lw(-p.z))
        # greave: front shell from below the knee to the ankle, with a knee cap
        g = bm_band(Vector((x, 0.004, 0)), 0.050, 0.050, an.z + 0.03, kn.z - 0.02, segs=8, flare=0.75, thick=0.08,
                    front_only=(math.radians(-30), math.radians(210)))
        rigid(obj_from_bm(f'Greave_{s}', g, 'Bronze', arm), H(f'leg_{s}_shin'))
        cap = bm_ellipsoid(kn + Vector((0, 0.038, 0.0)), (0.036, 0.018, 0.036), 8, 4)
        rigid(obj_from_bm(f'KneeCap_{s}', cap, 'Bronze', arm), H(f'leg_{s}_shin'))
        # sandal straps up the ankle (on the shin)
        for k in range(3):
            z = an.z + 0.006 + 0.022 * k
            st = bm_tube([Vector((x, 0, z)), Vector((x, 0, z + 0.009))], [0.033 - 0.002 * k, 0.033 - 0.002 * k], 8, caps=(False, False))
            rigid(obj_from_bm(f'AnkleStrap_{s}{k}', st, 'Leather', arm), H(f'leg_{s}_shin'))
        # foot: sole + upper (ankle bone) and the toe part (toe bone), drawn under the shin facing +Y
        fc = Vector((x, 0.04, ground))
        sole = bm_box(fc + Vector((0, -0.02, 0.008)), (RIGHT, FWD, UP), (0.072, 0.17, 0.016), bevel=0.004)
        upper = bm_box(fc + Vector((0, -0.03, 0.040)), (RIGHT, FWD, UP), (0.060, 0.12, 0.050), bevel=0.014)
        ob = obj_from_bm(f'Foot_{s}', merge(sole, upper), 'Leather', arm)
        rigid(ob, H(f'leg_{s}_ankle'))
        effector_parts.append((ob, f'leg_{s}'))
        toe_sole = bm_box(fc + Vector((0, 0.10, 0.008)), (RIGHT, FWD, UP), (0.070, 0.08, 0.016), bevel=0.004)
        toes = bm_box(fc + Vector((0, 0.095, 0.026)), (RIGHT, FWD, UP), (0.060, 0.07, 0.026), bevel=0.010)
        ob = obj_from_bm(f'Toes_{s}', toe_sole, 'Leather', arm)
        rigid(ob, H(f'leg_{s}_foot'))
        effector_parts.append((ob, f'leg_{s}'))
        ob = obj_from_bm(f'ToesSkin_{s}', toes, 'Skin', arm)
        rigid(ob, H(f'leg_{s}_foot'))
        effector_parts.append((ob, f'leg_{s}'))
        hob = bm_tube([fc + Vector((-0.03, 0.04, 0.03)), fc + Vector((0.03, 0.04, 0.03))], [0.012, 0.012], 6)
        ob = obj_from_bm(f'FootStrap_{s}', hob, 'Leather', arm)
        rigid(ob, H(f'leg_{s}_ankle'))
        effector_parts.append((ob, f'leg_{s}'))

    # ================================================================ SCUTUM (left forearm)
    s = left_arm[-1]
    sc = (elbow[s] + wrist[s]) / 2 + FWD * 0.075
    hw, hh = 0.16, 0.26
    frame = bpy.data.objects.new('ScutumFrame', None)
    bpy.context.scene.collection.objects.link(frame)
    frame.matrix_world = frame_matrix(sc, RIGHT, FWD, UP)
    shield = bmesh.new()
    cols, rws, R = 6, 4, 0.30
    grid = []
    for i in range(cols + 1):
        u = -hw + 2 * hw * i / cols
        a = u / R
        col = []
        for j in range(rws + 1):
            v = -hh + 2 * hh * j / rws
            front = sc + RIGHT * (R * math.sin(a)) + FWD * (R * (math.cos(a) - 1)) + UP * v
            col.append(front)
        grid.append(col)
    vf = [[shield.verts.new(p) for p in col] for col in grid]
    vb = [[shield.verts.new(p - FWD * 0.012) for p in col] for col in grid]
    for i in range(cols):
        for j in range(rws):
            shield.faces.new((vf[i][j], vf[i + 1][j], vf[i + 1][j + 1], vf[i][j + 1]))
            shield.faces.new((vb[i][j + 1], vb[i + 1][j + 1], vb[i + 1][j], vb[i][j]))
    for i in range(cols):
        shield.faces.new((vf[i][0], vb[i][0], vb[i + 1][0], vf[i + 1][0]))
        shield.faces.new((vf[i + 1][rws], vb[i + 1][rws], vb[i][rws], vf[i][rws]))
    for j in range(rws):
        shield.faces.new((vf[0][j + 1], vb[0][j + 1], vb[0][j], vf[0][j]))
        shield.faces.new((vf[cols][j], vb[cols][j], vb[cols][j + 1], vf[cols][j + 1]))
    rigid(obj_from_bm('Scutum', shield, 'Scutum', arm), scutum_bone)
    # gold rim (top / bottom bars follow the curve) and boss
    rim = bmesh.new()
    for v in (-hh, hh):
        pts = [grid[i][0] * 0 + sc + RIGHT * (R * math.sin((-hw + 2 * hw * i / cols) / R)) +
               FWD * (R * (math.cos((-hw + 2 * hw * i / cols) / R) - 1) - 0.004) + UP * v for i in range(0, cols + 1, 2)]
        t = bm_tube(pts, [0.010] * len(pts), 4, caps=(True, True), ref=FWD)
        rim = merge(rim, t)
    for u in (-hw, hw):
        a = u / R
        base = sc + RIGHT * (R * math.sin(a)) + FWD * (R * (math.cos(a) - 1) - 0.004)
        t = bm_tube([base - UP * hh, base + UP * hh], [0.010, 0.010], 4, caps=(True, True), ref=FWD)
        rim = merge(rim, t)
    rigid(obj_from_bm('ScutumRim', rim, 'Gold', arm), scutum_bone)
    boss = bm_ellipsoid(sc + FWD * 0.004, (0.055, 0.055, 0.030), 10, 3, frame=Matrix.Rotation(math.radians(-90), 4, 'X'), zmin=0.0)
    rigid(obj_from_bm('ScutumBoss', boss, 'Gold', arm), scutum_bone)
    bossplate = bm_box(sc + FWD * 0.002, (RIGHT, FWD, UP), (0.13, 0.006, 0.13), bevel=0.004)
    rigid(obj_from_bm('ScutumBossPlate', bossplate, 'Iron', arm), scutum_bone)

    # ================================================================ GLADIUS (right hand): grip through the fist,
    # blade forward and up
    s = right_arm[-1]
    g0 = wrist[s] + Vector((side_of[s] * 0.045, 0, 0))
    bd = (FWD * 0.9 + UP * 0.45).normalized()
    side = RIGHT
    flat = bd.cross(side).normalized()
    grip = bm_tube([g0 - bd * 0.045, g0 + bd * 0.040], [0.011, 0.011], 8, ref=side)
    pommel = bm_ellipsoid(g0 - bd * 0.052, (0.018, 0.018, 0.016), 8, 4, frame=frame_matrix(Vector(), side, flat, bd))
    guard = bm_box(g0 + bd * 0.048, (side, flat, bd), (0.065, 0.024, 0.014), bevel=0.004)
    blade = bmesh.new()
    L, W, T = 0.30, 0.024, 0.006
    secs = [(0.0, 1.0), (0.75, 0.92), (0.92, 0.55), (1.0, 0.0)]
    rings = []
    for t, k in secs:
        c = g0 + bd * (0.055 + L * t)
        rings.append([blade.verts.new(c + side * W * k), blade.verts.new(c + flat * T * max(k, 0.2)),
                      blade.verts.new(c - side * W * k), blade.verts.new(c - flat * T * max(k, 0.2))])
    for i in range(len(rings) - 1):
        for q in range(4):
            blade.faces.new((rings[i][q], rings[i][(q + 1) % 4], rings[i + 1][(q + 1) % 4], rings[i + 1][q]))
    blade.faces.new(list(reversed(rings[0])))
    for nm, bm_, mat in (('GladiusGrip', grip, 'Leather'), ('GladiusPommel', pommel, 'Gold'), ('GladiusGuard', guard, 'Gold'),
                         ('GladiusBlade', blade, 'Iron')):
        ob = obj_from_bm(nm, bm_, mat, arm)
        rigid(ob, gladius_bone)
        effector_parts.append((ob, right_arm))

    # ---- apply modifiers other than the armature (solidify), triangulate, count
    for ob, _ in parts:
        for m in list(ob.modifiers):
            if m.type != 'ARMATURE':
                with bpy.context.temp_override(object=ob, active_object=ob):
                    bpy.ops.object.modifier_apply(modifier=m.name)
        ob.data.materials.clear()
        bm = bmesh.new()      # closed shapes (e.g. after solidify): normals outside
        bm.from_mesh(ob.data)
        if all(e.is_manifold for e in bm.edges):
            bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
            bm.to_mesh(ob.data)
        bm.free()
    frames = {'face': face, 'scutum': frame, 'scutum_size': (hw, hh), 'scutum_normal': FWD}
    mats = build_shaders(frames)
    head_bone = H('head')

    def on_head(ob, f, dl):
        gi = ob.vertex_groups[head_bone].index if head_bone in ob.vertex_groups else -1
        return all(v[dl].get(gi, 0) > 0.5 for v in f.verts)

    def shield_front(ob, f, dl):
        return f.normal.dot(FWD) > 0.3
    UV_BOOST['Skin'] = (on_head, 3.0)
    UV_BOOST['Scutum'] = (shield_front, 4.0)
    global TEX_DUMP
    TEX_DUMP = os.path.splitext(dst)[0]
    joined = bake_all(mats)
    tris = sum(triangulate(o) for o in joined.values())
    log(f'{len(parts)} parts in {len(joined)} materials, {tris} triangles')

    # ---- preview (as drawn in game: hands and feet in place)
    bpy.data.objects.remove(face, do_unlink=True)
    bpy.data.objects.remove(frame, do_unlink=True)
    base = os.path.splitext(dst)[0]
    preview = base + '.preview.png'
    import render  # noqa: E402  (tools/blender/render.py)
    state = bpy.context.scene.render.engine
    # render a copy of the scene so the render settings do not leak into the export
    render.render(preview)
    bpy.context.scene.render.engine = state
    for o in list(bpy.context.scene.objects):
        if o.name.startswith('PreviewCam'):
            bpy.data.objects.remove(o, do_unlink=True)

    # ---- move effector parts to the game data's bind positions. They were joined per material: move their
    # vertices by bone (each such vertex is 100% on a hand / foot / gladius bone).
    eff_bones = {}
    for s in 'ab':
        eff_bones[H(f'arm_{s}_hand')] = f'arm_{s}'
        eff_bones[H(f'leg_{s}_ankle')] = f'leg_{s}'
        eff_bones[H(f'leg_{s}_foot')] = f'leg_{s}'
    eff_bones[gladius_bone] = right_arm
    moved = 0
    for ob in joined.values():
        for v in ob.data.vertices:
            gs = [(ob.vertex_groups[g.group].name, g.weight) for g in v.groups if g.weight > 1e-3]
            if len(gs) == 1 and gs[0][0] in eff_bones:
                m = effector[eff_bones[gs[0][0]]]
                v.co = ob.matrix_world.inverted() @ (m @ (ob.matrix_world @ v.co))
                moved += 1
        ob.data.update()
        # normals of the moved feet turn with them: recompute custom normals not needed (no custom split normals)
    log(f'moved {moved} hand / foot / gladius vertices to the game data bind positions')
    pose = {b: [list(r) for r in effector[k].inverted()] for b, k in eff_bones.items()}
    json.dump(pose, open(base + '.pose.json', 'w', encoding='utf-8'), indent=1)

    # ---- roles by bone name and the import options for the loop
    args = ['--replace']
    if not template_skeleton:
        rb = {'B_material_joint': 'JOBJ_1', 'C': {k: (None if v is None else f'JOBJ_{v}') for k, v in roles.items()}}
        rpath = base + '.roles_by_name.json'
        json.dump(rb, open(rpath, 'w', encoding='utf-8'), indent=2)
        args += ['--skeleton', 'glb', '--roles', rpath]
    open(base + '.args', 'w', encoding='utf-8', newline='\n').write('\n'.join(args) + '\n')
    log(f'import options: {" ".join(args)}')
    common.save(dst)


if __name__ == '__main__':
    main()
