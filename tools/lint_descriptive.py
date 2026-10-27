"""Check symbols/descriptive.txt and symbols/cr_globals.txt (our own names for Custom Robo's code and data).

Rules (see CONTRIBUTING.md):
  - every name starts with cr_ (so nobody mistakes it for a real/verified name)
  - every line has an evidence comment:  // why: <what the code does that justifies the name>
  - no address that already has a verified name in symbols/symbols.txt
  - no duplicate names or addresses; the address must be a function start (dtk analysis)
Exit code 1 on any problem.  usage: python -I tools/lint_descriptive.py
"""
import os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LINE = re.compile(r'^(\S+) = \.text:0x([0-9A-Fa-f]{8}); // type:function scope:global // why: (.+)$')


def addrs(path):
    out = set()
    for l in open(path, encoding='utf-8'):
        m = re.match(r'\S+ = \S*:0x([0-9A-Fa-f]{8});.*type:function', l)
        if m: out.add(int(m.group(1), 16))
    return out


def main():
    verified = addrs(os.path.join(G, 'symbols/symbols.txt'))
    starts = addrs(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
    names, seen, problems, n = set(), set(), [], 0
    for i, l in enumerate(open(os.path.join(G, 'symbols/descriptive.txt'), encoding='utf-8'), 1):
        l = l.rstrip('\n')
        if not l.strip() or l.startswith('//'): continue
        m = LINE.match(l)
        if not m: problems.append(f'{i}: bad format (needs "cr_Name = .text:0xADDR; // type:function scope:global // why: ...")'); continue
        name, a = m.group(1), int(m.group(2), 16)
        n += 1
        if not re.fullmatch(r'cr_[A-Za-z0-9_]+', name): problems.append(f'{i}: {name} must start with cr_')
        if a in verified: problems.append(f'{i}: {name} {a:08X} already has a verified name')
        if a not in starts: problems.append(f'{i}: {name} {a:08X} is not a function start')
        if name in names: problems.append(f'{i}: duplicate name {name}')
        if a in seen: problems.append(f'{i}: duplicate address {a:08X}')
        names.add(name); seen.add(a)
    # symbols/cr_globals.txt: cr_g_ names, a datatype, a why note, no duplicates, inside a data section
    gline = re.compile(r'^(\S+) = (\.\w+):0x([0-9A-Fa-f]{8}); // type:object datatype:\w+\**(?:\[\d+\])? // why: (.+)$')
    gnames, gseen, ng = set(), set(), 0
    for i, l in enumerate(open(os.path.join(G, 'symbols/cr_globals.txt'), encoding='utf-8'), 1):
        l = l.rstrip('\n')
        if not l.strip() or l.startswith('//'): continue
        m = gline.match(l)
        if not m: problems.append(f'cr_globals.txt {i}: bad format (needs "cr_g_Name = .bss:0xADDR; // type:object datatype:Type[N] // why: ...")'); continue
        name, sec, a = m.group(1), m.group(2), int(m.group(3), 16)
        ng += 1
        if not re.fullmatch(r'cr_g_[A-Za-z0-9_]+', name): problems.append(f'cr_globals.txt {i}: {name} must start with cr_g_')
        if sec in ('.text', '.init'): problems.append(f'cr_globals.txt {i}: {name} is in a code section')
        if name in gnames: problems.append(f'cr_globals.txt {i}: duplicate name {name}')
        if a in gseen: problems.append(f'cr_globals.txt {i}: duplicate address {a:08X}')
        gnames.add(name); gseen.add(a)
    print(f'descriptive: {n} function names, {ng} globals, {len(problems)} problems')
    for p in problems: print('  ' + p)
    sys.exit(1 if problems else 0)


if __name__ == '__main__':
    main()
