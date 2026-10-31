"""Batch 9: the promoted game files (decomp/src/game, tools/promote_drafts.py) follow our names before they are linked.

A promoted file calls / uses its neighbours by the name they had when batch 12 wrote it (fn_80198144). When a name
changes afterwards (a new layout-proof name, a hand name in symbols/symbols.txt: plan.md step 68b gave 0x80198144
the name HSD_JObjDispAll), the link of batch 9 fails on the old name ("undefined: 'fn_80198144'") before batch 12
can write the file again. So batch 9 first renames, in every file that still has promote_drafts.py's marker line:
an identifier that the previous config symbols.txt (the one before this run's gen_dtk_symbols.py) gives to an
address that the new one names differently gets the new name, when
  - the old name is a dtk automatic name (fn_ / lbl_ / ...) and the new one is not: the address got a name, or
  - neither is automatic: the name changed.
A real name that became automatic is left alone: before gen_c_symbols.py the new file lacks the names the C objects
define themselves, so that is no rename (and a name truly lost fails the link, as it should).
The compiled object is the same but for the symbol names of the renamed relocations; promote_drafts.py writes the
same file again in batch 12. Files without the marker (edited by hand) are never touched.
usage: python -I tools/sync_game_names.py <previous config symbols.txt>     (prints what it renamed)
"""
import os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAME = os.path.join(G, 'decomp/src/game')
CFG = os.path.join(G, 'decomp/config/GXCE01/symbols.txt')
MARK = "/* Custom Robo game code, promoted by tools/promote_drafts.py"
AUTO = re.compile(r'^(fn_|lbl_|jumptable_|@|gap_|FUN_|func_|sub_)')
SYM = re.compile(r'^(\S+) = (\.\w+):0x([0-9A-Fa-f]+);')
IDENT = re.compile(r'\b[A-Za-z_]\w*\b')


def load(p):
    """-> name -> (section, addr), (section, addr) -> name (the first name at an address)"""
    by_name, by_addr = {}, {}
    for l in open(p, encoding='utf-8'):
        m = SYM.match(l)
        if not m: continue
        k = (m.group(2), int(m.group(3), 16))
        by_name.setdefault(m.group(1), k)
        by_addr.setdefault(k, m.group(1))
    return by_name, by_addr


def main():
    if len(sys.argv) != 2: sys.exit(__doc__)
    if not os.path.isfile(sys.argv[1]):
        print('sync_game_names: no previous symbols.txt, nothing to rename'); return
    old, _ = load(sys.argv[1])
    _, new = load(CFG)
    renames = {}
    for n, k in old.items():
        m = new.get(k)
        if not m or m == n: continue
        if AUTO.match(n) and not AUTO.match(m) or not AUTO.match(n) and not AUTO.match(m):
            renames[n] = m
    changed = 0
    for f in sorted(os.listdir(GAME)) if os.path.isdir(GAME) else []:
        p = os.path.join(GAME, f)
        if not f.endswith('.c'): continue
        text = open(p, encoding='utf-8', newline='').read()
        if not text.startswith(MARK): continue
        ids = set(IDENT.findall(text))
        here = {n: m for n, m in renames.items() if n in ids}
        clash = [m for m in here.values() if m in ids]
        if clash: sys.exit(f'sync_game_names: decomp/src/game/{f} already uses {clash}: cannot rename {sorted(here)}')
        if not here: continue
        text = IDENT.sub(lambda x: here.get(x.group(0), x.group(0)), text)
        open(p, 'w', encoding='utf-8', newline='').write(text)
        changed += 1
        print(f'sync_game_names: decomp/src/game/{f}: ' + ', '.join(f'{n} -> {m}' for n, m in sorted(here.items())))
    print(f'sync_game_names: {changed} promoted game file(s) renamed to our current names')


if __name__ == '__main__':
    main()
