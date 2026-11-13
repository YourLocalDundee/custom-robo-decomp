"""triage.py: why does an open draft score low? (Nov 13, 2026)

  python -I tools/triage.py --low 40 --top 100 [--out decomp/drafts] [--jobs 8] [--list build/triage/low100.txt]
  python -I tools/triage.py FUNC [FUNC ...]

The draft score (draft_batch.compare) aligns whole words, so a draft whose only fault is its register numbering or
its stack frame scores near 0% however close its code is. Per function this compiles the draft (as hd.py does) and
reports next to that score:
  size      words of the draft / words of the original
  shape     the instruction kinds aligned (opcode + extended opcode only: registers, offsets and immediates dropped)
  regs      the words equal once only the register fields are dropped (offsets and immediates kept)
  frame     the stwu r1 frame size and the first saved GPR (stmw / stw r14..r31) of each side, '=' when equal
High shape with low score = registers or frame; low shape = the control flow or the statements are wrong.
--from DIR[,DIR]: take FUNC.c from these folders first (work in progress), then tools/hand_drafts, then the drafts.
Writes build/triage/<--name>.tsv sorted by size.
"""
import difflib
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import draft_batch as db  # noqa: E402

av = sys.argv[1:]
opts, pos, i = {}, [], 0
while i < len(av):
    if av[i].startswith('--'):
        opts[av[i][2:]] = av[i + 1]
        i += 2
    else:
        pos.append(av[i])
        i += 1
OUT = os.path.join(G, opts.get('out', os.path.join('decomp', 'drafts')))
JOBS = int(opts.get('jobs', 8))
ver, flags = db.game_flags()
db.init(OUT, ver, flags)


def report_rows():
    rows = {}
    for l in open(os.path.join(OUT, 'draft_report.txt'), encoding='utf-8'):
        m = re.match(r'([0-9A-F]{8})\s+(\S+)\s+(\d+)\s+(\d+)%', l)
        if m:
            rows[m.group(2)] = (int(m.group(3)), int(m.group(4)))
    return rows


def originals():
    found = {}
    for f in sorted(os.listdir(os.path.join(OUT, 'orig'))):
        e = db.orig_elf(os.path.join(OUT, 'orig', f))
        for n in e.syms:
            found.setdefault(n, e)
    return found


def compile_draft(name, src):
    body = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    pre = ''
    if body.startswith(db.NO_RO):
        body, pre = body[len(db.NO_RO):], '#define CR_NO_RO\n'
    if '#include "ctx.h"' in body:
        text = body
    else:
        text = ('// triage of %s\n%s#define %s %s__proto\n#include "ctx.h"\n#undef %s\n\n%s'
                % (name, pre, name, name, name, body))
    tmp = tempfile.mkdtemp(prefix='tri_')
    c, o = os.path.join(tmp, name + '.c'), os.path.join(tmp, name + '.o')
    open(c, 'w', encoding='utf-8', newline='\n').write(text)
    cmd = db.mwcc_cmd(ver) + ['-c'] + flags + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', OUT.replace(os.sep, '/')] + \
        sum([['-i', x] for x in db.CTX_INC], []) + ['-o', o, c.replace('\\', '/')]
    rc, so, se = db.run(cmd, 300, cwd=db.D)
    if rc or not os.path.exists(o):
        return None
    return db.Elf(open(o, 'rb').read()).func(name)


def kind(w):
    op = w >> 26
    if op in (19, 31, 59, 63):
        return (op << 16) | ((w >> 1) & 0x3FF)
    if op == 30:
        return (op << 16) | ((w >> 1) & 0xF)
    return op << 16


def noregs(w):
    """drop the register fields (rD/rS, rA, rB) where the form has them; keep the rest"""
    op = w >> 26
    if op in (31, 63, 59, 19, 4):
        return w & ~0x03FFF800
    if op in (16, 18):
        return w
    return w & ~0x03FF0000


def frame(ws):
    fs, sv = None, None
    for w in ws[:12]:
        if w >> 16 == 0x9421:               # stwu r1,-N(r1)
            fs = 0x10000 - (w & 0xFFFF)
        if w >> 26 == 47 and (w >> 16) & 31 == 1:     # stmw rS,d(r1)
            sv = (w >> 21) & 31
    if sv is None:
        regs = [(w >> 21) & 31 for w in ws[:40] if w >> 26 == 36 and (w >> 16) & 31 == 1 and (w >> 21) & 31 >= 14]
        sv = min(regs) if regs else 32
    return fs, sv


OD = os.path.join(db.D, 'build', 'binutils', 'powerpc-eabi-objdump.exe')


def mnems(ws):
    """per instruction: mnemonic, with 'r1' added for loads / stores off the stack pointer (spills, locals)"""
    import struct
    pth = os.path.join(tempfile.mkdtemp(prefix='trm_'), 'x.bin')
    open(pth, 'wb').write(struct.pack('>%dI' % len(ws), *ws))
    r = subprocess.run([OD, '-D', '-b', 'binary', '-m', 'powerpc', '-EB', pth], capture_output=True, text=True).stdout
    out = []
    for ln in r.splitlines():
        if re.match(r'\s+[0-9a-f]+:	', ln):
            t = ln.split('	', 2)[-1].strip().split(None, 1)
            mn = t[0] if t else '?'
            if len(t) > 1 and re.search(r'\(r1\)$', t[1]):
                mn += '@r1'
            out.append(mn)
    return out


def ratio(a, b):
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return sum(bl.size for bl in sm.get_matching_blocks()) * 100 // max(len(a), len(b), 1)


FROM = [os.path.join(G, d) for d in opts.get('from', '').split(',') if d]


def triage(name, orig_elf, rows):
    src = next((os.path.join(d, name + '.c') for d in FROM if os.path.exists(os.path.join(d, name + '.c'))),
               os.path.join(G, 'tools', 'hand_drafts', name + '.c'))
    if not os.path.exists(src):
        src = os.path.join(OUT, 'c', name + '.c')
    o = orig_elf.func(name)
    m = compile_draft(name, src)
    if m is None:
        return name, rows.get(name, (0, 0))[0], None
    p, _ = db.compare(o, m)
    wo, wm = o[0], m[0]
    shape = ratio([kind(w) for w in wo], [kind(w) for w in wm])
    regs = ratio([noregs(w) for w in wo], [noregs(w) for w in wm])
    fo, fm = frame(wo), frame(wm)
    import collections
    co, cm = collections.Counter(mnems(wo)), collections.Counter(mnems(wm))
    return name, len(wo) * 4, (p, len(wm) / len(wo), shape, regs, fo, fm, co, cm)


rows = report_rows()
if pos:
    funcs = pos
else:
    lo = int(opts.get('low', 40))
    top = int(opts.get('top', 100))
    hand = {f[:-2] for f in os.listdir(os.path.join(G, 'tools', 'hand_drafts')) if f.endswith('.c')}
    cand = [(b, n) for n, (b, p) in rows.items() if p < lo and n not in hand]
    funcs = [n for b, n in sorted(cand, reverse=True)[:top]]
    lst = os.path.join(G, opts.get('list', os.path.join('build', 'triage', 'low%d.txt' % top)))
    os.makedirs(os.path.dirname(lst), exist_ok=True)
    open(lst, 'w', encoding='utf-8', newline='\n').write('\n'.join(funcs) + '\n')
ORIG = originals()
with ThreadPoolExecutor(JOBS) as ex:
    res = list(ex.map(lambda n: triage(n, ORIG[n], rows), funcs))
os.makedirs(os.path.join(G, 'build', 'triage'), exist_ok=True)
tsv = os.path.join(G, 'build', 'triage', opts.get('name', 'triage') + '.tsv')
import collections
TOT = collections.Counter()
lines = ['function\tbytes\tscore\tsize\tshape\tregs\tframe_orig\tframe_draft']
print(f'{"function":<34}{"bytes":>7}{"score":>7}{"size":>7}{"shape":>7}{"regs":>6}  frame orig / draft')
for name, b, r in sorted(res, key=lambda x: -(x[1] or 0)):
    if r is None:
        print(f'{name:<34}{b:>7}  did not compile')
        continue
    p, sz, sh, rg, fo, fm, co, cm = r
    fr = '=' if fo == fm else f'{fo[0]}/r{fo[1]}  {fm[0]}/r{fm[1]}'
    ex = sorted(set(co) | set(cm), key=lambda k: -abs(cm[k] - co[k]))[:5]
    exs = ' '.join(f'{k}{cm[k] - co[k]:+d}' for k in ex if cm[k] != co[k])
    for k in set(co) | set(cm):
        TOT[k] += cm[k] - co[k]
    print(f'{name:<34}{b:>7}{p:>6}%{sz:>7.2f}{sh:>6}%{rg:>5}%  {fr:<22} {exs}')
    lines.append(f'{name}\t{b}\t{p}\t{sz:.3f}\t{sh}\t{rg}\t{fo[0]}/r{fo[1]}\t{fm[0]}/r{fm[1]}')
open(tsv, 'w', encoding='utf-8', newline='\n').write('\n'.join(lines) + '\n')
print('excess instructions over all (draft - original):',
      ' '.join(f'{k}{v:+d}' for k, v in sorted(TOT.items(), key=lambda kv: -abs(kv[1]))[:30]))
print('triage:', tsv)
