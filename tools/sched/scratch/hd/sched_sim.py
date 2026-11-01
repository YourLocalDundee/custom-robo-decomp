"""Fit a list-scheduler model: take the UNSCHEDULED compile (pre-schedule order) of a C file, build the DAG with the
inferred alias rules, simulate variants, compare to the SCHEDULED compile. usage: python -I sched_sim.py file.c [fn]"""
import os, re, subprocess, sys, itertools
G = r"D:\emulators\gameTesting-hsd2"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
cfile = os.path.abspath(sys.argv[1]); fn = sys.argv[2] if len(sys.argv) > 2 else None
ver, flags = db.game_flags()
drafts = os.path.join(db.D, "drafts")
def compile_(opt):
    fl = [f for f in flags]
    i = fl.index('-opt'); fl[i + 1] = opt
    o = os.path.splitext(cfile)[0] + ('.sched.o' if 'schedule' in opt else '.nosched.o')
    cmd = db.mwcc_cmd(ver) + ['-c'] + fl + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', drafts.replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o, cfile.replace(os.sep, '/')]
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors="replace")
    if r.returncode: print("COMPILE FAILED", r.stdout, r.stderr); sys.exit(1)
    s = o + ".s"
    subprocess.run([os.path.join(G, "tools", "dtk", "dtk.exe"), "elf", "disasm", o, s], check=True, capture_output=True)
    txt = open(s, encoding="utf-8", errors="replace").read()
    m = re.search(r"^\.fn (%s)\b.*?^\.endfn" % (re.escape(fn) if fn else r'\w+'), txt, re.S | re.M)
    return [l.split("*/", 1)[1].strip() for l in m.group(0).splitlines() if l.startswith("/*")]
pre = compile_('level=0,peephole,speed')
post = compile_('level=0,peephole,schedule,speed')
def op(t): return re.sub(r'\br\d+\b', 'r', t)
print("PRE (unscheduled):"); [print('  %2d %s' % (i, t)) for i, t in enumerate(pre)]
print("POST (scheduled):"); [print('  %2d %s' % (i, t)) for i, t in enumerate(post)]
