"""The game's painted look for an AI-made model's bake (Nov 10, 2026; docs/robo_look.md). Shared by
tools/blender/atlas_ai_body.py and tools/blender/ai_part.py (--look=paint); plain numpy, no Blender calls.

The stock robos' textures are painted, not photographed: flat colour areas, one smooth light ramp per piece (chrome
and gold as pale -> deep ramps), dark panel lines where materials meet, no noise; plus a small specular mask of what
shines (every stock body: one 512x512 CMPR on TEX0 + one 128x128 CMPR specular light map on TEX1, ~2,400-2,800
triangles; parts 256 / 128 textures and 400-900 triangles; tools/robo_look_scan.py). An AI bake is a photo of worn
material: it goes to mush when shrunk. paint() sorts every texel into one of the robo's materials (by what it looks
like in the bake), cleans the label map into flat shapes, repaints each material with its own ramp (lit by a baked
normal if given, else by the bake's smoothed lightness), keeps big relief / occlusion, draws panel lines on the
material borders and sets the specular mask per material.
"""
import json

import numpy as np

# name, AI colour (sRGB, as it shows in the bake), paint dark, paint light, specular, ramp strength
SAMURAI = [
    ('red', (0.62, 0.09, 0.08), (0.55, 0.06, 0.05), (0.92, 0.20, 0.14), 0.45, 0.55),
    # the AI's lacquer is mostly this dark (seen 20% of the bake against 3% bright red): painted as the same red
    ('maroon', (0.30, 0.06, 0.06), (0.50, 0.05, 0.05), (0.86, 0.17, 0.12), 0.40, 0.55),
    ('gold', (0.78, 0.60, 0.30), (0.52, 0.34, 0.09), (1.00, 0.90, 0.52), 0.85, 1.00),
    ('black', (0.09, 0.09, 0.10), (0.07, 0.07, 0.09), (0.26, 0.26, 0.30), 0.25, 0.60),
    ('chrome', (0.60, 0.61, 0.64), (0.36, 0.38, 0.44), (0.97, 0.98, 1.00), 1.00, 1.00),
    ('cream', (0.86, 0.82, 0.74), (0.62, 0.58, 0.52), (0.96, 0.93, 0.86), 0.20, 0.50),
    ('cyan', (0.30, 0.85, 0.95), (0.30, 0.88, 1.00), (0.70, 1.00, 1.00), 0.00, 0.25),
]
GLOW = ('cyan',)          # materials drawn without shade or panel lines (lights, visors)


def palette(path=None):
    """--paint=<json>: another robo's materials (a list of the SAMURAI fields); --paint=auto: found from the bake"""
    if path == 'auto':
        return 'auto'
    return [tuple(x) for x in json.load(open(path, encoding='utf-8'))] if path else SAMURAI


def from_lab(lab):
    L, a, b = lab[..., 0], lab[..., 1], lab[..., 2]
    fy = (L + 16) / 116
    f = np.stack([fy + a / 500, fy, fy - b / 200], -1)
    xyz = np.where(f > 0.2069, f ** 3, (f - 16 / 116) / 7.787) * np.array([0.9505, 1.0, 1.089])
    M = np.array([[3.2406, -1.5372, -0.4986], [-0.9689, 1.8758, 0.0415], [0.0557, -0.2040, 1.0570]])
    lin = np.clip(xyz @ M.T, 0, 1)
    return np.where(lin <= 0.0031308, lin * 12.92, 1.055 * lin ** (1 / 2.4) - 0.055)


def auto_palette(src, k=8, seed=7):
    """--paint=auto (Nov 10, docs/robo_look.md "Auto palette"): any robo without a hand-made material list. k-means
    (Lab, lightness weighted 0.55 as the matching) over the bake's non-black texels; each cluster becomes a material:
    its ramp runs from L-22 to L+24 with the chroma lifted 1.2x (painted colours are cleaner than the photo); the
    specular follows what it looks like: pale grey -> chrome 1.0, yellow / orange and bright -> gold 0.85, dark ->
    0.25, else 0.45; a bright saturated cyan / green / blue cluster is a light (unshaded)"""
    px = src.reshape(-1, 3)
    px = px[px.max(1) > 0.03]
    rng = np.random.default_rng(seed)
    smp = to_lab(px[rng.choice(len(px), min(len(px), 60000), replace=False)])
    w = np.array([0.55, 1.0, 1.0])
    cen = smp[rng.choice(len(smp), k, replace=False)]
    for _ in range(25):
        lab_id = (((smp[:, None] - cen[None]) * w) ** 2).sum(-1).argmin(1)
        cen = np.array([smp[lab_id == i].mean(0) if (lab_id == i).any() else cen[i] for i in range(k)])
    pal = []
    for i, c in enumerate(cen):
        L, a, b = c
        ch = float(np.hypot(a, b))
        hue = float(np.degrees(np.arctan2(b, a))) % 360
        dark = from_lab(np.array([max(L - 22, 3), a * 1.2, b * 1.2]))
        light = from_lab(np.array([min(L + 24, 98), a * 1.2, b * 1.2]))
        if ch < 12 and L > 45:
            name, sp, ramp = f'chrome{i}', 1.0, 1.0
            dark = from_lab(np.array([max(L - 30, 20), a, b]))
            light = from_lab(np.array([min(L + 35, 99), a, b]))
        elif 50 < hue < 100 and ch > 25 and L > 45:
            name, sp, ramp = f'gold{i}', 0.85, 1.0
        elif 150 < hue < 290 and ch > 30 and L > 55:
            name, sp, ramp = f'glow{i}', 0.0, 0.25
        elif L < 22:
            name, sp, ramp = f'dark{i}', 0.25, 0.6
        else:
            name, sp, ramp = f'paint{i}', 0.45, 0.55
        pal.append((name, tuple(float(x) for x in from_lab(c)), tuple(float(x) for x in dark),
                    tuple(float(x) for x in light), sp, ramp))
    return pal


def boxblur(a, r):
    if r < 1:
        return a
    k = 2 * r + 1
    p = np.pad(a, ((r, r), (0, 0), (0, 0)), mode='edge').astype(np.float64)
    c = np.concatenate([np.zeros_like(p[:1]), np.cumsum(p, axis=0)], 0)
    a = (c[k:] - c[:-k]) / k
    p = np.pad(a, ((0, 0), (r, r), (0, 0)), mode='edge')
    c = np.concatenate([np.zeros_like(p[:, :1]), np.cumsum(p, axis=1)], 1)
    return ((c[:, k:] - c[:, :-k]) / k).astype(np.float32)


def gblur(a, r):
    """~Gaussian of radius r (three box passes)"""
    rb = max(1, int(round(r / 1.7)))
    for _ in range(3):
        a = boxblur(a, rb)
    return a


def to_lab(c):
    c = np.clip(c, 0, 1)
    lin = np.where(c <= 0.04045, c / 12.92, ((c + 0.055) / 1.055) ** 2.4)
    M = np.array([[0.4124, 0.3576, 0.1805], [0.2126, 0.7152, 0.0722], [0.0193, 0.1192, 0.9505]])
    xyz = lin @ M.T / np.array([0.9505, 1.0, 1.089])
    f = np.where(xyz > 0.008856, np.cbrt(np.maximum(xyz, 0)), 7.787 * xyz + 16 / 116)
    return np.stack([116 * f[..., 1] - 16, 500 * (f[..., 0] - f[..., 1]), 200 * (f[..., 1] - f[..., 2])], -1)


def dilate(img, valid, iters):
    """fill the texels outside `valid` (gutters) from their island neighbours, one ring per pass (the bilinear
    filter and the CMPR 4x4 blocks then only ever see an island's own colours at its edge)"""
    img = img.copy()
    valid = valid.copy()
    for _ in range(iters):
        if valid.all():
            break
        acc = np.zeros_like(img)
        cnt = np.zeros(valid.shape, np.float32)
        for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            v = np.roll(np.roll(valid, dy, 0), dx, 1)
            acc += np.roll(np.roll(img, dy, 0), dx, 1) * v[..., None]
            cnt += v
        new = ~valid & (cnt > 0)
        img[new] = acc[new] / cnt[new][:, None]
        valid |= new
    return img


def paint(src, ss, pal=SAMURAI, occ=None, shade=None, nrm=None, light=(0.0, 0.55, 0.83), curv=None, edges=0.0,
          isl=None, gutter=0):
    """src: H x W x 3 bake (sRGB 0..1), ss: bake texels per final texel. occ / shade: H x W x 1 (ambient occlusion,
    relief shade around 1), nrm: H x W x 3 object-space normals, curv: H x W x 1 pointiness (0.5 flat) with
    edges = strength. Returns (rgb, spec H x W x 1, labels, report); report['palette'] is the material list used."""
    if pal == 'auto':
        pal = auto_palette(src)
    H, W = src.shape[:2]
    k = len(pal)
    lab = to_lab(gblur(src, ss)).reshape(-1, 3)
    anchors = to_lab(np.array([p[1] for p in pal], dtype=np.float64))
    w = np.array([0.55, 1.0, 1.0])          # lightness counts less than hue: shading does not flip a material
    ids = np.empty(H * W, np.int64)
    for s in range(0, H * W, 1 << 20):      # in slices (a 2048 x 2048 bake x 7 materials)
        d2 = (((lab[s:s + (1 << 20), None, :] - anchors[None]) * w) ** 2).sum(-1)
        ids[s:s + (1 << 20)] = d2.argmin(1)
    ids = ids.reshape(H, W)
    # flat shapes: a majority vote over a window, twice (1.5 / 1 final texels: wider wiped out thin gold trims)
    for r in (int(1.5 * ss), ss):
        onehot = np.stack([(ids == i).astype(np.float32) for i in range(k)], -1)
        ids = boxblur(onehot, max(1, r)).argmax(-1)
    if nrm is not None:
        L = np.array(light, np.float64)
        L /= np.linalg.norm(L)
        nd = gblur(nrm, ss)
        nd /= np.maximum(np.linalg.norm(nd, axis=-1, keepdims=True), 1e-6)
        t = np.clip(0.5 + 0.55 * (nd @ L), 0, 1)[..., None].astype(np.float32)
    else:
        lum = gblur(src.mean(-1, keepdims=True), 4 * ss)
        t = np.clip(lum / max(float(lum[lum > 0.02].mean()) if (lum > 0.02).any() else 0.5, 1e-3) * 0.5, 0, 1)
    occ_s = gblur(occ, 2 * ss) if occ is not None else np.ones((H, W, 1), np.float32)
    shade_s = np.clip(1 + 0.8 * (gblur(shade, max(1, ss // 2)) - 1), 0.75, 1.3) if shade is not None else 1.0
    out = np.zeros((H, W, 3), np.float32)
    spec = np.zeros((H, W, 1), np.float32)
    for i, (name, _ai, dark, light_, sp, ramp) in enumerate(pal):
        m = (ids == i)[..., None]
        if not m.any():
            continue
        dk, lt = np.array(dark, np.float32), np.array(light_, np.float32)
        mid = (dk + lt) / 2
        c = mid + (dk + (lt - dk) * t - mid) * ramp
        if name not in GLOW:
            c = c * (0.78 + 0.22 * occ_s) * shade_s
        out = np.where(m, c, out)
        spec = np.where(m, np.float32(sp), spec)
    lights = np.array([p[3] for p in pal], np.float32)
    # panel lines only where the painted colour changes: materials painted alike (the samurai's red and dark red)
    # share a group, else their noisy border drew squiggles through the lacquer
    grp = np.array([next(j for j in range(k) if np.abs(lights[j] - lights[i]).max() < 0.12) for i in range(k)])
    gid = grp[ids]
    edge = np.zeros((H, W), bool)
    rr = max(1, ss // 2)
    for dy, dx in ((rr, 0), (0, rr), (rr, rr), (rr, -rr)):
        e_ = np.roll(np.roll(gid, dy, 0), dx, 1) != gid
        if isl is not None:
            # a line only inside one UV island: across an island's border (the gutter, the next island) a line here
            # is a seam on the model
            e_ &= (np.roll(np.roll(isl, dy, 0), dx, 1) == isl) & (isl >= 0)
        edge |= e_
    glow = np.isin(ids, [i for i, p in enumerate(pal) if p[0] in GLOW or p[0].startswith('glow')])
    if curv is not None and edges > 0:
        # convex edges lit, crevices shaded (Nov 10, docs/robo_look.md "Edge highlights"): the bright bevel lines and
        # dark seams hand-painted textures carry. Pointiness: 0.5 flat; its spread on an AI scan is narrow, so it is
        # normalised by its own spread (the 2nd / 98th percentiles) after a light blur (plate-edge scale, not noise)
        cv = gblur(curv, max(1, ss // 2))[..., 0]
        lo_, hi_ = np.percentile(cv, 2), np.percentile(cv, 98)
        mid = float(np.median(cv))
        conv = np.clip((cv - mid) / max(hi_ - mid, 1e-4), 0, 1) ** 1.5
        conc = np.clip((mid - cv) / max(mid - lo_, 1e-4), 0, 1) ** 1.5
        keep = ~glow
        # lit toward the material's own light colour (toward white turned the lacquer pink)
        lt_map = lights[ids]
        out = np.where(keep[..., None], out + (np.clip(lt_map * 1.08, 0, 1) - out) * (0.6 * edges * conv)[..., None], out)
        out = np.where(keep[..., None], out * (1 - 0.45 * edges * conc)[..., None], out)
    line = gblur((edge & ~glow).astype(np.float32)[..., None], max(1, ss // 2))
    out = out * (1 - 0.55 * np.clip(line * 2.2, 0, 1))
    if isl is not None and gutter > 0:
        valid = isl >= 0
        out = dilate(out, valid, gutter)
        spec = dilate(spec, valid, gutter)
    report = {p[0]: round(float((ids == i).mean()) * 100, 1) for i, p in enumerate(pal)}
    report['palette'] = pal
    return np.clip(out, 0, 1), spec, ids, report
