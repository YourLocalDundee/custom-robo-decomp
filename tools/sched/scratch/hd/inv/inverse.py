"""Inverse search for cr_CM_DUMMY_008 (docs/sched_model.md section 11; scratch, step 89).
usage: python -I inverse.py HOOKLOG SEED RESTARTS ITERATIONS [DIRECT] [PTARGET] [OBJCNT] [OBJPC]
Takes the first-pass PCode of the hook log (tools/sched/hooklog.py), anneals over input orders (a definition before
its uses, aliasing accesses of different statements in the log's order), replays the first pass, colours with the
original's registers (one consistent virtual -> physical map), replays the final pass with the logged prologue /
epilogue, and scores the words equal to the original's. DIRECT: the order is taken as the first pass's OUTPUT;
PTARGET: score against the output order a DIRECT run saved (P_target.txt beside this file)."""
import os, sys, json, itertools, re, collections
SCHED = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
sys.path.insert(0, SCHED)
import replay as r, hookcheck as h
t = r.Tables()
man = json.load(open(os.path.join(SCHED, 'corpus', 'manifest.json')))
log = sys.argv[1]
orig = man['originals']['cr_CM_DUMMY_008']
body, frame, fr = r.body_blocks(orig)[0]

def dsig(text):
    i = r.Ins(text)
    op = i.op
    if (r.is_load(op) or r.is_store(op)) and len(i.ops) > 1:
        m = r.MEM.match(i.ops[1]); d = m.group(1) if m else ''
        n = r.to_int(d)
        return '%s %s' % (op, n if n is not None else 0)
    if op == 'addi':
        n = r.to_int(i.ops[2]); return 'addi %s' % (n if n is not None else 0)
    if op == 'rlwinm':
        return op
    return op

def hsig(i):
    op = i.op
    nums = [o for o in i.operands if o[0] in 'MX']
    if r.is_load(op) or r.is_store(op) or op == 'addi':
        v = int(nums[0][1:].split(':')[0]) if nums else 0
        return '%s %s' % (op, v)
    return op

T = [dsig(x) for x in body if not x.startswith('blr')]
blocks = h.parse(log, t.names)
b = [x for x in blocks if x.pass_ == 0][0]
ins = b.ins
# instructions the passes between the schedulings delete (the hook's -D lines that never reach the final pass)
_final, _deleted, _f = set(), set(), False
for _l in open(log, encoding='latin1'):
    if _l.startswith('S force=1'): _f = True
    if _f and _l.startswith('I '): _final.add(_l.split()[1])
    if _l.startswith('-D '): _deleted.add(_l.split()[4])
DELETED = {a for a in _deleted if a not in _final}
FP, FOFF = None, 0
for i in ins:
    if i.op == 'addi' and i.defs and i.defs[0][0] == 'r' and 14 <= int(i.defs[0][1:]) < 32:
        nums = [o for o in i.operands if o[0] == 'M']
        if nums: FP, FOFF = i.defs[0], int(nums[0][1:].split(':')[0])
print('folded pointer', FP, FOFF, 'deleted between the passes', sorted(i.op + ' ' + ' '.join(i.operands[:3]) for i in ins if i.addr in DELETED))
print('PCode', len(ins), 'original body', len(T))
print(sorted(collections.Counter(hsig(i) for i in ins).items()))
print(sorted(collections.Counter(T).items()))
groups = collections.OrderedDict()
for i in ins:
    groups.setdefault(i.line, []).append(i)
print('groups', [(k, len(v)) for k, v in groups.items()])
best = []
br = [i for i in ins if i.op.startswith('b')]
keys = [k for k in groups if not all(i.op == 'blr' for i in groups[k])]
for perm in itertools.permutations(keys):
    seq = [i for k in perm for i in groups[k] if not i.op == 'blr'] + [i for i in ins if i.op == 'blr']
    # def before use of locals / vregs
    seen = set(); ok = True
    for i in seq:
        if any(u not in seen and u not in ('r1', 'r0') and int(u[1:]) >= 30 for u in i.uses if u[0] == 'r'):
            ok = False; break
        seen.update(i.defs)
    if not ok: continue
    seq2 = []
    for k, i in enumerate(seq):
        n = h.HIns.__new__(h.HIns); n.__dict__.update(i.__dict__); n.idx = k; n.mem = h.alias_mem(i); seq2.append(n)
    s = r.Scheduler(t, r.Config())
    out = s.schedule(seq2)
    S = []
    for x in out:
        if x.op == 'addi' and x.defs == [FP]:
            continue
        g = hsig(x)
        if FP in x.uses and (r.is_load(x.op) or r.is_store(x.op)):
            g = g.split()[0] + ' %d' % (FOFF + int(g.split()[1]))
        S.append(g)
    sc = sum(1 for a, c in zip(S, T) if a == c)
    best.append((sc, perm, S))
best.sort(reverse=True)
print(len(best), [b[:2] for b in best[:5]]); print(best[0][2]); print(T)

# ---- both passes: pass-1 order, the original's registers as the colouring, the final pass
OT = [x for x in orig]
ob_body = body
def dregs(text):
    i = r.Ins(text)
    out = []
    for k, o in enumerate(i.ops):
        m = r.MEM.match(o)
        if m:
            if m.group(2) != 'r0': out.append(m.group(2))
        elif re.match(r'^[rf]\d+$', o):
            out.append(o)
    return out
def hregs(i, fold):
    out = []
    for o in i.operands:
        if o[0] == 'R':
            c, rr, fl = o[1:].split(':')
            if c == '4' and rr == '0' and not int(fl, 16) & 3: continue
            name = '%s%s' % (h.CLS[int(c)], rr)
            out.append(fold.get(name, name))
    return out
def colour(order):
    """map each pass-1 instruction onto an original body instruction (same sig, one consistent register map)"""
    fold = {}
    items = []
    for x in order:
        if x.op == 'addi' and x.defs == [FP]:
            fold[FP] = [u for u in x.uses][0]; continue
        g = hsig(x)
        if FP in x.uses and (r.is_load(x.op) or r.is_store(x.op)):
            g = g.split()[0] + ' %d' % (FOFF + int(g.split()[1]))
        items.append((g, hregs(x, fold)))
    cand = collections.defaultdict(list)
    bodyx = [b for b in ob_body if not b.startswith('blr')]
    for j, tx in enumerate(bodyx): cand[dsig(tx)].append(j)
    used, vmap, res = set(), {}, {}
    def go(k):
        if k == len(items): return True
        g, regs = items[k]
        for j in cand.get(g, []):
            if j in used: continue
            pr = dregs(bodyx[j])
            if len(pr) != len(regs): continue
            new = {}
            ok = True
            for a, b2 in zip(regs, pr):
                if vmap.get(a, new.get(a, b2)) != b2: ok = False; break
                if a not in vmap: new[a] = b2
            if not ok: continue
            used.add(j); vmap.update(new); res[k] = j
            if go(k + 1): return True
            used.discard(j); del res[k]
            for a in new: vmap.pop(a)
        return False
    if not go(0): return None
    COL['vmap'] = dict(vmap); COL['fold'] = dict(fold)
    return [bodyx[res[k]] for k in range(len(items))]
COL = {}
pro = [x for x in frame if not x.startswith(('lwz', 'addi'))]
epi = [x for x in frame if x not in pro]
results = []
for sc, perm, S in best:
    pass
cnt = 0
for perm in [b[1] for b in best]:
    seq = [i for k in perm for i in groups[k]]
    seq2 = []
    for k, i in enumerate(seq):
        n = h.HIns.__new__(h.HIns); n.__dict__.update(i.__dict__); n.idx = k; n.mem = h.alias_mem(i); seq2.append(n)
    out = r.Scheduler(t, r.Config()).schedule(seq2)
    phys = colour(out)
    if phys is None:
        continue
    cnt += 1
    blk = pro + phys + epi + ['blr']
    fin = r.run_final(r.Scheduler(t, r.Config()), [blk], [True])[0]
    sc = sum(1 for a, c in zip(fin, OT) if a == c)
    results.append((sc, perm))
results.sort(reverse=True)
print('colourable', cnt, 'best both-pass scores vs original (exact words incl. registers):', results[:6])

# ---- simulated annealing over instruction-level input orders
import random, math
random.seed(int(sys.argv[2]) if len(sys.argv) > 2 else 1)
base_seq = list(ins)
if 'OBJCNT' in sys.argv:
    objal = [i.alias for i in ins if i.op == 'add'][0]
    for i in ins:
        if 'r31' in i.uses and (r.is_load(i.op) or r.is_store(i.op)):
            i.alias = objal
if 'OBJPC' in sys.argv:
    objal = [i.alias for i in ins if i.op == 'add'][0]
    for i in ins:
        if 'r30' in i.uses and r.is_load(i.op):
            i.alias = objal
memi = [i for i in base_seq if h.alias_mem(i)]
conf = []
for a in range(len(memi)):
    for b2 in range(a + 1, len(memi)):
        x, y = memi[a], memi[b2]
        mx, my = h.alias_mem(x), h.alias_mem(y)
        if (mx[0] == 'S' or my[0] == 'S') and r.may_alias(mx[1], my[1], r.Config()) and x.line != y.line:
            conf.append((x.addr, y.addr))
def legal(seq):
    seen = set()
    for i in seq:
        if any(u not in seen for u in i.uses if u[0] in 'rf' and u != 'r1'): return False
        seen.update(i.defs)
    pos = {i.addr: k for k, i in enumerate(seq)}
    return all(pos[a] < pos[b2] for a, b2 in conf) if MEMKEEP else True
MEMKEEP = 'NOMEM' not in sys.argv
PT = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'P_target.txt')).read().split() if 'PTARGET' in sys.argv else None
def evaluate(seq):
    seq2 = []
    for k, i in enumerate(seq):
        n = h.HIns.__new__(h.HIns); n.__dict__.update(i.__dict__); n.idx = k; n.mem = h.alias_mem(i); seq2.append(n)
    out = r.Scheduler(t, r.Config()).schedule(seq2)
    S = []
    for x in out:
        if x.op == 'addi' and x.defs == [FP]: continue
        g = hsig(x)
        if FP in x.uses and (r.is_load(x.op) or r.is_store(x.op)): g = g.split()[0] + ' %d' % (FOFF + int(g.split()[1]))
        S.append(g)
    p1 = sum(1 for a, c in zip(S, T) if a == c)
    phys = colour(out)
    if phys is None:
        return p1 * 0.1, 0, None
    fin = r.run_final(r.Scheduler(t, r.Config()), [pro + phys + [e for e in epi if not e.startswith('addi')]], [True])[0] + [e for e in epi if e.startswith('addi')] + ['blr']
    ex = sum(1 for a, c in zip(fin, OT) if a == c)
    return ex + p1 * 0.1, ex, fin
fblk = [b for b in h.parse(log, t.names) if b.pass_ == 1][0]
FPRO = [x for x in fblk.ins[:3]]
FEPI = [x for x in fblk.ins if x.op == 'lwz' and 'r1' in x.uses]
import copy
def evaluate(seq):
    seq2 = []
    for k, i in enumerate(seq):
        n = h.HIns.__new__(h.HIns); n.__dict__.update(i.__dict__); n.idx = k; n.mem = h.alias_mem(i); seq2.append(n)
    out = seq2 if 'DIRECT' in sys.argv else r.Scheduler(t, r.Config()).schedule(seq2)
    out = [x for x in out if not getattr(x, 'synth', False) and not (x.addr in DELETED and x.defs != [FP])]
    if PT is not None:
        m = sum(1 for a, c in zip(out, PT) if a.addr == c)
        return m, m, [x.addr for x in out]
    S = []
    for x in out:
        if x.op == 'addi' and x.defs == [FP]: continue
        g = hsig(x)
        if FP in x.uses and (r.is_load(x.op) or r.is_store(x.op)): g = g.split()[0] + ' %d' % (FOFF + int(g.split()[1]))
        S.append(g)
    p1 = sum(1 for a, c in zip(S, T) if a == c)
    phys = colour(out)
    if phys is None:
        return p1 * 0.1, 0, None
    vmap, fold = COL['vmap'], COL['fold']
    fin_in = []
    for x in FPRO:
        y = copy.copy(x); y.mem = h.alias_mem(x); y.text = 'PRO'; fin_in.append(y)
    k = 0
    for x in out:
        if x.op == 'addi' and x.defs == [FP]: continue
        y = copy.copy(x)
        ren = lambda v: vmap.get(fold.get(v, v), fold.get(v, v))
        y.uses = [ren(u) for u in x.uses]; y.defs = [ren(d) for d in x.defs]
        y.ops = [ren(o) for o in x.ops]
        y.mem = h.alias_mem(x); y.text = phys[k]; k += 1
        fin_in.append(y)
    for x in FEPI:
        y = copy.copy(x); y.mem = h.alias_mem(x); fin_in.append(y)
    for n_, y in enumerate(fin_in): y.idx = n_
    for y in FPRO + FEPI: pass
    fo = r.Scheduler(t, r.Config()).schedule(fin_in, final=True)
    texts = []
    pro_t = [x for x in orig if x.startswith(('stwu', 'stw r31', 'stw r30'))]
    epi_t = [x for x in orig if x.startswith(('lwz r31, 0xc(r1)', 'lwz r30, 0x8(r1)'))]
    for y in fo:
        if y in fin_in[:3]: texts.append(pro_t[fin_in.index(y)])
        elif y in fin_in[-2:]: texts.append(epi_t[fin_in[-2:].index(y)])
        else: texts.append(y.text)
    fin = texts + ['addi r1, r1, 0x10', 'blr']
    ex = sum(1 for a, c in zip(fin, OT) if a == c)
    return ex + p1 * 0.1, ex, fin
if 'SYNTH' in sys.argv:
    # a synthetic instruction the passes between the schedulings delete: placed in the first pass's input, removed
    # from its output before colouring
    perms = [b[1] for b in best if legal([i for k in b[1] for i in groups[k]])]
    def mk(op, defs, uses, like=None):
        x = h.HIns.__new__(h.HIns)
        if like is not None:
            x.__dict__.update(like.__dict__)
        else:
            x.__dict__.update(ins[0].__dict__); x.flags = 0x80004000; x.alias = None; x.operands = []
        x.op = x.mn = op; x.defs = list(defs); x.uses = list(uses); x.ops = list(defs) + list(uses)
        x.synth = True; x.addr = 'SYN'; x.text = 'synthetic ' + op
        return x
    cands = []
    for X in ins:
        if X.defs and not (X.flags & 0x40004) and not X.op.startswith('b'):
            cands.append(('dup ' + X.op + ' ' + ' '.join(X.operands[:3]), lambda X=X: mk(X.op, ['r900'], X.uses, X)))
    vregs = sorted({d for X in ins for d in X.defs})
    for v in vregs:
        cands.append(('mr from ' + v, lambda v=v: mk('mr', ['r901'], [v])))
        cands.append(('mr into ' + v, lambda v=v: mk('mr', [v], ['r902'])))
    cands.append(('li', lambda: mk('li', ['r903'], [])))
    if 'INPLACE' in sys.argv:
        cands = [('inplace ' + op + ' ' + v, lambda v=v, op=op: mk(op, [v], [v])) for v in vregs for op in ('rlwinm', 'extsh')]
    res = []
    n = 0
    for perm in perms:
        base_seq = [i for k in perm for i in groups[k]]
        for name, f in cands:
            for pos in range(len(base_seq) + 1):
                x = f()
                seq = base_seq[:pos] + [x] + base_seq[pos:]
                seen = set(); ok = True
                for i in seq:
                    if any(u not in seen for u in i.uses if u[0] in 'rf' and u != 'r1' and u != 'r902'):
                        ok = False; break
                    seen.update(i.defs)
                if not ok:
                    continue
                n += 1
                sc_, ex_, fin_ = evaluate(seq)
                res.append((ex_, name, pos, perm))
    res.sort(key=lambda z: -z[0])
    print('SYNTH', n, 'evaluations; best', res[:12])
    import collections as _c
    full = [z for z in res if z[0] == 38]
    print('38-solutions', len(full)); print(_c.Counter((z[1], z[3]) for z in full).most_common(40))
    sys.exit(0)
best_all = (-1, 0, None, None)
N = int(sys.argv[4]) if len(sys.argv) > 4 else 3000
for restart in range(int(sys.argv[3]) if len(sys.argv) > 3 else 5):
    legal_perms = [b[1] for b in best if legal([i for k in b[1] for i in groups[k]])]
    if restart == 0: print('legal statement orders', len(legal_perms), 'best', [b[:2] for b in best if b[1] in legal_perms][:3])
    seq = [i for k in random.choice(legal_perms) for i in groups[k]]
    sc, ex, fin = evaluate(seq)
    for it in range(N):
        temp = 2.0 * (1 - it / N) + 0.05
        new = list(seq)
        if random.random() < 0.3:
            g = random.choice(list(groups)); idxs = [k for k, i in enumerate(new) if i.line == g]
            blk = [new[k] for k in idxs]
            for k in reversed(idxs): new.pop(k)
            at = random.randrange(len(new) + 1)
            new[at:at] = blk
        else:
            a = random.randrange(len(new)); x = new.pop(a); new.insert(random.randrange(len(new) + 1), x)
        if not legal(new): continue
        s2, e2, f2 = evaluate(new)
        if s2 >= sc or random.random() < math.exp((s2 - sc) / temp):
            seq, sc, ex, fin = new, s2, e2, f2
            if ex > best_all[1]:
                best_all = (sc, ex, list(seq), fin)
        if ex == (len(PT) if PT else len(OT)): break
    print('restart', restart, 'best exact', best_all[1], flush=True)
    if best_all[1] == (len(PT) if PT else len(OT)): break
sc, ex, seq, fin = best_all
print('BEST', ex, 'of', len(OT))
if 'DIRECT' in sys.argv and ex == len(OT):
    open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'P_target.txt'), 'w').write(' '.join(i.addr for i in seq))
for i in seq: print('   in ', i.line, i.op, ' '.join(i.operands[:4]))
for a, c in zip(fin, OT): print('%s %-36s %s' % (' ' if a == c else '*', a, c))
