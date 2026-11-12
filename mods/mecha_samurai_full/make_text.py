"""The mecha samurai's names and descriptions in the game text (bmg/info_usa.bin, 29 BMG message files back to back).

  python -I mods/mecha_samurai_full/make_text.py        -> files/bmg/info_usa.bin

Message file 9 = part names, file 0 = part descriptions, both indexed by the running part index: body b, gun 34 + g,
bomb 98 + b, pod 150 + p, legs 207 + l (cr_Text_GetBodyName .. GetLegName add cr_g_BodyCount and the other counts;
docs/robo_tables.md #10 / #11). A BMG file: INF1 (u16 count, u16 entry size, entries: u32 offset into DAT1, ...) and
DAT1 (Shift-JIS strings, 0x1A escapes). The changed messages' DAT1 is rebuilt string by string and padded back to its
original size, so every file keeps its size and offset in info_usa.bin (the game reads the whole file; a bigger file
is not needed). Escapes at the start of a description (the {ff00010049} colour code) are kept.
"""
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
SRC = G / 'Custom Robo extracted' / 'files' / 'bmg' / 'info_usa.bin'

NAMES = {0: 'Samurai', 34: 'Tanegashima', 98: 'Horoku Bomb', 150: 'Sashimono Pod', 207: 'Kaze Legs'}
DESCS = {
    0: 'A mecha samurai in red lacquered\nplate. Low, wide stance and a fast\niai lunge.\nCan air-dash twice.\n'
       'Collision: Draws, then lunges\nwith a sweeping strike.',
    34: 'A twin-barrel arm cannon.\nFires two rounds side by side,\nthree times. Cyan and gold\ntrails.\nRange: medium.',
    98: 'A lacquered mortar gauntlet.\nLobs a horoku bomb.',
    150: 'Back-mounted pods in banner\ncases.',
    207: 'Calf thrusters with twin\ngold jets.',
}


def files(d):
    out, i = [], 0
    while True:
        j = d.find(b'MESGbmg1', i)
        if j < 0:
            return out
        out.append(j)
        i = j + 8


def sections(d, s):
    nsec = struct.unpack_from('>I', d, s + 0xC)[0]
    p, sec = s + 0x20, {}
    for _ in range(nsec):
        tag, ln = d[p:p + 4], struct.unpack_from('>I', d, p + 4)[0]
        sec[tag] = (p, ln)
        p += ln
    return sec


def raw_message(d, q):
    """the bytes of one message up to (not including) its 0, escapes kept"""
    b = bytearray()
    while d[q]:
        if d[q] == 0x1A:
            ln = d[q + 1]
            b += d[q:q + ln]
            q += ln
            continue
        b.append(d[q])
        q += 1
    return bytes(b)


def rewrite(d, s, new):
    """new: {index: bytes}; returns d with this BMG file's DAT1 rebuilt (same size)"""
    sec = sections(d, s)
    inf, _ = sec[b'INF1']
    dat, dln = sec[b'DAT1']
    n, esz = struct.unpack_from('>HH', d, inf + 8)
    offs = [struct.unpack_from('>I', d, inf + 0x10 + m * esz)[0] for m in range(n)]
    msgs = [raw_message(d, dat + 8 + o) for o in offs]
    for k, v in new.items():
        msgs[k] = v
    body = bytearray(b'\0')                       # DAT1 starts with an empty string in these files
    first = d[dat + 8]
    if first != 0:
        body = bytearray()
    newoffs = []
    for m in msgs:
        newoffs.append(len(body))
        body += m + b'\0'
    if len(body) > dln - 8:
        raise SystemExit(f'BMG file at {s:#x}: DAT1 would grow by {len(body) - (dln - 8)} bytes; shorten the texts')
    body += b'\0' * (dln - 8 - len(body))
    out = bytearray(d)
    out[dat + 8:dat + dln] = body
    for m, o in enumerate(newoffs):
        struct.pack_into('>I', out, inf + 0x10 + m * esz, o)
    return bytes(out)


def main():
    d = SRC.read_bytes()
    fs = files(d)
    names = {k: v.encode('shift_jis') for k, v in NAMES.items()}
    descs = {}
    s0 = fs[0]
    sec = sections(d, s0)
    inf, _ = sec[b'INF1']
    dat, _ = sec[b'DAT1']
    esz = struct.unpack_from('>H', d, inf + 10)[0]
    for k, v in DESCS.items():
        old = raw_message(d, dat + 8 + struct.unpack_from('>I', d, inf + 0x10 + k * esz)[0])
        pre = old[:old[1]] if old[:1] == b'\x1a' else b''          # keep the leading colour escape
        descs[k] = pre + v.encode('shift_jis')
    d = rewrite(d, fs[9], names)
    d = rewrite(d, files(d)[0], descs)
    out = MOD / 'files' / 'bmg' / 'info_usa.bin'
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(d)
    print(f'{out.relative_to(MOD)}: {len(d)} bytes (same as the disc: {len(d) == SRC.stat().st_size})')


if __name__ == '__main__':
    main()
