import re, itertools, subprocess, sys
g=open('vD.c').read()
g=g.replace("    s32 dead;\n","    Vec *pp;\n").replace("    if (0) { dead = 1; }\n","")
lines=g.split('\n')
pat=re.compile(r'^(\s+)p\.([xyz]) = (.*;)$')
groups=[]; i=0
while i<len(lines):
    if pat.match(lines[i]) and i+2<len(lines) and pat.match(lines[i+1]) and pat.match(lines[i+2]):
        groups.append(i); i+=3
    else: i+=1
def build(sel, members, defpos):
    out=[]
    for idx,l in enumerate(lines):
        gi=[k for k,gs in enumerate(groups) if gs<=idx<gs+3]
        if gi and gi[0] in sel:
            m=pat.match(l)
            if idx==groups[gi[0]] and defpos=='group': out.append(m.group(1)+"pp = &p;")
            if m.group(2) in members: out.append(m.group(1)+"pp->"+m.group(2)+" = "+m.group(3))
            else: out.append(l)
        else: out.append(l)
    s='\n'.join(out)
    if defpos=='top': s=s.replace("    n = 42;\n","    n = 42;\n    pp = &p;\n")
    return s
def score(src, name):
    open(name,'w').write(src)
    r=subprocess.run([sys.executable,'-I','hm.py','fn_8006EEF8',name,'--diff'],capture_output=True,text=True)
    m=re.search(r'\((\d+) of (\d+) words',r.stdout)
    ours=[l.split(' | ',1)[1] if ' | ' in l else '' for l in r.stdout.split('\n')[1:]]
    gap=not any(re.search(r'\br30\b',o) for o in ours)
    return (int(m.group(1)) if m else -1), (int(m.group(2)) if m else -1), gap
res=[]
for members in ['x','y','z','xy','xz','yz','xyz']:
    for sel in [(1,3),(0,1,2,3),(1,),(3,)]:
        for defpos in ['group','top']:
            s=score(build(set(sel),set(members),defpos),'pps2.c')
            res.append((s,members,sel,defpos)); print(members,sel,defpos,s,flush=True)
res.sort(reverse=True); print('BEST',res[:6])
