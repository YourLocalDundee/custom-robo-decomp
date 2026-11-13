"""index_loads.py: m2c's byte arithmetic on an array of records -> the record field, for MWCC's indexed loads (Nov 13).

  python -I tools/index_loads.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: the original reads a field of an array element as `rlwinm r3, i, 5, ...; addi r3, r3, 0x10; lhzx r3, base, r3`
(the index and the field offset added first, then an indexed load); m2c writes the address as bytes,
`M2C_FIELD((base + ((i << 5) + 0x10)), u16 *, 0)`, which MWCC reassociates into `add r, base, i * 32; lhz r, 0x10(r)`.
The source had `p[i].field` (fn_8007B054 by hand: `((cr_Elem20 *) base)[i].f10` gives the original's lhzx, 35 ->
38%). tools/triage.py shows the pair (indexed loads short, add + plain loads over) in ~20 of the 100 largest low
drafts (fn_80109314 lhzx -50 / add +50, fn_8000C104 lbzx -33, fn_80082F84 / fn_80083C78 lwzx -27,
cr_SEQ_SCENARIO_FrameC lfsx -56, fn_800E29B8 lfsx -20, ...).

What it does: `M2C_FIELD((B + (X + K)), T *, 0)`, `M2C_FIELD((B + X + K), T *, 0)` and `M2C_FIELD((B + X), T *, K)`
with X an element offset (`(v * S)`, `(v << k)`, `((v << k) & M)`, the mask being v's zero extension) become
`((cr_El_S_K_T *) (B))[v].v`, the element struct (S bytes, a T at K) declared before the function. Each of the
three forms is tried on its own and all together (hd.py); kept when the equal words go up.
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
SIZE = {'u8': 1, 's8': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'f32': 4}
NUM = r'(0x[0-9A-Fa-f]+|\d+)'
V = r'(\(\w+\)\s*)?([A-Za-z_]\w*)'
IDX = [  # (regex for X, -> (var, stride))
    (r'\(' + V + r' \* ' + NUM + r'\)', lambda m: (m.group(2), int(m.group(3), 0))),
    (r'\(' + V + r' << (\d+)\)', lambda m: (m.group(2), 1 << int(m.group(3)))),
    (r'\(\(' + V + r' << (\d+)\) & ' + NUM + r'\)', lambda m: (m.group(2), 1 << int(m.group(3)))),
    (r'\(\(' + V + r' \* ' + NUM + r'\) & ' + NUM + r'\)', lambda m: (m.group(2), int(m.group(3), 0))),
]
# form d, an array of T inside a record: `M2C_FIELD((B + K), T *, X)` with X = v * sizeof(T) -> `((T *) (B + K))[v]`
# (fn_80109314: `M2C_FIELD((lbl_80238008 + 0x3732), u16 *, (var_r28 * 2) & 0x1FFFE)`, the original `addi r3, r31,
# 0x3732; clrlslwi r6, r28, 16, 1; lhzx r0, r3, r6`)
FORM_D = re.compile(r'M2C_FIELD\((\([A-Za-z_][\w.\->\[\]]* \+ (?:0x[0-9A-Fa-f]+|\d+)\)), ([\w ]+\**) \*, (\(?\(?(\(\w+\)\s*)?'
                    r'([A-Za-z_]\w*) (?:\*|<<) (0x[0-9A-Fa-f]+|\d+)\)?(?: & (?:0x[0-9A-Fa-f]+|\d+))?\)?)\)')


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='ixl_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def tsize(t):
    t = t.strip()
    return 4 if t.endswith('*') else SIZE.get(t)


def rewrite(text, forms):
    types = {}
    n = 0

    def elem(S, K, T):
        sz = tsize(T)
        if sz is None or K + sz > S or K % sz:
            return None
        nm = 'cr_El%X_%X_%s' % (S, K, re.sub(r'\W', 'p', T.strip()))
        types[nm] = (S, K, T.strip(), sz)
        return nm

    for xre, get in IDX:
        X = r'(?P<X>' + xre + r')'
        B = r'(?P<B>[A-Za-z_][\w.\->\[\]]*)'
        T = r'(?P<T>[\w ]+\**) \*'
        K = r'(?P<K>0x[0-9A-Fa-f]+|\d+)'
        pats = []
        if 'a' in forms:   # (B + (X + K)), T *, 0
            pats.append(re.compile(r'M2C_FIELD\(\(' + B + r' \+ \(' + X + r' \+ ' + K + r'\)\), ' + T + r', 0\)'))
        if 'b' in forms:   # (B + X + K), T *, 0
            pats.append(re.compile(r'M2C_FIELD\(\(' + B + r' \+ ' + X + r' \+ ' + K + r'\), ' + T + r', 0\)'))
        if 'c' in forms:   # (B + X), T *, K
            pats.append(re.compile(r'M2C_FIELD\(\(' + B + r' \+ ' + X + r'\), ' + T + r', ' + K + r'\)'))
        for p in pats:
            def fix(m):
                nonlocal n
                xm = re.fullmatch(xre, m.group('X'))
                if not xm:
                    return m.group(0)
                var, S = get(xm)
                nm = elem(S, int(m.group('K'), 0), m.group('T'))
                if nm is None:
                    return m.group(0)
                n += 1
                return f'((struct {nm} *) ({m.group("B")}))[{var}].v'
            text = p.sub(fix, text)
    if 'd' in forms:
        def fixd(m):
            nonlocal n
            T, var, k = m.group(2).strip(), m.group(5), int(m.group(6), 0)
            sz = tsize(T)
            if m.group(3).count('(') != m.group(3).count(')'):
                return m.group(0)
            stride = (1 << k) if '<<' in m.group(3) else k
            if sz is None or stride != sz:
                return m.group(0)
            n += 1
            return f'(({T} *) {m.group(1)})[{var}]'
        text = FORM_D.sub(fixd, text)
    if not n:
        return text, 0
    k = re.search(r'^\w[^;\n]*\)\s*\{\s*$', text, re.M)
    decl = ''
    for nm, (S, K, T, sz) in sorted(types.items()):
        body = (f'    u8 pad[0x{K:X}];\n' if K else '') + f'    {T} v;\n' + (f'    u8 tail[0x{S - K - sz:X}];\n' if S - K - sz else '')
        decl += f'struct {nm} {{\n{body}}};\n\n'
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
    base = score(func, text, 'base')
    best, btxt, how = base, None, ''
    for forms in ('abcd', 'abc', 'a', 'b', 'c', 'd', 'ab'):
        new, n = rewrite(text, forms)
        if not n:
            continue
        sc = score(func, new, forms)
        if sc[1] > best[1]:
            best, btxt, how = sc, new, f'{forms}: {n}'
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
        if text is None:
            continue
        gain += best[1] - base[1]
        open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
        print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}), {how} kept', flush=True)
    print(f'index_loads: +{gain} equal words')
