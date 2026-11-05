"""callmap.py SEED... [--up N] [--down N] [--out FILE.md]: a call map around some functions, from the Ghidra index
(decompiled/index.db, tools/build_index.py) and the draft report (decomp/drafts/draft_report.txt): for each seed, its
callers N levels up and its callees N levels down (direct calls; tables of function pointers are DATA references and
are listed separately), each with its name, size and decomp status (100% linked from C / draft %, m2c failure). A seed
is a name or an address. Nov 5, 2026: written to map the part / motion chain (docs/formats/model_entry.md "Part
files"); the map is a reading aid built from Ghidra's references, not proof."""
import os
import re
import sqlite3
import sys
from collections import defaultdict

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load():
    c = sqlite3.connect(os.path.join(G, 'decompiled', 'index.db'))
    funcs = {a: (n, sz) for a, n, sz in c.execute('SELECT addr, name, size FROM funcs')}
    calls, callers, datarefs = defaultdict(set), defaultdict(set), defaultdict(set)
    starts = sorted(funcs)
    for f, at, t, k in c.execute('SELECT func, at, target, kind FROM refs'):
        if t in funcs and t != f:
            if k in ('UNCONDITIONAL_CALL', 'COMPUTED_CALL'):
                calls[f].add(t)
                callers[t].add(f)
            elif k in ('DATA', 'PARAM', 'READ'):
                datarefs[t].add(f)
    status = {}
    for l in open(os.path.join(G, 'decomp', 'drafts', 'draft_report.txt'), encoding='utf-8'):
        m = re.match(r'^([0-9A-F]{8})\s+(\S+)\s+(\d+)\s+(.+?)\s*$', l)
        if m:
            st = m.group(4)
            status[int(m.group(1), 16)] = ('100%' if st.startswith('100%') else
                                          ('m2c fails' if 'm2c failed' in st else st.split()[0]))
    linked = set()
    src = os.path.join(G, 'decomp', 'src', 'game')
    names = {n: a for a, (n, _) in funcs.items()}
    for f in os.listdir(src):
        if f.endswith('.c'):
            body = open(os.path.join(src, f), encoding='utf-8', errors='replace').read()
            for m in re.finditer(r'^\w[\w\s\*]*?\b(\w+)\(', body, re.M):
                if m.group(1) in names:
                    linked.add(names[m.group(1)])
    return funcs, calls, callers, datarefs, status, linked, names


def label(a, funcs, status, linked):
    n, sz = funcs.get(a, ('?', 0))
    st = 'C, linked' if a in linked else (status.get(a, 'library' if a >= 0x80131C08 or a < 0x800055E0 else '-'))
    return f'`{n}` (0x{a:08X}, {sz} B, {st})'


def main(argv):
    up = int(argv[argv.index('--up') + 1]) if '--up' in argv else 2
    down = int(argv[argv.index('--down') + 1]) if '--down' in argv else 1
    out = argv[argv.index('--out') + 1] if '--out' in argv else None
    seeds = [x for i, x in enumerate(argv) if not x.startswith('--') and (i == 0 or argv[i - 1] not in ('--up', '--down', '--out'))]
    funcs, calls, callers, datarefs, status, linked, names = load()
    lines = []
    for s in seeds:
        m = re.fullmatch(r'(?:fn_|FUN_|0x)?([0-9A-Fa-f]{8})', s)
        a = names.get(s) or int(m.group(1), 16)
        lines.append(f'### {label(a, funcs, status, linked)}')
        lv, seen = [a], {a}
        for d in range(1, up + 1):
            nxt = sorted({c for x in lv for c in callers[x]} - seen)
            if not nxt:
                break
            lines.append(f'- callers, level {d}: ' + '; '.join(label(x, funcs, status, linked) for x in nxt))
            seen |= set(nxt)
            lv = nxt
        tb = sorted(datarefs[a])
        if tb:
            lines.append('- referenced as data (function tables / pointers) by: ' + '; '.join(label(x, funcs, status, linked) for x in tb))
        lv, seen = [a], {a}
        for d in range(1, down + 1):
            nxt = sorted({c for x in lv for c in calls[x]} - seen)
            if not nxt:
                break
            lines.append(f'- calls, level {d}: ' + '; '.join(label(x, funcs, status, linked) for x in nxt))
            seen |= set(nxt)
            lv = nxt
        lines.append('')
    text = '\n'.join(lines) + '\n'
    if out:
        open(out, 'w', encoding='utf-8', newline='\r\n').write(text)
    else:
        sys.stdout.write(text)


if __name__ == '__main__':
    main(sys.argv[1:])
