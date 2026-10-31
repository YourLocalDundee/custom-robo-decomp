"""Harvest the real names the game itself still contains, game-wide, before anything gets a made-up name.

Sources (all from the game's own main.dol):
  1. name tables  - runs of 4-byte words in data that point at strings: an enum's names in index order
                    (e.g. SEQ_LOGO, SEQ_MOVIE, ...). Listed with the code that uses each table.
  2. named strings - identifier-like strings (UPPER_NAMES, lower_snake, CamelCase), source file names, format
                    strings, with the functions that reference them directly.
  3. robo bodies   - the body codes in the file names of the body pictures (archive 36, CR_S1_<code>.tpl) and the two
                    names at the start of each body stats record (0x801C6C44, 0x134 per body), by body number
                    (plan.md step 71, docs/robo_tables.md).
  4. leads         - behaviour seen in the code that is not proven enough for a name yet (LEADS below, kept by hand).
Writes docs/name_evidence.md (and docs/name_tables.tsv). Nothing is applied: names go into symbols/ by hand, quoting
this evidence (CONTRIBUTING.md, "real names first").
usage: python -I tools/name_evidence.py
"""
import collections, os, re, sqlite3, struct, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import make_mod  # noqa: E402  (dol_offset)

KINDS = [('UPPER_NAME', re.compile(r'[A-Z][A-Z0-9_]{3,}')),
         ('lower_snake', re.compile(r'[a-z][a-z0-9]*(?:_[a-z0-9]+)+')),
         ('CamelCase', re.compile(r'[A-Za-z][A-Za-z0-9]*[A-Z][a-z][A-Za-z0-9]*')),
         ('source file', re.compile(r'\w+\.[ch]')),
         ('format', re.compile(r'.*%[0-9.]*[sdxXf].*'))]


# Hand-kept leads (plan.md step 71): what the code shows, not enough for a cr_ name yet (or the address is used by name
# in a C file of decomp/src, so it cannot be renamed here). Printed as section 4.
LEADS = [
    ('0x8024E120', 'battle robos: 4 x 0x33520 bytes (body +0x1CC, HP +0x74, body stats copy +0x1B98); used as lbl_8024E120 '
                   'in decomp/src, so it keeps its lbl_ name until that C is renamed with it'),
    ('0x8044F348', 'current battle robo pointer (cr_Battle_SetCurrentPointers); lbl_8044F348 in decomp/src'),
    ('0x801C6C44', 'body stats table (34 x 0x134; names at +0x00 / +0x20, special-part byte +0x45, u16 +0x40 = 1000 for '
                   'Ray 01 and 30000 for R Type 3, HP-like but unproven); lbl_801C6C44 in another stream\'s C'),
    ('0x801C5BD0', 'battle loadouts (4 x 0x10: team, body, gun, bomb, pod, leg, HP, controller); lbl_801C5BD0 in another '
                   'stream\'s C'),
    ('0x8023BA84', 'part load requests (4 players x 5 categories x 0x14, 0xFF end); lbl_8023BA84 in another stream\'s C'),
    ('0x80075950', 'loads the gun / bomb / pod / leg effect entries and the body\'s attack_effect entry of the 4 loadouts; '
                   'fn_80075950 is used by name in another stream\'s C'),
    ('0x80066948', '(message file, message) -> text of bmg/info*.bin; fn_80066948 is called from decomp/src'),
    ('0x801DE868', 'a second copy of the weapon / body request lists (16 weapon + 4 body requests, 0xFF ends, buffer '
                   'offsets); no code or data reference found'),
    ('0x803BF79C', 'part bitsets, one save area (5 categories x 0x20 bytes = 4 x 64 bits); 0x803BFB9C is the other area, '
                   'picked by mode flag 0x800000 of 0x80321D40; what the two areas and sets 2-4 mean is unproven'),
    ('0x800F9014', 'part name + description for a menu cursor (the cr_Text_Get*Name of the category, '
                   'cr_Text_MaskUnownedSpecialPart, cr_Text_GetPartDescription into 0x80400604); fn_800F8820 not traced'),
    ('0x800A63C0', 'part viewer / new-part screen: category 0x803BFC95, part 0x803BFC96, opens chara.bin entry part x 5 '
                   'for a body; which screen it is is not proven'),
    ('0x80065DB0', 'two 0x28-byte windows at 0x8032CC20: state +0 = 3 when +8 is set (close request?); caller cr_SYS_WINDOW_CLOSE'),
    ('0x8012481C', 'menu sprites 0x8044F860 / 0x8044F85C: flag 1 and part ids chosen from bits 2..0x20; screen unknown'),
]


def kind(v):
    if '/' in v: return None
    for k, rx in KINDS:
        if rx.fullmatch(v): return k
    return None


def main():
    con = sqlite3.connect(os.path.join(G, 'decompiled', 'index.db'))
    strings = {a: v for a, v in con.execute('select addr, value from strings') if v}
    name_of = dict(con.execute('select addr, name from funcs'))
    users = collections.defaultdict(set)
    for f, t in con.execute('select func, target from refs'):
        users[t].add(f)
    dol = open(os.path.join(G, 'Custom Robo extracted', 'sys', 'main.dol'), 'rb').read()
    offs = struct.unpack_from('>18I', dol, 0); addrs = struct.unpack_from('>18I', dol, 0x48)
    sizes = struct.unpack_from('>18I', dol, 0x90)

    # 1. name tables: consecutive words pointing at strings (data sections only: 7..17)
    tables = []
    for o, a, s in list(zip(offs, addrs, sizes))[7:]:
        if not s: continue
        run = []
        for i in range(0, s - 3, 4):
            w = struct.unpack_from('>I', dol, o + i)[0]
            if w in strings:
                run.append((a + i, w))
            else:
                if len(run) >= 3: tables.append(run)
                run = []
        if len(run) >= 3: tables.append(run)

    def fn_users(addr_lo, addr_hi):
        fs = set()
        for t in range(addr_lo, addr_hi + 1, 4):
            fs |= users.get(t, set())
        return sorted(name_of.get(f, f'{f:08X}') for f in fs)

    lines = ['# Real-name evidence (generated by tools/name_evidence.py)', '',
             'The game\'s own names, found in main.dol. Use these before inventing a `cr_` name (CONTRIBUTING.md).', '',
             f'## Name tables ({len(tables)}): pointer arrays to strings - an enum\'s names in index order', '',
             '| table | entries | first names | used by |', '|---|---|---|---|']
    tsv = ['table\tindex\tname']
    for run in sorted(tables, key=lambda r: -len(r)):
        base = run[0][0]
        names = [strings[w] for _, w in run]
        if not any(kind(n) for n in names[:4]): continue   # skip tables of plain text / paths
        us = fn_users(base, base)
        lines.append(f'| 0x{base:08X} | {len(run)} | {", ".join(n[:28] for n in names[:5])}{" ..." if len(names) > 5 else ""} '
                     f'| {", ".join(us[:4]) or "(no direct code reference)"} |')
        tsv += [f'{base:08X}\t{i}\t{n}' for i, n in enumerate(names)]

    by_kind = collections.defaultdict(list)
    for a, v in sorted(strings.items()):
        k = kind(v)
        if k: by_kind[k].append((a, v))
    lines += ['', '## Named strings with the code that uses them directly', '']
    for k, items in by_kind.items():
        used = [(a, v, sorted(name_of.get(f, '?') for f in users.get(a, ()))) for a, v in items]
        direct = [u for u in used if u[2]]
        lines += [f'### {k}: {len(items)} strings, {len(direct)} referenced directly by code', '',
                  '| string | used by |', '|---|---|']
        lines += [f'| `{v[:60]}` | {", ".join(fs[:4])} |' for a, v, fs in direct[:400]]
        lines.append('')
    # 3. robo bodies: archive 36's file names (pointer list 0x801F1200[36] -> 0x10-byte descs) and the stats names
    def rd(addr, n):
        for o, a, sz in zip(offs, addrs, sizes):
            if sz and a <= addr and addr + n <= a + sz: return dol[o + addr - a:o + addr - a + n]
        return b''
    def u32(addr): return struct.unpack('>I', rd(addr, 4))[0]
    def cstr(addr, n): return rd(addr, n).split(b'\0')[0]
    lines += ['## Robo bodies: the game\'s own body codes and stats names, by body number', '',
              'Codes from the body pictures `sprite/parts_illustrated_book/body_tpl/CR_S1_<code>.tpl` (archive 36, one file '
              'per body); names from the body stats record (0x801C6C44 + body x 0x134: +0x20 romaji, +0x00 Shift-JIS). '
              'The US names are in bmg/info_usa.bin message file 9. Tables and what grows for a new body: '
              '[robo_tables.md](robo_tables.md).', '',
              '| body | code | stats name (+0x20) | stats name (+0x00) |', '|---|---|---|---|']
    desc = u32(0x801F1200 + 36 * 4)
    for b in range(34):
        path = cstr(u32(desc + b * 0x10), 0x80).decode('latin1')
        code = path.rsplit('CR_S1_', 1)[-1].split('.')[0]
        rec = 0x801C6C44 + b * 0x134
        en = ''.join(c if 0x20 <= ord(c) < 0x7F else '?' for c in cstr(rec + 0x20, 0x20).decode('latin1'))
        jp = cstr(rec, 0x20).decode('shift_jis', 'replace')
        lines.append(f'| {b} | {code} | {en} | {jp} |')
    lines += ['', '## Leads (by hand, plan.md step 71): behaviour seen, not proven enough for a name yet', '',
              '| address | lead |', '|---|---|']
    lines += [f'| {a} | {t} |' for a, t in LEADS]
    lines.append('')
    open(os.path.join(G, 'docs', 'name_evidence.md'), 'w', encoding='utf-8').write('\n'.join(lines) + '\n')
    open(os.path.join(G, 'docs', 'name_tables.tsv'), 'w', encoding='utf-8').write('\n'.join(tsv) + '\n')
    print(f'name_evidence: {len(tables)} pointer tables to strings, '
          + ', '.join(f'{len(v)} {k}' for k, v in by_kind.items()) + ' -> docs/name_evidence.md, docs/name_tables.tsv')


if __name__ == '__main__':
    main()
