"""cror_fix.py: m2c's `cror` errors back to the float comparison they are (Nov 13, 2026).

  python -I tools/cror_fix.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: MWCC compiles `a >= b` / `a <= b` on floats as `fcmpo cr0, fa, fb; cror eq, gt, eq` (`cror 2, 1, 2`) or
`cror eq, lt, eq` (`cror 2, 0, 2`) and then branches on eq. m2c does not know cror: it leaves
`M2C_ERROR(/* unknown instruction: cror 0x2, 0x1, 0x2 */);` (compiled as nothing) and reads the branch as testing eq
alone, so the next condition has `a == b` (or `a != b`) where the source had `a >= b` (or `a < b`). Six of the 100
largest low drafts have it (fn_80085CE4, fn_8006C970, fn_8006B848, fn_8006207C, fn_8005ED28, cr_SEQ_SCENARIO_FrameC).

What it does: drops the error line and, in the next statement that has a comparison, turns its first `==` / `!=`
into `>=` / `<` (cror 2,1,2) or `<=` / `>` (cror 2,0,2). Scored with hd.py, kept when the equal words go up.
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
ERR = re.compile(r'^\s*M2C_ERROR\(/\* unknown instruction: cror 0x2, 0x([01]), 0x2 \*/\);\s*$')
OPS = {'1': {'==': '>=', '!=': '<'}, '0': {'==': '<=', '!=': '>'}}


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='cro_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def rewrite(text):
    lines = text.split('\n')
    out, n, k = [], 0, 0
    while k < len(lines):
        m = ERR.match(lines[k])
        if not m:
            out.append(lines[k])
            k += 1
            continue
        j = k + 1
        while j < len(lines) and not re.search(r'[!=]=', lines[j]):
            j += 1
        if j < len(lines):
            op = re.search(r'(==|!=)', lines[j]).group(1)
            lines[j] = lines[j].replace(op, OPS[m.group(1)][op], 1)
            n += 1
        k += 1
    return '\n'.join(out), n


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
    new, n = rewrite(text)
    if not n:
        return func, None
    base = score(func, text, 'base')
    sc = score(func, new, 'cror')
    return func, (base, sc, n, new if sc[1] > base[1] else None)


if __name__ == '__main__':
    funcs = pos
    if 'list' in opts:
        funcs += [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
    with ThreadPoolExecutor(JOBS) as ex:
        res = list(ex.map(one, funcs))
    os.makedirs(KEEP, exist_ok=True)
    gain = 0
    for func, r in res:
        if r is None:
            continue
        base, best, n, text = r
        note = ''
        if text is not None:
            gain += best[1] - base[1]
            open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
            note = ' kept'
        print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}), {n} compares{note}', flush=True)
    print(f'cror_fix: +{gain} equal words')
