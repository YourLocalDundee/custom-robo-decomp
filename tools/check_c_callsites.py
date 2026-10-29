"""Names from the matched C's call sites and definitions (plan.md Naming policy, rules R7 / R8; Phase 4 Milestone 4).

A C file whose function is 100% in objdiff (same instructions as main.dol) calls / uses other functions and data BY
NAME: each such reference is a relocation of the compiled object against an undefined symbol (or against one of the
file's own globals, step 56). main.dol's own
relocation at the same instruction (the built main.elf keeps every relocation, absolute addresses) points at the
retail address of that name. So the matched C names what it calls, the way every GC decomp names its symbols.

Run by batch 9 after the build (ninja has compiled every decomp/src file, objdiff written build/GXCE01/report.json,
and main.elf is the retail-SHA-1 link):
  1. For every C object (decomp/build/GXCE01/src/<object>.o), every function objdiff reports at 100% (fuzzy match),
     and every relocation of it against an undefined symbol NAME (or a global the C file defines) at function offset d: main.elf's relocation at
     <the function's retail address> + d must be of the same type; its target minus the C relocation's addend is
     NAME's address.
  2. Every `// c-callsite` line of symbols/symbols.txt is checked: its name must be derived, at exactly its address
     (every call site of every matched function agrees), and the call site it cites (obj:<C file> at:<function>+0xD)
     must be one of them. Any other result: FAIL, exit 1.
  2b. Every `// c-define obj:<C file>` line (rule R8): tools/gen_dtk_splits.py's decomp/reports/splits_report.txt must
     list that C file defining exactly that name at that address, in that section, with that size (`DEF` lines: the C
     file matches its whole object by bytes and relocations, its data placed by its own uses). Else FAIL.
  3. decomp/reports/c_callsites_report.txt (CRLF, deterministic): the checked lines, then every derived name whose
     address still has a dtk default name (fn_ / lbl_ ...: a name the next step can add) and every derived name that
     differs from the name main.elf has there (a disagreement to look at; not counted as a failure, because a name
     of ours can be a reference's alias, e.g. a weak symbol).
Usage: python -I tools/check_c_callsites.py
"""
import json
import os
import re
import struct
import sys
from collections import defaultdict

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import reloc_audit as ra  # noqa: E402

D = os.path.join(G, 'decomp')
B = os.path.join(D, 'build', 'GXCE01')
SYMBOLS = os.path.join(G, 'symbols', 'symbols.txt')
REPORT = os.path.join(D, 'reports', 'c_callsites_report.txt')
SPLITS_REPORT = os.path.join(D, 'reports', 'splits_report.txt')
LINE = re.compile(r'^(\S+) = \.(\w+):0x([0-9A-Fa-f]{8}); // .*// c-callsite obj:(\S+) at:(\S+)\+0x([0-9A-Fa-f]+)')
DLINE = re.compile(r'^(\S+) = (\.\w+):0x([0-9A-Fa-f]{8}); // .*size:0x([0-9A-Fa-f]+).*// c-define obj:(\S+) ')
DEF = re.compile(r'^DEF 0x([0-9A-F]{8}) (\S+) (\S+) (\S+) size:0x([0-9A-F]+) ')
DEFAULT = re.compile(r'^(fn|lbl|jumptable|gap|switch|float|double|string|wstring)_[0-9A-Fa-f_]+$|^@')
RTYPE = {1: 'ADDR32', 4: 'ADDR16_LO', 5: 'ADDR16_HI', 6: 'ADDR16_HA', 10: 'REL24', 11: 'REL14', 109: 'EMB_SDA21'}


def obj_relocs(path):
    """-> (functions {name: (section index, offset, size)}, [(section index, offset, type, undefined name, addend)])"""
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('>I', d, 0x20)
    shentsize, shnum, _ = struct.unpack_from('>HHH', d, 0x2E)
    sh = [struct.unpack_from('>10I', d, shoff + i * shentsize) for i in range(shnum)]
    symsec = next(s for s in sh if s[1] == 2)
    strt = sh[symsec[6]]
    syms = []
    for o in range(symsec[4], symsec[4] + symsec[5], 16):
        n, v, sz, info, _, shn = struct.unpack_from('>IIIBBH', d, o)
        s = strt[4] + n
        syms.append((d[s:d.index(b'\0', s)].decode('latin-1'), v, sz, info & 15, shn, info >> 4))
    funcs = {n: (shn, v, sz) for n, v, sz, t, shn, b in syms if t == 2 and shn and n}
    rels = []
    for s in sh:
        if s[1] != 4:
            continue
        for o in range(s[4], s[4] + s[5], 12):
            off, info, add = struct.unpack_from('>IIi', d, o)
            sy = syms[info >> 8]
            if sy[0] and (sy[4] == 0 or (sy[5] == 1 and sy[4] < 0xFF00)):   # undefined, or this file's own global
                rels.append((s[7], off, info & 255, sy[0], add))
    return funcs, rels


def main():
    rep = json.load(open(os.path.join(B, 'report.json')))
    od = json.load(open(os.path.join(D, 'objdiff.json')))
    base = {u['name']: u.get('base_path') for u in od['units']}
    elf = ra.Elf(os.path.join(B, 'main.elf'))
    names_at = defaultdict(list)
    for n, v, sz, t, b, shn in elf.syms:
        if n and t not in (3, 4) and not n.startswith('...'):   # any name main.elf has there: labels, linker symbols too
            names_at[v].append(n)
    derived = defaultdict(lambda: defaultdict(list))   # name -> address -> [site text]
    bad_sites = []
    for u in sorted(rep['units'], key=lambda u: u['name']):
        bp = base.get(u['name'])
        if not bp or not os.path.exists(os.path.join(D, bp)):
            continue
        obj = u['name'][len('main/'):]
        full = {f['name']: int(f['metadata']['virtual_address']) for f in u.get('functions', [])
                if f.get('fuzzy_match_percent') == 100.0 and f.get('metadata', {}).get('virtual_address')}
        funcs, rels = obj_relocs(os.path.join(D, bp))
        for sec, off, typ, name, add in rels:
            own = [(fn, fo) for fn, (fs, fo, fsz) in funcs.items() if fs == sec and fo <= off < fo + fsz and fn in full]
            if not own:
                continue
            fn, fo = own[0]
            site = full[fn] + off - fo
            r = elf.relocs.get(site)
            if r is None and typ == 109:
                r = elf.relocs.get(site & ~3)   # mwld puts a linked SDA21 relocation on the instruction, not its field
            where = f'{obj} {fn}+0x{off - fo:X} (0x{site:08X} {RTYPE.get(typ, typ)})'
            if not r or r[0] != typ:
                bad_sites.append(f'{where}: main.elf has {"no relocation" if not r else "type " + str(r[0])} there')
                continue
            derived[name][(r[1] - add) & 0xFFFFFFFF].append(where)   # the C's addend (a field of NAME) taken off
    errors, rows = [], []
    lines = [ln for ln in open(SYMBOLS, encoding='utf-8').read().splitlines() if '// c-callsite ' in ln]
    for ln in lines:
        m = LINE.match(ln)
        if not m:
            errors.append(f'FAIL unparsed c-callsite line: {ln}')
            continue
        name, addr = m[1], int(m[3], 16)
        cite = f'{os.path.splitext(m[4])[0]} {m[5]}+0x{int(m[6], 16):X} '   # as the derived sites are written
        got = derived.get(name, {})
        if set(got) != {addr}:
            errors.append(f'FAIL {name} 0x{addr:08X}: derived ' + (', '.join(f'0x{a:08X}' for a in sorted(got)) or 'nothing'))
        elif not any(s.startswith(cite) for s in got[addr]):
            errors.append(f'FAIL {name} 0x{addr:08X}: the cited call site {m[4]} {m[5]}+0x{int(m[6], 16):X} is not one of the derived ones')
        else:
            rows.append(f'OK   {name} 0x{addr:08X}: {len(got[addr])} call site(s), all agree; e.g. {got[addr][0]}')
    # c-define (rule R8): the C file matches its whole object (tools/gen_dtk_splits.py: every function's bytes and
    # relocations are main.dol's, its data placed by its own uses) and defines NAME at that address
    defs = defaultdict(set)
    for ln in open(SPLITS_REPORT, encoding='utf-8').read().splitlines():
        d = DEF.match(ln)
        if d:
            defs[d[2]].add((int(d[1], 16), d[3], d[4], int(d[5], 16)))
    dlines = [ln for ln in open(SYMBOLS, encoding='utf-8').read().splitlines() if '// c-define ' in ln]
    for ln in dlines:
        m = DLINE.match(ln)
        if not m:
            errors.append(f'FAIL unparsed c-define line: {ln}')
            continue
        name, sec, addr, size, obj = m[1], m[2], int(m[3], 16), int(m[4], 16), m[5]
        got = {(a, s, z) for a, o, s, z in defs.get(name, ()) if o == obj}
        if got != {(addr, sec, size)}:
            errors.append(f'FAIL {name} 0x{addr:08X}: {obj} defines it at ' +
                          (', '.join(f'{s}:0x{a:08X} size 0x{z:X}' for a, s, z in sorted(got)) or 'no place (splits_report.txt)'))
        else:
            rows.append(f'OK   {name} 0x{addr:08X}: defined there by {obj} ({sec}, size 0x{size:X})')
    lines += dlines
    proposed, differ = [], []
    for name in sorted(derived):
        for a in sorted(derived[name]):
            have = names_at.get(a, [])
            if name in have:
                continue
            txt = f'{name} 0x{a:08X} (main.elf: {", ".join(have) or "no symbol"}): {len(derived[name][a])} site(s), e.g. {derived[name][a][0]}'
            (proposed if all(DEFAULT.match(h) for h in have) else differ).append(txt)
    multi = [f'{n}: ' + ', '.join(f'0x{a:08X}' for a in sorted(derived[n])) for n in sorted(derived) if len(derived[n]) > 1]
    out = [f'c-callsite names (tools/check_c_callsites.py): {len(lines)} lines in symbols/symbols.txt, {len(rows)} OK, '
           f'{len(errors)} FAIL; {sum(len(v) for v in derived.values())} names derived from matched C call sites, '
           f'{len(proposed)} at a dtk default name, {len(differ)} differ from our name, {len(multi)} at two addresses, '
           f'{len(bad_sites)} sites without the same relocation', '']
    out += errors + rows + [''] + ['UNNAMED ' + p for p in proposed] + ['DIFFERS ' + p for p in differ] + \
        ['TWO ADDRESSES ' + p for p in multi] + ['NO RELOCATION ' + p for p in sorted(bad_sites)]
    with open(REPORT, 'w', encoding='utf-8', newline='\r\n') as f:
        f.write('\n'.join(out) + '\n')
    print(out[0])
    for e in errors:
        print(e)
    return 1 if errors else 0


if __name__ == '__main__':
    sys.exit(main())
