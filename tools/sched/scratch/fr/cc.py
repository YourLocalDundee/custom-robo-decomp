"""scratch: compile a C file with the game flags and print the disassembly of every function: cc.py file.c"""
import sys, os, re, subprocess
G = r'D:\emulators\gameTesting-hsd'
sys.path.insert(0, os.path.join(G, 'tools'))
os.chdir(os.path.join(G, 'decomp'))
import draft_batch as db
here = os.path.dirname(os.path.abspath(__file__))
src = os.path.join(here, sys.argv[1])
ver, flags = db.game_flags()
flags = [f for f in flags if f != '-i' and not f.startswith('include')] + ['-i', 'include']
o = src[:-2] + '.o'
if os.path.exists(o): os.remove(o)
cmd = db.mwcc_cmd(ver) + ['-c'] + flags + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', here.replace(os.sep, '/'), '-i', os.path.join(db.D, 'drafts').replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o, src.replace(chr(92), '/')]
r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors='replace')
if r.returncode or not os.path.exists(o):
    print((r.stdout + r.stderr).strip()); sys.exit(1)
s = o + '.s'
subprocess.run([os.path.join(G, 'tools/dtk/dtk.exe'), 'elf', 'disasm', os.path.basename(o), os.path.basename(s)], capture_output=True, cwd=here)
for l in open(s, encoding='utf-8', errors='replace'):
    if l.startswith('/*'):
        print(re.sub(r'^/\* [0-9A-F]+ [0-9A-F]+  [0-9A-F ]+ \*/\s*', '', l.rstrip()))
    elif l.startswith('.fn') or l.startswith('.L'):
        print(l.rstrip())
