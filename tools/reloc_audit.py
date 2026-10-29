"""Relocation audit of the Phase 4 build: which pointers in main.dol did decomp-toolkit's analysis miss (or invent)?

Plan.md Phase 4 Milestone 2 (a shiftable build). dtk turns every pointer it finds into a relocation (a symbol
reference) in its split objects; mwld keeps them all in main.elf (.rela.* sections, absolute addresses). Code and data
can only move if EVERY pointer is a relocation and NO non-pointer is one. This module finds both kinds of mistake from
main.dol's own bytes, independently of dtk:

  M = the moving range: 0x80003100 (start of .init) up to __ArenaLo (stack, debug stack and arena start are linker
      symbols placed after .sbss2, so they move too). Values below (low memory 0x80000000-0x800030FF, the OS globals)
      and above (0x81700000 __ArenaHi, hardware 0xCC00xxxx) never move.

  Code (every function symbol in .init / .text): constant propagation over the function's control-flow graph
  (dol_asserts' PowerPC model, extended with provenance: which instruction made the high half, which relocation
  anchored the value). Jump-table targets (ADDR32 relocations into the function) and blocks no path reaches start
  from the entry state (r2 / r13 = the small-data bases, everything else unknown). Events:
    MAT   addi / ori / addis making a value in M from a raw lis (hi) -> the lis needs an @ha/@h relocation and this
          instruction an @l one ("lis/addi pair"; split across blocks is covered because the state flows across them);
    EA    a D-form load / store whose effective address is in M: from a raw lis -> both instructions need
          relocations; from r2 / r13 -> an @sda21 relocation (small-data access);
    SDAADDR  addi rD, r2/r13, imm (the address of a small-data object) -> @sda21;
    CROSS a value anchored by a relocation to object S (+ offset arithmetic, e.g. lis/addi of S then lwz 0x120(rX))
          that lands in a DIFFERENT dtk object T: not a miss, the code builds T's address arithmetically from S, so S
          and T must stay together ("glued": padding between them would break it).
    LOGIC / SCMP  a relocated value used as a number: an operand of and / or / xor / nor / ... or of a signed
          compare (cmpw) -> a false positive (a bit mask that happens to look like an address).
  Data (.ctors .dtors .rodata .data .sdata .sdata2, every 4-aligned word): a word whose value is in M and that has
  no ADDR32 relocation -> RAW (a candidate miss; on this DOL there are none, and gen_dtk_relocs.py stops if one
  appears, so it gets looked at before any rule is written for it).
  classify(): a data relocation into the middle of a function is a jump table only if all its cases lie in one
  function that has a bctr and references the table (or the jumptable_ right before it); otherwise it is a false
  positive. CROSS spans are merged into glue spans. consistency(): every relocated value the code or data holds
  equals its relocation target (used on the shifted build by tools/shift_test.py).
tools/gen_dtk_relocs.py turns misses and false positives into config.yml add_relocations / block_relocations, so
nothing is edited by hand.

Library module + command line:  python -I tools/reloc_audit.py [main.elf]
(default: decomp/build/GXCE01/main.elf -> summary on stdout).
"""
import bisect
import os
import struct
import sys
from collections import defaultdict

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import dol_asserts as da  # noqa: E402  PowerPC model (branch decoding)

# relocation types (ELF PowerPC ABI)
R_ADDR32, R_LO, R_HI, R_HA, R_REL24, R_REL14, R_SDA21 = 1, 4, 5, 6, 10, 11, 109
CODE = ('.init', '.text')
DATA = ('.ctors', '.dtors', '.rodata', '.data', '.sdata', '.sdata2')
DFORM_LOAD = set(range(32, 56)) - {46, 47}     # lwz .. stfdu (lmw / stmw handled separately)
# and / andc / xor / nor / eqv / orc / or / nand (opcode 31): a relocated address used as a bit mask is no address
LOGIC31 = {28, 60, 124, 284, 316, 412, 444, 476}


class Elf:
    """the parts of a linked mwld ELF we need: sections, symbols, relocations (absolute addresses)"""

    def __init__(self, path):
        d = self.d = open(path, 'rb').read()
        if d[:4] != b'\x7fELF' or d[4] != 1 or d[5] != 2: raise ValueError(f'{path}: not a 32-bit big-endian ELF')
        shoff, = struct.unpack_from('>I', d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from('>HHH', d, 0x2E)
        sh = [struct.unpack_from('>10I', d, shoff + i * shentsize) for i in range(shnum)]
        strtab = sh[shstrndx]

        def nm(tab, off):
            o = tab[4] + off
            return d[o:d.index(b'\0', o)].decode('latin-1')
        self.sections = []   # (index, name, type, addr, size, offset, flags)
        for i, s in enumerate(sh):
            self.sections.append((i, nm(strtab, s[0]), s[1], s[3], s[5], s[4], s[2]))
        symsec = next(s for s in sh if s[1] == 2)
        symstr = sh[symsec[6]]
        self.syms = []       # (name, value, size, type, bind, shndx)
        for o in range(symsec[4], symsec[4] + symsec[5], 16):
            n, v, sz, info, other, shn = struct.unpack_from('>IIIBBH', d, o)
            self.syms.append((nm(symstr, n), v, sz, info & 15, info >> 4, shn))
        self.relocs = {}     # site address -> (type, target, symbol name, addend); site = r_offset
        for s in sh:
            if s[1] != 4: continue
            for o in range(s[4], s[4] + s[5], 12):
                off, info, add = struct.unpack_from('>IIi', d, o)
                sy = self.syms[info >> 8]
                self.relocs[off] = (info & 255, (sy[1] + add) & 0xffffffff, sy[0], add)
        self.alloc = [s for s in self.sections if s[6] & 2 and s[4]]

    def sym(self, name):
        for s in self.syms:
            if s[0] == name: return s[1]
        raise KeyError(name)

    def section_of(self, a):
        for s in self.alloc:
            if s[3] <= a < s[3] + s[4]: return s[1]
        return None

    def word(self, a):
        for s in self.alloc:
            if s[2] == 1 and s[3] <= a <= s[3] + s[4] - 4:
                return struct.unpack_from('>I', self.d, s[5] + a - s[3])[0]
        return None


class Objects:
    """dtk's objects (sized symbols) by address: containing(a) -> (start, end, name) or None"""

    def __init__(self, elf):
        best = {}
        for n, v, sz, t, b, shn in elf.syms:
            if t not in (1, 2) or not v or not elf.section_of(v): continue   # OBJECT / FUNC
            if v not in best or sz > best[v][1]: best[v] = (n, sz)
        self.starts = sorted(best)
        self.info = best
        self.ends = []
        for i, v in enumerate(self.starts):
            nxt = self.starts[i + 1] if i + 1 < len(self.starts) else 0xffffffff
            sz = best[v][1]
            self.ends.append(min(v + sz, nxt) if sz else nxt)

    def containing(self, a):
        i = bisect.bisect_right(self.starts, a) - 1
        if i < 0 or a >= self.ends[i]: return None
        return self.starts[i], self.ends[i], self.info[self.starts[i]][0]

    def is_start(self, a): return a in self.info


def _s16(x): return x - 0x10000 if x & 0x8000 else x


VARY = da.VARY


class Code:
    """constant propagation with provenance over every function; collects the events described in the docstring"""

    def __init__(self, elf, objs, lo, hi):
        self.elf, self.objs, self.lo, self.hi = elf, objs, lo, hi
        self.sda = {2: elf.sym('_SDA2_BASE_'), 13: elf.sym('_SDA_BASE_')}
        self.events = []    # (kind, insn address, value, detail dict)
        rel = elf.relocs
        # jump-table / data-referenced code addresses: blocks reached by bctr start from the entry state
        self.code_refs = {t for a, (ty, t, n, ad) in rel.items() if ty == R_ADDR32}
        self.funcs = sorted({(v, sz, n) for n, v, sz, t, b, shn in elf.syms
                             if t == 2 and sz and elf.section_of(v) in CODE})

    def inm(self, v): return self.lo <= v < self.hi

    def run(self):
        for s, sz, name in self.funcs:
            self._func(s, s + sz, name)
        return self.events

    def _entry(self):
        return {r: (v, 'sda', None) for r, v in self.sda.items()}

    def _func(self, s, e, name):
        elf = self.elf
        words = {a: elf.word(a) for a in range(s, e, 4)}
        leaders = {s}
        for a, w in words.items():
            b = da._branch(w, a)
            if b:
                tg, ft, call = b
                for t in tg:
                    if s <= t < e: leaders.add(t)
                if a + 4 < e: leaders.add(a + 4)
        for t in self.code_refs:
            if s <= t < e: leaders.add(t)
        order = sorted(leaders)
        blocks = {}
        for i, b in enumerate(order):
            blocks[b] = (b, order[i + 1] if i + 1 < len(order) else e)
        ins = {b: None for b in order}
        ins[s] = self._entry()
        # blocks no path reaches (jump-table cases, dead code) start from the entry state, except in hand-written
        # asm that reuses r2 / r13 (MetroTRK, __init_registers): there they start knowing nothing
        writes_sda = any(((w >> 26) in (14, 15, 32, 33, 34, 35, 40, 41, 42, 43, 46) and (w >> 21) & 31 in (2, 13))
                         or ((w >> 26) == 24 and (w >> 16) & 31 in (2, 13))
                         or ((w >> 26) == 31 and (w >> 21) & 31 in (2, 13) and ((w >> 1) & 0x3ff) in (339, 83, 371))
                         for w in words.values())
        seed = {} if writes_sda else self._entry()
        work = [s]
        done = set()
        pending = [b for b in order if b != s]
        while True:
            while work:
                b = work.pop()
                done.add(b)
                st = dict(ins[b])
                for succ in self._block(blocks[b], words, st, None):
                    if succ not in ins: continue
                    m = st if ins[succ] is None else _merge(ins[succ], st)
                    if ins[succ] is None or m != ins[succ] or succ not in done:
                        ins[succ] = m
                        work.append(succ)
            rest = [b for b in pending if b not in done]
            if not rest: break
            b = rest[0]
            ins[b] = dict(seed) if ins[b] is None else _merge(ins[b], seed)
            work.append(b)
        for b in order:   # final pass: collect events with the converged states
            self._block(blocks[b], words, dict(ins[b] if ins[b] is not None else seed), name)

    def _block(self, blk, words, st, fname):
        """run one block on st (in place); returns successors; records events when fname is given"""
        b, end = blk
        self.block = b
        rel = self.elf.relocs
        for a in range(b, end, 4):
            w = words[a]
            br = da._branch(w, a)
            if br:
                tg, ft, call = br
                if call:
                    for r in (0,) + tuple(range(3, 13)): st[r] = VARY
                    if a + 4 < end: continue
                    return [a + 4]
                return list(tg) + ([a + 4] if ft else [])
            self._step(a, w, st, fname, rel)
        return [end]

    def _ev(self, fname, kind, a, v, **kw):
        if fname is not None: self.events.append((kind, a, v, dict(kw, func=fname)))

    def _step(self, a, w, st, fname, rel):
        op, rd, ra, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xffff
        r = rel.get(a + 2) or rel.get(a)
        if op in (14, 15):                                   # addi / addis (li / lis)
            if ra == 0:
                st[rd] = ((imm << 16) if op == 15 else _s16(imm) & 0xffffffff, None, a if op == 15 else None)
                return
            base = st.get(ra, VARY)
            if base is VARY: st[rd] = VARY; return
            v = (base[0] + ((imm << 16) if op == 15 else _s16(imm))) & 0xffffffff
            st[rd] = self._derive(a, v, base, r, fname, 'addis' if op == 15 else 'addi', ra, inter=op == 15)
        elif op == 24:                                       # ori rA, rS, uimm
            base = st.get(rd, VARY)
            if base is VARY: st[ra] = VARY; return
            st[ra] = self._derive(a, base[0] | imm, base, r, fname, 'ori', rd)
        elif op == 31 and ((w >> 1) & 0x3ff) == 444 and rd == (w >> 11) & 31:   # mr
            st[ra] = st.get(rd, VARY)
        else:
            if op == 31 and ((w >> 1) & 0x3ff) in LOGIC31 or op in (26, 27, 28, 29):
                for src in ((rd, (w >> 11) & 31) if op == 31 else (rd,)):
                    v = st.get(src, VARY)
                    if v is not VARY and v[1] not in (None, 'sda'):
                        self._ev(fname, 'LOGIC', a, v[0], anchor=v[1], reg=src)
            if op == 31 and ((w >> 1) & 0x3ff) == 0:   # cmpw: a signed compare of an address is suspicious
                for src in (ra, (w >> 11) & 31):
                    v = st.get(src, VARY)
                    if v is not VARY and v[1] not in (None, 'sda'):
                        self._ev(fname, 'SCMP', a, v[0], anchor=v[1], reg=src)
            if op in DFORM_LOAD or op in (46, 47):
                base = st.get(ra, VARY) if ra else VARY
                if base is not VARY:
                    ea = (base[0] + _s16(imm)) & 0xffffffff
                    if self.inm(ea): self._access(a, ea, base, r, fname, ra)
            plain = {k: (VARY if v is VARY else 0) for k, v in st.items()}
            da._step(w, plain)
            for k, v in plain.items():
                if v is VARY: st[k] = VARY

    def _derive(self, a, v, base, r, fname, how, src, inter=False):
        bv, anchor, hi = base
        if anchor == 'sda':
            if self.inm(v):
                ok = r is not None and r[0] == R_SDA21
                self._ev(fname, 'SDAADDR', a, v, reloc=bool(ok), reg=src, target=r[1] if ok else None)
                if ok: return (v, r[1], None)      # anchored at the small-data object
            return (v, None, None)
        if anchor is None:
            if r is not None and r[0] in (R_LO, R_HI, R_HA, R_SDA21):
                self._ev(fname, 'RELMAT', a, v, hi=hi, target=r[1], split=hi is not None and hi < self.block)
                return (v, r[1], hi)       # relocated materialisation: anchored at the relocation target
            if self.inm(v):
                hr = rel_hi(self.elf.relocs, hi)
                self._ev(fname, 'MAT', a, v, reloc_lo=r is not None, reloc_hi=hr, hi=hi, how=how)
            return (v, None, hi)
        if r is not None and r[0] in (R_LO, R_HI, R_HA):
            return (v, r[1], hi)
        if not inter: self._cross(a, v, anchor, fname)   # addis rD, rA, n: an intermediate (the EA is checked)
        return (v, anchor, hi)

    def _access(self, a, ea, base, r, fname, ra):
        bv, anchor, hi = base
        if anchor == 'sda':
            self._ev(fname, 'EA', a, ea, base='sda', reloc=r is not None and r[0] == R_SDA21, reg=ra,
                     target=r[1] if r else None)
        elif anchor is None:
            hr = rel_hi(self.elf.relocs, hi)
            self._ev(fname, 'EA', a, ea, base='lis', reloc=r is not None, reloc_hi=hr, hi=hi,
                     target=r[1] if r else None, split=hi is not None and hi < self.block)
        elif r is None:
            self._cross(a, ea, anchor, fname)

    def _cross(self, a, v, anchor, fname):
        if not self.inm(v) or not self.inm(anchor): return
        oa, ov = self.objs.containing(anchor), self.objs.containing(v)
        if oa != ov:
            self._ev(fname, 'CROSS', a, v, anchor=anchor, obj_anchor=oa, obj_value=ov)


def rel_hi(relocs, hi):
    if hi is None: return None
    r = relocs.get(hi + 2)
    return r is not None and r[0] in (R_HA, R_HI)


def _merge(a, b):
    return {r: v for r, v in a.items() if v is not VARY and b.get(r, VARY) == v}


def data_scan(elf, objs, lo, hi):
    """RAW (unrelocated in-range words) and the relocated words of every data section"""
    raw, rels = [], []
    for i, n, t, addr, size, off, fl in elf.alloc:
        if n not in DATA or t != 1: continue
        for a in range(addr, addr + size - 3, 4):
            w = struct.unpack_from('>I', elf.d, off + a - addr)[0]
            r = elf.relocs.get(a)
            if r is not None and r[0] == R_ADDR32: rels.append((a, r)); continue
            if lo <= w < hi: raw.append((a, w))
    return raw, rels


def moving_range(elf):
    return min(s[3] for s in elf.alloc), elf.sym('__ArenaLo') + 1


def audit(path):
    elf = Elf(path)
    objs = Objects(elf)
    lo, hi = moving_range(elf)
    code = Code(elf, objs, lo, hi)
    events = code.run()
    raw, rels = data_scan(elf, objs, lo, hi)
    return elf, objs, (lo, hi), events, raw, rels


def is_miss(k, d):
    """a code event whose relocation(s) dtk did not make"""
    if k == 'MAT': return not (d['reloc_lo'] and d['reloc_hi'])
    if k == 'EA': return not (d['reloc'] and (d['base'] == 'sda' or d['reloc_hi']))
    if k == 'SDAADDR': return not d['reloc']
    return False


def bctr_in(elf, s, e):
    return any(elf.word(a) == 0x4E800420 for a in range(s, e, 4))


def classify(elf, objs, events, rels):
    """-> dict: misses (code events without relocation), fp_data (data relocations that are no pointer),
    fp_code (code relocations whose value is no address), jumptables (verified), glue (merged spans), stats"""
    funcs = sorted((v, v + sz, n) for n, v, sz, t, b, shn in elf.syms if t == 2 and sz and elf.section_of(v) in CODE)
    fstarts = [f[0] for f in funcs]
    fset = set(fstarts)

    def func_of(a):
        i = bisect.bisect_right(fstarts, a) - 1
        return funcs[i] if i >= 0 and a < funcs[i][1] else None
    misses = [e for e in events if is_miss(e[0], e[3])]
    # which dtk objects each function references (relocations in it, and arithmetic CROSS values)
    refs = defaultdict(set)
    for a, (ty, t, n, ad) in elf.relocs.items():
        if elf.section_of(a) in CODE and ty != R_REL24 and ty != R_REL14:
            f, o = func_of(a), objs.containing(t)
            if f and o: refs[f[0]].add(o[0])
    for k, a, v, d in events:
        if k == 'CROSS' and d['obj_value']:
            f = func_of(a)
            if f: refs[f[0]].add(d['obj_value'][0])
    # data relocations into code that are not a function start: a jump table of the function holding the targets?
    by_obj = defaultdict(list)
    for a, r in rels:
        if elf.section_of(r[1]) in CODE and r[1] not in fset:
            by_obj[objs.containing(a)].append((a, r))
    jumptables, fp_data = [], []
    for o, lst in sorted(by_obj.items()):
        fs = {func_of(r[1]) for a, r in lst}
        f = fs.pop() if len(fs) == 1 else None
        prev = None
        if o:
            i = objs.starts.index(o[0])
            prev = objs.starts[i - 1] if i else None
        why = None
        if f is None: why = 'targets in more than one function or outside functions'
        elif not bctr_in(elf, f[0], f[1]): why = f'{f[2]} has no bctr (no switch)'
        elif o and o[0] in refs[f[0]]: pass
        elif prev is not None and prev in refs[f[0]] and objs.info[prev][0].startswith('jumptable_'):
            pass   # continues the jump table right before it, which the function uses
        else: why = f'{f[2]} has a bctr but never references this table'
        entry = (o, f, lst, why)
        (fp_data if why else jumptables).append(entry)
    # code relocations whose value is used as a bit mask / signed number: the lis and every value made from it
    flagged = [(a, v, d) for k, a, v, d in events if k in ('LOGIC', 'SCMP')]
    anchors = {d['anchor'] for a, v, d in flagged}
    fp_code = {}   # hi (lis) site -> {'lo': [sites], 'uses': [(kind, site, value)]}
    for k, a, v, d in events:
        if k == 'RELMAT' and d['target'] in anchors and d['hi'] is not None:
            fp_code.setdefault(d['hi'], {'lo': set(), 'uses': [], 'func': d['func']})
    for k, a, v, d in events:
        if k == 'RELMAT' and d['hi'] in fp_code: fp_code[d['hi']]['lo'].add((a, v, d['target']))
    for k, a, v, d in events:
        if k in ('LOGIC', 'SCMP'):
            for h, e in fp_code.items():
                if any(t == d['anchor'] for x, y, t in e['lo']): e['uses'].append((k, a, v))
    # glue: constant-offset arithmetic from one dtk object into another -> merged spans that padding must not split
    spans = sorted((min(d['anchor'], v), max(d['anchor'], v)) for k, a, v, d in events if k == 'CROSS')
    glue = []
    for lo_, hi_ in spans:
        if glue and lo_ <= glue[-1][1]: glue[-1][1] = max(glue[-1][1], hi_)
        else: glue.append([lo_, hi_])
    stats = defaultdict(int)
    for k, a, v, d in events:
        stats[k] += 1
        if d.get('split'): stats[k + ' split across blocks'] += 1
        if k == 'EA': stats['EA ' + d['base']] += 1
    return dict(misses=misses, fp_data=fp_data, fp_code=fp_code, jumptables=jumptables, glue=glue, stats=stats,
                func_of=func_of)


def consistency(elf, events, rels):
    """every relocated value the code makes / every relocated data word equals its relocation target"""
    bad = []
    for k, a, v, d in events:
        if k in ('RELMAT', 'SDAADDR') and d.get('target') is not None and v != d['target']: bad.append((k, a, v, d))
        if k == 'EA' and d.get('reloc') and d.get('target') is not None and v != d['target']: bad.append((k, a, v, d))
    for a, r in rels:
        if elf.word(a) != r[1]: bad.append(('ADDR32', a, elf.word(a), {'target': r[1]}))
    return bad


def main(argv):
    path = argv[1] if len(argv) > 1 else os.path.join(G, 'decomp/build/GXCE01/main.elf')
    elf, objs, (lo, hi), events, raw, rels = audit(path)
    c = classify(elf, objs, events, rels)
    print(f'moving range 0x{lo:08X}-0x{hi:08X}; relocations {len(elf.relocs)}')
    for k in sorted(c['stats']): print(f'  {k}: {c["stats"][k]}')
    print(f'code misses {len(c["misses"])}, data raw words {len(raw)}, data false positives {len(c["fp_data"])}, '
          f'code false positives {len(c["fp_code"])} lis, verified jump tables (unnamed) {len(c["jumptables"])}, '
          f'glue spans {len(c["glue"])}, inconsistent {len(consistency(elf, events, rels))}')


if __name__ == '__main__':
    sys.exit(main(sys.argv))
