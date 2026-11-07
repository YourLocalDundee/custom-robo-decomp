"""Tint every material of a model red: a visible test edit for the effect models (docs/MODDING.md "Effects").

usage: blender -b --factory-startup --python tools/blender/effect_tint.py -- <in.glb> <out.glb> [<roles.json>, ignored]

Multiplies the Base Color of each material by (1, 0.25, 0.25), as a Multiply node between the texture and the shader
when Base Color comes from a texture (the glTF exporter writes that as the base colour factor): `HSDRawViewer --import`
writes a changed factor as the MObj's diffuse colour, which shows wherever the material modulates its texture by
that colour (COLORMAP_MODULATE) or has no texture. Geometry and textures stay as they are.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402

TINT = (1.0, 0.25, 0.25)


def main():
    src, dst, _ = common.args(2, __doc__, 3)
    common.load(src)
    seen = set()
    for obj in common.model_meshes():
        for slot in obj.material_slots:
            mat = slot.material
            if mat is None or mat.name in seen or mat.node_tree is None:
                continue
            seen.add(mat.name)
            tree = mat.node_tree
            for node in list(tree.nodes):
                if node.type != 'BSDF_PRINCIPLED':
                    continue
                base = node.inputs['Base Color']
                if not base.is_linked:
                    c = base.default_value
                    base.default_value = (c[0] * TINT[0], c[1] * TINT[1], c[2] * TINT[2], c[3])
                    continue
                src = base.links[0].from_socket
                mix = tree.nodes.new('ShaderNodeMix')
                mix.data_type = 'RGBA'
                mix.blend_type = 'MULTIPLY'
                mix.inputs['Factor'].default_value = 1.0
                mix.inputs['B'].default_value = (*TINT, 1.0)
                tree.links.new(src, mix.inputs['A'])
                tree.links.new(mix.outputs['Result'], base)
    print(f'effect_tint: {len(seen)} material(s) tinted')
    common.save(dst)


if __name__ == '__main__':
    main()
