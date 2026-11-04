"""step 110: score source variants of one HSD function. usage: var.py OBJ FUNC 'old' 'new' ['old2' 'new2' ...] (one variant per pair; 'BASE' scores the file as is)"""
import re, subprocess, sys
G = 'D:/emulators/gameTesting'
obj, fn = sys.argv[1], sys.argv[2]
src = open('%s/decomp/src/sysdolphin/baselib/%s.c' % (G, obj), newline='').read().replace('\r\n', '\n')
base = G + '/decomp/src/sysdolphin/baselib/'
# (includes left as they are: nd.py adds the source folder after the object's -i paths)
pairs = sys.argv[3:]
def score(s, tag):
    import os
    d = '%s/tools/sched/scratch/hsd110/v_%s_%s' % (G, fn, tag)
    os.makedirs(d, exist_ok=True)
    p = '%s/%s.c' % (d, obj)   # the file keeps its name: the asserts pass __FILE__
    open(p, 'w', newline='\n').write(s)
    r = subprocess.run([sys.executable, '-I', G + '/tools/sched/scratch/sh/nd.py', '..', 'sysdolphin/baselib/' + obj, p, fn, '-q'], capture_output=True, text=True, cwd=G + '/decomp')
    print(tag, '|', (r.stdout.strip().splitlines() or [r.stderr.strip()[-200:]])[0], flush=True)
score(src, 'base')
for k in range(0, len(pairs), 2):
    a, b = pairs[k].replace(chr(92) + 'n', '\n'), pairs[k + 1].replace(chr(92) + 'n', '\n')
    if a not in src:
        print(k // 2, '| pattern not found'); continue
    score(src.replace(a, b, 1), str(k // 2))
