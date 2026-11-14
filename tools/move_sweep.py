"""move_sweep.py FUNC SRC PATTERN [PATTERN ...] [--rounds N] [--out FILE]: move single lines of a draft to every
place in its function and keep the best (Nov 14, 2026).

order_sweep.py / order_climb.py reorder statements inside one run (consecutive simple statements at one indent).
Many drafts need a move across a run boundary instead: m2c prints the statements in the scheduler's order, and a
statement of the body can come out inside the script command prologue (cr_SYS_SAVE_RET: `cr_g_Scenario.unk_000 |= 1`
printed between the history update and the PC step, 43%; first in the body after the prologue, 100%), or an
argument read lands in the wrong place relative to the stores (cr_E_WIN). Here each line that contains a PATTERN
(the first such line) is taken out and put back at every line of the function body (any indent, any block: the
score decides; a draft is only ever accepted at 100%, where its code is the original's), each version compiled and
scored with tools/sched/scratch/hand/hd.py (equal words); the best place is kept and the next PATTERN moved, for N
rounds (default 2) or until 100%. A 100% result is checked with relcheck.py. Writes the best version to FILE
(default build/move_sweep/FUNC.c); prints one line per move.

Patterns are plain substrings of the line, e.g. "unk_40 = 8" or "time = pc[1]". Useful after typing a draft by
hand: name the statements whose place is unsure (the stores of a small block, a local's first assignment).
"""
import os
import re
import subprocess
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
NL = chr(10)
HD = os.path.join('tools', 'sched', 'scratch', 'hand', 'hd.py')
RC = os.path.join('tools', 'sched', 'scratch', 'hand', 'relcheck.py')


def score(f, lines, tmp):
    open(tmp, 'w', encoding='utf-8', newline=NL).write(NL.join(lines))
    out = subprocess.run([sys.executable, '-I', HD, 'decomp/drafts', f, tmp], capture_output=True,
                         text=True).stdout.split(NL)[0]
    m = re.search(r': (\d+)%\s+\((\d+) equal', out)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def body_range(f, lines):
    head = re.compile(r'^[A-Za-z_][\w \*]*\b' + re.escape(f) + r'\(')
    s = next(k for k, l in enumerate(lines) if head.match(l) and l.rstrip().endswith('{')) + 1
    depth, e = 1, s
    for k in range(s, len(lines)):
        depth += lines[k].count('{') - lines[k].count('}')
        if depth == 0:
            e = k
            break
    return s, e


def sweep(f, lines, pat, tmp):
    i = next((k for k, l in enumerate(lines) if pat in l), None)
    if i is None:
        return None, None
    rest = lines[:i] + lines[i + 1:]
    s, e = body_range(f, rest)
    best = None
    for p in range(s, e + 1):
        cand = rest[:p] + [lines[i]] + rest[p:]
        sc = score(f, cand, tmp)
        if best is None or sc > best[0]:
            best = (sc, cand)
    return best


def main():
    a = sys.argv[1:]
    rounds, out = 2, None
    if '--rounds' in a:
        k = a.index('--rounds')
        rounds = int(a[k + 1])
        del a[k:k + 2]
    if '--out' in a:
        k = a.index('--out')
        out = a[k + 1]
        del a[k:k + 2]
    f, src, pats = a[0], a[1], a[2:]
    out = out or os.path.join('build', 'move_sweep', f + '.c')
    os.makedirs(os.path.dirname(out) or '.', exist_ok=True)
    tmp = out + '.tmp.c'
    lines = open(src, encoding='utf-8').read().replace(chr(13), '').split(NL)
    cur = score(f, lines, tmp)
    print(f'{f}: start {cur[0]}% ({cur[1]} words)')
    for r in range(rounds):
        for pat in pats:
            sc, cand = sweep(f, lines, pat, tmp)
            if sc is None:
                print(f'  round {r + 1}: no line contains {pat!r}')
                continue
            if sc >= cur:
                lines, cur = cand, sc
            print(f'  round {r + 1}: {pat!r} -> {cur[0]}% ({cur[1]} words)')
            if cur[0] == 100:
                break
        if cur[0] == 100:
            break
    open(out, 'w', encoding='utf-8', newline=NL).write(NL.join(lines))
    if os.path.exists(tmp):
        os.remove(tmp)
    msg = ''
    if cur[0] == 100:
        rc = subprocess.run([sys.executable, '-I', RC, 'decomp/drafts', f, out], capture_output=True,
                            text=True).stdout.strip().split(NL)[-1]
        msg = ' | ' + rc + (' | MATCH' if rc.startswith('relocations equal') else '')
    print(f'{f}: best {cur[0]}% ({cur[1]} words) -> {out}{msg}')


if __name__ == '__main__':
    main()
