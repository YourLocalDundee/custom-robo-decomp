"""Name the remaining SDK library functions in 0x80131000..0x8015F000 (OS, DVD, VI, PAD, AI/AR/ARQ/DSP,
CARD, GX, MTX, TRK) from reference decomps, with independent evidence only.

Rules (each proposal says which one gave it):
  R3 layout proof (same as tools/layout_match.py): between two consecutive verified anchors in the same
     order in both binaries, the unnamed functions are aligned with the reference's named functions by
     size (DP); a candidate needs size within TOL of the reference size and both neighbours matched too
     (or being the anchors). Accepted names become anchors; repeat to a fixpoint over all references.
  R5 call graph: the function's verified callees and callers (>= 2 distinct verified neighbours in
     total, each neighbour a verified name) match exactly one reference function, i.e. exactly one
     function in the reference sources calls every one of those callees and is called by every one of
     those callers; that function must be defined in the same reference source file (translation unit)
     as at least one of the neighbours.
  R6 unique string: the function references a string literal that occurs in exactly one function in the
     reference sources (OSReport / OSPanic text, ASSERT expression), and in no other of our functions.
Sanity for every rule: size ratio within 0.5..2.0 of the reference size when the reference has a size,
name unique in that reference, never C++ mangled, name not already used by another of our functions,
and a name proposed for two addresses is dropped.
Iterates R3 -> R5 -> R6 until nothing new is found.

Reads only repo / reference files. Writes symbols/candidates_sdk.txt (symbols.txt format) and
symbols/lib_unnamed_sdk.txt (every function in the range still unnamed, with the reason).
usage: python -I tools/match_lib_sdk.py
References: tools/melee + tools/refs/*/config/*/symbols.txt (sizes) and their src trees (C text).
"""
import bisect, glob, os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import export_struct  # noqa: E402  Ghidra export, structure only (no Ghidra names)
LO, HI = 0x80131000, 0x8015F000
TOL = 0.10
LIB_START = 0x80131000
MAX_GAP = 60
MANGLED = re.compile(r'^[^_].*__\w')
GENERIC = ('fn_', 'lbl_', 'jumptable_', '@', 'gap_', 'FUN_', 'func_', 'sub_', 'zz_', 'kar_', 'xd_', 'DAT_', 'LAB_')
PLACEHOLDER = re.compile(r'[0-9A-Fa-f]{6,8}$')  # map names that end in an address are placeholders
REFS = [os.path.join(G, 'tools/melee/config/GALE01/symbols.txt')] + sorted(
    glob.glob(os.path.join(G, 'tools/refs/*/config/*/symbols.txt')), key=lambda p: p.replace('/', '\\'))   # Windows path order on every OS (ties -> the same reference)


def load(p):
    r = []
    for l in open(p, encoding='utf-8', errors='replace'):
        m = re.match(r'(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)', l)
        if m: r.append((int(m.group(3), 16), int(m.group(4), 16), m.group(1)))
    return sorted(r)


def tag(p):
    game, cfg = os.path.relpath(p, G).replace(os.sep, '/').split('/config/')
    return game.split('/')[-1] + '/' + cfg.split('/')[0]


def close(a, b): return abs(a - b) <= TOL * b


cr = load(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
ci = {a: i for i, (a, s, n) in enumerate(cr)}
size_of = {a: s for a, s, n in cr}
verified = {}
for l in open(os.path.join(G, 'symbols/symbols.txt'), encoding='utf-8'):
    m = re.match(r'(\S+) = \.\w+:0x([0-9A-Fa-f]+);.*type:function', l)
    if m: verified[int(m.group(2), 16)] = m.group(1)
base_names = set(verified.values())
found = {}  # addr -> (name, rule, reftag, why)

refsyms = {}  # tag -> list
MULTI = set()
for ref in REFS:
    ml = [x for x in load(ref) if not x[2].startswith(GENERIC) and not PLACEHOLDER.search(x[2])]
    counts = {}
    for x in ml: counts[x[2]] = counts.get(x[2], 0) + 1
    refsyms[tag(ref)] = [x for x in ml if counts[x[2]] == 1]
    for n, c in counts.items():
        if c > 1: MULTI.add(n)
# a static name that some reference has in two translation units (WriteCallback, EraseCallback, OnReset) is
# never proposed, even from a reference that has it only once
refsyms = {t: [x for x in ml if x[2] not in MULTI] for t, ml in refsyms.items()}
# reference size per name: list of (tag, size)
# Maps of games whose SDK names were partly assigned by hand (Kirby Air Ride wowjinxy/KAR, Pokemon XD):
# a name from them is used only if another reference game also has a function of that name.
CROSSCHECK = ('kar_wowjinxy/', 'xd/')
_games = {}
for t, ml in refsyms.items():
    for a, s, n in ml: _games.setdefault(n, set()).add(t.split('/')[0])
for t in list(refsyms):
    if t.startswith(CROSSCHECK):
        g = t.split('/')[0]
        refsyms[t] = [x for x in refsyms[t] if _games[x[2]] - {g} - {c.rstrip('/') for c in CROSSCHECK}]
refsize = {}
for t, ml in refsyms.items():
    for a, s, n in ml: refsize.setdefault(n, []).append((t, s))


def accept(new):
    """new: addr -> (name, rule, tag, why). Drop names used twice / already used; returns accepted count."""
    used = set(verified.values())
    names = [v[0] for v in new.values()]
    k = 0
    for a, v in sorted(new.items()):
        if a in verified or v[0] in used or v[0] in MULTI or names.count(v[0]) != 1 or MANGLED.match(v[0]): continue
        rs = [s for t, s in refsize.get(v[0], [])]
        if rs and not any(0.5 <= size_of[a] / s <= 2.0 for s in rs): continue
        verified[a] = v[0]; found[a] = v; k += 1
    return k


def r3():
    total = 0
    for t, ml in refsyms.items():
        mi = {}
        for i, (a, s, n) in enumerate(ml): mi.setdefault(n, i)
        while True:
            anc = sorted((ci[a], mi[n]) for a, n in verified.items() if n in mi and a in ci)
            tails, idx, prev = [], [], [-1] * len(anc)
            for p_, (c, m) in enumerate(anc):
                q = bisect.bisect_left(tails, m)
                if q == len(tails): tails.append(m); idx.append(p_)
                else: tails[q] = m; idx[q] = p_
                prev[p_] = idx[q - 1] if q else -1
            chain, p_ = [], idx[-1] if idx else -1
            while p_ >= 0: chain.append(anc[p_]); p_ = prev[p_]
            chain.reverse()
            new = {}
            for (c0, m0), (c1, m1) in zip(chain, chain[1:]):
                C = cr[c0 + 1:c1]; M = ml[m0 + 1:m1]
                if not C or not M or len(M) > 4 * len(C) + 20: continue
                if len(C) > MAX_GAP and C[0][0] < LIB_START: continue
                n, k = len(C), len(M)
                D = [[0] * (k + 1) for _ in range(n + 1)]
                for i in range(n - 1, -1, -1):
                    for j in range(k - 1, -1, -1):
                        best = max(D[i + 1][j], D[i][j + 1])
                        if close(C[i][1], M[j][1]): best = max(best, D[i + 1][j + 1] + 1)
                        D[i][j] = best
                i = j = 0; pairs = []
                while i < n and j < k:
                    if close(C[i][1], M[j][1]) and D[i][j] == D[i + 1][j + 1] + 1:
                        pairs.append((i, j)); i += 1; j += 1
                    elif D[i + 1][j] >= D[i][j + 1]: i += 1
                    else: j += 1
                ps = set(pairs)
                for i, j in pairs:
                    prev_ok = (i == 0 and j == 0) or (i - 1, j - 1) in ps
                    next_ok = (i == n - 1 and j == k - 1) or (i + 1, j + 1) in ps
                    a = C[i][0]
                    if prev_ok and next_ok and a not in verified and LO <= a < HI:
                        new.setdefault(a, (M[j][2], 'R3', t,
                                           f'size 0x{C[i][1]:X} vs ref 0x{M[j][1]:X}, between {verified[cr[c0][0]]} and {verified[cr[c1][0]]}'))
            got = accept(new)
            total += got
            if not got: break
    return total


def align(C, M):
    n, k = len(C), len(M)
    D = [[0] * (k + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        for j in range(k - 1, -1, -1):
            best = max(D[i + 1][j], D[i][j + 1])
            if close(C[i][1], M[j][1]): best = max(best, D[i + 1][j + 1] + 1)
            D[i][j] = best
    i = j = 0; pairs = []
    while i < n and j < k:
        if close(C[i][1], M[j][1]) and D[i][j] == D[i + 1][j + 1] + 1:
            pairs.append((i, j)); i += 1; j += 1
        elif D[i + 1][j] >= D[i][j + 1]: i += 1
        else: j += 1
    ps = set(pairs)
    return [(i, j) for i, j in pairs
            if ((i == 0 and j == 0) or (i - 1, j - 1) in ps) and ((i == n - 1 and j == k - 1) or (i + 1, j + 1) in ps)]


def r3_local():
    """R3 on each run of unnamed functions between its two nearest verified neighbours (L, R) in our
    binary, when L and R also appear in that order in a reference (global anchor chains drop anchors when
    the link order of a whole library differs between games, e.g. AX placed before CARD)."""
    total = 0
    for t, ml in refsyms.items():
        mi = {}
        for i, (a, s, n) in enumerate(ml): mi.setdefault(n, i)
        while True:
            new = {}
            i = 0
            idx = [k for k, (a, s, n) in enumerate(cr) if LO <= a < HI]
            k0 = idx[0]
            k = k0
            while k <= idx[-1]:
                if cr[k][0] in verified: k += 1; continue
                e = k
                while e < len(cr) and cr[e][0] not in verified: e += 1
                L, R = cr[k - 1][0], cr[e][0] if e < len(cr) else None
                C = cr[k:e]
                k = e
                if R is None or L not in verified or verified[L] not in mi or verified[R] not in mi: continue
                m0, m1 = mi[verified[L]], mi[verified[R]]
                M = ml[m0 + 1:m1]
                if m1 <= m0 or not M or len(M) > 4 * len(C) + 20: continue
                for i, j in align(C, M):
                    a = C[i][0]
                    if LO <= a < HI and a not in verified:
                        new.setdefault(a, (M[j][2], 'R3', t,
                                           f'size 0x{C[i][1]:X} vs ref 0x{M[j][1]:X}, run between {verified[L]} and {verified[R]}'))
            got = accept(new)
            total += got
            if not got: break
    return total


# ---------------------------------------------------------------- reference C sources (text only)
# C sources: matching decomps and the SDK source decomps (dolsdk2001/2004). Not kar_wowjinxy / kar: their SDK
# code is lumped into one file per library (src/Dolphin/ax.c, card.c, gx.c), so file = translation unit fails.
SRC_SKIP = ('kar_wowjinxy', 'kar')
SRC_ROOTS = [os.path.join(G, 'tools/melee/src')] + [p for p in sorted(glob.glob(os.path.join(G, 'tools/refs/*/src')), key=lambda p: p.replace('/', '\\')) + sorted(
    glob.glob(os.path.join(G, 'tools/refs/*/libs')), key=lambda p: p.replace('/', '\\')) if os.path.basename(os.path.dirname(p)) not in SRC_SKIP]   # Windows path order on every OS (ties -> the same reference)
HEAD = re.compile(r'(\w+)\s*\(([^;{}]*)\)\s*$')
KEYW = {'if', 'for', 'while', 'switch', 'return', 'sizeof', 'do', 'else'}
STR = re.compile(r'"((?:[^"\\\n]|\\.)*)"')
IDENT = re.compile(r'\b([A-Za-z_]\w*)\b')


def parse_c(path, defs):
    txt = open(path, encoding='utf-8', errors='replace').read()
    txt = re.sub(r'/\*.*?\*/', ' ', txt, flags=re.S)
    txt = re.sub(r'//[^\n]*', ' ', txt)
    txt = re.sub(r'^[ \t]*#[^\n]*(\\\n[^\n]*)*', ' ', txt, flags=re.M)
    depth, start, last = 0, 0, 0
    i, n = 0, len(txt)
    while i < n:
        ch = txt[i]
        if ch == '"':
            j = i + 1
            while j < n and txt[j] != '"':
                j += 2 if txt[j] == '\\' else 1
            i = j + 1; continue
        if ch == "'":
            j = i + 1
            while j < n and txt[j] != "'":
                j += 2 if txt[j] == '\\' else 1
            i = j + 1; continue
        if ch == '{':
            if depth == 0: start = i; head = txt[last:i]
            depth += 1
        elif ch == '}':
            depth -= 1
            if depth == 0:
                m = HEAD.search(head)
                if m and m.group(1) not in KEYW and '=' not in head.split('(')[0]:
                    body = txt[start:i + 1]
                    d = defs.setdefault(m.group(1), {'files': set(), 'ids': set(), 'strs': set()})
                    d['files'].add(os.path.basename(path).lower())
                    d['strs'].update(STR.findall(body))
                    d['ids'].update(IDENT.findall(STR.sub('""', body)))
                last = i + 1
            if depth < 0: depth = 0
        elif ch == ';' and depth == 0:
            last = i + 1
        i += 1


TREES = {}  # reference name -> {function name: {'files', 'ids', 'strs'}}; each reference decomp kept apart
for root in SRC_ROOTS:
    tree = os.path.relpath(root, G).replace(os.sep, '/').split('/')[-2]
    defs = TREES.setdefault(tree, {})
    for dp, dn, fn in sorted(os.walk(root)):
        for f in sorted(fn):
            if f.endswith('.c'): parse_c(os.path.join(dp, f), defs)
# simple identifier aliases from the reference headers (#define VECCrossProduct PSVECCrossProduct): a body that
# mentions the alias also mentions every function it may stand for (both the C_ and PS variants, conservative)
DEFINE = re.compile(r'^[ 	]*#[ 	]*define[ 	]+([A-Za-z_]\w*)[ 	]+([A-Za-z_]\w*)[ 	]*$', re.M)
for root in SRC_ROOTS:
    tree = os.path.relpath(root, G).replace(os.sep, '/').split('/')[-2]
    alias = {}
    for inc in (root, os.path.join(os.path.dirname(root), 'include')):
        for dp, dn, fn in sorted(os.walk(inc)):
            for f in sorted(fn):
                if f.endswith(('.h', '.c')):
                    for x, y in DEFINE.findall(open(os.path.join(dp, f), encoding='utf-8', errors='replace').read()):
                        alias.setdefault(x, set()).add(y)
    for d in TREES[tree].values():
        d['ids'] |= {y for x in list(d['ids']) for y in alias.get(x, ())}
for defs in TREES.values():
    for nm, d in defs.items(): d['ids'].discard(nm)

# ---------------------------------------------------------------- our call graph (Ghidra xrefs, by address)
db = export_struct.connect()
starts = set(size_of)
out_e, in_e = {}, {}
for f, t, kind in db.execute("select func, target, kind from refs where kind in "
                             "('UNCONDITIONAL_CALL','COMPUTED_CALL','DATA','UNCONDITIONAL_JUMP')"):
    if t in starts and t != f:
        out_e.setdefault(f, set()).add(t); in_e.setdefault(t, set()).add(f)
our_strs = {}
str_users = {}
strings = dict(db.execute('select addr, value from strings'))
for f, t in db.execute("select func, target from refs where kind in ('READ','DATA','PARAM')"):
    if t in strings:
        our_strs.setdefault(f, set()).add(t); str_users.setdefault(t, set()).add(f)


def ok_size(a, nm):
    rs = [s for t, s in refsize.get(nm, [])]
    return bool(rs) and any(0.5 <= size_of[a] / s <= 2.0 for s in rs), rs


def r5_tree(defs, a, s):
    callees = sorted({verified[x] for x in out_e.get(a, ()) if x in verified and verified[x] in defs})
    callers = sorted({verified[x] for x in in_e.get(a, ()) if x in verified and verified[x] in defs})
    if len(set(callees) | set(callers)) < 2: return None
    # the candidate must mention every verified callee, and every verified caller must mention the candidate
    cand = None
    for c in callers:
        u = {x for x in defs[c]['ids'] if x in defs}
        cand = u if cand is None else cand & u
    if cand is None: cand = set(defs)
    cand = {g for g in cand if all(x in defs[g]['ids'] for x in callees)}
    cand = {g for g in cand if not g.startswith(GENERIC) and not MANGLED.match(g)}
    cand -= set(verified.values())
    if len(cand) != 1: return None
    g = next(iter(cand))
    files = defs[g]['files']
    if len(files) != 1: return None  # static name defined in several translation units
    nb_files = set().union(*(defs[x]['files'] for x in callees + callers))
    if not (files & nb_files): return None  # not in the same translation unit as any neighbour
    return g, next(iter(files)), callees, callers


def r5():
    new = {}
    for a, s, _ in cr:
        if not (LO <= a < HI) or a in verified: continue
        hits = {}
        for tree, defs in sorted(TREES.items()):
            h = r5_tree(defs, a, s)
            if h: hits.setdefault(h[0], []).append((tree,) + h[1:])
        if len(hits) != 1: continue  # no reference, or two references disagree
        g, hl = next(iter(hits.items()))
        okz, rs = ok_size(a, g)
        if not okz: continue
        tree, fl, callees, callers = hl[0]
        new[a] = (g, 'R5', tree + '/' + fl,
                  f'only {g} ({fl}) uses ' + (','.join(callees) or '-') + ' and is used by ' +
                  (','.join(callers) or '-') + f' (refs agreeing: {len(hl)}); size 0x{s:X} vs ref 0x{min(rs, key=lambda r: abs(r - s)):X}')
    return accept(new)


def r6():
    new = {}
    for a, s, _ in cr:
        if not (LO <= a < HI) or a in verified: continue
        hits = set()
        for sa in our_strs.get(a, ()):
            v = strings[sa]
            if len(v) < 8 or str_users.get(sa) != {a}: continue
            for tree, defs in sorted(TREES.items()):
                o = [nm for nm, d in defs.items() if v in d['strs']]
                if len(o) == 1 and len(defs[o[0]]['files']) == 1:
                    hits.add((o[0], tree + '/' + next(iter(defs[o[0]]['files'])), sa, v))
        names = {h[0] for h in hits}
        if len(names) != 1: continue
        g = names.pop()
        if g.startswith(GENERIC) or MANGLED.match(g): continue
        okz, rs = ok_size(a, g)
        if not okz: continue
        h = sorted(hits)[0]
        new[a] = (g, 'R6', h[1],
                  f'string 0x{h[2]:08X} "{h[3][:50]}" occurs only in {g}; size 0x{s:X} vs ref 0x{min(rs, key=lambda r: abs(r - s)):X}')
    return accept(new)



# ---------------------------------------------------------------- the game's own data (R6, game string)
# Pointers to a function stored in main.dol data sections (function tables): if a neighbouring word (+-8 bytes)
# points to a string that is exactly a reference function name, that string names it. Also strings in the
# function itself or in its callers of the form "Name(): ..." / "Name: ..." with Name a reference function name.
DOL = open(os.path.join(G, 'Custom Robo extracted', 'sys', 'main.dol'), 'rb').read()
_h = [int.from_bytes(DOL[i:i + 4], 'big') for i in range(0, 216, 4)]
SECTIONS = [(_h[i], _h[18 + i], _h[36 + i]) for i in range(18) if _h[36 + i]]  # file offset, address, size
DATA_PTRS = {}  # target -> [address of the word]
for i, (off, addr, sz) in enumerate(SECTIONS):
    if addr < 0x801B65E0: continue  # .text / .init: code, not data
    for k in range(0, sz - 3, 4):
        w = int.from_bytes(DOL[off + k:off + k + 4], 'big')
        if LO <= w < HI: DATA_PTRS.setdefault(w, []).append(addr + k)


def dol_word(va):
    for off, addr, sz in SECTIONS:
        if addr <= va < addr + sz - 3: return int.from_bytes(DOL[off + va - addr:off + va - addr + 4], 'big')


ALLNAMES = set(refsize) | {n for defs in TREES.values() for n in defs}
NAMEFMT = re.compile(r'^\s*([A-Za-z_]\w+)\s*(?:\(\s*\))?\s*:')


def game_strings(a):
    """name candidates from the game's own strings: [(name, string address, how)]"""
    out = []
    for p in DATA_PTRS.get(a, ()):
        for dv in (-8, -4, 4, 8):
            t = dol_word(p + dv)
            if t in strings and strings[t] in ALLNAMES: out.append((strings[t], t, f'table word 0x{p:08X}'))
    for sa in our_strs.get(a, ()):
        m = NAMEFMT.match(strings[sa])
        if m and m.group(1) in ALLNAMES: out.append((m.group(1), sa, 'own string'))
    return out


def r6_game():
    new = {}
    for a, s, _ in cr:
        if not (LO <= a < HI) or a in verified: continue
        hits = game_strings(a)
        names = {h[0] for h in hits}
        if len(names) != 1: continue
        g = names.pop()
        okz, rs = ok_size(a, g)
        if not okz or g.startswith(GENERIC) or MANGLED.match(g): continue
        h = hits[0]
        new[a] = (g, 'R6', 'game', f'game string 0x{h[1]:08X} "{strings[h[1]][:40]}" ({h[2]}); size 0x{s:X} vs ref 0x{min(rs, key=lambda r: abs(r - s)):X}')
    return accept(new)


NAMEDOCS = ''.join(open(os.path.join(G, 'docs', f), encoding='utf-8', errors='replace').read()
                   for f in ('name_tables.tsv', 'name_evidence.md') if os.path.exists(os.path.join(G, 'docs', f)))


def why_not(a):
    callees = sorted({verified[x] for x in out_e.get(a, ()) if x in verified})
    callers = sorted({verified[x] for x in in_e.get(a, ()) if x in verified})
    ss = [strings[x][:30] for x in our_strs.get(a, ())]
    cs = sorted({strings[t][:30] for c in in_e.get(a, ()) for t in our_strs.get(c, ())})[:4]
    gs = [h[0] for h in game_strings(a)]
    k = ci[a]
    lo = next((verified[cr[i][0]] for i in range(k - 1, -1, -1) if cr[i][0] in verified), '-')
    hi = next((verified[cr[i][0]] for i in range(k + 1, len(cr)) if cr[i][0] in verified), '-')
    return (f'between {lo} and {hi}: no reference run of the same sizes (R3); verified neighbours callees={",".join(callees) or "-"} '
            f'callers={",".join(callers) or "-"} not unique in reference sources (R5); own strings={ss or "none"}, '
            f'caller strings={cs or "none"}, data pointers to it={len(DATA_PTRS.get(a, ()))}, game name strings={gs or "none"} (R6); '
            f'docs/name_tables.tsv + docs/name_evidence.md: {"entry - check" if f"{a:08X}" in NAMEDOCS or f"{a:08x}" in NAMEDOCS else "no entry"}')


if __name__ == '__main__':
    while True:
        n3 = r3() + r3_local()
        n5 = r5()
        n6 = r6() + r6_game()
        print('R3', n3, 'R5', n5, 'R6', n6)
        if not (n3 or n5 or n6): break
    with open(os.path.join(G, 'symbols/candidates_sdk.txt'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('// SDK library names 0x80131000-0x8015F000 from tools/match_lib_sdk.py (rules R3/R5/R6, see its header)\n')
        for a in sorted(found):
            nm, rule, rt, why = found[a]
            f.write(f'{nm} = .text:0x{a:08X}; // type:function size:0x{size_of[a]:X} scope:global // {rule} ref:{rt} why: {why}\n')
    with open(os.path.join(G, 'symbols/lib_unnamed_sdk.txt'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('// functions in 0x80131000-0x8015F000 still unnamed after tools/match_lib_sdk.py: addr size | why no evidence\n')
        for a, s, _ in cr:
            if LO <= a < HI and a not in verified:
                f.write(f'0x{a:08X} 0x{s:X} | {why_not(a)}\n')
    print('proposed', len(found), 'by rule', {r: sum(1 for v in found.values() if v[1] == r) for r in ('R3', 'R5', 'R6')},
          'still unnamed', sum(1 for a, s, _ in cr if LO <= a < HI and a not in verified))
