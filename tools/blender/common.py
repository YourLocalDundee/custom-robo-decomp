"""Shared pieces of the Blender scripts in tools/blender (run by Blender's own Python, never by python -I).

Import / export settings for the model loop (docs/MODDING.md "Blender loop"):
  import: glTF importer defaults (bone_heuristic BLENDER, guess_original_bind_pose) - HSDRawViewer --import takes
          the bone transforms from the template .dat, so Blender's bone orientation does not matter, only the
          names JOBJ_n and the parent of each bone.
  export: GLB, +Y up, skins on, no animations, no shape keys, materials and images (PNG) exported, no leaf
          or extra bones, only the visible objects of the scene.
"""
import sys

import bpy


def args(n, doc, most=None):
    """The arguments after '--' on the Blender command line: at least n, at most `most` (default n); missing
    optional ones are None. tools/model_loop.ps1 / .sh always pass <in.glb> <out.glb> <roles.json>."""
    a = sys.argv[sys.argv.index('--') + 1:] if '--' in sys.argv else []
    if not n <= len(a) <= (most or n):
        sys.exit(doc)
    return a + [None] * ((most or n) - len(a))


def load(path):
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bpy.ops.import_scene.gltf(filepath=path)
    arms = [o for o in bpy.context.scene.objects if o.type == 'ARMATURE']
    if len(arms) != 1:
        sys.exit(f'{path}: expected one armature, found {len(arms)}')
    return arms[0]


def model_meshes():
    """The model's meshes: visible mesh objects (the glTF importer adds a hidden bone-shape Icosphere in the
    collection glTF_not_exported; it is not part of the model and is not exported)."""
    return [o for o in bpy.context.scene.objects if o.type == 'MESH' and o.visible_get()]


def save(path):
    bpy.ops.export_scene.gltf(
        filepath=path, export_format='GLB', export_yup=True, export_skins=True, export_animations=False,
        export_morph=False, export_materials='EXPORT', export_image_format='AUTO', export_texcoords=True,
        export_normals=True, export_apply=False, export_def_bones=False, use_visible=True)
    print(f'blender: wrote {path}')
