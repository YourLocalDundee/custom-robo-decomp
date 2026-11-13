"""best_of.py: per function the best-scoring draft of several work folders, and a before / after table (Nov 13, 2026).

  python -I tools/best_of.py --list build/triage/low100.txt --dirs build/low100,build/hand100 [--into build/low100]
         [--table build/triage/low100_progress.tsv]

Scores each function's m2c draft (decomp/drafts/c, the start) and its FUNC.c in every --dirs folder with
tools/triage.py's measures (score, shape, regs), copies the version with the most equal words into --into (default:
the first folder) and writes the table: function, bytes, start score / shape / regs, best score / shape / regs, the
folder it came from. Totals at the end: equal words at the start and now, over the list.
"""
import os
import re
import shutil
import subprocess
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
av = sys.argv[1:]
opts = {av[i][2:]: av[i + 1] for i in range(0, len(av) - 1, 2) if av[i].startswith('--')}
funcs = [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
dirs = [d for d in opts.get('dirs', 'build/low100').split(',') if d]
into = os.path.join(G, opts.get('into', dirs[0]))
table = os.path.join(G, opts.get('table', os.path.join('build', 'triage', 'progress.tsv')))


def triage(from_dir, name):
    """triage.py over the list with FUNC.c taken from from_dir first; -> {func: (score, shape, regs, words)}"""
    args = [sys.executable, '-I', os.path.join(G, 'tools', 'triage.py')] + funcs + ['--name', name]
    if from_dir:
        args += ['--from', from_dir]
    subprocess.run(args, capture_output=True, text=True, cwd=G)
    out = {}
    for l in open(os.path.join(G, 'build', 'triage', name + '.tsv'), encoding='utf-8').read().split('\n')[1:]:
        f = l.split('\t')
        if len(f) >= 6:
            out[f[0]] = (int(f[2]), int(f[4]), int(f[5]))
    return out


def words(path, func):
    r = subprocess.run([sys.executable, '-I', os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'hd.py'),
                        os.path.join(G, 'decomp', 'drafts'), func, path], capture_output=True, text=True, cwd=G)
    m = re.search(r'\((\d+) equal words\)', r.stdout)
    return int(m.group(1)) if m else -1


start = triage('', 'bo_start')
per = {d: triage(d, 'bo_' + os.path.basename(d)) for d in dirs}
rows, w0, w1 = [], 0, 0
for f in funcs:
    m2c = os.path.join(G, 'decomp', 'drafts', 'c', f + '.c')
    base_w = words(m2c, f)
    best = (base_w, None)
    for d in dirs:
        p = os.path.join(G, d, f + '.c')
        if os.path.exists(p):
            w = words(p, f)
            if w > best[0]:
                best = (w, d)
    if best[1] and os.path.normpath(os.path.join(G, best[1])) != os.path.normpath(into):
        os.makedirs(into, exist_ok=True)
        shutil.copy(os.path.join(G, best[1], f + '.c'), os.path.join(into, f + '.c'))
    s0 = start.get(f, (0, 0, 0))
    s1 = per[best[1]].get(f, s0) if best[1] else s0
    w0 += max(base_w, 0)
    w1 += max(best[0], 0)
    rows.append((f, s0, s1, best[1] or 'm2c', base_w, best[0]))
os.makedirs(os.path.dirname(table), exist_ok=True)
with open(table, 'w', encoding='utf-8', newline='\n') as t:
    t.write('function\tscore0\tshape0\tregs0\tscore\tshape\tregs\twords0\twords\tfrom\n')
    for f, s0, s1, src, a, b in rows:
        t.write(f'{f}\t{s0[0]}\t{s0[1]}\t{s0[2]}\t{s1[0]}\t{s1[1]}\t{s1[2]}\t{a}\t{b}\t{src}\n')
up = sum(1 for r in rows if r[5] > r[4])
print(f'best_of: {len(funcs)} functions, {up} up; equal words {w0} -> {w1} (+{w1 - w0}); table {table}')
