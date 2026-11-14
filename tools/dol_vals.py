"""Values of main.dol data at an address, printed as C initialiser lists.

    python -I tools/dol_vals.py ADDR COUNT [TYPE]     TYPE: f32 (default), u32, s32, u16, s16, u8, s8, f64

ADDR is hex (0x801C3758 or lbl_801C3758). Used to write the initialiser of a local table whose constant m2c
shows as a run of stack words copied from a .rodata label (docs/drafts.md, Nov 13). f32 / f64 values are
printed with enough digits to round-trip, so the compiled constant is byte-identical to the original's.
"""
import os
import struct
import sys

R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOL = os.path.join(R, 'decomp', 'orig', 'GXCE01', 'sys', 'main.dol')
FMT = {'f32': ('>f', 4), 'u32': ('>I', 4), 's32': ('>i', 4), 'u16': ('>H', 2), 's16': ('>h', 2), 'u8': ('>B', 1),
       's8': ('>b', 1), 'f64': ('>d', 8)}


def read(addr, size):
    d = open(DOL, 'rb').read()
    for i in range(18):
        off, = struct.unpack_from('>I', d, i * 4)
        base, = struct.unpack_from('>I', d, 0x48 + i * 4)
        ln, = struct.unpack_from('>I', d, 0x90 + i * 4)
        if ln and base <= addr and addr + size <= base + ln:
            return d[off + addr - base: off + addr - base + size]
    raise SystemExit('address 0x%08X not in a loaded section of main.dol' % addr)


def f32s(b):
    v, = struct.unpack('>f', b)
    for digits in range(1, 10):
        s = '%.*g' % (digits, v)
        if struct.pack('>f', float(s)) == b:
            break
    s = repr(float(s))
    return s + 'f'


def f64s(b):
    v, = struct.unpack('>d', b)
    s = repr(v)
    return s


def values(addr, count, typ):
    fmt, sz = FMT[typ]
    raw = read(addr, count * sz)
    out = []
    for i in range(count):
        b = raw[i * sz:(i + 1) * sz]
        if typ == 'f32':
            out.append(f32s(b))
        elif typ == 'f64':
            out.append(f64s(b))
        else:
            v, = struct.unpack(fmt, b)
            out.append(('0x%X' % v) if typ[0] == 'u' and v > 9 else str(v))
    return out


def main():
    a = sys.argv[1]
    addr = int(a[4:] if a.startswith('lbl_') else a, 16)
    count = int(sys.argv[2], 0)
    typ = sys.argv[3] if len(sys.argv) > 3 else 'f32'
    print('{ ' + ', '.join(values(addr, count, typ)) + ' }')


if __name__ == '__main__':
    main()
