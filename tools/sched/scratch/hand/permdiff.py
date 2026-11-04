"""permdiff.py FUNC [OUTPUT]: what decomp-permuter changed, as a statement diff of tools/sched/permuter/FUNC/base.c
against an output's source.c (the best-scoring output when none is given). Whitespace and parentheses are dropped
before comparing (the permuter re-prints the whole file), so only real changes remain."""
import difflib
import os
import re
import sys

func = sys.argv[1]
d = os.path.join('tools', 'sched', 'permuter', func)
if len(sys.argv) > 2:
    out = sys.argv[2]
else:
    outs = [o for o in os.listdir(d) if o.startswith('output-')]
    out = min(outs, key=lambda o: int(open(os.path.join(d, o, 'score.txt')).read().split()[0]))
print('output:', out, 'score', open(os.path.join(d, out, 'score.txt')).read().split()[0])


def norm(t):
    t = t[t.index(func + '('):]
    t = re.sub(r'\s+', ' ', t)
    t = re.sub(r'[()]', '', t)
    t = re.sub(r'\s*\*\s*', '*', t)
    return [x.strip() for x in re.split(r'(?<=[;{}])', t) if x.strip()]


a = norm(open(os.path.join(d, 'base.c')).read())
b = norm(open(os.path.join(d, out, 'source.c')).read())
for line in difflib.unified_diff(a, b, lineterm='', n=1):
    print(line[:240])
