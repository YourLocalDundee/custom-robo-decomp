"""Score C shapes of cr_CM_DUMMY_008 (docs/sched_model.md section 16): for each .c, the compile through the hook
(tools/sched/hooklog.py), the word score against the original (scratch/hd/pr.py), what the passes between the
schedulings delete, and the inverse search (scratch/hd/inv/inverse.py: the best both-pass score any input order of
the shape's first-pass PCode reaches).
usage: python -I tools/sched/shape_eval.py DIR_OR_FILES ... [--anneal RESTARTS STEPS]
Sources belong under tools/sched/corpus (gitignored); each log is written beside its source."""
import argparse, glob, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PR = os.path.join(HERE, 'scratch', 'hd', 'pr.py')
INV = os.path.join(HERE, 'scratch', 'hd', 'inv', 'inverse.py')
HOOK = os.path.join(HERE, 'hooklog.py')


def run(args):
    return subprocess.run([sys.executable, '-I'] + args, capture_output=True, text=True, cwd=HERE, errors='replace')


def evaluate(src, restarts, steps):
    log = os.path.splitext(src)[0] + '.hook.txt'
    r = run([HOOK, src, '--log', log])
    if r.returncode:
        return {'src': src, 'error': (r.stdout + r.stderr).strip().splitlines()[-1:]}
    p = run([PR, 'cr_CM_DUMMY_008', src, '--quiet'])
    for e in ('.o', '.o.s'):
        try:
            os.remove(os.path.splitext(src)[0] + e)
        except OSError:
            pass
    m = re.search(r'pos (\d+) / seq (\d+) / ops (\d+) of (\d+) \(orig 38, ours (\d+)\)', p.stdout)
    res = {'src': src, 'pos': int(m.group(1)) if m else -1, 'ops': int(m.group(3)) if m else -1,
           'words': int(m.group(5)) if m else -1}
    q = run([INV, log, '3', str(restarts), str(steps)])
    d = re.search(r'deleted between the passes (\[.*\])', q.stdout)
    b = re.search(r'BEST (\d+) of', q.stdout)
    n = re.search(r'PCode (\d+)', q.stdout)
    res['deleted'] = d.group(1) if d else '?'
    res['inverse'] = int(b.group(1)) if b else -1
    res['pcode'] = int(n.group(1)) if n else -1
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('paths', nargs='+')
    ap.add_argument('--anneal', nargs=2, type=int, default=[3, 2000])
    a = ap.parse_args()
    files = []
    for x in a.paths:
        files += sorted(glob.glob(os.path.join(x, '*.c'))) if os.path.isdir(x) else [x]
    for f in files:
        r = evaluate(os.path.abspath(f), *a.anneal)
        if 'error' in r:
            print('%-10s compile failed %s' % (os.path.basename(f), r['error']), flush=True)
            continue
        print('%-10s words %2d  pos %2d  masked %2d  pcode %2d  inverse best %2d  deleted %s' % (
            os.path.basename(f), r['words'], r['pos'], r['ops'], r['pcode'], r['inverse'], r['deleted']), flush=True)


if __name__ == '__main__':
    main()
