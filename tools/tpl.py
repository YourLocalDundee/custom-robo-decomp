"""GameCube TPL pictures: decode CMPR (14) / CI8 (9, RGB5A3 or RGB565 palette) / RGB565 (4) / RGB5A3 (5) to PNG, encode a PNG back over a
template TPL in the template's own format and size (the menus' part pictures, docs/full_custom_robo.md).

  python -I tools/tpl.py info <file.tpl>
  python -I tools/tpl.py png <file.tpl> <out.png>
  python -I tools/tpl.py encode <template.tpl> <in.png> <out.tpl>      (resized to the template's size; CMPR or CI8)

Needs Pillow + numpy (tools/pyenv). One image per file (every part picture checked has one).
TPL: u32 magic 0x0020AF30, u32 image count, u32 table offset (12); table: {image header offset, palette header offset};
image header: u16 height, u16 width, u32 format, u32 data offset, wrap / filter / LOD fields; palette header: u16
count, u8 unpacked, u8 pad, u32 format (0 IA8, 1 RGB565, 2 RGB5A3), u32 data offset.
CMPR: 8x8 tiles of four 4x4 DXT1 sub-blocks (big-endian colours, 2-bit indices MSB first). CI8: 8x4 tiles of bytes.
"""
import struct
import sys

import numpy as np
from PIL import Image


def header(d):
    io, po = struct.unpack_from('>II', d, 12)
    h, w, fmt, off = struct.unpack_from('>HHII', d, io)
    pal = struct.unpack_from('>HBBII', d, po) if po else None
    return io, po, w, h, fmt, off, pal


def c565(v):
    return np.stack([((v >> 11) & 31) * 255 // 31, ((v >> 5) & 63) * 255 // 63, (v & 31) * 255 // 31], -1)


def c5a3(v):
    out = np.zeros(v.shape + (4,), np.int32)
    op = (v & 0x8000) != 0
    out[op, 0] = ((v[op] >> 10) & 31) * 255 // 31
    out[op, 1] = ((v[op] >> 5) & 31) * 255 // 31
    out[op, 2] = (v[op] & 31) * 255 // 31
    out[op, 3] = 255
    t = ~op
    out[t, 0] = ((v[t] >> 8) & 15) * 17
    out[t, 1] = ((v[t] >> 4) & 15) * 17
    out[t, 2] = (v[t] & 15) * 17
    out[t, 3] = ((v[t] >> 12) & 7) * 255 // 7
    return out


def decode(d):
    io, po, w, h, fmt, off, pal = header(d)
    W8, H8 = (w + 7) // 8 * 8, (h + 7) // 8 * 8
    img = np.zeros((H8, W8, 4), np.uint8)
    if fmt == 14:
        p = off
        for ty in range(0, H8, 8):
            for tx in range(0, W8, 8):
                for sy in (0, 4):
                    for sx in (0, 4):
                        c0, c1 = struct.unpack_from('>HH', d, p)
                        bits = struct.unpack_from('>I', d, p + 4)[0]
                        p += 8
                        a, b = c565(np.array(c0)), c565(np.array(c1))
                        if c0 > c1:
                            pal4 = [list(a) + [255], list(b) + [255], list((2 * a + b) // 3) + [255], list((a + 2 * b) // 3) + [255]]
                        else:
                            pal4 = [list(a) + [255], list(b) + [255], list((a + b) // 2) + [255], [0, 0, 0, 0]]
                        for i in range(16):
                            k = (bits >> (30 - 2 * i)) & 3
                            img[ty + sy + i // 4, tx + sx + i % 4] = pal4[k]
    elif fmt == 9:
        n, _, _, pf, poff = pal
        pv = np.array(struct.unpack_from('>%dH' % n, d, poff))
        cols = c5a3(pv) if pf == 2 else np.concatenate([c565(pv), np.full((n, 1), 255)], -1)
        p = off
        for ty in range(0, H8 // 4 * 4, 4):
            for tx in range(0, W8, 8):
                for y in range(4):
                    for x in range(8):
                        if ty + y < H8:
                            img[ty + y, tx + x] = cols[d[p]]
                        p += 1
    elif fmt in (4, 5):
        W4, H4 = (w + 3) // 4 * 4, (h + 3) // 4 * 4
        img = np.zeros((H4, W4, 4), np.uint8)
        p = off
        for ty in range(0, H4, 4):
            for tx in range(0, W4, 4):
                v = np.array(struct.unpack_from('>16H', d, p)).reshape(4, 4)
                p += 32
                img[ty:ty + 4, tx:tx + 4] = (np.concatenate([c565(v), np.full((4, 4, 1), 255)], -1) if fmt == 4 else c5a3(v))
    else:
        raise SystemExit(f'format {fmt} not supported')
    return Image.fromarray(img[:h, :w], 'RGBA')


def to565(c):
    c = np.clip(c, 0, 255).astype(np.int32)
    return ((c[..., 0] * 31 + 127) // 255 << 11) | ((c[..., 1] * 63 + 127) // 255 << 5) | ((c[..., 2] * 31 + 127) // 255)


def enc_block(px):
    """px 16x4 (RGBA) -> 8 bytes DXT1 (GX order); transparent pixels (alpha < 128) use the 3-colour + transparent mode"""
    alpha = px[:, 3] < 128
    rgb = px[:, :3].astype(np.float64)
    op = rgb[~alpha] if (~alpha).any() else rgb
    mean = op.mean(0)
    u, s, vt = np.linalg.svd(op - mean) if len(op) > 1 else (None, None, np.eye(3))
    axis = vt[0]
    t = (op - mean) @ axis
    a, b = mean + axis * t.max(), mean + axis * t.min()
    c0, c1 = int(to565(a)), int(to565(b))
    if alpha.any():
        if c0 > c1:
            c0, c1 = c1, c0
        ca, cb = c565(np.array(c0)), c565(np.array(c1))
        pal = np.array([ca, cb, (ca + cb) // 2])
    else:
        if c0 < c1:
            c0, c1 = c1, c0
        if c0 == c1:
            c1 = max(c0 - 1, 0) if c0 else 0
            if c0 == 0:
                c0 = 1
        ca, cb = c565(np.array(c0)), c565(np.array(c1))
        pal = np.array([ca, cb, (2 * ca + cb) // 3, (ca + 2 * cb) // 3])
    bits = 0
    for i in range(16):
        if alpha[i]:
            k = 3
        else:
            k = int(np.argmin(((pal - rgb[i]) ** 2).sum(1)))
        bits |= k << (30 - 2 * i)
    return struct.pack('>HHI', c0, c1, bits)


def encode_cmpr(img, w, h):
    W8, H8 = (w + 7) // 8 * 8, (h + 7) // 8 * 8
    a = np.zeros((H8, W8, 4), np.uint8)
    a[:h, :w] = np.array(img.convert('RGBA'))
    out = bytearray()
    for ty in range(0, H8, 8):
        for tx in range(0, W8, 8):
            for sy in (0, 4):
                for sx in (0, 4):
                    out += enc_block(a[ty + sy:ty + sy + 4, tx + sx:tx + sx + 4].reshape(16, 4))
    return bytes(out)


def encode_ci8(img, w, h, n, pf):
    """a 256-colour palette (Pillow's median cut on RGBA), written as RGB5A3 (pf 2) or RGB565 (pf 1)"""
    q = img.convert('RGBA').quantize(colors=min(n, 256), method=Image.Quantize.FASTOCTREE)
    pal = np.array(q.getpalette('RGBA')[:4 * min(n, 256)]).reshape(-1, 4)
    pal = np.concatenate([pal, np.zeros((n - len(pal), 4), int)]) if len(pal) < n else pal
    if pf == 2:
        pv = []
        for r, g, b, al in pal:
            if al >= 224:
                pv.append(0x8000 | (r * 31 // 255) << 10 | (g * 31 // 255) << 5 | b * 31 // 255)
            else:
                pv.append((al * 7 // 255) << 12 | (r * 15 // 255) << 8 | (g * 15 // 255) << 4 | b * 15 // 255)
    else:
        pv = [int(to565(np.array(c[:3]))) for c in pal]
    idx = np.array(q)
    W8, H4 = (w + 7) // 8 * 8, (h + 3) // 4 * 4
    a = np.zeros((H4, W8), np.uint8)
    a[:h, :w] = idx
    out = bytearray()
    for ty in range(0, H4, 4):
        for tx in range(0, W8, 8):
            out += a[ty:ty + 4, tx:tx + 8].tobytes()
    return bytes(out), struct.pack('>%dH' % n, *pv)


def encode(tpl, png):
    d = bytearray(tpl)
    io, po, w, h, fmt, off, pal = header(d)
    img = Image.open(png).convert('RGBA').resize((w, h), Image.LANCZOS)
    if fmt == 14:
        data = encode_cmpr(img, w, h)
    elif fmt == 9:
        data, pdata = encode_ci8(img, w, h, pal[0], pal[3])
        d[pal[4]:pal[4] + len(pdata)] = pdata
    elif fmt in (4, 5):
        W4, H4 = (w + 3) // 4 * 4, (h + 3) // 4 * 4
        a = np.zeros((H4, W4, 4), np.int32)
        a[:h, :w] = np.array(img)
        if fmt == 4:
            v = to565(a[..., :3])
        else:
            op = a[..., 3] >= 224
            v = np.where(op, 0x8000 | (a[..., 0] * 31 // 255) << 10 | (a[..., 1] * 31 // 255) << 5 | a[..., 2] * 31 // 255,
                         (a[..., 3] * 7 // 255) << 12 | (a[..., 0] * 15 // 255) << 8 | (a[..., 1] * 15 // 255) << 4 | a[..., 2] * 15 // 255)
        data = b''.join(struct.pack('>16H', *[int(x) for x in v[ty:ty + 4, tx:tx + 4].ravel()])
                        for ty in range(0, H4, 4) for tx in range(0, W4, 4))
    else:
        raise SystemExit(f'format {fmt} not supported')
    if off + len(data) > len(d):
        raise SystemExit('encoded data longer than the template image')
    d[off:off + len(data)] = data
    return bytes(d)


def main():
    a = sys.argv[1:]
    if a[0] == 'info':
        d = open(a[1], 'rb').read()
        io, po, w, h, fmt, off, pal = header(d)
        print(f'{a[1]}: {w}x{h} format {fmt}, data at {off:#x}, palette {pal}')
    elif a[0] == 'png':
        decode(open(a[1], 'rb').read()).save(a[2])
    elif a[0] == 'encode':
        open(a[3], 'wb').write(encode(open(a[1], 'rb').read(), a[2]))
        print(f'wrote {a[3]}')


if __name__ == '__main__':
    main()
