"""step 110: extra dead uses of one local (`v = v;` in the head rule's `if (0)` block, 1-3 times) to fix the register rank;
saves a 100% result as tools/hand_drafts/<name>.c. usage: python -I dead_uses110.py SRC_DIR NAME ..."""
import os, re, subprocess, sys
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
d = sys.argv[1]
for n in sys.argv[2:]:
    s = open(os.path.join(d, n + '.c')).read()
    blk = '    if (0) {\n        (*cmd_cnt)++;\n    }'
    if blk not in s:
        print(n, 'no dead block'); continue
    fi = s.index('void %s(' % n)
    decl = s[fi:s.index('\n\n', fi)]
    locs = [m.group(1) for m in re.finditer(r'^\s+[\w ]+?\**\s*(\w+);$', decl, re.M) if m.group(1) != 'cmd_cnt']
    q = os.path.join(d, n + '_du.c')
    best = (score(n, s, q)[0], None, 0, s)
    tot = score(n, s, q)[1]
    for v in locs:
        for k in (1, 2, 3):
            t = s.replace(blk, '    if (0) {\n        (*cmd_cnt)++;\n' + ('        %s = %s;\n' % (v, v)) * k + '    }')
            sc, _ = score(n, t, q)
            if sc > best[0]: best = (sc, v, k, t)
            if sc == tot: break
        if best[0] == tot: break
    sc, v, k, t = best
    print('%-26s %d/%d  extra uses: %s x%d %s' % (n, sc, tot, v, k, 'MATCH' if sc == tot else ''), flush=True)
    if sc == tot and v:
        out = [l for l in t.split('\n') if not (l.startswith('// m2c draft') or l.startswith('#define %s ' % n) or l.startswith('#include "ctx.h"') or l.startswith('#undef %s' % n))]
        while out and not out[0].strip(): out.pop(0)
        open(os.path.join(G, 'tools', 'hand_drafts', n + '.c'), 'w', newline='\n').write(
            '/* %s: step 110 hand draft (the head rule plus %d dead use(s) of %s in its `if (0)` block: dead uses count for the register order, so %s gets the register it has in the original) */\n' % (n, k, v, v) + '\n'.join(out))
