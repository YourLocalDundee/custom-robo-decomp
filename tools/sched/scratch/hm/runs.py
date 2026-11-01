"""scratch: the file-run table from promote_report.txt + draft_report.txt"""
import re, sys
G = r'D:\emulators\gameTesting-hsd2'
rep = open(G + r'\decomp\drafts\draft_report.txt', encoding='utf-8').read().splitlines()
funcs = []  # (addr, name, size, status)
for l in rep:
    m = re.match(r'^([0-9A-F]{8})  (\S+)\s+(\d+)  (.*?)\s*$', l)
    if m: funcs.append((int(m.group(1), 16), m.group(2), int(m.group(3)), m.group(4)))
idx = {f[1]: i for i, f in enumerate(funcs)}
pr = open(G + r'\decomp\reports\promote_report.txt', encoding='utf-8').read().splitlines()
rows = []
for l in pr:
    m = re.match(r'^([0-9A-F]{8})-([0-9A-F]{8})  (\S+)\.\.(\S+)  (\d+) functions, (\d+) at 100%: (.*)$', l)
    if not m: continue
    a, b = idx[m.group(3)], idx[m.group(4)]
    mem = funcs[a:b + 1]
    miss = [f for f in mem if '100% MATCH' not in f[3]]
    def pct(f):
        mm = re.match(r'(\d+)%', f[3]); return mm.group(1) + '%' if mm else f[3].split('  ')[0]
    rows.append((len(miss), m.group(1), m.group(2), m.group(3), m.group(4), len(mem), len(mem) - len(miss), sum(f[2] for f in miss), miss, m.group(7), pct))
rows.sort(key=lambda r: (r[0], r[7]))
print('| run (first..last) | span | functions | at 100% | blockers | blocker bytes | blockers (size, match) |')
print('|---|---|---|---|---|---|---|')
for r in rows:
    n, a, b, f1, f2, nf, n100, bb, miss, why, pct = r
    if n == 0:
        bl = why
    else:
        bl = ', '.join(f'{f[1]} (0x{f[2]:X}, {pct(f)})' for f in miss[:8]) + (f' ... +{n-8} more' if n > 8 else '')
    print(f'| {f1}..{f2} | 0x{a}-0x{b} | {nf} | {n100} | {n} | 0x{bb:X} | {bl} |')
