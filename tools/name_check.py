"""The name batches (1, 6, 7) in check mode, and the guard of their apply mode (tools/batches/run_all.ps1 / .sh).

The committed name files are the truth. run_all.ps1 runs the name batches against the fresh Ghidra export, reports
what they would change, and puts the committed files back: names change only on purpose (run_all -ApplyNames), never
as a side effect of a verification run.

usage: python -I tools/name_check.py snapshot DIR        copy the name files to DIR
       python -I tools/name_check.py report DIR OUT      what the batches changed against DIR -> OUT, then restore DIR
       python -I tools/name_check.py guard DIR           apply mode: fail when a name the C sources use was dropped
                                                         or moved (compared with the copy in DIR)
"""
import difflib
import os
import re
import shutil
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SYM = os.path.join(G, 'symbols')
# everything batches 1 + 6 + 7 write (tools/batches/batch1_names.ps1, batch6_lib_candidates.ps1, batch7_game_names.ps1)
NAME_FILES = ['symbols.txt', 'layout_candidates.txt', 'candidates_sdk.txt', 'candidates_rt.txt', 'candidates_hsd.txt',
              'verify_report.txt', 'lib_candidates_report.txt', 'lib_unnamed_hsd.txt', 'lib_unnamed_sdk.txt',
              'lib_unnamed_rt.txt', 'descriptive.txt', 'game_real.txt', 'game_unnamed_real.txt']
# the files that define names (the others are reports)
DEFINING = ['symbols.txt', 'candidates_sdk.txt', 'candidates_rt.txt', 'candidates_hsd.txt', 'descriptive.txt']
DEF = re.compile(r'^(\S+) = \.[\w.]+:0x([0-9A-Fa-f]{8});')


def names(folder):
    out = {}
    for f in DEFINING:
        p = os.path.join(folder, f)
        if os.path.exists(p):
            for line in open(p, encoding='utf-8', errors='replace'):
                m = DEF.match(line)
                if m:
                    out.setdefault(m.group(1), m.group(2).upper())
    return out


def c_words():
    words = set()
    for root, _, files in os.walk(os.path.join(G, 'decomp', 'src')):
        for f in files:
            if f.endswith(('.c', '.h')):
                words.update(re.findall(r'\b[A-Za-z_]\w*\b', open(os.path.join(root, f), encoding='utf-8', errors='replace').read()))
    return words


def snapshot(d):
    os.makedirs(d, exist_ok=True)
    for f in NAME_FILES:
        p = os.path.join(SYM, f)
        q = os.path.join(d, f)
        if os.path.exists(p):
            shutil.copyfile(p, q)
        elif os.path.exists(q):
            os.remove(q)


def restore(d):
    for f in NAME_FILES:
        q = os.path.join(d, f)
        p = os.path.join(SYM, f)
        if os.path.exists(q):
            shutil.copyfile(q, p)
        elif os.path.exists(p):
            os.remove(p)


def report(d, out):
    old, new = names(d), names(SYM)
    added = sorted(n for n in new if n not in old)
    dropped = sorted(n for n in old if n not in new)
    moved = sorted(n for n in new if n in old and new[n] != old[n])
    used = c_words()
    lines = ['# name proposals: what the name batches (1, 6, 7) would change against the committed names',
             '# (tools/name_check.py; nothing applied - run tools/batches/run_all.ps1 -ApplyNames to take them)',
             'added %d, dropped %d (%d used by the C sources), moved %d' % (
                 len(added), len(dropped), sum(1 for n in dropped if n in used), len(moved)), '']
    lines += ['added   %s 0x%s' % (n, new[n]) for n in added]
    lines += ['dropped %s 0x%s%s' % (n, old[n], '  (used by decomp/src)' if n in used else '') for n in dropped]
    lines += ['moved   %s 0x%s -> 0x%s' % (n, old[n], new[n]) for n in moved]
    for f in DEFINING:
        a = os.path.join(d, f)
        b = os.path.join(SYM, f)
        if not (os.path.exists(a) and os.path.exists(b)):
            continue
        x = open(a, encoding='utf-8', errors='replace').read().splitlines()
        y = open(b, encoding='utf-8', errors='replace').read().splitlines()
        diff = list(difflib.unified_diff(x, y, 'committed/' + f, 'batches/' + f, n=0, lineterm=''))
        if diff:
            lines += ['', '## %s: %d lines differ' % (f, sum(1 for l in diff if l[:1] in '+-') - 2)] + diff
    with open(out, 'w', encoding='utf-8', newline='\r\n') as fh:
        fh.write('\n'.join(lines) + '\n')
    restore(d)
    print('names: committed names kept; the batches would add %d, drop %d, move %d (%s)' % (
        len(added), len(dropped), len(moved), os.path.relpath(out, G)))


def guard(d):
    old, new = names(d), names(SYM)
    used = c_words()
    bad = [n for n in old if n in used and new.get(n) != old[n]]
    if bad:
        print('names: the name batches dropped or moved %d name(s) the C sources use: %s' % (
            len(bad), ', '.join(sorted(bad)[:20])))
        sys.exit(1)
    print('names: every name the C sources use is kept')


def main(a):
    if len(a) == 3 and a[1] == 'snapshot':
        snapshot(a[2])
    elif len(a) == 4 and a[1] == 'report':
        report(a[2], a[3])
    elif len(a) == 3 and a[1] == 'guard':
        guard(a[2])
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main(sys.argv)
