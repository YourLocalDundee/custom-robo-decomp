"""part_mounts.py [--check]: the part mount of every robo body (cr_g_BodyData, 0x801C5C10, 34 x 0x78: after two floats
and the motion set pointer, a scale and an offset vector per part; cr_BodyData in symbols/cr_types.h) as the Markdown
table of docs/robo_tables.md "Part mounts per body", read from the retail main.dol (decomp/orig/GXCE01/sys/main.dol)
with the body names of that doc's body table. --check: the table in the doc must equal the generated one (Nov 5,
2026; how the game uses the fields: docs/formats/model_entry.md "Part files")."""
import os
import re
import struct
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOC = os.path.join(G, 'docs', 'robo_tables.md')
TABLE, STRIDE, COUNT = 0x801C5C10, 0x78, 34


def dol_reader(path):
    d = open(path, 'rb').read()
    off, addr, size = (struct.unpack_from('>18I', d, o) for o in (0, 72, 144))

    def rd(a, n):
        for o, ad, s in zip(off, addr, size):
            if ad <= a < ad + s:
                return d[o + a - ad:o + a - ad + n]
        raise SystemExit(f'0x{a:08X} is not in main.dol')
    return rd


def table():
    names = {}
    for line in open(DOC, encoding='utf-8'):
        for m in re.finditer(r'\|\s*(\d+)\s*\|\s*(\w+)\s*\|\s*([^|]+?)\s*\|', line):
            names.setdefault(int(m.group(1)), m.group(3))
    rd = dol_reader(os.path.join(G, 'decomp', 'orig', 'GXCE01', 'sys', 'main.dol'))

    def v(t):
        return '(%s)' % ', '.join('%g' % x for x in t)
    rows = ['| body | name | gun offset | bomb offset | pod A offset | pod B offset | shin offset | non-1 scales |',
            '|---|---|---|---|---|---|---|---|']
    for b in range(COUNT):
        f = struct.unpack('>27f', rd(TABLE + b * STRIDE + 0xC, 108))
        vec = [f[i * 3:i * 3 + 3] for i in range(9)]
        odd = ', '.join(f'{k} {v(vec[i])}' for k, i in (('gun', 0), ('bomb', 2), ('pod', 4), ('leg', 7))
                        if any(abs(x - 1) > 1e-6 for x in vec[i])) or '-'
        rows.append(f'| {b} | {names.get(b, "?")} | {v(vec[1])} | {v(vec[3])} | {v(vec[5])} | {v(vec[6])} | '
                    f'{v(vec[8])} | {odd} |')
    return rows


def main(argv):
    rows = table()
    if '--check' in argv:
        doc = [l.rstrip('\r\n') for l in open(DOC, encoding='utf-8')]
        i = doc.index(rows[0]) if rows[0] in doc else -1
        ok = i >= 0 and doc[i:i + len(rows)] == rows
        print('part mounts table: ' + ('matches main.dol' if ok else 'DIFFERS from main.dol'))
        sys.exit(0 if ok else 1)
    print('\n'.join(rows))


if __name__ == '__main__':
    main(sys.argv[1:])
