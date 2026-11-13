"""struct_copies.py: m2c's M2C_STRUCT_COPY loops (with their one-unit tails) as struct assignments (Nov 13, 2026).

  python -I tools/struct_copies.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: MWCC copies a struct assignment of N bytes with a counted loop over pairs of units (`mtctr; lwz; lwzu; stw;
stwu; bdnz`, both pointers biased by one unit) and the odd unit after it. m2c reads that as
    tD = DEST - U;  tS = SRC - U;                        (or inline: `(s32) (P - U) + U`)
    M2C_STRUCT_COPY((s32) tD + U, (s32) tS + U, N);
    (*(T *)((s8 *)((tD + N)) + (U))) = (T) (*(T *)((s8 *)((tS + N)) + (U)));     (the tail, when N / U is odd)
and M2C_STRUCT_COPY is memcpy in m2c's macros: the draft calls memcpy where the original has the loop, and the
call takes scratch registers and saved ones around it. draft_batch.py's struct_copy_macro (step 109a) turns the
plain macro into a struct assignment but not the tail form; in the 100 largest low drafts 18 copies are left.

What it does: each macro (and the tail line right after it, when it reads tD + N / tS + N) becomes
`*(cr_CopyM_U *) (D) = *(cr_CopyM_U *) (S);` with M = N (+ U with the tail), D / S the unbiased addresses (tD + U),
the struct type (u32 / u16 / u8 array of M / U units) declared before the function. Scored with hd.py; kept when the
equal words go up.
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
NUM = re.compile(r'0x[0-9A-Fa-f]+|\d+')


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='scp_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def split_args(s):
    out, depth, cur = [], 0, ''
    for ch in s:
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += ch
    out.append(cur.strip())
    return out


def unbias(a, u):
    """`(s32) X + U` -> the address X + U as an expression; `(s32) (P - U) + U` -> P"""
    m = re.fullmatch(r'\(s32\)\s*(.*?)\s*\+\s*(0x[0-9A-Fa-f]+|\d+)', a.strip())
    if not m or int(m.group(2), 0) != u:
        return None
    x = m.group(1).strip()
    b = re.fullmatch(r'\((.*) - (0x[0-9A-Fa-f]+|\d+)\)', x)
    if b and int(b.group(2), 0) == u and not re.match(r'^\(+\s*(u32|s32|f32|u16|s16) \*', b.group(1)):
        return b.group(1)
    return f'((u8 *) ({x}) + {u})'


def rewrite(text):
    lines = text.split('\n')
    types = {}
    n = 0
    j = 0
    while j < len(lines):
        m = re.match(r'^(\s*)M2C_STRUCT_COPY\((.*)\);\s*$', lines[j])
        if not m:
            j += 1
            continue
        args = split_args(m.group(2))
        if len(args) != 3 or not NUM.fullmatch(args[2]):
            j += 1
            continue
        N = int(args[2], 0)
        u = None
        for cand in (4, 2, 1):
            if re.search(r'\+\s*%d\s*$' % cand, args[0]) and re.search(r'\+\s*%d\s*$' % cand, args[1]):
                u = cand
                break
        if u is None:
            j += 1
            continue
        D, S = unbias(args[0], u), unbias(args[1], u)
        if D is None or S is None:
            j += 1
            continue
        M = N
        drop = 0
        if j + 1 < len(lines):
            dm = re.match(r'^\s*\(s32\)\s*(\w+)', args[0]), re.match(r'^\s*\(s32\)\s*(\w+)', args[1])
            if dm[0] and dm[1]:
                tD, tS = dm[0].group(1), dm[1].group(1)
                nxt = lines[j + 1]
                if re.search(r'\b' + tD + r'\s*\+\s*0x%X\b' % N, nxt, re.I) and re.search(r'\b' + tS + r'\s*\+\s*0x%X\b' % N, nxt, re.I):
                    M += u
                    drop = 1
        t = 'cr_Copy%X_%d' % (M, u)
        types[t] = (M, u)
        lines[j:j + 1 + drop] = [f'{m.group(1)}*({t} *) ({D}) = *({t} *) ({S});']
        n += 1
        j += 1
    if not n:
        return text, 0
    text = '\n'.join(lines)
    k = re.search(r'^\w[^;\n]*\)\s*\{\s*$', text, re.M)
    decl = ''.join('typedef struct %s {\n    %s w[0x%X];\n} %s;\n\n' % (t, {4: 'u32', 2: 'u16', 1: 'u8'}[u], M // u, t)
                   for t, (M, u) in sorted(types.items()) if ('struct %s {' % t) not in text)
    return text[:k.start()] + decl + text[k.start():], n


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
    if not n:
        return func, None
    base = score(func, text, 'base')
    return func, (base, score(func, new, 'copy'), n, new)


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
        base, best, n, text = r
        note = ''
        if best[1] > base[1]:
            gain += best[1] - base[1]
            open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
            note = ' kept'
        print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}), {n} copies{note}', flush=True)
    print(f'struct_copies: +{gain} equal words')
