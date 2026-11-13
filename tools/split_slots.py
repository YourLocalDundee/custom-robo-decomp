"""split_slots.py: one variable per value for m2c's stack-slot variables (Nov 13, 2026).

  python -I tools/split_slots.py FUNC [FUNC ...] [--list file] [--out decomp/drafts] [--save] [--jobs 8]
         [--mode text|block]

Why: in a big function MWCC runs out of registers and spills, and a spilled value is stored to a stack slot after
it is made and reloaded before each use (`stw r0,0x160(r1)` / `lwz r0,0x160(r1)`). Slots are reused by values whose
lives do not overlap. m2c names a slot as one variable (sp160) for the whole function, so the draft has one long-lived
variable where the original had many short ones; the draft then needs more registers than the original had, spills
more, and every word after the first difference is off (the low-score giant drafts: tools/triage.py shows 1.3 - 1.8x
the original's size, twice its frame, ~7,000 extra stack loads and stores over the 100 largest).

What it does: each plain scalar spN (declared `T spN;`, never address-taken, not an array or struct) gets a new name
at each plain assignment `spN = ...;`; the uses after it (in text order) up to the next assignment take that name.
--mode block only splits where the assignment and all its uses sit between the same labels (no goto / loop can
reach a use from another assignment), the safe form. Text order is not the flow order in loops, so --mode text can
change what a draft means; only a word-for-word match (which hd.py and relcheck.py check) makes a draft a source.
Scores each function before and after with tools/sched/scratch/hand/hd.py; improved drafts go to build/split_slots/.
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
MODE = opts.get('mode', 'block')
DECL = re.compile(r'^    ([A-Za-z_][\w ]*?\**)\s*(sp[0-9A-F]+(?:_\w+)?);(\s*/\*.*\*/)?\s*$')
LABEL = re.compile(r'^\s*[A-Za-z_]\w*:\s*$')


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='spl_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return ((int(m.group(1)), int(m.group(2))) if m else (-1, -1)), f


def split(text):
    lines = text.split('\n')
    try:
        start = next(k for k, l in enumerate(lines) if re.match(r'^\S.*\)\s*\{\s*$', l) and '(' in l)
    except StopIteration:
        return text, 0
    decl = {}
    k = start + 1
    while k < len(lines) and (DECL.match(lines[k]) or re.match(r'^    [A-Za-z_][\w ]*\**\s*\w+(\[\w*\])?;', lines[k])):
        m = DECL.match(lines[k])
        if m:
            decl[m.group(2)] = (k, m.group(1).strip())
        k += 1
    body0 = k
    body = '\n'.join(lines[body0:])
    new_decls = {}
    n_split = 0
    for v, (dk, ty) in decl.items():
        if re.search(r'&\s*' + v + r'\b', body) or re.search(r'\b' + v + r'\s*\[', body) or re.search(r'\b' + v + r'\s*\.', body):
            continue
        word = re.compile(r'\b' + v + r'\b')
        asg = re.compile(r'^\s*' + v + r'\s*=[^=]')
        idx = [j for j in range(body0, len(lines)) if word.search(lines[j])]
        defs = [j for j in idx if asg.match(lines[j])]
        if len(defs) < 2:
            continue
        # the webs, text order
        webs = []
        cur = None
        for j in idx:
            if j in defs:
                rhs_uses = len(word.findall(lines[j])) > 1
                if rhs_uses and cur is not None:          # spN = f(spN): keeps the old value: same web
                    webs[-1].append(j)
                    continue
                cur = [j]
                webs.append(cur)
            elif cur is not None:
                cur.append(j)
        if len(webs) < 2:
            continue
        if MODE == 'block':
            labels = [j for j in range(body0, len(lines)) if LABEL.match(lines[j]) or re.search(r'\b(while|for|do)\b', lines[j])]
            ok = []
            for w in webs:
                lo, hi = w[0], w[-1]
                ok.append(not any(lo < j <= hi for j in labels))
            # uses before the first def (a loop back edge) also block the first web
            if idx[0] not in defs:
                continue
        else:
            ok = [True] * len(webs)
        for n, (w, good) in enumerate(zip(webs, ok)):
            if n == 0 or not good:
                continue
            nm = f'{v}_v{n}'
            for j in w:
                lines[j] = word.sub(nm, lines[j])
            new_decls.setdefault(dk, []).append(f'    {ty} {nm};' if not ty.endswith('*') else f'    {ty}{nm};')
            n_split += 1
    for dk in sorted(new_decls, reverse=True):
        lines[dk + 1:dk + 1] = new_decls[dk]
    return '\n'.join(lines), n_split


def one(func):
    src = os.path.join(G, 'tools', 'hand_drafts', func + '.c')
    if not os.path.exists(src):
        src = os.path.join(OUT, 'c', func + '.c')
    if not os.path.exists(src):
        return func, None
    text = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    base, _ = score(func, text, 'base')
    new, n = split(text)
    if not n:
        return func, (base, base, 0, None)
    best, f = score(func, new, 'split')
    return func, (base, best, n, new)


funcs = pos
if 'list' in opts:
    funcs += [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
with ThreadPoolExecutor(JOBS) as ex:
    res = list(ex.map(one, funcs))
keep = os.path.join(G, 'build', 'split_slots')
os.makedirs(keep, exist_ok=True)
gain = 0
for func, r in res:
    if r is None:
        print(f'{func}: no draft')
        continue
    base, best, n, new = r
    mark = ''
    if best[1] > base[1]:
        gain += best[1] - base[1]
        open(os.path.join(keep, func + '.c'), 'w', encoding='utf-8', newline='\n').write(new)
        mark = ' kept'
    print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}), {n} split{mark}', flush=True)
print(f'split_slots ({MODE}): +{gain} equal words')
