"""unk_stack.py: m2c's `unkspX` stack areas as arrays, with byte offsets that really are bytes (Nov 13, 2026).

  python -I tools/unk_stack.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: when m2c cannot type a stack area it declares a scalar (`f32 unksp930;`) and indexes it with byte offsets,
`*(&unksp930 + ((i * 8) & 0x7FFF8))`: pointer arithmetic on an f32 * scales the offset by 4 again, so the draft
reads and writes the wrong places, and its frame lacks the area (fn_800E4AC4: 0x630 + 0x300 + 0x600 bytes of
arrays, a 4000-byte frame against the draft's 256). Four drafts have it: fn_8002B3C4, fn_8002CC54, fn_800E7310,
fn_800E4AC4 (all among the 100 largest low ones). fn_800E4AC4 by hand (three 192-element arrays of {id, count},
f32, {id, value}): 7 -> 26%.

What it does, keeping m2c's expressions: each unkspX whose address is taken becomes an array `T unkspX[N]` that runs
to the next unkspY (or to the original's register save area), `*(&unkspX + E)` becomes `*(T *) ((u8 *) unkspX +
E)`, `&unkspX` becomes `unkspX` and a plain `unkspX` use `unkspX[0]`. The declarations are tried in three orders
(m2c's, by offset up, by offset down: MWCC lays stack objects out in declaration order); kept when the equal words
go up (hd.py). The arrays' real element types are hand work (fn_800E4AC4's structs).
"""
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
SIZE = {'u8': 1, 's8': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'f32': 4, 'f64': 8}
DECL = re.compile(r'^    ([\w ]+?\**)\s*\b(unksp([0-9A-F]+));\s*$', re.M)


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='uks_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def save_start(func):
    lo = None
    for l in open(os.path.join(OUT, 'asm', func + '.s'), encoding='utf-8').read().split('\n')[:40]:
        m = re.search(r'\t(stmw|stfd|psq_st) (r\d+|f\d+), (0x[0-9a-f]+|\d+)\(r1\)', l)
        if m:
            off = int(m.group(3), 0)
            lo = off if lo is None else min(lo, off)
    return lo


def rewrite(text, func, order):
    decls = [(m.group(1).strip(), m.group(2), int(m.group(3), 16)) for m in DECL.finditer(text)]
    if not decls:
        return text, 0
    offs = sorted(o for _, _, o in decls)
    top = save_start(func)
    n = 0
    newdecl = {}
    for ty, name, off in decls:
        if not re.search(r'&\s*' + name + r'\b', text):
            newdecl[name] = f'    {ty} {name};'
            continue
        sz = 4 if ty.endswith('*') else SIZE.get(ty, 4)
        nxt = next((o for o in offs if o > off), top)
        if nxt is None or nxt - off < 2 * sz:
            newdecl[name] = f'    {ty} {name};'
            continue
        cnt = (nxt - off) // sz
        t = ty
        ph = '@@' + name + '@@'
        text = re.sub(r'\*\(&' + name + r' \+ ', f'*({t} *) ((u8 *) {ph} + ', text)
        text = re.sub(r'&\s*' + name + r'\b', ph, text)
        # what is left is a plain use of the area's first element (the declaration line is rebuilt below)
        text = re.sub(r'(?<![\w@])' + name + r'\b(?!\s*\[)(?!@@)(?!;\s*$)', name + '[0]', text)
        text = text.replace(ph, name)
        newdecl[name] = f'    {ty} {name}[{cnt}];'
        n += 1
    if not n:
        return text, 0
    lines = text.split('\n')
    idx = [k for k, l in enumerate(lines) if DECL.match(l) or re.match(r'^    [\w ]+?\**\s*\bunksp[0-9A-F]+\[\d+\];', l)
           or re.match(r'^    [\w ]+?\**\s*\bunksp[0-9A-F]+(\[0\])?;\s*$', l)]
    if not idx:
        return text, 0
    names = [d[1] for d in decls]
    if order == 'up':
        names = sorted(names, key=lambda x: int(x[5:], 16))
    elif order == 'down':
        names = sorted(names, key=lambda x: -int(x[5:], 16))
    first = idx[0]
    lines = [l for k, l in enumerate(lines) if k not in idx]
    lines[first:first] = [newdecl[x] for x in names]
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
    if not re.search(r'\*\(&unksp[0-9A-F]+ \+ ', text):
        return func, None
    base = score(func, text, 'base')
    best, btxt, how = base, None, ''
    for order in ('m2c', 'up', 'down'):
        new, n = rewrite(text, func, order)
        if not n:
            continue
        sc = score(func, new, order)
        print(f'  {func} {order}: {sc[0]}% ({sc[1]})', flush=True)
        if sc[1] > best[1]:
            best, btxt, how = sc, new, f'{n} areas, {order}'
    return func, (base, best, how, btxt)


if __name__ == '__main__':
    funcs = pos
    if 'list' in opts:
        funcs += [l.strip() for l in open(os.path.join(G, opts['list']), encoding='utf-8') if l.strip()]
    with ThreadPoolExecutor(JOBS) as ex:
        res = list(ex.map(one, funcs))
    os.makedirs(KEEP, exist_ok=True)
    gain = 0
    for func, r in res:
        if r is None:
            continue
        base, best, how, text = r
        note = ''
        if text is not None:
            gain += best[1] - base[1]
            open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
            note = ' kept'
        print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}) {how}{note}', flush=True)
    print(f'unk_stack: +{gain} equal words')
