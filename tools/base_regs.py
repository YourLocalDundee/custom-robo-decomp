"""base_regs.py: a global's address held in a saved register -> a local pointer in the draft (Nov 13, 2026).

  python -I tools/base_regs.py FUNC [FUNC ...] [--list file] [--out decomp/drafts] [--jobs 8] [--save]

Why: the original often loads a global's address once (`lis r3,sym@ha; addi r31,r3,sym@l`, or `addi rX` then
`mr r31,rX`) and reaches every field through r31 for the rest of the function: the source had a local pointer
(`Foo *p = &gFoo;`). m2c folds r31 + offset back into `sym + offset`, so the draft reloads the address (lis/addi)
at every use and leaves r31 to something else: triage.py counts ~4,000 extra lis over the 100 largest low drafts,
and 79 of those 100 originals have such a register.

What it does: per (saved register r14..r31, data symbol) pair the original sets with addi sym@l / mr, it declares
`T *base_sym = sym;` (an array) or `= &sym;` (a struct / scalar) and rewrites the draft's uses of sym through it
(`sym.` -> `base->`, `&sym` -> `base`, an array name -> `base`). The pointer is set at the top of the body or just
before the first use; each symbol, each combination of symbols taken greedily, is scored with hd.py and kept only
when the equal words go up. Improved drafts go to build/base_regs/; a 100% one is checked with relcheck.py and
saved as a hand draft with --save.
"""
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HD = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'hd.py')
REL = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'relcheck.py')
av = sys.argv[1:]
opts, pos, i = {}, [], 0
while i < len(av):
    if av[i].startswith('--'):
        k = av[i][2:]
        if k == 'save':
            opts[k] = True
            i += 1
        else:
            opts[k] = av[i + 1]
            i += 2
    else:
        pos.append(av[i])
        i += 1
OUT = os.path.join(G, opts.get('out', os.path.join('decomp', 'drafts')))
JOBS = int(opts.get('jobs', 8))
CTX = open(os.path.join(OUT, 'ctx.h'), encoding='utf-8', errors='replace').read()


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='bre_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def pairs(func):
    a = open(os.path.join(OUT, 'asm', func + '.s'), encoding='utf-8').read().split('\n')
    regsym, hits = {}, []
    for l in a:
        # Nov 13: a small-data object's address is `li rN, sym@sda21` (r13-relative; cr_StageSelector_Init keeps
        # two in r15 / r20)
        m = re.search(r'\taddi (r\d+), r\d+, (\w+)@l', l) or re.search(r'\tli (r\d+), (\w+)@sda21', l)
        if m:
            regsym[m.group(1)] = m.group(2)
            if int(m.group(1)[1:]) >= 14:
                hits.append(m.group(2))
            continue
        m = re.search(r'\tmr (r\d+), (r\d+)$', l)
        if m and m.group(2) in regsym and int(m.group(1)[1:]) >= 14:
            hits.append(regsym[m.group(2)])
        m = re.search(r'\t\w+ (r\d+),', l)
        if m:
            regsym.pop(m.group(1), None)
    out = []
    for s in hits:
        if s not in out and not s.startswith('fn_'):
            out.append(s)
    return out


def decl_of(sym, text):
    for src in (text, CTX):
        m = re.search(r'^(?:CR_SECTION\("[^"]*"\)\s*)?extern\s+(?:const\s+)?([\w ]+?)\s*(\**)\s*\b' + sym +
                      r'\b\s*(\[[^\]]*\])?\s*;', src, re.M)
        if m:
            return m.group(1).strip(), m.group(2), bool(m.group(3))
    return None


def apply(text, sym, where):
    d = decl_of(sym, text)
    if d is None:
        return None
    ty, stars, arr = d
    lines = text.split('\n')
    try:
        start = next(k for k, l in enumerate(lines) if re.match(r'^\S.*\)\s*\{\s*$', l) and '(' in l)
    except StopIteration:
        return None
    k = start + 1
    while k < len(lines) and re.match(r'^    [A-Za-z_][\w ]*\**\s*\w+(\[\w*\])?;', lines[k]):
        k += 1
    body0 = k
    b = 'base_' + sym
    w = re.compile(r'(?<![\w.>])' + sym + r'\b')
    used = [j for j in range(body0, len(lines)) if w.search(lines[j])]
    if not used:
        return None
    for j in used:
        l = lines[j]
        if arr:
            l = w.sub(b, l)
        else:
            l = re.sub(r'&\s*' + sym + r'\b', b, l)
            l = re.sub(r'(?<![\w.>])' + sym + r'\s*\.', b + '->', l)
            l = w.sub('(*' + b + ')', l)
        lines[j] = l
    if arr:
        dl, init = f'    {ty} {stars}*{b};', f'{b} = {sym};'
    else:
        dl, init = f'    {ty} {stars}*{b};', f'{b} = &{sym};'
    at = body0 if where == 'top' else used[0]
    ind = re.match(r'^(\s*)', lines[at]).group(1) if where != 'top' else '    '
    # a first use inside a condition / loop head: set it before that statement
    lines[at:at] = [ind + init]
    lines[body0:body0] = [dl]
    return '\n'.join(lines)


def reuse(text, sym):
    """Nov 13: the draft already has m2c's register variable for the address (`var_r31 = lbl_80386550;`, or
    `= &sym;`) but names the global itself at the later uses: those uses (until the variable is assigned again) go
    through the variable (fn_80082F84 9 -> 14%: its 83 extra lis gone)."""
    m = re.search(r'^(\s*)((?:var|temp)_r\w+) = (&?)' + sym + r';\s*$', text, re.M)
    if not m:
        return None
    v, amp = m.group(2), m.group(3)
    head, tail = text[:m.end()], text[m.end():]
    k = re.search(r'^\s*' + v + r'\s*=[^=]', tail, re.M)
    cut = k.start() if k else len(tail)
    seg = tail[:cut]
    if amp:
        seg = re.sub(r'&\s*' + sym + r'\b', v, seg)
        seg = re.sub(r'(?<![\w.>])' + sym + r'\s*\.', v + '->', seg)
        seg = re.sub(r'(?<![\w.>])' + sym + r'\b', '(*' + v + ')', seg)
    else:
        seg = re.sub(r'(?<![\w.>])' + sym + r'\b', v, seg)
    new = head + seg + tail[cut:]
    return new if new != text else None


SRC_DIRS = [os.path.join(G, d) for d in opts.get('from', '').split(',') if d]


def one(func):
    src = next((os.path.join(d, func + '.c') for d in SRC_DIRS + [os.path.join(G, 'tools', 'hand_drafts'),
                                                                     os.path.join(OUT, 'c')]
                if os.path.exists(os.path.join(d, func + '.c'))), None)
    if src is None:
        return func, None
    text = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    base = score(func, text, 'base')
    best, cur, took = base, text, []
    # every reuse at once first (fn_80082F84: either alone 86 -> 59 / 64 words, both 118)
    allr, k = text, 0
    for sym in pairs(func):
        t = reuse(allr, sym)
        if t is not None:
            allr, k = t, k + 1
    if k > 1:
        sc = score(func, allr, 'allreuse')
        if sc[1] > best[1]:
            best, cur = sc, allr
            took.append(f'reuse x{k}')
    for n, sym in enumerate(pairs(func)):
        cands = []
        t = reuse(cur, sym)
        if t is not None:
            cands.append((score(func, t, f'{n}reuse'), t, 'reuse'))
        for where in ('top', 'first'):
            t = apply(cur, sym, where)
            if t is not None:
                cands.append((score(func, t, f'{n}{where}'), t, where))
        if not cands:
            continue
        sc, t, where = max(cands, key=lambda c: c[0][1])
        if sc[1] > best[1]:
            best, cur = sc, t
            took.append(f'{sym}@{where}')
    return func, (base, best, took, cur)


if 'dump' in opts:          # --dump SYM: write the draft with SYM rewritten (top) to stdout's file, compile message
    t = apply(open(os.path.join(OUT, 'c', pos[0] + '.c'), encoding='utf-8').read(), opts['dump'], 'top')
    f = os.path.join(G, 'build', 'base_regs', pos[0] + '_dump.c')
    os.makedirs(os.path.dirname(f), exist_ok=True)
    open(f, 'w', encoding='utf-8', newline='\n').write(t or '')
    print(pairs(pos[0]), f, subprocess.run([sys.executable, '-I', HD, OUT, pos[0], f], capture_output=True, text=True,
                                         cwd=G).stdout[:1500])
    sys.exit()
funcs = pos
if 'list' in opts:
    funcs += [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
with ThreadPoolExecutor(JOBS) as ex:
    res = list(ex.map(one, funcs))
keep = os.path.join(G, opts.get('keep', os.path.join('build', 'base_regs')))
os.makedirs(keep, exist_ok=True)
gain = 0
for func, r in res:
    if r is None:
        print(f'{func}: no draft')
        continue
    base, best, took, text = r
    note = ''
    if best[1] > base[1]:
        gain += best[1] - base[1]
        open(os.path.join(keep, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
        note = ' kept ' + ' '.join(took)
    if best[0] == 100:
        f = os.path.join(tempfile.gettempdir(), func + '_bre.c')
        open(f, 'w', encoding='utf-8', newline='\n').write(text)
        rc = subprocess.run([sys.executable, '-I', REL, OUT, func, f], capture_output=True, text=True, cwd=G)
        note += ' MATCH' + (', relocations equal' if 'relocations equal' in rc.stdout else ', relocations differ')
    print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}){note}', flush=True)
print(f'base_regs: +{gain} equal words')
