"""reuse_exprs.py: later copies of a register variable's expression go through the variable (Nov 13, 2026).

  python -I tools/reuse_exprs.py FUNC [FUNC ...] [--list file] [--from dir[,dir]] [--keep build/low100] [--jobs 8]

Why: m2c writes `var_r22 = lbl_80386550 + 0x6AC;` where the original put that address in r22, and then writes the
expression out again at the uses (`&(lbl_80386550 + 0x6AC)[i]`) where the original used r22 (`addi r22, r31, 0x6ac`
once, `lwzx r3, r22, r4` at the uses). The draft recomputes the address at every use (fn_80082F84: the original
has 70 more addi, the draft 26 more add). This is base_regs.py's `reuse` for any expression, not only a global.

What it does: per `var_rN = E;` / `temp_rN_M = E;` with E an address or arithmetic expression (identifiers,
numbers, + - * << & casts; at least one operator) whose identifiers are not assigned until the variable is: the
later occurrences of E (textually, parenthesised or not) up to the variable's next assignment become the variable.
All at once, then one by one while that helps (hd.py); kept when the equal words go up.
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
ASG = re.compile(r'^(\s*)((?:var|temp)_[rf]\w+) = (.+);\s*$')
PURE = re.compile(r'^[\w\s+\-*<>&|().]*$')
KW = {'u8', 's8', 'u16', 's16', 'u32', 's32', 'f32', 'f64', 'void', 'struct', 'unsigned', 'char', 'long', 'int'}


def score(func, text, tag):
    tmp = tempfile.mkdtemp(prefix='rex_')
    f = os.path.join(tmp, f'{func}_{tag}.c')
    open(f, 'w', encoding='utf-8', newline='\n').write(text)
    r = subprocess.run([sys.executable, '-I', HD, OUT, func, f], capture_output=True, text=True, cwd=G)
    m = re.search(r':\s*(\d+)%\s*\((\d+) equal words\)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (-1, -1)


def cands(text):
    lines = text.split('\n')
    out = []
    for j, l in enumerate(lines):
        m = ASG.match(l)
        if not m:
            continue
        v, e = m.group(2), m.group(3).strip()
        if not PURE.match(e) or not re.search(r'[+\-*<&|]', e) or v in e or len(e) < 8:
            continue
        ids = set(re.findall(r'\b[A-Za-z_]\w*\b', e)) - KW
        # the variable's range: to its next assignment, or the end of the enclosing block
        ind = len(m.group(1))
        end = len(lines)
        for k in range(j + 1, len(lines)):
            s = lines[k]
            if re.match(r'^\s*' + v + r'\s*(=[^=]|\+=|-=)', s) or (s.strip() and len(s) - len(s.lstrip()) < ind and s.strip().startswith('}')):
                end = k
                break
            if any(re.match(r'^\s*' + x + r'\s*(=[^=]|\+=|-=|\|=|&=)', s) for x in ids):
                end = k
                break
        out.append((j, v, e, end))
    return out


def apply(text, cs):
    lines = text.split('\n')
    n = 0
    for j, v, e, end in cs:
        pats = [re.escape('(' + e + ')'), re.escape(e)]
        for k in range(j + 1, end):
            for p in pats:
                new = re.sub(r'(?<![\w.])' + p + r'(?![\w(])', v, lines[k])
                if new != lines[k]:
                    lines[k] = new
                    n += 1
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
    cs = cands(text)
    useful = [c for c in cs if apply(text, [c])[1]]
    if not useful:
        return func, None
    base = score(func, text, 'base')
    best, btxt, how = base, None, ''
    t, n = apply(text, useful)
    sc = score(func, t, 'all')
    if sc[1] > best[1]:
        best, btxt, how = sc, t, f'all {len(useful)}'
    if len(useful) <= 30:
        cur = list(useful) if btxt else []
        for c in useful:
            trial = [x for x in cur if x is not c] if c in cur else cur + [c]
            t, _ = apply(text, trial)
            sc = score(func, t, 'one%d' % c[0])
            if sc[1] > best[1]:
                best, btxt, cur = sc, t, trial
                how = f'{len(cur)} of {len(useful)}'
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
        if r is None or r[3] is None:
            continue
        base, best, how, text = r
        gain += best[1] - base[1]
        open(os.path.join(KEEP, func + '.c'), 'w', encoding='utf-8', newline='\n').write(text)
        print(f'{func}: {base[0]}% ({base[1]}) -> {best[0]}% ({best[1]}), {how} kept', flush=True)
    print(f'reuse_exprs: +{gain} equal words')
