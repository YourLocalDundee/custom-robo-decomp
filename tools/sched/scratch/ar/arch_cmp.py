import sys, tempfile
from pathlib import Path
G = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(G/'tools/compiler_probe'))
import probe as P, hsd_hunt as H
src = Path(sys.argv[1]).resolve()
real = G/'decomp/src/sysdolphin/baselib/archive.c'
inc = next(i for f, i, d in H.c_sets()['ours'] if f.name == 'archive.c')
defs = ['-DVERSION_GXCE01', '-DMUST_MATCH'] + H.COMMON_DEFS
ours = P.our_functions()
import struct
d = P.DOL.read_bytes()
secs = [(struct.unpack_from('>I',d,0x48+4*i)[0], struct.unpack_from('>I',d,4*i)[0], struct.unpack_from('>I',d,0x90+4*i)[0]) for i in range(18)]
def rd(a,n):
    for va,off,sz in secs:
        if va<=a and a+n<=va+sz: return d[off+a-va:off+a-va+n]
P.OUT.mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory(dir=P.OUT) as t:
    funcs, err = P.compile_one('1.3.2', H.HSD_FLAGS, ('x', 'x', src, inc + ['-i', str(real.parent)], defs, []), t)
if err: print('ERR', err); sys.exit(1)
for n, (code, masks) in funcs.items():
    if n in ours:
        a, s = ours[n]; e, sc = P.compare(code, masks, rd(a, s))
        print(f'{n:30s} ours 0x{s:X} compiled 0x{len(code):X} {"EXACT" if e else f"{sc:.3f}"}')
    else: print(n, 'not in our symbols', hex(len(code)))
