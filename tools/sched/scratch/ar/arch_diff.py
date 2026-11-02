import sys, tempfile, struct, subprocess, difflib, re
from pathlib import Path
G = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(G/'tools/compiler_probe'))
import probe as P, hsd_hunt as H
src = Path(sys.argv[1]).resolve(); fn = sys.argv[2]
real = G/'decomp/src/sysdolphin/baselib/archive.c'
inc = next(i for f, i, d in H.c_sets()['ours'] if f.name == 'archive.c')
d = P.DOL.read_bytes()
secs = [(struct.unpack_from('>I',d,0x48+4*i)[0], struct.unpack_from('>I',d,4*i)[0], struct.unpack_from('>I',d,0x90+4*i)[0]) for i in range(18)]
def rd(a,n):
    for va,off,sz in secs:
        if va<=a and a+n<=va+sz: return d[off+a-va:off+a-va+n]
with tempfile.TemporaryDirectory(dir=P.OUT) as t:
    funcs, err = P.compile_one('1.3.2', H.HSD_FLAGS, ('x','x',src, inc+['-i',str(real.parent)], ['-DVERSION_GXCE01','-DMUST_MATCH']+H.COMMON_DEFS, []), t)
code, masks = funcs[fn]; a, s = P.our_functions()[fn]; tgt = rd(a, s)
OD = str(G/'decomp/build/binutils/powerpc-eabi-objdump.exe')
def dis(b, m):
    p = Path(P.OUT)/'x.bin'
    w = [struct.unpack_from('>I', b, i)[0] & ~m[i//4] for i in range(0, len(b), 4)]
    p.write_bytes(b''.join(struct.pack('>I', x) for x in w))
    out = subprocess.run([OD, '-D', '-b', 'binary', '-m', 'powerpc', '-EB', str(p)], capture_output=True, text=True).stdout
    L = [re.sub(r'\s+', ' ', l.split('\t', 2)[-1]).strip() for l in out.splitlines() if re.match(r'\s+[0-9a-f]+:\t', l)]
    return [re.sub(r'0x[0-9a-f]+ <.*>|[0-9a-f]+ <.*>', 'X', x) for x in L]
A, B = dis(code, masks), dis(tgt, masks)
sm = difflib.SequenceMatcher(None, A, B, autojunk=False)
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == 'equal': continue
    print(f'--- {op} compiled[{i1}:{i2}] retail[{j1}:{j2}]')
    for k in range(max(i2-i1, j2-j1)):
        x = A[i1+k] if i1+k < i2 else ''; y = B[j1+k] if j1+k < j2 else ''
        print(f'   {x:40s} | {y}')
