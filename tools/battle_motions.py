"""battle_motions.py: the battle motion set of every body, with motion names (read-only, standard library only).

usage: python -I tools/battle_motions.py [--dol PATH] [--body N] [--slot S] [--tsv]

cr_g_BattleMotionSets (main.dol 0x801C51B8): 34 sets of 0x4C bytes = 38 u16 motion requests, one set per body
(cr_g_BodyData 0x801C5C10 word 2 points at it, copied to battle robo +0x554; this tool follows that pointer, so
its rows are by body number). The battle code plays `set[cr_g_BattleMotionSlot]`
(cr_Motion_Play). A request is the motion ID (low 13 bits, & 0x1FFF) plus flag bits 0x2000 / 0x4000 / 0x8000;
0x8000 also selects the body's own joint animation frame (battle robo +0x85B, docs/IMPORT_EXPORT_GUIDE.md 3c).
Motion names: cr_g_MotionNames 0x80206DB0 (docs/name_tables.tsv, table 80206DB0): 3 strings per motion ID, the
name is string id * 3 (the viewer fn_800A0E64 prints [id * 3]).
"""
import argparse
import os
import struct

R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SETS, NSETS, SLOTS = 0x801C51B8, 34, 38


def dol_reader(path):
    b = open(path, 'rb').read()
    offs = struct.unpack_from('>18I', b, 0)
    addrs = struct.unpack_from('>18I', b, 0x48)
    sizes = struct.unpack_from('>18I', b, 0x90)

    def read(addr, n):
        for o, a, s in zip(offs, addrs, sizes):
            if s and a <= addr < a + s:
                return b[o + addr - a:o + addr - a + n]
        raise ValueError('address %08X not in the DOL' % addr)
    return read


def names():
    out = {}
    for line in open(os.path.join(R, 'docs', 'name_tables.tsv'), encoding='utf-8'):
        p = line.rstrip('\n').split('\t')
        if len(p) == 3 and p[0] == '80206DB0':
            i = int(p[1])
            if i % 3 == 0:          # cr_g_MotionNames: 3 strings per motion ID, the name first
                out[i // 3] = p[2]
    return out


def body_names():
    out = {}
    for line in open(os.path.join(R, 'docs', 'robo_tables.md'), encoding='utf-8'):
        p = [x.strip() for x in line.split('|')]
        if len(p) >= 8 and p[1].isdigit() and p[5].isdigit():
            out[int(p[1])] = p[3]
            out[int(p[5])] = p[7]
    return out


BODYDATA = 0x801C5C10   # cr_g_BodyData: 0x78 bytes per body, word 2 = pointer to the body's motion set


def set_index(dol):
    """body -> motion set number, through cr_g_BodyData word 2 (bodies 27..33 are not their own number: body 33,
    Chickenheart, added late, uses set 27; docs/robo_tables.md)"""
    read = dol_reader(dol)
    return [(struct.unpack('>I', read(BODYDATA + b * 0x78 + 8, 4))[0] - SETS) // 0x4C for b in range(NSETS)]


def sets(dol):
    """the 38 motion requests of each BODY (followed through cr_g_BodyData), index = body number"""
    read = dol_reader(dol)
    raw = read(SETS, NSETS * 0x4C)
    by_set = [list(struct.unpack_from('>38H', raw, i * 0x4C)) for i in range(NSETS)]
    return [by_set[s] for s in set_index(dol)]


def flags(req):
    return ''.join(c for bit, c in ((0x8000, 'T'), (0x4000, 'B'), (0x2000, 'A')) if req & bit)


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('--dol', default=os.path.join(R, 'decomp', 'orig', 'GXCE01', 'sys', 'main.dol'))
    ap.add_argument('--body', type=int)
    ap.add_argument('--slot', type=int)
    ap.add_argument('--tsv', action='store_true')
    a = ap.parse_args()
    nm, bn = names(), body_names()
    ss = sets(a.dol)
    if a.tsv:
        print('body\tbody_name\tslot\trequest\tmotion_id\tflags\tname')
    for i, s in enumerate(ss):
        if a.body is not None and i != a.body:
            continue
        if not a.tsv:
            print('== body %d %s' % (i, bn.get(i, '')))
        for k, req in enumerate(s):
            if a.slot is not None and k != a.slot:
                continue
            mid = req & 0x1FFF
            if a.tsv:
                print('%d\t%s\t%d\t0x%04X\t%d\t%s\t%s' % (i, bn.get(i, ''), k, req, mid, flags(req), nm.get(mid, '?')))
            else:
                print('  slot %2d  0x%04X  id %4d %-3s %s' % (k, req, mid, flags(req), nm.get(mid, '?')))
