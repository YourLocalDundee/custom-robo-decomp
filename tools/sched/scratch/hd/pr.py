"""scratch: compile a C file with the game flags (+/- extras), disassemble with dtk, compare with the original .s
usage: python -I pr.py NAME file.c [--flags "..."] [--drop "-opt"] [--quiet]"""
import os, re, subprocess, sys, difflib
G = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", ".."))   # the repo root
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
D = db.D
name, cfile = sys.argv[1], os.path.abspath(sys.argv[2])
extra = []; drop = []; quiet = False
args = sys.argv[3:]
while args:
    a = args.pop(0)
    if a == "--flags": extra += args.pop(0).split()
    elif a == "--drop": drop.append(args.pop(0))
    elif a == "--quiet": quiet = True
ver, flags = db.game_flags()
out = []
i = 0
while i < len(flags):
    f = flags[i]
    if f in drop:
        i += 2 if i + 1 < len(flags) and not flags[i+1].startswith("-") else 1
        continue
    out.append(f); i += 1
flags = out + extra
drafts = os.path.join(D, "drafts")
o = os.path.splitext(cfile)[0] + ".o"
if os.path.exists(o): os.remove(o)
cmd = db.mwcc_cmd(ver) + ['-c'] + flags + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', drafts.replace(os.sep, '/'), '-i', os.path.dirname(cfile).replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o, cfile.replace(os.sep, '/')]
r = subprocess.run(cmd, capture_output=True, text=True, cwd=D, errors="replace")
if r.returncode or not os.path.exists(o):
    print("COMPILE FAILED"); print(r.stdout, r.stderr); sys.exit(1)
dtk = os.path.join(G, "tools", "dtk", "dtk.exe")
s = o + ".s"
subprocess.run([dtk, "elf", "disasm", o, s], check=True, capture_output=True)
txt = open(s, encoding="utf-8", errors="replace").read()
m = re.search(r"^\.fn %s\b.*?^\.endfn" % re.escape(name), txt, re.S | re.M)
ours = [l for l in m.group(0).splitlines() if l.startswith("/*")] if m else []
orig = [l for l in open(os.path.join(drafts, "asm", name + ".s"), encoding="utf-8").read().splitlines() if l.startswith("/*")]
def ins(l):
    t = l.split("*/", 1)[1].strip() if "*/" in l else ""
    t = re.sub(r'lbl_803BE358', 'cr_g_Script', t)
    t = re.sub(r'lbl_8044F778', 'cr_g_ScriptPC', t); t = re.sub(r'lbl_8044F770', 'cr_g_ScriptDepth', t)
    t = re.sub(r'\.L_[0-9A-Fa-f]+|lbl_[0-9A-Fa-f]+(?=$|[ ,@])', 'LBL', t)
    return t
A = [ins(l) for l in orig]; Bl = [ins(l) for l in ours]
eq = sum(1 for a, b in zip(A, Bl) if a == b)
sm = difflib.SequenceMatcher(None, A, Bl, autojunk=False)
seq = sum(i2 - i1 for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag == 'equal')
def op(t): return re.sub(r'r\d+', 'r', t)
A2 = [op(a) for a in A]; B2 = [op(b) for b in Bl]
sm2 = difflib.SequenceMatcher(None, A2, B2, autojunk=False)
ops = sum(i2 - i1 for tag, i1, i2, j1, j2 in sm2.get_opcodes() if tag == 'equal')
print("%s: pos %d / seq %d / ops %d of %d (orig %d, ours %d)" % (name, eq, seq, ops, max(len(A), len(Bl)), len(A), len(Bl)))
if not quiet:
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            a = A[i1 + k] if i1 + k < i2 else ''
            b = Bl[j1 + k] if j1 + k < j2 else ''
            print('%s %-40s | %s' % (' ' if tag == 'equal' else '*', a, b))
    print("flags:", " ".join(flags))
