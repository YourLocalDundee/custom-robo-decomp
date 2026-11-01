"""scratch: compile a hand-written C file of one game function with the game's flags against the drafts' ctx.h and
compare with the original: hm.py NAME file.c [--diff] [--flags EXTRA...]"""
import sys, os, glob, re, subprocess, difflib
G = r'D:\emulators\gameTesting-hsd'
sys.path.insert(0, os.path.join(G, 'tools'))
os.chdir(os.path.join(G, 'decomp'))
import draft_batch as db
name, src = sys.argv[1], os.path.join(os.path.dirname(os.path.abspath(__file__)), sys.argv[2])
diff = '--diff' in sys.argv
extra = sys.argv[sys.argv.index('--flags') + 1:] if '--flags' in sys.argv else []
ver, flags = db.game_flags()
flags = [f for f in flags if f != '-i' and not f.startswith('include')] + ['-i', 'include'] + extra
if '--nosched' in sys.argv: flags = [f.replace(',schedule', '') for f in flags]
D = db.D
out = os.path.join(D, 'drafts')
addr = int(name[3:], 16) if name.startswith('fn_') else None
if addr is None:
    for l in open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8'):
        m = re.match(r'^%s = \.text:0x([0-9A-F]+);' % re.escape(name), l)
        if m: addr = int(m.group(1), 16)
objs = sorted(glob.glob(os.path.join(D, 'build/GXCE01/obj/auto_*_text.o')))
best = max((o for o in objs if int(re.search(r'auto_\d+_([0-9A-F]{8})_', o).group(1), 16) <= addr), key=lambda o: int(re.search(r'auto_\d+_([0-9A-F]{8})_', o).group(1), 16))
orig = db.Elf(open(best, 'rb').read()).func(name)
here = os.path.dirname(os.path.abspath(__file__))
o = os.path.join(here, name + '.o')
cmd = db.mwcc_cmd(ver) + ['-c'] + flags + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', here.replace(os.sep, '/'), '-i', out.replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o, src.replace(chr(92), '/')]
r = subprocess.run(cmd, capture_output=True, text=True, cwd=D, errors='replace')
if r.returncode or not os.path.exists(o):
    print((r.stdout + r.stderr).strip()); sys.exit(1)
mine = db.Elf(open(o, 'rb').read()).func(name)
p, eq = db.compare(orig, mine)
print(f'{name}: {p}% ({eq} of {max(len(orig[0]), len(mine[0]))} words; orig {len(orig[0])}, mine {len(mine[0])})')
def dis(obj, fn):
    s = obj + '.s'
    rr = subprocess.run([os.path.join(G, 'tools/dtk/dtk.exe'), 'elf', 'disasm', os.path.basename(obj), os.path.basename(s)], capture_output=True, text=True, errors='replace', cwd=os.path.dirname(obj))
    if rr.returncode: print(rr.stderr)
    txt = open(s, encoding='utf-8', errors='replace').read()
    m = re.search(r'^\.fn %s,.*?\n(.*?)^\.endfn' % re.escape(fn), txt, re.S | re.M)
    lines = []
    for l in m.group(1).splitlines():
        l = re.sub(r'^/\* [0-9A-F]{8} [0-9A-F]{8}  [0-9A-F ]{11} \*/\t', '', l)
        l = re.sub(r'^/\* [0-9A-F]+ [0-9A-F]+  [0-9A-F ]+ \*/\s*', '', l)
        lines.append(l.replace('cr_WGFifo@ha', '0xcc01').replace('cr_WGFifo@l', '-0x8000'))
    return lines
if diff or p < 100:
    a, b = dis(best, name), dis(o, name)
    norm = lambda L: [x.replace('cr_WGFifo@ha', '0xcc01').replace('cr_WGFifo@l', '-0x8000') for x in L2] if False else None
    norm = lambda L: [re.sub(r'\.L_[0-9A-F]+', '.L', re.sub(r'\b(lbl|fn|jumptable)_[0-9A-F]{8}\b|@\w+|\$\$\d+', 'SYM', x)) for x in L]
    sm = difflib.SequenceMatcher(None, norm(a), norm(b), autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            if diff:
                for k in range(i2 - i1): print(f'   {a[i1+k]:48s} | {b[j1+k]}')
            else:
                print(f'   ... {i2-i1} equal')
        else:
            for k in range(max(i2 - i1, j2 - j1)):
                l = a[i1+k] if i1+k < i2 else ''
                rr = b[j1+k] if j1+k < j2 else ''
                print(f'{tag[0].upper()}  {l:48s} | {rr}')
