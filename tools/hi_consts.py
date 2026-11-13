"""hi_consts.py: m2c's bare address halves (`sp70 = 0x80400000;` ... `sp70 + 0x26FC`) back to symbols (Nov 13, 2026).

  python -I tools/hi_consts.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: in a big function MWCC treats the high half of a global's address (`lis rX, sym@ha`) as a common
subexpression: one `lis` serves many uses, and when registers run out it is spilled (`lis r0,0x8040; stw r0,0x3c(r1)`
... `lwz r3,0x3c(r1); addi r6,r3,0x7260`). The disassembly cannot tie the spilled half to its symbol (the matching
`@l` is far away, after a reload), so the asm has a bare `lis r0, 0x8040` and m2c a bare constant
`sp3C = (T *)0x80400000;` with `sp3C + 0x7260` at the uses. In the draft that constant is an integer the compiler
keeps live in a register of its own (hoisted, never rematerialised): fn_80101E00's draft holds a dozen of them,
which is a good part of why the giant drafts are 1.5x the original with twice its frame. 101 originals have such a
bare `lis`, the 100 largest low drafts ~650 constants.

What it does: per variable V assigned a bare high half H (0x80000000..0x80500000, low 16 bits 0, optionally cast):
each use `V + L` / `(u8 *) V + L` / `(T) V + L` (first addend, a number) is the address A = H + L (L sign-extended,
as addi does); A is placed in the symbol table (decomp/config/*/symbols.txt) and the use becomes
`((u8 *) &sym + (A - sym))`, cast back to the type the use had (`(s32)` for an integer V). Assignments of V that are
then unused are dropped, and V's declaration when nothing is left. Scored with hd.py, kept when the equal words go up.
"""
import bisect
import glob
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HD = os.path.join(G, 'tools', 'sched', 'scratch', 'hand', 'hd.py')
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
KEEP = os.path.join(G, opts.get('keep', os.path.join('build', 'low100')))
FROM = [os.path.join(G, d) for d in opts.get('from', os.path.relpath(KEEP, G)).split(',') if d]
SYMS = []
for p in glob.glob(os.path.join(G, 'decomp', 'config', '*', 'symbols.txt')):
    for l in open(p, encoding='utf-8'):
        m = re.match(r'(\w+) = \.(\w+):0x([0-9A-Fa-f]+);.*type:object(?:.*size:0x([0-9A-Fa-f]+))?', l)
        if m and m.group(2) in ('data', 'bss', 'sdata', 'sbss', 'rodata', 'sdata2', 'sbss2'):
            SYMS.append((int(m.group(3), 16), int(m.group(4) or '0', 16), m.group(1)))
SYMS.sort()
ADDRS = [s[0] for s in SYMS]


def sym_at(a):
    k = bisect.bisect_right(ADDRS, a) - 1
    if k < 0:
        return None
    s, n, name = SYMS[k]
    if a < s + max(n, 1) or (a == s):
        return name, a - s
    return None


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='hic_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


NUM = r'(0x[0-9A-Fa-f]+|\d+)'
CAST = r'(?:\(\s*[\w ]+\**\s*\)\s*)?'


HI_ASG = re.compile(r'^\s*(\w+)\s*=\s*' + CAST + r'(0x80[0-5][0-9A-Fa-f]0000)\s*;[ \t]*$')


def rewrite(text):
    decl = {}
    for m in re.finditer(r'^    ([\w ]+?\**)\s*\b(\w+);', text, re.M):
        decl[m.group(2)] = m.group(1).strip()
    lines = text.split('\n')
    hi = set()
    held = {}                     # V -> H while V holds a bare high half (text order)
    n = 0
    for j, l in enumerate(lines):
        m = HI_ASG.match(l)
        if m:
            held[m.group(1)] = int(m.group(2), 16)
            hi.add(m.group(1))
            continue
        if not held:
            continue

        def fix(m):
            nonlocal n
            cast, v, L = (m.group(1) or '').strip(), m.group(2), int(m.group(3), 0)
            if v not in held or L >= 0x8000 or L < 0x10:
                return m.group(0)
            s = sym_at(held[v] + L)
            if s is None:
                return m.group(0)
            name, off = s
            n += 1
            addr = f'((u8 *) &{name} + 0x{off:X})' if off else f'((u8 *) &{name})'
            if cast:
                return addr if cast.replace(' ', '') == '(u8*)' else f'({cast} {addr})'
            return f'(({decl.get(v, "s32")}) {addr})'
        names = '|'.join(re.escape(v) for v in held)
        lines[j] = re.sub(r'(\(\s*[\w ]+\**\s*\)\s*)?\b(' + names + r')\s*\+\s*' + NUM + r'\b', fix, l)
        # a new value of V ends its web (after the uses on this line)
        for v in list(held):
            if re.match(r'^\s*' + v + r'\s*(=[^=]|\+=|-=)', l) or re.search(r'&\s*' + v + r'\b', l):
                held.pop(v)
    # the constants' assignments (and declarations) nothing reads any more
    for v in hi:
        w = re.compile(r'\b' + v + r'\b')
        dl = re.compile(r'^    [\w ]+\**\s*\b' + v + r';')
        if not any(w.search(l) and not HI_ASG.match(l) and not dl.match(l) for l in lines):
            lines = [l for l in lines if not (w.search(l) and (HI_ASG.match(l) or dl.match(l)))]
    return '\n'.join(lines), n


def src_of(func):
    for d in FROM + [os.path.join(G, 'tools', 'hand_drafts'), os.path.join(OUT, 'c')]:
        p = os.path.join(d, func + '.c')
        if os.path.exists(p):
            return p
    return None


def one(func):
    src = src_of(func)
    if src is None:
        return func, None
    text = open(src, encoding='utf-8').read().replace('\r\n', '\n')
    new, n = rewrite(text)
    base = score(func, text, 'base')
    if not n:
        return func, (base, base, 0, text)
    sc = score(func, new, 'hi')
    return func, (base, sc, n, new)


funcs = pos
if 'list' in opts:
    funcs += [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
with ThreadPoolExecutor(JOBS) as ex:
    res = list(ex.map(one, funcs))
os.makedirs(KEEP, exist_ok=True)
gain = 0
for func, r in res:
    if r is None:
        print(f'{func}: no draft')
        continue
    base, best, n, text = r
    note = ''
    if best[1] > base[1]:
        gain += best[1] - base[1]
        open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
        note = ' kept'
    if n:
        print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}), {n} uses{note}', flush=True)
print(f'hi_consts: +{gain} equal words')
