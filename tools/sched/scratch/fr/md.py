"""masked diff of a hm.py --diff file: register numbers masked, print the hunks with their line numbers of the original"""
import sys, re, difflib
A=[];B=[]
for l in open(sys.argv[1], encoding='utf-8', errors='replace'):
    if not re.match(r'^(   |[RDI]  )', l): continue
    a,b=l[3:].split(' | ',1) if ' | ' in l else (l[3:],'')
    a=a.strip(); b=b.strip()
    if a and not a.startswith('.L'): A.append(a)
    if b and not b.startswith('.L'): B.append(b)
def m(t): return re.sub(r'\br\d+\b','r',t)
Am=[m(x) for x in A]; Bm=[m(x) for x in B]
sm=difflib.SequenceMatcher(None,Am,Bm,autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    print('--- orig %d-%d | ours %d-%d (%s)'%(i1,i2-1,j1,j2-1,tag))
    for k in range(max(i2-i1,j2-j1)):
        a=A[i1+k] if i1+k<i2 else ''; b=B[j1+k] if j1+k<j2 else ''
        print('   %-40s | %s'%(a,b))
