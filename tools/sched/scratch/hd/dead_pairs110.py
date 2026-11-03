"""step 110: dead uses for TWO locals at once (each 1-2 `v = v;` in the head rule's if (0) block) plus the counter's
declaration position; saves a 100% result as a hand draft. usage: python -I dead_pairs110.py SRC_DIR NAME"""
import os, re, subprocess, sys, itertools
HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.abspath(os.path.join(HERE, '..', '..', '..', '..'))
PR = os.path.join(HERE, 'pr.py')
def score(n, s, q):
    open(q, 'w').write(s)
    r = subprocess.run([sys.executable, '-I', PR, n, q, '--quiet'], capture_output=True, text=True)
    l = r.stdout.strip().splitlines()
    if not l or 'pos ' not in l[0]: return -1, 0
    a = l[0].split('pos ')[1]
    return int(a.split(' /')[0]), int(a.split('(orig ')[1].split(',')[0])
d, n = sys.argv[1], sys.argv[2]
s = open(os.path.join(d, n + '.c')).read()
blk = '    if (0) {\n        (*cmd_cnt)++;\n    }'
fi = s.index('void %s(' % n)
decl = s[fi:s.index('\n\n', fi)]
locs = [m.group(1) for m in re.finditer(r'^\s+[\w ]+?\**\s*(\w+);$', decl, re.M) if m.group(1) != 'cmd_cnt']
q = os.path.join(d, n + '_pr.c')
base, tot = score(n, s, q)
best = (base, None, s)
for (v1, k1), (v2, k2) in itertools.combinations([(v, k) for v in locs for k in (1, 2)], 2):
    if v1 == v2: continue
    t = s.replace(blk, '    if (0) {\n        (*cmd_cnt)++;\n' + ('        %s = %s;\n' % (v1, v1)) * k1 + ('        %s = %s;\n' % (v2, v2)) * k2 + '    }')
    sc, _ = score(n, t, q)
    if sc > best[0]:
        best = (sc, (v1, k1, v2, k2), t); print('  better', sc, best[1], flush=True)
    if sc == tot: break
print('%s %d/%d %s %s' % (n, best[0], tot, best[1], 'MATCH' if best[0] == tot else ''))
if best[0] == tot:
    t = best[2]
    out = [l for l in t.split('\n') if not (l.startswith('// m2c draft') or l.startswith('#define %s ' % n) or l.startswith('#include "ctx.h"') or l.startswith('#undef %s' % n))]
    while out and not out[0].strip(): out.pop(0)
    open(os.path.join(G, 'tools', 'hand_drafts', n + '.c'), 'w', newline='\n').write('/* %s: step 110 hand draft (dead uses %s in the head rule\'s if (0) block fix the register order) */\n' % (n, best[1]) + '\n'.join(out))
