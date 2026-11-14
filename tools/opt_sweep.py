"""opt_sweep.py LIST SRCDIRS KEEPDIR [JOBS]: which drafts belong to code compiled above optimization level 0?

Most of the game is `-opt level=0,peephole,schedule`, but some functions are not: cr_SinCos16 is a frameless leaf
with its locals in volatile registers and matches only under `#pragma optimization_level 4` (Nov 13, 2026).  For
each function of LIST the draft is compiled as it is and inside `#pragma push / optimization_level N / pop` for
N = 1 .. 4; a line per function with the five scores (equal words), and the draft with the best level's pragma
written to KEEPDIR when that beats level 0.
SRCDIRS: folders with comma, first that has FUNC.c wins; then tools/hand_drafts, decomp/drafts/c."""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
NL, TAB = chr(10), chr(9)


def score(f, text, keep, tag):
    tmp = os.path.join(keep, f'{f}_o{tag}.tmp.c')
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


def wrap(s, n):
    # after the draft's own leading comment / directive lines
    lines = s.split(NL)
    k = 0
    while k < len(lines) and (lines[k].startswith('//') or lines[k].startswith('/*') or lines[k].startswith('#define') or
                              lines[k].startswith('#include') or lines[k].startswith('#undef') or not lines[k].strip()):
        k += 1
    return NL.join(lines[:k] + ['#pragma push', f'#pragma optimization_level {n}'] + lines[k:] + ['#pragma pop', ''])


def main():
    lst, srcdirs, keep = sys.argv[1:4]
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
    os.makedirs(keep, exist_ok=True)

    def one(f):
        src = None
        for d in srcdirs.split(',') + ['tools/hand_drafts', 'decomp/drafts/c']:
            if os.path.exists(f'{d}/{f}.c'):
                src = f'{d}/{f}.c'
                break
        s = open(src, encoding='utf-8').read().replace(chr(13) + NL, NL)
        sc = [score(f, s, keep, 0)] + [score(f, wrap(s, n), keep, n) for n in (1, 2, 3, 4)]
        best = max(range(5), key=lambda n: sc[n])
        if best and sc[best] > sc[0]:
            open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(wrap(s, best))
        return TAB.join([f] + [f'{a}/{b}' for a, b in sc] + [f'best={best}'])

    with ThreadPoolExecutor(jobs) as ex:
        for line in ex.map(one, funcs):
            print(line, flush=True)


if __name__ == '__main__':
    main()
