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
Writes symbols/layout_candidates.txt; batch1_names.ps1 appends it to symbols.txt.
usage: python -I tools/layout_match.py [--tol 0.10] [ref symbols.txt ...]   (default: Melee, then tools/refs/*)
"""
import bisect, glob, os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
args = sys.argv[1:]
TOL = 0.10
if '--tol' in args:
    i = args.index('--tol'); TOL = float(args[i + 1]); del args[i:i + 2]
REFS = args or [os.path.join(G, 'tools/melee/config/GALE01/symbols.txt')] + sorted(
    glob.glob(os.path.join(G, 'tools/refs/*/config/*/symbols.txt')))
MAX_GAP = 60
LIB_START = 0x80131000  # first SDK function; below this (after .init) is Custom Robo's own code
MANGLED = re.compile(r'^[^_].*__\w')
GENERIC = ('fn_', 'lbl_', 'jumptable_', '@', 'gap_', 'FUN_', 'func_', 'sub_')


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


cr = load(os.path.join(G, 'tools/dtk/proj/symbols.txt'))
ci = {a: i for i, (a, s, n) in enumerate(cr)}
verified = {}
for l in open(os.path.join(G, 'symbols/symbols.txt'), encoding='utf-8'):
    m = re.match(r'(\S+) = \.\w+:0x([0-9A-Fa-f]+);', l)
    if m and '// layout-proof' not in l: verified[int(m.group(2), 16)] = m.group(1)
found = {}  # addr -> (addr, size, name, ref size, lo anchor, hi anchor, ref tag)

for ref in REFS:
    ml = [x for x in load(ref) if not x[2].startswith(GENERIC)]
    counts = {}
    for x in ml: counts[x[2]] = counts.get(x[2], 0) + 1
    ml = [x for x in ml if counts[x[2]] == 1]  # static names used in several files (OnReset, ...) are ambiguous
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
                if prev_ok and next_ok and C[i][0] not in verified and not MANGLED.match(M[j][2]):
                    out.append((C[i][0], C[i][1], M[j][2], M[j][1], cr[c0][0], cr[c1][0], tag(ref)))
        used = set(verified.values())
        new = {}
        for t in out:
            if t[2] not in used: new.setdefault(t[0], t)
        names = [t[2] for t in new.values()]  # a name proposed for two addresses is dropped
        new = {a: t for a, t in new.items() if names.count(t[2]) == 1}
        if not new: break
        for a, t in new.items(): verified[a] = t[2]; found[a] = t
    print(f'{tag(ref)}: anchors {len(chain)}, total candidates so far {len(found)}')

with open(os.path.join(G, 'symbols/layout_candidates.txt'), 'w') as f:
    f.write('// layout-proof candidates: addr size | reference name, its size, which reference | anchored between\n')
    for a, s, nm, ms, lo, hi, rt in sorted(found.values()):
        f.write(f'{nm} = .text:0x{a:08X}; // type:function size:0x{s:X} ref:{rt} ref_size:0x{ms:X} '
                f'anchors:0x{lo:08X}-0x{hi:08X}\n')
print('candidates', len(found))
