"""run pr.py --quiet on every .c in a folder (parallel), print scores sorted. usage: python -I runall.py NAME DIR [extra args]"""
import sys, os, subprocess, concurrent.futures as cf, re
S = os.path.dirname(os.path.abspath(__file__))
name, d = sys.argv[1], sys.argv[2]
extra = sys.argv[3:]
files = sorted(f for f in os.listdir(d) if f.endswith('.c'))
def one(f):
    r = subprocess.run([sys.executable, '-I', os.path.join(S, 'pr.py'), name, os.path.join(d, f), '--quiet'] + extra, capture_output=True, text=True)
    out = (r.stdout.strip().splitlines() or ['?'])
    return f, out[0] if 'pos' in out[0] else 'FAIL ' + ' '.join(out[:3])[:160]
with cf.ThreadPoolExecutor(8) as ex:
    res = list(ex.map(one, files))
def key(x):
    m = re.search(r'pos (\d+) / seq (\d+)', x[1]); return (-int(m.group(1)), -int(m.group(2))) if m else (1, 0)
for f, r in sorted(res, key=key): print('%-28s %s' % (f, r))
