"""Read bmg/info_usa.bin (29 BMG message files back to back): list files or dump one.

  python -I tools/bmg.py                  every file: number, message count, first messages
  python -I tools/bmg.py <file> [first] [count]   messages of one file (escapes shown as {..})
"""
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[1]
SRC = G / 'Custom Robo extracted' / 'files' / 'bmg' / 'info_usa.bin'


def files(d):
    out, i = [], 0
    while True:
        j = d.find(b'MESGbmg1', i)
        if j < 0:
            return out
        out.append(j)
        i = j + 8


def messages(d, s):
    nsec = struct.unpack_from('>I', d, s + 0xC)[0]
    p, inf, dat = s + 0x20, None, None
    for _ in range(nsec):
        tag, ln = d[p:p + 4], struct.unpack_from('>I', d, p + 4)[0]
        if tag == b'INF1':
            inf = p
        elif tag == b'DAT1':
            dat = p
        p += ln
    n, esz = struct.unpack_from('>HH', d, inf + 8)
    out = []
    for m in range(n):
        q = dat + 8 + struct.unpack_from('>I', d, inf + 0x10 + m * esz)[0]
        b = bytearray()
        while d[q]:
            if d[q] == 0x1A:                       # escape: 0x1A, length (incl. these two), payload
                ln = d[q + 1]
                b += b'{' + d[q + 2:q + ln].hex().encode() + b'}'
                q += ln
                continue
            b.append(d[q])
            q += 1
        out.append(bytes(b).decode('shift_jis', 'replace'))
    return out


def main():
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    d = SRC.read_bytes()
    fs = files(d)
    if len(sys.argv) < 2:
        for k, s in enumerate(fs):
            ms = messages(d, s)
            print(k, len(ms), ' | '.join(m.replace('\n', ' ')[:24] for m in ms[:4]))
        return
    k = int(sys.argv[1])
    first = int(sys.argv[2]) if len(sys.argv) > 2 else 0
    count = int(sys.argv[3]) if len(sys.argv) > 3 else 9999
    for i, m in enumerate(messages(d, fs[k])[first:first + count], first):
        print(i, m.replace('\n', ' / '))


if __name__ == '__main__':
    main()
