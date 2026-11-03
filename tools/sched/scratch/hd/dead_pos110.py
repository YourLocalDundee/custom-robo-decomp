"""step 110: for a script handler draft, search the dead-code counter's declaration position and dead form (the register
rank of the head rule's counter local); print the best, save a 100% one as tools/hand_drafts/<name>.c.
usage: python -I dead_pos110.py SRC_DIR NAME ..."""
import os, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.abspath(os.path.join(HERE, '..', '..', '..', '..'))
PR = os.path.join(HERE, 'pr.py')
DEAD = {'inc': '(*cmd_cnt)++;', 'two': '(*cmd_cnt)++; (*cmd_cnt)++;', 'three': '(*cmd_cnt)++; (*cmd_cnt)++; (*cmd_cnt)++;',
        'self': 'cmd_cnt = cmd_cnt;', 'store': '*cmd_cnt = 0;'}
def score(n, s, q):
    open(q, 'w').write(s)
    r = subprocess.run([sys.executable, '-I', PR, n, q, '--quiet'], capture_output=True, text=True)
    l = r.stdout.strip().splitlines()
    if not l or 'pos ' not in l[0]: return -1, 0
    a = l[0].split('pos ')[1]
    pos = int(a.split(' /')[0]); tot = int(a.split('(orig ')[1].split(',')[0])
    return pos, tot
d = sys.argv[1]
for n in sys.argv[2:]:
    s = open(os.path.join(d, n + '.c')).read()
    if 'u32 *cmd_cnt;' not in s or 'if (0) {' not in s:
        print(n, 'no dead counter'); continue
    L = s.split('\n')
    del L[next(k for k, l in enumerate(L) if l.strip() == 'u32 *cmd_cnt;')]
    fi = next(k for k, l in enumerate(L) if l.startswith('void %s(' % n))
    ndecl = next(k for k in range(fi + 1, len(L)) if not L[k].strip())
    best = None
    q = os.path.join(d, n + '_dp.c')
    for pos in range(fi + 1, ndecl + 1):
        for dk, dv in DEAD.items():
            M = list(L); M.insert(pos, '    u32 *cmd_cnt;')
            t = '\n'.join(M).replace('    if (0) {\n        (*cmd_cnt)++;\n    }', '    if (0) {\n        %s\n    }' % dv)
            sc, tot = score(n, t, q)
            if best is None or sc > best[0]: best = (sc, tot, pos - fi, dk, t)
            if sc == tot: break
        if best and best[0] == best[1]: break
    sc, tot, p, dk, t = best
    tag = 'MATCH' if sc == tot else ''
    print('%-26s %d/%d  decl %d  %s %s' % (n, sc, tot, p, dk, tag), flush=True)
    if sc == tot:
        out = [l for l in t.split('\n') if not (l.startswith('// m2c draft') or l.startswith('#define %s ' % n) or l.startswith('#include "ctx.h"') or l.startswith('#undef %s' % n))]
        while out and not out[0].strip(): out.pop(0)
        open(os.path.join(G, 'tools', 'hand_drafts', n + '.c'), 'w', newline='\n').write(
            '/* %s: step 110 hand draft (the head rule, its dead-code counter declared at position %d with `%s`: its rank among the locals gives the original\'s registers) */\n' % (n, p, DEAD[dk]) + '\n'.join(out))
