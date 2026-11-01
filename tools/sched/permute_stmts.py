"""Statement-order search for a schedule-only draft (docs/drafts.md "Step 92"): the top-level statements of its
function body (one per line in decomp/drafts/c/NAME.c) permuted under def-before-use of the locals, each order compiled
and scored with scratch/hd/pr.py (temporary files in tools/sched/corpus/perm); prints the best order.
usage: python -I tools/sched/permute_stmts.py NAME [MAX_ORDERS]"""
import itertools, os, re, subprocess, sys
HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.abspath(os.path.join(HERE, '..', '..'))
D = os.path.join(HERE, 'corpus', 'perm')
os.makedirs(D, exist_ok=True)
PR = os.path.join(G, 'tools', 'sched', 'scratch', 'hd', 'pr.py')
name = sys.argv[1]; limit = int(sys.argv[2]) if len(sys.argv) > 2 else 800
src = open(os.path.join(G, 'decomp', 'drafts', 'c', name + '.c'), encoding='utf-8').read()
m = re.search(r'\n(\w[^\n]*\b%s\([^)]*\)) \{\n(.*?)\n\}\n' % re.escape(name), src, re.S)
head, body = m.group(1), m.group(2).split('\n')
decl = [l for l in body if re.match(r'^    \w[\w\s\*]*\s\**\w+;$', l) and '=' not in l]
stmts = [l for l in body if l not in decl and l.strip()]
if any(not s.startswith('    ') or s.startswith('     ') or not s.rstrip().endswith(';') for s in stmts):
    print('not a flat statement list'); sys.exit(0)
locs = [re.search(r'(\w+);$', d).group(1) for d in decl]
def ok(order):
    seen = set()
    for s in order:
        lhs = re.match(r'^\s*(\w+) = ', s)
        rhs = s.split(' = ', 1)[1] if lhs else s
        if any(re.search(r'\b%s\b' % v, rhs) and v not in seen for v in locs):
            return False
        if lhs and lhs.group(1) in locs:
            seen.add(lhs.group(1))
    return True
def score(order, tag):
    f = os.path.join(D, '%s_%s.c' % (name, tag))
    open(f, 'w', encoding='utf-8').write(src[:m.start(2)] + '\n'.join(decl + [''] + list(order)) + src[m.end(2):])
    r = subprocess.run([sys.executable, '-I', PR, name, f, '--quiet'], capture_output=True, text=True)
    for e in ('.c', '.o', '.o.s'):
        try: os.remove(f[:-2] + e)
        except OSError: pass
    mm = re.search(r'pos (\d+) / seq (\d+) / ops (\d+) of (\d+)', r.stdout)
    return (int(mm.group(1)), int(mm.group(4))) if mm else (-1, 0)
res = []
for k, order in enumerate(itertools.permutations(stmts)):
    if k >= limit * 50: break
    if not ok(order): continue
    res.append((score(order, 'p'), order))
    if len(res) >= limit or res[-1][0][0] == res[-1][0][1]: break
res.sort(key=lambda x: -x[0][0])
print(name, len(stmts), 'statements,', len(res), 'orders; best', res[0][0] if res else None)
if res:
    print('\n'.join(res[0][1]))
