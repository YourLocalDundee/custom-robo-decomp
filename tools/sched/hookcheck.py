"""Read the scheduler hook's log (tools/sched/hook, tools/sched/hooklog.py) and test the replay against it
(docs/sched_model.md section 10):
  dag   the replay's DAG built from the logged PCode vs the compiler's logged DAG (edges, latencies, heights)
  pick  the replay's cycle loop run on the compiler's own DAG vs the compiler's picks
usage: python -I tools/sched/hookcheck.py LOG [--show N] [--block K]
"""
import argparse, collections, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import replay as rp  # noqa: E402

CLS = {0: 's', 1: 'cr', 2: 'v', 3: 'f', 4: 'r'}


class Alias:
    """a logged 2.x alias: type 0 object, 1 range (object, offset, size), 2 set (member bits)"""

    def __init__(self, text):
        a = text.split(':')
        self.ptr, self.t, self.obj = a[0], int(a[1][1:]), a[2][1:]
        self.off, self.size, self.index = int(a[3]), int(a[4]), int(a[5][1:])
        self.bits = 0
        if len(a) > 6:
            w = a[6]
            for k in range(len(w) // 8):
                self.bits |= int(w[8 * k:8 * k + 8], 16) << (32 * k)

    def member(self, i):
        return bool(self.bits >> i & 1)

    def may(self, o):
        """alias_may_alias 0x511CE0"""
        a, b = self, o
        if a.t == 0 and b.t == 0:
            return a.ptr == b.ptr
        if {a.t, b.t} == {0, 1}:
            return a.obj == b.obj
        if a.t == 1 and b.t == 1:
            return a.obj == b.obj and a.off < b.off + b.size and b.off < a.off + a.size
        if a.t == 2 and b.t == 2:
            return a.ptr == b.ptr or bool(a.bits & b.bits)
        if a.t == 2:
            return a.member(b.index)
        return b.member(a.index)


class HIns:
    """a logged PCode instruction, shaped like replay.Ins for the scheduler and the machine model"""

    def __init__(self, line, names, idx):
        f = line.split()
        self.addr = f[1]
        self.opn = int(f[2][3:])
        self.op = names[self.opn] if 0 <= self.opn < len(names) else 'op%d' % self.opn
        self.mn = self.op
        self.flags = int(f[3][3:], 16)
        self.line = int(f[4][3:])
        f = f[:4] + f[5:]
        al = f[4][3:]
        self.alias = None if al == '0' else Alias(al)
        self.operands = f[5:]
        self.uses, self.defs, self.ops = [], [], []
        for o in self.operands:
            if o[0] == 'R':
                c, r, fl = o[1:].split(':')
                name = '%s%s' % (CLS.get(int(c), 'x'), r)
                fl = int(fl, 16)
                self.ops.append(name)
                if int(c) == 4 and (r in ('2', '13') or (r == '0' and not fl & 3)):
                    continue
                if fl & 2:                 # use + def counts as a def only (0x508390 takes flags & 2)
                    self.defs.append(name)
                elif fl & 1:
                    self.uses.append(name)
            else:
                self.ops.append(o)
        self.rec = False
        self.idx = idx
        self.phys = None
        self.mem = None
        self.text = '%s %s' % (self.op, ' '.join(self.operands))


class HBlock:
    def __init__(self, pass_, head):
        self.pass_ = pass_
        self.head = head
        self.ins = []
        self.nodes = []      # (addr, lat, h, d, np, e, [(succ addr, lat)])
        self.picks = []      # (cycle, [(addr, h, d, rc, check)], chosen addr or None)


def parse(path, names):
    blocks, pass_, cur = [], None, None
    for line in open(path, encoding='latin1'):
        t = line[:2]
        if t == 'S ':
            pass_ = int(line.split('=')[1])
        elif t == 'K ':
            cur = HBlock(pass_, line.strip())
            blocks.append(cur)
        elif t == 'I ' and cur is not None:
            cur.ins.append(HIns(line, names, len(cur.ins)))
        elif t == 'N ' and cur is not None:
            f = line.split()
            kv = dict(x.split('=') for x in f[2:8])
            succ = [(x.split(':')[0], int(x.split(':')[1])) for x in f[8:]]
            cur.nodes.append((kv['ins'], int(kv['lat']), int(kv['h']), int(kv['d']), int(kv['np']), int(kv['e']), succ))
        elif t == 'P ' and cur is not None:
            m = re.match(r'P (\d+)(.*) -> (\w+)', line.strip())
            cands = []
            for x in m.group(2).split():
                a, h, d, rc, ck = x.split(':')
                cands.append((a, int(h), int(d), int(rc), int(ck)))
            ch = m.group(3)
            cur.picks.append((int(m.group(1)), cands, None if ch == '00000000' else ch))
    return blocks


def logged_order(b):
    return [ch for _, _, ch in b.picks if ch]


def nodes_from_log(b, byaddr):
    nodes = {}
    for i, (a, lat, h, d, np_, e, succ) in enumerate(b.nodes):
        n = rp.Node(byaddr[a], lat, i)
        n.height, n.latest, n.npred, n.earliest = h, d, np_, e
        nodes[a] = n
    for a, lat, h, d, np_, e, succ in b.nodes:
        for s, l in succ:
            nodes[a].succ[nodes[s]] = l
    return [nodes[a] for a, *_ in b.nodes]


def alias_mem(ins):
    """the logged alias as the replay's memory object: load if flags & 0x20002, else store if flags & 0x40004"""
    if ins.flags & 0x20002:
        return ('L', ins.alias)
    if ins.flags & 0x40004:
        return ('S', ins.alias)
    return None


def check(log, show=0, only=None, cfg=None):
    t = rp.Tables()
    cfg = cfg or rp.Config()
    blocks = parse(log, t.names)
    res = collections.Counter()
    shown = 0
    for k, b in enumerate(blocks):
        if only is not None and k != only:
            continue
        if not b.nodes:
            continue
        byaddr = {i.addr: i for i in b.ins}
        if len(byaddr) != len(b.nodes):          # final pass: the PCode list was not dumped completely
            continue
        final = b.pass_ == 1
        # 1. the DAG
        for i in b.ins:
            i.mem = alias_mem(i)
        s = rp.Scheduler(t, cfg)
        mine = s.build(b.ins, final)
        logged = {a: (lat, h, d, dict(succ)) for a, lat, h, d, np_, e, succ in b.nodes}
        dag_ok = True
        for n in mine:
            lat, h, d, succ = logged[n.ins.addr]
            ms = {m.ins.addr: v for m, v in n.succ.items()}
            if ms != succ or n.height != h:
                dag_ok = False
                if shown < show:
                    shown += 1
                    print('DAG block %d %s: %s lat %d/%d h %d/%d' % (k, n.ins.addr, n.ins.text, n.lat, lat, n.height, h))
                    print('   extra  ', {a: v for a, v in ms.items() if succ.get(a) != v})
                    print('   missing', {a: v for a, v in succ.items() if ms.get(a) != v})
        res['blocks'] += 1
        res['dag_ok'] += dag_ok
        # 2. the picks on the compiler's own DAG
        nodes = nodes_from_log(b, byaddr)
        order = [i.addr for i in s.run(nodes, final)]
        want = logged_order(b)
        res['pick_ok'] += order == want
        if order != want and shown < show:
            shown += 1
            j = next(x for x in range(len(want)) if x >= len(order) or order[x] != want[x])
            print('PICK block %d (%s, pass %d): first difference at %d: replay %s, compiler %s' % (
                k, b.head, b.pass_, j, byaddr[order[j]].text if j < len(order) else '-', byaddr[want[j]].text))
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('log', nargs='+')
    ap.add_argument('--show', type=int, default=0)
    ap.add_argument('--block', type=int)
    a = ap.parse_args()
    tot = collections.Counter()
    for log in a.log:
        r = check(log, a.show, a.block)
        print('%s: %d blocks, DAG equal %d, picks equal %d' % (os.path.basename(log), r['blocks'], r['dag_ok'],
                                                               r['pick_ok']))
        tot.update(r)
    if len(a.log) > 1:
        print('total: %d blocks, DAG equal %d, picks equal %d' % (tot['blocks'], tot['dag_ok'], tot['pick_ok']))


if __name__ == '__main__':
    main()
