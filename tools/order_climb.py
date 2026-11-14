"""order_climb.py LIST SRCDIRS KEEPDIR [JOBS]: statement order by hill climbing, for runs too long to enumerate.

m2c writes statements in the order of the instructions, which is the scheduler's order, not the source's, and
MWCC hands out registers in the order the source creates values (order_sweep.py, Nov 12). order_sweep.py
enumerates the legal orders of a run; a run of 25 independent statements (fn_8013098C: one block of flag
updates and stores) has far too many. Here each run of consecutive simple statements is climbed instead: every
move of one statement to another place in its run is compiled and scored (hd.py, equal words), the best move
is taken, and that repeats until no move gains (or the function matches). No dependence analysis: a move is
kept only for its score, and a draft is only ever accepted at 100%, where the code is the original's.
A run is at least three single-line statements at one indent with nothing between them (no declarations,
labels, braces, return / goto / break). The best version is written to KEEPDIR when it beats the start.
SRCDIRS: folders with comma, first that has FUNC.c wins; then tools/hand_drafts, decomp/drafts/c."""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
NL, TAB = chr(10), chr(9)
TYPE = re.compile(r'^\s*(?:const |static |struct |unsigned |signed |volatile )*(?:[usf](?:8|16|32|64)|int|char|short|long|float|double|void|'
                  r'[A-Z]\w*)\b[\w\s\*]*?[\*\s]\w+(?:\[[^\]]*\])*(?:\s*=.*)?;$')
MAXRUN, ROUNDS = 40, 30


def score(f, text, keep, tag):
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


def simple(l):
    s = l.strip()
    if not s.endswith(';') or s.startswith(('return', 'goto', 'break', 'continue', 'case', 'default', '}', '{', '//', '/*', '#')):
        return False
    if s.count('(') != s.count(')') or '{' in s or '}' in s:
        return False
    if TYPE.match(l) and not re.match(r'^\s*\w+\s*(?:\(|=|\+=|-=|\|=|&=|\[|\.|->)', l):
        return False
    return True


def runs(lines):
    out, i = [], 0
    while i < len(lines):
        if simple(lines[i]):
            j = i
            ind = len(lines[i]) - len(lines[i].lstrip())
            while j < len(lines) and simple(lines[j]) and len(lines[j]) - len(lines[j].lstrip()) == ind:
                j += 1
            if 3 <= j - i <= MAXRUN:
                out.append((i, j))
            i = max(j, i + 1)
        else:
            i += 1
    return out


def climb(f, text, keep, ex):
    lines = text.split(NL)
    start = next((k for k, l in enumerate(lines) if l and not l[0].isspace() and l.rstrip().endswith('{') and '(' in l
                  and not l.startswith(('static inline', 'typedef', 'struct'))), 0)
    best = score(f, text, keep, 'base')
    base, moves = best, 0
    for a, b in [r for r in runs(lines) if r[0] > start]:
        for _ in range(ROUNDS):
            if best[1] == 100:
                break
            n = b - a
            cand = []
            for i in range(n):
                for j in range(n):
                    if i == j or (j == i + 1):
                        continue
                    run = lines[a:b]
                    x = run.pop(i)
                    run.insert(j if j < i else j - 1, x)
                    if run != lines[a:b]:
                        cand.append(run)
            seen, uniq = set(), []
            for c in cand:
                k = NL.join(c)
                if k not in seen:
                    seen.add(k)
                    uniq.append(c)
            res = list(ex.map(lambda t: score(f, NL.join(lines[:a] + t[1] + lines[b:]), keep, f'm{t[0]}'), enumerate(uniq)))
            k = max(range(len(res)), key=lambda q: res[q]) if res else -1
            if k < 0 or res[k] <= best:
                break
            best = res[k]
            lines[a:b] = uniq[k]
            moves += 1
    return base, best, moves, NL.join(lines)


def main():
    lst, srcdirs, keep = sys.argv[1:4]
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
    os.makedirs(keep, exist_ok=True)
    with ThreadPoolExecutor(jobs) as ex:
        for f in funcs:
            src = None
            for d in srcdirs.split(',') + ['tools/hand_drafts', 'decomp/drafts/c']:
                if os.path.exists(f'{d}/{f}.c'):
                    src = f'{d}/{f}.c'
                    break
            if src is None:
                print(f + TAB + 'no draft', flush=True)
                continue
            s = open(src, encoding='utf-8').read().replace(chr(13) + NL, NL)
            base, best, moves, out = climb(f, s, keep, ex)
            if best > base:
                open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(out)
            print(TAB.join([f, f'{base[0]}/{base[1]}', f'{best[0]}/{best[1]}', f'moves={moves}']), flush=True)


if __name__ == '__main__':
    main()
