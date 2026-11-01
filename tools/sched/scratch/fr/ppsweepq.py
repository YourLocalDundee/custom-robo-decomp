import re, itertools, subprocess, sys
g=open('vD.c').read()
g=g.replace("    s32 dead;\n","    Vec *pq;\n").replace("    if (0) { dead = 1; }\n","")
lines=g.split('\n')
pat=re.compile(r'^(\s+)q\.([xyz]) = (.*;)$')
groups=[]; i=0
while i<len(lines):
    if pat.match(lines[i]) and i+2<len(lines) and pat.match(lines[i+1]) and pat.match(lines[i+2]):
        groups.append(i); i+=3
    else: i+=1
print('p groups', len(groups))
def build(sel, defs=True):
    L=list(lines); off=0
    out=[]
    for idx,l in enumerate(lines):
        gi=[k for k,gs in enumerate(groups) if gs<=idx<gs+3]
        if gi and gi[0] in sel:
            m=pat.match(l)
            if idx==groups[gi[0]] and defs: out.append(m.group(1)+"pq = &q;")
            out.append(m.group(1)+"pq->"+m.group(2)+" = "+m.group(3))
        else: out.append(l)
    return '\n'.join(out)
def score(src, name):
    open(name,'w').write(src)
    r=subprocess.run([sys.executable,'-I','hm.py','fn_8006EEF8',name,'--diff'],capture_output=True,text=True)
    m=re.search(r'\((\d+) of (\d+) words',r.stdout)
    gap='r30' not in r.stdout.split('\n',1)[1].replace('| ','|').split('|')[1::2].__str__() if False else None
    # check the gap: ours must not use r30 anywhere
    ours=[l.split(' | ',1)[1] if ' | ' in l else '' for l in r.stdout.split('\n')[1:]]
    gap=not any(re.search(r'\br30\b',o) for o in ours)
    return (int(m.group(1)) if m else -1), (int(m.group(2)) if m else -1), gap
res=[]
for n in (1,2):
    for sel in itertools.combinations(range(len(groups)), n):
        s=score(build(set(sel)),'pps.c')
        res.append((s,sel)); print(sel, s, flush=True)
res.sort(reverse=True)
print('BEST', res[:8])
