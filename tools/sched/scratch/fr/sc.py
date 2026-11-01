"""score a hm.py --diff file: words equal by position and by sequence with register numbers masked (r0-r12 only, and all)"""
import sys, re, difflib
A=[];B=[]
for l in open(sys.argv[1], encoding='utf-8', errors='replace'):
    if not re.match(r'^(   |[RDI]  )', l): continue
    a,b=l[3:].split(' | ',1) if ' | ' in l else (l[3:],'')
    a=a.strip(); b=b.strip()
    if a: A.append(a)
    if b: B.append(b)
def m_tmp(t): return re.sub(r'\br(\d|1[0-2])\b','rT',t)
def m_all(t): return re.sub(r'\br\d+\b','r',t)
def seq(X,Y):
    sm=difflib.SequenceMatcher(None,X,Y,autojunk=False); return sum(b.size for b in sm.get_matching_blocks())
def pos(X,Y): return sum(1 for x,y in zip(X,Y) if x==y)
n=max(len(A),len(B))
print('n',len(A),len(B))
print('exact: pos %d seq %d'%(pos(A,B),seq(A,B)))
At=[m_tmp(x) for x in A]; Bt=[m_tmp(x) for x in B]
print('temps masked (r0-r12): pos %d seq %d'%(pos(At,Bt),seq(At,Bt)))
Aa=[m_all(x) for x in A]; Ba=[m_all(x) for x in B]
print('all regs masked: pos %d seq %d'%(pos(Aa,Ba),seq(Aa,Ba)))
