"""step 110: score source variants of one HSD function, several edits per variant.
usage: var2.py OBJ FUNC VARIANTS.py   (VARIANTS.py defines V = {tag: [(old, new), ...]}; every edit replaces once)"""
import os, subprocess, sys
G = 'D:/emulators/gameTesting'
obj, fn, vf = sys.argv[1], sys.argv[2], sys.argv[3]
src = open('%s/decomp/src/sysdolphin/baselib/%s.c' % (G, obj), newline='').read().replace('\r\n', '\n')
ns = {}
exec(open(vf, encoding='utf-8').read(), ns)


def score(s, tag):
    d = '%s/tools/sched/scratch/hsd110/v_%s_%s' % (G, fn, tag)
    os.makedirs(d, exist_ok=True)
    p = '%s/%s.c' % (d, obj)   # the file keeps its name: the asserts pass __FILE__
    open(p, 'w', newline='\n').write(s)
    r = subprocess.run([sys.executable, '-I', G + '/tools/sched/scratch/sh/nd.py', '..', 'sysdolphin/baselib/' + obj, p, fn, '-q'],
                       capture_output=True, text=True, cwd=G + '/decomp')
    out = r.stdout.strip().splitlines() or [r.stderr.strip()[-200:]]
    err = [l for l in out if 'Error' in l or 'rror:' in l]
    print(tag, '|', out[-1] if not err else 'ERR ' + ' / '.join(out[-6:])[:300], flush=True)


for tag, edits in ns['V'].items():
    s = src
    ok = True
    for a, b in edits:
        if a not in s:
            print(tag, '| pattern not found:', a[:60]); ok = False; break
        s = s.replace(a, b, 1)
    if ok:
        score(s, tag)
