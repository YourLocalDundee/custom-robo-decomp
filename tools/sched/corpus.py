"""The scheduler corpus (docs/sched_model.md): compile each scheduler test program with GC/2.6 and the game flags of
decomp/configure.py twice, WITH `schedule` (the game's `-opt level=0,peephole,schedule,speed`) and WITHOUT it
(`-opt level=0,peephole,speed`), disassemble both objects with dtk and keep both listings per function, plus the
original's words of the two target functions (cr_CM_DUMMY_008, fn_8006EEF8) from decomp/drafts/asm.

Inputs (tools/sched/scratch, saved by steps 84 / 85): hd/m/*.c (micro-programs), hd/base.c and hd/v0.c (the pure
handler head cr_CM_DUMMY_008), fr/v13.c (fn_8006EEF8). A file's own folder is searched for headers first (fr/ has its
own ctx.h), then decomp/drafts (ctx.h) and the draft include paths of tools/draft_batch.py.
Output (gitignored): tools/sched/corpus/<id>.sched.o / .nosched.o and their .s, manifest.json:
  {"files": {id: {"src", "ok", "error", "functions": {name: {"sched": [...], "nosched": [...]}}}},
   "originals": {name: [...]}}
Each listing is the instruction text of dtk's disassembly (the address / bytes comment removed, local labels kept as
`LBL:` lines so blocks can be split). Usage: python -I tools/sched/corpus.py [--only ID ...]
"""
import argparse, json, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(G, 'tools'))
import draft_batch as db  # noqa: E402

SCRATCH = os.path.join(HERE, 'scratch')
OUT = os.path.join(HERE, 'corpus')
DTK = os.path.join(G, 'tools', 'dtk', 'dtk.exe')
OPT_SCHED = 'level=0,peephole,schedule,speed'
OPT_NOSCHED = 'level=0,peephole,speed'
TARGETS = ['cr_CM_DUMMY_008', 'fn_8006EEF8']


def sources():
    out = []
    m = os.path.join(SCRATCH, 'hd', 'm')
    for f in sorted(os.listdir(m)):
        if f.endswith('.c'):
            out.append(('m_' + f[:-2], os.path.join(m, f)))
    out.append(('hd_base', os.path.join(SCRATCH, 'hd', 'base.c')))
    out.append(('hd_v0', os.path.join(SCRATCH, 'hd', 'v0.c')))
    out.append(('fr_v13', os.path.join(SCRATCH, 'fr', 'v13.c')))
    return out


def listing(path):
    """dtk listing -> {function: [lines]}; a line is an instruction text or 'LBL:<label>' at a local label"""
    funcs, cur = {}, None
    for l in open(path, encoding='utf-8', errors='replace'):
        l = l.rstrip('\n')
        m = re.match(r'^\.fn (\S+), \w+', l)
        if m:
            cur = funcs.setdefault(m.group(1), [])
            continue
        if l.startswith('.endfn'):
            cur = None
            continue
        if cur is None:
            continue
        if l.startswith('/*') and '*/' in l:
            cur.append(l.split('*/', 1)[1].strip())
        elif re.match(r'^[.\w@$]+:', l):
            cur.append('LBL:' + l.split(':', 1)[0])
    return funcs


def compile_one(ident, src, opt, ver, flags):
    fl = list(flags)
    fl[fl.index('-opt') + 1] = opt
    tag = 'sched' if 'schedule' in opt else 'nosched'
    o = os.path.join(OUT, '%s.%s.o' % (ident, tag))
    if os.path.exists(o):
        os.remove(o)
    drafts = os.path.join(db.D, 'drafts')
    cmd = (db.mwcc_cmd(ver) + ['-c'] + fl + db.CTX_DEFS + ['-DCR_DRAFT_CC',
           '-i', os.path.dirname(src).replace(os.sep, '/'), '-i', drafts.replace(os.sep, '/')] +
           sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o.replace(os.sep, '/'), src.replace(os.sep, '/')])
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors='replace')
    if r.returncode or not os.path.exists(o):
        return None, (r.stdout + r.stderr).strip()[-600:]
    s = o + '.s'
    r = subprocess.run([DTK, 'elf', 'disasm', o, s], capture_output=True, text=True, errors='replace')
    if r.returncode:
        return None, 'dtk: ' + (r.stdout + r.stderr).strip()[-300:]
    return listing(s), None


def originals():
    out = {}
    for name in TARGETS:
        p = os.path.join(db.D, 'drafts', 'asm', name + '.s')
        if os.path.exists(p):
            out[name] = listing(p).get(name, [])
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--only', nargs='*', help='corpus ids (m_s1, hd_base, fr_v13, ...)')
    a = ap.parse_args()
    os.makedirs(OUT, exist_ok=True)
    ver, flags = db.game_flags()
    man_path = os.path.join(OUT, 'manifest.json')
    man = json.load(open(man_path)) if a.only and os.path.exists(man_path) else {'files': {}}
    man['flags'] = flags
    man['compiler'] = ver
    for ident, src in sources():
        if a.only and ident not in a.only:
            continue
        ent = {'src': os.path.relpath(src, G).replace(os.sep, '/'), 'ok': True, 'error': None, 'functions': {}}
        res = {}
        for opt in (OPT_SCHED, OPT_NOSCHED):
            f, err = compile_one(ident, src, opt, ver, flags)
            if f is None:
                ent['ok'] = False
                ent['error'] = '%s: %s' % (opt, err)
                break
            res['sched' if 'schedule' in opt else 'nosched'] = f
        if ent['ok']:
            for name in res['sched']:
                ent['functions'][name] = {'sched': res['sched'][name], 'nosched': res['nosched'].get(name, [])}
        man['files'][ident] = ent
        print('%-10s %s' % (ident, 'ok %d function(s)' % len(ent['functions']) if ent['ok'] else 'FAILED'))
    man['originals'] = originals()
    with open(man_path, 'w', encoding='utf-8') as fh:
        json.dump(man, fh, indent=1)
    bad = [k for k, v in man['files'].items() if not v['ok']]
    print('%d files, %d compiled, %d failed%s' % (len(man['files']), len(man['files']) - len(bad), len(bad),
                                                 (': ' + ', '.join(bad)) if bad else ''))


if __name__ == '__main__':
    main()
