"""Fit list-scheduler policies to mwcc's output. usage: python -I fit.py file.c[:fn] ..."""
import os, re, subprocess, sys, itertools, difflib
G = r"D:\emulators\gameTesting-hsd2"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
ver, flags = db.game_flags()
drafts = os.path.join(db.D, "drafts")


def compile_(cfile, fn, opt):
    fl = list(flags); i = fl.index('-opt'); fl[i + 1] = opt
    o = os.path.splitext(cfile)[0] + ('.sched.o' if 'schedule' in opt else '.nosched.o')
    cmd = db.mwcc_cmd(ver) + ['-c'] + fl + db.CTX_DEFS + ['-DCR_DRAFT_CC', '-i', drafts.replace(os.sep, '/')] + sum([['-i', i] for i in db.CTX_INC], []) + ['-o', o, cfile.replace(os.sep, '/')]
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=db.D, errors="replace")
    if r.returncode:
        print("COMPILE FAILED", r.stdout, r.stderr); sys.exit(1)
    s = o + ".s"
    subprocess.run([os.path.join(G, "tools", "dtk", "dtk.exe"), "elf", "disasm", o, s], check=True, capture_output=True)
    txt = open(s, encoding="utf-8", errors="replace").read()
    m = re.search(r"^\.fn (%s)\b.*?^\.endfn" % (re.escape(fn) if fn else r'\w+'), txt, re.S | re.M)
    return [l.split("*/", 1)[1].strip() for l in m.group(0).splitlines() if l.startswith("/*")]


def reg(a):
    m = re.fullmatch(r'r(\d+)', a)
    return int(m.group(1)) if m else None


def parse(ins):
    op, _, rest = ins.partition(' ')
    args = [a.strip() for a in rest.split(',')] if rest else []
    d = {'op': op, 'defs': set(), 'uses': set(), 'mem': None, 'txt': ins}
    if op == 'stwu':
        d['defs'].add(1); d['uses'].add(1); d['mem'] = ('S', 'stack', 'r1', 0, False)
    elif op in ('lwz', 'lha', 'lhz', 'lbz', 'stw', 'sth', 'stb', 'lfs', 'stfs'):
        kind = 'L' if op[0] == 'l' else 'S'
        m = re.fullmatch(r'(.+)\((r\d+)\)', args[1])
        off, base = m.group(1), reg(m.group(2))
        if '@sda21' in off:
            d['mem'] = (kind, 'sym', off.split('@')[0], 0, False)
        elif base == 1:
            d['mem'] = (kind, 'stack', 'r1', int(off, 16), False); d['uses'].add(1)
        else:
            d['mem'] = (kind, 'reg', base, int(off, 16), False); d['uses'].add(base)
        (d['defs'] if kind == 'L' else d['uses']).add(reg(args[0]))
    elif op == 'blr':
        pass
    else:
        d['defs'].add(reg(args[0]))
        for a in args[1:]:
            r = reg(a)
            if r is not None:
                d['uses'].add(r)
    return d


def build(pre):
    N = [parse(t) for t in pre]
    val = {}
    cur = {}
    for i, n in enumerate(N):
        n['pred'] = set()
        for u in n['uses']:
            if u in cur:
                n['pred'].add(cur[u])
        if n['mem'] and n['mem'][1] == 'reg':
            k, _, base, off, _ = n['mem']
            v = val.get(base)
            if v and v[0] == 'sym':
                n['mem'] = (k, 'sym', v[1], off, v[2])
            else:
                n['mem'] = (k, 'ptr', 'r%d' % base, off, False)
        for dd in n['defs']:
            cur[dd] = i
            if n['op'] == 'lis':
                val[dd] = ('sym', n['txt'].split(',')[1].strip().replace('@ha', ''), False)
            elif n['op'] == 'addi' and '@l' in n['txt']:
                src = int(re.search(r', r(\d+),', n['txt']).group(1)); v = val.get(src); val[dd] = v if v else ('tmp',)
            elif n['op'] in ('add', 'addi'):
                vs = [val.get(s) for s in sorted(n['uses'])]
                sym = [v for v in vs if v and v[0] == 'sym']
                val[dd] = (sym[0][0], sym[0][1], True) if sym else ('tmp',)
                if 14 <= dd <= 31:
                    val[dd] = ('local', dd)
            else:
                val[dd] = ('local', dd) if 14 <= dd <= 31 else ('tmp',)
    return N


def alias(a, b):
    if a[1] == 'ptr' or b[1] == 'ptr':
        return True
    if a[1] != b[1]:
        return False
    if a[1] == 'stack':
        return a[3] == b[3]
    if a[2] != b[2]:
        return False
    if a[4] or b[4]:
        return True
    return a[3] == b[3]


def edges(N, stack_barrier):
    E = {i: set() for i in range(len(N))}
    for i, n in enumerate(N):
        for p in n['pred']:
            E[p].add(i)
        for j in range(i):
            m = N[j]
            if (m['uses'] & n['defs']) or (m['defs'] & n['defs']):
                E[j].add(i)
            if n['mem'] and m['mem']:
                if n['mem'][0] == 'L' and m['mem'][0] == 'L':
                    continue
                am, an = m['mem'], n['mem']
                if am[1] == 'stack' or an[1] == 'stack':
                    if stack_barrier or (am[1] == 'stack' and an[1] == 'stack' and am[3] == an[3]):
                        E[j].add(i)
                    continue
                if alias(am, an):
                    E[j].add(i)
        if n['op'] == 'blr':
            for j in range(i):
                E[j].add(i)
    return E


def simulate(N, E, pol):
    def lat(n):
        return pol['L'] if n['mem'] and n['mem'][0] == 'L' else 1
    n = len(N)
    h = [0] * n
    for i in reversed(range(n)):
        h[i] = lat(N[i]) + max([h[s] for s in E[i]], default=0)
    preds = {i: set() for i in range(n)}
    for i in E:
        for s in E[i]:
            preds[s].add(i)
    done = {}
    order = []
    cyc = 0
    while len(order) < n and cyc < 10 * n:
        issued = 0; lsu = 0
        cand = [i for i in range(n) if i not in done and all(p in done and done[p] + (lat(N[p]) if pol['readylat'] else 1) <= cyc for p in preds[i])]
        cand.sort(key=lambda i: (-h[i], i if pol['tie'] == 'pre' else -i))
        for i in cand:
            if issued >= pol['W']:
                break
            if N[i]['mem'] and lsu >= pol['LSU']:
                continue
            order.append(i); done[i] = cyc; issued += 1
            if N[i]['mem']:
                lsu += 1
        cyc += 1
    return order


def op(t):
    return re.sub(r'\br\d+\b', 'r', t)


def score(post, N, order):
    got = [op(N[i]['txt']) for i in order]
    want = [op(t) for t in post]
    sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
    return sum(i2 - i1 for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag == 'equal'), len(want)


if __name__ == '__main__':
    data = []
    for f in sys.argv[1:]:
        fn = None
        if ':' in f:
            f, fn = f.split(':')
        p = os.path.abspath(f)
        data.append((f, compile_(p, fn, 'level=0,peephole,speed'), compile_(p, fn, 'level=0,peephole,schedule,speed')))
    results = []
    def rev_sim(N, E, pol):
        n = len(N); R = {i: set() for i in range(n)}
        for i in E:
            for s in E[i]: R[s].add(i)
        N2 = list(reversed(N)); E2 = {n - 1 - i: set(n - 1 - s for s in R[i]) for i in range(n)}
        o = simulate(N2, E2, pol)
        return [n - 1 - i for i in reversed(o)]
    for L, W, LSU, sb, tie, rl, bu in itertools.product([1, 2, 3], [1, 2, 3, 4], [1, 2], [False, True], ['pre', 'rev'], [True, False], [False, True]):
        pol = dict(L=L, W=W, LSU=LSU, tie=tie, readylat=rl, bu=bu)
        tot = 0; mx = 0; per = []
        for f, pre, post in data:
            N = build(pre); E = edges(N, sb); order = (rev_sim if bu else simulate)(N, E, pol)
            s, m = score(post, N, order); tot += s; mx += m; per.append('%d/%d' % (s, m))
        results.append((tot, mx, pol, sb, per))
    results.sort(key=lambda r: -r[0])
    for tot, mx, pol, sb, per in results[:12]:
        print(tot, mx, pol, 'stack_barrier' if sb else '', per)
