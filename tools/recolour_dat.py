"""Recolour the paletted textures of an HSD .dat in place: every TObj's palette (and the palettes of texture
animations) is hue-shifted to a target hue, keeping each colour's brightness. The file keeps its size; only palette
bytes change. Formats: palette RGB5A3 (2), RGB565 (1), IA8 (0, left as is: grey).

  python -I tools/recolour_dat.py <in.dat> <out.dat> <hue degrees> [sat x] [val x]
  python -I tools/recolour_dat.py <in.dat> <out.dat> gold | crimson
"""
import colorsys
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import scene_data as S  # noqa: E402

NAMED = {'gold': (44, 1.15, 1.0), 'crimson': (354, 1.2, 0.95)}


def shift(rgb, hue, sat, val):
    h, s, v = colorsys.rgb_to_hsv(*(x / 255 for x in rgb))
    if s < 0.08:
        s = 0.18 if v > 0.5 else s
    return [round(x * 255) for x in colorsys.hsv_to_rgb(hue / 360, min(1, s * sat), min(1, v * val))]


def rgb5a3_dec(w):
    if w & 0x8000:
        return [((w >> 10) & 31) * 255 // 31, ((w >> 5) & 31) * 255 // 31, (w & 31) * 255 // 31], None
    return [((w >> 8) & 15) * 17, ((w >> 4) & 15) * 17, (w & 15) * 17], (w >> 12) & 7


def rgb5a3_enc(rgb, a3):
    if a3 is None:
        return 0x8000 | (rgb[0] * 31 // 255) << 10 | (rgb[1] * 31 // 255) << 5 | rgb[2] * 31 // 255
    return a3 << 12 | (rgb[0] // 17) << 8 | (rgb[1] // 17) << 4 | rgb[2] // 17


def tobjs(d):
    """every TObj reachable from the scene_data / joint roots"""
    out = set()

    def mobj(m):
        t = d.ptr(m + 8)
        while t is not None:
            out.add(t)
            t = d.ptr(t + 4)
    for name, o in d.roots:
        roots = []
        if name == 'scene_data':
            models = d.ptr(o)
            i = 0
            while models is not None and d.ptr(models + 4 * i) is not None:
                roots.append(d.ptr(d.ptr(models + 4 * i)))
                i += 1
        else:
            roots.append(o)
        for r in roots:
            for j, dep, fl, dobj in S.walk_joints(d, r):
                while dobj is not None:
                    m = d.ptr(dobj + 8)
                    if m is not None:
                        mobj(m)
                    dobj = d.ptr(dobj + 4)
    return out


def tluts(d):
    found = set()
    for t in tobjs(d):
        tl = d.ptr(t + 0x50)
        if tl is not None:
            found.add(tl)
    return found


def main():
    src, dst, h = sys.argv[1], sys.argv[2], sys.argv[3]
    hue, sat, val = NAMED[h] if h in NAMED else (float(h), float(sys.argv[4]) if len(sys.argv) > 4 else 1.0,
                                                   float(sys.argv[5]) if len(sys.argv) > 5 else 1.0)
    d = S.Dat(src)
    raw = bytearray(d.raw)
    n = 0
    for tl in sorted(tluts(d)):
        data, fmt, count = d.ptr(tl), d.u32(tl + 4), d.u16(tl + 0xC)
        if data is None or fmt not in (1, 2):
            continue
        for i in range(count):
            o = 0x20 + data + 2 * i
            w = struct.unpack_from('>H', raw, o)[0]
            if fmt == 2:
                rgb, a3 = rgb5a3_dec(w)
                w2 = rgb5a3_enc(shift(rgb, hue, sat, val), a3)
            else:
                rgb = [((w >> 11) & 31) * 255 // 31, ((w >> 5) & 63) * 255 // 63, (w & 31) * 255 // 31]
                r, g, b = shift(rgb, hue, sat, val)
                w2 = (r * 31 // 255) << 11 | (g * 63 // 255) << 5 | b * 31 // 255
            struct.pack_into('>H', raw, o, w2)
        n += 1
    Path(dst).parent.mkdir(parents=True, exist_ok=True)
    Path(dst).write_bytes(bytes(raw))
    print(f'{src}: {n} palettes recoloured -> {dst}')


if __name__ == '__main__':
    main()
