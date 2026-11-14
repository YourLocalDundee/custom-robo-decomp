"""gapfix.py LIST SRCDIRS KEEPDIR [JOBS]: a saved register of the original that nothing uses is a narrow intermediate
whose extension the peephole folded into the next one (`s16 g; g = kind - 6; slot = g;` with a u8 slot: extsh +
clrlwi 24 -> clrlwi 24, and g keeps its place in the register order).  For each function of LIST whose original has
more unused saved registers than the draft: try an intermediate (u16, s16, s8, u8) at every assignment to a narrow
local / field and at every narrow cast, declared last; where that adds an unused saved register without adding an
instruction, try every declaration position; keep the best by equal words.  One round per missing register.
SRCDIRS: folders with comma, first that has FUNC.c wins; then tools/hand_drafts, decomp/drafts/c.
Writes KEEPDIR/FUNC.c when the words go up, KEEPDIR/s/FUNC.c when only the register count got right (for the
rank sweep).  A line per function."""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gaps as gapmod  # noqa: E402

lst, srcdirs, keep = sys.argv[1:4]
jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
os.makedirs(os.path.join(keep, 's'), exist_ok=True)
NL = chr(10)
TAB = chr(9)
NARROW = ('u8', 's8', 'u16', 's16')


def measure(f, text, tag='g'):
    """-> (equal words, percent, instructions of mine, unused saved registers of mine)"""
    tmp = os.path.join(keep, f'{f}_{tag}.tmp.c')
    open(tmp, 'w', encoding='utf-8', newline=NL).write(text)
    try:
        out = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/hd.py', 'decomp/drafts', f, tmp,
                              '-w', '0', '99999'], capture_output=True, text=True, timeout=300).stdout.split(NL)
    except subprocess.TimeoutExpired:
        out = ['']
    try:
        os.remove(tmp)
    except OSError:
        pass
    m = re.search(r'(\d+)%\s+\((\d+) equal', out[0])
    if not m:
        return (-1, -1, 0, 0)
    mine = [l[5:45].strip() for l in out[1:] if re.match(r'\s*\d+  ', l)]
    mine = [x for x in mine if x]
    saved, used = set(), set()
    for i in mine:
        mm = re.match(r'stmw r(\d+),', i)
        if mm:
            saved |= set(range(int(mm.group(1)), 32))
            continue
        mm = re.match(r'(stw|lwz) r(\d+),\d+\(r1\)$', i)
        if mm and 14 <= int(mm.group(2)) <= 31:
            if mm.group(1) == 'stw':
                saved.add(int(mm.group(2)))
            continue
        if i.startswith('lmw '):
            continue
        for r in re.findall(r'\br(\d+)\b', i):
            used.add(int(r))
    return (int(m.group(2)), int(m.group(1)), len(mine), len(saved - used))


def split(s, f):
    m = re.search(r'(?m)^[A-Za-z].*\b' + re.escape(f) + r'\(([^)]*)\) \{$', s)
    if not m:
        return None
    i = m.end() + 1
    j = s.find(NL + NL, i)
    if j < 0:
        return None
    return i, j


def decl_types(s, i, j):
    out = {}
    for l in s[i:j].split(NL):
        m = re.match(r'\s+(\w+) (\w+);$', l)
        if m:
            out[m.group(2)] = m.group(1)
    return out


def close(s, i):
    d = 0
    for k in range(i, len(s)):
        if s[k] == '(':
            d += 1
        elif s[k] == ')':
            d -= 1
            if d == 0:
                return k
    return -1


def top_assign(line):
    """index of the ' = ' of a plain assignment at paren depth 0, else -1"""
    d = 0
    for k, c in enumerate(line):
        if c == '(' or c == '[':
            d += 1
        elif c == ')' or c == ']':
            d -= 1
        elif c == '=' and d == 0 and line[k - 1:k + 2] == ' = ':
            return k - 1
    return -1


def sites(s, i, j):
    """candidate rewrites: (body offset of the line, new lines text) with GAPV / as placeholder for the variable"""
    types = decl_types(s, i, j)
    body0 = j + 2
    out, pos = [], body0
    for line in s[body0:].split(NL):
        st = line.strip()
        ind = line[:len(line) - len(line.lstrip())]
        plain = st.endswith(';') and not re.match(r'(for|while|if|return|\}|else|do|switch|case|goto|break)\b', st)
        if plain:
            k = top_assign(line)
            if k > 0:
                lhs, rhs = line[len(ind):k], line[k + 3:-1]
                narrow = types.get(lhs) in NARROW or re.search(r', (u8|s8|u16|s16) \*', lhs.split('M2C_FIELD(')[-1] if 'M2C_FIELD(' in lhs else '')
                if narrow and 'GAPV' not in rhs and not re.fullmatch(r'[-\w]+U?', rhs):
                    out.append((pos, len(line), f'{ind}GAPV = {rhs};{NL}{ind}{lhs} = GAPV;'))
        if plain or re.match(r'if \(', st):
            for m in re.finditer(r'\((u8|s8|u16|s16)\) (\(|M2C_FIELD\()', line):
                a = m.end() - 1
                b = close(line, a)
                if b < 0:
                    continue
                e0 = m.end() if m.group(2) == '(' else m.end() - len('M2C_FIELD(')
                expr = line[e0:b] if m.group(2) == '(' else line[e0:b + 1]
                if re.fullmatch(r'[-\w]+U?', expr.strip()):
                    continue
                new = line[:m.end() - len(m.group(2))] + 'GAPV' + line[b + 1:]
                out.append((pos, len(line), f'{ind}GAPV = {expr};{NL}{new}'))
        pos += len(line) + 1
    return out


def apply(s, i, j, site, ty, name, at):
    """site applied, `ty name;` declared before top-level declaration line number `at` (None: last)"""
    pos, ln, new = site
    t = s[:pos] + new.replace('GAPV', name) + s[pos + ln:]
    decl = s[i:j].split(NL)
    tops, d = [], 0
    for n, l in enumerate(decl):
        if d == 0:
            tops.append(n)
        d += l.count('{') - l.count('}')
    line = f'    {ty} {name};'
    if at is None or at >= len(tops):
        decl.append(line)
    else:
        decl.insert(tops[at], line)
    return t[:i] + NL.join(decl) + t[j:], len(tops)


def one(f):
    g = gapmod.gaps(f)
    if not g or not g[1]:
        return f + TAB + 'no-gap'
    want = len(g[1])
    src = None
    for d in srcdirs.split(',') + ['tools/hand_drafts', 'decomp/drafts/c']:
        if os.path.exists(f'{d}/{f}.c'):
            src = f'{d}/{f}.c'
            break
    s = open(src, encoding='utf-8').read().replace(chr(13) + NL, NL)
    base = measure(f, s)
    if base[0] < 0:
        return f + TAB + 'no-compile'
    start, log, structural = base, [], False
    for rnd in range(want):
        if base[3] >= want or base[1] == 100:
            break
        sp = split(s, f)
        if not sp:
            return f + TAB + 'no-parse'
        i, j = sp
        name = f'gap{rnd}'
        cands = []
        for site in sites(s, i, j):
            for ty in ('s16', 'u16', 's8', 'u8'):
                t, ntop = apply(s, i, j, site, ty, name, None)
                m = measure(f, t)
                if m[0] >= 0 and m[3] == base[3] + 1 and m[2] <= base[2]:
                    cands.append((m, site, ty, ntop))
        if not cands:
            break
        cands.sort(key=lambda c: -c[0][0])
        best = None
        for m, site, ty, ntop in cands[:3]:
            if best is None or m > best[0]:
                best = (m, apply(s, i, j, site, ty, name, None)[0], ty, 'last')
            for at in range(ntop):
                t, _ = apply(s, i, j, site, ty, name, at)
                mm = measure(f, t)
                if mm[0] >= 0 and mm[3] == base[3] + 1 and mm > best[0]:
                    best = (mm, t, ty, at)
                    if mm[1] == 100:
                        break
            if best[0][1] == 100:
                break
        s, base, structural = best[1], best[0], True
        log.append(f'{best[2]}@{best[3]}')
    if base[0] > start[0]:
        open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(s)
    elif structural:
        open(os.path.join(keep, 's', f + '.c'), 'w', encoding='utf-8', newline=NL).write(s)
    return TAB.join([f, str(start[1]), str(base[1]), str(start[0]), str(base[0]), f'gaps {start[3]}->{base[3]}/{want}',
                     ' '.join(log)])


with ThreadPoolExecutor(jobs) as ex:
    for line in ex.map(one, funcs):
        print(line, flush=True)
