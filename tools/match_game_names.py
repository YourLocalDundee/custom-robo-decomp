"""Game-code naming, stage 1: the REAL names Custom Robo itself contains (no invented names).

Reads only main.dol ("Custom Robo extracted/sys/main.dol"), the dtk analysis (tools/dtk/proj/symbols.txt: function
starts and sizes) and our committed names (symbols/symbols.txt, symbols/descriptive.txt without its generated
`// game-real` lines, symbols/cr_globals.txt) - never the Ghidra export, decompiled/index.db or the decompiled C - so the
result is the same on every run, fresh or not, whatever names the last rebuild applied.

Code references come from the instruction bytes: per function, constant propagation over its control-flow graph
(tools/dol_asserts.py's PowerPC model: li / lis / addi / ori / mr, r2 / r13 from __init_registers); a block no branch
reaches (a switch case) starts from the entry state. A "reference" to X = an addi / addis / ori result equal to X, or
a load / store whose effective address is X.

Evidence kinds (each output line names its kind):
  G1 parallel tables. A name table (a maximal run of N >= 2 words in a data section, each pointing to a NUL-terminated
     printable ASCII string, all N strings distinct) directly adjacent to a function table (a maximal run of exactly N
     words, each a dtk function start), and proven to be indexed together:
       (a) one function references the bases of both tables, or
       (b) the name table has no code reference (debug data) and a function that references the function table's
           base also compares an index against N (cmpwi / cmplwi rX, N: the dispatcher's bound).
     Then function table[i] gets the name of string table[i] for every i where: the function occurs once in the table
     (a handler shared by two names gets no name), the string is a C identifier, and the result is not already used.
     Transform (step 44's convention): "cr_" + the string exactly as written (no case change, nothing removed).
     Also checked: {string pointer, function pointer} records (struct arrays, stride 8..0x40): none exist in main.dol.
  G2 self-naming strings. A string the function itself references, of the form "Name(): ..." (optionally after
     "Warning: " / "Error: "), where Name is a C identifier: the SDK / HSD style of messages that name the function
     printing them. Emitted only for a function with no name yet, when no other function references the string, Name is
     not already the name of another function, and the rule agrees with every already-named function it matches
     (precision reported). The weaker form "Name: ..." / "Name : ..." is only reported (it gives shortened names, e.g.
     "DoReadRegisters :" in TRKDoReadRegisters). Name -> "cr_" + Name.
  G3 source-file names. Strings "*.c" / "*.cpp" the function passes to __assert / HSD_Panic / OSPanic (r3 at the call)
     or references: written as module notes (`// G3 ...` lines), never as function names. Headers (*.h) are inlines
     from the library, not the caller's module: counted, not listed.

Writes
  symbols/game_real.txt            one line per name:  0xADDR = cr_Name // game-real G1|G2 <evidence>
                                   then the G3 module notes; CONFLICT lines (and exit 1) when a name would replace a
                                   different existing name, or reuse a name that exists at another address
  symbols/game_unnamed_real.txt    the leftovers: tables / strings that looked promising but are not proven, and why
tools/batches/batch7_game_names.ps1 merges the name lines into symbols/descriptive.txt.
usage: python -I tools/match_game_names.py
"""
import os, re, sys
from collections import defaultdict

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import dol_asserts as da  # noqa: E402  DOL reader + PowerPC constant propagation

P = lambda *a: os.path.join(G, *a)
DOL = P('Custom Robo extracted', 'sys', 'main.dol')
OUT = P('symbols', 'game_real.txt')
LEFT = P('symbols', 'game_unnamed_real.txt')
GAME_END = 0x80131000            # game code is 0x80005xxx .. here; the SDK starts at 0x80131000 (batch6)
SYMLINE = re.compile(r'^(\S+) = (\.\w+):0x([0-9A-Fa-f]{8});(.*)$')
IDENT = re.compile(r'[A-Za-z_][A-Za-z0-9_]*')
ASSERT_FUNCS = ('__assert', 'HSD_Panic', 'OSPanic')
G2_STRONG = re.compile(r'^(?:Warning: |Error: )?([A-Za-z_]\w*)\(\)\s*:')
G2_WEAK = re.compile(r'^([A-Za-z_]\w*) ?: ')
SRC = re.compile(r'^[\w./-]+\.(c|cpp|h)$')


class Dol(da.Dol):
    def __init__(self, path):
        super().__init__(path)
        self.secs = [(i, a, s) for i, (a, s) in enumerate(zip(self.adr, self.sz)) if s]

    def section(self, a):
        for i, b, s in self.secs:
            if b <= a < b + s: return i
        return None

    def string(self, a, ws=''):
        """printable ASCII (plus the characters in ws), NUL-terminated, in a data section (7..17), else None"""
        i = self.section(a) if a else None
        if i is None or i < 7: return None
        s = self.cstr(a)
        if not s or len(s) > 255 or not all(' ' <= c <= '~' or c in ws for c in s): return None
        return s

    def message(self, a):
        """a string that may contain newlines / tabs (OSReport-style messages); shown with them escaped"""
        s = self.string(a, '\n\t\r')
        return s.replace('\n', '\\n').replace('\t', '\\t').replace('\r', '\\r') if s else None


def load_names():
    """our committed names: {addr: name} for functions, {name: addr} for everything, minus generated game-real lines"""
    at, by = {}, {}
    for f in ('symbols.txt', 'descriptive.txt', 'cr_globals.txt'):
        for l in open(P('symbols', f), encoding='utf-8'):
            m = SYMLINE.match(l.rstrip('\r\n'))
            if not m or '// game-real ' in l: continue
            a = int(m.group(3), 16)
            if 'type:function' in m.group(4): at.setdefault(a, (m.group(1), f))
            by.setdefault(m.group(1), a)
    return at, by


def dtk_functions():
    fs = {}
    for l in open(P('tools', 'dtk', 'proj', 'symbols.txt'), encoding='utf-8'):
        m = re.match(r'^(\S+) = \.(?:text|init):0x([0-9A-Fa-f]{8}); // type:function size:0x([0-9A-Fa-f]+)', l)
        if m: fs[int(m.group(2), 16)] = (int(m.group(3), 16), m.group(1))
    return fs


def analyse(dol, f, size, entry):
    """-> (refs {value: [insn addr]}, calls [(insn addr, target, r3 or None)], cmp immediates {imm})"""
    end = f + size
    words = {a: dol.word(a) for a in range(f, end, 4)}
    if any(w is None for w in words.values()): return {}, [], set()
    lead = {f}
    for a, w in words.items():
        br = da._branch(w, a)
        if br is None: continue
        lead.update(t for t in br[0] if f <= t < end)
        if a + 4 < end: lead.add(a + 4)
    lead = sorted(lead)
    blocks, succ = {}, {}
    for i, b0 in enumerate(lead):
        b1 = lead[i + 1] if i + 1 < len(lead) else end
        blocks[b0] = b1
        br = da._branch(words[b1 - 4], b1 - 4)
        if br is None: succ[b0] = [b1] if b1 < end else []
        else: succ[b0] = [t for t in br[0] if f <= t < end] + ([b1] if br[1] and b1 < end else [])

    def run(b0, st):
        st = dict(st)
        for a in range(b0, blocks[b0], 4):
            br = da._branch(words[a], a)
            if br is None: da._step(words[a], st)
            elif br[2]: da._clobber(st)
        return st

    state = {b: None for b in blocks}
    state[f] = dict(entry)
    seeds = [f]
    while seeds:
        todo = list(seeds)
        while todo:
            b = todo.pop(0)
            out = run(b, state[b])
            for t in succ[b]:
                m = da._merge(state[t], out)
                if m != state[t]:
                    state[t] = m
                    if t not in todo: todo.append(t)
        seeds = [b for b in lead if state[b] is None]   # unreached (switch cases): start from the entry state
        for b in seeds: state[b] = dict(entry)
    refs, calls, cmps = defaultdict(list), [], set()
    for b0 in lead:
        st = dict(state[b0])
        for a in range(b0, blocks[b0], 4):
            w = words[a]
            op, rd, ra, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xffff
            br = da._branch(w, a)
            if br is not None:
                if br[2] and w >> 26 == 18:
                    t = w & 0x3fffffc
                    t = (t - 0x4000000 if t & 0x2000000 else t)
                    t = t if w & 2 else (a + t) & 0xffffffff
                    r3 = st.get(3, da.VARY)
                    calls.append((a, t, None if r3 is da.VARY else r3))
                if br[2]: da._clobber(st)
                continue
            if op in (10, 11): cmps.add(imm if op == 10 else da._s16(imm))
            if 32 <= op <= 55 and ra and st.get(ra, da.VARY) is not da.VARY:     # D-form load / store
                refs[(st[ra] + da._s16(imm)) & 0xffffffff].append(a)
            da._step(w, st)
            if op in (14, 15) and st.get(rd, da.VARY) is not da.VARY: refs[st[rd]].append(a)
            if op == 24 and st.get(ra, da.VARY) is not da.VARY: refs[st[ra]].append(a)
    return refs, calls, cmps


def runs(dol, funcs):
    """maximal runs of string pointers ('S') and function pointers ('F') in the data sections: [(kind, start, n)]"""
    out = []
    for i, b, s in dol.secs:
        if i < 7: continue
        cur = None
        for a in range(b, b + s - 3, 4):
            w = dol.word(a)
            k = 'F' if w in funcs else ('S' if dol.string(w) is not None else None)
            if cur and cur[0] == k: cur[2] += 1
            else:
                if cur and cur[0] and cur[2] >= 2: out.append(tuple(cur))
                cur = [k, a, 1]
        if cur and cur[0] and cur[2] >= 2: out.append(tuple(cur))
    return out


def main():
    dol = Dol(DOL)
    funcs = dtk_functions()
    names_at, names_by = load_names()
    init = next((a, s) for a, (s, n) in funcs.items() if n == '__init_registers')
    entry = da.small_data_bases(dol, init)
    assert_at = {names_by[n] for n in ASSERT_FUNCS if n in names_by}
    if len(assert_at) != len(ASSERT_FUNCS) or set(entry) != {2, 13}:
        sys.exit('match_game_names.py: __assert / HSD_Panic / OSPanic (symbols.txt) or the small-data bases not found')

    refd = defaultdict(set)            # address -> functions referencing it
    fref, fcalls, fcmp = {}, {}, {}
    for f in sorted(funcs):
        r, c, k = analyse(dol, f, funcs[f][0], entry)
        fref[f], fcalls[f], fcmp[f] = r, c, k
        for v in r: refd[v].add(f)

    lines, left, conflicts, agree = [], [], [], []
    taken = dict(names_by)             # name -> address, grows as names are emitted

    def emit(kind, addr, name, ev):
        old = names_at.get(addr)
        if old and old[0] == name and old[1] == 'descriptive.txt':
            agree.append(f'{addr:08X} {name}')   # the same hand name: emitted, batch 7 takes the hand line over
        elif old:
            conflicts.append(f'CONFLICT {kind} 0x{addr:08X}: {name} would replace {old[0]} ({old[1]}) // {ev}'); return 'conflict'
        elif name in taken and taken[name] != addr:
            conflicts.append(f'CONFLICT {kind} 0x{addr:08X}: {name} already names 0x{taken[name]:08X} // {ev}'); return 'conflict'
        taken[name] = addr
        lines.append(f'0x{addr:08X} = {name} // game-real {kind} {ev}')
        return 'new'

    # ---------------------------------------------------------------- G1
    rr = runs(dol, funcs)
    S = [r for r in rr if r[0] == 'S']
    F = [r for r in rr if r[0] == 'F']
    g1_tables = []
    for _, sa, sn in S:
        strs = [dol.string(dol.word(sa + 4 * i)) for i in range(sn)]
        head = f'name table 0x{sa:08X} ({sn} strings: {", ".join(repr(x) for x in strs[:3])}{" ..." if sn > 3 else ""})'
        adj = [(fa, fn) for _, fa, fn in F if fa == sa + 4 * sn or fa + 4 * fn == sa]
        if not adj:
            users = sorted(refd.get(sa, ()))
            left.append(f'G1 {head}: no function table directly before or after it'
                        + (f'; referenced by {", ".join(f"0x{u:08X}" for u in users)}' if users else '; no code reference'))
            continue
        for fa, fn in adj:
            if fn != sn:
                left.append(f'G1 {head}: adjacent function table 0x{fa:08X} has {fn} entries, not {sn} (counts differ: not paired)')
                continue
            if len(set(strs)) != sn:
                left.append(f'G1 {head}: strings not distinct'); continue
            fusers, susers = refd.get(fa, set()), refd.get(sa, set())
            both = sorted(fusers & susers)
            bound = sorted(u for u in fusers if sn in fcmp[u])
            if both: proof = 'one function references both bases: ' + ', '.join(f'0x{u:08X}' for u in both)
            elif not susers and bound: proof = (f'name table has no code reference; {len(bound)} function(s) referencing the function table '
                                                f'compare an index with {sn}: ' + ', '.join(f'0x{u:08X}' for u in bound))
            else:
                left.append(f'G1 {head}: function table 0x{fa:08X} ({fn}) adjacent, but no function references both and '
                            f'no dispatcher compares an index with {sn}'); continue
            g1_tables.append((sa, fa, sn, proof))
    g1 = defaultdict(int)
    table_notes = []
    for sa, fa, n, proof in g1_tables:
        ws = [dol.word(fa + 4 * i) for i in range(n)]
        count = defaultdict(int)
        for w in ws: count[w] += 1
        st = defaultdict(int)
        for i in range(n):
            s_addr = dol.word(sa + 4 * i)
            s = dol.string(s_addr)
            f = ws[i]
            ev = f'names:0x{sa:08X}[{i}]@0x{s_addr:08X} funcs:0x{fa:08X}[{i}] "{s}"'
            if count[f] > 1:
                left.append(f'G1 0x{f:08X} "{s}": function table 0x{fa:08X}[{i}] - the function is at {count[f]} indexes, so no single name'); st['shared'] += 1; continue
            if not IDENT.fullmatch(s):
                left.append(f'G1 0x{f:08X} "{s}": table 0x{fa:08X}[{i}] - the string is not a C identifier (no transform beyond the cr_ prefix)'); st['not-ident'] += 1; continue
            res = emit('G1', f, 'cr_' + s, ev)
            st['named' if res == 'new' else res] += 1
            if res == 'new': g1['new'] += 1
        table_notes.append(f'// G1 names 0x{sa:08X} + functions 0x{fa:08X}, {n} entries, adjacent; proof: {proof}; '
                           + ', '.join(f'{k} {v}' for k, v in sorted(st.items())))

    # struct arrays {string pointer, function pointer}: none expected; report any found as leftovers. Every record has a
    # function pointer, so start from the function pointers in data: 3 at a constant stride, a string pointer at one offset
    fptr = sorted(a for i, b, s in dol.secs if i >= 7 for a in range(b, b + s - 3, 4) if dol.word(a) in funcs)
    fset, seen = set(fptr), set()
    for p in fptr:
        for stride in range(8, 0x44, 4):
            if p in seen or p + stride not in fset or p + 2 * stride not in fset: continue
            for d in range(-stride + 4, stride, 4):
                if d and all(dol.string(dol.word(p + k * stride + d)) is not None for k in range(3)):
                    n = 3
                    while p + n * stride in fset and dol.string(dol.word(p + n * stride + d)) is not None: n += 1
                    seen.update(p + k * stride for k in range(n))
                    left.append(f'G1 struct array at 0x{p + min(d, 0):08X}: stride 0x{stride:X}, {n} records of (function pointer, '
                                f'string pointer at {d:+d}): not handled, check by hand')
                    break

    # ---------------------------------------------------------------- G2 / G3
    g2_new, g2_check, g3 = 0, [], []
    g3_headers = 0
    for f in sorted(funcs):
        own = sorted({v for v in fref[f] if dol.string(v) is not None})
        for v in sorted({v for v in fref[f] if dol.message(v) is not None}):
            s = dol.message(v)
            m = G2_STRONG.match(s)
            weak = None if m else G2_WEAK.match(s)
            if m or weak:
                ident = (m or weak).group(1)
                if not re.search(r'[a-z]', ident) or not re.search(r'[A-Z_]', ident) or len(ident) < 4: continue
                named = names_at.get(f)
                others = sorted(refd[v] - {f})
                rec = f'0x{f:08X} string 0x{v:08X} "{s[:60]}"'
                if named:
                    g2_check.append((ident == named[0], f'{"agrees" if ident == named[0] else "differs"}: {rec} vs our name {named[0]}', bool(m)))
                elif not m:
                    left.append(f'G2 {rec}: weak form "{ident}:" (gives shortened names), not used for naming')
                elif others:
                    left.append(f'G2 {rec}: string also referenced by ' + ', '.join(f'0x{o:08X}' for o in others))
                elif ident in names_by or ident in taken:
                    left.append(f'G2 {rec}: "{ident}" is already the name of another function')
                else:
                    g2_new += 1 if emit('G2', f, 'cr_' + ident, f'string:0x{v:08X} "{s[:60]}"') == 'new' else 0
        files = set()
        for a, t, r3 in fcalls[f]:
            if t in assert_at and r3 is not None and dol.string(r3) and SRC.match(dol.string(r3)): files.add((dol.string(r3), r3, 'assert'))
        for v in own:
            if SRC.match(dol.string(v)) and not any(x[1] == v for x in files): files.add((dol.string(v), v, 'ref'))
        for name, v, how in sorted(files):
            if name.endswith('.h'): g3_headers += 1; continue
            nm = names_at.get(f, (funcs[f][1], 'dtk'))[0]
            g3.append(f'// G3 0x{f:08X} {name} ({"passed to an assert/panic call" if how == "assert" else "referenced"}, string 0x{v:08X}) - {nm}')
    g2_strong = [c for c in g2_check if c[2]]
    precision = f'{sum(1 for c in g2_strong if c[0])}/{len(g2_strong)}'
    for ok, txt, strong in g2_check:
        if not ok: left.append(f'G2 check ({"strong" if strong else "weak"} form) {txt}')
    if any(not c[0] for c in g2_strong):
        # the strong form named an already-named function differently: the rule is not reliable, emit nothing
        lines[:] = [l for l in lines if ' G2 ' not in l]; g2_new = 0
        left.append('G2: strong form disagreed with an existing name - no G2 names emitted this run')

    left.append('G1 note: sequence handlers (name table 0x801DEAB4, five 15-entry tables indexed by FUN_8000878c / FUN_800055e8) '
                'and camera-mode handlers (0x801F88A4 18 names vs 0x801F7FF0 21 [start, frame] pairs) are named by hand in '
                'symbols/descriptive.txt (plan.md steps 42 and 45): the role suffix (_Enter, _FrameA, _Start, ...) is ours, '
                'so they are not regenerated here; tools/crosscheck_names.py checks their table entries every run')

    summary = (f'game_real: G1 {g1["new"]} names from {len(g1_tables)} table pair(s), G2 {g2_new} names '
               f'(strong-form precision on named functions {precision}), G3 {len(g3)} module notes ({g3_headers} header '
               f'strings not listed); {len(conflicts)} conflicts, {len(left)} leftovers')
    with open(OUT, 'w', encoding='utf-8', newline='\r\n') as fo:
        fo.write('// ' + summary + '\n')
        fo.write('// generated by tools/match_game_names.py (method in its docstring); merged into symbols/descriptive.txt by '
                 'tools/batches/batch7_game_names.ps1\n')
        fo.write('// format: 0xADDR = cr_Name // game-real G1 names:<name table>[i]@<string> funcs:<function table>[i] "<string>"\n')
        for c in conflicts: fo.write(c + '\n')
        for t in table_notes: fo.write(t + '\n')
        for l in lines: fo.write(l + '\n')
        for l in g3: fo.write(l + '\n')
    with open(LEFT, 'w', encoding='utf-8', newline='\r\n') as fo:
        fo.write('// real-name leads in main.dol not used by tools/match_game_names.py (not proven), with the reason\n')
        for l in left: fo.write(l + '\n')
    print(summary)
    if agree: print(f'{len(agree)} of these names are already hand lines in symbols/descriptive.txt (same name, same address): taken over')
    for c in conflicts: print(c)
    return 1 if conflicts else 0


if __name__ == '__main__':
    sys.exit(main())
