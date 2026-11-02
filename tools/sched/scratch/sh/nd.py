# nd.py REPO OBJSTEM SRC FUNC [-q]: compile SRC with build.ninja's cflags for src/<OBJSTEM>.c, score/diff FUNC vs main.dol
import sys, struct, subprocess, difflib, re, tempfile, shlex
from pathlib import Path
G = Path(sys.argv[1]); stem = sys.argv[2]; src = Path(sys.argv[3]).resolve(); fn = sys.argv[4]
sys.path.insert(0, str(G/'tools/compiler_probe')); import probe as P
D = G/'decomp'
nin = (D/'build.ninja').read_text().replace('$\n', '')
BS = chr(92); key = 'build build' + BS + 'GXCE01' + BS + 'src' + BS + stem.replace('/', BS) + '.o: mwcc'
i = nin.index(key); blk = nin[i:nin.index('\nbuild ', i+10)]
cflags = re.search(r'\n  cflags = (.*)', blk)[1]; ver = re.search(r'mw_version = (\S+)', blk)[1]
out = Path(tempfile.gettempdir())/'nd.o'
args = shlex.split(cflags.replace(chr(92), '/'), posix=True)
cmd = [str(D/'build/tools/sjiswrap.exe'), str(D/'build/compilers'/ver.replace(chr(92),'/')/'mwcceppc.exe')] + args + ['-c', str(src), '-o', str(out)]
r = subprocess.run(cmd, capture_output=True, cwd=str(D))
if r.returncode: print((r.stdout+r.stderr).decode('latin-1')[-1500:]); sys.exit(1)
funcs = P.elf_functions(out.read_bytes())
a, s = P.our_functions()[fn]
code, masks = funcs[fn]
d = P.DOL.read_bytes()
secs = [(struct.unpack_from('>I',d,0x48+4*i)[0], struct.unpack_from('>I',d,4*i)[0], struct.unpack_from('>I',d,0x90+4*i)[0]) for i in range(18)]
def rd(a,n):
    for va,off,sz in secs:
        if va<=a and a+n<=va+sz: return d[off+a-va:off+a-va+n]
tgt = rd(a, s); e, sc = P.compare(code, masks, tgt)
print(f'{fn}: compiled 0x{len(code):X} retail 0x{s:X} {"EXACT" if e else f"{sc:.4f}"}')
if e or '-q' in sys.argv: sys.exit(0)
OD = str(D/'build/binutils/powerpc-eabi-objdump.exe')
def dis(b, m):
    p = Path(tempfile.gettempdir())/'nd.bin'
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
