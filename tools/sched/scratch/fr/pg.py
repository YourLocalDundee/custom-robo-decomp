"""per-group permutation sweep of the three-float assignment groups of a C file: pg.py base.c"""
import re, itertools, subprocess, sys, os
base=sys.argv[1]
g=open(base).read()
lines=g.split('\n')
pat=re.compile(r'^\s+(p\.|q\.|pp->)[xyz] = .*;$')
groups=[]; i=0
while i<len(lines):
    if pat.match(lines[i]) and i+2<len(lines) and pat.match(lines[i+1]) and pat.match(lines[i+2]):
        groups.append(i); i+=3
    else: i+=1
def score(L, name):
    open(name,'w').write('\n'.join(L))
    r=subprocess.run([sys.executable,'-I','hm.py','fn_8006EEF8',name],capture_output=True,text=True)
    m=re.search(r'\((\d+) of',r.stdout)
    return int(m.group(1)) if m else -1
cur=list(lines); best=score(cur,'pg_cur.c'); print('base',best)
for gi_idx,gi in enumerate(groups):
    trio=[cur[gi],cur[gi+1],cur[gi+2]]
    res=[]
    for perm in itertools.permutations(range(3)):
        L=list(cur)
        for k in range(3): L[gi+k]=trio[perm[k]]
        res.append((score(L,'pg_try.c'),perm))
    res.sort(reverse=True)
    print('group',gi_idx,'line',gi+1,res)
    if res[0][0]>best:
        best=res[0][0]; perm=res[0][1]
        for k in range(3): cur[gi+k]=trio[perm[k]]
        print('  -> take',perm,best)
open('pg_best.c','w').write('\n'.join(cur)); print('best',best)
