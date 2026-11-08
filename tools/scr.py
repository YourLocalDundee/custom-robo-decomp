"""Story scripts (files/scenario/snr_*.bin, "SCR\0"): decode commands; command sizes come from each handler in
main.dol (tools/scr_sizes.tsv: number, name, size in bytes; read from where each handler advances cr_g_ScriptPC).

File: "SCR\0", u32 size, ..., the build date as text at +0x20, then from 0x40 (u32 offset, u32 id) entry points
until an offset of 0; an entry's developer name is the string at id + 0x40 (the name table after the code). Code is
big-endian s16 words: opcode (index into cr_g_ScrCmdNames / cr_g_ScrCmdFuncs), then its arguments. Jump and call
targets (SYS_JP, SYS_CALL, MAP_INIT_SCRIPT ...) are entry-table offsets (entry index * 8). In the real chapter
scripts the code decodes without a gap from the first entry point to the name table (sweep()); the test scripts
snr_f_25..f_29 have data between their pieces of code.

  python -I tools/scr.py dump <file> [entry]       decoded commands from one / every entry point, with entry names
  python -I tools/scr.py grep <command> [...]      every use of these commands in all scripts (whole code), with arguments
  python -I tools/scr.py info <file>               build date and entry points with their names
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


def name(d, i):
    o = i + 0x40
    return d[o:d.index(b'\0', o)].decode('ascii', 'replace').lstrip('\0\x01\x02\x03\x04\x05\x06\x07\x08')


def sweep(d):
    """every command from the first entry point on, until a word that is not a command (the name table)"""
    p = min(o for o, i in entries(d))
    while p + 2 <= len(d):
        op = struct.unpack_from('>h', d, p)[0]
        if op not in SIZES:
            return
        n, s = SIZES[op]
        yield p, n, (struct.unpack_from('>%dh' % (s // 2 - 1), d, p + 2) if s > 2 else ())
        p += s


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
            print('== entry %#x (id %#x) %s' % (o, i, name(d, i)))
            for p, n, a in walk(d, o):
                print('  %#06x %s %s' % (p, n, list(a)))
    elif cmd == 'grep':
        want = set(sys.argv[2:])
        for f in scripts():
            d = Path(f).read_bytes()
            for p, n, a in sweep(d):
                if n in want:
                    print('%s\t%#x\t%s\t%s' % (Path(f).name, p, n, ' '.join(map(str, a))))
    elif cmd == 'info':
        d = Path(sys.argv[2]).read_bytes()
        print('built', d[0x20:d.index(b'\0', 0x20)].decode('ascii', 'replace'))
        for k, (o, i) in enumerate(entries(d)):
            print('  %3d  %#06x  %s' % (k, o, name(d, i)))


if __name__ == '__main__':
    main()
