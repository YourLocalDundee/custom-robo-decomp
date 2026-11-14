"""goto_loops.py LIST SRCDIRS KEEPDIR [JOBS]: m2c's `loop_N:` + `goto loop_N;` loops as while loops (Nov 13, 2026).

m2c prints a loop whose body leaves through a test as a label and gotos:
    loop_6:                          while (i < 0x100) {
        if (i < 0x100) {                 if (!(D)) {
            if (D) {                         break;
                BODY                     }
                goto loop_6;             BODY
            }                        }
        }
The original is a `while` / `for` with a `break` (MWCC at level 0 jumps to the test at the bottom: `b test` ...
`blt body`; the goto form tests at the top).  fn_8009381C went 90 -> 98% with its four loops rewritten.
For each label whose gotos all sit in the `if (C) { ... }` right after it, these forms are tried and the best by
equal words kept (labels from the innermost out, until nothing changes):
  plain    the goto is the last statement of the block:           while (C) { BODY }
  break    the block is one `if (D) { BODY; goto L; }`:           while (C) { if (!D) { break; } BODY }
           (!D by De Morgan over == / != terms, else `!(D)`)
  cont     the same block kept:                                   while (C) { if (D) { BODY; continue; } break; }
  general  any placement outside nested loops:                    while (C) { BODY with continue; break; }
SRCDIRS: folders with comma, first that has FUNC.c wins; then tools/hand_drafts, decomp/drafts/c.
A line per function: name, start %, best %, start words, best words, the forms used.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
NL, TAB = chr(10), chr(9)


def score(f, text, keep):
    tmp = os.path.join(keep, f + '_gl.tmp.c')
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


def indent(l):
    return len(l) - len(l.lstrip(' '))


def split_top(expr, op):
    """expr split at the top-level occurrences of op (' || ' / ' && '), or None when the other one is at top level"""
    parts, d, last, k = [], 0, 0, 0
    other = ' && ' if op == ' || ' else ' || '
    while k < len(expr):
        c = expr[k]
        if c == '(':
            d += 1
        elif c == ')':
            d -= 1
        elif d == 0 and expr.startswith(other, k):
            return None
        elif d == 0 and expr.startswith(op, k):
            parts.append(expr[last:k])
            last = k + len(op)
            k += len(op) - 1
        k += 1
    parts.append(expr[last:])
    return parts


def strip_parens(t):
    t = t.strip()
    while t.startswith('(') and t.endswith(')'):
        d = 0
        for k, c in enumerate(t):
            if c == '(':
                d += 1
            elif c == ')':
                d -= 1
                if d == 0 and k != len(t) - 1:
                    return t
        t = t[1:-1].strip()
    return t


def neg_term(t):
    u = strip_parens(t)
    d = 0
    for k in range(len(u) - 3):
        c = u[k]
        if c == '(':
            d += 1
        elif c == ')':
            d -= 1
        elif d == 0 and u[k:k + 4] in (' == ', ' != '):
            return '(' + u[:k] + (' != ' if u[k:k + 4] == ' == ' else ' == ') + u[k + 4:] + ')'
    if u.startswith('!') and not u.startswith('!='):
        return strip_parens(u[1:]) if u[1:].startswith('(') else u[1:]
    return '!(' + u + ')'


def negations(d):
    out = ['!(' + d + ')']
    for op, nop in ((' || ', ' && '), (' && ', ' || ')):
        parts = split_top(d, op)
        if parts and len(parts) > 1:
            out.insert(0, nop.join(neg_term(p) for p in parts))
            return out
    n = neg_term(d)
    if n != out[0]:
        out.insert(0, strip_parens(n))
    return out


def block_end(lines, start):
    """index of the line that closes the block opened on lines[start] (brace counting)"""
    d = 0
    for k in range(start, len(lines)):
        d += lines[k].count('{') - lines[k].count('}')
        if d == 0:
            return k
    return -1


def candidates(lines, li):
    """rewrites of the loop at label line li -> [(name, new lines)]"""
    m = re.match(r'\s*(loop_\w+):\s*$', lines[li])
    label = m.group(1)
    a = li + 1
    hm = re.match(r'( *)if \((.*)\) \{$', lines[a]) if a < len(lines) else None
    if not hm:
        return []
    ind, cond = hm.group(1), hm.group(2)
    b = block_end(lines, a)
    if b < 0 or lines[b].strip() != '}':
        return []
    body = lines[a + 1:b]
    goto = f'goto {label};'
    where = [k for k, l in enumerate(body) if l.strip() == goto]
    total = sum(1 for l in lines if l.strip() == goto)
    if not where or len(where) != total:
        return []
    # a goto inside a nested loop or switch of the body would bind `continue` to that loop
    depth_loops = []
    stack = []
    for k, l in enumerate(body):
        st = l.strip()
        if st.endswith('{'):
            stack.append(bool(re.match(r'(\} )?(while|for|do|switch)\b', st)))
        if k in where and any(stack):
            depth_loops.append(k)
        if st.startswith('}'):
            if stack:
                stack.pop()
            if st.endswith('{'):
                stack.append(bool(re.match(r'\} (while|for|do|switch)\b', st)))
    head, tail = lines[:li], lines[b + 1:]
    out = []
    w = f'{ind}while ({cond}) {{'
    one = ind + '    '
    if len(where) == 1 and where[0] == len(body) - 1:
        out.append(('plain', head + [w] + body[:-1] + [ind + '}'] + tail))
    im = re.match(r'( *)if \((.*)\) \{$', body[0]) if body else None
    if im and len(where) == 1 and block_end(body, 0) == len(body) - 1 and where[0] == len(body) - 2:
        inner = body[1:-2]
        ded = [l[4:] if l.startswith('    ') else l for l in inner]
        for n in negations(im.group(2)):
            out.append(('break', head + [w, f'{one}if ({n}) {{', f'{one}    break;', f'{one}}}'] + ded + [ind + '}'] + tail))
        out.append(('cont', head + [w] + body[:-2] + [body[-2].replace(goto, 'continue;'), body[-1], f'{one}break;', ind + '}'] + tail))
    if not depth_loops and not out:
        nb = [l.replace(goto, 'continue;') if l.strip() == goto else l for l in body]
        out.append(('general', head + [w] + nb + [f'{one}break;', ind + '}'] + tail))
    return out


def one_func(f, srcdirs, keep):
    src = None
    for d in srcdirs.split(',') + ['tools/hand_drafts', 'decomp/drafts/c']:
        if os.path.exists(f'{d}/{f}.c'):
            src = f'{d}/{f}.c'
            break
    s = open(src, encoding='utf-8').read().replace(chr(13) + NL, NL)
    if 'goto loop_' not in s:
        return f + TAB + 'no-loop'
    start = best = score(f, s, keep)
    if start[0] < 0:
        return f + TAB + 'no-compile'
    lines = s.split(NL)
    used = []
    for rnd in range(12):
        labels = [k for k, l in enumerate(lines) if re.match(r'\s*loop_\w+:\s*$', l)]
        moved = False
        for li in reversed(labels):         # the innermost loops come later in the text
            got = None
            for name, nl in candidates(lines, li):
                sc = score(f, NL.join(nl), keep)
                if sc > best or (got is None and sc >= best and sc[0] >= 0 and name in ('plain', 'break')):
                    if sc >= best:
                        best, got = sc, (name, nl)
            if got:
                lines = got[1]
                used.append(got[0])
                moved = True
                break
        if not moved:
            break
    if best > start or (used and best >= start):
        open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(NL.join(lines))
    return TAB.join([f, str(start[1]), str(best[1]), str(start[0]), str(best[0]), ' '.join(used)])


def main():
    lst, srcdirs, keep = sys.argv[1:4]
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
    os.makedirs(keep, exist_ok=True)
    with ThreadPoolExecutor(jobs) as ex:
        for line in ex.map(lambda f: one_func(f, srcdirs, keep), funcs):
            print(line, flush=True)


if __name__ == '__main__':
    main()
