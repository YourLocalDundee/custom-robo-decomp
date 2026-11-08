"""Story scripts (files/scenario/snr_*.bin, "SCR\0"): decode commands; command sizes come from each handler in
main.dol (tools/scr_sizes.tsv: number, name, size in bytes; read from where each handler advances cr_g_ScriptPC).

File: "SCR\0", u32 size, ..., then from 0x40 (u32 offset, u32 id) entry points until an offset of 0; code is
big-endian s16 words: opcode (index into cr_g_ScrCmdNames / cr_g_ScrCmdFuncs), then its arguments.

  python -I tools/scr.py dump <file> [entry]       decoded commands from one / every entry point
  python -I tools/scr.py grep <command> [...]      every use of these commands in all scripts, with arguments
"""
import glob
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[1]
SIZES = {int(a): (b, int(c)) for a, b, c in (l.split('\t') for l in (G / 'tools' / 'scr_sizes.tsv').read_text().split('\n') if l.strip())}
END = {'SYS_SCR_END', 'SYS_RET'}


def entries(d):
    out, p = [], 0x40
    while True:
        o, i = struct.unpack_from('>II', d, p)
        if o == 0 or o >= len(d):
            return out
        out.append((o, i))
        p += 8


def walk(d, p, limit=100000):
    for _ in range(limit):
        if p + 2 > len(d):
            return
        op = struct.unpack_from('>h', d, p)[0]
        if op not in SIZES:
            yield p, '?%#x' % (op & 0xFFFF), ()
            return
        name, size = SIZES[op]
        args = struct.unpack_from('>%dh' % (size // 2 - 1), d, p + 2) if size > 2 else ()
        yield p, name, args
        p += size
        if name in END:
            return


def scripts():
    return sorted(glob.glob(str(G / 'Custom Robo extracted' / 'files' / 'scenario' / 'snr_*.bin')))


def main():
    sys.stdout.reconfigure(encoding='utf-8', errors='replace')
    cmd = sys.argv[1]
    if cmd == 'dump':
        d = Path(sys.argv[2]).read_bytes()
        es = entries(d)
        sel = [es[int(sys.argv[3])]] if len(sys.argv) > 3 else es
        for o, i in sel:
            print('== entry %#x (id %#x)' % (o, i))
            for p, n, a in walk(d, o):
                print('  %#06x %s %s' % (p, n, list(a)))
    elif cmd == 'grep':
        want = set(sys.argv[2:])
        for f in scripts():
            d = Path(f).read_bytes()
            seen = set()
            for o, i in entries(d):
                for p, n, a in walk(d, o):
                    if n in want and p not in seen:
                        seen.add(p)
                        print('%s\t%#x\t%s\t%s' % (Path(f).name, p, n, ' '.join(map(str, a))))


if __name__ == '__main__':
    main()
