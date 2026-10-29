"""Assert / panic calls of main.dol from the instruction bytes: (function, file string, expression string) by address.

Used by tools/match_lib_hsd.py (rule R4) instead of the decompiled C, so its result depends only on main.dol and the
Ghidra export's structure (call sites and computed-jump targets by address, decompiled/xrefs/refs.tsv), never on which
names or prototypes the rebuild applied, nor on how Ghidra printed the arguments.
For every call (bl) to an assert function, the values of r3 (file) and r5 (expression) at the call come from constant
propagation over the function's control-flow graph (PowerPC li / lis / addi / ori / mr; r2 / r13 = the small-data
bases set by __init_registers; a call clobbers r0, r3-r12). A call whose r3 or r5 is not the same constant on every
path, or does not point at a string, is left out. Library module, no command line.
"""
import struct

VARY = object()   # the register holds different or unknown values


class Dol:
    def __init__(self, path):
        self.d = open(path, 'rb').read()
        self.off = struct.unpack_from('>18I', self.d, 0)
        self.adr = struct.unpack_from('>18I', self.d, 0x48)
        self.sz = struct.unpack_from('>18I', self.d, 0x90)

    def word(self, a):
        for o, b, s in zip(self.off, self.adr, self.sz):
            if s and b <= a <= b + s - 4: return struct.unpack_from('>I', self.d, o + a - b)[0]
        return None

    def cstr(self, addr):
        for o, a, s in zip(self.off, self.adr, self.sz):
            if s and a <= addr < a + s:
                f = o + addr - a
                return self.d[f:self.d.find(b'\0', f)].decode('latin-1')
        return None


def _s16(x): return x - 0x10000 if x & 0x8000 else x


# primary opcode 31: these extended opcodes write rA (logical / shift / extend) or no GPR at all (compare, store,
# floating-point load, cache, sync, mt*); every other one writes rD
_LOGIC31 = {24, 26, 28, 60, 124, 284, 316, 412, 444, 476, 536, 792, 824, 922, 954}
# (535 / 567 / 599 / 631 = lfsx / lfsux / lfdx / lfdux load a floating-point register)
_NODEST31 = {0, 4, 32, 54, 86, 144, 146, 150, 151, 183, 210, 215, 242, 246, 247, 278, 306, 370, 407, 438, 439, 467,
             470, 535, 566, 567, 598, 599, 631, 661, 662, 663, 695, 725, 727, 759, 854, 914, 918, 978, 982, 983, 1014}
_UPDATE31 = {55, 119, 183, 247, 311, 375, 439, 503, 567, 599, 631, 663, 695, 727, 759}   # indexed load/store with update


def _step(w, st):
    """apply one non-branch instruction to the register state st (reg -> int or VARY), in place"""
    op, rd, ra, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xffff
    if op in (14, 15):                                   # addi / addis (li / lis when rA = 0)
        base = 0 if ra == 0 else st.get(ra, VARY)
        st[rd] = VARY if base is VARY else (base + ((imm << 16) if op == 15 else _s16(imm))) & 0xffffffff
    elif op == 24:                                       # ori rA, rS, imm
        s = st.get(rd, VARY)
        st[ra] = VARY if s is VARY else s | imm
    elif op == 31:
        xo = (w >> 1) & 0x3ff
        if xo == 444 and rd == (w >> 11) & 31: st[ra] = st.get(rd, VARY)   # mr rA, rS
        elif xo in _LOGIC31: st[ra] = VARY
        elif xo not in _NODEST31: st[rd] = VARY
        if xo in _UPDATE31: st[ra] = VARY
    elif op in (20, 21, 23, 25, 26, 27, 28, 29): st[ra] = VARY           # rotate / logical immediate: write rA
    elif op == 46:                                                       # lmw rD..r31
        for r in range(rd, 32): st[r] = VARY
    elif op in (7, 8, 12, 13, 32, 33, 34, 35, 40, 41, 42, 43):           # mulli, subfic, addic(.), loads
        st[rd] = VARY
        if op in (33, 35, 41, 43): st[ra] = VARY
    elif op in (37, 39, 45, 49, 51, 53, 55): st[ra] = VARY               # store / fp load with update
    # everything else (stores, compares, floating point, paired singles, traps) writes no GPR


def _clobber(st):
    for r in (0,) + tuple(range(3, 13)): st[r] = VARY


def _merge(a, b):
    if a is None: return {r: v for r, v in b.items() if v is not VARY}
    return {r: v for r, v in a.items() if b.get(r, VARY) == v}


def _branch(w, at):
    """-> (targets, falls through, is a call) for a branch instruction, None for any other"""
    op = w >> 26
    if op == 18:
        t = w & 0x3fffffc
        t = t - 0x4000000 if t & 0x2000000 else t
        t = t if w & 2 else (at + t) & 0xffffffff
        return ([], True, True) if w & 1 else ([t], False, False)
    if op == 16:
        t = _s16(w & 0xfffc)
        t = t if w & 2 else (at + t) & 0xffffffff
        return ([], True, True) if w & 1 else ([t], ((w >> 21) & 0x14) != 0x14, False)
    if op == 19 and ((w >> 1) & 0x3ff) in (16, 528):    # bclr / bcctr (computed-jump targets come from the caller)
        if w & 1: return [], True, True
        return [], ((w >> 21) & 0x14) != 0x14, False
    return None


def small_data_bases(dol, init_registers):
    """{2: r2, 13: r13} from running __init_registers (start, size) straight through"""
    st = {}
    s, n = init_registers
    for a in range(s, s + n, 4):
        w = dol.word(a)
        if w is None or _branch(w, a): break
        _step(w, st)
    return {r: st[r] for r in (2, 13) if st.get(r, VARY) is not VARY}


def assert_calls(dol, functions, call_sites, computed_jumps, entry, trace=None):
    """functions: {start: size}; call_sites: {function start: [call address, ...]} (the calls to assert functions);
    computed_jumps: {bctr address: [target, ...]}; entry: known registers at every function entry (small_data_bases).
    -> {(function, r3, r5)} for every call where both are one constant on every path and point at a string"""
    out = set()
    for f in sorted(call_sites):
        end = f + functions.get(f, 0)
        words = {a: dol.word(a) for a in range(f, end, 4)}
        if not words or any(w is None for w in words.values()): continue
        lead = {f}
        for a, w in words.items():
            br = _branch(w, a)
            if br is None: continue
            lead.update(t for t in br[0] + computed_jumps.get(a, []) if f <= t < end)
            if a + 4 < end: lead.add(a + 4)
        lead = sorted(lead)
        blocks, succ = {}, {}
        for i, b0 in enumerate(lead):
            b1 = lead[i + 1] if i + 1 < len(lead) else end
            blocks[b0] = b1
            br = _branch(words[b1 - 4], b1 - 4)
            if br is None: succ[b0] = [b1] if b1 < end else []
            else:
                tg = [t for t in br[0] + computed_jumps.get(b1 - 4, []) if f <= t < end]
                succ[b0] = tg + ([b1] if br[1] and b1 < end else [])

        def run(b0, st, stop=None):
            st = dict(st)
            for a in range(b0, blocks[b0], 4):
                if a == stop: break
                br = _branch(words[a], a)
                if br is None: _step(words[a], st)
                elif br[2]: _clobber(st)
            return st
        state_in = {b: (dict(entry) if b == f else None) for b in blocks}   # blocks nothing reaches stay None
        todo = [b for b in lead if state_in[b] is not None]
        while todo:
            b = todo.pop(0)
            st_out = run(b, state_in[b])
            for t in succ[b]:
                m = _merge(state_in[t], st_out)
                if m != state_in[t]:
                    state_in[t] = m
                    if t not in todo: todo.append(t)
        for at in sorted(call_sites[f]):
            b = max(x for x in blocks if x <= at)
            if state_in[b] is None:
                if trace is not None: trace[at] = 'unreachable block %08x' % b
                continue
            st = run(b, state_in[b], stop=at)
            r3, r5 = st.get(3, VARY), st.get(5, VARY)
            if trace is not None: trace[at] = {r: '%08x' % v for r, v in st.items() if v is not VARY}
            if r3 is VARY or r5 is VARY or dol.cstr(r3) is None or dol.cstr(r5) is None: continue
            out.add((f, r3, r5))
    return out
