"""Load a glb exported by HSDRawViewer and export it again unchanged: tests the Blender part of the model loop.

usage: blender -b --factory-startup --python tools/blender/roundtrip.py -- <in.glb> <out.glb> [<roles.json>, ignored]

The import and export settings live in common.py (shared with the edit scripts such as legionnaire.py), so a
round trip goes through exactly the same Blender steps as an edit. See docs/MODDING.md "Blender loop".
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common  # noqa: E402


def main():
    src, dst, _ = common.args(2, __doc__, 3)
    common.load(src)
    common.save(dst)


if __name__ == '__main__':
    main()
