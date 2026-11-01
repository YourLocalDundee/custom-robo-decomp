"""compile + disassemble f, print instructions only. usage: python -I dis.py file.c [--flags ..]"""
import os, re, subprocess, sys
G = r"D:\emulators\gameTesting-hsd2"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
cfile = os.path.abspath(sys.argv[1]); extra = []
args = sys.argv[2:]
while args:
    a = args.pop(0)
    if a == "--flags": extra += args.pop(0).split()
ver, flags = db.game_flags(); flags += extra
drafts = os.path.join(db.D, "drafts")
o = os.path.splitext(cfile)[0] + ".o"
cmd = db.mwcc_cmd(ver) + ['-c'] + flags + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', drafts.replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o, cfile.replace(os.sep, '/')]
r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors="replace")
if r.returncode: print("COMPILE FAILED", r.stdout, r.stderr); sys.exit(1)
s = o + ".s"
subprocess.run([os.path.join(G, "tools", "dtk", "dtk.exe"), "elf", "disasm", o, s], check=True, capture_output=True)
for l in open(s, encoding="utf-8", errors="replace"):
    if l.startswith("/*"): print("   " + l.split("*/", 1)[1].strip())
    elif l.startswith(".fn"): print(l.strip())
