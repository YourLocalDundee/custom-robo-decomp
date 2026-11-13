"""inline_temps.py: put m2c's pure temporaries back into the expressions that use them (Nov 13, 2026).

  python -I tools/inline_temps.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: m2c writes every value the original held in a register as a named temporary (`temp_r26_288 = lbl_80386448 +
0xB0;`), at the point the scheduled code made it. Many of them are the compiler's own common subexpressions (the
source wrote the address at each use and MWCC computed it once). As named variables in the draft they live from
their assignment to their last use, and in a big function that is what runs MWCC out of registers: the drafts of the
100 largest low-score functions are up to 1.8x the original with twice its frame (tools/triage.py).

What it does: a temporary assigned once (`temp_* = E;`, or var_* when it is assigned once) whose E is pure (no call,
no assignment, no memory read: identifiers, numbers, + - * << >> & | ^ ~ casts and parentheses; with --reads 1 also
M2C_FIELD / [] / * reads) and whose E's variables are not assigned between the definition and the last use (in text
order, all inside the definition's block) is replaced by (E) at each use and its assignment and declaration dropped.
Three sets are tried per function (addresses only: E names a global; all pure; pure + reads) and then, from the best
set, single temps are put back one at a time while that helps. A set is kept when the equal words go up (hd.py);
results go to --keep (default build/low100).
"""
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HD = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'hd.py')
av = sys.argv[1:]
opts, pos, i = {}, [], 0
while i < len(av):
    if av[i].startswith('--'):
        opts[av[i][2:]] = av[i + 1]
        i += 2
    else:
        pos.append(av[i])
        i += 1
OUT = os.path.join(G, opts.get('out', os.path.join('decomp', 'drafts')))
JOBS = int(opts.get('jobs', 8))
KEEP = os.path.join(G, opts.get('keep', os.path.join('build', 'low100')))
FROM = [os.path.join(G, d) for d in opts.get('from', os.path.relpath(KEEP, G)).split(',') if d]
PURE = re.compile(r'^[\w\s+\-*<>&|^~().]*$')
READS = re.compile(r'^[\w\s+\-*<>&|^~().,\[\]]*$')


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='itm_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def ident_set(e):
    return set(re.findall(r'\b[A-Za-z_]\w*\b', e))


def block_end(lines, j):
    """the last line of the block that line j sits in (its indent level)"""
    ind = len(lines[j]) - len(lines[j].lstrip())
    k = j + 1
    while k < len(lines):
        s = lines[k]
        if s.strip() and len(s) - len(s.lstrip()) < ind and s.strip().startswith('}'):
            return k - 1
        k += 1
    return len(lines) - 1


def candidates(text):
    lines = text.split('\n')
    try:
        start = next(k for k, l in enumerate(lines) if re.match(r'^\S.*\)\s*\{\s*$', l) and '(' in l)
    except StopIteration:
        return lines, []
    decl = {}
    k = start + 1
    while k < len(lines) and re.match(r'^    [A-Za-z_][\w ]*\**\s*\w+(\[\w*\])?;', lines[k]):
        m = re.match(r'^    .*?\b((?:temp|var)_[a-z]\d+_\d+)\s*;', lines[k])
        if m:
            decl[m.group(1)] = k
        k += 1
    body0 = k
    labels = [j for j in range(body0, len(lines)) if re.match(r'^\s*\w+:\s*$', lines[j])]
    out = []
    for v, dk in decl.items():
        w = re.compile(r'\b' + v + r'\b')
        idx = [j for j in range(body0, len(lines)) if w.search(lines[j])]
        asg = [j for j in idx if re.match(r'^\s*' + v + r'\s*=[^=]', lines[j])]
        if len(asg) != 1 or idx[0] != asg[0] or len(idx) < 2:
            continue
        mod = re.compile(r'(?<![\w.>])' + v + r'\s*(\+=|-=|\*=|/=|\|=|&=|\^=|<<=|>>=|\+\+|--)|(\+\+|--)\s*' + v + r'\b|&\s*' + v + r'\b')
        if any(mod.search(lines[j]) for j in idx):
            continue
        d = asg[0]
        m = re.match(r'^\s*' + v + r'\s*=\s*(.+);\s*$', lines[d])
        if not m:
            continue
        e = m.group(1).strip()
        if '=' in e or '?' in e:
            continue
        kind = None
        if PURE.match(e) and not re.search(r'\*\s*\(|\(\s*\*|^\*|[^\w)\s]\s*\*', e) and not re.search(r'\w\s*\(', e.replace('(s32)', '').replace('(u32)', '')):
            kind = 'addr' if re.search(r'\b(lbl_|cr_g_)\w+', e) else 'pure'
        elif READS.match(e) and not re.search(r'\b(?!M2C_FIELD\b)[A-Za-z_]\w*\s*\(', re.sub(r'\((?:\s*(?:[us]\d+|f32|f64|void|struct \w+|\w+)\s*\**\s*)\)', '', e)):
            kind = 'read'
        if kind is None:
            continue
        end = block_end(lines, d)
        if idx[-1] > end or any(d < j <= idx[-1] for j in labels):
            continue
        vs = ident_set(e) - {'M2C_FIELD', 's32', 'u32', 's16', 'u16', 's8', 'u8', 'f32', 'f64', 'void', 'struct'}
        moved = False
        for j in range(d + 1, idx[-1] + 1):
            l = lines[j]
            for x in vs:
                if re.search(r'(?<![\w.>])' + x + r'\s*(=[^=]|\+=|-=|\*=|/=|\|=|&=|\^=|<<=|>>=|\+\+|--)', l) or \
                        re.search(r'(\+\+|--)\s*' + x + r'\b', l):
                    moved = True
            if kind == 'read' and (re.search(r'(?<![=!<>])=(?!=)', l) or re.search(r'\w\s*\(', l)):
                moved = True        # a store or a call between: the read may see another value
            if moved:
                break
        if moved:
            continue
        out.append((v, dk, d, idx[1:], e, kind))
    return lines, out


def apply(lines, cands):
    lines = list(lines)
    drop = set()
    for v, dk, d, uses, e, kind in cands:
        for j in uses:
            lines[j] = re.sub(r'\b' + v + r'\b', '(' + e + ')', lines[j])
        drop |= {dk, d}
    # a later candidate's E can name an earlier one: substitute until stable
    names = {c[0]: c[4] for c in cands}
    for j in range(len(lines)):
        if j in drop:
            continue
        for _ in range(4):
            new = lines[j]
            for v, e in names.items():
                new = re.sub(r'\b' + v + r'\b', '(' + e + ')', new)
            if new == lines[j]:
                break
            lines[j] = new
    return '\n'.join(l for j, l in enumerate(lines) if j not in drop)


def src_of(func):
    for d in FROM + [os.path.join(G, 'tools', 'hand_drafts'), os.path.join(OUT, 'c')]:
        p = os.path.join(d, func + '.c')
        if os.path.exists(p):
            return p
    return None


def one(func):
    src = src_of(func)
    if src is None:
        return func, None
    text = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    base = score(func, text, 'base')
    lines, cands = candidates(text)
    if not cands:
        return func, (base, base, '', text)
    sets = {'addr': [c for c in cands if c[5] == 'addr'],
            'pure': [c for c in cands if c[5] in ('addr', 'pure')],
            'all': cands}
    best, btxt, bname, bset = base, text, '', []
    for nm, cs in sets.items():
        if not cs:
            continue
        t = apply(lines, cs)
        sc = score(func, t, nm)
        if sc[1] > best[1]:
            best, btxt, bname, bset = sc, t, nm, cs
    if bset and len(bset) <= 40:
        # put single temps back while that helps
        cur = list(bset)
        for c in list(bset):
            trial = [x for x in cur if x is not c]
            t = apply(lines, trial)
            sc = score(func, t, 'k' + c[0])
            if sc[1] > best[1]:
                best, btxt, cur = sc, t, trial
                bname += '-' + c[0]
    return func, (base, best, f'{bname} ({len(bset)} of {len(cands)})' if bname else f'none of {len(cands)}', btxt)


funcs = pos
if 'list' in opts:
    funcs += [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
with ThreadPoolExecutor(JOBS) as ex:
    res = list(ex.map(one, funcs))
os.makedirs(KEEP, exist_ok=True)
gain = 0
for func, r in res:
    if r is None:
        print(f'{func}: no draft')
        continue
    base, best, how, text = r
    note = ''
    if best[1] > base[1]:
        gain += best[1] - base[1]
        open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
        note = ' kept'
    print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}) {how}{note}', flush=True)
print(f'inline_temps: +{gain} equal words')
