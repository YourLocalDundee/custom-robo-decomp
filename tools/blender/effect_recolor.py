"""Recolour an effect model (shot, blast, charge attack) onto a gradient: every texture's pixels and every material's
base colour are mapped by brightness onto the gradient, alpha kept. For a robo's own look (the mecha samurai: dark
red -> cyan -> gold-white; docs/full_custom_robo.md).

usage: blender -b --factory-startup --python tools/blender/effect_recolor.py -- <in.glb> <out.glb> [<ignored>]
       env EFFECT_GRADIENT="r,g,b;r,g,b;r,g,b" (dark, middle, bright; 0..1) overrides the samurai gradient

`HSDRawViewer --import` re-encodes a kept mesh's texture in its own format when its pixels changed, and writes a
changed base colour factor as the MObj's diffuse colour (docs/MODDING.md "What HSDRawViewer --import keeps").
"""
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

GRAD = [(0.35, 0.02, 0.04), (0.0, 0.80, 1.0), (1.0, 0.92, 0.62)]
if os.environ.get('EFFECT_GRADIENT'):
    GRAD = [tuple(float(x) for x in p.split(',')) for p in os.environ['EFFECT_GRADIENT'].split(';')]
G = np.array(GRAD, dtype=np.float32)


def remap(rgb):
    """brightness (max channel, so saturated colours count as bright) -> the gradient, keeping some of the original
    contrast"""
    v = np.clip(rgb.max(axis=-1, keepdims=True), 0, 1)
    t = v * 2
    lo = G[0] + (G[1] - G[0]) * np.clip(t, 0, 1)
    hi = G[1] + (G[2] - G[1]) * np.clip(t - 1, 0, 1)
    out = np.where(t <= 1, lo, hi)
    return np.clip(out * (0.6 + 0.4 * v / np.maximum(v.mean(), 1e-3)), 0, 1)


def main():
    src, dst, _ = common.args(2, __doc__, 3)
    common.load(src)
    imgs, mats = set(), set()
    for obj in common.model_meshes():
        for slot in obj.material_slots:
            mat = slot.material
            if mat is None or mat.node_tree is None or mat.name in mats:
                continue
            mats.add(mat.name)
            for node in mat.node_tree.nodes:
                if node.type == 'TEX_IMAGE' and node.image and node.image.name not in imgs:
                    im = node.image
                    imgs.add(im.name)
                    px = np.array(im.pixels[:], dtype=np.float32).reshape(-1, 4)
                    px[:, :3] = remap(px[:, :3])
                    im.pixels[:] = px.ravel()
                    im.update()
                    im.pack()
                if node.type == 'BSDF_PRINCIPLED' and not node.inputs['Base Color'].is_linked:
                    c = node.inputs['Base Color'].default_value
                    n = remap(np.array([[c[0], c[1], c[2]]], dtype=np.float32))[0]
                    node.inputs['Base Color'].default_value = (float(n[0]), float(n[1]), float(n[2]), c[3])
    print(f'effect_recolor: {len(imgs)} texture(s), {len(mats)} material(s)')
    common.save(dst)


if __name__ == '__main__':
    main()
