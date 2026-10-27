"""Render a quick front + side preview PNG of a .glb (bind pose, flat shaded) to sanity-check exports.

Usage: tools/pyenv/Scripts/python render_preview.py <model.glb> <out.png>
"""
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import trimesh
from mpl_toolkits.mplot3d.art3d import Poly3DCollection


def main():
    scene = trimesh.load(sys.argv[1], force="scene")
    mesh = scene.dump(concatenate=True)
    v, f = mesh.vertices, mesh.faces
    tris = v[f]
    n = np.cross(tris[:, 1] - tris[:, 0], tris[:, 2] - tris[:, 0])
    n /= np.linalg.norm(n, axis=1, keepdims=True) + 1e-9
    fig = plt.figure(figsize=(10, 5))
    for i, (elev, azim, title) in enumerate([(10, -90, "front"), (10, 0, "side")]):
        ax = fig.add_subplot(1, 2, i + 1, projection="3d")
        light = np.array([0.3, -1.0, 0.6])
        shade = np.clip(np.abs(n @ (light / np.linalg.norm(light))), 0.15, 1)
        ax.add_collection3d(Poly3DCollection(tris[:, :, [0, 2, 1]], facecolors=plt.cm.Greys_r(shade * 0.85),
                                             edgecolor="none"))
        lo, hi = v.min(0), v.max(0)
        c, r = (lo + hi) / 2, (hi - lo).max() / 2
        ax.set_xlim(c[0] - r, c[0] + r)
        ax.set_ylim(c[2] - r, c[2] + r)
        ax.set_zlim(c[1] - r, c[1] + r)
        ax.view_init(elev, azim)
        ax.set_axis_off()
        ax.set_title(title)
    fig.suptitle(f"{sys.argv[1].split(chr(92))[-1]}  verts={len(v)} tris={len(f)}")
    fig.savefig(sys.argv[2], dpi=90, bbox_inches="tight")


if __name__ == "__main__":
    main()
