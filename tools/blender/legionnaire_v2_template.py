"""legionnaire_v2.py on the template skeleton: the same complete legionnaire, but Ray 01's 55 joints kept exactly
(no Crest / Scutum / Gladius bones: those parts ride the head, left forearm and right hand joints), so the import
needs no --skeleton glb and the game gets Ray 01's own joint paths (no roles file). The fallback if the custom
skeleton of tools/blender/legionnaire_v2.py misbehaves in battle (plan.md step 73).

usage: blender -b --factory-startup --python tools/blender/legionnaire_v2_template.py -- <in.glb> <out.glb> <roles.json>
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import legionnaire_v2  # noqa: E402

if __name__ == '__main__':
    legionnaire_v2.main(template_skeleton=True)
