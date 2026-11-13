"""recip_sweep.py: `x * 0.0625f` in a draft tried as `x / 16.0f` (Nov 12, 2026).

  python -I tools/recip_sweep.py [--save] [--lo 40]

MWCC turns a float division by a power of two into a multiply by the reciprocal, with the operands in the order
(x, c); a multiply written with the constant loads the constant first and gives other scratch registers and the
operands reversed. m2c only sees the multiply. fn_80130B48 (96 -> 100%): `(f32) (0x10 - n) / 16.0f`.
For every open draft (the order_sweep.py result in build/order_sweep/ when there is one, else decomp/drafts/c) with
such a multiply: each one alone and all together rewritten as the division, scored with hd.py; a 100% with equal
relocations is saved to tools/hand_drafts with --save, an improvement to build/order_sweep/.
"""
import os
import re
import subprocess
import sys
import tempfile

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HD = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'hd.py')
REL = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'relcheck.py')
OUT = os.path.join(G, 'decomp', 'drafts')
SAVE = '--save' in sys.argv
LO = int(sys.argv[sys.argv.index('--lo') + 1]) if '--lo' in sys.argv else 40
REC = {}
for k in range(1, 12):
    v = 1.0 / (1 << k)
    REC[repr(v)] = f'{float(1 << k)}'
PAT = re.compile(r'\* (' + '|'.join(re.escape(k) for k in REC) + r')(f?)\b')
# constants named by their data label (most drafts): the label's float value read from main.dol
sys.path.insert(0, os.path.join(G, 'tools'))
import struct  # noqa: E402
import crmotion  # noqa: E402
DOL = crmotion.load_dol(None)
LBL = re.compile(r'\* (lbl_(80[0-9A-F]{6}))\b(?!\[)')


def label_recip(addr):
    try:
        v = struct.unpack('>f', DOL.read(int(addr, 16), 4))[0]
    except Exception:
        return None
    return REC.get(repr(float(v)))


hand = {f[:-2] for f in os.listdir(os.path.join(G, 'tools', 'hand_drafts')) if f.endswith('.c')}
tmp = tempfile.mkdtemp(prefix='recip_')


def score(func, text, tag):
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return ((int(m.group(1)), int(m.group(2))) if m else (-1, -1)), f


wins = 0
for l in open(os.path.join(OUT, 'draft_report.txt'), encoding='utf-8'):
    m = re.match(r'([0-9A-F]{8})\s+(\S+)\s+(\d+)\s+(\d+)%', l)
    if not m or not (LO <= int(m.group(4)) < 100) or m.group(2) in hand:
        continue
    func = m.group(2)
    src = os.path.join(G, 'build', 'order_sweep', func + '.c')
    if not os.path.exists(src):
        src = os.path.join(OUT, 'c', func + '.c')
    text = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    hits = [(h.start(), h.end(), '/ ' + REC[h.group(1)] + h.group(2)) for h in PAT.finditer(text)]
    for h in LBL.finditer(text):
        r_ = label_recip(h.group(2))
        if r_:
            hits.append((h.start(), h.end(), '/ ' + r_ + 'f'))
    if not hits:
        continue
    base, _ = score(func, text, 'base')
    best, btext = base, text
    cands = [[h] for h in hits] + ([hits] if len(hits) > 1 else [])
    for ci, hs in enumerate(cands):
        t = text
        for h in sorted(hs, key=lambda h: -h[0]):
            t = t[:h[0]] + h[2] + t[h[1]:]
        sc, f = score(func, t, str(ci))
        if sc > best:
            best, btext = sc, t
    note = ''
    if best > base:
        if best[0] == 100:
            f = os.path.join(tmp, func + '_win.c')
            open(f, 'w', encoding='utf-8', newline='\n').write(btext)
            ok = 'relocations equal' in subprocess.run([sys.executable, '-I', REL, OUT, func, f], capture_output=True, text=True, cwd=G).stdout
            note = ' MATCH' if ok else ' 100% words, relocations differ'
            if ok:
                wins += 1
                if SAVE:
                    body = btext[btext.index('\n', btext.index('#undef')) + 1:].lstrip('\n') if '#undef' in btext else btext
                    hdr = (f'/* {func}: Nov 12, 2026 hand draft (m2c {m.group(4)}%): a multiply by a power-of-two reciprocal '
                           f'written as the division (tools/recip_sweep.py) */\n\n')
                    open(os.path.join(G, 'tools', 'hand_drafts', func + '.c'), 'w', encoding='utf-8', newline='\n').write(hdr + body)
                    note += ', saved'
        else:
            os.makedirs(os.path.join(G, 'build', 'order_sweep'), exist_ok=True)
            open(os.path.join(G, 'build', 'order_sweep', func + '.c'), 'w', encoding='utf-8', newline='\n').write(btext)
    print(f'{func}: {len(hits)} multiplies, {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}){note}', flush=True)
print(f'recip_sweep: {wins} new match(es)')
