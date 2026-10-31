"""Harvest the real names the game itself still contains, game-wide, before anything gets a made-up name.

Sources (all from the game's own main.dol):
  1. name tables  - runs of 4-byte words in data that point at strings: an enum's names in index order
                    (e.g. SEQ_LOGO, SEQ_MOVIE, ...). Listed with the code that uses each table.
  2. named strings - identifier-like strings (UPPER_NAMES, lower_snake, CamelCase), source file names, format
                    strings, with the functions that reference them directly.
  3. robo bodies   - the body codes in the file names of the body pictures (archive 36, CR_S1_<code>.tpl) and the two
                    names at the start of each body stats record (0x801C6C44, 0x134 per body), by body number
                    (plan.md step 71, docs/robo_tables.md).
  4. leads         - behaviour seen in the code that is not proven enough for a name yet (LEADS below, kept by hand; plan.md steps 71, 79, 81).
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
    ('0x803BF79C', "the save's unlock bits (cr_g_SaveData.unlocks, a cr_UnlockBits since step 81: five part categories x four 64-bit sets, then the stage set); cr_g_Scenario.part_bits (0x803BFB9C, picked by cr_g_GameMode.flags bit 0x800000) has the same shape; set 0 of a category is the owned parts, sets 1-3 are not traced"),
    ('0x800F9014', 'part name + description for a menu cursor (the cr_Text_Get*Name of the category, '
                   'cr_Text_MaskUnownedSpecialPart, cr_Text_GetPartDescription into 0x80400604); fn_800F8820 not traced'),
    ('0x800A63C0', 'part viewer / new-part screen: category 0x803BFC95, part 0x803BFC96, opens chara.bin entry part x 5 '
                   'for a body; which screen it is is not proven'),
    ('0x80065DB0', 'two 0x28-byte windows at 0x8032CC20: state +0 = 3 when +8 is set (close request?); caller cr_SYS_WINDOW_CLOSE'),
    ('0x8012481C', 'menu sprites 0x8044F860 / 0x8044F85C: flag 1 and part ids chosen from bits 2..0x20; screen unknown'),
    # step 79: the next data objects of the base-register scan (addressed as one struct from `lis/addi` of the object), untyped
    ('0x8031F020', "the movie player's state (0x188): cr_SEQ_MOVIE_FrameB / FrameC and fn_800177F0 / fn_80017E10 / fn_8001838C / fn_80018630 / fn_80018844 (DVDOpen / DVDReadAsyncPrio / strcmp) read 57 word fields and the bytes +0xA4..+0xA7 from one base; cr_SEQ_MOVIE_FrameA addresses it as lbl_8031B5A0 + 0x3A80 (one base register for several .bss objects of the same source file); field meanings not traced"),
    ('0x800A0804', '(n): fn_800F5448(n >> 1), fn_800F21C8(n >> 1, n & 1), then clears bit 0x400 of cr_g_ScenarioSlotPtr->status; called by cr_SEQ_SCENARIO_Enter: a load of save slot n >> 1 into the scenario if fn_800F21C8 (0x3280 bytes, ORs cr_g_SaveData.unlocks into cr_g_Scenario.part_bits and reads new_unlocks) is that load; its draft is 95% with the typed pointer (one pointer load short)'),
    ('0x801E1A24', "the unlock table of cr_Save_ApplyUnlocks: 12-byte entries {battles_finished threshold (3, 6, 9, ...), kind << 16 (0x40000 body, 0x80000 gun, 0x100000 bomb, 0x200000 pod, 0x400000 leg, 0x10000 mode, 0x20000 stage), item}; dtk's 0x56C object is not a multiple of 12 and ends in a list 0x22..0x26 and three code pointers, so its end is open"),
    ('0x80011E08', "(unlock entry*): queues the item per kind at 0x8031BF54 / 0x8031BFCC / 0x8031C044 / 0x8031C0BC / 0x8031C134 ..., parts offset by the category counts: the menus' new-item announcements, if those lists are what they show"),
    ('0x803756A2', 'the current stage id (u8, inside lbl_8037569C of 0x25C4): cr_StageSelector_Init starts from it, cr_SEQ_BATTLE_FrameA counts the battle at cr_g_SaveData.stage_battles[it], fn_8010FD38 / fn_80111040 / fn_800B0CF8 / fn_800BB034 / fn_800BC5C8 write it; the object around it is not typed'),
    ('0x8031B5A0', "0x784 by dtk, but cr_SEQ_MOVIE_FrameA keeps its address in r31 and addresses +0x3514..+0x3B0C through it (lbl_8031F020 and beyond): the logo / movie code's .bss objects 0x8031B5A0..0x8031F1A8 come from one source file; fn_80009A90 writes most of its first 0x240 bytes"),
    ('0x80392A44', "4 object pointers: fn_80089FB8 / fn_8008A030 / fn_8008A0A8 clear / set bit 0 of each one's first word, fn_80089F40 ORs their +0x18 words; set by fn_8008ACEC, read by the customize screen (fn_80124F5C / fn_8012F3C4); kind of object unknown"),
    ('0x8031BEC4', "the name entry screen's state (0x14: three words and a byte at +0x10) of cr_SEQ_NAME_ENTRY_FrameA and fn_8000B258 / fn_8000B42C / fn_8000B55C / fn_8000B878 / fn_8000C104; field meanings not traced"),
    ('0x80330D1C', 'a 0x30-byte text window parameter block (byte, two floats, bytes, a float, four bytes, a byte, a float, a word) written whole by each of the nine window creators fn_800672AC / fn_80067904 / fn_80069DA4 / fn_8006A00C / fn_8006A1A8 / fn_8006A344 / fn_8006A4E0 / fn_8006A7D8 / fn_8006B144: the style of the window being created'),
    ('0x80111CE8', "the stage selector's two exits (cr_StageSelector_Update result 0 / 1): fn_80111CE8 returns cr_g_SeqPrevious, or SEQ_MODE_SELECTOR unless cr_g_StageSelector.flags & 2, and calls cr_Customize_SetMode(cr_g_Customize.mode) when that is SEQ_MENU; fn_80111D50 not read"),
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
    lines += ['', '## Leads (by hand, plan.md steps 71, 79 and 81): behaviour seen, not proven enough for a name yet', '',
              '| address | lead |', '|---|---|']
    lines += [f'| {a} | {t} |' for a, t in LEADS]
    lines.append('')
    open(os.path.join(G, 'docs', 'name_evidence.md'), 'w', encoding='utf-8').write('\n'.join(lines) + '\n')
    open(os.path.join(G, 'docs', 'name_tables.tsv'), 'w', encoding='utf-8').write('\n'.join(tsv) + '\n')
    print(f'name_evidence: {len(tables)} pointer tables to strings, '
          + ', '.join(f'{len(v)} {k}' for k, v in by_kind.items()) + ' -> docs/name_evidence.md, docs/name_tables.tsv')


if __name__ == '__main__':
    main()
