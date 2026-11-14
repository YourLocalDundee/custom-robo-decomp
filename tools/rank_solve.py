"""rank_solve.py LIST SRCDIRS KEEPDIR [JOBS]: the saved registers of a draft computed, not searched (Nov 13, 2026).

GC/2.6 at `-opt level=0` gives the callee-saved registers to the named locals and parameters before colouring, by the
rule of `allocate_saved_gprs` / `allocate_saved_fprs` in the compiler's decompilation (rayanht/mwcc, CodeGen.c):
repeatedly take the candidate with the highest usage (at least 2), a later candidate winning a tie, over the
parameters in order and then the locals in reverse declaration order; r31 / f31 down.  So:
  - the variable with the most uses gets r31;
  - a tie goes to a local before a parameter, to the first-declared of two locals, to the later of two parameters;
  - a variable with usage 1 gets no register (a parameter stays in its stack home, a local goes to the stack).
Usage, measured on this compiler by counting the dead uses that flip a pair (docs/drafts.md, Nov 13): every
occurrence 1 (no loop weight, unlike GC/1.x with -opt speed); 2 for `v += x`, `v++`, `++v`; 2 for an indirection
through the variable with no cast between (`*v`, `*(v + 1)`, `v[i]`, `v->m`; `*(T *) v` and M2C_FIELD(v, ...) are
1; `&v->m` is 1).  The model gives the register order of 200 of 207 matched drafts with m2c's register names.

For each function of LIST: the target register of a local is in its m2c name (var_r29, temp_r27_3, var_f30), of
a parameter in the original's entry block (`mr r27, r3`).  The draft's own dead uses and self copies are dropped,
the usage counted, the targeted locals declared in register order (their declaration lines permuted in place) and
the fewest `(void) v;` lines added that make the rule give the target order.  One compile; kept when the equal
words go up.  SRCDIRS: folders with comma, first that has FUNC.c wins; then tools/hand_drafts, decomp/drafts/c.
A line per function: name, start %, new %, start words, new words, what was added.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
NL, TAB = chr(10), chr(9)
KW = {'for', 'while', 'do'}
FLOATS = ('f32', 'f64', 'float', 'double')


def parse(s, f):
    """-> (params [(name, type)], locals [(name, type, aggregate, line index)], decl lines, body start, body end)"""
    m = re.search(r'(?m)^[A-Za-z].*\b' + re.escape(f) + r'\(([^)]*)\) \{$', s)
    if not m:
        return None
    params = []
    for p in m.group(1).split(','):
        w = re.findall(r'[A-Za-z_]\w*', p)
        if w and w[-1] != 'void':
            params.append((w[-1], ' '.join(w[:-1]), '*' in p))
    i = m.end() + 1
    j = s.find(NL + NL, i)
    if j < 0:
        return None
    lines = s[i:j].split(NL)
    locs, depth = [], 0
    for n, l in enumerate(lines):
        d0 = depth
        depth += l.count('{') - l.count('}')
        if d0 > 0 or depth > 0:
            continue
        mm = re.match(r'[ ]+([\w ]+?)[ ]*(\**)[ ]*(\w+);$', l)
        if mm:
            locs.append((mm.group(3), mm.group(1).strip(), bool(mm.group(2)), n))
    k = s.find(NL + '}', j)
    return params, locs, lines, i, j, (k if k > 0 else len(s))


def usage(body, names):
    use = {n: 0 for n in names}
    toks = re.findall(r'[A-Za-z_]\w*|->|\+\+|--|(?:<<|>>|[-+*/%&|^])=|0x[0-9A-Fa-f]+|\d[\w.]*|[{}();]|\S', body)
    toks = ['op=' if re.fullmatch(r'(?:<<|>>|[-+*/%&|^])=', x) else x for x in toks]
    for ti, t in enumerate(toks):
        if t not in use:
            continue
        prev = toks[ti - 1] if ti else ''
        nxt = toks[ti + 1] if ti + 1 < len(toks) else ''
        two = prev in ('++', '--') or nxt in ('++', '--', 'op=', '->', '[')
        if not two:
            k = ti - 1
            if prev == '(':
                k = ti - 2
            if k >= 0 and toks[k] == '*':
                b = toks[k - 1] if k else ''
                two = not (re.match(r'[A-Za-z_0-9]', b) or b in (')', ']'))
        use[t] += 2 if two else 1
    return use


def regno(n, cls):
    m = re.match(r'(?:var|temp)_%s(\d+)(?:_\d+)?$' % cls, n)
    return int(m.group(1)) if m and 14 <= int(m.group(1)) <= 31 else None


def param_regs(f, params):
    """{param: ('r', n) / ('f', n)} from the original's entry code: mr rN, r3.. / fmr fN, f1.."""
    p = f'decomp/drafts/asm/{f}.s'
    out = {}
    if not os.path.exists(p):
        return out
    gi, fi, home = 3, 1, {}
    for name, ty, ptr in params:
        if not ptr and ty.split()[-1] in FLOATS:
            home[('f', fi)] = name
            fi += 1
        else:
            home[('r', gi)] = name
            gi += 1
    for l in open(p, encoding='utf-8'):
        m = re.search(r'\*/\s+(\S+)\s*(.*)', l)
        if not m:
            continue
        op, args = m.group(1), m.group(2)
        if op in ('bl', 'b', 'blr', 'bctrl') or op.startswith('b') and op not in ('bl',):
            break
        mm = re.fullmatch(r'r(\d+), r(\d+)', args) if op == 'mr' else (re.fullmatch(r'f(\d+), f(\d+)', args) if op == 'fmr' else None)
        if mm:
            c = 'r' if op == 'mr' else 'f'
            d, sr = int(mm.group(1)), int(mm.group(2))
            if (c, sr) in home and 14 <= d <= 31 and home[(c, sr)] not in out:
                out[home[(c, sr)]] = (c, d)
    return out


def solve(s, f):
    """-> (new text, note) or (None, reason)"""
    # the draft's own dead uses and self copies go: the solver decides them
    s = re.sub(r'(?m)^[ ]+(?:\(void\) \w+; ?)+[ ]*' + NL, '', s)
    s = re.sub(r'(?m)^[ ]+(\w+) = \1;' + NL, '', s)
    r = parse(s, f)
    if not r:
        return None, 'no-parse'
    params, locs, lines, i, j, k = r
    body = re.sub(r'/\*.*?\*/', '', s[j:k], flags=re.S)
    body = re.sub(r'"(?:[^"\\]|\\.)*"', '""', body)
    names = [p[0] for p in params] + [l[0] for l in locs]
    use = usage(body, names)
    preg = param_regs(f, params)
    notes, dead = [], {}
    new_lines = list(lines)
    for cls in ('r', 'f'):
        isf = cls == 'f'
        target = {}
        for n, ty, ptr, ln in locs:
            if (not ptr and ty.split()[-1] in FLOATS) != isf:
                continue
            g = regno(n, cls)
            if g is not None:
                target[n] = g
        for n, (c, g) in preg.items():
            if c == cls:
                target[n] = g
        if len(target) < 2 or len(set(target.values())) != len(target):
            continue
        order = sorted(target, key=lambda n: -target[n])
        islocal = {l[0]: l[3] for l in locs}
        pidx = {p[0]: n for n, p in enumerate(params)}
        # the targeted locals declared in register order, in the slots they already occupy
        tl = [n for n in order if n in islocal]
        slots = sorted(islocal[n] for n in tl)
        for n, sl in zip(tl, slots):
            new_lines[sl] = lines[islocal[n]]
        rank = {n: x for x, n in enumerate(tl)}        # declaration rank after the permutation

        def wins_tie(a, b):
            if a in rank and b in rank:
                return rank[a] < rank[b]
            if a in rank:
                return True
            if b in rank:
                return False
            return pidx[a] > pidx[b]
        req = {}
        low = order[-1]
        req[low] = max(use[low], 2)
        for x in range(len(order) - 2, -1, -1):
            a, b = order[x], order[x + 1]
            need = req[b] if wins_tie(a, b) else req[b] + 1
            req[a] = max(use[a], need)
        for n in order:
            if req[n] > use[n]:
                dead[n] = req[n] - use[n]
        notes.append(cls + ':' + ' '.join(f'{n}={use[n]}' + (f'+{dead[n]}' if n in dead else '') for n in order))
    if not notes:
        return None, 'no-target'
    add = ''.join('    ' + f'(void) {n}; ' * c + NL for n, c in dead.items())
    t = s[:i] + NL.join(new_lines) + NL + NL + add + s[j + 2:]
    return t, ' | '.join(notes)


def score(f, text, keep):
    tmp = os.path.join(keep, f + '_rs.tmp.c')
    open(tmp, 'w', encoding='utf-8', newline=NL).write(text)
    try:
        r = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/hd.py', 'decomp/drafts', f, tmp],
                           capture_output=True, text=True, timeout=300).stdout.split(NL)[0]
    except subprocess.TimeoutExpired:
        r = ''
    try:
        os.remove(tmp)
    except OSError:
        pass
    m = re.search(r'(\d+)%\s+\((\d+) equal', r)
    return (int(m.group(2)), int(m.group(1))) if m else (-1, -1)


def main():
    lst, srcdirs, keep = sys.argv[1:4]
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
    os.makedirs(keep, exist_ok=True)

    def one(f):
        src = None
        for d in srcdirs.split(',') + ['tools/hand_drafts', 'decomp/drafts/c']:
            if os.path.exists(f'{d}/{f}.c'):
                src = f'{d}/{f}.c'
                break
        s = open(src, encoding='utf-8').read().replace(chr(13) + NL, NL)
        t, note = solve(s, f)
        if t is None:
            return f + TAB + note
        a = score(f, s, keep)
        b = score(f, t, keep)
        if b > a:
            open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(t)
        return TAB.join([f, str(a[1]), str(b[1]), str(a[0]), str(b[0]), note])

    with ThreadPoolExecutor(jobs) as ex:
        for line in ex.map(one, funcs):
            print(line, flush=True)


if __name__ == '__main__':
    main()
