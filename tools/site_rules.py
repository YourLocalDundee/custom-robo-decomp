"""site_rules.py LIST SRCDIR KEEPDIR [JOBS]: site-by-site source rewrites (Nov 13, 2026, the top 150 set), each kept
only when the equal words go up:
  regroup  M2C_FIELD((B + ((I) + C)), T *, 0) -> M2C_FIELD((B + (I)), T *, C)   (the constant as the field offset:
           `lhax` with the index I + C becomes / stops being `add; lha C(r)`), and the reverse
  unmask   &A[(i * n) & MASK] -> (A + (i * n))   (m2c's mask of a u8 / u16 index)
  align    E & 0xFFFFFFFC -> (E >> 2) << 2  (and ~7 / ~0xF / ~0x1F with 3 / 4 / 5): clrrwi from the value's own register
  voidcall F(args); with a value prototype in ctx_protos.h -> ((void (*)(params)) F)(args);  (the result of a
           value call stays in r3 until the statement ends, so the next statement's temporaries start at r4)
  valuecall the reverse: F(args); with a void prototype -> ((s32 (*)(params)) F)(args);  (fn_8006A344 returns a
           value its callers drop; the context says void)
  home     spN = argK; (m2c's copy of a parameter that lives in its stack home) -> the parameter itself
Source: SRCDIR/FUNC.c, else tools/hand_drafts, else decomp/drafts/c."""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
lst, srcdir, keep = sys.argv[1:4]
jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
os.makedirs(keep, exist_ok=True)
NL, TAB = chr(10), chr(9)
P = r'\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)'      # a parenthesised group, 3 levels
RULES = [
    ('regroup', re.compile(r' \+ \((' + P + r') \+ (0x[0-9A-F]+|\d+)\)\), ([\w ]+ \*+), 0\)'),
     lambda m: f' + {m.group(1)}), {m.group(3)}, {m.group(2)})'),
    ('ungroup', re.compile(r' \+ (' + P + r')\), ([\w ]+ \*+), (0x[0-9A-F]+|[1-9]\d*)\)'),
     lambda m: f' + ({m.group(1)} + {m.group(3)})), {m.group(2)}, 0)'),
    ('unmask', re.compile(r'&(\w+)\[(\(\w+ \* (?:0x)?[0-9A-F]+\)) & 0x[0-9A-F]+\]'),
     lambda m: f'({m.group(1)} + {m.group(2)})'),
    ('align', re.compile(r'(M2C_FIELD' + P + r'|\w+) & (0xFFFFFFFC|0xFFFFFFF8|0xFFFFFFF0|0xFFFFFFE0)U?'),
     lambda m: '(({0} >> {1}) << {1})'.format(m.group(1), {'C': 2, '8': 3, '0': 4}[m.group(2)[-1]] if m.group(2) != '0xFFFFFFE0' else 5)),
]


def protos():
    """non-void prototypes of the context: name -> parameter list text"""
    out = {}
    for l in open('decomp/drafts/ctx_protos.h', encoding='utf-8'):
        m = re.match(r'(\w[\w \*]*?)\b(\w+)\(([^;]*)\);$', l.strip())
        if m and m.group(1).strip() != 'void':
            out[m.group(2)] = m.group(3)
    return out


PROTOS = protos()
CALL = re.compile(r'(?m)^([ ]+)(\w+)(\(.*\);)$')


def voidcall(m):
    name = m.group(2)
    if name not in PROTOS:
        return m.group(0)
    return f'{m.group(1)}((void (*)({PROTOS[name]})) {name}){m.group(3)}'


RULES.append(('voidcall', CALL, voidcall))


def void_protos():
    out = {}
    for l in open('decomp/drafts/ctx_protos.h', encoding='utf-8'):
        m = re.match(r'void (\w+)\(([^;]*)\);$', l.strip())
        if m:
            out[m.group(1)] = m.group(2)
    return out


VOIDS = void_protos()


def valuecall(m):
    name = m.group(2)
    if name not in VOIDS:
        return m.group(0)
    return f'{m.group(1)}((s32 (*)({VOIDS[name]})) {name}){m.group(3)}'


RULES.append(('valuecall', CALL, valuecall))


def score(f, text):
    tmp = os.path.join(keep, f + '_rw2.tmp.c')
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


def one(f):
    p = os.path.join(srcdir, f + '.c')
    if not os.path.exists(p):
        p = f'tools/hand_drafts/{f}.c' if os.path.exists(f'tools/hand_drafts/{f}.c') else f'decomp/drafts/c/{f}.c'
    s = open(p, encoding='utf-8').read().replace(chr(13) + NL, NL)
    if not any(r[1].search(s) for r in RULES) and not re.search(r'(?m)^[ ]+sp[0-9A-F]+ = arg\d+;$', s):
        return f + TAB + 'no-site'
    start = best = score(f, s)
    if start[0] < 0:
        return f + TAB + 'no-compile'
    used, tries = [], 0
    for m in list(re.finditer(r'(?m)^[ ]+(sp[0-9A-F]+) = (arg\d+);$' + NL, s)):
        sp, arg = m.group(1), m.group(2)
        if m.group(0) not in s or not re.search(r'(?m)^[ ]+[\w ]+[ \*]' + sp + ';$', s):
            continue
        t = s.replace(m.group(0), '', 1)
        t = re.sub(r'(?m)^[ ]+[\w ]+[ \*]' + sp + ';$' + NL, '', t, count=1)
        t = re.sub(r'\b' + sp + r'\b', arg, t)
        tries += 1
        sc = score(f, t)
        if sc > best:
            s, best = t, sc
            used.append('home')
    for name, pat, fn in RULES:
        k = 0
        while tries < 80:
            ms = list(pat.finditer(s))
            if k >= len(ms):
                break
            m = ms[k]
            t = s[:m.start()] + fn(m) + s[m.end():]
            if t == s:
                k += 1
                continue
            tries += 1
            sc = score(f, t)
            if sc > best:
                s, best = t, sc
                used.append(name)
                if name in ('regroup', 'ungroup'):
                    k += 1      # the rewritten site still matches the reverse rule's shape; move on
            else:
                k += 1
    if best > start:
        open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(s)
    return TAB.join([f, str(start[1]), str(best[1]), str(start[0]), str(best[0]), ' '.join(sorted(set(used))), str(tries)])


with ThreadPoolExecutor(jobs) as ex:
    for line in ex.map(one, funcs):
        print(line, flush=True)
