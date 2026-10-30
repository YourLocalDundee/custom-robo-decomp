"""Layout-proof name candidates (naming policy rule 3), from one or more reference decomps.

Between two consecutive verified anchors (our name == reference name, same order in both games),
align the unnamed functions to the reference's verified symbols by size (DP). A candidate is kept
only when its size is within --tol of the reference size and both neighbours are also matched
(or are the anchors). Accepted names become anchors and the search repeats until nothing new is
found, then moves to the next reference. Guards against matching game code by chance:
  - a gap that reaches into game code (below LIB_START) is skipped when longer than MAX_GAP functions;
    gaps fully inside the SDK / HSD library range may be any length
  - C++ mangled names (foo__5ClassFv) are never proposed: the SDK and HSD are C
  - names that occur twice in one reference (file-static names like OnReset) are never proposed
Ambiguity rule (a run of same-size functions, e.g. 0x20 math wrappers, lets a size-only alignment shift by one):
  a pair (ours i, reference j) is "forced" when it is the only optimal pair at its level of the alignment, i.e.
  every alignment with the maximal number of size matches pairs ours i with reference j. A pair that is not forced
  is only emitted when tools/layout_evidence.py confirms the name by call graph / strings against every alternative
  (the other optimal partners, the reference neighbours +-4 in every reference); otherwise it is dropped.
  Without the Ghidra export decompiled/xrefs (fresh checkout) only forced pairs are emitted.
Writes symbols/layout_candidates.txt; batch1_names.ps1 appends it to symbols.txt.
usage: python -I tools/layout_match.py [--tol 0.10] [--permissive] [ref symbols.txt ...]   (default: Melee, then tools/refs/*)
  --permissive  the old rule (every pair, forced or not); used by tools/audit_layout.py, not by batch1
"""
import bisect, glob, os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import layout_evidence as E  # noqa: E402

TOL = 0.10
MAX_GAP = 60
LIB_START = 0x80131000  # first SDK function; below this (after .init) is Custom Robo's own code
MANGLED = re.compile(r'^[^_].*__\w')
GENERIC = ('fn_', 'lbl_', 'jumptable_', '@', 'gap_', 'FUN_', 'func_', 'sub_')
NAMES_ONLY_IF_ELSEWHERE = '/kar_wowjinxy/'  # reference whose own names need a second reference
SHIFT = 4  # reference neighbours considered as alternatives of a non-forced pair


def load(p):
    r = []
    for l in open(p, encoding='utf-8'):
        m = re.match(r'(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)', l)
        if m: r.append((int(m.group(3), 16), int(m.group(4), 16), m.group(1)))
    return sorted(r)


def close(a, b): return abs(a - b) <= TOL * b  # b = reference size, same rule as verify_symbols.py


def tag(p):
    game, cfg = os.path.relpath(p, G).replace(os.sep, '/').split('/config/')
    return game.split('/')[-1] + '/' + cfg.split('/')[0]


def default_refs():
    return [os.path.join(G, 'tools/melee/config/GALE01/symbols.txt')] + sorted(
        glob.glob(os.path.join(G, 'tools/refs/*/config/*/symbols.txt')), key=lambda p: p.replace('/', '\\'))   # Windows path order on every OS (ties -> the same reference)


def ref_lists(refs):
    out = []
    for ref in refs:
        ml = [x for x in load(ref) if not x[2].startswith(GENERIC)]
        counts = {}
        for x in ml: counts[x[2]] = counts.get(x[2], 0) + 1
        out.append((ref, [x for x in ml if counts[x[2]] == 1]))  # static names used in several files are ambiguous
    # wowjinxy/KAR invents names (e.g. HSD_SListAppend for Melee's HSD_SListAllocAndAppend): its map still anchors
    # positions and sizes, but a name from it is only used when another reference has the same name
    others = {x[2] for r, ml in out if NAMES_ONLY_IF_ELSEWHERE not in r.replace(os.sep, '/') for x in ml}
    out = [(r, [x if NAMES_ONLY_IF_ELSEWHERE not in r.replace(os.sep, '/') or x[2] in others else (x[0], x[1], '@' + x[2])
                for x in ml]) for r, ml in out]
    return out


def name_positions(lists):
    pos = {}   # name -> [(list index, position)]
    for li, (ref, ml) in enumerate(lists):
        for j, x in enumerate(ml): pos.setdefault(x[2], []).append((li, j))
    return pos


def pool_names(size, name, p, lists, pos, used):
    """Alternatives of name for a function of this size: p plus the neighbours +-SHIFT of name in every reference,
    keeping only names of a close size in some reference that are not used elsewhere."""
    p = set(p)
    for li, jj in pos.get(name, ()):
        l2 = lists[li][1]
        p |= {l2[x][2] for x in range(max(0, jj - SHIFT), min(len(l2), jj + SHIFT + 1))}
    ok = set()
    for r, l2 in lists:
        for x in l2:
            if x[2] in p and close(size, x[1]): ok.add(x[2])
    return sorted(x for x in p if x == name or (x in ok and x not in used and not MANGLED.match(x)))


def run(refs=None, permissive=False, log=print, evidence=True, banned=frozenset(), extra_pool=None, trust=None):
    """-> (found, verified0). found: addr -> dict(addr size name ref_size lo hi ref forced shaky pool status why)
    permissive: accept every pair like the old rule, but still tag forced / shaky (anchored on a non-forced name).
    banned: (addr, name) pairs never proposed. extra_pool: addr -> more alternative names.
    trust: addr -> name, extra names trusted as call-graph evidence only (the previous round's result), not anchors."""
    refs = refs or default_refs()
    cr = load(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
    ci = {a: i for i, (a, s, n) in enumerate(cr)}
    verified = {}
    for l in open(os.path.join(G, 'symbols/symbols.txt'), encoding='utf-8'):
        m = re.match(r'(\S+) = \.\w+:0x([0-9A-Fa-f]+);', l)
        # anchors and evidence: the committed names only. Our own layout-proof output and batch6's lib candidates are
        # neither (batch6 runs after this and builds on our result), so the result never depends on an earlier run
        if m and '// layout-proof' not in l and '// lib-candidate' not in l: verified[int(m.group(2), 16)] = m.group(1)
    verified0 = dict(verified)
    evid = dict(verified)   # names trusted as call-graph evidence (and ruling out alternatives)
    lists = ref_lists(refs)
    pos = name_positions(lists)
    ours = E.Ours() if evidence else None
    defs = E.source_defs() if ours and ours.ok else None
    found = {}
    shaky = set()  # addresses whose name came from a non-forced pair or from a gap bounded by one

    def pool_for(size, name, ml, alt_js, j):
        p = {ml[x][2] for x in alt_js}
        p |= {ml[x][2] for x in range(max(0, j - SHIFT), min(len(ml), j + SHIFT + 1))}
        return pool_names(size, name, p, lists, pos, set(verified.values()))

    for ref, ml in lists:
        mi = {}
        for i, (a, s, n) in enumerate(ml): mi.setdefault(n, i)
        while True:
            anc = sorted((ci[a], mi[n]) for a, n in verified.items() if n in mi and a in ci)
            # longest chain of anchors in the same order in both binaries (LIS on the reference index)
            tails, idx, prev = [], [], [-1] * len(anc)
            for p_, (c, m) in enumerate(anc):
                t = bisect.bisect_left(tails, m)
                if t == len(tails): tails.append(m); idx.append(p_)
                else: tails[t] = m; idx[t] = p_
                prev[p_] = idx[t - 1] if t else -1
            chain, p_ = [], idx[-1] if idx else -1
            while p_ >= 0: chain.append(anc[p_]); p_ = prev[p_]
            chain.reverse()

            out = []
            for (c0, m0), (c1, m1) in zip(chain, chain[1:]):
                C = cr[c0 + 1:c1]; M = ml[m0 + 1:m1]
                if not C or not M or len(M) > 4 * len(C) + 20: continue
                if len(C) > MAX_GAP and C[0][0] < LIB_START: continue
                n, k = len(C), len(M)
                cl = [[close(C[i][1], M[j][1]) for j in range(k)] for i in range(n)]
                D = [[0] * (k + 1) for _ in range(n + 1)]   # D[i][j]: best of C[i:], M[j:]
                for i in range(n - 1, -1, -1):
                    Di, Dn, ci_ = D[i], D[i + 1], cl[i]
                    for j in range(k - 1, -1, -1):
                        best = Dn[j] if Dn[j] > Di[j + 1] else Di[j + 1]
                        if ci_[j] and Dn[j + 1] + 1 > best: best = Dn[j + 1] + 1
                        Di[j] = best
                F = [[0] * (k + 1) for _ in range(n + 1)]   # F[i][j]: best of C[:i], M[:j]
                for i in range(1, n + 1):
                    Fi, Fp, ci_ = F[i], F[i - 1], cl[i - 1]
                    for j in range(1, k + 1):
                        best = Fp[j] if Fp[j] > Fi[j - 1] else Fi[j - 1]
                        if ci_[j - 1] and Fp[j - 1] + 1 > best: best = Fp[j - 1] + 1
                        Fi[j] = best
                opt = D[0][0]
                level = {}   # level -> optimal pairs (every optimal alignment has exactly one pair per level)
                alt = {}     # ours i -> reference js of optimal pairs
                for i in range(n):
                    for j in range(k):
                        if cl[i][j] and F[i][j] + 1 + D[i + 1][j + 1] == opt:
                            level.setdefault(F[i][j] + 1, []).append((i, j)); alt.setdefault(i, []).append(j)
                i = j = 0; pairs = []
                while i < n and j < k:
                    if cl[i][j] and D[i][j] == D[i + 1][j + 1] + 1:
                        pairs.append((i, j)); i += 1; j += 1
                    elif D[i + 1][j] >= D[i][j + 1]: i += 1
                    else: j += 1
                ps = set(pairs)
                gap_shaky = cr[c0][0] in shaky or cr[c1][0] in shaky
                for i, j in pairs:
                    prev_ok = (i == 0 and j == 0) or (i - 1, j - 1) in ps
                    next_ok = (i == n - 1 and j == k - 1) or (i + 1, j + 1) in ps
                    if prev_ok and next_ok and C[i][0] not in verified and not MANGLED.match(M[j][2]) and not M[j][2].startswith('@') and                             (C[i][0], M[j][2]) not in banned:
                        forced = len(level[F[i][j] + 1]) == 1
                        out.append(dict(addr=C[i][0], size=C[i][1], name=M[j][2], ref_size=M[j][1], lo=cr[c0][0],
                                        hi=cr[c1][0], ref=tag(ref), forced=forced, shaky=gap_shaky or not forced,
                                        alt_js=[m0 + 1 + x for x in alt[i]], j=m0 + 1 + j))
            used = set(verified.values())
            new = {}
            for t in out:
                if t['name'] not in used: new.setdefault(t['addr'], t)
            names = [t['name'] for t in new.values()]  # a name proposed for two addresses is dropped
            new = {a: t for a, t in new.items() if names.count(t['name']) == 1}
            acc = {}
            for a, t in sorted(new.items()):
                t['pool'] = pool_for(t['size'], t['name'], ml, t.pop('alt_js'), t.pop('j'))
                if extra_pool and a in extra_pool: t['pool'] = sorted(set(t['pool']) | set(extra_pool[a]))
                t['right'] = None
                if defs is not None:
                    # evidence uses every name accepted so far (and the previous round's forced ones); an alternative is only
                    # ruled out by a non-circular name elsewhere: verified or forced (a confirmed pair must not confirm
                    # its same-size sibling by taking the other name away)
                    tr = dict(trust or {})
                    tr.update(evid)
                    tr.update(verified)
                    tr = {k: v for k, v in tr.items() if k != a and (v != t['name'] or k in verified)}
                    st, t['right'], why = E.judge(ours, defs, a, t['name'], t['pool'], tr, t['shaky'], taken=evid)
                    # a forced pair stays unless evidence names another function (WRONG); a contradiction alone
                    # (e.g. a caller's source does not use it: other SDK version, inlining) is kept and logged
                    if not t['shaky'] and st in ('UNPROVEN', 'CONTRADICTED'):
                        st, why = 'FORCED', 'unique optimal alignment; ' + why
                    t['status'], t['why'] = st, why
                elif not t['shaky']:
                    t['status'], t['why'] = 'FORCED', 'unique optimal alignment (no Ghidra export: no evidence)'
                else:
                    t['status'], t['why'] = 'UNPROVEN', 'no Ghidra export: evidence not available'
                # FORCED / CONFIRMED are emitted; WRONG / CONTRADICTED / UNPROVEN are not
                if permissive or t['status'] in ('FORCED', 'CONFIRMED'): acc[a] = t
            if not acc: break
            for a, t in acc.items():
                verified[a] = t['name']; found[a] = t
                if t['status'] == 'FORCED': evid[a] = t['name']
                if permissive and t['shaky'] and t['status'] != 'CONFIRMED': shaky.add(a)
        if log: log(f'{tag(ref)}: anchors {len(chain)}, total candidates so far {len(found)}')
    return found, verified0


NEAR = 6  # an immediate neighbour's name must lie within this many functions in a reference


def coherence(found, verified0, refs=None, ignore=frozenset()):
    """Neighbour check on the result: for a name N whose immediate neighbours (ours) are named P / Q, every reference
    that has N and P must place P before N within NEAR functions (Q after N), else the side is incoherent.
    A name is incoherent when every checkable side is. Names at the addresses in ignore count as unnamed.
    -> (incoherent addrs, addr -> names between P and Q in the refs)"""
    lists = ref_lists(refs or default_refs())
    pos = [{x[2]: j for j, x in enumerate(ml)} for r, ml in lists]
    cr = load(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
    names = dict(verified0)
    names.update({a: t['name'] for a, t in found.items() if a not in ignore})
    bad, between = set(), {}
    for i, (a, s, _) in enumerate(cr):
        if a not in found: continue
        n = found[a]['name']
        p = names.get(cr[i - 1][0]) if i else None
        q = names.get(cr[i + 1][0]) if i + 1 < len(cr) else None
        sides = []
        for nb, sign in ((p, 1), (q, -1)):
            if not nb: continue
            both = [d for d in pos if n in d and nb in d]
            if both: sides.append(any(0 < sign * (d[n] - d[nb]) <= NEAR for d in both))
        if sides and not any(sides): bad.add(a)
        if p and q:
            b = set()
            for (r, ml), d in zip(lists, pos):
                if p in d and q in d and 0 < d[q] - d[p] <= 2 * NEAR:
                    b |= {ml[j][2] for j in range(d[p] + 1, d[q]) if close(s, ml[j][1])}
            if b: between[a] = sorted(b)
    return bad, between


def validate(found, verified0):
    """Re-check every CONFIRMED name of the final result inductively: evidence may use only verified names, forced
    layout names and names re-confirmed before (to a fixpoint), so no two ambiguous names confirm each other across
    rounds. -> addresses whose confirmation does not hold."""
    ours = E.Ours()
    if not ours.ok: return set()
    defs = E.source_defs()
    base = dict(verified0)
    base.update({a: t['name'] for a, t in found.items() if t['status'] == 'FORCED'})
    todo = {a for a, t in found.items() if t['status'] == 'CONFIRMED'}
    ok = {}
    while True:
        tr = dict(base); tr.update(ok)
        new = {a for a in sorted(todo - set(ok))
               if E.judge(ours, defs, a, found[a]['name'], found[a]['pool'], {k: v for k, v in tr.items() if k != a},
                          True, taken=base)[0] == 'CONFIRMED'}
        if not new: break
        ok.update({a: found[a]['name'] for a in new})
    return todo - set(ok)


def match(refs=None, log=print):
    """Strict rule, then the neighbour check (drop incoherent names unless confirmed) and the inductive validation of
    every confirmed name (validate); each drop bans that (addr, name) and reruns. The previous round's forced names are
    trusted as evidence. After the last round any name still failing validation is left out."""
    banned, extra, trust = set(), {}, {}
    for _ in range(10):
        found, verified0 = run(refs, False, log, banned=frozenset(banned), extra_pool=extra, trust=trust)
        bad, between = coherence(found, verified0, refs)
        for a, b in between.items(): extra[a] = sorted(set(extra.get(a, ())) | set(b))
        bad = {a for a in bad if found[a]['status'] != 'CONFIRMED'}
        # a name only incoherent next to another incoherent one may be fine: drop those incoherent on their own first
        alone, _ = coherence(found, verified0, refs, ignore=frozenset(bad))
        drop = {(a, found[a]['name']) for a in (alone & bad or bad)} - banned
        why = 'incoherent neighbours'
        if not drop:
            drop = {(a, found[a]['name']) for a in validate(found, verified0)} - banned
            why = 'not confirmed from verified + forced names alone'
            if not drop: return found, verified0, banned
        if log: log(f'{why}: {len(drop)} name(s) dropped, rerun: ' + ', '.join(f'{n}@0x{a:08X}' for a, n in sorted(drop)))
        banned |= drop
        # the next round trusts this round's forced names as evidence (a name confirmed by a callee found later);
        # never its confirmed names: two ambiguous names would confirm each other (A by caller B, B by callee A)
        trust = {a: t['name'] for a, t in found.items() if t['status'] == 'FORCED'}
    left = validate(found, verified0)
    if log and left: log(f'after the last round {len(left)} unvalidated name(s) left out: ' +
                         ', '.join(f"{found[a]['name']}@0x{a:08X}" for a in sorted(left)))
    return {a: t for a, t in found.items() if a not in left}, verified0, banned


def main():
    global TOL
    args = sys.argv[1:]
    if '--tol' in args:
        i = args.index('--tol'); TOL = float(args[i + 1]); del args[i:i + 2]
    permissive = '--permissive' in args
    args = [a for a in args if a != '--permissive']
    if permissive: found, _ = run(args or None, True)
    else: found, _, banned = match(args or None)
    with open(os.path.join(G, 'symbols/layout_candidates.txt'), 'w', newline='\r\n') as f:   # CRLF on every OS (checksummed)
        f.write('// layout-proof candidates: addr size | reference name, its size, which reference | anchored between\n')
        f.write('// rule: forced alignment (unique optimal), or a non-forced pair confirmed by tools/layout_evidence.py\n')
        for a in sorted(found):
            t = found[a]
            if t['status'] == 'CONFIRMED':
                f.write(f"// evidence {t['name']} 0x{a:08X}: {t['why']}\n")
            f.write(f"{t['name']} = .text:0x{a:08X}; // type:function size:0x{t['size']:X} ref:{t['ref']} "
                    f"ref_size:0x{t['ref_size']:X} anchors:0x{t['lo']:08X}-0x{t['hi']:08X}\n")
    st = {}
    for t in found.values(): st[t['status']] = st.get(t['status'], 0) + 1
    print('candidates', len(found), ' '.join(f'{k}={v}' for k, v in sorted(st.items())))


if __name__ == '__main__':
    main()
