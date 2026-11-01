"""A replay of the mwcceppc instruction scheduler (docs/sched_model.md): the GC/1.x algorithm of rayanht/mwcc
src/backend/Scheduler.c (DAG built from the last instruction backwards, heights, deadlines, a cycle loop issuing up to
the machine model's width through the urgency / release-count / height / opcode-rank / program-order pick) with the
GC/2.6 machine models, read at run time from the compiler binary (read only): the opcode table at 0x5C0FA8 (18-byte
records, the pick rank is byte 9) and the per-processor tables (6 bytes per opcode: unit, latency, three stage
cycles, serialize). `-proc gekko` selects the 750 model in GC/2.6 (docs/sched_model.md section 2).

The input is a block as instruction text in dtk's syntax (registers physical or renamed); the output is the scheduled
order. Options (Config) switch the points the 1.x source and the 2.x notes disagree on, so the corpus can decide.

Usage:
  python -I tools/sched/replay.py --corpus [--report tools/sched/corpus_report.txt]   measure over tools/sched/corpus
  python -I tools/sched/replay.py --trace FILE_ID [FUNCTION]   one corpus function, pass 1, every pick printed
"""
import argparse, collections, itertools, json, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.dirname(os.path.dirname(HERE))
EXE = os.path.join(G, 'decomp', 'build', 'compilers', 'GC', '2.6', 'mwcceppc.exe')
CORPUS = os.path.join(HERE, 'corpus')

OPCODE_TABLE = 0x5C0FA8      # GC/2.6: 471 records of 18 bytes (name, format, ..., byte 9 = pick rank)
OPCODE_COUNT = 471
MODELS = {                   # GC/2.6 MachineInfo addresses (Scheduler_Schedule's switch, docs/sched_community.md)
    '750': 0x5D5B50,         # processor byte 4: -proc 740 / 750 / gekko
    '603': 0x5D3970,         # processor byte 2 and the default
}                            # (7400 0x5D2C70 and 7450 0x5D1C48 have other record layouts: not read here)


# ---- the compiler's tables -------------------------------------------------------------------------------------------
class PE:
    def __init__(self, path):
        self.d = open(path, 'rb').read()
        pe = struct.unpack_from('<I', self.d, 0x3c)[0]
        nsec = struct.unpack_from('<H', self.d, pe + 6)[0]
        opt = struct.unpack_from('<H', self.d, pe + 20)[0]
        base = struct.unpack_from('<I', self.d, pe + 24 + 28)[0]
        self.secs = []
        o = pe + 24 + opt
        for _ in range(nsec):
            vs, va, rs, ra = struct.unpack_from('<IIII', self.d, o + 8)
            self.secs.append((base + va, vs, ra, rs))
            o += 40

    def rd(self, va, n):
        for v, vs, ra, rs in self.secs:
            if v <= va < v + max(vs, rs):
                return self.d[ra + va - v:ra + va - v + n]
        raise ValueError(hex(va))

    def u32(self, va):
        return struct.unpack('<I', self.rd(va, 4))[0]

    def cstr(self, va):
        b = self.rd(va, 64)
        return b[:b.index(b'\0')].decode('latin1')


class Tables:
    def __init__(self, exe=EXE):
        pe = PE(exe)
        self.names, self.rank = [], {}
        for i in range(OPCODE_COUNT):
            e = pe.rd(OPCODE_TABLE + 18 * i, 18)
            n = pe.cstr(struct.unpack('<I', e[:4])[0]).lower()
            self.names.append(n)
            self.rank[n] = e[9]
        self.models = {}
        for name, addr in MODELS.items():
            hdr = [pe.u32(addr + 4 * k) for k in range(10)]
            # getLatency (4th dword) reads the latency byte as movsx eax, byte [eax*2 + table + 1]
            code = pe.rd(hdr[3], 0x40)
            k = code.find(b'\x0f\xbe\x04\x45')
            tbl = struct.unpack('<I', code[k + 4:k + 8])[0] - 1 if k >= 0 else None
            ops = {}
            if tbl is not None:
                for i, n in enumerate(self.names):
                    ops[n] = tuple(pe.rd(tbl + 6 * i, 6))
            self.models[name] = {'width': hdr[0], 'omit_anti': hdr[1], 'extra': hdr[2], 'table': tbl, 'ops': ops}


# ---- instruction parsing ---------------------------------------------------------------------------------------------
BRANCH_COND = re.compile(r'^b(eq|ne|lt|ge|gt|le|so|ns|dnz|dnzt|dnzf|dz|dzt|dzf|t|f)(lr|ctr)?(l)?[+-]?$')
ALIAS = {'slwi': 'rlwinm', 'srwi': 'rlwinm', 'clrlwi': 'rlwinm', 'clrrwi': 'rlwinm', 'rotlwi': 'rlwinm',
         'extlwi': 'rlwinm', 'extrwi': 'rlwinm', 'clrlslwi': 'rlwinm', 'rotlw': 'rlwnm', 'inslwi': 'rlwimi',
         'insrwi': 'rlwimi', 'sub': 'subf', 'subc': 'subfc', 'subi': 'addi', 'subis': 'addis', 'subic': 'addic',
         'cmpwi': 'cmpi', 'cmpw': 'cmp', 'cmplwi': 'cmpli', 'cmplw': 'cmpl', 'crclr': 'crxor', 'crset': 'creqv',
         'crmove': 'cror', 'crnot': 'crnor', 'mtcr': 'mtcrf', 'mfxer': 'mfxer', 'mtxer': 'mtxer'}
LOAD_SIZE = {'b': 1, 'h': 2, 'w': 4, 'fs': 4, 'fd': 8}


def pcop(mn):
    """dtk mnemonic -> PCode opcode name (lower case) and record flag"""
    rec = mn.endswith('.')
    m = mn.rstrip('.')
    if m in ALIAS:
        m = ALIAS[m]
    elif BRANCH_COND.match(m) and m not in ('bl', 'blr', 'bctr', 'bctrl', 'blrl'):
        c = BRANCH_COND.match(m)
        cond = c.group(1)
        if cond.startswith('d'):
            m = 'bdnz' if cond.startswith('dnz') else 'bdz'
        else:
            m = 'bt' + (c.group(2) or '') if cond in ('eq', 'lt', 'gt', 'so', 't') else 'bf' + (c.group(2) or '')
    return m, rec


MEM = re.compile(r'^(.*)\((r\w+)\)$')
REG = re.compile(r'^(r|f)(\d+|v\d+|\d+in)$')     # physical, virtual (rv12, fv3) or live-in (r3in)
CRF = re.compile(r'^cr(\d)$')


class Ins:
    """one instruction: text, PCode opcode, register uses / defs, memory access"""
    __slots__ = ('text', 'mn', 'op', 'rec', 'ops', 'uses', 'defs', 'mem', 'idx', 'phys')

    def __init__(self, text, idx=0):
        self.text = text
        self.idx = idx
        parts = text.split(None, 1)
        self.mn = parts[0]
        self.ops = [o.strip() for o in parts[1].split(',')] if len(parts) > 1 else []
        self.op, self.rec = pcop(self.mn)
        self.uses, self.defs = regs_of(self)
        self.mem = None   # (kind 'L'/'S', base register or None, displacement text, size)
        self.phys = None


def is_store(op):
    return op.startswith('st') or op.startswith('psq_st')


def is_load(op):
    return (op.startswith('l') and op not in ('li', 'lis')) or op.startswith('psq_l')


def regs_of(ins):
    """(uses, defs): register keys 'r5', 'f3', 'cr0', 'lr', 'ctr', 'xer'"""
    op, ops = ins.op, ins.ops
    uses, defs = [], []

    def reg(o):
        return o if REG.match(o) else None

    def use(o):
        if o and o not in ('r2', 'r13'):
            uses.append(o)

    def mem_base(o):
        m = MEM.match(o)
        if m and m.group(2) != 'r0':
            use(m.group(2))
        return m

    if is_store(op):
        if op in ('stmw',):
            n = int(ops[0][1:])
            for k in range(n, 32):
                use('r%d' % k)
        else:
            use(reg(ops[0]))
        if len(ops) > 1:
            m = mem_base(ops[1])
            if not m:                          # indexed: stwx rS, rA, rB
                if ops[1] != 'r0':
                    use(reg(ops[1]))
                if len(ops) > 2:
                    use(reg(ops[2]))
            if op.endswith('u') or op.endswith('ux') or op.startswith('psq_stu'):
                b = m.group(2) if m else ops[1]
                defs.append(b)
    elif is_load(op):
        if op == 'lmw':
            for k in range(int(ops[0][1:]), 32):
                defs.append('r%d' % k)
        else:
            defs.append(ops[0])
        m = mem_base(ops[1]) if len(ops) > 1 else None
        if len(ops) > 1 and not m:
            if ops[1] != 'r0':
                use(reg(ops[1]))
            if len(ops) > 2:
                use(reg(ops[2]))
        if op.endswith('u') or op.endswith('ux') or op.startswith('psq_lu'):
            defs.append(m.group(2) if m else ops[1])
    elif op in ('cmpi', 'cmp', 'cmpli', 'cmpl', 'fcmpu', 'fcmpo'):
        rest = ops
        if ops and CRF.match(ops[0]):
            defs.append(ops[0])
            rest = ops[1:]
        else:
            defs.append('cr0')
        for o in rest:
            use(reg(o))
    elif op.startswith('b'):
        if op in ('bt', 'bf', 'btlr', 'bflr', 'btctr', 'bfctr'):
            crs = [o for o in ops if CRF.match(o)]
            uses.append(crs[0] if crs else 'cr0')
        if op in ('bdnz', 'bdz'):
            uses.append('ctr')
            defs.append('ctr')
        if op.endswith('lr') or op == 'blr':
            uses.append('lr')
        if op.endswith('ctr') or op in ('bctr', 'bctrl'):
            uses.append('ctr')
        if op in ('bl', 'bctrl', 'blrl'):
            defs.append('lr')
    elif op == 'mtlr':
        use(reg(ops[0])); defs.append('lr')
    elif op == 'mflr':
        defs.append(ops[0]); uses.append('lr')
    elif op == 'mtctr':
        use(reg(ops[0])); defs.append('ctr')
    elif op == 'mfctr':
        defs.append(ops[0]); uses.append('ctr')
    elif op.startswith('cr'):
        for i, o in enumerate(ops):
            f = 'cr%d' % (int(o) // 4) if o.isdigit() else o
            (defs if i == 0 else uses).append(f)
    elif op in ('stfiwx',):
        for o in ops:
            use(reg(o))
    else:
        if ops:
            if reg(ops[0]):
                defs.append(ops[0])
            for o in ops[1:]:
                use(reg(o))
            if op in ('rlwimi',):
                use(reg(ops[0]))
        if op in ('addic', 'subfic', 'srawi', 'sraw', 'addc', 'subfc'):
            defs.append('xer')
        if op in ('adde', 'subfe', 'addze', 'subfze', 'addme', 'subfme'):
            uses.append('xer'); defs.append('xer')
    if ins.rec and 'cr0' not in defs:
        defs.append('cr0')
    return [u for u in uses if u], defs


# ---- memory objects --------------------------------------------------------------------------------------------------
def access_size(op):
    if op in ('lmw', 'stmw'):
        return 128
    if op.startswith('psq'):
        return 8
    for k in ('fd', 'fs', 'b', 'h', 'w'):
        if op[1:].startswith(k) or op[2:].startswith(k):
            return LOAD_SIZE[k]
    return 4


def to_int(s):
    try:
        return int(s, 0)
    except ValueError:
        return None


LOCAL_PTR_WORST = [True]   # an access through a pointer local (a callee-saved register) has the worst-case alias
SYMREF = re.compile(r'^(?:"([^"]+)"|([^"@]+))@(sda21|l|ha)$')


def annotate_memory(block, regval=None):
    """forward pass: give each load / store a memory object (sym, offset or None, size), ('@frame', off, size),
    ('@abs', address, size), '@const' (a compiler literal: no dependences), or None (a pointer access)"""
    val = dict(regval or {})
    val['r1'] = ('frame', 0)
    for ins in block:
        op, ops = ins.op, ins.ops
        if is_load(ins.op) or is_store(ins.op):
            kind = 'S' if is_store(op) else 'L'
            obj = None
            disp = ops[1] if len(ops) > 1 else ''
            m = MEM.match(disp)
            size = access_size(op)
            if m:
                d, base = m.group(1), m.group(2)
                sm = SYMREF.match(d)
                if sm and sm.group(3) == 'sda21':
                    obj = (sm.group(1) or sm.group(2), 0, size)
                elif sm:
                    v = val.get(base)
                    nm = sm.group(1) or sm.group(2)
                    obj = (nm, 0, size) if (v and v[0] == 'ha' and v[1] == nm) else (nm, None, size)
                else:
                    off = to_int(d)
                    v = val.get(base)
                    if v is None or off is None:
                        obj = None
                    elif v[0] == 'addr':
                        obj = (v[1], None if v[2] is None else v[2] + off, size)
                    elif v[0] == 'frame':
                        obj = ('@frame', None if v[1] is None else v[1] + off, size)
                    elif v[0] == 'abs':
                        obj = ('@abs', v[1] + off, size)
                    elif v[0] == 'ha':
                        obj = (v[1], None, size)
            else:   # indexed
                vs = [val.get(o) for o in ops[1:3]]
                obj = None
                for v in vs:
                    if v and v[0] == 'addr':
                        obj = (v[1], None, size)
                    elif v and v[0] == 'frame':
                        obj = ('@frame', None, size)
            if obj and obj[0].startswith('@') and obj[0] not in ('@frame', '@abs') and kind == 'L':
                obj = '@const'
            if op in ('stwu',) and ops and MEM.match(disp) and MEM.match(disp).group(2) == 'r1':
                obj = ('@frame', None, 128)
            ins.mem = (kind, obj)
        # register values
        d = ins.defs[0] if ins.defs else None
        nv = None
        if op == 'lis' and len(ops) == 2:
            sm = SYMREF.match(ops[1])
            if sm and sm.group(3) == 'ha':
                nv = ('ha', sm.group(1) or sm.group(2))
            elif to_int(ops[1]) is not None:
                nv = ('abs', (to_int(ops[1]) << 16) & 0xffffffff)
        elif op in ('addi',) and len(ops) == 3:
            b = val.get(ops[1])
            sm = SYMREF.match(ops[2])
            imm = to_int(ops[2])
            if sm and sm.group(3) == 'l' and b and b[0] == 'ha':
                nv = ('addr', sm.group(1) or sm.group(2), 0)
            elif b and imm is not None:
                if b[0] == 'addr':
                    nv = ('addr', b[1], None if b[2] is None else b[2] + imm)
                elif b[0] == 'frame':
                    nv = ('frame', None if b[1] is None else b[1] + imm)
                elif b[0] == 'abs':
                    nv = ('abs', b[1] + imm)
        elif op == 'add' and len(ops) == 3:
            for o in ops[1:]:
                b = val.get(o)
                if b and b[0] == 'addr':
                    nv = ('addr', b[1], None)
                elif b and b[0] == 'frame':
                    nv = ('frame', None)
        elif op == 'mr' and len(ops) == 2:
            nv = val.get(ops[1])
        for dd in ins.defs:
            val.pop(dd, None)
        if d and nv is not None and d != 'r1' and not (LOCAL_PTR_WORST[0] and re.match(r'^r(1[4-9]|2\d|3[01])$', d)):
            val[d] = nv
    return val


def may_alias(a, b, cfg):
    """two memory objects (non-const); None = a pointer access"""
    if a is None or b is None:
        other = b if a is None else a
        if other is None:
            return True
        if other[0] == '@frame':
            return cfg.frame_alias
        return True
    if a[0] != b[0]:
        return False
    if cfg.mem == 'object':
        return True
    if a[1] is None or b[1] is None:
        return True
    return a[1] < b[1] + b[2] and b[1] < a[1] + a[2]


def gpr_conflict(ins, other):
    """0x507BF0: other's first operand is a GPR it defines and ins reads or writes that GPR"""
    if other is None or not other.ops:
        return False
    r = other.ops[0]
    if not (REG.match(r) and r.startswith('r')) or r not in other.defs:
        return False
    return r in ins.uses or r in ins.defs


# ---- the machine simulations (rayanht MachineSimulation750.c / 603.c, the GC/2.6 tables) ------------------------------
class Sim750:
    """9 stages: 0 BPU, 1 IU1, 2 IU2, 3 LSU1, 4 LSU2, 5 FPU1, 6 FPU2, 7 FPU3, 8 SRU; 6 in flight, 2 retires a cycle"""
    RING = 6

    def __init__(self, ops):
        self.ops = ops
        self.st = [None] * 9
        self.rem = [0] * 9
        self.slots = self.RING
        self.ring = [None] * self.RING
        self.done = [0] * self.RING
        self.pending = 0
        self.head = 0
        self.tail = 0
        self.last1 = self.last2 = None
        self.iu_conflict = True
        self.store_rule = True

    def info(self, ins):
        return self.ops.get(ins.op, (2, 1, 1, 0, 0, 0))

    LSU2 = 4

    def check(self, ins):
        if self.slots == 0:
            return False
        k = self.info(ins)[0]
        if k == 2:
            b1, b2 = self.st[1], self.st[2]
            if b1 is not None and b2 is not None:
                return False
            if b1 is None and b2 is None:
                return True
            if self.iu_conflict:
                # GC/2.6 (0x507BF0, dead in 1.x): no second integer unit for an instruction that reads or writes the
                # GPR the other unit's instruction defines, nor one defined by an integer instruction that completed
                # in the last cycle
                for other in (b1 if b1 is not None else b2, self.last1, self.last2):
                    if gpr_conflict(ins, other):
                        return False
        elif self.st[k] is not None:
            return False
        return self.store_ok(ins)

    def store_ok(self, ins):
        """a store does not issue while the second load / store stage holds a store (instruction flag 4)"""
        o = self.st[self.LSU2]
        return not (self.store_rule and o is not None and is_store(ins.op) and is_store(o.op))

    def issue(self, ins):
        u, lat, s0, s1, s2, ser = self.info(ins)
        self.pending += 1
        self.slots -= 1
        self.ring[self.tail] = ins
        self.done[self.tail] = 0
        self.tail = (self.tail + 1) % self.RING
        k = u
        if k == 2 and self.st[1] is None:
            k = 1
        self.st[k] = ins
        self.rem[k] = s0

    def _complete(self, ins):
        for j in range(self.RING):
            if self.ring[j] is ins:
                self.done[j] = 1
                return

    def advance(self):
        self.last1 = self.last2 = None
        for i in range(len(self.st)):
            if self.st[i] is not None and self.rem[i]:
                self.rem[i] -= 1
        for _ in range(2):
            if self.pending and self.done[self.head]:
                self.ring[self.head] = None
                self.pending -= 1
                self.slots += 1
                self.head = (self.head + 1) % self.RING
            else:
                break
        for i in self.COMPLETE:
            if self.st[i] is not None and self.rem[i] == 0:
                self._complete(self.st[i])
                if i == 1:
                    self.last1 = self.st[i]
                elif i == 2 and self.RING == 6:
                    self.last2 = self.st[i]
                self.st[i] = None
        f1 = self.st[self.FPU1]
        if f1 is not None and self.rem[self.FPU1] == 0 and f1.op in ('fdiv', 'fdivs'):
            self._complete(f1)
            self.st[self.FPU1] = None
        for src, dst, cyc in self.MOVES:
            if self.st[src] is not None and self.rem[src] == 0 and self.st[dst] is None:
                ins = self.st[src]
                self.st[dst] = ins
                self.rem[dst] = self.info(ins)[2 + cyc]
                self.st[src] = None

    COMPLETE = (1, 4, 7, 8, 0, 2)
    FPU1 = 5
    MOVES = ((6, 7, 2), (5, 6, 1), (3, 4, 1))


class Sim603(Sim750):
    """8 stages: 0 BPU, 1 IU, 2 LSU1, 3 LSU2, 4 FPU1, 5 FPU2, 6 FPU3, 7 SRU; 5 in flight"""
    RING = 5

    def __init__(self, ops):
        super().__init__(ops)
        self.st = [None] * 8
        self.rem = [0] * 8

    LSU2 = 3

    def check(self, ins):
        if self.slots == 0:
            return False
        return self.st[self.info(ins)[0]] is None and self.store_ok(ins)

    def issue(self, ins):
        u, lat, s0, s1, s2, ser = self.info(ins)
        self.pending += 1
        self.slots -= 1
        self.ring[self.tail] = ins
        self.done[self.tail] = 0
        self.tail = (self.tail + 1) % self.RING
        self.st[u] = ins
        self.rem[u] = s0

    COMPLETE = (1, 3, 6, 7, 0)
    FPU1 = 4
    MOVES = ((5, 6, 2), (4, 5, 1), (2, 3, 1))


# ---- the scheduler ---------------------------------------------------------------------------------------------------
class Config:
    def __init__(self, **kw):
        self.model = '750'          # machine model
        self.mem = 'offset'         # 'object': one symbol = one object (1.x); 'offset': disjoint offsets independent
        self.maxh = 'block'         # 'block': max height reset per block (rayanht); 'global': kept across blocks
        self.same_cycle = 'all'     # zero-latency successor issues in the same cycle: 'all' | 'noload' | 'none'
        self.frame_alias = True     # pointer accesses alias stack (frame) objects
        self.rank = True            # opcode-rank tie-break (first pass, virtual registers)
        self.final_alias_all = False  # final pass: every memory access ordered
        self.cr_anti0 = True        # GC/2.6: CR anti / output dependences have latency 0 (1.x: producer latency)
        self.iu_conflict = True     # GC/2.6 750: the second integer unit's operand conflict check
        self.store_rule = True      # a store waits while LSU stage 2 holds a store
        self.frame_inorder = True   # prologue / epilogue instructions keep their relative order
        self.fold = 'target'                   # the forward peephole's addi fold after the first pass
        self.final_blocks = 'epilogue'  # final pass on: 'all' blocks | 'frame' only blocks that got frame code (the
                                     # first pass marks a block scheduled, flag 8, and the final pass skips it)
        for k, v in kw.items():
            setattr(self, k, v)

    def key(self):
        return 'model=%s mem=%s maxh=%s same=%s frame=%s rank=%s fall=%s cr0=%d iu=%d st=%d fin=%s' % (
            self.model, self.mem, self.maxh, self.same_cycle, int(self.frame_alias), int(self.rank),
            int(self.final_alias_all), self.cr_anti0, self.iu_conflict, self.store_rule, self.final_blocks)


class Node:
    __slots__ = ('ins', 'lat', 'height', 'succ', 'npred', 'earliest', 'latest', 'done', 'pos')

    def __init__(self, ins, lat, pos):
        self.ins, self.lat, self.height, self.pos = ins, lat, lat, pos
        self.succ = {}            # successor node -> edge latency
        self.npred = 0
        self.earliest = 0
        self.latest = 0
        self.done = False


class Scheduler:
    def __init__(self, tables, cfg):
        self.t = tables
        self.cfg = cfg
        self.model = tables.models[cfg.model]
        self.max_height = 0

    def latency(self, ins):
        info = self.model['ops'].get(ins.op)
        lat = info[1] if info else 1
        if lat >= 128:
            lat -= 256
        if ins.rec:
            lat += 2
        if ins.op in ('lmw', 'stmw'):
            lat += 1        # + PCode operand count - 2 (rD, d, rA)
        return lat

    def omit_class(self, r):
        """register classes whose anti / output dependences have latency 0 when the model says so: GPR, FPR (1.x);
        in GC/2.6 every class but SPR (class 0), so CR fields too"""
        if r in ('lr', 'ctr', 'xer'):
            return False
        if r.startswith('cr'):
            return self.cfg.cr_anti0
        return True

    def serialize(self, ins):
        info = self.model['ops'].get(ins.op)
        return bool(info and info[5])

    def add_dep(self, owner, dep, has_lat):
        if owner is dep:
            return
        v = owner.lat if has_lat else 0
        if dep in owner.succ:
            if v > owner.succ[dep]:
                owner.succ[dep] = v
                owner.height = max(owner.height, dep.height + v)
            return
        owner.succ[dep] = v
        dep.npred += 1
        owner.height = max(owner.height, dep.height + v)

    def build(self, block, final):
        cfg = self.cfg
        if cfg.maxh == 'block':
            self.max_height = 0
        nodes = [Node(ins, self.latency(ins), i) for i, ins in enumerate(block)]
        later_defs = collections.defaultdict(list)
        later_uses = collections.defaultdict(list)
        reads, writes, barriers, in_order = [], [], [], []
        later = []
        branch = None
        omit = self.model['omit_anti']
        for n in reversed(nodes):
            ins = n.ins
            # registers, operand order: defs first as in PCode (the destination is operand 0)
            for r in ins.defs:
                for m in later_uses[r]:
                    self.add_dep(n, m, True)
                for m in later_defs[r]:
                    anti_lat = not (omit and self.omit_class(r))
                    self.add_dep(n, m, anti_lat)
            for r in ins.uses:
                for m in later_defs[r]:
                    anti_lat = not (omit and self.omit_class(r))
                    self.add_dep(n, m, anti_lat)
            for r in ins.defs:
                later_defs[r].append(n)
            for r in ins.uses:
                later_uses[r].append(n)
            # memory
            if ins.mem and ins.mem[1] != '@const':
                kind, obj = ins.mem
                alias_all = final and cfg.final_alias_all
                for m in writes:
                    if alias_all or may_alias(obj, m.ins.mem[1], cfg):
                        self.add_dep(n, m, True)
                if kind == 'S':
                    for m in reads:
                        if alias_all or may_alias(obj, m.ins.mem[1], cfg):
                            self.add_dep(n, m, True)
                    writes.append(n)
                else:
                    reads.append(n)
            # frame code keeps its order (the 0x80 instruction list of GC/2.6: each depends on every later one, 0)
            if cfg.frame_inorder and ins.phys == 'frame':
                for m in in_order:
                    self.add_dep(n, m, cfg.frame_inorder == 'lat')
                in_order.append(n)
            # barriers
            if self.serialize(ins):
                for m in later:
                    self.add_dep(n, m, False)
                barriers.append(n)
            for b in barriers:
                if b is not n:
                    self.add_dep(n, b, False)
            if not n.succ and branch is not None:
                self.add_dep(n, branch, False)
            if ins.op.startswith('b') and ins.op not in ('bl', 'bctrl', 'blrl'):
                branch = n
            if n.height > self.max_height:
                self.max_height = n.height
            later.append(n)
        for n in nodes:
            n.latest = self.max_height - n.height
        return nodes

    def release_count(self, n):
        return sum(1 for m in n.succ if m.npred == 1)

    def schedule(self, block, final=False, trace=None):
        """block: list of Ins in program order -> scheduled list of Ins"""
        if len(block) <= 2:
            return list(block)
        cfg = self.cfg
        nodes = self.build(block, final)
        sim = (Sim750 if cfg.model == '750' else Sim603)(self.model['ops'])
        sim.iu_conflict = cfg.iu_conflict
        sim.store_rule = cfg.store_rule
        use_rank = cfg.rank and not final
        left = list(nodes)
        out = []
        cyc = 0
        while left:
            slot = 0
            while slot < self.model['width'] and left:
                best = None
                for c in left:
                    if c.npred or c.earliest > cyc or not sim.check(c.ins):
                        continue
                    if best is None:
                        best = c
                        continue
                    b_urg, c_urg = best.latest <= cyc, c.latest <= cyc
                    if b_urg and not c_urg:
                        continue
                    if c_urg and not b_urg:
                        best = c
                        continue
                    cc, bc = self.release_count(c), self.release_count(best)
                    if cc != bc:
                        if cc > bc:
                            best = c
                        continue
                    if c.height != best.height:
                        if c.height > best.height:
                            best = c
                        continue
                    if use_rank and self.t.rank.get(c.ins.op, 9) < self.t.rank.get(best.ins.op, 9):
                        best = c
                if best is None:
                    break
                if trace is not None:
                    cands = [c for c in left if not c.npred and c.earliest <= cyc]
                    trace.append('cycle %d pick %-32s from %s' % (cyc, best.ins.text, '; '.join(
                        '%s h%d d%d r%d' % (c.ins.text, c.height, c.latest, self.release_count(c)) for c in cands)))
                for m, v in best.succ.items():
                    m.npred -= 1
                    e = v
                    if v == 0 and final and cfg.same_cycle == 'noload' and is_load(m.ins.op):
                        e = 1
                    if v == 0 and cfg.same_cycle == 'none':
                        e = 1
                    m.earliest = max(m.earliest, cyc + e)
                out.append(best.ins)
                sim.issue(best.ins)
                left.remove(best)
                slot += 1
            sim.advance()
            cyc += 1
            if cyc > 100000:
                out += [n.ins for n in left]
                break
        return out


# ---- listings, blocks, comparison ------------------------------------------------------------------------------------
def is_block_end(text):
    mn = text.split(None, 1)[0]
    op, _ = pcop(mn)
    return op.startswith('b') and op not in ('bl', 'bctrl', 'blrl')


def blocks_of(lines):
    """listing lines (with 'LBL:' markers) -> list of blocks (lists of instruction text)"""
    out, cur = [], []
    for l in lines:
        if l.startswith('LBL:'):
            if cur:
                out.append(cur)
            cur = []
            continue
        cur.append(l)
        if is_block_end(l):
            out.append(cur)
            cur = []
    if cur:
        out.append(cur)
    return out


def mask(text):
    t = re.sub(r'\b[rf](?:\d+in|v\d+|\d+)\b', lambda m: m.group(0)[0] if m.group(0) not in ('r1', 'r2', 'r13') else m.group(0), text)
    t = re.sub(r'\.L_[0-9A-Fa-f]+|lbl_[0-9A-Fa-f]+(?=@)', 'LBL', t)
    return t


def frame_code(block, first, last):
    """indices of the prologue / epilogue instructions of a block (generated after register colouring)"""
    out = set()
    for i, t in enumerate(block):
        ins = Ins(t)
        op, ops = ins.op, ins.ops
        m = MEM.match(ops[1]) if len(ops) > 1 else None
        on_r1 = bool(m and m.group(2) == 'r1')
        saved = bool(ops and re.match(r'^[rf]\d+$', ops[0]) and int(ops[0][1:]) >= 14)
        if first:
            if op == 'stwu' and on_r1:
                out.add(i)
            elif op == 'mflr':
                out.add(i)
            elif op in ('stw', 'stfd', 'psq_st', 'stmw') and on_r1 and (saved or (op == 'stw' and ops[0] == 'r0' and i > 0
                                                                                 and 'mflr' in block[i - 1:i] + block[max(0, i - 2):i])):
                out.add(i)
        if last:
            if op in ('lwz', 'lfd', 'psq_l', 'lmw') and on_r1 and saved:
                out.add(i)
            elif op == 'lwz' and on_r1 and ops[0] == 'r0' and any(b.startswith('mtlr r0') for b in block[i + 1:]):
                out.add(i)
            elif op == 'mtlr':
                out.add(i)
            elif op == 'addi' and ops[:2] == ['r1', 'r1']:
                out.add(i)
    return out


VOLATILE = re.compile(r'^(r(0|[3-9]|1[0-2])|f([0-9]|1[0-3]))$')


def rename_virtual(block):
    """each definition of a volatile register (r0, r3-r12, f0-f13) becomes a new virtual register, as in the first
    pass; callee-saved registers (the level-0 locals) keep their name"""
    cur, n, out = {}, itertools.count(), []
    for t in block:
        ins = Ins(t)
        mn, ops = ins.mn, list(ins.ops)

        def sub_use(o):
            def rep(m):
                r = m.group(0)
                if VOLATILE.match(r):
                    if r not in cur:
                        cur[r] = r + 'in'
                    return cur[r]
                return r
            return re.sub(r'\b[rf]\d+\b', rep, o)
        defs = set(ins.defs)
        new = []
        for i, o in enumerate(ops):
            m = MEM.match(o)
            if m:
                base = m.group(2)
                if base == 'r0':
                    new.append(o)
                else:
                    new.append(m.group(1) + '(' + sub_use(base) + ')')
            elif i == 0 and REG.match(o) and o in defs and o not in ins.uses:
                new.append(o)          # renamed below
            else:
                new.append(sub_use(o) if REG.match(o) else o)
        for i, o in enumerate(ops):
            if i == 0 and REG.match(o) and o in defs and VOLATILE.match(o):
                v = '%sv%d' % (o[0], next(n))
                cur[o] = v
                new[0] = v
            elif i == 0 and REG.match(o) and o in defs and o in ins.uses and VOLATILE.match(o):
                v = '%sv%d' % (o[0], next(n))
                cur[o] = v
                new[0] = v
        # update forms and multi-defs other than operand 0
        out.append(mn + (' ' + ', '.join(new) if new else ''))
    return out


def score(a, b):
    """masked sequences a (replay) and b (compiler): equal positions, longest common subsequence"""
    pos = sum(1 for x, y in zip(a, b) if x == y)
    import difflib
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    lcs = sum(bl.size for bl in sm.get_matching_blocks())
    return pos, lcs, max(len(a), len(b))


def body_blocks(listing):
    bl = blocks_of(listing)
    out = []
    for i, b in enumerate(bl):
        fr = frame_code(b, i == 0, b and b[-1].startswith('blr'))
        out.append(([t for k, t in enumerate(b) if k not in fr], [t for k, t in enumerate(b) if k in fr], fr))
    return out


def fold_addi(order, func_text, allow=None):
    """the forward peephole after the first pass (virtual registers): `addi P, B, c` whose result is used only as the
    base of D-form loads / stores is folded into them (`d(P)` -> `c+d(B)`) and dropped; P keeps its saved register
    when it is a local (docs/drafts.md "Step 85" rule 2)"""
    out = list(order)
    for i, ins in enumerate(order):
        if ins.op != 'addi' or len(ins.ops) != 3 or to_int(ins.ops[2]) is None:
            continue
        if allow is not None and not allow(ins):
            continue
        P, B = ins.ops[0], ins.ops[1]
        if P == B:
            continue
        users = [j for j in range(i + 1, len(order)) if P in order[j].uses or P in order[j].defs]
        if not users or sum(1 for t in func_text if re.search(r'\b%s\b' % re.escape(P), t)) > len(users) + 1:
            continue
        ok = True
        for j in users:
            u = order[j]
            m = MEM.match(u.ops[1]) if len(u.ops) > 1 else None
            if P in u.defs or not m or m.group(2) != P or to_int(m.group(1)) is None or u.ops[0] == P:
                ok = False
                break
        # B must not be redefined between the addi and the last use
        if ok and any(B in order[j].defs for j in range(i + 1, users[-1] + 1)):
            ok = False
        if not ok:
            continue
        c = to_int(ins.ops[2])
        for j in users:
            u = out[j]
            m = MEM.match(u.ops[1])
            ops = list(u.ops)
            ops[1] = '%s(%s)' % (hex(c + to_int(m.group(1))), B)
            out[j] = Ins(u.mn + ' ' + ', '.join(ops), u.idx)
            out[j].mem = u.mem
        out[i] = None
    return [x for x in out if x is not None]


def run_pass1(sched, listing):
    """first pass over the body of every block of a no-schedule listing (virtual registers, no frame code), then the
    forward peephole's addi fold when the configuration asks for it; returns instruction text per block (virtual
    register names: compare masked)"""
    res = []
    bodies = body_blocks(listing)
    vtexts = [rename_virtual(b) for b, _, _ in bodies]
    allv = [t for v in vtexts for t in v]
    for v in vtexts:
        ins = [Ins(t, i) for i, t in enumerate(v)]
        annotate_memory(ins)
        order = sched.schedule(ins, final=False)
        if sched.cfg.fold == 'virtual':
            order = fold_addi(order, allv)
        res.append([x.text for x in order])
    return res


def run_final(sched, block_texts, frames=None):
    out = []
    for k, b in enumerate(block_texts):
        fb = sched.cfg.final_blocks
        if frames is not None and ((fb == 'frame' and not frames[k]) or
                                   (fb == 'epilogue' and not (frames[k] and b and b[-1].startswith('blr')))):
            out.append(list(b))
            continue
        ins = [Ins(t, i) for i, t in enumerate(b)]
        annotate_memory(ins)
        for i in frame_code(b, k == 0, bool(b) and b[-1].startswith('blr')):
            ins[i].phys = 'frame'
            if ins[i].mem:
                ins[i].mem = (ins[i].mem[0], None)     # no alias: the worst case, ordered against every access
        out.append([x.text for x in sched.schedule(ins, final=True)])
    return out


def map_registers(vtexts, ptexts):
    """give each first-pass instruction (virtual registers) the target's instruction text: a one-to-one matching of
    equal masked text under one consistent virtual -> physical register map (backtracking); unmatched ones keep
    their text"""
    regs = re.compile(r'\b[rf](?:\d+in|v\d+|\d+)\b')
    cand = collections.defaultdict(list)
    for j, t in enumerate(ptexts):
        cand[mask(t)].append(j)
    order = sorted(range(len(vtexts)), key=lambda i: len(cand.get(mask(vtexts[i]), [])))
    used, vmap, res = set(), {}, {}
    budget = [20000]

    def ok(i, j):
        a, b = regs.findall(vtexts[i]), regs.findall(ptexts[j])
        if len(a) != len(b):
            return None
        new = {}
        for x, y in zip(a, b):
            if vmap.get(x, new.get(x, y)) != y:
                return None
            if x not in vmap:
                new[x] = y
        return new

    def go(k):
        budget[0] -= 1
        if k == len(order) or budget[0] < 0:
            return True
        i = order[k]
        for j in cand.get(mask(vtexts[i]), []):
            if j in used:
                continue
            new = ok(i, j)
            if new is None:
                continue
            used.add(j); vmap.update(new); res[i] = j
            if go(k + 1):
                return True
            used.discard(j); res.pop(i, None)
            for x in new:
                vmap.pop(x, None)
        if not cand.get(mask(vtexts[i])) or all(j in used for j in cand[mask(vtexts[i])]):
            return go(k + 1)
        return False

    go(0)
    return [ptexts[res[i]] if i in res else vtexts[i] for i in range(len(vtexts))]


def fold_to_target(p1, tbody):
    """per use: `d(P)` of `addi P, B, c` becomes `c+d(B)` where the target has that form to spare; the addi goes
    when the target no longer has it (after colouring the fold needs B's register alive at the use)"""
    tm = collections.Counter(mask(t) for t in tbody)
    pm = collections.Counter(mask(t) for t in p1)
    out = list(p1)
    ins = [Ins(t, i) for i, t in enumerate(p1)]
    for i, a in enumerate(ins):
        if a.op != 'addi' or len(a.ops) != 3 or to_int(a.ops[2]) is None or a.ops[0] == a.ops[1]:
            continue
        P, B, c = a.ops[0], a.ops[1], to_int(a.ops[2])
        for j in range(i + 1, len(ins)):
            u = ins[j]
            if P in u.defs and P not in u.uses:
                break
            m = MEM.match(u.ops[1]) if len(u.ops) > 1 else None
            if not m or m.group(2) != P or to_int(m.group(1)) is None or u.ops[0] == P:
                continue
            ops = list(u.ops)
            ops[1] = '%s(%s)' % (hex(c + to_int(m.group(1))), B)
            f = u.mn + ' ' + ', '.join(ops)
            if tm[mask(f)] > pm[mask(f)]:
                pm[mask(f)] += 1
                pm[mask(out[j])] -= 1
                out[j] = f
        if pm[mask(a.text)] > tm[mask(a.text)] and not any(re.search(r'\b%s\b' % re.escape(P), t) for t in out[i + 1:]):
            pm[mask(a.text)] -= 1
            out[i] = None
    return [t for t in out if t is not None]


def two_pass(sched_final, p1_blocks, target_blocks):
    """the replay of both passes: the first pass's order with the compiler's physical registers (each instruction
    mapped onto the target's instruction by map_registers, the post-colouring addi fold by fold_to_target) and the
    frame code of the target put back (prologue first, epilogue before the block's last instruction); run_final then
    runs the final pass on the blocks it reschedules"""
    out = []
    for p1, (tbody, tframe, fr) in zip(p1_blocks, target_blocks):
        if sched_final.cfg.fold == 'target':
            # the addi fold runs after colouring (it needs the base's physical register to survive to the last use):
            # fold the addi the target no longer has
            p1 = fold_to_target(p1, tbody)
        phys = map_registers(p1, tbody)
        pro = [t for t in tframe if not t.startswith(('lwz', 'lfd', 'psq_l', 'lmw', 'mtlr', 'addi'))]
        epi = [t for t in tframe if t not in pro]
        blk = pro + phys
        if epi:
            last = blk.pop() if blk and is_block_end(blk[-1]) else None
            blk = blk + epi + ([last] if last else [])
        out.append(blk)
    return out


# ---- the corpus measurement ------------------------------------------------------------------------------------------
ABLATIONS = [
    ('the GC/2.6 model (default)', {}),
    ('603 model (the 1.2.5 gekko fallback)', {'model': '603'}),
    ('no second-integer-unit operand check (1.x 750)', {'iu_conflict': False}),
    ('no store-after-store LSU rule', {'store_rule': False}),
    ('one symbol = one object (1.x alias rule)', {'mem': 'object'}),
    ('max height kept across blocks', {'maxh': 'global'}),
    ('zero-latency successors wait a cycle', {'same_cycle': 'none'}),
    ('final pass: loads after a 0-latency edge wait', {'same_cycle': 'noload'}),
    ('no opcode-rank tie-break', {'rank': False}),
    ('CR anti dependences keep latency (1.x)', {'cr_anti0': False}),
    ('final pass on every block', {'final_blocks': 'all'}),
    ('final pass on every block with frame code', {'final_blocks': 'frame'}),
    ('frame code not kept in order', {'frame_inorder': False}),
    ('final pass orders every memory access', {'final_alias_all': True}),
    ('addi fold in the first pass (virtual registers)', {'fold': 'virtual'}),
    ('no addi fold', {'fold': False}),
]


def configs():
    for name, kw in ABLATIONS:
        yield name, Config(**kw)


def measure(man, tables, cfg, detail=None):
    tot = collections.Counter()
    per_file = {}
    for fid, ent in sorted(man['files'].items()):
        if not ent['ok']:
            continue
        fc = collections.Counter()
        s1 = Scheduler(tables, cfg)
        s2 = Scheduler(tables, cfg)
        for fn, lst in ent['functions'].items():
            nb, sb = body_blocks(lst['nosched']), body_blocks(lst['sched'])
            if len(nb) != len(sb):
                fc['shape_mismatch'] += 1
                continue
            p1 = run_pass1(s1, lst['nosched'])
            for k, (r, (tb, tf, _), (nbody, _, _)) in enumerate(zip(p1, sb, nb)):
                a, b, c = [mask(t) for t in r], [mask(t) for t in tb], [mask(t) for t in nbody]
                if len(b) <= 2 or tf:
                    continue
                same_set = sorted(a) == sorted(b)
                fc['p1_blocks'] += 1
                fc['p1_comparable'] += same_set
                fc['p1_exact'] += (a == b)
                p, l, n = score(a, b)
                fc['p1_pos'] += p
                fc['p1_words'] += n
                p0, _, _ = score(c, b)
                fc['base_pos'] += p0
                if detail is not None and a != b:
                    detail.append((fid, fn, k, a, b, c))
            full = two_pass(s2, p1, sb)
            fin = run_final(s2, full, [bool(f) for _, f, _ in sb])
            tgt = blocks_of(lst['sched'])
            for r, b in zip(fin, tgt):
                if len(b) <= 2:
                    continue
                a2, b2 = [mask(t) for t in r], [mask(t) for t in b]
                fc['p2_blocks'] += 1
                fc['p2_exact'] += (a2 == b2)
                p, l, n = score(a2, b2)
                fc['p2_pos'] += p
                fc['p2_words'] += n
            fix = run_final(s2, tgt, [bool(f) for _, f, _ in sb])
            for r, b in zip(fix, tgt):
                if len(b) <= 2:
                    continue
                fc['fix_blocks'] += 1
                fc['fix_exact'] += ([mask(t) for t in r] == [mask(t) for t in b])
        per_file[fid] = fc
        tot.update(fc)
    return tot, per_file


def originals(man, tables, cfg, out):
    """the targets: our best C's no-schedule listing through both passes against the original's words"""
    for fn, orig in sorted(man['originals'].items()):
        s = Scheduler(tables, cfg)
        ob = blocks_of(orig)
        frames = [bool(f) for _, f, _ in body_blocks(orig)]
        fin = run_final(s, ob, frames)
        moved = [k for k, (x, y) in enumerate(zip(fin, ob)) if x != y]
        out.append('%s original: the final pass replayed on the original words is %s' % (
            fn, 'a fixed point (%d words)' % sum(1 for x in orig if not x.startswith('LBL:')) if not moved else 'not a fixed point in block(s) %s' % moved))
    pairs = [('hd_base', 'cr_CM_DUMMY_008'), ('hd_v0', 'cr_CM_DUMMY_008'), ('fr_v13', 'fn_8006EEF8')]
    for fid, fn in pairs:
        ent = man['files'].get(fid)
        orig = man['originals'].get(fn)
        if not ent or not ent['ok'] or not orig or fn not in ent['functions']:
            continue
        lst = ent['functions'][fn]
        s = Scheduler(tables, cfg)
        p1 = run_pass1(s, lst['nosched'])
        ob = body_blocks(orig)
        sb = body_blocks(lst['sched'])
        if len(ob) != len(p1):
            out.append('%s %s: block count differs (%d / %d)' % (fid, fn, len(p1), len(ob)))
            continue
        fin = run_final(s, two_pass(s, p1, ob), [bool(f) for _, f, _ in ob])
        ot = blocks_of(orig)
        st = blocks_of(lst['sched'])
        A = [mask(t) for b in fin for t in b]
        B = [mask(t) for b in ot for t in b]
        C = [mask(t) for b in st for t in b]
        p, l, n = score(A, B)
        pc, lc, nc = score(C, B)
        P1 = [mask(t) for b in p1 for t in b]
        OB = [mask(t) for b, _, _ in ob for t in b]
        SB = [mask(t) for b, _, _ in sb for t in b]
        q, _, m = score(P1, OB)
        qc, _, mc = score(P1, SB)
        ex = sum(1 for x, y in zip(fin, ot) if [mask(t) for t in x] == [mask(t) for t in y] and len(y) > 2)
        nb = sum(1 for y in ot if len(y) > 2)
        out.append('%s -> %s: replay (both passes) vs original %d/%d words by position (masked), %d/%d blocks exact; '
                   'the compiler on the same C: %d/%d; first pass alone vs original body %d/%d, vs the compiler body %d/%d'
                   % (fid, fn, p, n, ex, nb, pc, nc, q, m, qc, mc))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--corpus', action='store_true')
    ap.add_argument('--report', default=os.path.join(HERE, 'corpus_report.txt'))
    ap.add_argument('--trace', nargs='+')
    ap.add_argument('--model', default=None)
    a = ap.parse_args()
    tables = Tables()
    man = json.load(open(os.path.join(CORPUS, 'manifest.json'), encoding='utf-8'))
    if a.trace:
        ent = man['files'][a.trace[0]]
        fn = a.trace[1] if len(a.trace) > 1 else next(iter(ent['functions']))
        lst = ent['functions'][fn]
        cfg = Config(**({'model': a.model} if a.model else {}))
        s = Scheduler(tables, cfg)
        for body, frame, _ in body_blocks(lst['nosched']):
            v = rename_virtual(body)
            ins = [Ins(t, i) for i, t in enumerate(v)]
            annotate_memory(ins)
            tr = []
            order = s.schedule(ins, trace=tr)
            print('--- block (%d)' % len(ins))
            for i in ins:
                print('   %-40s mem=%s' % (i.text, i.mem))
            for l in tr:
                print('   ' + l)
        return
    if not a.corpus:
        ap.print_help()
        return
    rows = []
    best = None
    for name, cfg in configs():
        tot, per = measure(man, tables, cfg)
        rows.append((name, cfg, tot, per))
        if best is None:
            best = (None, cfg, tot, per)
    lines = ['Scheduler replay over the corpus (tools/sched/replay.py --corpus; corpus: tools/sched/corpus.py, %d files)'
             % len(man['files']), '',
             'p1 = first pass replayed on the no-schedule listing (volatile registers renamed to virtual ones, frame '
             'code removed) vs the compiler\'s scheduled listing without its frame code; base = the no-schedule order '
             'itself; p2 = both passes (first pass, the compiler\'s physical registers, frame code, final pass) vs the '
             'scheduled listing; fix = the final pass run on the scheduled listing (a fixed point if the model is '
             'right). Blocks of 3+ instructions; words masked (register numbers).', '']
    lines.append('%-50s %12s %14s %14s %12s %14s %10s' % ('configuration (one switch changed from the default)',
                                                          'p1 exact', 'p1 words', 'base words', 'p2 exact', 'p2 words',
                                                          'fix exact'))
    for name, cfg, t, _ in rows:
        lines.append('%-50s %5d/%-6d %6d/%-7d %6d/%-7d %5d/%-6d %6d/%-7d %4d/%-5d' % (
            name, t['p1_exact'], t['p1_blocks'], t['p1_pos'], t['p1_words'], t['base_pos'], t['p1_words'],
            t['p2_exact'], t['p2_blocks'], t['p2_pos'], t['p2_words'], t['fix_exact'], t['fix_blocks']))
    _, cfg, tot, per = best
    lines += ['', 'Default configuration: ' + cfg.key(), '',
              '%-10s %12s %12s %12s %12s %12s' % ('file', 'p1 exact', 'p1 words', 'base words', 'p2 exact', 'fix exact')]
    for fid, t in sorted(per.items()):
        lines.append('%-10s %5d/%-6d %5d/%-6d %5d/%-6d %5d/%-6d %5d/%-6d' % (
            fid, t['p1_exact'], t['p1_blocks'], t['p1_pos'], t['p1_words'], t['base_pos'], t['p1_words'],
            t['p2_exact'], t['p2_blocks'], t['fix_exact'], t['fix_blocks']))
    bad = [k for k, v in man['files'].items() if not v['ok']]
    lines += ['', 'Not compiled: ' + (', '.join(bad) or 'none'), '', 'The targets (default configuration):']
    originals(man, tables, cfg, lines)
    with open(a.report, 'w', encoding='utf-8', newline='\r\n') as fh:
        fh.write('\n'.join(lines) + '\n')
    print('\n'.join(lines[:12] + lines[-20:]))


if __name__ == '__main__':
    main()
