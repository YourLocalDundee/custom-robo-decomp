# compile shadow.c, compare its data sections with main.dol's ranges; print symbols and first differing offsets
import sys, struct, subprocess, tempfile
from pathlib import Path
G = Path(sys.argv[1]); src = Path(sys.argv[2]).resolve()
sys.path.insert(0, str(G/'tools/compiler_probe')); import probe as P
D = G/'decomp'; out = Path(tempfile.gettempdir())/'shdd.o'
cmd = [str(D/'build/tools/sjiswrap.exe'), str(D/'build/compilers/GC/1.3.2/mwcceppc.exe'), '-nodefaults','-proc','gekko','-align','powerpc','-enum','int','-fp','hardware','-Cpp_exceptions','off','-pragma','cats off','-nosyspath','-maxerrors','3','-cwd','explicit','-i',str(D/'include/kar_shadow'),'-i',str(D/'include/kar_shadow/sysdolphin'),'-W','off','-DBUILD_VERSION=0','-DVERSION_GXCE01','-DNDEBUG=1','-O4,p','-inline','auto','-fp_contract','on','-str','reuse','-DMUST_MATCH','-lang=c','-c',str(src),'-o',str(out)]
r = subprocess.run(cmd, capture_output=True, cwd=str(D))
if r.returncode: print((r.stdout+r.stderr).decode('latin-1')[-1500:]); sys.exit(1)
d = out.read_bytes()
shoff, = struct.unpack_from('>I', d, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('>HHH', d, 0x2E)
sh = [struct.unpack_from('>10I', d, shoff + i*shentsize) for i in range(shnum)]
nm = lambda tab, o: d[sh[tab][4]+o:d.index(b'\0', sh[tab][4]+o)].decode()
names = [nm(shstrndx, s[0]) for s in sh]
st = next(i for i,s in enumerate(sh) if s[1]==2)
syms = []
for k in range(sh[st][5]//16):
    n,v,sz,info,oth,shn = struct.unpack_from('>IIIBBH', d, sh[st][4]+k*16)
    if shn and shn < len(sh) and (info & 15) in (0,1): syms.append((names[shn], v, sz, nm(sh[st][6], n)))
dol = P.DOL.read_bytes()
secs = [(struct.unpack_from('>I',dol,0x48+4*i)[0], struct.unpack_from('>I',dol,4*i)[0], struct.unpack_from('>I',dol,0x90+4*i)[0]) for i in range(18)]
def rd(a,n):
    for va,off,sz in secs:
        if va<=a and a+n<=va+sz: return dol[off+a-va:off+a-va+n]
RET = {'.data': (0x8024D0D0, 0x8024D268), '.sdata': (0x8044F210, 0x8044F229), '.sdata2': (0x80452968, 0x804529A0), '.bss': (0x8044B780, 0x8044B7B0)}
for i,s in enumerate(sh):
    n = names[i]
    if n not in RET: continue
    lo, hi = RET[n]; size = s[5]
    mine = d[s[4]:s[4]+size] if s[1] != 8 else b'\0'*size
    ret = rd(lo, hi-lo) if n != '.bss' else b'\0'*(hi-lo)
    diffs = [k for k in range(min(len(mine), len(ret))) if mine[k] != ret[k]]
    print(f'{n:8s} compiled 0x{size:X} retail 0x{hi-lo:X}  differing bytes {len(diffs)}' + (f' first at +0x{diffs[0]:X}' if diffs else ''))
    for sec,v,sz,sn in sorted(x for x in syms if x[0]==n):
        print(f'     +0x{v:03X} {sz:#5x} {sn}')
