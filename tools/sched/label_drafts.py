"""Label the near-miss game drafts by the kind of difference left (docs/drafts.md "Step 91", docs/sched_model.md).

For every function at 80-99% in decomp/drafts/draft_report.txt (the last batch 12), compile its draft
(decomp/drafts/c/<name>.c) with GC/2.6 and the game flags, disassemble it with dtk and compare it with the original
(decomp/drafts/asm/<name>.s), block by block (blocks end at branches, labels and calls, as the compiler's PCode does):
  frame          only the frame code differs (stack size, saved registers, their stores / loads)
  register-only  the body is the original word for word once register numbers are masked
  schedule-only  every block has the original's instructions (registers masked) in another order
  structural     at least one block has other instructions (a different source shape, type, constant ...)
A function whose frame differs as well carries "+frame". Output: decomp/reports/sched_labels.txt (one line per
function, then the counts). Usage: python -I tools/sched/label_drafts.py [--jobs N] [--only NAME ...]
"""
import argparse, collections, concurrent.futures as cf, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(G, 'tools'))
sys.path.insert(0, HERE)
import draft_batch as db  # noqa: E402
import corpus  # noqa: E402
import replay as rp  # noqa: E402

DR = os.path.join(db.D, 'drafts')
TMP = os.path.join(HERE, 'corpus', 'labels')
OUT = os.path.join(db.D, 'reports', 'sched_labels.txt')


def near_misses():
    out = []
    for l in open(os.path.join(DR, 'draft_report.txt'), encoding='utf-8', errors='replace'):
        f = l.split()
        if len(f) >= 4 and re.match(r'^[0-9A-F]{8}$', f[0]) and re.match(r'^\d+%$', f[3]):
            v = int(f[3][:-1])
            if 80 <= v <= 99:
                out.append((f[1], v))
    return out


def norm(t):
    t = re.sub(r'\.L_[0-9A-Fa-f]+|lbl_[0-9A-Fa-f]+(?=$|[ ,@])', 'LBL', t)
    return t


def compile_draft(name):
    src = os.path.join(DR, 'c', name + '.c')
    o = os.path.join(TMP, name + '.o')
    ver, flags = db.game_flags()
    cmd = (db.mwcc_cmd(ver) + ['-c'] + flags + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', DR.replace(os.sep, '/')] +
           sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o.replace(os.sep, '/'), src.replace(os.sep, '/')])
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors='replace')
    if r.returncode or not os.path.exists(o):
        return None
    s = o + '.s'
    subprocess.run([corpus.DTK, 'elf', 'disasm', o, s], capture_output=True)
    return corpus.listing(s).get(name)


def label(name):
    ours = compile_draft(name)
    orig = corpus.listing(os.path.join(DR, 'asm', name + '.s')).get(name)
    if not ours or not orig:
        return name, 'not compiled', 0, 0
    ours = [norm(t) if not t.startswith('LBL:') else 'LBL:' for t in ours]
    orig = [norm(t) if not t.startswith('LBL:') else 'LBL:' for t in orig]
    words = sum(1 for t in orig if t != 'LBL:')
    equal = sum(1 for a, b in zip([t for t in ours if t != 'LBL:'], [t for t in orig if t != 'LBL:']) if a == b)
    if ours == orig:
        return name, 'match', equal, words
    ob, tb = rp.body_blocks(ours), rp.body_blocks(orig)
    frame = [sorted(f) for _, f, _ in ob] != [sorted(f) for _, f, _ in tb] or len(ob) != len(tb)
    if len(ob) != len(tb):
        return name, 'structural+frame' if frame else 'structural', equal, words
    mb = [[rp.mask(t) for t in b] for b, _, _ in ob]
    tm = [[rp.mask(t) for t in b] for b, _, _ in tb]
    if [b for b, _, _ in ob] == [b for b, _, _ in tb]:
        kind = 'frame'
    elif mb == tm:
        kind = 'register-only'
    elif all(sorted(a) == sorted(b) for a, b in zip(mb, tm)):
        kind = 'schedule-only'
    else:
        kind = 'structural'
    if frame and kind != 'frame':
        kind += '+frame'
    return name, kind, equal, words


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--only', nargs='*')
    a = ap.parse_args()
    os.makedirs(TMP, exist_ok=True)
    todo = near_misses()
    pct = dict(todo)
    names = [n for n, _ in todo if not a.only or n in a.only]
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        res = list(ex.map(label, names))
    cnt = collections.Counter(k.split('+')[0] for _, k, _, _ in res)
    cntf = collections.Counter(k for _, k, _, _ in res)
    lines = ['Near-miss drafts by the kind of difference left (tools/sched/label_drafts.py; drafts at 80-99% in '
             'decomp/drafts/draft_report.txt, compiled again and compared block by block with registers masked)', '']
    lines += ['%-44s %4s %-22s %s' % ('function', 'rep', 'label', 'words equal / original')]
    for n, k, e, w in sorted(res, key=lambda x: (x[1], x[0])):
        lines.append('%-44s %3d%% %-22s %d / %d' % (n, pct.get(n, 0), k, e, w))
    lines += ['', 'Counts (%d functions):' % len(res)]
    for k, v in sorted(cnt.items(), key=lambda x: -x[1]):
        lines.append('  %-16s %d' % (k, v))
    lines += ['', 'With the frame difference split out:']
    for k, v in sorted(cntf.items(), key=lambda x: -x[1]):
        lines.append('  %-22s %d' % (k, v))
    if not a.only:
        with open(OUT, 'w', encoding='utf-8', newline='\r\n') as fh:
            fh.write('\n'.join(lines) + '\n')
    print('\n'.join(lines[-(len(cnt) + len(cntf) + 5):]))


if __name__ == '__main__':
    main()
