"""dolphin_capture.py: the Dolphin game window's own pixels (PrintWindow; works when covered) + both robos' bone and part-joint matrices (robo_preview.py --gamemats / --gameparts). Nov 12, 2026.
  python -I wincap.py out.png            one frame
  python -I wincap.py outdir N SECONDS   N frames, one every SECONDS (outdir/frame_HHMMSS.png)
"""
import ctypes
import ctypes.wintypes as wt
import os
import struct
import sys
import time
import zlib

u, g = ctypes.windll.user32, ctypes.windll.gdi32
u.SetProcessDPIAware()
SCALE = int(os.environ.get('WINCAP_SCALE', '2'))   # 2 = half size (3840 wide window -> 1920)


class BIH(ctypes.Structure):
    _fields_ = [('a', wt.DWORD), ('w', ctypes.c_long), ('h', ctypes.c_long), ('p', wt.WORD), ('b', wt.WORD),
                ('c', wt.DWORD), ('s', wt.DWORD), ('x', ctypes.c_long), ('y', ctypes.c_long), ('u', wt.DWORD),
                ('i', wt.DWORD)]


def find():
    hits = []

    @ctypes.WINFUNCTYPE(ctypes.c_bool, wt.HWND, wt.LPARAM)
    def cb(h, _):
        n = u.GetWindowTextLengthW(h)
        b = ctypes.create_unicode_buffer(n + 1)
        u.GetWindowTextW(h, b, n + 1)
        if '(GXCE01)' in b.value and u.IsWindowVisible(h):
            hits.append(h)
        return True
    u.EnumWindows(cb, 0)
    return hits[0] if hits else None


def grab(path):
    h = find()
    if not h:
        print('no Dolphin game window')
        return False
    r = wt.RECT()
    u.GetClientRect(h, ctypes.byref(r))
    w, hh = r.right, r.bottom
    hdc = u.GetDC(h)
    mdc = g.CreateCompatibleDC(hdc)
    bmp = g.CreateCompatibleBitmap(hdc, w, hh)
    g.SelectObject(mdc, bmp)
    u.PrintWindow(h, mdc, 3)      # PW_CLIENTONLY | PW_RENDERFULLCONTENT
    W0, H0 = w, hh
    w, hh = W0 // SCALE, H0 // SCALE
    sdc = g.CreateCompatibleDC(hdc)
    sbmp = g.CreateCompatibleBitmap(hdc, w, hh)
    g.SelectObject(sdc, sbmp)
    g.SetStretchBltMode(sdc, 4)   # HALFTONE
    g.StretchBlt(sdc, 0, 0, w, hh, mdc, 0, 0, W0, H0, 0x00CC0020)
    bi = BIH(ctypes.sizeof(BIH), w, -hh, 1, 32, 0, 0, 0, 0, 0, 0)
    buf = ctypes.create_string_buffer(w * hh * 4)
    g.GetDIBits(sdc, sbmp, 0, hh, buf, ctypes.byref(bi), 0)
    g.DeleteObject(sbmp)
    g.DeleteDC(sdc)
    g.DeleteObject(bmp)
    g.DeleteDC(mdc)
    u.ReleaseDC(h, hdc)
    raw = bytearray(buf.raw)
    raw[0::4], raw[2::4] = raw[2::4], raw[0::4]          # BGRX -> RGBX
    stride = w * 4
    rows = b''.join(b'\x00' + bytes(raw[y * stride:(y + 1) * stride]) for y in range(hh))

    def ch(t, d):
        return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d) & 0xffffffff)
    # RGBA with the X byte forced opaque
    rows = bytearray(rows)
    for y in range(hh):
        base = y * (stride + 1) + 1
        rows[base + 3:base + stride:4] = b'\xff' * w
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + ch(b'IHDR', struct.pack('>IIBBBBB', w, hh, 8, 6, 0, 0, 0))
                           + ch(b'IDAT', zlib.compress(bytes(rows), 6)) + ch(b'IEND', b''))
    print(w, hh, path)
    return True


if len(sys.argv) == 2:
    grab(sys.argv[1])
else:
    import json
    sys.path.insert(0, r'D:\emulators\gameTesting\tools')
    import dolphin_mem
    out, n, dt = sys.argv[1], int(sys.argv[2]), float(sys.argv[3])
    os.makedirs(out, exist_ok=True)
    mem = None
    for _ in range(n):
        stamp = time.strftime('%H%M%S')
        ok = grab(os.path.join(out, f'frame_{stamp}.png'))
        try:   # the two robos' bone matrices at the same moment (robo_preview.py --gamemats)
            mem = mem or dolphin_mem.open_mem1()
            mats = {}
            for r in range(2):
                raw = mem.read(0x8024E120 + r * 0x33520 + 0x1434, 20 * 48)
                ms = [list(struct.unpack_from('>12f', raw, i * 48)) for i in range(20)]
                tx, tz = ms[0][3], ms[0][11]
                for M in ms:
                    M[3] -= tx
                    M[11] -= tz
                mats[str(r)] = ms
                # every part joint's world matrix as the game drew it (JObj tree depth-first = JOBJ_n order):
                # robo +0x944 gun, +0x948 bomb, +0x94C / +0x950 pods, +0x954 / +0x958 / +0x95C / +0x960 legs
                parts = {}
                for off in range(0x944, 0x964, 4):
                    root = mem.u32(0x8024E120 + r * 0x33520 + off)
                    js, stack = [], [root]
                    while stack and len(js) < 64:
                        j = stack.pop()
                        if not 0x80000000 <= j < 0x84000000:
                            continue
                        m12 = list(struct.unpack('>12f', mem.read(j + 0x44, 48)))
                        m12[3] -= tx
                        m12[11] -= tz
                        js.append(m12)
                        nxt, child = mem.u32(j + 0x8), mem.u32(j + 0x10)
                        if nxt:
                            stack.append(nxt)
                        if child:
                            stack.append(child)
                    parts[hex(off)] = js
                mats[f'parts{r}'] = parts
            mats['seq'] = mem.u32(0x8044F2EC) if hasattr(mem, 'u32') else None
            json.dump(mats, open(os.path.join(out, f'mats_{stamp}.json'), 'w'))
        except Exception as e:
            mem = None
            print('mats:', e)
        time.sleep(dt)
