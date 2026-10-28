"""Find the fields hidden in a cr_ struct's unknown stretches (u8 unk_XXXX[N]) from how the C accesses them, and split
the stretches into typed fields.

Reads symbols/cr_types.h and decompiled/main_dol.c. An access like  *(float *)(robo->unk_194c + 0x10)  is a 4-byte
float at struct offset 0x194C + 0x10;  robo->unk_0bc8[3]  is a byte at 0xBC8 + 3. A field is created at an offset only
when every access there agrees on one size and it does not overlap the next field; everything else stays u8 unk_.
New fields keep unk_XXXX names (meaning unknown) with a "// auto: N uses in M functions" note - give them real names
by hand when the code shows what they are.

usage: python -I tools/struct_fields.py <struct> report      -> docs/struct_<struct>.md (offset, size, kind, uses)
       python -I tools/struct_fields.py <struct> apply       -> rewrites the struct's unk_ stretches in cr_types.h
"""
import collections, os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SIZES = {'float': 4, 'int': 4, 'uint': 4, 'undefined4': 4, 'long': 4, 'ulong': 4, 'f32': 4, 's32': 4, 'u32': 4,
         'short': 2, 'ushort': 2, 'undefined2': 2, 's16': 2, 'u16': 2,
         'char': 1, 'byte': 1, 'uchar': 1, 'undefined1': 1, 'undefined': 1, 'bool': 1, 'u8': 1, 's8': 1,
         'double': 8, 'undefined8': 8, 'longlong': 8, 'ulonglong': 8}
HDR = re.compile(r'^// ([0-9a-f]{8})  (\S+)$', re.M)
OFF = r'(0x[0-9a-f]+|\d+)'


def struct_block(text, name):
    m = re.search(r'typedef struct ' + name + r' \{\n(.*?)\n\} ' + name + ';', text, re.S)
    if not m: sys.exit(f'struct {name} not found in symbols/cr_types.h')
    return m


def gaps_of(block):
    """unk stretches: name -> (offset, length)"""
    return {m.group(2): (int(m.group(1), 16), int(m.group(3), 16))
            for m in re.finditer(r'/\* \+([0-9A-F]+) \*/ u8 (unk_[0-9a-f]+)\[0x([0-9A-F]+)\];', block)}


def kind_of(cast, stars):
    if stars > 1: return 'ptr', 4
    base = cast.split()[-1]
    if base not in SIZES and stars == 1: return 'ptr', 4   # *(SomeStruct **) style handled above; unknown -> skip
    size = SIZES.get(base)
    if size is None: return None, None
    if base in ('float', 'f32'): return 'f32', 4
    if base == 'double': return 'f64', 8
    signed = base in ('int', 'short', 'char', 'long', 's32', 's16', 's8', 'longlong')
    return ('s' if signed else 'u') + str(size * 8), size


def scan(gaps, c):
    """-> {abs offset: Counter((kind, size))}, {abs offset: set(functions)}"""
    names = '|'.join(gaps)
    acc = collections.defaultdict(collections.Counter)
    funcs = collections.defaultdict(set)
    heads = [(m.start(), m.group(2)) for m in HDR.finditer(c)]
    def func_at(pos):
        lo, hi = 0, len(heads) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if heads[mid][0] <= pos: lo = mid
            else: hi = mid - 1
        return heads[lo][1]
    pats = [  # (regex, has offset group)
        (re.compile(r'\*\(([\w ]+?)\s*(\*+)\)\s*\([^()]*?->(' + names + r')\s*\+\s*' + OFF + r'\s*\)'), True),
        (re.compile(r'\*\(([\w ]+?)\s*(\*+)\)\s*[\w.\[\]>-]*?->(' + names + r')\b(?!\s*[\[+])'), False),
    ]
    for rx, has_off in pats:
        for m in rx.finditer(c):
            kind, size = kind_of(m.group(1).strip(), len(m.group(2)))
            if not kind: continue
            off = gaps[m.group(3)][0] + (int(m.group(4), 0) if has_off else 0)
            acc[off][(kind, size)] += 1; funcs[off].add(func_at(m.start()))
    for m in re.finditer(r'->(' + names + r')\[' + OFF + r'\]', c):          # byte index
        off = gaps[m.group(1)][0] + int(m.group(2), 0)
        acc[off][('u8', 1)] += 1; funcs[off].add(func_at(m.start()))
    return acc, funcs


def plan(gaps, acc):
    """-> {gap name: [(offset, kind, size) fields]} only consistent, non-overlapping, inside the gap"""
    out = {}
    for name, (start, length) in gaps.items():
        fields, end = [], start
        for off in sorted(o for o in acc if start <= o < start + length):
            kinds = acc[off]
            sizes = {s for _, s in kinds}
            if len(sizes) != 1: continue                                    # disagreeing sizes: leave unknown
            size = sizes.pop()
            kind = kinds.most_common(1)[0][0][0]
            if size == 1 and len(kinds) > 1: kind = 'u8'
            if off < end or off + size > start + length or off % min(size, 4): continue
            fields.append((off, kind, size)); end = off + size
        # drop a field that overlaps the next one
        out[name] = [f for i, f in enumerate(fields) if i + 1 == len(fields) or f[0] + f[2] <= fields[i + 1][0]]
    return out


C_TYPE = {'f32': 'f32', 'f64': 'f64', 'ptr': 'void*', 'u32': 'u32', 's32': 's32', 'u16': 'u16', 's16': 's16',
          'u8': 'u8', 's8': 's8', 'u64': 'u64', 's64': 's64'}


def main():
    if len(sys.argv) != 3 or sys.argv[2] not in ('report', 'apply'): sys.exit(__doc__)
    name, mode = sys.argv[1], sys.argv[2]
    tp = os.path.join(G, 'symbols', 'cr_types.h')
    text = open(tp, encoding='utf-8').read()
    m = struct_block(text, name)
    gaps = gaps_of(m.group(1))
    c = open(os.path.join(G, 'decompiled', 'main_dol.c'), encoding='utf-8', errors='replace').read()
    acc, funcs = scan(gaps, c)
    fields = plan(gaps, acc)
    if mode == 'report':
        lines = [f'# {name}: accesses inside unknown stretches (generated by tools/struct_fields.py)', '',
                 '| offset | accesses (kind/size x count) | functions | becomes field |', '|---|---|---|---|']
        chosen = {f[0] for fl in fields.values() for f in fl}
        for off in sorted(acc):
            ks = ', '.join(f'{k}/{s} x{n}' for (k, s), n in acc[off].most_common())
            fs = ', '.join(sorted(funcs[off])[:4]) + (' ...' if len(funcs[off]) > 4 else '')
            lines.append(f'| +0x{off:04X} | {ks} | {len(funcs[off])}: {fs} | {"yes" if off in chosen else "no (conflict/overlap)"} |')
        open(os.path.join(G, 'docs', f'struct_{name}.md'), 'w', encoding='utf-8').write('\n'.join(lines) + '\n')
        print(f'{name}: {len(acc)} offsets accessed in unknown stretches, {len(chosen)} can become fields -> docs/struct_{name}.md')
        return
    block = m.group(1)
    total = 0
    for gname, (start, length) in gaps.items():
        fl = fields[gname]
        if not fl: continue
        rows, pos = [], start
        for off, kind, size in fl:
            if off > pos: rows.append(f'    /* +{pos:04X} */ u8 unk_{pos:04x}[0x{off - pos:X}];')
            n = sum(acc[off].values()); nf = len(funcs[off])
            rows.append(f'    /* +{off:04X} */ {C_TYPE[kind]} unk_{off:04x};'.ljust(44) + f'// auto: {n} uses in {nf} functions')
            pos = off + size; total += 1
        if pos < start + length: rows.append(f'    /* +{pos:04X} */ u8 unk_{pos:04x}[0x{start + length - pos:X}];')
        old = re.search(r'    /\* \+' + f'{start:04X}' + r' \*/ u8 ' + gname + r'\[0x[0-9A-F]+\];[^\n]*', block).group(0)
        block = block.replace(old, '\n'.join(rows), 1)
    text = text[:m.start(1)] + block + text[m.end(1):]
    open(tp, 'w', encoding='utf-8').write(text)
    print(f'{name}: {total} typed fields created in {sum(1 for f in fields.values() if f)} unknown stretches')


if __name__ == '__main__':
    main()
