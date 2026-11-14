"""rank_sweep.py LIST SRCDIR KEEPDIR [JOBS] [KMAX]: saved registers go by use count, so a dead use `(void) v;` moves a
parameter or local up the order without changing the code.  For each function of LIST (a file, or names with commas):
for each parameter and local, try 1..KMAX dead uses (kept only when the equal words go up), up to 3 rounds; the same
again on the source without its `x = x;` self copies (m2c's way of giving a parameter a register: 2 uses).
Source: SRCDIR/FUNC.c, else tools/hand_drafts, else decomp/drafts/c.  Writes KEEPDIR/FUNC.c when it improves, and a
line per function: name, start %, best %, start words, best words, the counts.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
lst, srcdir, keep = sys.argv[1:4]
jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
KMAX = int(sys.argv[5]) if len(sys.argv) > 5 else 6
funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
os.makedirs(keep, exist_ok=True)
NL = chr(10)
TAB = chr(9)


def score(f, text, tag):
    tmp = os.path.join(keep, f'{f}_{tag}.tmp.c')
    open(tmp, 'w', encoding='utf-8', newline=NL).write(text)
    try:
        r = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/hd.py', 'decomp/drafts', f, tmp],
                           capture_output=True, text=True, timeout=300).stdout.split(NL)[0]
    except subprocess.TimeoutExpired:
        r = ''
    try:
        os.remove(tmp)
    except OSError:
        pass
    m = re.search(r'(\d+)%\s+\((\d+) equal', r)
    return (int(m.group(2)), int(m.group(1))) if m else (-1, -1)


def variables(s, f):
    m = re.search(r'(?m)^[A-Za-z].*\b' + re.escape(f) + r'\(([^)]*)\) \{$', s)
    if not m:
        return None, []
    names = []
    for p in m.group(1).split(','):
        w = re.findall(r'[A-Za-z_]\w*', p)
        if w and w[-1] != 'void':
            names.append(w[-1])
    i = m.end() + 1
    j = s.find(NL + NL, i)
    if j < 0:
        return i, names
    depth = 0
    for l in s[i:j].split(NL):
        depth += l.count('{') - l.count('}')
        if depth > 0:
            continue
        t = l.split('}')[-1] if '}' in l else l
        t = t.split('=')[0]
        w = re.findall(r'[A-Za-z_]\w*', re.sub(r'\[[^\]]*\]', '', t))
        if w:
            names.append(w[-1])
    return j + 2, names


def one(f):
    p = os.path.join(srcdir, f + '.c')
    if not os.path.exists(p):
        p = f'tools/hand_drafts/{f}.c' if os.path.exists(f'tools/hand_drafts/{f}.c') else f'decomp/drafts/c/{f}.c'
    s0 = open(p, encoding='utf-8').read().replace(chr(13) + NL, NL)
    start = score(f, s0, 'r')
    if start[0] < 0:
        return f + TAB + 'no-compile'
    bases = [s0]
    s1 = re.sub(r'(?m)^[ ]+(\w+) = \1;$' + NL, '', s0)
    if s1 != s0:
        bases.append(s1)
    top = (start, s0, '')
    for bi, s in enumerate(bases):
        at, names = variables(s, f)
        if at is None or not names:
            continue
        counts = {n: 0 for n in names}

        def build(c):
            dead = ''.join('    ' + '(void) %s; ' % n * k + NL for n, k in c.items() if k)
            return s[:at] + dead + s[at:]
        best = start if bi == 0 else score(f, s, 'r')
        for rnd in range(3):
            moved = False
            for n in names:
                for k in range(0, KMAX + 1):
                    if k == counts[n]:
                        continue
                    c = dict(counts)
                    c[n] = k
                    sc = score(f, build(c), 'r')
                    if sc > best:
                        best, counts, moved = sc, c, True
                        if sc[1] == 100:
                            break
                if best[1] == 100:
                    break
            if not moved or best[1] == 100:
                break
        if best > top[0]:
            top = (best, build(counts), ('noself ' if bi else '') + ' '.join(f'{n}:{k}' for n, k in counts.items() if k))
        if best[1] == 100:
            break
    best = top[0]
    if best > start:
        open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(top[1])
    return TAB.join([f, str(start[1]), str(best[1]), str(start[0]), str(best[0]), top[2]])


with ThreadPoolExecutor(jobs) as ex:
    for line in ex.map(one, funcs):
        print(line, flush=True)
