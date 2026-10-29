"""Give the objects built from C their own symbols in decomp/config/GXCE01/symbols.txt (plan.md Phase 4 Milestone 4).

dtk names what it finds in main.dol (fn_ / lbl_ / @NN guesses, one lbl_ for a run of strings); a C object has its own
symbols (every string literal its own @NN, every static its own size). objdiff pairs data by NAME, so a C object's
data can only reach 100% when its split (target) object has the C's symbols. Like every GC decomp's symbols.txt, the
names come from the matched C, here generated instead of typed:

Run by batch 9 after the first ninja (which linked main.elf WITH the configure.py CR_MATCHING objects from C and had to
give the retail SHA-1, so every address is the retail one):
  1. For every CR_MATCHING object, the symbols main.elf has inside that object's ranges of splits.txt (mwld keeps
     every local symbol of the C object: @NN literals, statics, functions), with their size, type and binding.
  2. In decomp/config/GXCE01/symbols.txt every symbol inside those ranges is replaced by them (nothing outside the
     ranges changes). The bytes are the same, so the next split gives the same main.dol; batch 9 then splits again
     and checks the SHA-1, objdiff 100% and the fixpoint (a second split changes nothing).
Deterministic: the symbols come from the C objects alone (compiled from decomp/src, linked at retail addresses).
Prints whether symbols.txt changed (exit 0 either way; batch 9 re-splits when it did).
Usage: python -I tools/gen_c_symbols.py
"""
import os
import re
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import reloc_audit as ra  # noqa: E402
from check_c_objects import configure_lists  # noqa: E402

D = os.path.join(G, 'decomp')
CFG = os.path.join(D, 'config', 'GXCE01')
ELF = os.path.join(D, 'build', 'GXCE01', 'main.elf')
LINE = re.compile(r'^([^\s=]+) = (\.[\w.]+):0x([0-9A-Fa-f]+);')
STT = {0: 'label', 1: 'object', 2: 'function'}


def ranges(names):
    out, cur = [], None
    for ln in open(os.path.join(CFG, 'splits.txt'), encoding='utf-8'):
        if ln.strip() and not ln[0].isspace():
            cur = ln.split(':')[0].strip()
        elif cur in names and ln.strip():
            m = re.match(r'\s+(\S+)\s+start:0x([0-9A-Fa-f]+) end:0x([0-9A-Fa-f]+)', ln)
            out.append((m[1], int(m[2], 16), int(m[3], 16), cur))
    return out


def main():
    matching, _ = configure_lists()
    rs = ranges(set(matching))
    if not rs:
        print('gen_c_symbols: no C objects linked (CR_MATCHING is empty): symbols.txt unchanged')
        return
    elf = ra.Elf(ELF)
    secname = {s[0]: s[1] for s in elf.sections}
    new = {}   # section -> [(addr, order, line)]
    for sec, lo, hi, obj in rs:
        for n, v, sz, t, b, shn in elf.syms:
            if secname.get(shn) != sec or not (lo <= v < hi) or t not in STT or not n or n.startswith('...'):
                continue
            if t == 0 and sz == 0:
                kind = 'type:label'
            else:
                kind = f'type:{STT[t]} size:0x{sz:X}'
            scope = {0: ' scope:local', 1: ' scope:global', 2: ' scope:weak'}.get(b, '')
            # at one address the sized symbol first, labels after it (mwld hangs otherwise: gen_dtk_symbols.py)
            new.setdefault(sec, []).append((v, 1 if t == 0 and sz == 0 else 0, f'{n} = {sec}:0x{v:08X}; // {kind}{scope}'))
    path = os.path.join(CFG, 'symbols.txt')
    old = open(path, 'rb').read().decode()
    out = []
    lines = old.split('\n')
    inside = lambda sec, a: any(s == sec and lo <= a < hi for s, lo, hi, _ in rs)
    pending = {sec: sorted(v) for sec, v in new.items()}
    before = {}   # (section, address) -> the name symbols.txt had there before (the first one at that address)
    for ln in lines:
        m = LINE.match(ln)
        if m:
            sec, a = m[2], int(m[3], 16)
            # first emit the C symbols that come before this line in its section
            p = pending.get(sec)
            while p and p[0][0] < a:
                out.append(p.pop(0)[2])
            if inside(sec, a):
                before.setdefault((sec, a, 'label' if 'type:label' in ln else 'sized'), m[1])
                continue
        out.append(ln)
    # C symbols after the last dtk symbol of their section
    leftover = [x for v in pending.values() for x in v]
    if leftover:
        # insert each after the last line of its section
        for sec, p in pending.items():
            idx = max((i for i, ln in enumerate(out) if (m := LINE.match(ln)) and m[2] == sec), default=len(out) - 1)
            for x in reversed(p):
                out.insert(idx + 1, x[2])
    # functions and global objects the C names differently from symbols.txt before (dtk default names: the matched C
    # is the proof of the real name; a name from our own files would mean they disagree) -> c_symbols_report.txt
    renamed = []
    for sec, v in sorted(new.items()):
        for a, lab, ln in sorted(v):
            nm = ln.split(' = ')[0]
            was = before.get((sec, a, 'label' if 'type:label' in ln else 'sized'))
            if was is not None and was != nm and ('type:function' in ln or 'scope:global' in ln):
                renamed.append(f'  {sec:8} 0x{a:08X}  {was} -> {nm}')
    rep = ['symbols from the objects built from C (tools/gen_c_symbols.py): main.elf\'s symbols inside each object\'s',
           'ranges replace those of decomp/config/GXCE01/symbols.txt there (same bytes; the C\'s names, sizes, scopes).',
           f'objects: {len(set(r[3] for r in rs))}; symbols written: {sum(len(v) for v in new.values())}',
           'functions and globals the C names differently from symbols.txt before it (a dtk default name: the matched C',
           'is the proof of the real name; one of our own names: the two disagree):'] + (renamed or ['  none'])
    open(os.path.join(D, 'reports', 'c_symbols_report.txt'), 'wb').write(('\r\n'.join(rep) + '\r\n').encode())
    text = '\n'.join(out)
    changed = text != old
    if changed:
        open(path, 'wb').write(text.encode())
    n = sum(len(v) for v in new.values())
    print(f'gen_c_symbols: {len(set(r[3] for r in rs))} C objects, {n} symbols from main.elf; '
          f'symbols.txt {"CHANGED" if changed else "unchanged"}')


if __name__ == '__main__':
    main()
