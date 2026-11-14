"""gaps.py [LIST]: saved registers of the original that nothing uses (a folded narrow intermediate, see gapfix.py).
Reads decomp/drafts/asm/FUNC.s; prints FUNC, saved registers, the unused ones. Without LIST: every open draft."""
import os
import re
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)


def gaps(f):
    p = f'decomp/drafts/asm/{f}.s'
    if not os.path.exists(p):
        return None
    ins = []
    for l in open(p, encoding='utf-8'):
        m = re.match(r'/\* \w+ \w+  [0-9A-F ]+\*/\s+(.*)', l)
        if m:
            ins.append(m.group(1).strip())
    saved, body = set(), []
    for i in ins:
        m = re.match(r'stmw r(\d+), ', i)
        if m:
            saved |= set(range(int(m.group(1)), 32))
            continue
        m = re.match(r'(stw|lwz) r(\d+), 0x[0-9a-f]+\(r1\)$', i)
        if m and 14 <= int(m.group(2)) <= 31:
            if m.group(1) == 'stw':
                saved.add(int(m.group(2)))
            continue
        if re.match(r'lmw r\d+, ', i):
            continue
        m = re.match(r'bl _savegpr_(\d+)', i)
        if m:
            saved |= set(range(int(m.group(1)), 32))
        body.append(i)
    used = set()
    for i in body:
        for r in re.findall(r'\br(\d+)\b', i):
            used.add(int(r))
    return sorted(saved), sorted(saved - used)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        funcs = [l.strip() for l in open(sys.argv[1], encoding='utf-8') if l.strip()]
    else:
        funcs = []
        for l in open('decomp/drafts/draft_report.txt', encoding='utf-8').read().split(chr(10))[2:]:
            m = re.match(r'(\w{8})\s+(\S+)\s+(\d+)\s+(.*)', l)
            if m and 'MATCH' not in m.group(4):
                funcs.append(m.group(2))
    n = 0
    for f in funcs:
        g = gaps(f)
        if g and g[1]:
            n += 1
            print(f, 'saved r%d-r31' % g[0][0], 'unused', ' '.join('r%d' % r for r in g[1]))
    print(n, 'of', len(funcs), 'with an unused saved register', file=sys.stderr)
