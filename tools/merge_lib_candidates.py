"""Merge helper for tools/batches/batch6_lib_candidates.ps1.

Reads symbols/candidates_*.txt, checks every line against symbols.txt (already stripped of old lib-candidate lines),
symbols/descriptive.txt and tools/dtk/proj/symbols.txt, and writes
  - the accepted lines, tagged "lib-candidate", to the file given as argv[1]
  - the layout-proof lines that an accepted candidate replaces on purpose (why: ... replaces:OLD) to argv[2]
  - the report lines of this round to argv[3] (batch6 composes symbols/lib_candidates_report.txt)
Rejected: address below 0x80131000 or not a function start of the same size in dtk's analysis; same address with
different names (all lines involved); same name at two addresses (all lines involved); name or address already in
symbols.txt / descriptive.txt (unless replaced as above); placeholder-looking, C++ mangled or malformed names.
Deterministic: no timestamps.
usage: python -I tools/merge_lib_candidates.py <accepted out> <replaced out> <report part out>
stdout: candidates-read accepted rejected replaced
"""
import glob, os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIB_START = 0x80131000
LINE = re.compile(r'^(\S+) = \.text:0x([0-9A-Fa-f]{8}); // type:function size:0x([0-9A-Fa-f]+) scope:global '
                  r'// (R[3-6]) ref:(\S+)(?: \S+:\S+)* why: (.+)$')
SYM = re.compile(r'^(\S+) = \.\w+:0x([0-9A-Fa-f]{8});')
PLACEHOLDER = re.compile(r'(_[0-9a-fA-F]{6,8}$)|^(fn_|lbl_|jumptable_|@|gap_|FUN_|func_|sub_|kar_|zz_|unk_|loc_|dat_|DAT_|LAB_)')
MANGLED = re.compile(r'^[^_].*__\w')


def read(p):
    return open(p, encoding='utf-8').read().splitlines() if os.path.exists(p) else []


syms, names = {}, {}   # addr -> (name, line); name -> addr
for l in read(os.path.join(G, 'symbols/symbols.txt')):
    m = SYM.match(l)
    if m and ' type:function' in l:
        syms[int(m.group(2), 16)] = (m.group(1), l)
        names[m.group(1)] = int(m.group(2), 16)
desc_a, desc_n = {}, {}
for l in read(os.path.join(G, 'symbols/descriptive.txt')):
    m = SYM.match(l)
    if m:
        desc_a[int(m.group(2), 16)] = m.group(1)
        desc_n[m.group(1)] = int(m.group(2), 16)
dtk = {}
for l in read(os.path.join(G, 'tools/dtk/proj/symbols.txt')):
    m = re.match(r'(\S+) = \.text:0x([0-9A-F]{8}); // type:function size:0x([0-9A-F]+)', l)
    if m: dtk[int(m.group(2), 16)] = int(m.group(3), 16)

cands = []   # (file, lineno, name, addr, size, rule, text)
report = []
rej = {}     # (file, lineno) -> reason


def reject(c, why): rej.setdefault((c[0], c[1]), why)


for f in sorted(glob.glob(os.path.join(G, 'symbols/candidates_*.txt'))):
    fn = os.path.basename(f)
    for i, l in enumerate(read(f), 1):
        if not l.strip() or l.startswith('//'): continue
        m = LINE.match(l)
        if not m:
            report.append(f'REJECT {fn}:{i} malformed line: {l[:100]}')
            continue
        name, addr = m.group(1), int(m.group(2), 16)
        if addr in syms and syms[addr][0] == name and '// lib-candidate ' in syms[addr][1]:
            continue   # already merged in an earlier round of this batch run
        cands.append((fn, i, name, addr, int(m.group(3), 16), m.group(4), l))

for c in cands:
    _, _, name, addr, size, rule, text = c
    if addr < LIB_START: reject(c, f'address 0x{addr:08X} below 0x{LIB_START:08X}')
    elif dtk.get(addr) != size: reject(c, f'not a function of size 0x{size:X} in dtk analysis (dtk: {dtk.get(addr)})')
    if PLACEHOLDER.search(name) or MANGLED.match(name) or not re.match(r'^[A-Za-z_][A-Za-z0-9_]*$', name):
        reject(c, 'placeholder / mangled / invalid name')
    if name.startswith('cr_'): reject(c, 'cr_ names belong in descriptive.txt')

# conflicts among candidates (identical duplicates are fine)
by_addr, by_name = {}, {}
for c in cands:
    by_addr.setdefault(c[3], set()).add(c[2])
    by_name.setdefault(c[2], set()).add(c[3])
for c in cands:
    if len(by_addr[c[3]]) > 1:
        reject(c, 'address has different names across candidates: ' + ', '.join(sorted(by_addr[c[3]])))
    if len(by_name[c[2]]) > 1:
        reject(c, 'name proposed for several addresses: ' + ', '.join('0x%08X' % a for a in sorted(by_name[c[2]])))

# layout-proof lines replaced on purpose
replaced = {}
for c in cands:
    if (c[0], c[1]) in rej: continue
    m = re.search(r'replaces:(\S+)', c[6])
    if not m: continue
    old = syms.get(c[3])
    if old and old[0] in (m.group(1), c[2]) and '// layout-proof ' in old[1]: replaced[c[3]] = old[0]
    elif not old: pass   # layout_match no longer emits the shifted name there: nothing to replace
    else: reject(c,f'replaces:{m.group(1)} but 0x{c[3]:08X} holds {old[0] if old else "nothing"} (not a layout-proof line)')

accepted, seen = [], set()
for c in cands:
    if (c[0], c[1]) in rej: continue
    _, _, name, addr, size, rule, text = c
    if addr in syms and addr not in replaced: reject(c, f'address already named {syms[addr][0]} in symbols.txt'); continue
    if name in names and names[name] not in replaced: reject(c, f'name already used at 0x{names[name]:08X} in symbols.txt'); continue
    if addr in desc_a: reject(c, f'address already named {desc_a[addr]} in descriptive.txt'); continue
    if name in desc_n: reject(c, f'name already used at 0x{desc_n[name]:08X} in descriptive.txt'); continue
    if (name, addr) in seen: continue
    seen.add((name, addr))
    accepted.append((addr, name, text.replace(f' scope:global // {rule} ', f' scope:global // lib-candidate {rule} ', 1)))

accepted.sort()
with open(sys.argv[1], 'w', encoding='utf-8', newline='\n') as f:
    for a, n, t in accepted: f.write(t + '\n')
with open(sys.argv[2], 'w', encoding='utf-8', newline='\n') as f:
    for a in sorted(replaced): f.write(f'{replaced[a]} 0x{a:08X}\n')
rules = {}
for a, n, t in accepted:
    r = re.search(r'lib-candidate (R\d)', t).group(1)
    rules[r] = rules.get(r, 0) + 1
nrej = len(rej) + len(report)
lines = []
for c in cands:
    if (c[0], c[1]) in rej: lines.append(f'REJECT {c[0]}:{c[1]} {c[2]} 0x{c[3]:08X}: {rej[(c[0], c[1])]}')
lines += report
for a in sorted(replaced): lines.append(f'REPLACED layout-proof {replaced[a]} at 0x{a:08X}')
for a, n, t in accepted:
    m = re.search(r'lib-candidate (R\d) ref:(\S+)', t)
    lines.append(f'ACCEPT {n} 0x{a:08X} {m.group(1)} {m.group(2)}')
with open(sys.argv[3], 'w', encoding='utf-8', newline='\n') as f:
    f.write('\n'.join(lines) + '\n')
print(f'{len(cands)} {len(accepted)} {nrej} {len(replaced)}')
