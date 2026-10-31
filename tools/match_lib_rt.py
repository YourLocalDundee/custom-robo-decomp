"""Names for the Metrowerks runtime / MSL C / math / EXI / SI range 0x8015F000-0x8016D408 (naming policy rules 3, 5, 6).

R3 layout proof, per reference decomp: between two verified anchors (names in symbols.txt that exist exactly once in the
   reference and keep the same order in both binaries) align our unnamed functions with the reference's symbols by size
   (within 10%). A pair is used only when it is in EVERY maximal alignment (unambiguous), and
   - size is exactly equal and >= 0x40 in one game, or the same name is proposed by >= 2 different games
     (tp / tww / pikmin2 ... count as one game each, regardless of region or version), and
   - no two references propose different names for the address.
R5 call graph: not used when it cannot be shown uniquely (see symbols/lib_unnamed_rt.txt for each reason).
R6 unique string literal: a string used by exactly one function here and exactly one reference function.
Guards: no C++ mangled names, no name that occurs twice in one reference, no name already in symbols.txt.
Writes symbols/candidates_rt.txt and symbols/lib_unnamed_rt.txt. Deterministic. Reads symbols.txt including the lines batch6 already merged (tag "lib-candidate"), so it only
proposes names not yet merged: run it through tools/batches/batch6_lib_candidates.ps1 (rounds with the other matchers).
usage: python -I tools/match_lib_rt.py
"""
import bisect, glob, os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import export_struct  # noqa: E402  Ghidra export, structure only (no Ghidra names)
LO, HI = 0x8015F000, 0x8016D408
TOL = 0.10
MANGLED = re.compile(r'^[^_].*__\w')
GENERIC = ('fn_', 'lbl_', 'jumptable_', '@', 'gap_', 'FUN_', 'func_', 'sub_')
FN = re.compile(r'(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // type:function(?: size:0x([0-9A-Fa-f]+))?')


def load(p):
    r = []
    for l in open(p, encoding='utf-8'):
        m = FN.match(l)
        if m and m.group(4): r.append((int(m.group(3), 16), int(m.group(4), 16), m.group(1)))
    return sorted(r)


PLACEHOLDER = re.compile(r'(_[0-9a-fA-F]{6,8}$)|^(kar_|zz_|unk_|sub_|loc_|dat_)')


def placeholder(n): return n.startswith(GENERIC) or bool(PLACEHOLDER.search(n))


def close(a, b): return abs(a - b) <= TOL * b


def tag(p):
    game, cfg = os.path.relpath(p, G).replace(os.sep, '/').split('/config/')
    return game.split('/')[-1] + '/' + cfg.split('/')[0]


def game_of(t): return t.split('/')[0]


def align(C, M, banned=frozenset()):
    """max number of order-preserving size-matched pairs (and one such alignment)"""
    n, k = len(C), len(M)
    D = [[0] * (k + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        for j in range(k - 1, -1, -1):
            best = max(D[i + 1][j], D[i][j + 1])
            if (i, j) not in banned and close(C[i][1], M[j][1]):
                best = max(best, D[i + 1][j + 1] + (3 if C[i][1] == M[j][1] else 1))
            D[i][j] = best
    i = j = 0; pairs = []
    while i < n and j < k:
        if (i, j) not in banned and close(C[i][1], M[j][1]) and                 D[i][j] == D[i + 1][j + 1] + (3 if C[i][1] == M[j][1] else 1):
            pairs.append((i, j)); i += 1; j += 1
        elif D[i + 1][j] >= D[i][j + 1]: i += 1
        else: j += 1
    return D[0][0], pairs


cr = load(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
verified = {}  # addr -> name (from symbols.txt, not our own merged candidates)
layout_named = set()
for l in open(os.path.join(G, 'symbols/symbols.txt'), encoding='utf-8'):
    m = FN.match(l)
    if m:
        verified[int(m.group(3), 16)] = m.group(1)
        if '// layout-proof' in l: layout_named.add(int(m.group(3), 16))
descriptive = set()
for l in open(os.path.join(G, 'symbols/descriptive.txt'), encoding='utf-8'):
    m = FN.match(l)
    if m: descriptive.add(int(m.group(3), 16))
size_at = {a: s for a, s, n in cr}
# A layout-proof name whose neighbour in our binary has exactly the same size is not trusted as an anchor: runs of
# same-size functions (the 0x20-byte math wrappers, the 0x24 float wrappers) can be shifted by one by a size-only match.
suspect = set()
for i, (a, sz, n) in enumerate(cr):
    if a in layout_named and any(0 <= j < len(cr) and cr[j][1] == sz for j in (i - 1, i + 1)): suspect.add(a)
anchors = {a: n for a, n in verified.items() if a not in suspect}
REFS = [os.path.join(G, 'tools/melee/config/GALE01/symbols.txt')] + sorted(
    glob.glob(os.path.join(G, 'tools/refs/*/config/*/symbols.txt')), key=lambda p: p.replace('/', '\\'))   # Windows path order on every OS (ties -> the same reference)
refdata = []
for ref in REFS:
    ml = [x for x in load(ref) if not placeholder(x[2])]
    counts = {}
    for x in ml: counts[x[2]] = counts.get(x[2], 0) + 1
    allnames = set(x[2] for x in ml)
    ml = [x for x in ml if counts[x[2]] == 1]
    refdata.append((tag(ref), ml, {n: i for i, (a, s, n) in enumerate(ml)}))

REASON = {}
DEFAULT_REASON = ('no R3/R5/R6 evidence. Checked: layout between verified anchors in all references, sole-callee and caller-side '
                  'source match, game strings (Ghidra export strings of the function and its callers), docs/name_evidence.md, docs/name_tables.tsv')
cidx = {a: i for i, (a, _, _) in enumerate(cr)}
found = {}   # addr -> (name, rule, ref tag, why)
for rnd in range(20):
    props = {}  # addr -> {name: [(tag, ref size, exact, anchors)]}
    used = set(verified.values())
    for t, ml, mi in refdata:
        # runs of unnamed functions in our range
        i = 0
        while i < len(cr):
            a = cr[i][0]
            if a in anchors or not (LO <= a < HI): i += 1; continue
            j = i
            while j < len(cr) and cr[j][0] not in anchors and cr[j][0] < HI: j += 1
            C = cr[i:j]
            A = cr[i - 1] if i else None
            B = cr[j] if j < len(cr) else None
            i = j
            if not A or not B or A[0] not in anchors or B[0] not in anchors: continue
            na, nb = anchors[A[0]], anchors[B[0]]
            if na not in mi or nb not in mi or mi[na] >= mi[nb]: continue
            M = [x for x in ml[mi[na] + 1:mi[nb]] if x[2] not in used and not MANGLED.match(x[2])]
            if not M or len(M) > 4 * len(C) + 20: continue
            L, pairs = align(C, M)
            # exact-size evidence: the only function of this size in the gap on both sides, and part of a best alignment
            for (ci, mj) in pairs:
                same_c = [x for x in C if x[1] == C[ci][1]]
                same_m = [x for x in M if x[1] == M[mj][1]]
                if C[ci][1] == M[mj][1] and len(same_c) == 1 and len(same_m) == 1:
                    props.setdefault(C[ci][0], {}).setdefault(M[mj][2], []).append(
                        (t, M[mj][1], False, f'{na}..{nb}'))
            for (ci, mj) in pairs:
                L2, _ = align(C, M, frozenset([(ci, mj)]))
                if L2 == L: continue  # some other maximal alignment avoids this pair
                props.setdefault(C[ci][0], {}).setdefault(M[mj][2], []).append(
                    (t, M[mj][1], C[ci][1] == M[mj][1], f'{na}..{nb}'))
    new = {}
    for addr, byname in sorted(props.items()):
        if addr in verified: continue   # R3 never renames; wrong layout-proof names are handled by R5 below
        # a run of same-size functions (the 0x20 / 0x24 math wrappers) can be shifted by one by any size-based match:
        # such addresses are left to the call-graph rules (R5) below
        ix = cidx[addr]
        if size_at[addr] >= 0x10 and any(0 <= j < len(cr) and cr[j][1] == size_at[addr] for j in (ix - 1, ix + 1)): continue
        # references that disagree: only a name with EXACT size evidence while every other name has none is kept
        ex = {n: [e for e in ev if e[2] or e[2] is False and e[1] == size_at[addr]] for n, ev in byname.items()}
        ex = {n: v for n, v in ex.items() if v}
        if len(byname) == 1: name = next(iter(byname))
        elif len(ex) == 1: name = next(iter(ex))
        else: continue
        ev = byname[name]
        size = size_at[addr]
        games = sorted(set(game_of(e[0]) for e in ev))
        exact_games = sorted(set(game_of(e[0]) for e in ev if e[2]))
        if not ((size >= 0x40 and exact_games) or len(games) >= 2): continue
        if name in used or addr in descriptive: continue
        e = next((x for x in ev if x[1] == size), ev[0])
        new[addr] = (name, 'R3', e[0], f'same order between verified {e[3]}, size 0x{size:X} vs ref 0x{e[1]:X}; '
                     f'unambiguous alignment, {len(games)} game(s): {",".join(games)}')
    names = [v[0] for v in new.values()]
    new = {a: v for a, v in new.items() if names.count(v[0]) == 1}
    if not new: break
    for a, v in new.items(): verified[a] = v[0]; anchors[a] = v[0]; found[a] = v


# ---- R5: sole-callee wrappers (call graph + the reference's own source / symbol sizes)
con = export_struct.connect()
fstart = {a for a, _, _ in cr}


def callees(addr):
    return sorted({t for (t, k) in con.execute('SELECT target, kind FROM refs WHERE func=?', (addr,))
                   if (k == 'UNCONDITIONAL_CALL' or k == 'UNCONDITIONAL_JUMP') and t in fstart and t != addr})


def callers(addr):
    return sorted({f for (f, k) in con.execute('SELECT func, kind FROM refs WHERE target=?', (addr,))
                   if k == 'UNCONDITIONAL_CALL'})


refsyms = {}   # name -> {game: size} over every reference (unique names only)
for t, ml, mi in refdata:
    for a, sz, n in ml: refsyms.setdefault(n, {})[t] = sz
srcs = {}      # fdlibm wrapper source: name -> text, from reference source trees
for f in sorted(glob.glob(os.path.join(G, 'tools/refs/*/src/**/w_*.c'), recursive=True), key=lambda p: p.replace('/', '\\')):   # Windows path order on every OS (the first file is cited)
    srcs.setdefault(os.path.basename(f)[2:-2], []).append((f, open(f, encoding='utf-8', errors='ignore').read()))


def free_name(n, addr):  # not used at any other address
    return all(nm != n for a, nm in verified.items() if a != addr) and n not in [v[0] for a, v in found.items() if a != addr]


for _pass in range(2):  # second pass: wrappers of wrappers (powf calls pow)
    r5 = {}
    for a, sz, nm in cr:
        if a in found: continue
        if not (LO <= a < HI) or sz > 0x30: continue
        if a in verified and a not in layout_named: continue   # dtk / assert names are never replaced
        cs = callees(a)
        if len(cs) != 1: continue
        g = verified.get(cs[0]) or (found[cs[0]][0] if cs[0] in found else None)
        if not g: continue
        cand = None
        m = re.match(r'__ieee754_(\w+)$', g)
        if m and m.group(1) in srcs and m.group(1) in refsyms:
            x = m.group(1)
            okfiles = [os.path.relpath(f, G).replace(os.sep, '/') for f, txt in srcs[x]
                       if re.search(r'(?<!\w)%s\s*\([^)]*\)\s*\{[^}]*?(?<!\w)%s\s*\(' % (x, g), txt, re.S)]
            games = sorted(t for t, rs in refsyms[x].items() if close(sz, rs))
            if okfiles and len(set(game_of(t) for t in games)) >= 2:
                cand = (x, f'sole callee is {g} (0x{cs[0]:08X}); {okfiles[0]} defines {x}() as a call to {g}; '
                           f'{x} has size ~0x{sz:X} in {len(games)} reference versions')
        elif g in ('tan', 'sin', 'cos', 'pow'):
            x = g + 'f'
            if x in refsyms:
                games = sorted(t for t, rs in refsyms[x].items() if close(sz, rs))
                exact = [t for t in games if refsyms[x][t] == sz]
                if exact and (len(set(game_of(t) for t in games)) >= 2 or sz == 0x24):
                    cand = (x, f'sole callee is verified {g}; reference {x} is the same float wrapper size 0x{sz:X} '
                               f'in {len(games)} versions ({",".join(sorted(set(game_of(t) for t in games)))})')
        if cand and a in descriptive:
            REASON[a] = f'proven {cand[0]} by R5 ({cand[1]}) but the address already has a descriptive cr_ name (symbols/descriptive.txt, not edited here)'
        elif cand: r5[a] = (cand[0], 'R5', games[0], cand[1])
    names5 = [v[0] for v in r5.values()]
    for a, v in sorted(r5.items()):
        # the name may currently sit (wrongly) on a layout-proof line at ANOTHER address that is itself replaced
        other = [aa for aa, nm in verified.items() if nm == v[0] and aa != a]
        if names5.count(v[0]) != 1 or any(aa not in r5 for aa in other): continue
        why = v[3]
        if a in verified: why += f'; replaces:{verified[a]} (layout-proof pair was shifted by one in a run of same-size wrappers)'
        found[a] = (v[0], 'R5', v[2], why)


# ---- R5 (caller side): a verified caller whose reference source calls exactly one function our other callees do not explain
csrc = []
for f in sorted(glob.glob(os.path.join(G, 'tools/refs/*/src/**/*.c'), recursive=True), key=lambda p: p.replace('/', '\\')):   # Windows path order on every OS (ties -> the same reference)
    try: csrc.append((os.path.relpath(f, G).replace(os.sep, '/'), open(f, encoding='utf-8', errors='ignore').read()))
    except OSError: pass
KW = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else', 'do'}


def ref_calls(name):
    """{source file: set(identifiers called)} for every definition of `name` in the reference sources"""
    out = {}
    for f, txt in csrc:
        for m in re.finditer(r'(?<!\w)%s\s*\([^;{}]*\)\s*\{' % re.escape(name), txt):
            i, d = m.end(), 1
            while i < len(txt) and d:
                d += (txt[i] == '{') - (txt[i] == '}'); i += 1
            out[f] = set(x for x in re.findall(r'(\w+)\s*\(', txt[m.end():i]) if x not in KW)
    return out


for a, sz, nm in cr:
    if not (LO <= a < HI) or a in verified or a in found or a in descriptive: continue
    for c in callers(a):
        cname = verified.get(c) or (found[c][0] if c in found else None)
        if not cname or placeholder(cname): continue
        ours = callees(c)
        mine = [verified.get(x) or (found[x][0] if x in found else None) for x in ours]
        if any(m is None and x != a for m, x in zip(mine, ours)): continue   # another unexplained callee
        known = set(m for m in mine if m)
        rc = ref_calls(cname)
        if not rc: continue
        res = {f: ids - known - {cname} for f, ids in rc.items()}
        res = {f: r for f, r in res.items() if len(r) == 1}
        names = set(next(iter(r)) for r in res.values())
        if len(res) != len(rc) or len(names) != 1: continue
        x = names.pop()
        if placeholder(x) or x not in refsyms or x in set(verified.values()) | set(v[0] for v in found.values()): continue
        if not any(close(sz, rs) for rs in refsyms[x].values()): continue
        if sum(1 for cc in callers(a)) != 1 and False: continue
        found[a] = (x, 'R5', sorted(refsyms[x])[0], f'sole unexplained callee of verified {cname} (0x{c:08X}); '
                    f'{sorted(res)[0]} defines {cname}() calling {x}; {x} has size ~0x{sz:X} in '
                    f'{sum(1 for rs in refsyms[x].values() if close(sz, rs))} reference versions')
        break

# ---- outputs
with open(os.path.join(G, 'symbols/candidates_rt.txt'), 'w', encoding='utf-8', newline='\n') as f:
    for a in sorted(found):
        nm, rule, rt, why = found[a]
        f.write(f'{nm} = .text:0x{a:08X}; // type:function size:0x{size_at[a]:X} scope:global // {rule} ref:{rt} why: {why}\n')
cnt = {r: sum(1 for v in found.values() if v[1] == r) for r in ('R3', 'R5', 'R6')}
print('candidates', len(found), cnt)

rest = [(a, s_, n) for a, s_, n in cr if LO <= a < HI and (a not in verified or a in layout_named and a in found and False) and a not in found]
with open(os.path.join(G, 'symbols/lib_unnamed_rt.txt'), 'w', encoding='utf-8', newline='\n') as f:
    f.write('// still unnamed in 0x8015F000-0x8016D408 after tools/match_lib_rt.py: address size why\n')
    for a, sz, n in rest:
        f.write(f'0x{a:08X} 0x{sz:X} {n} // {REASON.get(a, DEFAULT_REASON)}\n')
print('still unnamed', len(rest))
