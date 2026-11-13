"""order_sweep.py: statement-order sweep of a game-code draft (Nov 12, 2026).

  python -I tools/order_sweep.py FUNC [FUNC ...] [--out decomp/drafts] [--src <draft.c>] [--cap 240] [--jobs 8]
         [--save] [--min 60]
  python -I tools/order_sweep.py --open 70 99 [--save]      every open draft scoring 70..99% in the draft report

Why: MWCC numbers its virtual registers in the order the source creates values, and the colouring hands out the
scratch registers in that order, so two sources that differ only in the order of independent statements compile to
the same instructions with different registers (and a different schedule). m2c writes statements in instruction
order, which is the scheduler's order, not the source's. fn_8001EA48 (the Hermite key interpolation, 89%): of the 63
legal orders of its seven statements exactly one gives the original (1 - t, 2t, (1 - t)^2, then h00, h11, h10, h01).

What it does: finds every run of consecutive simple statements (assignments, calls) at one indent, works out which
pairs must keep their order (one defines what the other uses or defines; memory writes against memory reads /
writes; calls against everything that touches memory), enumerates the orders that respect that (all of them up to
--cap, a random sample above), scores each with tools/sched/scratch/hand/hd.py (the draft compiled with the game
compiler and compared with the original) and keeps the best per run, run after run. A 100% result is checked with
relcheck.py (hd.py masks relocated fields) and, with --save, written to tools/hand_drafts/FUNC.c.
Decompiled drafts are never committed; a hand draft is (it is the matched source).
"""
import itertools
import os
import random
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HD = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'hd.py')
REL = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'relcheck.py')
args = [a for a in sys.argv[1:] if not a.startswith('--')]
opts = {}
av = sys.argv[1:]
i = 0
pos = []
while i < len(av):
    if av[i].startswith('--'):
        k = av[i][2:]
        if k in ('save',):
            opts[k] = True
            i += 1
        elif k == 'open':
            opts[k] = (int(av[i + 1]), int(av[i + 2]))
            i += 3
        else:
            opts[k] = av[i + 1]
            i += 2
    else:
        pos.append(av[i])
        i += 1
OUT = os.path.join(G, opts.get('out', os.path.join('decomp', 'drafts')))
CAP = int(opts.get('cap', 240))
JOBS = int(opts.get('jobs', 8))
LOCAL = re.compile(r'^(var_|sp|arg|farg|temp_|phi_|saved_|unksp)')
ASSIGN = re.compile(r'^(\s+)(.+?)\s*(=|\+=|-=|\*=|/=|\|=|&=|\^=|<<=|>>=)\s*(.+);\s*$')
CALL = re.compile(r'^(\s+)([A-Za-z_]\w*)\((.*)\);\s*$')
KEYWORD = re.compile(r'^\s*(return|goto|break|continue|if|else|while|for|do|switch|case|default)\b')
NOT_CALL = {'M2C_FIELD', 'sizeof', 'M2C_ERROR', 'M2C_BITWISE'}


def score(func, text, tmp, tag):
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def idents(s):
    s = re.sub(r'"[^"]*"', '', s)
    return set(re.findall(r'[A-Za-z_]\w*(?:\.\w+)*', s))


def analyse(line):
    """-> (defs, uses, memw, memr, call) or None when the line is not a movable statement"""
    if KEYWORD.match(line) or '{' in line or '}' in line or line.strip().startswith(('//', '/*', '#')):
        return None
    m = ASSIGN.match(line)
    if m and not re.match(r'^[A-Za-z_]\w*\s+\**[A-Za-z_]\w*(\[\w*\])?$', m.group(2).strip()) and '==' not in m.group(2):
        lhs, op, rhs = m.group(2).strip(), m.group(3), m.group(4)
        ids = idents(rhs)
        calls = {c for c in re.findall(r'([A-Za-z_]\w*)\s*\(', rhs) if c not in NOT_CALL and not re.match(r'^[suf]\d+$|^void$', c)}
        plain = re.match(r'^[A-Za-z_]\w*(?:\.\w+)*$', lhs) and LOCAL.match(lhs)
        defs = {lhs} if plain else set()
        uses = set(ids)
        if op != '=':
            uses |= idents(lhs)
        if not plain:
            uses |= idents(lhs)
        memw = not plain
        memr = any(not LOCAL.match(x.split('.')[0]) and not re.match(r'^[suf]\d+$|^M2C_|^void$|^struct$|^\d', x) for x in uses) \
            or '->' in rhs or '*' in rhs.replace('* ', ' ').replace(' *', ' ') and '(' in rhs
        return defs, uses, memw, memr, bool(calls)
    m = CALL.match(line)
    if m and m.group(2) not in NOT_CALL:
        return set(), idents(m.group(3)), True, True, True
    return None


def conflict(a, b):
    da, ua, wa, ra, ca = a
    db, ub, wb, rb, cb = b

    def hit(d, u):
        return any(x == y or x.startswith(y + '.') or y.startswith(x + '.') for x in d for y in u)
    if hit(da, ub) or hit(db, ua) or hit(da, db):
        return True
    if (wa and (wb or rb)) or (wb and ra):
        return True
    if ca and (cb or wb or rb) or cb and (wa or ra):
        return True
    return False


def orders(n, must, cap):
    """topological orders of range(n) under must[(i, j)] (i before j); all if few, else a random sample"""
    res = []

    def rec(done, left):
        if len(res) >= cap * 4:
            return
        if not left:
            res.append(tuple(done))
            return
        for k in list(left):
            if all((j, k) not in must for j in left if j != k):
                rec(done + [k], left - {k})
    rec([], set(range(n)))
    if len(res) > cap:
        random.seed(n * 7919 + len(must))
        ident = tuple(range(n))
        res = [ident] + random.sample([r for r in res if r != ident], cap - 1)
    return res


def sweep(func, src):
    tmp = tempfile.mkdtemp(prefix='osw_')
    text = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    lines = text.split('\n')
    base = score(func, text, tmp, 'base')
    if base[0] < 0:
        return None
    best = base
    # the runs
    info = [analyse(l) for l in lines]
    runs, k = [], 0
    while k < len(lines):
        if info[k] is None:
            k += 1
            continue
        ind = len(lines[k]) - len(lines[k].lstrip())
        j = k
        while j < len(lines) and info[j] is not None and len(lines[j]) - len(lines[j].lstrip()) == ind:
            j += 1
        if j - k >= 2:
            runs.append((k, j))
        k = j
    tried = 0
    for a, b in runs:
        n = b - a
        if n > 12:
            continue
        must = {(i, j) for i in range(n) for j in range(i + 1, n) if conflict(info[a + i], info[a + j])}
        if len(must) == n * (n - 1) // 2:
            continue
        cand = [o for o in orders(n, must, CAP) if o != tuple(range(n))]
        if not cand:
            continue

        def run_one(o):
            new = lines[:a] + [lines[a + x] for x in o] + lines[b:]
            return score(func, '\n'.join(new), tmp, f'{a}_{"".join(map(str, o))[:40]}'), o
        with ThreadPoolExecutor(JOBS) as ex:
            res = list(ex.map(run_one, cand))
        tried += len(cand)
        top = max(res, key=lambda r: r[0])
        if top[0] > best:
            best = top[0]
            lines = lines[:a] + [lines[a + x] for x in top[1]] + lines[b:]
            info = [analyse(l) for l in lines]
            if best[0] == 100:
                break
    return base, best, tried, '\n'.join(lines), len(runs)


def open_list(lo, hi):
    hand = {f[:-2] for f in os.listdir(os.path.join(G, 'tools', 'hand_drafts')) if f.endswith('.c')}
    out = []
    for l in open(os.path.join(OUT, 'draft_report.txt'), encoding='utf-8'):
        m = re.match(r'([0-9A-F]{8})\s+(\S+)\s+(\d+)\s+(\d+)%', l)
        if m and lo <= int(m.group(4)) <= hi and m.group(2) not in hand:
            out.append((int(m.group(4)), m.group(2)))
    return [f for _, f in sorted(out, reverse=True)]


funcs = open_list(*opts['open']) if 'open' in opts else pos
if not funcs:
    sys.exit(__doc__)
print(f'order_sweep: {len(funcs)} function(s)', flush=True)
wins = 0
for func in funcs:
    src = opts.get('src') if len(funcs) == 1 and opts.get('src') else os.path.join(OUT, 'c', func + '.c')
    if not os.path.exists(src):
        print(f'{func}: no draft')
        continue
    r = sweep(func, src)
    if r is None:
        print(f'{func}: did not compile', flush=True)
        continue
    base, best, tried, text, nruns = r
    note = ''
    if best[0] == 100:
        f = os.path.join(tempfile.gettempdir(), func + '_osw.c')
        open(f, 'w', encoding='utf-8', newline='\n').write(text)
        rc = subprocess.run([sys.executable, '-I', REL, OUT, func, f], capture_output=True, text=True, cwd=G)
        ok = 'relocations equal' in rc.stdout
        note = ' MATCH, relocations equal' if ok else ' 100% words but relocations differ'
        if ok:
            wins += 1
            if opts.get('save'):
                body = text[text.index('\n', text.index('#undef')) + 1:].lstrip('\n') if '#undef' in text else text
                hdr = (f'/* {func}: Nov 12, 2026 hand draft (m2c {base[0]}%): statement order (tools/order_sweep.py: the '
                       f'scratch registers follow the order the source creates its values in) */\n\n')
                open(os.path.join(G, 'tools', 'hand_drafts', func + '.c'), 'w', encoding='utf-8', newline='\n').write(hdr + body)
                note += ', saved'
    elif best > base:
        keep = os.path.join(G, 'build', 'order_sweep')
        os.makedirs(keep, exist_ok=True)
        open(os.path.join(keep, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
    print(f'{func}: {base[0]}% ({base[1]} words) -> {best[0]}% ({best[1]}) over {tried} orders in {nruns} runs{note}', flush=True)
print(f'order_sweep: {wins} new match(es)')
