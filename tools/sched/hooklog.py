"""Compile one C file with the analysis copy of GC/2.6 (tools/sched/bin, its lmgr326b.dll the scheduler hook of
tools/sched/hook) and the game flags, and write the hook's log (docs/sched_model.md section 10).
usage: python -I tools/sched/hooklog.py file.c [--log out.txt] | --all [--opt level=0,peephole,schedule,speed]
Imported by replay.py (`hook_compile`)."""
import argparse, os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(G, 'tools'))
sys.path.insert(0, HERE)
import draft_batch as db  # noqa: E402
import corpus  # noqa: E402

BIN = os.path.join(HERE, 'bin', 'mwcceppc.exe')


def hook_compile(src, log, opt=corpus.OPT_SCHED, obj=None):
    if not os.path.exists(os.path.join(HERE, 'bin', 'lmgr326b.dll')):
        raise SystemExit('the hook is not built: pwsh tools/sched/hook/build.ps1')
    ver, flags = db.game_flags()
    fl = list(flags)
    fl[fl.index('-opt') + 1] = opt
    obj = obj or os.path.splitext(log)[0] + '.o'
    if os.path.exists(log):
        os.remove(log)
    drafts = os.path.join(db.D, 'drafts')
    cmd = ([BIN, '-c'] + fl + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', os.path.dirname(src).replace(os.sep, '/'),
           '-i', drafts.replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) +
           ['-o', obj.replace(os.sep, '/'), src.replace(os.sep, '/')])
    env = dict(os.environ, SCHEDLOG=os.path.abspath(log))
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors='replace', env=env)
    if r.returncode:
        raise RuntimeError((r.stdout + r.stderr)[-800:])
    return log


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('src', nargs='?')
    ap.add_argument('--all', action='store_true', help='every corpus source -> tools/sched/corpus/<id>.hook.txt')
    ap.add_argument('--log')
    ap.add_argument('--opt', default=corpus.OPT_SCHED)
    a = ap.parse_args()
    if a.all:
        for ident, src in corpus.sources():
            try:
                print(hook_compile(src, os.path.join(corpus.OUT, ident + '.hook.txt'), a.opt))
            except RuntimeError as e:
                print(ident, 'FAILED', str(e).splitlines()[-1:] if str(e) else '')
        return
    log = a.log or os.path.join(HERE, 'corpus', os.path.splitext(os.path.basename(a.src))[0] + '.hook.txt')
    print(hook_compile(os.path.abspath(a.src), os.path.abspath(log), a.opt))


if __name__ == '__main__':
    main()
