"""Roadmap Phase 0 step 4: compare Ghidra's output with decomp-toolkit's own analysis.

Checks:
  1. function starts: in dtk only / in Ghidra only (C export headers vs dtk symbols.txt)
  2. names: every name in symbols/symbols.txt appears at its address in the C export
  3. small-data: functions still using unaff_r2 (Ghidra did not resolve r2) and dtk data labels
     in .sdata/.sdata2/.sbss/.sbss2 that the C never mentions
Writes symbols/crosscheck_report.txt.  usage: python -I tools/crosscheck_dtk.py
"""
import os, re

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SYM = re.compile(r'^(\S+) = (\.\w+):0x([0-9A-Fa-f]{8});.*?type:(\w+)')
HDR = re.compile(r'^// ([0-9a-f]{8})  (\S+)$')


def syms(path):
    out = []
    for l in open(path, encoding='utf-8'):
        m = SYM.match(l)
        if m: out.append((m.group(1), m.group(2), int(m.group(3), 16), m.group(4)))
    return out


def main():
    dtk = syms(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
    ours = syms(os.path.join(G, 'symbols/symbols.txt'))
    c = open(os.path.join(G, 'decompiled/main_dol.c'), encoding='utf-8', errors='replace').read()
    ghidra, cur, r2 = {}, None, []
    for line in c.split('\n'):
        m = HDR.match(line)
        if m: cur = (int(m.group(1), 16), m.group(2)); ghidra[cur[0]] = cur[1]
        elif cur and 'unaff_r2' in line and (not r2 or r2[-1] != cur): r2.append(cur)
    dfun = {a for n, s, a, t in dtk if t == 'function'}
    only_dtk = sorted(dfun - set(ghidra))
    only_ghidra = sorted(set(ghidra) - dfun)
    wrong = [(n, a, ghidra.get(a)) for n, s, a, t in ours if t == 'function' and ghidra.get(a) != n]
    words = set(re.findall(r'\b\w+\b', c))
    small = [(n, s, a) for n, s, a, t in dtk if s in ('.sdata', '.sdata2', '.sbss', '.sbss2') and t != 'function'
             and not n.startswith(('lbl_', '@', '...'))]
    unseen = [x for x in small if x[0] not in words and f'DAT_{x[2]:08x}' not in words]

    rep = [f'functions: dtk {len(dfun)}, ghidra {len(ghidra)}; only in dtk {len(only_dtk)}; only in ghidra {len(only_ghidra)}',
           f'names from symbols.txt not at their address in the C: {len(wrong)}',
           f'functions with unaff_r2: {len(r2)}',
           f'dtk named small-data labels (no lbl_/@ auto names): {len(small)}; not mentioned in the C by name or DAT_ address: {len(unseen)}', '']
    rep += ['## only in dtk'] + [f'{a:08X}' for a in only_dtk]
    rep += ['', '## only in ghidra'] + [f'{a:08X} {ghidra[a]}' for a in only_ghidra]
    rep += ['', '## name not applied (name, addr, ghidra has)'] + [f'{n} {a:08X} {g}' for n, a, g in wrong]
    rep += ['', '## unaff_r2 functions'] + [f'{a:08X} {n}' for a, n in r2]
    rep += ['', '## small-data labels unseen in C'] + [f'{s} {a:08X} {n}' for n, s, a in unseen]
    open(os.path.join(G, 'symbols/crosscheck_report.txt'), 'w', encoding='utf-8').write('\n'.join(rep) + '\n')
    print('crosscheck: ' + ' | '.join(rep[:4]))


if __name__ == '__main__':
    main()
