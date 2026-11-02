# usage: sh_diff.py REPO SRC FUNC [ADDR]  - compile SRC with shadow.c's build flags, instruction diff of FUNC vs main.dol
import sys, struct, subprocess, difflib, re, tempfile
from pathlib import Path
G = Path(sys.argv[1]); src = Path(sys.argv[2]).resolve(); fn = sys.argv[3]
sys.path.insert(0, str(G/'tools/compiler_probe')); import probe as P
D = G/'decomp'
out = Path(tempfile.gettempdir())/'shd.o'
cmd = [str(D/'build/tools/sjiswrap.exe'), str(D/'build/compilers/GC/1.3.2/mwcceppc.exe'), '-nodefaults','-proc','gekko','-align','powerpc','-enum','int','-fp','hardware','-Cpp_exceptions','off','-pragma','cats off','-nosyspath','-maxerrors','3','-cwd','explicit','-i',str(D/'include/kar_shadow'),'-i',str(D/'include/kar_shadow/sysdolphin'),'-W','off','-DBUILD_VERSION=0','-DVERSION_GXCE01','-DNDEBUG=1','-O4,p','-inline','auto','-fp_contract','on','-str','reuse','-DMUST_MATCH','-lang=c','-c',str(src),'-o',str(out)]
r = subprocess.run(cmd, capture_output=True, cwd=str(D))
if r.returncode: print((r.stdout+r.stderr).decode('latin-1')[-1500:]); sys.exit(1)
funcs = P.elf_functions(out.read_bytes())
ours = P.our_functions()
a, s = ours[fn] if fn in ours else (int(sys.argv[4],16), None)
code, masks = funcs[fn]; s = s or len(code)
d = P.DOL.read_bytes()
secs = [(struct.unpack_from('>I',d,0x48+4*i)[0], struct.unpack_from('>I',d,4*i)[0], struct.unpack_from('>I',d,0x90+4*i)[0]) for i in range(18)]
def rd(a,n):
    for va,off,sz in secs:
        if va<=a and a+n<=va+sz: return d[off+a-va:off+a-va+n]
tgt = rd(a, s); e, sc = P.compare(code, masks, tgt)
print(f'{fn}: compiled 0x{len(code):X} retail 0x{s:X} {"EXACT" if e else f"{sc:.3f}"}')
if e or '-q' in sys.argv: sys.exit(0)
OD = str(D/'build/binutils/powerpc-eabi-objdump.exe')
def dis(b, m):
    p = Path(tempfile.gettempdir())/'x.bin'
    w = [struct.unpack_from('>I', b, i)[0] & ~(m[i//4] if i//4 < len(m) else 0) for i in range(0, len(b), 4)]
    p.write_bytes(b''.join(struct.pack('>I', x) for x in w))
    o = subprocess.run([OD,'-D','-b','binary','-m','powerpc','-EB',str(p)], capture_output=True, text=True).stdout
    return [re.sub(r'\s+',' ',l.split('\t',2)[-1]).strip() for l in o.splitlines() if re.match(r'\s+[0-9a-f]+:\t', l)]
A, B = dis(code, masks), dis(tgt, masks)
for op,i1,i2,j1,j2 in difflib.SequenceMatcher(None, A, B, autojunk=False).get_opcodes():
    if op == 'equal': continue
    print(f'--- {op} c[{i1}:{i2}] r[{j1}:{j2}]')
    for k in range(max(i2-i1, j2-j1)):
        x = A[i1+k] if i1+k < i2 else ''; y = B[j1+k] if j1+k < j2 else ''
        print(f'   {x:42s} | {y}')
