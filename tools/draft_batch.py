"""Draft and check the game code: m2c C draft -> compile with the game compiler -> compare with the original.

For every function of the game code (0x800055E0 up to the first library object of decomp/config/GXCE01/splits.txt,
~1,751 functions; dtk's automatic objects there and the game objects linked from C, decomp/src/game):
  1. its asm comes from dtk's split (the asm of every object of decomp/build/GXCE01/config.json in that range, read
     only; dtk writes asm for the objects linked from C too);
  2. m2c (tools/m2c, pinned in tools/pins.txt) writes a C draft, with our names (they are already in the asm) and
     our types as context: the HSD headers + symbols/cr_types.h, preprocessed once with the game compiler's own -E;
  3. the draft is compiled with the game-code compiler and flags (docs/compiler.md; the flags are read from
     decomp/configure.py CR_LIBS["game"] and cflags_cr, so they cannot drift);
  4. the compiled function is compared with the original function in dtk's split object (decomp/build/GXCE01/obj/
     <object>.o, the object that holds it) word by word. Fields a relocation fills in (call targets, @ha / @l halves, small-data
     offsets) are masked on both sides. 100% = the same number of words and every word equal (the only meaning
     tools/promote_drafts.py relies on); below that, match % = aligned equal words (difflib's longest matching blocks,
     so one inserted instruction costs one word, not the rest of the function) / max(words of both), at most 99.
     (The 100% test is the comparison docs/compiler.md "Method" uses; objdiff-cli is not needed.)
The context m2c and the compiler see (ctx.h / ctx_pp.c) also declares every lbl_ data object of
decomp/config/GXCE01/symbols.txt with its real size and section (data_decls): without a size the compiler would take
every unknown global for small data (one sda21 load where the original has lis/addi), and the game's own functions
with the signatures of their own drafts (a first m2c pass over all functions, ctx_protos.h): the callers then pass
the arguments the callee's draft takes. The draft is post-processed before it is compiled (all of it mechanical and
in this file, so a rerun reproduces it; nothing is edited by hand): stack arrays m2c names but does not declare
(from its --stack-structs template), its `x = saved_reg_rN;` reads of never-written registers, and then a repair
loop on the compiler's own messages (a cast where it reports an illegal implicit conversion, `u8 *` where it reports
arithmetic on `void *`), each fix checked by compiling again. m2c runs through m2c_run.py, which also makes it print the
width of a load it cannot place in a field (M2C_WIDTH; the pinned checkout is not changed). A draft that does not
match but scores 40% or more is also compiled with the VARIANTS of the post-processing (rules whose evidence is not in
the draft: register order, parameter copies, the null pointer's spelling) and the best-scoring one is kept.
Everything goes into decomp/drafts/ (gitignored: decompiled output must never be committed):
  c/<name>.c  the drafts      asm/<name>.s  the input   obj/  compiled objects   log/  m2c / compiler messages of failures
  draft_report.txt  one line per function + totals     ctx_pp.c  the m2c context     orig/  copies of the original objects
  ctx_protos.h  the prototypes of the game functions from their own drafts (a full run rewrites it; --only / --sample
                reuse the one of the last full run, so they see the same context)
Also writes decomp/reports/draft_summary.txt (counts only, no code; committed).
Deterministic: functions in address order, m2c --deterministic-vars, no times or paths in the reports.
Reads decomp/build as input only and never runs ninja. Usage:
  python -I tools/draft_batch.py [--sample N] [--only NAME ...] [--jobs J] [--out DIR] [--no-summary]
"""
import argparse, collections, concurrent.futures as cf, difflib, os, re, shutil, struct, subprocess, sys, time

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = os.path.join(G, 'decomp')
GAME_START = 0x800055E0
B = os.path.join(D, 'build/GXCE01')
MWCC = None


def mwcc_cmd(ver):
    """the compiler command prefix: mwcceppc.exe, through the pinned wibo (or wine) off Windows, as configure.py does"""
    exe = os.path.join(D, 'build/compilers', ver, 'mwcceppc.exe')
    if os.name == 'nt':
        return [exe]
    wibo = os.path.join(D, 'build/tools/wibo')
    return [wibo if os.path.exists(wibo) else 'wine', exe]
M2C = os.path.join(G, 'tools/m2c')
# step 64: m2c prints a load or store it could not place in a field as `*p` / `*(p + x)` whatever p's C type points to:
# `*(lbl_801F9E08 + off)` on a u8[] is a byte to the compiler where the original loads a word (lwz), `*lbl_8044F728`
# on a u8 * where it loads a halfword. m2c knows the access width (its type is the load's), so m2c_run.py wraps it in
# m2c's own M2C_FIELD(p, T *, 0) when the pointer's C element size (p's declared type; for `p + x` the pointer
# operand's) is known and is not the access's (an untyped `void *` / integer base is left to the repair rules). The pinned m2c checkout is not changed: the wrapper patches the class at run time.
M2C_WIDTH = r'''import re
import m2c.translate as _t
_fmt = _t.StructAccess.format


def _target_size(e):
    # the element size of the C type of a dereferenced expression; 0 for an integer (the compiler cannot dereference
    # it: the repair rules would guess s32), None when unknown or an address constant (left as m2c prints it)
    e = _t.late_unwrap(e)
    if isinstance(e, _t.Literal) or (isinstance(e, _t.Cast) and isinstance(_t.late_unwrap(e.expr), _t.Literal)):
        return None
    if isinstance(e, _t.Cast) and e.silent:
        return _target_size(e.expr)
    if isinstance(e, _t.BinaryOp) and e.op in ('+', '-'):
        for side in (e.left, e.right):
            if _t.late_unwrap(side).type.is_pointer_or_array():
                return _target_size(side)
        return 0   # no side typed as a pointer by m2c: the C type is the draft's void * / integer
    ty = e.type
    if ty.is_array() or ty.is_pointer():
        tgt = ty.data().ptr_to
        if tgt is None or tgt.is_void():
            return 0   # void *: the repair rules would guess s32 where m2c knows f32 (fn_8008CDE4)
        return tgt.get_size_bytes() if not tgt.is_struct() else None
    return 0 if ty.is_int() else None


def _scalar(ty):
    if ty.is_int() or ty.is_float():
        return True
    tgt = ty.get_pointer_target() if ty.is_pointer() else None
    return tgt is not None and not tgt.is_function()


def _width_format(self, fmt):
    s = _fmt(self, fmt)
    if not fmt.valid_syntax or self.offset != 0:
        return s
    var = _t.late_unwrap(self.struct_var)
    want = self.type.get_size_bytes()
    if not s.startswith('*'):
        # step 69: m2c's own array spelling of `*(a + i)`, `lbl_801F10C0[(x * 4) & 0xFC]` on a u8[] where the code
        # loads a word (fn_8001B3F0): the access's width at that byte offset
        if want in (2, 4) and isinstance(var, _t.BinaryOp) and var.op == '+' and _target_size(var.left) == 1 \
                and _t.late_unwrap(var.right).type.is_int() and s.startswith(var.left.format(fmt) + '[') \
                and not re.fullmatch(r'sp[0-9A-F]+', _t.format_expr(var.right, fmt)):   # a stack slot m2c reuses as a pointer
            t = _t.Type.ptr(self.type).format(fmt) if _scalar(self.type) else ('u32 *' if want == 4 else 'u16 *')
            return 'M2C_FIELD(%s, %s, %s)' % (var.left.format(fmt), t, _t.format_expr(var.right, fmt))
        inner = _t.late_unwrap(var.expr) if isinstance(var, _t.AddressOf) else None
        if want in (2, 4) and isinstance(inner, _t.ArrayAccess) and _target_size(inner.ptr) == 1 \
                and _t.late_unwrap(inner.index).type.is_int() \
                and not re.fullmatch(r'sp[0-9A-F]+', _t.format_expr(inner.index, fmt)):
            t = _t.Type.ptr(self.type).format(fmt) if _scalar(self.type) else ('u32 *' if want == 4 else 'u16 *')
            return 'M2C_FIELD(%s, %s, %s)' % (inner.ptr.format(fmt), t, _t.format_expr(inner.index, fmt))
        return s
    if not want or s not in ('*' + var.format(fmt), '*(' + var.format(fmt) + ')'):
        return s
    if isinstance(var, _t.Cast) and not var.silent and var.type.is_pointer() and want in (1, 2, 4, 8):
        # step 80: `*((cr_Pad *) ((u8 *) p + off)) = 0` (fn_80020204: a byte store at a variable offset into the typed
        # array; m2c casts the address to the element type and the compiler cannot assign an int to a struct): the
        # access with its own width
        tgt = var.type.get_pointer_target()
        if tgt is not None and tgt.is_struct():
            t = _t.Type.ptr(self.type).format(fmt) if _scalar(self.type) else {1: 'u8 *', 2: 'u16 *', 4: 'u32 *', 8: 'u64 *'}[want]
            return 'M2C_FIELD(%s, %s, 0)' % (var.expr.format(fmt), t)
    if not _scalar(self.type):
        # step 69: a word / halfword m2c could not tell from a pointer (its int-or-pointer type) read from a byte array
        # (`(u32) *(lbl_8024E120 + i * 0x33520) != 0`, dtk's untyped data: a lbz where the original has lwz:
        # fn_8000768C, fn_800BC9A4, fn_8010D2D0): the access's own width; step 80: also through a `void *` / integer
        # base (`*(arg0 + i * 4 + 0xAC)` of a `void *` parameter that the repair loop makes `u8 *`: fn_8008688C's
        # lwzx / stwx came out as lbz / stb)
        if want in (2, 4) and _target_size(var) in (0, 1):
            return _split_field(var, 'u32 *' if want == 4 else 'u16 *', fmt)
        return s
    if isinstance(var, _t.BinaryOp) and var.op == '+':
        # step 69: `*(chara->motion_set + i * 2)` on a struct pointer (m2c's offset is in bytes, C would scale it by the
        # struct): the access with its own width at that byte offset (cr_C_ACT_MOT: lhzx, the repair rule read a u32)
        for p, n in ((var.left, var.right), (var.right, var.left)):
            pt = _t.late_unwrap(p).type
            tgt = pt.get_pointer_target() if pt.is_pointer() else None
            if tgt is not None and tgt.is_struct() and _t.late_unwrap(n).type.is_int():
                return 'M2C_FIELD(%s, %s, %s)' % (p.format(fmt), _t.Type.ptr(self.type).format(fmt), _t.format_expr(n, fmt))
    have = _target_size(var)
    if have is None or have == want:
        return s
    return _split_field(var, _t.Type.ptr(self.type).format(fmt), fmt)


def _split_field(var, t, fmt):
    # step 80, cycle 2 tried `*(p + k)` -> M2C_FIELD(p, T *, k) (the offset as the macro's argument: an indexed load with
    # the sum in a register, fn_8008688C's lwzx); it lost 16 matches whose originals fold the constant of k into the
    # displacement (fn_8007A1D0, fn_800AB468 families: 100% -> 82-84%), so the whole sum stays at offset 0
    return 'M2C_FIELD(%s, %s, 0)' % (var.format(fmt), t)


_afmt = _t.ArrayAccess.format


def _array_width_format(self, fmt):
    # `lbl_801F10C0[i]` on a u8[] where the code loads a word: the index is a byte offset there
    want = self.type.get_size_bytes()
    if fmt.valid_syntax and want and want > 1 and _target_size(self.ptr) == 1:
        if _scalar(self.type):
            return 'M2C_FIELD(%s, %s, %s)' % (self.ptr.format(fmt), _t.Type.ptr(self.type).format(fmt), _t.format_expr(self.index, fmt))
        if want in (2, 4):   # step 69: m2c's int-or-pointer word (fn_8001B3F0), as in _width_format
            return 'M2C_FIELD(%s, %s, %s)' % (self.ptr.format(fmt), 'u32 *' if want == 4 else 'u16 *', _t.format_expr(self.index, fmt))
    return _afmt(self, fmt)


_t.StructAccess.format = _width_format
_t.ArrayAccess.format = _array_width_format
'''
MELEE = '../tools/melee'
# the same defines tools/scripts/ParseHeaders.java uses: no __MWERKS__ (inline asm drops out), M2CTX = the plain-C paths
CTX_DEFS = ['-DM2CTX', '-D__attribute__(x)=', '-D__declspec(x)=']
CTX_INC = [MELEE + '/src', MELEE + '/src/sysdolphin/baselib', MELEE + '/src/MSL', MELEE + '/src/Runtime',
           MELEE + '/libs/doldecomp/include', MELEE + '/libs/dolphin/include', MELEE + '/libs/dolphin/include/libc',
           '../tools/m2c']
FN = re.compile(r'^\.fn (\S+), \w+')
ENDFN = re.compile(r'^\.endfn (\S+)')
HDR = re.compile(r'^# \.text:0x[0-9A-Fa-f]+ \| 0x([0-9A-F]{8}) \| size: 0x([0-9A-F]+)')
CROR = re.compile(r'\bcror eq, (lt|gt), eq\b')


def game_end():
    """the end of the game code: the first .text of a library object (not game/) of splits.txt after GAME_START"""
    end, path = None, None
    for l in open(os.path.join(D, 'config/GXCE01/splits.txt'), encoding='utf-8'):
        if l.strip() and not l[0].isspace():
            path = l.split(':')[0].strip()
        m = re.match(r'^\s+\.text\s+start:0x([0-9A-Fa-f]+)', l)
        if m and path and not path.startswith('game/') and not path.startswith('Sections'):
            a = int(m.group(1), 16)
            if a >= GAME_START and (end is None or a < end):
                end = a
    return end


def unit_files():
    """[(asm path, object path)] of every object dtk split (decomp/build/GXCE01/config.json), in its order"""
    import json
    out = []
    for u in json.load(open(os.path.join(B, 'config.json')))['units']:
        o = os.path.join(D, u['object'])
        a = os.path.join(B, 'asm', os.path.relpath(o, os.path.join(B, 'obj')))[:-2] + '.s'
        if os.path.exists(a):
            out.append((a, o))
    return out


def game_functions(with_lib=False):
    """[(name, address, size, asm lines, original object)] of every game function, in address order, and the jump
    tables {name: lines} of the whole split (from dtk's asm of every object); with_lib: also {name: the same tuple}
    of every other function of the split (the libraries), for the signatures of the ones the game code calls"""
    end = game_end()
    funcs, jt, lib = [], {}, {}
    for asm, obj in unit_files():
        cur, buf, addr, size, blk = None, [], 0, 0, None
        nf, nl = len(funcs), len(lib)
        local, daddr = {}, None   # a C object's own jump tables ("@16", step 69): renamed jumptable_<address>
        for l in open(asm, encoding='utf-8', errors='replace').read().split(chr(10)):
            m = HDR.match(l)
            if m:
                addr, size = int(m.group(1), 16), int(m.group(2), 16)
            m = re.match(r'^# \.\w+:0x[0-9A-Fa-f]+ \| 0x([0-9A-F]{8}) \|', l)
            if m:
                daddr = m.group(1)
            m = re.match(r'^\.obj "(@\d+)", local', l)
            if m and not cur and daddr:
                local[m.group(1)] = 'jumptable_%s' % daddr
                blk = [local[m.group(1)], []]
                continue
            if blk and re.match(r'^\s+\.rel \S+, (\.L_[0-9A-F]{8})\s*$', l):
                blk[1].append('\t.4byte ' + re.match(r'^\s+\.rel \S+, (\.L_[0-9A-F]{8})', l).group(1))
                continue
            m = FN.match(l)
            if m:
                cur, buf = m.group(1), [l]
                continue
            if cur:
                buf.append(l)
                if ENDFN.match(l):
                    if GAME_START <= addr < end:
                        funcs.append((cur, addr, size, buf, obj))
                    else:
                        lib[cur] = (cur, addr, size, buf, obj)
                    cur = None
                continue
            m = re.match(r'^\.obj (jumptable_[0-9A-F]{8}),', l)
            if m:
                blk = [m.group(1), []]
            elif blk and l.startswith('.endobj'):
                if blk[0] in local.values() and not all(re.match(r'^\t\.4byte \.L_', x) for x in blk[1] if x.strip()):
                    # a local object that is not a jump table (a constant, a string of a file object's own pool, step 84):
                    # not copied; the functions name it lbl_<address>, as the drafts did before the object was built
                    # from C and tools/gen_c_symbols.py wrote the C object's own "@NN" into symbols.txt
                    local = {k: (v if v != blk[0] else 'lbl_' + v[len('jumptable_'):]) for k, v in local.items()}
                else:
                    jt[blk[0]] = blk[1]
                blk = None
            elif blk:
                blk[1].append(l)
        if local:
            # the functions of this object name their table "@16"; the copied table is jumptable_<address>
            def ren(t):
                t = list(t)
                t[3] = [re.sub(r'"(@\d+)"', lambda m: local.get(m.group(1), m.group(0)), x) for x in t[3]]
                return tuple(t)
            funcs[nf:] = [ren(f) for f in funcs[nf:]]
            for k in list(lib)[nl:]:
                lib[k] = ren(lib[k])
    funcs.sort(key=lambda f: f[1])
    return (funcs, jt, lib) if with_lib else (funcs, jt)


def game_flags():
    """cflags of configure.py: cflags_cr + CR_LIBS['game'] (read from the source so the draft check cannot drift)."""
    src = open(os.path.join(D, 'configure.py'), encoding='utf-8').read()
    m = re.search(r'^cflags_cr = \[(.*?)^\]', src, re.S | re.M)
    base = re.findall(r'^\s+f?(["\'])(.*)\1,', m.group(1), re.M)
    base = [b[1] for b in base]
    g = re.search(r'^\s+"game": \("([^"]+)", \[\*cflags_cr, (.*)\]\),', src, re.M)
    extra = [x[1] for x in re.findall(r'(["\'])(.*?)\1', g.group(2))]
    ver = g.group(1)
    flags = []
    for f in base + extra:
        f = f.replace('{version_num}', '0').replace('{config.version}', 'GXCE01')   # VERSIONS.index('GXCE01') == 0
        if f.startswith('-') and ' ' in f:
            k, v = f.split(' ', 1)
            flags += [k, v.strip('"')]
        else:
            flags.append(f)
    return ver, flags


# ---- ELF32 big-endian (relocatable objects only) ---------------------------------------------------------------------
class Elf:
    def __init__(self, data):
        self.d = data
        shoff, = struct.unpack('>I', data[0x20:0x24])
        shentsize, shnum, shstrndx = struct.unpack('>HHH', data[0x2E:0x34])
        self.sec = []
        for i in range(shnum):
            o = shoff + i * shentsize
            self.sec.append(struct.unpack('>IIIIIIIIII', data[o:o + 40]))   # name type flags addr off size link info align entsize
        stro = self.sec[shstrndx][4]
        self.secname = [self._cstr(stro + s[0]) for s in self.sec]
        self.syms = {}
        self.symlist = []
        for i, s in enumerate(self.sec):
            if s[1] == 2:   # SHT_SYMTAB
                strt = self.sec[s[6]][4]
                for j in range(s[5] // 16):
                    n, v, sz, info, oth, shn = struct.unpack('>IIIBBH', data[s[4] + j * 16:s[4] + j * 16 + 16])
                    name = self._cstr(strt + n)
                    self.symlist.append((name, v, sz, info, shn))
                    if (info & 0xF) == 2 and name not in self.syms:
                        self.syms[name] = (v, sz, shn)
        self.relocs = collections.defaultdict(dict)   # section index -> {offset: type}
        for i, s in enumerate(self.sec):
            if s[1] in (4, 9):   # RELA / REL
                ent = 12 if s[1] == 4 else 8
                for j in range(s[5] // ent):
                    r = struct.unpack('>III', data[s[4] + j * ent:s[4] + j * ent + 12]) if ent == 12 else struct.unpack('>II', data[s[4] + j * 8:s[4] + j * 8 + 8])
                    self.relocs[s[7]][r[0]] = r[1] & 0xFF

    def _cstr(self, o):
        e = self.d.index(b'\0', o)
        return self.d[o:e].decode('latin-1')

    def func(self, name):
        """(words, {word index: reloc type}) of a function, or None."""
        s = self.syms.get(name)
        if not s:
            return None
        v, sz, shn = s
        off = self.sec[shn][4]
        raw = self.d[off + v:off + v + sz]
        words = struct.unpack('>%dI' % (sz // 4), raw)
        rel = {(o - v) // 4: t for o, t in self.relocs[shn].items() if v <= o < v + sz}
        return words, rel


MASK = {1: 0xFFFFFFFF, 2: 0x03FFFFFC, 4: 0xFFFF, 5: 0xFFFF, 6: 0xFFFF, 7: 0xFFFC, 8: 0xFFFC, 9: 0xFFFC,
        10: 0x03FFFFFC, 11: 0xFFFC, 109: 0x001FFFFF}


def compare(a, b):
    """(percent, equal words): 100 only if both have the same words after masking the relocation fields (position by
    position, the fields of either side masked on both); otherwise the words equal after an alignment (difflib's
    matching blocks over the words with their own relocation fields masked, so an inserted or dropped instruction
    costs one word), as a percent of the longer side, at most 99."""
    wa, ra = a
    wb, rb = b
    n = max(len(wa), len(wb))
    if n == 0:
        return 100, 0
    if len(wa) == len(wb):
        for i in range(n):
            m = MASK.get(ra.get(i), 0xFFFF) if i in ra else 0
            m |= MASK.get(rb.get(i), 0xFFFF) if i in rb else 0
            if (wa[i] & ~m & 0xFFFFFFFF) != (wb[i] & ~m & 0xFFFFFFFF):
                break
        else:
            return 100, n

    def masked(w, r):
        return [(x & ~MASK.get(r[i], 0xFFFF) & 0xFFFFFFFF) if i in r else x for i, x in enumerate(w)]
    sm = difflib.SequenceMatcher(None, masked(wa, ra), masked(wb, rb), autojunk=False)
    eq = sum(bl.size for bl in sm.get_matching_blocks())
    return min(99, eq * 100 // n), eq


DATA_TYPES = {'byte': 'u8', '2byte': 'u16', '4byte': 'u32', 'float': 'f32', 'double': 'f64', 'string': 'char'}
ELEM = {'u8': 1, 'char': 1, 'u16': 2, 's16': 2, 'u32': 4, 'f32': 4, 'f64': 8}
SMALL = ('sdata', 'sbss', 'sdata2', 'sbss2')


DECL_SECTION = {'data': '.data', 'bss': '.data', 'rodata': '.rodata', 'sdata': '.sdata', 'sbss': '.sdata',
                'sdata2': '.sdata2', 'sbss2': '.sdata2'}
SECTION_PRELUDE = ('\n// CR_SECTION(s): the section of a data object, so the compiler picks the original data model whatever its\n'
                   '// size (small data = sda21, large = lis/addi); __declspec only when a draft is compiled (-DCR_DRAFT_CC),\n'
                   '// nothing for m2c\'s context (tools/draft_batch.py data_decls)\n'
                   '#ifdef CR_DRAFT_CC\n#undef __declspec\n#define CR_SECTION(s) __declspec(section s)\n'
                   '#else\n#define CR_SECTION(s)\n#endif\n')


ACCESS_WIDTH = {'lbz': 1, 'stb': 1, 'lha': 2, 'lhz': 2, 'sth': 2, 'lwz': 4, 'stw': 4, 'lfs': 4, 'stfs': 4, 'lfd': 8,
                'stfd': 8}
LOAD_SIGN = {'lha': 's', 'lhau': 's', 'lhax': 's', 'lhz': 'u', 'lhzu': 'u', 'lhzx': 'u',
             'lbz': 'b', 'lbzu': 'b', 'lbzx': 'b'}


def load_signs(funcs):
    """{lbl_ object: set of 's' / 'u'} of the halfword loads the game code makes from it (step 77): directly
    (`lha r0, lbl_X@sda21(r0)`, `lha r0, lbl_X@l(r3)`) or through a register that holds its address (`addi r3, r3,
    lbl_X@l` / `li r3, lbl_X@sda21`, until the register is written again; `lha r0, 0x40(r3)`, `lhax r0, r3, r4`).
    dtk types a 2-byte object `2byte` (u16) whatever the code does with it; an object every load of which is `lha`
    is a signed array (s16): `(s16) lbl_801C6C00[0x20]` read as u16 compiled to lhz (fn_80045B40).
    Also the EXTENT of every object the code addresses through such a register: the end of the furthest load / store
    at a constant offset (`li r3, lbl_8044F87C@sda21; lbz r0, 1(r3)`: 2 bytes, dtk's object is 1), in
    load_signs.extents (data_decls declares an object the code reads beyond its dtk size with that extent)."""
    signs = collections.defaultdict(set)
    ext = load_signs.extents = {}
    ins = re.compile(r'^/\* [0-9A-F]{8} [0-9A-F]{8}  [0-9A-F ]{11} \*/\t(\w+)\.? (.*)$')
    for f in funcs:
        held = {}
        for l in f[3]:
            m = ins.match(l)
            if not m:
                if re.match(r'^\.L_', l):
                    held = {}
                continue
            op, args = m.group(1), [a.strip() for a in m.group(2).split(',')]
            sym = re.search(r'\b(lbl_[0-9A-F]{8})@(?:l|sda21)\b', m.group(2))
            if op in LOAD_SIGN and LOAD_SIGN[op] != 'b':
                if sym:
                    signs[sym.group(1)].add(LOAD_SIGN[op])
                else:
                    bases = re.findall(r'\((r\d+)\)', m.group(2)) if not op.endswith('x') else args[1:3]
                    for b in bases:
                        if b in held:
                            signs[held[b]].add(LOAD_SIGN[op])
            mo = re.match(r'^(-?0x[0-9a-fA-F]+|-?\d+)\((r\d+)\)$', args[1]) if len(args) == 2 else None
            w = ACCESS_WIDTH.get(op)
            if mo and w and mo.group(2) in held and not sym:
                n = held[mo.group(2)]
                ext[n] = max(ext.get(n, 0), int(mo.group(1), 0) + w)
            dest = args[0] if args and re.match(r'^r\d+$', args[0]) and not op.startswith(('st', 'cmp', 'b')) else None
            if dest:
                held.pop(dest, None)
                if sym and (op == 'addi' or op == 'li'):
                    held[dest] = sym.group(1)
    return signs


def data_decls(ptrs={}, signs={}, extents={}):
    """extern declarations of every lbl_ data object of decomp/config/GXCE01/symbols.txt, with the element type dtk
    found (data:byte/2byte/4byte/float/double/string; u8 otherwise), its size, and the right data model: the game is
    compiled with -sdata 8 -sdata2 8, so the compiler takes an extern of at most 8 bytes for small data (sda21) and a
    larger one for large data (lis/addi). An object whose size disagrees with its section (a small-section object of
    more than 8 bytes, a large-section object of at most 8) gets its section named with CR_SECTION (__declspec(section),
    which sets the data model of an extern in GC/2.6; step 62: before, such objects were cut to u8[8] or declared
    u8 name[], which hid their type from m2c: a float in .rodata was read as bytes); a size-0 object of a large section
    stays u8 name[].
    signs (step 77): load_signs of the game code; a `2byte` object every halfword load of which is `lha` is s16.
    extents (step 77, load_signs.extents): an object the code reads or writes beyond its dtk size through its address
    (`li r3, X@sda21; lbz r0, 1(r3)`) is declared with that extent (in bytes, `u8 X[n]`, or in its element type when
    that divides it), if it still ends before the next symbol of its section: dtk sized it by what it saw at its
    start, the rest of the hole is the same object. As a scalar, m2c wrote `M2C_FIELD(&X, u8 *, 1)`, and the address
    of a scalar compiles differently from an array element (lbl_8044F87C: 150 such accesses). Not for the 4-byte
    objects m2c types as pointers.
    ptrs: {name: pointer type} for the 4-byte objects m2c itself typed as pointers in the first pass (`u8 *` when its
    drafts disagree or say `void *`, which the compiler cannot do arithmetic on).
    MIXED (step 64): a 4-byte object the first-pass drafts type as pointers to different things (`lbl_8044F728`: a
    struct pointer read as a byte in one function, a halfword in another) is not declared at all. m2c then declares it
    in each draft itself, with the type that draft needs (`extern u16 *lbl_8044F728;`, as in the first pass), and the
    load keeps its width; before, it was `u8 *` for every draft, and `*p` loaded a byte where the original loads a
    halfword (the fn_800AB468 family). Its section goes into a comment line `// CR_MIXED name prefix`, so a draft's
    own extern gets the same CR_SECTION (mixed_sections)."""
    out = ['\n// every lbl_ data object of decomp/config/GXCE01/symbols.txt with its size, so the compiler picks the\n'
           '// same data model as the original (small data only for the small sections; tools/draft_batch.py data_decls)\n']
    lines = open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8').read().split('\n')
    starts = collections.defaultdict(list)
    for l in lines:
        m = re.match(r'^\S+ = \.(\w+):0x([0-9A-F]+);', l)
        if m:
            starts[m.group(1)].append(int(m.group(2), 16))
    for v in starts.values():
        v.sort()
    for l in lines:
        m = re.match(r'^(lbl_[0-9A-F]{8}|@\d+) = \.(\w+):0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)
        if not m:
            continue
        name, sec, addr, size, rest = pool_name(m.group(1), m.group(3)), m.group(2), int(m.group(3), 16), int(m.group(4), 16), m.group(5)
        d = re.search(r'\bdata:(\w+)', rest)
        t = DATA_TYPES.get(d.group(1) if d else None, 'u8')
        if t == 'u16' and signs.get(name) == {'s'}:
            t = 's16'
        e = extents.get(name, 0)
        if size and e > size and not (size == 4 and name in ptrs):
            nxt = [a for a in starts[sec] if a > addr]
            if not nxt or addr + e <= nxt[0]:
                size = e
                if size % ELEM[t]:
                    t = 'u8'
        pre = ''
        if sec in SMALL:
            if size == 0:
                size = 1
            if size > 8:
                pre = 'CR_SECTION("%s") ' % DECL_SECTION[sec]
        elif size == 0 or sec not in DECL_SECTION:
            if size <= 8:
                out.append('extern u8 %s[];\n' % name)
                continue
        elif size <= 8:
            pre = 'CR_SECTION("%s") ' % DECL_SECTION[sec]
        es = ELEM[t]
        # step 110: read-only data (.sdata2, .rodata) declared const. The compiler lets a load of a const object move
        # ahead of stores through pointers (they cannot change it); declared plainly, every pointer store is a barrier
        # for it (cr_CM_ZOOM: the flag lbl_8045110C read before the camera's byte stores, 82 -> 91 of 91 words)
        cq = 'const ' if sec in ('sdata2', 'rodata') else ''
        if size == 4 and ptrs.get(name) == MIXED:
            out.append('// CR_MIXED %s %s\n' % (name, pre.strip()))
        elif size == 4 and name in ptrs:
            out.append('%sextern %s%s%s;\n' % (pre, ptrs[name], 'const ' if cq else '', name))
        elif size == es:
            out.append('%sextern %s%s %s;\n' % (pre, cq, t, name))
        elif size % es == 0:
            out.append('%sextern %s%s %s[%d];\n' % (pre, cq, t, name, size // es))
        else:
            out.append('%sextern %su8 %s[%d];\n' % (pre, cq, name, size))
    return ''.join(out)


MIXED = 'CR_MIXED'


def mixed_sections():
    """{name: CR_SECTION prefix or ''} of the objects data_decls leaves to each draft (MIXED), read once per process
    from the context (ctx.h)"""
    if 'mixed' not in W:
        try:
            text = open(os.path.join(W['out'], 'ctx.h'), encoding='utf-8').read()
        except OSError:
            text = ''
        W['mixed'] = {m.group(1): m.group(2).strip() for m in re.finditer(r'^// CR_MIXED (\w+) ?(.*)$', text, re.M)}
    return W['mixed']


def mixed_externs(body):
    """a draft's own `extern T *lbl_X;` of a MIXED object (data_decls) gets the object's section (CR_SECTION), like the
    context's declarations; m2c's `void *` (a base pointer only: arithmetic on it is illegal) is `u8 *`, as data_decls
    declares such an object when all drafts say `void *`"""
    mx = mixed_sections()

    def fix(m):
        if m.group(2) not in mx:
            return m.group(0)
        d = m.group(0).replace('extern void *', 'extern u8 *', 1)
        return (mx[m.group(2)] + ' ' + d) if mx[m.group(2)] else d
    return re.sub(r'^extern ([\w ]+ \**)(lbl_[0-9A-F]{8});', fix, body, flags=re.M)


def decl_sections():
    """{name: section} of every data declaration of the context that carries CR_SECTION (promote_drafts.py writes the
    same __declspec into a promoted file)"""
    out = {}
    for l in (data_decls() + globals_decls()).split('\n'):
        m = re.match(r'^CR_SECTION\("([^"]+)"\) extern [^;]*?\b(\w+)(?:\[\d*\])*;$', l)
        if m:
            out[m.group(2)] = m.group(1)
    return out


DEF = re.compile(r'^([A-Za-z_][\w \t\*]*?)\s*\**\b(\w+)\(((?:[^()]|\([^()]*\))*)\) \{$', re.M)


def signature(name, body):
    """the prototype of a draft's own definition line (`void fn_X(void *arg0, s32 arg1) {`), or None.
    m2c's `M2C_UNK arg_spN` parameters are dropped (step 64): m2c reads the stack slots that _savegpr_N / _restgpr_N
    address through r11 as stack arguments (all 17 such prototypes are library functions that save registers that
    way, e.g. fn_8019DC90). In the prototype it is one more parameter, so every caller's draft passed
    `M2C_ERROR(/* Read from unset register */)` for it, and the repair loop then loaded a 0 (`li r3,0`) that the
    original call does not have."""
    for m in DEF.finditer(body):
        if m.group(2) == name:
            line = body[m.start():m.end()]
            line = re.sub(r',\s*M2C_UNK arg_sp[0-9A-F]+\b', '', line)
            line = re.sub(r'\(M2C_UNK arg_sp[0-9A-F]+\)', '(void)', line)
            return line[:-2].rstrip() + ';'
    return None


EXTERN_FN = re.compile(r'^((?:unsigned |signed )?\w+(?: \*+)?) (\w+)\([^;{}]*\);\s*/\* extern \*/', re.M)
NARROW = ('u8', 's8', 'u16', 's16')


# a call whose result m2c could not see (the callee's own draft is void) followed by m2c's read of an unset r3
R3_READ = re.compile(r'^[ \t]*(\w+)\([^\n;]*\);\n[ \t]*[\w.\[\]>-]+ = M2C_ERROR\(/\* Read from unset register \$r3 \*/\);', re.M)


# functions r3_returns leaves void: callers that ignore their value compile to other registers once it has a return
# type (fn_8008B930: fn_8011DBB4, fn_80122044; fn_800972D8: cr_SEQ_TITLE_Enter, fn_80130848; fn_800932D0 itself),
# all 100% while void
R3_KEEP_VOID = ('fn_8008B930', 'fn_800972D8', 'fn_800932D0')
# step 109a: return types the callers' registers show where the callee's own draft cannot. A caller that ignores the
# value of a call to a value-returning function keeps r3 busy after it (its next temporary is r4); one that calls a
# void function takes r3. fn_8006A4E0 ends with `bl fn_8006A7D8` (s32) and leaves its value: fn_800F8D4C's
# `lis r4, 0x1` after the call (64 of 64 with it, the other callers' 100% drafts unchanged). fn_80098484's draft
# returns m2c's leftover r3 temporary (u8) though its only caller, main, ignores it and takes r3 (43 of 43 void).
# fn_80098290 (no call, r3 never written) is drafted `return arg0;`: its callers take r3 after it (fn_80064FE4's
# `lbz r3, 0x5(r31)`; 21 of its 77 callers' drafts change, none of them at 100%, the near ones all up).
R3_SET = {'fn_8006A4E0': 's32', 'fn_80098484': 'void', 'fn_80098290': 'void'}
# step 109a: parameter lists the callers show where the callee never reads the parameter (m2c then says s32):
# cr_SEQ_BATTLE_FrameC passes its u8 index unextended (`mr r3, r31`, an s32 parameter gives `clrlwi r3, r31, 24`);
# fn_8012855C passes fn_800AA17C a 0 or a 1 (`li r3, 0x0; bl fn_800AA17C`), which its 100% draft never reads
PARAM_SET = {'fn_80026F58': 'u8 arg0', 'fn_800AA17C': 's32 arg0'}


def r3_set(sigs):
    """R3_SET's return types and PARAM_SET's parameter lists into the pass-1 prototypes; returns how many"""
    n = 0
    for fn, t in R3_SET.items():
        if fn in sigs:
            i = sigs[fn][1].find(fn + '(')
            if i >= 0:
                sigs[fn] = (sigs[fn][0], t + ' ' + sigs[fn][1][i:])
                n += 1
    for fn, ps in PARAM_SET.items():
        if fn in sigs:
            i = sigs[fn][1].find(fn + '(')
            j = sigs[fn][1].rfind(')')
            if 0 <= i < j:
                sigs[fn] = (sigs[fn][0], sigs[fn][1][:i + len(fn) + 1] + ps + sigs[fn][1][j:])
                n += 1
    return n


def r3_returns(sigs, rets):
    """step 108: a function whose callers read r3 after the call returns a value, though its own draft is void: m2c
    saw no use of r3 at its end (fn_8008EE04 stores through the pointer fn_8008EA64 returned and leaves it in r3;
    fn_8008EDD8 / fn_8008EDA8 end with that call). Its prototype returns the integer or pointer type its callers
    declare for it in pass 1, so the callers' drafts take the value (`lbl_8044F710 = fn_8008EE04(0xE, lbl_802200E4);`,
    fn_800BD5F0 23 -> 26 of 26). Changes sigs in place; returns how many."""
    n = 0
    for fn, ts in rets.items():
        if fn not in sigs or fn in R3_KEEP_VOID:
            continue
        sig = sigs[fn][1]
        if not sig.startswith('void ') or sig.startswith('void *'):
            continue
        # pass 1 drafts the callers without our prototypes: m2c then declares the callee itself with the type its
        # caller's code needs (`s32 *fn_8008EE04(s32, void *); /* extern */`) instead of the unset-r3 error
        own = {x for x in ts if x not in ('CR_R3', 'void', '?', 'M2C_UNK')
               and (x.endswith('*') or x in ('s32', 'u32', 'int', 'unsigned int'))}
        if 'CR_R3' in ts or own:
            ptrs = {x for x in own if x.endswith('*')}
            ret = (next(iter(own)) if len(own) == 1 else 'void *' if ptrs == own or not own else None)
            if ret is None:
                continue
            sigs[fn] = (sigs[fn][0], ret + ('' if ret.endswith('*') else ' ') + sig[len('void '):])
            n += 1
    return n


def narrow_returns(sigs, rets):
    """callee return widths (step 64): a first-pass draft types its callee's return from what its own body does with
    it (`u8 fn_80099538(M2C_UNK); /* extern */`: it tests the low byte, `clrlwi.`), while the callee's own draft can
    only say s32 / u32 (it returns a register, `li r29,1 ... mr r3,r29`). When every caller that declares the callee
    agrees on one narrower integer type, the prototype (ctx_protos.h) takes it: the callers then compile the same
    test (fn_800F60A8, fn_800F67FC: `cmpwi r3,0` before). Changes sigs in place; returns how many."""
    n = 0
    for fn, ts in rets.items():
        if fn not in sigs or len(ts) != 1 or next(iter(ts)) not in NARROW:
            continue
        sig = sigs[fn][1]
        m = re.match(r'^(s32|u32|M2C_UNK) (%s\()' % re.escape(fn), sig)
        if m:
            sigs[fn] = (sigs[fn][0], next(iter(ts)) + ' ' + sig[len(m.group(1)) + 1:])
            n += 1
    return n


EXTERN_PARAMS = re.compile(r'^(?:(?:unsigned |signed )?\w+(?: \*+)?) (\w+)\(([^;{}()]*)\);\s*/\* extern \*/', re.M)
FLIP = {'u8': 's8', 's8': 'u8', 'u16': 's16', 's16': 'u16'}


def narrow_params(sigs, params):
    """callee parameter signedness (step 69): every first-pass caller declares its unknown callee with the argument
    types its own code passes (`M2C_UNK fn_80098104(s32, u8, s32, f32); /* extern */`: it clears the high bits,
    `clrlwi r4,r0,24`), the callee's own draft types its parameter by its own use (`s8 arg1`). When most of the callers
    that type that argument say the byte / halfword type of the other signedness and the others only s32 / u32 (a
    constant), the prototype (ctx_protos.h) takes the callers' type: as `s8` every caller's draft added an `extsb` (fn_8005C4D0's
    `(s8) (u8) x`). The callee's own draft keeps its signature. params: {fn: [tuple of the types, per caller]}.
    Changes sigs in place; returns how many parameters."""
    n = 0
    for fn, decls in params.items():
        if fn not in sigs:
            continue
        sig = sigs[fn][1]
        m = re.match(r'^(.*?\b%s\()(.*)(\);)$' % re.escape(fn), sig)
        if not m or m.group(2).strip() in ('', 'void'):
            continue
        own = [p.strip() for p in m.group(2).split(',')]
        changed = False
        for i, p in enumerate(own):
            pm = re.match(r'^(u8|s8|u16|s16) (\w+)$', p)
            if not pm:
                continue
            # callers that pass a constant say M2C_UNK (ignored) or s32 / u32 (no width of their own)
            got = [d[i] for d in decls if i < len(d) and d[i] != 'M2C_UNK']
            flip = FLIP[pm.group(1)]
            if got.count(flip) * 2 > len(got) and set(got) <= {flip, 's32', 'u32'}:
                own[i] = '%s %s' % (FLIP[pm.group(1)], pm.group(2))
                changed = True
                n += 1
        if changed:
            sigs[fn] = (sigs[fn][0], m.group(1) + ', '.join(own) + m.group(3))
    return n


def protos_text(sigs, ctx_pp):
    """ctx_protos.h: the signatures of the game functions from their own drafts, for the ones the context does not
    declare already (symbols/cr_types.h and the HSD / SDK headers win)"""
    out = ['// prototypes of the game functions from their own m2c drafts (first pass of tools/draft_batch.py);\n'
           '// gitignored like the drafts; a full run rewrites it, --only / --sample reuse it\n']
    for name in sorted(sigs, key=lambda n: sigs[n][0]):
        if not re.search(r'\b%s\s*\(' % re.escape(name), ctx_pp):
            out.append(sigs[name][1] + '\n')
    return ''.join(out)


SDK_INC = 'include/dolsdk2004'   # the decomp's pinned SDK headers (decomp/include; read only here)
# then MSL's math.h (Mario Party 4's MSL_C, decomp/include; read only): atan2 / acos, which pass 1 typed void(void)
MSL_INC = 'include/marioparty4/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common'
# library functions the game calls that no header we have declares (step 63; each with its evidence)
LIB_PROTOS = {
    # 0x8019AD9C, KAR's name (layout proof, symbols.txt): `stw r3, lbl_8044FFE0; blr` keeps its argument, and
    # 0x80197A00 loads that word, tests it for 0 and calls it (mtctr r12; bctrl): a function pointer
    'HSD_JObjSetPtclTargetCallback': ('tools/draft_batch.py LIB_PROTOS', 'void HSD_JObjSetPtclTargetCallback(void (*callback)());'),
}
KEYWORDS = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'signed', 'unsigned', 'const', 'volatile',
            'struct', 'union', 'enum', 'extern', 'register'}


def callees(funcs):
    """every `bl` target of the game functions' asm"""
    out = set()
    for f in funcs:
        for l in f[3]:
            m = re.search(r'\bbl\s+([A-Za-z_]\w*)\s*$', l)
            if m:
                out.add(m.group(1))
    return out


def declared(name, ctx_pp):
    return re.search(r'\b%s\s*\(' % re.escape(name), ctx_pp) is not None


def sdk_protos(names, ctx_pp):
    """{name: (header, prototype)} of the library functions in names that the context does not declare, from the SDK
    headers of decomp/include/dolsdk2004 (dolphin/, libc/): a function the game calls without a prototype is an
    implicit `int f()` to the compiler (float arguments promoted to double, the result an int: PSMTXRotRad, cos, ...).
    Only a one-line prototype whose types the context already knows is taken (the compiler must accept the context),
    or one whose unknown types have a one-line typedef in the same header that the context can take (AIDCallback;
    returned as the second value, emitted first). Then MSL's math.h (atan2, acos) and LIB_PROTOS (functions no header
    we have declares, each with its evidence). -> ({name: (header, prototype)}, [typedef lines])"""
    known = set(re.findall(r'\b[A-Za-z_]\w*\b', ctx_pp))
    found = {}
    typedefs = {}
    files_ = [(d, f) for d, _, fs in sorted(os.walk(os.path.join(D, SDK_INC))) for f in sorted(fs)]
    files_ += [(os.path.join(D, MSL_INC), 'math.h')]

    def typedef_of(ident, text):
        """a one-line typedef of ident in the same header whose own types the context knows (AIDCallback)"""
        for m in re.finditer(r'^[ \t]*typedef\b([^;{}#]*)\b%s\b([^;{}#]*);' % re.escape(ident), text, re.M):
            words = set(re.findall(r'\b[A-Za-z_]\w*\b', m.group(1) + m.group(2)))
            if all(w in known or w in KEYWORDS for w in words):
                return re.sub(r'\s+', ' ', m.group(0).strip())
        return None
    for d, f in files_:
        if not f.endswith('.h') or not os.path.exists(os.path.join(d, f)):
            continue
        text = open(os.path.join(d, f), encoding='utf-8', errors='replace').read()
        text = re.sub(r'//[^\n]*', '', re.sub(r'/\*.*?\*/', ' ', text, flags=re.S))
        for m in re.finditer(r'^[ \t]*((?:[A-Za-z_]\w*[ \t\*]+)+?)\s*\b(\w+)\s*\(([^(){};#]*)\)\s*;', text, re.M):
            ret, name, params = re.sub(r'\s+', ' ', m.group(1)).strip(), m.group(2), re.sub(r'\s+', ' ', m.group(3)).strip()
            if name not in names or name in found or {'static', 'inline', 'typedef', 'return'} & set(ret.split()):
                continue
            idents = set(re.findall(r'\b[A-Za-z_]\w*\b', ret.replace('extern ', '')))
            for p in params.split(','):
                w = re.findall(r'\b[A-Za-z_]\w*\b', p)
                if len(w) >= 2 and w[-1] not in KEYWORDS:
                    w = w[:-1]   # the parameter's name
                idents |= set(w)
            need = [i for i in sorted(idents) if i not in known and i not in KEYWORDS]
            tds = [typedef_of(i, text) for i in need]
            if all(tds):
                for i, td in zip(need, tds):
                    typedefs.setdefault(i, td)
                found[name] = (os.path.relpath(os.path.join(d, f), D).replace('\\', '/'),
                               '%s %s(%s);' % (ret.replace('extern ', ''), name, params))
    for name, v in LIB_PROTOS.items():
        if name in names and name not in found:
            found[name] = v
    return found, [typedefs[i] for i in sorted(typedefs)]


def globals_decls():
    """extern declarations of our typed globals (symbols/cr_globals.txt: cr_g_Name = .sec:0xADDR; // type:object datatype:T*[N]),
    each with its section (CR_SECTION, see data_decls: a 4-byte global of .bss such as cr_g_SeqCurrent is large data,
    lis/addi, in the original; without its section the compiler took it for small data)"""
    out = ['\n// our typed globals (symbols/cr_globals.txt)\n']
    for l in open(os.path.join(G, 'symbols/cr_globals.txt'), encoding='utf-8'):
        m = re.match(r'^(\w+) = \.(\w+):0x[0-9A-Fa-f]+; // type:object datatype:(\w+)(\**)((?:\[\d+\])*)(?=\s|;|/)', l)
        if m:
            pre = 'CR_SECTION("%s") ' % DECL_SECTION[m.group(2)] if m.group(2) in DECL_SECTION else ''
            out.append('%sextern %s%s %s%s;\n' % (pre, m.group(3), m.group(4), m.group(1), m.group(5)))
    return ''.join(out)



# ---- one function --------------------------------------------------------------------------------------------------------
W = {}   # per-process state


def init(out, ver, flags, pass1=False):
    W.update(out=out, ver=ver, flags=flags, orig={}, pass1=pass1)


def orig_elf(path):
    if path not in W['orig']:
        W['orig'][path] = Elf(open(path, 'rb').read())
    return W['orig'][path]


def run(cmd, timeout, cwd=None, env=None):
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout, cwd=cwd, env=env, errors='replace')
        return r.returncode, r.stdout, r.stderr
    except subprocess.TimeoutExpired:
        return -9, '', 'timeout'


def pool_map(ex, fn, items, idle=900):
    """ex.map with a watchdog: results in input order, but if NO job finishes for `idle` seconds while jobs are still
    pending the run is stopped with a message instead of waiting forever (a pool worker that died or a child that never
    returns leaves concurrent.futures waiting with no error: a batch 12 run hung that way for nine hours on Nov 1, 2026;
    every m2c / compiler call has its own 120 s timeout, so a silent 15-minute gap is a hang, not work)."""
    futs = [ex.submit(fn, it) for it in items]
    pending = set(futs)
    while pending:
        done, pending = cf.wait(pending, timeout=idle, return_when=cf.FIRST_COMPLETED)
        if not done:
            print('draft_batch: no job finished in %d s with %d of %d pending: the worker pool is hung, stopping (rerun the batch)'
                  % (idle, len(pending), len(futs)), file=sys.stderr, flush=True)
            for f in pending:
                f.cancel()
            for pr in list(getattr(ex, '_processes', {}).values()):
                try:
                    pr.kill()
                except Exception:
                    pass
            sys.exit(3)
    for f in futs:
        yield f.result()


ERRMSG = re.compile(r'^#\s+Error:\s*\^?\s*$')


def compile_error(text):
    """first compiler message, with identifiers/numbers normalised (so messages can be counted)."""
    lines = text.splitlines()
    for i, l in enumerate(lines):
        if re.match(r'^#\s+Error:', l):
            msg = l.split('Error:', 1)[1].strip(' ^')
            j = i + 1
            while not msg and j < len(lines):
                msg = lines[j].lstrip('# ').strip()
                j += 1
            msg = re.sub(r"'[^']*'", "'*'", msg)
            msg = re.sub(r'\b\d+\b', 'N', msg)
            return msg[:90]
    return (text.strip().splitlines() or ['no message'])[-1][:90]


# ---- post-processing of a draft (mechanical, before the compiler sees it) ----------------------------------------------
STACK = re.compile(r'^struct _m2c_stack_\w+ \{(.*?)\};[^\n]*\n\n?', re.S | re.M)
LOCAL_SIZE = {'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'f32': 4, 'f64': 8, 'u64': 8, 's64': 8,
              'M2C_UNK': 4, 'M2C_UNK8': 1, 'M2C_UNK16': 2, 'M2C_UNK32': 4, 'M2C_UNK64': 8}


def declare_stack_arrays(body):
    """m2c (--stack-structs) puts a template of the stack before the function; a local array in it (`f32 sp64[4]`, or
    `void *sp14[]` of unknown length: an address taken and indexed) is one m2c uses but does not declare. Declare
    every template field the body uses and does not declare (an unknown length runs to the next field of the
    template), and drop the template."""
    m = STACK.search(body)
    if not m:
        return body
    fields = [(int(o, 16), d) for o, d in re.findall(r'^\s+/\* 0x([0-9A-F]+) \*/ ([^;]*);', m.group(1), re.M)]
    sz = re.search(r'/\* size = 0x([0-9A-F]+) \*/', m.group(0))
    offs = [o for o, _ in fields] + ([int(sz.group(1), 16)] if sz else [])
    body = body[:m.start()] + body[m.end():]
    add = []
    for o, d in fields:
        dm = re.match(r'^(.*?)\s*\b(sp[0-9A-F]+)((?:\[\w*\])*)$', d)
        if not dm:
            continue
        t, nm, dims = dm.group(1).strip(), dm.group(2), dm.group(3)
        if dims == '[]':
            nxt = [x for x in offs if x > o]
            total = (min(nxt) - o) if nxt else 4
            es = 4 if t.endswith('*') else LOCAL_SIZE.get(t)
            if es is None or total % es:
                t, es = 'u8', 1
            dims = '[%d]' % max(1, total // es)
        add.append((nm, '    %s %s%s;' % (t, nm, dims)))
    # a stack name the body uses that the template only has inside a pad (`&spD4[0]`, nothing inferred): bytes up to
    # the next field
    have = {nm for nm, _ in add} | {re.sub(r'\[.*', '', d.split()[-1]) for _, d in fields}
    for nm in sorted(set(re.findall(r'\b(sp[0-9A-F]+)\b', body)) - have):
        o = int(nm[2:], 16)
        nxt = [x for x in offs if x > o]
        if nxt and o not in offs:
            add.append((nm, '    u8 %s[%d];' % (nm, min(nxt) - o)))
    lines = body.split('\n')
    for i, l in enumerate(lines):
        if DEF.match(l):
            j = i + 1
            while j < len(lines) and lines[j].strip():
                j += 1
            decl = ' '.join(lines[i + 1:j])
            rest = '\n'.join(lines[i + 1:])
            lines[i + 1:i + 1] = [d for nm, d in add
                                  if not re.search(r'\b%s\b' % nm, decl) and re.search(r'\b%s\b' % nm, rest)]
            break
    return '\n'.join(lines)


def drop_saved_reg_reads(body):
    """`var_r30_0 = saved_reg_r30;` is m2c's name for a read of a callee-saved register nothing wrote (a variable used
    before any assignment on that path): the original has no such statement, so it goes"""
    return re.sub(r'^[ \t]*\w+ = saved_reg_[rf]\d+;[ \t]*\n', '', body, flags=re.M)


def type_void_stores(body):
    """`*(void *)0xCC008000 = x;` (a store m2c could not type, e.g. to the GX FIFO): the store's type is the value's
    when that is a cast or a declared local; the compiler's message settles the rest (repair)"""
    def fix(m):
        rhs = m.group(3)
        cm = re.match(r'^\((u8|s8|u16|s16|u32|s32|f32|f64)\) ', rhs)
        t = cm.group(1) if cm else None
        if t is None and re.match(r'^\w+$', rhs):
            dm = re.search(r'^\s+(u8|s8|u16|s16|u32|s32|f32|f64) %s;$' % rhs, body, re.M)
            t = dm.group(1) if dm else None
        return m.group(0) if t is None else '%s*(%s *)%s = %s;' % (m.group(1), t, m.group(2), rhs)
    return re.sub(r'^(\s*)\*\(void \*\)(\w+) = (.*);$', fix, body, flags=re.M)


def drop_param_spills(body):
    """`sp8 = arg0;` with sp8 used nowhere else: the compiler's own store of a parameter to its stack home (level 0
    stores every parameter and loads it back; m2c shows the store as a local). The draft must not write it: the
    compiler does, by itself. The declaration goes with it."""
    for m in re.findall(r'^[ \t]*(sp[0-9A-F]+) = f?arg\d+;[ \t]*\n', body, re.M):
        if len(re.findall(r'\b%s\b' % m, body)) == 2:
            body = re.sub(r'^[ \t]*%s = f?arg\d+;[ \t]*\n' % m, '', body, flags=re.M)
            body = re.sub(r'^[ \t]*[\w \*]+\b%s;[ \t]*\n' % m, '', body, flags=re.M)
    return body


def declare_unk_stack(body):
    """`unksp14C` is m2c's name for a stack slot it could not place in its template (used through its address, e.g.
    `(&unksp14C)[i]`): declared as f32 where a use is a float expression, else s32. And a template type m2c makes up
    for an odd width (`s24 sp18`) becomes bytes."""
    body = re.sub(r'^(\s*)[su](24|40|48|56) (\w+);', lambda m: '%su8 %s[%d];' % (m.group(1), m.group(3), int(m.group(2)) // 8), body, flags=re.M)
    lines = body.split('\n')
    names = sorted(set(re.findall(r'\b(unksp[0-9A-F]+)\b', body)))
    add = []
    for nm in names:
        if re.search(r'^\s*[\w \*]+\b%s;' % nm, body, re.M):
            continue
        uses = [l for l in lines if re.search(r'\b%s\b' % nm, l)]
        f = any(re.search(r'\b(temp_f|var_f|farg|f32)\w*|\d\.\d', l) for l in uses)
        add.append('    %s %s;' % ('f32' if f else 's32', nm))
    if add:
        for i, l in enumerate(lines):
            if DEF.match(l):
                lines[i + 1:i + 1] = add
                break
    return '\n'.join(lines)


def close_paren(s, i):
    """index of the parenthesis that closes the one at s[i]"""
    d = 0
    for j in range(i, len(s)):
        if s[j] == '(':
            d += 1
        elif s[j] == ')':
            d -= 1
            if d == 0:
                return j
    return -1


SCALARS = ('u8', 's8', 'u16', 's16', 'u32', 's32', 'f32', 'f64')


def ctx_types():
    """{name: (type, deref)} of the data objects the drafts' context declares with a scalar type (deref: True for a
    `T name[N]` / `T *name`, the type is then the element's), read once per process from the context (ctx.h)"""
    if 'vtypes' not in W:
        vt = {}
        try:
            text = open(os.path.join(W['out'], 'ctx.h'), encoding='utf-8').read()
            text += open(os.path.join(W['out'], 'ctx_protos.h'), encoding='utf-8').read()
        except OSError:
            text = ''
        for m in re.finditer(r'^(?:CR_SECTION\("[^"]+"\) )?extern (%s) (\*?)(\w+)((?:\[\d*\])?);$' % '|'.join(SCALARS), text, re.M):
            vt[m.group(3)] = (m.group(1), bool(m.group(2) or m.group(4)))
        for m in re.finditer(r'^#define CR_PTR_(\w+) (%s) \*$' % '|'.join(SCALARS), text, re.M):
            vt[m.group(1)] = (m.group(2), True)
        W['vtypes'] = vt
    return W['vtypes']


def drop_same_casts(body):
    """`(u8) x` where x is already a u8 (a global of the context, `*p` of a u8 pointer, `M2C_FIELD(.., u8 *, ..)`): m2c
    writes the load's width as a cast, and at level 0 a cast of a u8 is one more `clrlwi.` where the original tests
    the loaded register (`cmplwi r0,0`); a cast to its own type means nothing, so it goes. Only in a comparison (the
    cast operand directly followed by ==, !=, <, >, <=, >=): elsewhere the same cast changes the register allocation of
    level 0 code both ways (cr_C_ACT_TURN_POINT 96% -> 79% without it), so m2c's text stays."""
    vt = ctx_types()
    out = []
    i = 0
    pat = re.compile(r'\((%s)\) (\*?)(M2C_FIELD\(|[A-Za-z_]\w*)' % '|'.join(SCALARS))
    cmp = re.compile(r' (==|!=|<|>|<=|>=) ')
    while True:
        m = pat.search(body, i)
        if not m:
            out.append(body[i:])
            break
        t, star, what = m.group(1), m.group(2), m.group(3)
        keep = True
        if what == 'M2C_FIELD(' and not star:
            e = close_paren(body, m.end() - 1)
            inner = body[m.end():e]
            if e > 0 and re.search(r', %s \*, [^,()]+$' % t, inner) and cmp.match(body, e + 1):
                keep = False
        elif what != 'M2C_FIELD(' and what in vt and cmp.match(body, m.end()):
            vt_t, deref = vt[what]
            if vt_t == t and deref == bool(star) and not (m.start() > 0 and re.match(r'[\w)\]]', body[m.start() - 1])):
                keep = False
        elif what != 'M2C_FIELD(' and what in vt and not star and body[m.end():m.end() + 1] == '[':
            # step 69: `(u8) lbl_8044F885[3] == 0` on a u8 array: the element's own type (cr_SEQ_MODE_SELECTOR_Leave)
            e = body.find(']', m.end())
            vt_t, deref = vt[what]
            if e > 0 and '[' not in body[m.end() + 1:e] and deref and vt_t == t and cmp.match(body, e + 1) \
                    and not (m.start() > 0 and re.match(r'[\w)\]]', body[m.start() - 1])):
                keep = False
        if keep:
            out.append(body[i:m.end()])
        else:
            out.append(body[i:m.start()] + star + what)
        i = m.end()
    return ''.join(out)


def drop_same_assign_casts(body):
    """a variant (step 77): `= (s16) lbl_801C6C00[0x20];` where the global (or its element) already has the cast's
    type: the value is assigned as it is loaded (fn_80045B40: `lha r0; sth r0`; with the cast the compiler took r4).
    Whole right-hand sides only (`= (T) X;`, `= (T) X[i];`, `= (T) *X;`)."""
    vt = ctx_types()

    def fix(m):
        t, star, x, idx = m.group(2), m.group(3), m.group(4), m.group(5)
        if x not in vt:
            return m.group(0)
        vt_t, deref = vt[x]
        if vt_t != t or deref != bool(star or idx) or (star and idx):
            return m.group(0)
        return '%s%s%s%s;' % (m.group(1), star, x, idx)
    return re.sub(r'(= )\((%s)\) (\*?)([A-Za-z_]\w*)((?:\[[^\[\]=;]*\])?);' % '|'.join(SCALARS), fix, body)


def drop_same_field_casts(body):
    """`(s32) M2C_FIELD(x, s32 *, n)` anywhere (not only in a comparison, drop_same_casts): a variant (step 69), since
    the cast moves registers both ways at level 0 (fn_8001838C's `x.A8 = (s32) x.64; x.C0 = 0;` hands r6 / r7 out in
    another order with it; cr_C_ACT_TURN_POINT needs its `(s16)`)"""
    out, i = [], 0
    for m in re.finditer(r'\((%s)\) M2C_FIELD\(' % '|'.join(SCALARS), body):
        if m.start() < i:
            continue
        e = close_paren(body, m.end() - 1)
        if e < 0 or not re.search(r', %s \*, [^,()]+$' % m.group(1), body[m.end():e]) \
                or (m.start() > 0 and re.match(r'[\w)\]]', body[m.start() - 1])):
            continue
        out.append(body[i:m.start()])
        i = m.start() + len(m.group(1)) + 3
    out.append(body[i:])
    return ''.join(out)


def while_loops(body):
    """m2c's `loop_N: if (c) { ...; goto loop_N; }` (a loop it did not structure) -> `while (c) { ... }` when nothing
    else jumps to loop_N: the compiler emits a while / for loop at level 0 as a branch to the test at the bottom
    (`b test; body; test: cmp; blt body`), the goto form tests at the top and branches back"""
    pat = re.compile(r'^(loop_\d+):\n([ \t]*)if \((.+)\) \{\n((?:\2    .*\n|[ \t]*\n)*?)\2    goto \1;\n\2\}\n', re.M)
    while True:
        for m in pat.finditer(body):
            if len(re.findall(r'\b%s\b' % m.group(1), body)) == 2:
                body = body[:m.start()] + '%swhile (%s) {\n%s%s}\n' % (m.group(2), m.group(3), m.group(4), m.group(2)) + body[m.end():]
                break
        else:
            return body


def table_call_args(body):
    """`(*(M2C_UNK (**)()) (base + i * 4))(a, base)`: m2c passes the jump table's own address as the last argument
    (it is still in r4 when the call happens: `li r4,table; lwzx r12,r4,r0`); the original passes only the others,
    and with the extra argument the compiler loads the address twice"""
    out = []
    i = 0
    for m in re.finditer(r'\(\*\(M2C_UNK \(\*\*\)\(\)\) \((\w+) \+ ', body):
        if m.start() < i:
            continue
        e = close_paren(body, m.start())
        if e < 0 or e + 1 >= len(body) or body[e + 1] != '(':
            continue
        ce = close_paren(body, e + 1)
        args = body[e + 2:ce]
        tail = ', ' + m.group(1)
        if args.endswith(tail):
            out.append(body[i:e + 2] + args[:-len(tail)])
            i = ce
    out.append(body[i:])
    return ''.join(out)


def table_call_self(body):
    """`temp_r3 = table + i * 4; *temp_r3(temp_r3, x);`: a call through a table slot whose own address m2c also passes
    as the first argument, because the original computed the slot's address in r3 (`add r3,r0,r3; lwz r12,0(r3);
    mtctr r12; bctrl`, fn_8005C4D0; step 69): the slot's address is not an argument of the source's call. The other
    registers m2c passes are kept (table_call_noargs, a variant, drops them as well)."""
    return re.sub(r'(?<![\w)\]])(\*(\w+)\()\2(?:, |(?=\)))', r'\1', body)


def global_call_self(body):
    """`((M2C_UNK (*)(u32 *)) *lbl_X)(lbl_X);`: a call through a function pointer at the start of a global, which m2c
    also passes the global's address: the original loads the pointer with `lis r3,X@ha; lwz r12,X@l(r3)`, so r3 holds
    only the high half of the address, not an argument (fn_8006FB9C, step 77). The argument goes, and the cast's
    parameter list with it. A test of that word right before the call (`if (*lbl_X != 0) {`) is a pointer test,
    `if ((void *) *lbl_X != NULL) {` (with void_null: `addi; li r0,0; lwz; cmplw` as the original)."""
    body = re.sub(r'\(\(M2C_UNK \(\*\)\([^()]*\)\) \*(lbl_[0-9A-F]{8})\)\(\1\)', r'((M2C_UNK (*)()) *\1)()', body)
    return re.sub(r'^([ \t]*)if \(\*(lbl_[0-9A-F]{8}) != 0\) \{\n(\1    \(\(M2C_UNK \(\*\)\([^()]*\)\) \*\2\)\()',
                  r'\1if ((void *) *\2 != NULL) {\n\3', body, flags=re.M)


def table_call_noargs(body):
    """a variant (step 69): `*temp(a, b);` -> `(*(M2C_UNK (**)()) temp)();` for a call through a pointer variable (the
    repair rule's spelling of such a call): m2c passes whatever the argument registers held at the call
    (fn_8005C4D0's r4 is still the script pointer it loaded the index through)"""
    return re.sub(r'^([ \t]+)\*((?!M2C_)\w+)\([^()]*\);$', r'\1(*(M2C_UNK (**)()) \2)();', body, flags=re.M)


def chain_assign(body):
    """a variant (step 69): `A = v; B = v;` (v a name or a constant, A and B of the same type: two fields of the same
    `T *` or two names) -> `B = A = v;`, the source's chained assignment: the compiler hands the second address another
    register than for two statements (fn_80114214: `arg0->x6 = lbl->x6 = arg1` style, the base in r4)"""
    pat = re.compile(r'^([ \t]+)(M2C_FIELD\([^;\n]*?, ([\w ]+\*), [^,()\n]+\)|\w+) = (\w+);\n\1(M2C_FIELD\([^;\n]*?, ([\w ]+\*), [^,()\n]+\)|\w+) = \4;$', re.M)

    def fix(m):
        if (m.group(3) or '') != (m.group(6) or '') or m.group(2) == m.group(5):
            return m.group(0)
        return '%s%s = %s = %s;' % (m.group(1), m.group(5), m.group(2), m.group(4))
    return pat.sub(fix, body)


def negate(cond):
    """the negation of a condition, `x == 0` -> `x != 0` for one comparison, else `!(cond)`"""
    m = re.fullmatch(r'(\w+) (==|!=) (\w+)', cond)
    if m:
        return '%s %s %s' % (m.group(1), '!=' if m.group(2) == '==' else '==', m.group(3))
    return '!(%s)' % cond


def invert_early_returns(body):
    """a variant (step 69): `if (c) { return; } rest` at the end of the function (only closing braces after rest)
    -> `if (!c) { rest }`: the same control flow, which the compiler lays out as `bne rest; b end` where m2c's
    early return is one `beq end` (fn_80095D7C, Melee's HSD_JObjSetupMatrix inline: `if (dirty) Sub(jobj);`)"""
    lines = body.split('\n')
    for i in range(len(lines) - 2):
        m = re.match(r'^(\s+)if \((.+)\) \{$', lines[i])
        if not m or lines[i + 1] != m.group(1) + '    return;' or lines[i + 2] != m.group(1) + '}':
            continue
        ind = m.group(1)
        j = i + 3
        while j < len(lines) and (lines[j].startswith(ind) or not lines[j].strip()):
            j += 1
        rest = lines[i + 3:j]
        if not any(x.strip() for x in rest) or any(re.search(r'\breturn\b|\bgoto\b|^\w+:', x.strip()) for x in rest):
            continue
        tail = lines[j:]
        if not all(re.fullmatch(r'\s*\}?\s*', x) for x in tail):
            continue
        lines[i:j] = ['%sif (%s) {' % (ind, negate(m.group(2)))] + [('    ' + x) if x.strip() else x for x in rest] + [ind + '}']
        return '\n'.join(lines)
    return body


def widen_masked_bytes(body):
    """a variant (step 69): a local `u8 v;` (u16 alike) whose every assignment is `v = (u8) (E);` (or a load of a u8 field) becomes a u32 that
    takes `(E) & 0xFF`: m2c types a register by the `rlwinm` that extracted its byte, but as a byte the compiler masks
    it again where it is used (`rlwinm r0,r30,2,22,29` for `v * 4` where the original has `slwi r0,r30,2`;
    fn_8005C628). The same values: v only ever holds the masked byte."""
    for m in list(re.finditer(r'^    (u8|u16) (\w+);$', body, re.M)):
        t, v = m.group(1), m.group(2)
        n = re.escape(v)
        asg = re.findall(r'^\s+%s = \(%s\) (.+);$' % (n, t), body, re.M)
        # (also a byte / halfword loaded from a field of its own width, `v = M2C_FIELD(p, u8 *, 0x8C);`: the load
        # zero-extends, so as a u32 it needs no mask either; fn_80077648 & co.'s `slwi` index)
        loads = re.findall(r'^\s+%s = M2C_FIELD\([^;]*, %s \*, [^,()]+\);$' % (n, t), body, re.M)
        writes = len(re.findall(r'\b%s\s*(?:[-+*/%%&|^]|<<|>>)?=(?!=)|\b%s\s*(?:\+\+|--)|(?:\+\+|--)\s*%s\b' % (n, n, n), body))
        if not (asg or loads) or writes != len(asg) + len(loads):
            continue
        mask = '0xFF' if t == 'u8' else '0xFFFF'
        body = body.replace(m.group(0), '    u32 %s;' % v, 1)
        body = re.sub(r'^(\s+%s = )\(%s\) (.+);$' % (n, t),
                      lambda a: '%s%s & %s;' % (a.group(1), a.group(2) if a.group(2).startswith('(')
                                                and close_paren(a.group(2), 0) == len(a.group(2)) - 1 else '(' + a.group(2) + ')', mask),
                      body, flags=re.M)
    return body


def redundant_returns(body):
    """a variant (step 69): `if (c) { ...; return; }` followed by a `return;` of the same block (or by the end of the
    function): the inner return is dropped. m2c writes a return wherever the code reaches the epilogue, the compiler
    emits each one as its own `blr` in a leaf function; the original has one (fn_8008BA44's switch case: one blr after
    the if where the draft had two). The same meaning."""
    pat = re.compile(r'^([ \t]+)if \(([^\n]*)\) \{\n((?:\1    [^\n]*\n|[ \t]*\n)*?)\1    return;\n\1\}\n(?=\1return;\n|\}\n)', re.M)
    return pat.sub(lambda m: '%sif (%s) {\n%s%s}\n' % (m.group(1), m.group(2), m.group(3), m.group(1)), body)


def double_negation(body):
    """m2c prints a negation of a negation as `--(x)` (fnmsub forms), which C reads as a decrement (`not an lvalue`)"""
    return re.sub(r'(?<![\w)\]+-])--\(', '- -(', body)


def narrow_int_casts(body):
    """`(u8) (s32) (f * x)`: m2c's two casts of a float converted to a byte / halfword (its (s32) is the fctiwz); the
    compiler converts `(u8) (s32) f` with an extra `clrlwi` after the fctiwz, `(u8) f` takes the converted word as it is,
    as the original does (fn_8005C4D0's argument; step 69). For an integer operand both spellings are the same code."""
    return re.sub(r'\((u8|s8|u16|s16)\) \(s32\) \(', r'(\1) (', body)


def minus_one_stores(body):
    """`lbl_8041A244[0x350] = -1U;` (m2c's -1 of an unsigned store, from `li r0,-1`): stored through a byte / halfword,
    -1U is 0xFF / 0xFFFF to the compiler (`li r0,255`), the original stored the int -1 (`li r0,-1`; fn_80111E54; step
    69). `= -1` gives the same bits for a word and the original's li for a byte / halfword."""
    return re.sub(r'(?<![=!<>]) = -1U;', ' = -1;', body)


def unsigned_const_stores(body):
    """`M2C_FIELD(x, s8 *, N) = 0xFF;`: a byte field stored with a constant of 0x80-0xFF is unsigned (m2c picks s8 for
    a `stb` it has no other use of; as s8 the constant is -1 to the compiler, `li r4,-1` instead of `li r4,255`); the
    same for a halfword and 0x8000-0xFFFF"""
    def fix(m):
        v = int(m.group(4), 0)
        if (m.group(2) == '8' and 0x80 <= v <= 0xFF) or (m.group(2) == '16' and 0x8000 <= v <= 0xFFFF):
            return '%su%s *%s%s' % (m.group(1), m.group(2), m.group(3), m.group(4))
        return m.group(0)
    return re.sub(r'(M2C_FIELD\([^;]*?, )s(8|16) \*(, [^,()]+\) = )(0x[0-9A-Fa-f]+|\d+)(?=;)', fix, body)


def compound_assign(body):
    """`X = X op Y;` -> `X op= Y;` (also `X = (T) (X op Y);` when T is X's own type, M2C_FIELD(.., T *, ..)): m2c writes a
    compound assignment out, and at level 0 the compiler then computes the left side twice (two loads of a global
    pointer, the sum in another register before the store); the game's source used the compound form. Only when X has
    no side effects (no call, ++ / --) and Y is a single operand (no binary operator outside parentheses), so the
    meaning is the same."""
    ops = ('<<', '>>', '+', '-', '*', '/', '%', '&', '|', '^')

    def single(y):
        d = 0
        i = 0
        while i < len(y):
            ch = y[i]
            if ch in '([':
                d += 1
            elif ch in ')]':
                d -= 1
            elif d == 0 and ch == ' ' and i + 1 < len(y) and y[i + 1] in '+-*/%&|^<>=!?:':
                return False
            i += 1
        return d == 0

    def fix(m):
        ind, x, rhs = m.group(1), m.group(2), m.group(3)
        if '++' in x or '--' in x or re.search(r'(?<!\w)(?!M2C_FIELD\W)[A-Za-z_]\w*\s*\(', x) or ' = ' in x:
            return m.group(0)
        cm = re.match(r'^\(([\w ]+?\**)\) \((.*)\)$', rhs)
        if cm:
            fm = re.match(r'^M2C_FIELD\(.*, ([\w ]+?) \*, [^,]+\)$', x)
            if not fm or fm.group(1) != cm.group(1) or not single('(' + cm.group(2) + ')'):
                return m.group(0)
            rhs = cm.group(2)
        for op in ops:
            pre = x + ' ' + op + ' '
            if rhs.startswith(pre) and single(rhs[len(pre):]):
                return '%s%s %s= %s;' % (ind, x, op, rhs[len(pre):])
        return m.group(0)
    return re.sub(r'^([ \t]+)([^\n;]+?) = ([^\n;]+);$', fix, body, flags=re.M)


def assert_ternary(body):
    """m2c's `if (c) { } else { __assert(file, line, text); }` -> `(c) ? (void) 0 : __assert(file, line, text);`
    (step 64): the HSD assert macro (Melee's HSD_ASSERT, `((cond) ? ((void) 0) : __assert(__FILE__, line, #cond))`) is a
    conditional expression, which the compiler emits at level 0 as `beq assert; b end`; the if / else form as one
    `bne end` (fn_80075120, cr_SEQ_TITLE_FrameC)"""
    pat = re.compile(r'^([ \t]*)if \((.+)\) \{\n\n?\1\} else \{\n\1    (__assert\([^;\n]*\);)\n\1\}\n', re.M)
    return pat.sub(lambda m: '%s(%s) ? (void) 0 : %s\n' % (m.group(1), m.group(2), m.group(3)), body)


HSD_INLINE_DIRTY = re.compile(
    r'^([ \t]*)\((\w+) != NULL\) \? \(void\) 0 : __assert\((\w+), 0x25DU, ([^;\n]+)\);\n'
    r'\1(\w+) = 0;\n'
    r'\1if \(!\(\2->flags & 0x800000\) && \(\2->flags & 0x40\)\) \{\n'
    r'\1    \5 = 1;\n'
    r'\1\}\n'
    r'\1if \(\5 ([!=])= 0\) \{', re.M)


def hsd_inlines(body):
    """m2c's expansion of HSD_JObjMtxIsDirty (this HSD revision's jobj.h has its assert at line 605, 0x25D; Melee's at
    564): `(j != NULL) ? (void) 0 : __assert(file, 0x25DU, text); v = 0; if (!(j->flags & 0x800000) && (j->flags &
    0x40)) { v = 1; } if (v != 0) {` -> `if (cr_JObjMtxIsDirty_N(j)) {`, the inline function defined before the
    draft's function (step 77). At level 0 the inlined function's own `result` takes a callee-saved register of its
    own and the test reads the inline's value; m2c's variable of the caller is dropped. The file and text strings are
    the draft's own extern objects (each game file's copy of "jobj.h" / "jobj"), so the inline adds no data. 46 game
    functions call __assert, 92 of the calls are this one."""
    defs = {}
    used = set()

    def fix(m):
        ind, j, f, t, v, op = m.groups()
        key = (f, t.strip())
        if key not in defs:
            defs[key] = 'cr_JObjMtxIsDirty_%d' % len(defs)
        used.add(v)
        return '%sif (%s%s(%s)) {' % (ind, '' if op == '!' else '!', defs[key], j)
    new = HSD_INLINE_DIRTY.sub(fix, body)
    if not defs:
        return body
    for v in used:   # m2c's result variables nothing uses any more: their declarations go too
        if len(re.findall(r'\b%s\b' % re.escape(v), new)) == 1:
            new = re.sub(r'^[ \t]+[\w ]+\b%s;\n' % re.escape(v), '', new, count=1, flags=re.M)
    inl = ''.join('static inline s32 %s(HSD_JObj *jobj) {\n    s32 result;\n\n'
                  '    (jobj != NULL) ? (void) 0 : __assert(%s, 0x25D, %s);\n    result = 0;\n'
                  '    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {\n        result = 1;\n    }\n'
                  '    return result;\n}\n\n' % (n, f, t) for (f, t), n in defs.items())
    m = re.search(r'^\w[^;\n]*\)\s*\{$', new, re.M)
    return new[:m.start()] + inl + new[m.start():] if m else body


# (step 69: also m2c's --reg-vars names, `var_r29` without the `_N` suffix)
REGVAR = re.compile(r'^\s+[^;=()]*?\b(?:var|temp)_([rf])(\d+)(?:_\w*)?(?:\[\w*\])*;\s*$')


def top_level_binop(ex):
    """True when ex has a binary operator outside brackets (it is not one operand)"""
    d = 0
    for i, ch in enumerate(ex):
        if ch in '([':
            d += 1
        elif ch in ')]':
            d -= 1
        elif d == 0 and ch == ' ' and i + 1 < len(ex) and ex[i + 1] in '+-*/%&|^<>=!?:':
            return True
    return False


def param_regs(body):
    """`var_r3_0 = arg0;` as a first statement: a variable m2c names after the register the parameter arrives in (argN
    in r3+N, fargN in f1+N) is the parameter itself: renamed, as param_copies does (fn_8008E598; step 64)"""
    return rename_param_copies(body, lambda v, a: param_home(v, a) and not v.startswith('sp'))


def param_homes(body):
    """`sp8 = arg1;` as a first statement, with sp8 used later (drop_param_spills takes the case where it is not): sp8
    may be the parameter's own stack home (level 0 keeps the parameter there; fn_800683B4) or a real local
    (fn_8005A668), so this is only a variant (step 64)"""
    return rename_param_copies(body, lambda v, a: v.startswith('sp'))


def param_home(v, a):
    """is v (m2c's name) parameter a's own place: its stack home spN, or the register it arrives in"""
    if re.match(r'^sp[0-9A-F]+$', v):
        return True
    m = re.match(r'^(?:var|temp)_([rf])(\d+)_', v)
    fl = a.startswith('farg')
    n = int(a[4:] if fl else a[3:])
    return bool(m) and (m.group(1) == 'f') == fl and int(m.group(2)) == (1 + n if fl else 3 + n)


def param_copies(body):
    """`var_r23_8 = arg0;` at the top level of the body, with arg0 used nowhere else and var_r23_8 declared with arg0's
    own type: the original worked on the parameter itself (kept in a callee-saved register, `mr r23,r3`); m2c's copy
    makes the compiler store the parameter to its stack home and load it back (`stw r3,8(r1); lwz r23,8(r1)`). The
    variable is renamed to the parameter, its declaration and the copy go (fn_80095700; step 64). Not in a leaf
    function (no bl): there a parameter stays in its own register, so a copy in r29-r31 is a real local (fn_80023304);
    the variants also try the draft without it."""
    if W.get('leaf'):
        return body

    def copy(v, a):
        return not param_home(v, a)
    return rename_param_copies(body, copy)


def assigned(name, text):
    '''True when text assigns name (`name = `, `name += `, `name++`, `--name`)'''
    n = re.escape(name)
    return bool(re.search(r'\b%s\s*(?:[-+*/%%&|^]|<<|>>)?=(?!=)|\b%s\s*(?:\+\+|--)|(?:\+\+|--)\s*%s\b' % (n, n, n), text))


def param_aliases(body):
    '''`var_r31 = arg0;` among the first statements, with var_r31 assigned nowhere else and arg0 never assigned: the
    two always hold the same value, so the copy is the parameter itself kept in its callee-saved register (as
    param_copies, which only takes a parameter used once; m2c's --reg-vars drafts name the parameter after the copy as
    well: fn_8001D05C; step 69). Not in a leaf function (param_copies says why); a variant leaves it out.'''
    if W.get('leaf'):
        return body
    return rename_param_copies(body, lambda v, a: not param_home(v, a), alias=True)


def rename_param_copies(body, which, alias=False):
    lines = body.split('\n')
    for i, l in enumerate(lines):
        dm = DEF.match(l)
        if not dm:
            continue
        params = {}
        for p in dm.group(3).split(','):
            pm = re.match(r'^\s*(.*?)\s*\b(f?arg\d+)\s*$', p)
            if pm:
                params[pm.group(2)] = re.sub(r'\s+', '', pm.group(1))
        text = '\n'.join(lines)
        for a, t in params.items():
            if not alias and len(re.findall(r'\b%s\b' % a, text)) != 2:
                continue
            cm = re.search(r'^    (\w+) = %s;\n' % a, text, re.M)
            if not cm:
                continue
            if alias and (assigned(a, text) or assigned(cm.group(1), text.replace(cm.group(0), ''))):
                continue
            # only a copy among the first statements (the prologue's): a later `sp8 = arg1;` is a real local
            # (fn_8005A668)
            dpos = text.find('\n\n', text.find(dm.group(2) + '('))
            if dpos < 0 or not re.fullmatch(r'(    \w+ = f?arg\d+;\n)*', text[dpos + 2:cm.start()]):
                continue
            v = cm.group(1)
            if not which(v, a):
                continue
            vd = re.search(r'^    ([^;=()\n]*?)\s*\b%s;\n' % v, text, re.M)
            if not vd or re.sub(r'\s+', '', vd.group(1)) != t:
                continue
            text = text[:cm.start()] + text[cm.end():]
            vd = re.search(r'^    ([^;=()\n]*?)\s*\b%s;\n' % v, text, re.M)
            text = text[:vd.start()] + text[vd.end():]
            text = re.sub(r'\b%s\b' % v, a, text)
        return text
    return body


VOLATILE = re.compile(r'^(?:temp|var)_(?:r(?:0|[3-9]|1[0-2])|f(?:[0-9]|1[0-3]))_\w+$')
SIMPLE_STMT = re.compile(r'^(\s+)(?!if\b|while\b|for\b|return\b|goto\b|case\b|default\b|switch\b|do\b)[^{}\n]*;$')
LOCAL_DECL = re.compile(r'^    ([A-Za-z_][\w ]*?\**)\s*\b(\w+);$')
FPTR_DECL = re.compile(r'^    ()[^;=\n]*?\(\*+(\w+)\)\([^;\n]*\);$')   # `M2C_UNK (**temp_r3_84)(M2C_UNK, u8 *);`
CASTS = {'M2C_FIELD', 'u8', 's8', 'u16', 's16', 'u32', 's32', 'f32', 'f64', 'void'}


def ctx_arrays():
    """the names the context declares as arrays (`extern u32 lbl_803BE358[289];`), read once per process"""
    if 'arrays' not in W:
        try:
            text = open(os.path.join(W['out'], 'ctx.h'), encoding='utf-8').read()
        except OSError:
            text = ''
        W['arrays'] = set(re.findall(r'^(?:CR_SECTION\("[^"]+"\) )?extern [^;()\n]*?\b(\w+)\[\d*\];$', text, re.M))
    return W['arrays']


def address_only(ex):
    """True when ex reads no memory through a pointer and calls nothing: names, `&name[i]`, `name[i]` of an array,
    arithmetic and casts (a `*` only as a multiplication or in a cast)"""
    if re.search(r'->|M2C_FIELD|\w\s*\(|\.', ex):
        return False
    s = re.sub(r'\((?:const |struct |unsigned |signed )*\w+(?: \*+)?\)', '', ex)
    if re.search(r'(?:^|[^\w)\]\s])\s*\*', s):
        return False
    return all(m.group(1) in ctx_arrays() for m in re.finditer(r'\b(\w+)\s*\[', ex))


def read_names(ex):
    """the names whose value ex reads: not an array whose address only it takes (`&a[i]`, a bare `a`); `a[i]` reads a"""
    arrays = ctx_arrays()
    out = set()
    for m in re.finditer(r'\b[A-Za-z_]\w*\b', ex):
        n = m.group(0)
        if n in CASTS:
            continue
        if n in arrays:
            after = ex[m.end():].lstrip()
            before = ex[:m.start()].rstrip()
            if not after.startswith('[') or before.endswith('&'):
                continue
        out.add(n)
    return out


def written_base(stmt):
    """the named object a simple statement writes (`lbl_803BE358[2] += 1;` -> lbl_803BE358, `x = ...` -> x,
    `M2C_FIELD(&a[i], s32 *, 8) = ...` -> a), when it writes one named object and calls nothing; else None"""
    m = re.match(r'^\s+(.+?)\s*(?:[-+*/%&|^]|<<|>>)?=(?!=)(.*);$', stmt) or re.match(r'^\s+(\w+)()(?:\+\+|--);$', stmt)
    if not m or re.search(r'\b\w+\s*\(', m.group(2).replace('M2C_FIELD(', '(')):
        return None
    lhs = m.group(1)
    for p in (r'^(\w+)$', r'^(\w+)\[(?:[^\[\]=]|\[[^\[\]]*\])*\]$',
              r'^M2C_FIELD\(&(\w+)(?:\[(?:[^\[\]]|\[[^\[\]]*\])*\])?, [\w ]+\*, [^,()]+\)$',
              r'^M2C_FIELD\((\w+), [\w ]+\*, [^,()]+\)$'):
        lm = re.match(p, lhs)
        if lm and (p == r'^(\w+)$' or lm.group(1) in ctx_arrays() or p.startswith(r'^M2C_FIELD\(&')):
            return lm.group(1)
    return None


def inline_volatile_temps(body):
    """a temporary m2c names after a volatile register (`temp_r0_9`, `temp_f1_27`: r0, r3-r12, f0-f13) assigned once
    and used once was an expression of the source, not a variable: at level 0 the compiler gives every C local a
    callee-saved register of its own (r31 / f31 down), so as a variable it took one the original never saves (the
    `stw r30` / `lwz r30` class; fn_8008C3B4). Its expression goes back into its use (step 69):
    - next to each other, or with statements in between that only update locals the expression does not read: in place;
    - when the statements in between only update locals the expression reads (`temp = (i << 5) + 0x1B; i += 1;
      p[temp] = 0xFF`: the scheduler moved the loop's `addi` before the store), the use comes first and those
      statements after it, as the source had them (`p[(i << 5) + 0x1B] = 0xFF; i += 1;`), when the use does not read
      them either.
    Only within one block (same indentation, simple statements in between); a type narrower than 32 bits keeps its
    cast."""
    lines = body.split('\n')
    dm = next((DEF.match(l) for l in lines if DEF.match(l)), None)
    if not dm:
        return body
    locs = set(re.findall(r'^    [A-Za-z_][\w ]*?\**\s*\b(\w+)(?:\[\w*\])*;$', body, re.M))
    locs |= set(re.findall(r'\b(f?arg\d+)\b', dm.group(3)))
    while True:
        for dk, l in enumerate(lines):
            m = LOCAL_DECL.match(l) or FPTR_DECL.match(l)
            if not m or not VOLATILE.match(m.group(2)):
                continue
            v, t = m.group(2), m.group(1).strip()
            pat = re.compile(r'\b%s\b' % re.escape(v))
            occ = [k for k, x in enumerate(lines) if k != dk and pat.search(x)]
            if len(occ) != 2 or sum(len(pat.findall(lines[k])) for k in occ) != 2:
                continue
            i, j = occ
            dmm = re.match(r'^(\s+)%s = (.+);$' % re.escape(v), lines[i])
            if not dmm:
                continue
            ind, ex = dmm.group(1), dmm.group(2)
            if not lines[j].startswith(ind) or lines[j][len(ind)] in ' }':
                continue
            mid = lines[i + 1:j]
            if any(not SIMPLE_STMT.match(x) or not x.startswith(ind) or x[len(ind)] == ' ' for x in mid):
                continue
            ids = set(re.findall(r'\b[A-Za-z_]\w*\b', ex)) - CASTS
            written, ok = set(), True
            # an expression of locals only (no memory, no call): any simple statements may stand in between that do
            # not assign those locals (fn_8001D05C: `temp_f1_27 = var_f31;` returned after three stores)
            pure = ids <= locs and not re.search(r'[*\[]|\w\s*\(', re.sub(r'\((?:%s)\)' % '|'.join(SCALARS), '', ex))
            if pure and not any(assigned(x, '\n'.join(mid)) for x in ids):
                mid = []
            elif mid and address_only(ex):
                # an address of named objects (`&lbl_803BE358[lbl_8044F770]`: no load through a pointer, no call)
                # past statements that only write named objects it does not read (`lbl_803BE358[2] += 1;`): two
                # named objects never overlap, so the address is the same at the use (the script commands'
                # common head, cr_S_BGM_STOP & co.; step 69)
                bases = [written_base(x) for x in mid]
                if all(bases) and not (set(bases) & read_names(ex)):
                    mid = []
            for x in mid:
                lm = re.match(r'^\s+(\w+)\s*(?:[-+*/%&|^]|<<|>>)?=(?!=)(.*);$', x) or re.match(r'^\s+(\w+)(\+\+|--);$', x)
                if not lm or lm.group(1) not in locs or re.search(r'[*\[]|\w\s*\(', lm.group(2)) \
                        or (set(re.findall(r'\b[A-Za-z_]\w*\b', lm.group(2))) - locs - CASTS):
                    ok = False
                    break
                written.add(lm.group(1))
            if not ok:
                continue
            new = ex if re.fullmatch(r'[\w.]+', ex) or (ex.startswith('(') and close_paren(ex, 0) == len(ex) - 1) else '(' + ex + ')'
            if t in ('u8', 's8', 'u16', 's16') or (t.endswith('*') and t not in ('void *', 'M2C_UNK *')):
                # the temporary's own type: a narrower integer is truncated, and a pointer keeps its element type
                # (fn_80005D38: `u16 *temp_r5 = lbl_80386378 + i * 0x34; *temp_r5 |= 2;` on the u8 array is a lhz / sth)
                new = '(%s) %s' % (t, new if new.startswith('(') else '(' + new + ')')
                if t.endswith('*'):
                    new = '(%s)' % new   # `((cr_Chara *) (&a[i]))->flags`: the cast before the -> / [
            use = pat.sub(lambda _: new, lines[j], count=1)
            if written & ids:
                uses = set(re.findall(r'\b[A-Za-z_]\w*\b', lines[j])) - {v}
                if not SIMPLE_STMT.match(lines[j]) or (written & uses):
                    continue
                lines = lines[:i] + [use] + mid + lines[j + 1:]
            else:
                lines = lines[:i] + lines[i + 1:j] + [use] + lines[j + 1:]
            del lines[dk]
            break
        else:
            return '\n'.join(lines)


def signed_fields(body):
    """`(s8) M2C_FIELD(x, u8 *, 9)` -> `M2C_FIELD(x, s8 *, 9)` (and s16 / u16): m2c types the load by its `lbz` and
    writes the sign extension as a cast; the compiler then loads into one register and extends into another
    (`lbz r0 / extsb r4,r0`), where a signed field is loaded and extended in the same one (`lbz r4 / extsb r4,r4`:
    fn_80008E80; step 64)"""
    out, i = [], 0
    for m in re.finditer(r'\((s8|s16)\) M2C_FIELD\(', body):
        if m.start() < i:
            continue
        e = close_paren(body, m.end() - 1)
        if e < 0:
            continue
        u = 'u' + m.group(1)[1:]
        inner = body[m.end():e]
        fm = re.search(r', %s \*, ([^,()]+)$' % u, inner)
        if not fm:
            continue
        out.append(body[i:m.start()] + 'M2C_FIELD(' + inner[:fm.start()] + ', %s *, %s)' % (m.group(1), fm.group(1)))
        i = e + 1
    out.append(body[i:])
    return ''.join(out)


def unsigned_fields(body):
    """`(u16) M2C_FIELD(x, s16 *, n)` -> `M2C_FIELD(x, u16 *, n)` (u8 / s8 alike; step 69): a field m2c typed signed by
    another use, read here as unsigned; as written the compiler loads it signed and clears the sign again
    (`lha r0 / clrlwi r0,r0,16` where the original has one `lhz`: cr_C_ACT_TURN_POINT). The same value. A variant:
    cr_C_ACT_MOVE's `countdown = (u16) M2C_FIELD(p, s16 *, 0xC);` matches only as m2c wrote it."""
    out, i = [], 0
    for m in re.finditer(r'\((u8|u16)\) M2C_FIELD\(', body):
        if m.start() < i:
            continue
        e = close_paren(body, m.end() - 1)
        if e < 0:
            continue
        inner = body[m.end():e]
        fm = re.search(r', s%s \*, ([^,()]+)$' % m.group(1)[1:], inner)
        if not fm:
            continue
        out.append(body[i:m.start()] + 'M2C_FIELD(' + inner[:fm.start()] + ', %s *, %s)' % (m.group(1), fm.group(1)))
        i = e + 1
    out.append(body[i:])
    return ''.join(out)


DOL = os.path.join(D, 'orig/GXCE01/sys/main.dol')


def dol_reader():
    """read(addr, size) of main.dol (decomp/orig, read only), through the DOL header's section table"""
    d = open(DOL, 'rb').read()
    offs = struct.unpack('>18I', d[0:0x48])
    addrs = struct.unpack('>18I', d[0x48:0x90])
    sizes = struct.unpack('>18I', d[0x90:0xD8])

    def read(a, n):
        for o, s, z in zip(offs, addrs, sizes):
            if s <= a and a + n <= s + z:
                return d[o + a - s:o + a - s + n]
        return None
    return read


def float_text(v, double):
    """the shortest C literal that is exactly v (as f32, or as f64), or None (NaN / infinity)"""
    if v != v or v in (float('inf'), float('-inf')):
        return None
    for digits in range(1, 18):
        t = '%.*g' % (digits, v)
        try:
            back = float(t) if double else struct.unpack('>f', struct.pack('>f', float(t)))[0]
        except OverflowError:
            continue
        if back == v:
            break
    if v == int(v) and abs(v) < 1e15:
        t = '%d.0' % v
    elif not re.search(r'[.eE]', t):
        t += '.0'
    elif t.startswith('.') or t.startswith('-.'):
        t = t.replace('.', '0.', 1)
    t += '' if double else 'f'
    return '(%s)' % t if t.startswith('-') else t


def pool_strings():
    """{name: C string literal} of the string objects of .sdata2 (dtk's data:string objects of symbols.txt, the bytes
    from main.dol, every byte as a hex escape; only a string whose NUL lies inside the object), read once per process
    (step 83: the strings of a file's constant pool, see float_literals)"""
    if 'strings' not in W:
        out = {}
        try:
            read = dol_reader()
        except OSError:
            read = None
        for l in open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8') if read else ():
            # dtk types most of them `data:string`; the Shift-JIS ones of fn_8002444C it leaves untyped (`size:0x8`), so an
            # untyped .sdata2 object counts too when its bytes are a non-empty string followed only by zeros
            m = re.match(r'^(lbl_[0-9A-F]{8}|@\d+) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)
            if not m or ('data:' in m.group(4) and 'data:string' not in m.group(4)):
                continue
            b = read(int(m.group(2), 16), int(m.group(3), 16))
            if b is None or 0 not in b or b[0] == 0 or any(b[b.index(0):]) or any(c < 0x20 for c in b[:b.index(0)]):
                continue
            out[pool_name(m.group(1), m.group(2))] = '"' + ''.join('\\x%02x' % c for c in b[:b.index(0)]) + '"'
        W['strings'] = out
    return W['strings']


def pool_floats():
    """{name: C literal} of the float / double constants of .sdata2 (dtk's data:float / data:double objects of
    symbols.txt, their values from main.dol), read once per process"""
    if 'floats' not in W:
        out = {}
        try:
            read = dol_reader()
        except OSError:
            read = None
        for l in open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8') if read else ():
            m = re.match(r'^(lbl_[0-9A-F]{8}|@\d+) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([48])\b.*\bdata:(float|double)\b', l)
            if not m:
                continue
            a, double = int(m.group(2), 16), m.group(4) == 'double'
            wide = False
            if int(m.group(3)) != (8 if double else 4):
                # step 109a: a float object of 8 bytes whose second word is zero is the last float of its file's pool
                # and the pool's padding to 8 (dtk gives the object the padding, m2c declares it `f32 X[2]` and reads
                # `*X`: fn_8005C7F4's 0.0f)
                if double or read(a + 4, 4) != bytes(4):
                    continue
                wide = True
            b = read(a, 8 if double else 4)
            if b is None:
                continue
            t = float_text(struct.unpack('>d' if double else '>f', b)[0], double)
            if t:
                out[pool_name(m.group(1), m.group(2))] = t
                if wide:
                    W.setdefault('floats8', set()).add(pool_name(m.group(1), m.group(2)))
        W['floats'] = out
    return W['floats']


def pool_name(name, addr):
    """the draft name of a data object of symbols.txt: its own lbl_ name, or lbl_<address> for a C file object's own
    pool symbol ("@22": tools/gen_c_symbols.py writes the C object's local symbols into symbols.txt once the object is
    built from C, step 84; a draft cannot declare a name with '@')"""
    return name if not name.startswith('@') else 'lbl_%08X' % int(addr, 16)


def float_literals(body):
    """a float / double constant of .sdata2 read by name (`M2C_FIELD(p, f32 *, 0x28) = lbl_80450CB0;`) written as the
    literal it holds (`0.0f`): the source wrote literals, and the compiler allocates a literal's register differently
    from an extern's (three stores of 0.0f load f0, f1, f0 with the second load moved up; three of an extern float f0,
    f0, f0: fn_80079C70, fn_8001CA84; step 69). The literal is the compiler's own pool constant (the file's .sdata2
    entry at that address: the compare masks the relocation); a variant, because the extern form matches elsewhere.
    Step 83: the same draft also writes the .sdata2 STRING literals it reads by name (`(s32) lbl_804504F8` ->
    `"\x96\xb3\x82\xb5"`, pool_strings): a file's constant pool holds its float literals and its short string
    literals in one sequence (first use order; `-str readonly` puts a literal of 8 bytes or less in .sdata2 as its own
    object: fn_8002444C's pool is 48, 56, 96, 88, "...", 192, "...", 72, "...", 160), so a draft that owns its floats
    must own the strings between them too, or the piece cannot be placed (promote_report.txt: "two piece starts").
    The code is the same either way (`li r3, X@sda21`); only the ownership changes, so this stays inside the variant."""
    fl = pool_floats()
    st = pool_strings()
    if not fl and not st:
        return body
    pat = re.compile(r'(?<![&\w])(lbl_[0-9A-F]{8})\b(?!\s*(?:\[|=[^=]|\+=|-=|\*=|/=))')
    # a string only where the draft takes the object's address as a char pointer or an integer (`(s8 *) lbl_X`,
    # `(s32) lbl_X`): dtk types these objects as plain bytes (`extern u8 lbl_804504F8[8];`), so a load of them stays
    spat = re.compile(r'\((?:s8|u8|char|const char) \*\) (lbl_[0-9A-F]{8})\b|(\((?:s32|u32)\) )(lbl_[0-9A-F]{8})\b')

    def strs(m):
        n = m.group(1) or m.group(3)
        if n not in st:
            return m.group(0)
        return (m.group(2) or '') + st[n]
    wide = W.get('floats8', set())

    def lit(l):
        # step 109a: the padded floats of pool_floats, declared as arrays (`f32 X[2]`), read as `*X` or `X[0]`
        l = re.sub(r'(?<![&\w])(?:\*(lbl_[0-9A-F]{8})\b(?!\s*\[)|(lbl_[0-9A-F]{8})\[0\])(?!\s*(?:=[^=]|\+=|-=|\*=|/=))',
                   lambda m: fl[m.group(1) or m.group(2)] if (m.group(1) or m.group(2)) in wide else m.group(0), l)
        return pat.sub(lambda m: m.group(1) if m.group(1) in wide else fl.get(m.group(1), m.group(1)), spat.sub(strs, l))
    return '\n'.join(l if re.search(r'\bextern\b', l) else lit(l) for l in body.split('\n'))


def void_null(body):
    """`p != NULL` with the null pointer written `((void *) 0)`: the compiler then loads the 0 into a register
    (`li r0,0; cmplw r3,r0`) where `NULL` (MSL's 0L) gives `cmplwi r3,0`; the game uses both (fn_80077290), so this is
    a variant (one(), step 64)"""
    return re.sub(r'([=!]=) NULL\b', r'\1 ((void *) 0)', body)


def pointer_tests(body):
    """a variant (step 77), always with void_null: `(u32) lbl_X != 0U` -> `(void *) lbl_X != ((void *) 0)`. A 4-byte
    global that the code only tests and loads (a callback or object pointer m2c could not type) compared with a null
    pointer: `li r0,0; lwz r4,X; cmplw r4,r0` (fn_800DF2A0), where an integer test gives `cmplwi`."""
    return re.sub(r'\((?:u32|s32)\) (lbl_[0-9A-F]{8}) ([=!]=) 0U?\b', r'(void *) \1 \2 NULL', body)


def narrow_minus_one(body):
    """`(u16) *p != -1U` / `M2C_FIELD(x, u16 *, 4) == -1U`: m2c's -1 of a halfword (byte) compare (`cmplwi r0,0xFFFF`) is
    -1U, which C compares with the promoted 16-bit value as 0xFFFFFFFF (`addis r0,r3,1; cmplwi r0,0xFFFF`, or no test
    at all); the halfword's own -1 is 0xFFFF (0xFF for a byte). fn_80130848, fn_800BAF3C (step 64)"""
    out, i = [], 0
    for m in re.finditer(r' (==|!=) -1U\b', body):
        if m.start() < i:
            continue
        st = expr_start(body, m.start())
        left = body[st:m.start()]
        cm = re.match(r'^\((u8|u16)\) (.*)$', left, re.S)
        t = None
        if cm and not top_level_binop(cm.group(2)):
            t = cm.group(1)
        elif left.startswith('M2C_FIELD(') and close_paren(left, 9) == len(left) - 1:
            fm = re.search(r', (u8|u16) \*, [^,()]+\)$', left)
            t = fm.group(1) if fm else None
        if t:
            out.append(body[i:m.start()] + ' %s %s' % (m.group(1), '0xFFFF' if t == 'u16' else '0xFF'))
            i = m.end()
    out.append(body[i:])
    return ''.join(out)


def register_order(body):
    """the locals m2c names after the original's register (`var_r27_15`, `temp_f31_3`) declared from the highest register
    down (r31, r30, ...; the same for f), in the slots the register-named declarations take (the others stay where m2c
    put them: stack locals keep their layout). The compiler hands the callee-saved registers out in declaration order
    from r31 down, so m2c's order gave fn_800773D0's three pointers r29 / r28 / r27 instead of r27 / r28 / r29 (step 64)"""
    lines = body.split('\n')
    for i, l in enumerate(lines):
        if DEF.match(l):
            j = i + 1
            while j < len(lines) and lines[j].strip():
                j += 1
            slots = [k for k in range(i + 1, j) if REGVAR.match(lines[k])]
            decl = [lines[k] for k in slots]
            decl.sort(key=lambda d: (REGVAR.match(d).group(1), -int(REGVAR.match(d).group(2))))
            for k, d in zip(slots, decl):
                lines[k] = d
            break
    return '\n'.join(lines)


def same_type_copies(body):
    """`M2C_FIELD(a, T *, n) = (T) M2C_FIELD(b, T *, m);`: a field copied to a field of the same type, with m2c's cast to
    that type; at level 0 the cast moves the loaded value to another register (`lhz r4 / sth r4` where the original
    has `lhz r0 / sth r0`: cr_Motion_LoadInfo). The cast means nothing, so it goes (step 64; only this whole-statement
    form and only unsigned types: drop_same_casts says why not in general, and the signed `(s16)` copy of
    cr_C_ACT_TURN_POINT matches only with its cast, 96% -> 79% without)"""
    def fix(m):
        if m.group(2) != m.group(3) or m.group(3) != m.group(5):
            return m.group(0)
        return '%s = %s;' % (m.group(1), m.group(4))
    return re.sub(r'^([ \t]+M2C_FIELD\([^;=]*?, (%s) \*, [^,()]+\)) = \((%s)\) (M2C_FIELD\([^;=]*?, (%s) \*, [^,()]+\));$'
                  % (('u8|u16|u32',) * 3), fix, body, flags=re.M)


BYTE_TYPES = {'u8', 's8', 'char', 'void', 'M2C_UNK8', 'unsigned char', 'signed char', 'const char', 'const u8'}
DECL = re.compile(r'^(?:CR_SECTION\("[^"]+"\) )?(?:extern |static )?((?:const |struct |union |unsigned |signed )*[A-Za-z_]\w*)'
                  r'(?: |(?= *\*)) *(\**) *(\w+)((?:\[\w*\])*);')


def pointee(t, stars, dims):
    """the element type of a declaration `t stars name dims` used as a pointer (`u32 x[5]` -> u32, `u8 **p` -> `u8 *`),
    or None when it is not a pointer / array"""
    if dims:
        return t + ' ' + stars if stars else t
    if stars:
        return (t + ' ' + stars[1:]) if len(stars) > 1 else t
    return None


def ctx_pointees():
    """{name: element type} of every pointer / array the context declares (ctx.h, read once per process)"""
    if 'pointees' not in W:
        out = {}
        try:
            text = open(os.path.join(W['out'], 'ctx.h'), encoding='utf-8').read()
        except OSError:
            text = ''
        for l in text.split('\n'):
            m = DECL.match(l)
            if m and m.group(1).split()[-1] not in ('typedef', 'return'):
                e = pointee(m.group(1), m.group(2), m.group(4))
                if e:
                    out[m.group(3)] = e
        W['pointees'] = out
    return W['pointees']


def elem_size(e):
    """the size of an element type, if it is a scalar or a pointer (None for a struct: unknown here)"""
    e = e.strip()
    if e.endswith('*'):
        return 4
    e = re.sub(r'^const ', '', e)
    return {**LOCAL_SIZE, 's32': 4, 'u32': 4, 'int': 4, 'BOOL': 4, 'f32': 4, 'f64': 8, 'short': 2}.get(e)


def element_terms(rhs, es):
    """the byte offset `(x * 0xC) + 4` as element terms for elements of es bytes (`x * 3`, `1`), when every top-level
    term is a literal or a product with a literal that es divides; else None"""
    ex = unbracket(rhs)
    parts, d, start = [], 0, 0
    for i, ch in enumerate(ex):
        if ch in '([':
            d += 1
        elif ch in ')]':
            d -= 1
        elif d == 0 and ex[i:i + 3] == ' + ':
            parts.append(ex[start:i])
            start = i + 3
        elif d == 0 and ch == ' ' and ex[i + 1:i + 2] in ('-', '<', '>', '&', '|', '^', '=', '!', '?', ':', '/', '%'):
            return None
    parts.append(ex[start:])
    out = []
    for t in parts:
        t = unbracket(t)
        lm = re.match(r'^(0x[0-9A-Fa-f]+|\d+)U?$', t)
        pm = re.match(r'^(.+) \* (0x[0-9A-Fa-f]+|\d+)U?$', t)
        if lm and int(lm.group(1), 0) % es == 0:
            k = int(lm.group(1), 0) // es
            out.append(('0x%X' % k) if k > 9 else str(k))
        elif pm and int(pm.group(2), 0) % es == 0 and not top_level_binop(pm.group(1)):
            k = int(pm.group(2), 0) // es
            out.append(pm.group(1) if k == 1 else '%s * %s' % (pm.group(1), ('0x%X' % k) if k > 9 else k))
        else:
            return None
    return out if len(out) > 1 or ' * ' in out[0] or out[0] != unbracket(rhs) else None


def byte_steps(body):
    """m2c prints `p + n` (an add it could not turn into a field or an element) with n in BYTES whatever p points to;
    C scales it by the element size. With `u32 lbl_8031FED8[5]` (dtk's 4-byte data) `lbl_8031FED8 + (i * 0xA4)` became
    i * 0xA4 * 4 (`slwi` more than the original: fn_8001F654, fn_8006FAC4; cr_Motion_LoadInfo's `rlwinm 4` for `2`).
    (step 64) For every pointer / array whose element is larger than a byte (a global of the context, a parameter or a
    local of the draft; struct pointers too), `p + n` / `p - n` and `p += n` / `p -= n` become: element steps when n is
    a constant multiple of the element size (`p + 8` -> `p + 2` on a u32 *), else the byte step written out with p's own
    type kept, `(T *) ((u8 *) p + n)`. Not when p is dereferenced, indexed, a field, or cast first."""
    names = dict(ctx_pointees())
    for l in body.split('\n'):
        m = DECL.match(l.strip()) if not DEF.match(l) else None
        if m:
            e = pointee(m.group(1), m.group(2), m.group(4))
            if e:
                names[m.group(3)] = e
            elif m.group(3) in names:
                del names[m.group(3)]   # a local scalar shadows a global
        dm = DEF.match(l)
        if dm:
            for p in dm.group(3).split(','):
                pm = re.match(r'^\s*((?:const |struct |unsigned |signed )*[A-Za-z_]\w*) (\**)(\w+)\s*$', p)
                if pm:
                    e = pointee(pm.group(1), pm.group(2), '')
                    if e:
                        names[pm.group(3)] = e
                    elif pm.group(3) in names:
                        del names[pm.group(3)]
    allp = set(names)
    names = {n: e for n, e in names.items() if e.strip() not in BYTE_TYPES and '(' not in e}
    if not names:
        return body
    pat = re.compile(r'(?<![\w.>*&])\b(%s) ([+-])(=?) ' % '|'.join(re.escape(n) for n in sorted(names, key=len, reverse=True)))
    stop = re.compile(r'\s*(?:[)\],;?:]|==|!=|<=|>=|&&|\|\||<<|>>|[<>&|^]|=(?!=))')
    out, i = [], 0
    while True:
        m = pat.search(body, i)
        if not m:
            out.append(body[i:])
            return ''.join(out)
        pre = body[max(0, m.start() - 12):m.start()]
        # skip `(s32) p + n` (an integer) and `x - p` (not the pointer operand)
        if re.search(r'\)\s*$', pre) and re.search(r'\((?:[us](?:8|16|32)|f32|f64|M2C_UNK\w*)\)\s*$', pre) \
                or re.search(r'[\w)\]]\s*[+\-]\s*$', pre):
            out.append(body[i:m.end()])
            i = m.end()
            continue
        # the right operand: up to the end of the additive expression at this bracket level
        j, depth = m.end(), 0
        while j < len(body):
            ch = body[j]
            if ch in '([':
                depth += 1
            elif ch in ')]' and depth:
                depth -= 1
            elif depth == 0 and (ch == '\n' or stop.match(body, j)) and not (ch == '>' and body[j - 1] == '-'):
                break
            j += 1
        rhs = body[m.end():j].rstrip()
        if not rhs:   # step 109b: `sp26C - &x->m` (the operand starts with `&`): left as it is
            out.append(body[i:m.end()])
            i = m.end()
            continue
        j = m.end() + len(rhs)
        name, op, assign = m.group(1), m.group(2), m.group(3)
        e = names[name]
        es = elem_size(e)
        lit = re.match(r'^(0x[0-9A-Fa-f]+|\d+)U?$', rhs)
        terms = element_terms(rhs, es) if es and es > 1 and op == '+' and not name.startswith('lbl_') else None
        if es and lit and int(lit.group(1), 0) % es == 0:
            k = int(lit.group(1), 0) // es
            new = '%s %s%s %s' % (name, op, assign, ('0x%X' % k) if k > 9 else k)
        elif terms:
            # `p + ((x * 0xC) + 4)` on a u32 *: `p + x * 3 + 1` (mulli 3, slwi 2, addi 4 as in fn_800BAF3C)
            new = '%s %s%s %s' % (name, op, assign, ' + '.join(terms))
        elif es == 1 or (op == '-' and unbracket(rhs) in allp):
            new = body[m.start():j]
        elif assign:
            new = '%s = (%s *) ((u8 *) %s %s %s)' % (name, e, name, op, rhs)
        else:
            new = '((%s *) ((u8 *) %s %s %s))' % (e, name, op, rhs)
        out.append(body[i:m.start()] + new)
        i = j


def stack_struct(body):
    """a variant (step 77): m2c names a local `spN` only for a stack slot the original read or wrote in memory, so
    the source kept that value in memory: a member of a local struct or array (fn_8006FBE0: `Vec3 d; d.x = ...;
    d.z = ...;` is sp8 / sp10, the translation copied out of a JObj is sp14..sp1C). Declared as scalars, the compiler
    gives them registers (f30 / f31) and drops the loads and stores. All the draft's spN locals (arrays too) become
    members of one local struct `sp`, at their offsets from the lowest one (padding in between), and every use
    `spN` -> `sp.spN`. Not when a slot overlaps another, a member would be misaligned, or a type is a function
    pointer / pointer to array; not when every scalar spN has its address taken already (those stay in memory)."""
    lines = body.split('\n')
    di = next((i for i, l in enumerate(lines) if DEF.match(l)), None)
    if di is None:
        return body
    j = di + 1
    decls = []
    while j < len(lines) and lines[j].strip():
        m = re.match(r'^\s+([\w ]+?\**)\s*\b(sp[0-9A-F]+)((?:\[\d+\])*);$', lines[j])
        if m:
            decls.append((j, m.group(1).strip(), m.group(2), m.group(3)))
        elif re.search(r'\bsp[0-9A-F]+\b', lines[j]):
            return body
        j += 1
    if not decls:
        return body
    rest = '\n'.join(lines[j:])
    scalars = [nm for _, t, nm, dims in decls if not dims]
    if not any(not re.search(r'&%s\b' % nm, rest) for nm in scalars):
        return body
    items = []
    for _, t, nm, dims in decls:
        es = 4 if t.endswith('*') else LOCAL_SIZE.get(t)
        if es is None:
            return body
        n = 1
        for d in re.findall(r'\[(\d+)\]', dims):
            n *= int(d)
        items.append((int(nm[2:], 16), es, es * n, t, nm, dims))
    items.sort()
    base = items[0][0]
    pos = base
    members = []
    for off, al, size, t, nm, dims in items:
        if off < pos or (off - base) % min(al, 8):
            return body
        if off > pos:
            members.append('        u8 pad_%X[%d];' % (pos, off - pos))
        members.append('        %s %s%s;' % (t, nm, dims))
        pos = off + size
    drop = {i for i, _, _, _ in decls}
    head = [l for i, l in enumerate(lines[di + 1:j], di + 1) if i not in drop]
    names = '|'.join(nm for _, _, nm, _ in decls)
    rest = re.sub(r'(?<![\w.>])(%s)\b' % names, r'sp.\1', rest)
    out = lines[:di + 1] + ['    struct {'] + members + ['    } sp;'] + head + rest.split('\n')
    return '\n'.join(out)


def assign_in_cond(body):
    """`t = X & C; X = t; if ((t != 0) && ...` -> `if (((X &= C) != 0) && ...` (step 77): the original tests the value
    its compound assignment stored (fn_8006FBE0: `rlwinm. r0,r0,0,26,27; stw r0; beq`); as a temporary of its own
    the value took a callee-saved register. Only when t is used nowhere else; X has no call, no ++ / --."""
    pat = re.compile(r'^([ \t]*)(temp_\w+) = ([^;\n]+?) ([&|^+\-]) ([^;\n]+);\n\1\3 = \2;\n\1(if \(|while \()(.*)$', re.M)

    def fix(m):
        ind, t, x, op, y, kw, cond = m.groups()
        if '(' in x and not x.startswith('M2C_FIELD(') or '++' in x or '--' in x or re.search(r'\w\(', x.replace('M2C_FIELD(', '')):
            return m.group(0)
        if len(re.findall(r'\b%s\b' % re.escape(t), cond)) != 1:
            return m.group(0)
        return '%s%s%s' % (ind, kw, re.sub(r'\b%s\b' % re.escape(t), lambda _: '(%s %s= %s)' % (x, op, y), cond, count=1))
    new = pat.sub(fix, body)
    if new == body:
        return body
    for t in set(re.findall(r'\b(temp_\w+)\b', body)):
        if len(re.findall(r'\b%s\b' % re.escape(t), new)) == 1:
            new = re.sub(r'^[ \t]+[\w \*]+\b%s;\n' % re.escape(t), '', new, count=1, flags=re.M)
    return new


def struct_copies(body):
    """three word copies `sp.spA = M2C_FIELD(P, T *, o); ... sp.sp(A+4) = M2C_FIELD(P, U *, o+4); ... sp.sp(A+8) = ...`
    into the local struct of stack_struct, at least one of them an integer word while another is a float: the
    original copied a Vec3 through integer registers (`lwz` / `stw`; a struct assignment, e.g. HSD_JObjGetTranslation's
    `*translate = jobj->translate`, fn_8006FBE0), which m2c types by each word's later use. -> one struct assignment
    `*(Vec3 *) &sp.spA = *(Vec3 *) ((u8 *) P + o);` where the first copy was; the statements between the three may
    only assign other sp members and must not read the later ones (step 77, with stack_struct)."""
    lines = body.split('\n')
    cp = re.compile(r'^([ \t]*)sp\.sp([0-9A-F]+) = M2C_FIELD\(([^;]+), (f32|s32|u32) \*, (0x[0-9A-F]+|\d+)\);$')
    changed = False
    i = 0
    while i < len(lines):
        m = cp.match(lines[i])
        if not m:
            i += 1
            continue
        ind, a0, p, t0, o0 = m.group(1), int(m.group(2), 16), m.group(3), m.group(4), int(m.group(5), 0)
        found = {0: (i, t0)}
        k = i + 1
        ok = True
        while k < len(lines) and k <= i + 6 and len(found) < 3:
            mk = cp.match(lines[k])
            if mk and mk.group(3) == p and mk.group(1) == ind:
                d = int(mk.group(2), 16) - a0
                if d in (4, 8) and int(mk.group(5), 0) - o0 == d and d not in found:
                    found[d] = (k, mk.group(4))
                    k += 1
                    continue
            ma = re.match(r'^[ \t]*sp\.sp[0-9A-F]+ = [^;]*;$', lines[k])
            later = ['sp.sp%X' % (a0 + d) for d in (4, 8) if d not in found]
            if not ma or any(re.search(r'%s\b' % re.escape(x), lines[k]) for x in later):
                ok = False
                break
            k += 1
        types = {t for _, t in found.values()}
        if ok and len(found) == 3 and 'f32' in types and types - {'f32'}:
            for idx in sorted((found[4][0], found[8][0]), reverse=True):
                del lines[idx]
            lines[i] = '%s*(Vec3 *) &sp.sp%X = *(Vec3 *) ((u8 *) %s + 0x%X);' % (ind, a0, p, o0)
            changed = True
        i += 1
    return '\n'.join(lines) if changed else body


def alias_offsets(body):
    """a variant (step 77): `t = b + K;` (both locals assigned once) and later `M2C_FIELD(b, T *, K)`: m2c folds the
    access at offset 0 of the alias back into its base (fn_80029C84: `temp_r29 = cam + 0x28` is a Vec4 pointer and
    `stfs f0,0(r29)` became `M2C_FIELD(cam, f32 *, 0x28)`, stored through r31 in the draft) -> `M2C_FIELD(t, T *, 0)`
    after t's assignment (a later alias of the same address first). The text cannot say which register the original used, so it is a variant."""
    out = body
    for m in reversed(list(re.finditer(r'^[ \t]*((?:temp|var)_\w+) = ((?:temp|var)_\w+|arg\d+) \+ (0x[0-9A-F]+|\d+);$', body, re.M))):
        t, b, k = m.group(1), m.group(2), int(m.group(3), 0)
        if len(re.findall(r'^[ \t]*%s = ' % re.escape(t), body, re.M)) != 1 or \
                len(re.findall(r'^[ \t]*%s (?:[-+*/|&^]?=) ' % re.escape(b), body, re.M)) > (0 if b.startswith('arg') else 1):
            continue
        at = out.find(m.group(0))
        if at < 0:
            continue
        head, tail = out[:at + len(m.group(0))], out[at + len(m.group(0)):]
        tail = re.sub(r'M2C_FIELD\(%s, ([^,()]+(?:\([^()]*\))?[^,()]*), (?:0x%X|%d)\)' % (re.escape(b), k, k),
                      lambda mm: 'M2C_FIELD(%s, %s, 0)' % (t, mm.group(1)), tail)
        out = head + tail
    return out


def member_types():
    """{struct: {member: type}} of symbols/cr_types.h and {global: type} of symbols/cr_globals.txt (step 79: the
    typed globals cr_g_Script / cr_g_Scenario / cr_g_GameMode / cr_g_Pads), read once per process; array and pointer
    marks are kept on the type ("u16[19]", "cr_ScenarioSlot*")"""
    if 'mtypes' not in W:
        structs, cur = {}, None
        for l in open(os.path.join(G, 'symbols/cr_types.h'), encoding='utf-8'):
            m = re.match(r'^typedef struct (\w+) \{', l)
            if m:
                cur = structs.setdefault(m.group(1), {})
                continue
            if l.startswith('}'):
                cur = None
                continue
            m = re.match(r'^\s*/\* \+[0-9A-Fa-f]+ \*/ ([\w ]+?)(\**) (\w+)((?:\[[^\]]+\])*);', l) if cur is not None else None
            if m:
                cur[m.group(3)] = m.group(1).strip() + m.group(2) + m.group(4)
        globs = {}
        for l in open(os.path.join(G, 'symbols/cr_globals.txt'), encoding='utf-8'):
            m = re.match(r'^(\w+) = \.\w+:0x[0-9A-Fa-f]+; // type:object datatype:(\w+\**(?:\[\d+\])*)', l)
            if m:
                globs[m.group(1)] = m.group(2)
        W['mtypes'] = (structs, globs)
    return W['mtypes']


def member_type(expr):
    """the declared type of `cr_g_X.a[i].b` / `cr_g_X->a` / `cr_g_X[i].a` from member_types(), as a scalar name
    ('u8', 's16', ...), or None when a step is unknown or the result is not a scalar"""
    structs, globs = member_types()
    parts = re.findall(r'(->|\.)?(\w+)((?:\[[^\[\]]*\])*)', expr)
    if not parts or parts[0][1] not in globs:
        return None
    t = globs[parts[0][1]]
    for sep, name, idx in parts:
        if sep:
            base = re.sub(r'[\[\]\d*]+$', '', t)
            if base not in structs or name not in structs[base]:
                return None
            t = structs[base][name]
        for _ in re.findall(r'\[', idx):   # one index strips one array / pointer level
            if t.endswith(']'):
                t = re.sub(r'\[[^\]]*\]$', '', t)
            elif t.endswith('*'):
                t = t[:-1]
            else:
                return None
    return t if t in SCALARS else None


MEMBER = r'cr_g_\w+(?:\[[^\[\]=;]*\])*(?:(?:->|\.)\w+(?:\[[^\[\]=;]*\])*)+'


def drop_same_member_casts(body):
    """a variant (step 79): `(u8) cr_g_Scenario.slot_index` where the member already has the cast's type, as
    drop_same_field_casts does for M2C_FIELD: m2c writes the cast for every typed member it compares or assigns, and
    the cast costs a register (fn_800D9A0C, fn_80125C2C: 100% as the untyped M2C_FIELD form, 98-99% with it)"""
    def fix(m):
        if m.start() > 0 and re.match(r'[\w)\]]', body[m.start() - 1]):
            return m.group(0)
        return m.group(2) if member_type(m.group(2)) == m.group(1) else m.group(0)
    return re.sub(r'\((%s)\) (%s)' % ('|'.join(SCALARS), MEMBER), fix, body)


def expand_ptr_compound(body):
    """a variant (step 79): `cr_g_P->f |= Y;` through a global pointer -> `cr_g_P->f = cr_g_P->f | Y;`: the original
    loads the pointer twice (`lwz r4, P; lwz r3, P; lwz r0, 0(r4); or; stw r0, 0(r3)`: fn_800A0804, every script
    handler's test-and-clear of cr_g_ScenarioSlotPtr->status), which the compound form compiles to one load"""
    return re.sub(r'^(\s*)(cr_g_\w+->\w+) (<<|>>|[-+*/%&|^])= ([^;]+);$', r'\1\2 = \2 \3 \4;', body, flags=re.M)


def script_counter_ptr(body):
    """step 80: the 300 script-command handlers (cr_g_ScrCmdFuncs) begin alike: log the command word at cr_g_ScriptPC,
    advance the PC, step the history index, count the command in cr_g_Script.cmd_count[cr_g_ScriptDepth]. The
    original saves r30 and never uses it: a pointer local to the counter whose only use the peephole folded back
    into the base register (`lwz r3, 0xc(r4); addi; stw r0, 0xc(r4)` where the pointer would have been r30 =
    r4 + 0xC; the compiler keeps the callee-saved slot). The untyped drafts had m2c's own `temp_r4 = &lbl[depth]`
    and scored 78-85%; the typed member (step 79) made m2c fold it (`cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1`,
    about 8 points lower). The counter's address goes back into a local, set right after the PC is read:
    `cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth]; ... (*cmd_cnt)++;`. 85% is the family's ceiling with every
    statement order and spelling tried (docs/drafts.md "Ninth generator"); the rest is the scheduler's."""
    m = re.search(r'^([ \t]+)cr_g_Script\.cmd_count\[cr_g_ScriptDepth\] \+= 1;$', body, re.M)
    if not m or 'cmd_cnt' in body:
        return body
    ind = m.group(1)
    head = body[:m.start()]
    anchor = None
    for pat in (r'^[ \t]+\w+ = cr_g_ScriptPC;\n', r'^[ \t]+cr_g_Script\.cmd_history\['):
        hits = list(re.finditer(pat, head, re.M))
        if hits:
            anchor = hits[-1].end() if pat.endswith(r'\n') else hits[-1].start()
            break
    if anchor is None:
        return body
    body = head[:anchor] + ind + 'cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];\n' + head[anchor:] + ind + '(*cmd_cnt)++;' + body[m.end():]
    return declare_cmd_cnt(body)


def script_head_dead(body):
    """step 110: the script-command head as the original has it (cr_CM_DUMMY_008 38 of 38 words: decomp-permuter found
    the order, docs/sched_model.md section 19). The statements in their plain order: the command word stored as
    `(s16)` (m2c's `(s32)` changes the first pass), the history index stepped and masked, the counter incremented in
    place, the PC advanced LAST; and the counter pointer local used only in dead code, `if (0) { (*cmd_cnt)++; }`: its
    uses still count for the register order, so it keeps its callee-saved register (the saved, never used r30 of the
    pure handlers) while its code is removed."""
    m = re.search(r'^([ \t]+)cr_g_Script\.cmd_count\[cr_g_ScriptDepth\] \+= 1;$', body, re.M)
    if not m or 'cmd_cnt' in body:
        return body
    ind = m.group(1)
    head = body[:m.start()]
    adv = list(re.finditer(r'^[ \t]*(cr_g_ScriptPC \+= \d+;)\n', head, re.M))
    hist = list(re.finditer(r'^([ \t]+cr_g_Script\.cmd_history\[cr_g_Script\.history_index\] = )\(s32\) '
                            r'(M2C_FIELD\(\w+, s16 \*, 0\)|\*\w+);$', head, re.M))
    if not adv or not hist:
        return body
    h = hist[-1]
    a = adv[-1]
    if a.start() < h.start():
        return body
    head = head[:a.start()] + head[a.end():]
    head = head[:h.start()] + h.group(1) + '(s16) ' + h.group(2) + ';' + head[h.end():]
    body = (head + ind + 'cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;\n' + ind + a.group(1) + '\n' +
            ind + 'if (0) {\n' + ind + '    (*cmd_cnt)++;\n' + ind + '}' + body[m.end():])
    return declare_cmd_cnt(body)


def cmd_cnt_last(body):
    """step 110 (a variant): the dead-code counter pointer of script_head_dead declared after the other locals. Its
    dead uses rank it among the live locals for the callee-saved registers; declared last it loses the ties
    (cr_CM_RESRV: the camera local takes r30, as in the original)."""
    if 'if (0) {' not in body or 'u32 *cmd_cnt;' not in body:
        return body
    lines = body.split('\n')
    i = next(k for k, l in enumerate(lines) if l.strip() == 'u32 *cmd_cnt;')
    d = lines.pop(i)
    j = i
    while j < len(lines) and re.match(r'^\s+[\w ]+?\**\s*\w+(?:\[\w*\])*;$', lines[j]):
        j += 1
    lines.insert(j, d)
    return '\n'.join(lines)


def declare_cmd_cnt(body):
    lines = body.split('\n')
    di = next((i for i, l in enumerate(lines) if DEF.match(l)), None)
    if di is None:
        return body
    decl = '    u32 *cmd_cnt;'
    # step 84: declared right after the PC local, not first: the compiler hands the callee-saved registers out in
    # declaration order (r31 down) and the original's PC local is the register above the pointer's (r31 / r30 in the
    # pure handlers); the 40 DUMMY handlers were at 44% with the pointer declared first and the two registers swapped
    pcm = re.search(r'^[ \t]+(\w+) = cr_g_ScriptPC;$', body, re.M)
    pi = None
    if pcm:
        for i in range(di + 1, len(lines)):
            if not lines[i].strip():
                break
            if re.match(r'^\s+[\w ]+?\**\s*%s;$' % re.escape(pcm.group(1)), lines[i]):
                pi = i
                break
    if pi is not None:
        lines.insert(pi + 1, decl)
    elif di + 1 < len(lines) and re.match(r'^\s+[\w ]+?\**\s*\w+(?:\[\w*\])*;$', lines[di + 1]):
        lines.insert(di + 1, decl)
    else:
        lines[di + 1:di + 1] = [decl, '']
    return '\n'.join(lines)


def member_offsets():
    """{struct: [(offset, type, name, dims)]} of symbols/cr_types.h (step 82), read once per process"""
    if 'moffs' not in W:
        structs, cur = {}, None
        for l in open(os.path.join(G, 'symbols/cr_types.h'), encoding='utf-8'):
            m = re.match(r'^typedef struct (\w+) \{', l)
            if m:
                cur = structs.setdefault(m.group(1), [])
                continue
            if l.startswith('}'):
                cur = None
                continue
            m = re.match(r'^\s*/\* \+([0-9A-Fa-f]+) \*/ ([\w ]+?)(\**) (\w+)((?:\[[^\]]+\])*);', l) if cur is not None else None
            if m:
                cur.append((int(m.group(1), 16), m.group(2).strip() + m.group(3), m.group(4), m.group(5)))
        W['moffs'] = structs
    return W['moffs']


def type_width(t):
    """the byte width of a scalar / pointer type name ('u16' 2, 'HSD_JObj *' 4), None when unknown"""
    t = t.strip()
    if t.endswith('*'):
        return 4
    return LOCAL_SIZE.get(t)


def split_slot_structs(body):
    """step 82: m2c gives a stack slot one C type, but the original reused the slot for the addresses of two typed
    globals (`sp48 = &cr_g_Scenario; ... sp48 = (cr_ScenarioState *) &cr_g_Script; sp48->unk_470 = 0;`), so the
    members of the second are looked up in the first's struct and the draft does not compile (cr_Scenario_Reset,
    cr_Save_ResetData, fn_8011023C). Every assignment of a typed global's address to a slot local starts a binding:
    when a slot is bound to more than one struct type, or to a type other than its declared one, each binding gets
    its own pointer local (`sp48_cr_g_Script`, typed with the global's struct) and the slot's uses up to the next
    assignment of the slot are renamed to it."""
    structs, globs = member_types()
    lines = body.split('\n')
    di = next((i for i, l in enumerate(lines) if DEF.match(l)), None)
    if di is None:
        return body
    ASSIGN = re.compile(r'^(\s*)(sp[0-9A-F]+) = (?:\((?:struct )?[\w ]+?\**\) )?\(?&(cr_g_\w+)\)?;$')
    SLOT = re.compile(r'^\s*(sp[0-9A-F]+) = ')
    bind = {}
    for l in lines[di:]:
        m = ASSIGN.match(l)
        if m and m.group(3) in globs and globs[m.group(3)] in structs:
            bind.setdefault(m.group(2), set()).add(m.group(3))
    todo = set()
    for sp, gl in bind.items():
        decl = re.search(r'^\s+([\w ]+?)\s*(\**)\s*\b%s;' % sp, body, re.M)
        if not decl:
            continue
        types = {globs[g] for g in gl}
        declared = decl.group(1).strip() if decl.group(2) == '*' else None
        if len(types) > 1 or declared not in types:
            todo.add(sp)
    if not todo:
        return body
    out = lines[:di + 1]
    cur = {}
    added = []
    for l in lines[di + 1:]:
        m = ASSIGN.match(l)
        if m and m.group(2) in todo and m.group(3) in globs:
            nm = '%s_%s' % (m.group(2), m.group(3))
            cur[m.group(2)] = nm
            if (nm, globs[m.group(3)]) not in added:
                added.append((nm, globs[m.group(3)]))
            out.append('%s%s = &%s;' % (m.group(1), nm, m.group(3)))
            continue
        sm = SLOT.match(l)
        if sm and sm.group(1) in cur:
            cur.pop(sm.group(1))
        for sp, nm in cur.items():
            l = re.sub(r'(?<![\w.>])%s\b' % sp, nm, l)
        out.append(l)
    out[di + 1:di + 1] = ['    %s *%s;' % (t, nm) for nm, t in added]
    return '\n'.join(out)


def unsigned_not_mask(body):
    """step 82: `cr_g_StageSelector.flags &= ~arg0;` on a u32 member with a signed operand: the original converted the
    operand first (`&= ~(u32) arg0`), which gives the base register the scheduler's first choice (fn_80111E0C 77 ->
    100%); with the s32 operand the compiler keeps the base in another register. Only a direct member of a global
    struct: through a global pointer with an index (cr_F_CLR, `cr_g_ScenarioSlotPtr->flags[i] &= ~x`) the cast
    costs three points."""
    def fix(m):
        if member_type(m.group(2)) not in ('u32', 'u16', 'u8'):
            return m.group(0)
        if not re.search(r'\bs32 \**%s\b' % m.group(3), body):
            return m.group(0)
        return '%s%s &= ~(u32) %s;' % (m.group(1), m.group(2), m.group(3))
    return re.sub(r'^(\s*)(cr_g_\w+(?:\.\w+)+) &= ~(\w+);$', fix, body, flags=re.M)


def member_array_index(body):
    """step 82: `M2C_FIELD((&cr_g_StageSelector + ((var_r28 * 2) & 0x1FE)), u16 *, 0x3E)` is pointer arithmetic on the
    struct (a step of sizeof: `mulli r6, r5, 0x354`); the struct has an array of that width at that offset, so the
    access is `cr_g_StageSelector.stage_msgs[var_r28]` (fn_80111EEC 92 -> 100%)"""
    structs, globs = member_types()
    offs = member_offsets()

    def fix(m):
        g, idx, scale, t, off = m.group(1), m.group(2), int(m.group(3), 0), m.group(4), int(m.group(5), 16)
        st = globs.get(g)
        if st not in offs or type_width(t) != scale:
            return m.group(0)
        for o, mt, name, dims in offs[st]:
            if o == off and dims.count('[') == 1 and type_width(mt) == scale and not mt.endswith('*'):
                return '%s.%s[%s]' % (g, name, idx)
        return m.group(0)
    return re.sub(r'M2C_FIELD\(\(&(cr_g_\w+) \+ \(?\((\w+) \* (0x[0-9A-F]+|\d+)\)(?: & 0x[0-9A-F]+)?\)?\), ([\w ]+?\s?\**)\*, 0x([0-9A-F]+)\)', fix, body)


def struct_sizes():
    """{struct: size} of the ASSERT_SIZE comments of symbols/cr_types.h (step 90), read once per process"""
    if 'ssizes' not in W:
        W['ssizes'] = {m.group(1): int(m.group(2), 16) for m in re.finditer(
            r'ASSERT_SIZE\((\w+), 0x([0-9A-Fa-f]+)\)', open(os.path.join(G, 'symbols/cr_types.h'), encoding='utf-8').read())}
    return W['ssizes']


def struct_byte_offset(body):
    """step 90: `M2C_FIELD((&cr_g_BattleRobos[arg0] + (var_r30 * 4)), s32 *, 0x864)`: m2c adds a byte offset to the address
    of a struct-typed global (or of one element of a struct array), and C scales the sum by the struct's size (0x33520
    here: the store lands 0x33520 * 4 * i bytes away). When the step is no multiple of the struct's size it is bytes:
    `(u8 *) &cr_g_BattleRobos[arg0] + (var_r30 * 4)` (fn_8010D2D0 65 -> 100%; the struct's first member was a u8 array
    before step 90's `update`, and m2c indexed that array instead). A step that is a multiple (member_array_index's
    `mulli 0x354`) stays."""
    structs, globs = member_types()
    sizes = struct_sizes()

    def fix(m):
        g, idx, mul, step = m.group(1), m.group(2) or '', m.group(3), int(m.group(4), 0)
        st = re.sub(r'\[\d+\]$', '', globs.get(g, ''))
        if st not in sizes or step % sizes[st] == 0:
            return m.group(0)
        return 'M2C_FIELD(((u8 *) &%s%s + (%s' % (g, idx, mul)
    return re.sub(r'M2C_FIELD\(\(&(cr_g_\w+)(\[\w+\])? \+ \((\w+ \* (0x[0-9A-F]+|\d+)\))', fix, body)


def indexed_offset(body):
    """step 82: `M2C_FIELD((arg0 + (((var_r30 * 4) & 0x3FC) + 0xAC)), u32 *, 0)` where the index is scaled by the access
    width: an array of that element type at the constant offset. The compiler folds the constant into the load
    (`add; lwz 0xac(r)`) where the original adds it to the index and loads indexed (`addi r0, r3, 0xac; lwzx`), the
    form it gives `p->array[i]`; the offset as the macro's argument compiles so (fn_8008688C). Only through a pointer
    variable (not a global's address: `lbl[i].field` folds in the original) and only when the scale is the width
    (a record array's field, `(i * 0x78) + 0x2C` for an s32, folds too: round 8 lost 16 matches without this gate)."""
    def fix(m):
        base, idx, scale, mask, const, t = m.group(1), m.group(2), int(m.group(3), 0), m.group(4) or '', m.group(5), m.group(6)
        if base.startswith('lbl_') or base.startswith('cr_g_') or type_width(t) != scale:
            return m.group(0)
        return 'M2C_FIELD((%s + ((%s * %s)%s)), %s *, %s)' % (base, idx, m.group(3), mask, t.strip(), const)
    return re.sub(r'M2C_FIELD\(\((\w+) \+ \(\(\((\w+) \* (0x[0-9A-F]+|\d+)\)( & 0x[0-9A-F]+)?\) \+ (0x[0-9A-F]+)\)\), ([\w ]+?\s?\**)\*, 0\)', fix, body)



def fold_temp_compound(body):
    """step 85: m2c's temporary for a caller-saved register that holds an address used twice in the next statement, as
    the base of the load and of the store of one field (`temp_r3_44 = p->a->b; M2C_FIELD(temp_r3_44, f32 *, 0x14) =
    (f32) ((f64) M2C_FIELD(temp_r3_44, f32 *, 0x14) + lbl_80450E70);`, or `M2C_FIELD(temp_r4_35, u16 *, 0x12) |=
    0x2000;`): a local of the draft gets a callee-saved register (`lwz r30, 4(r3)`, saved and restored), while the
    original evaluated the address once inside a compound assignment and kept it in r3 / r4 (`lwz r3, 4(r3); lfs f1,
    0x14(r3); ...; stfs f0, 0x14(r3)`). Folded into `M2C_FIELD(ADDR, T *, OFF) op= RHS;` when the register is r3-r12
    (a temporary, not a local the original kept across a call), the temporary has no other use, and, for the
    `(f32) ((f64) x + C)` form, C is a double (a literal without an f suffix, or an .sdata2 double), so the arithmetic
    stays the compiler's own f64 promotion. fn_80080B80 92 -> 100%, fn_80080FD8 (step 85)."""
    doubles = {k for k, v in pool_floats().items() if not v.endswith('f')}
    pat = re.compile(r'^([ \t]*)(temp_r(\d+)_\d+) = (.+);\n[ \t]*M2C_FIELD\(\2, ([\w ]+?) \*, (0x[0-9A-F]+|\d+)\) (?:'
                     r'(<<|>>|[-+*/%&|^])= (.+)|= \(f32\) \(\(f64\) M2C_FIELD\(\2, f32 \*, \6\) ([-+*/]) (.+)\));$', re.M)

    def fix(m):
        ind, tmp, reg, addr, t, off = m.group(1), m.group(2), int(m.group(3)), m.group(4), m.group(5), m.group(6)
        if not 3 <= reg <= 12 or len(re.findall(r'\b%s\b' % tmp, body)) != len(re.findall(r'\b%s\b' % tmp, m.group(0))) + 1:
            return m.group(0)   # the declaration is the only other occurrence
        if m.group(7):
            op, rhs = m.group(7), m.group(8)
        else:
            op, rhs = m.group(9), m.group(10)
            if t.strip() != 'f32' or not (re.fullmatch(r'-?\d+\.\d+(?:e-?\d+)?', rhs) or rhs in doubles):
                return m.group(0)
        if tmp in addr or tmp in rhs:
            return m.group(0)
        return '%sM2C_FIELD(%s, %s *, %s) %s= %s;' % (ind, addr, t, off, op, rhs)
    out = pat.sub(fix, body)
    if out != body:
        for tmp in set(re.findall(r'\btemp_r\d+_\d+\b', body)):
            if len(re.findall(r'\b%s\b' % tmp, out)) == 1:
                out = re.sub(r'^[ \t]*[\w ]+?\s?\**%s;[ \t]*\n' % tmp, '', out, count=1, flags=re.M)
    return out


def store_literals(body):
    """step 85, a variant: a float constant of .sdata2 written as the literal it holds only where it is a whole stored
    value (`M2C_FIELD(p, f32 *, 0x14) = (f32) lbl_80450E5C;` -> `= 0.0f;`), the constants in arithmetic and compares
    staying externs. The compiler schedules its own literal freely (the `lfs` of 0.0f moves above a store through a
    pointer: an extern may alias with it, a literal cannot), while the same function's `x + 3.875f < 255.0f` matches
    only with the externs (as literals the compiler swaps the operands: fn_80080A94 98 -> 100%, 91% with
    float_literals). The literal is the draft's own pool constant like float_literals' (ownership for the file run)."""
    fl = pool_floats()
    if not fl:
        return body
    return re.sub(r'= (?:\(f32\) |\(f64\) )?(lbl_[0-9A-F]{8});$', lambda m: '= %s;' % fl[m.group(1)] if m.group(1) in fl else m.group(0), body, flags=re.M)

def hoist_float_temps(body):
    """step 92, a variant: in a function whose body is a flat list of one-line statements (no blocks), every float
    temporary (`var_fNN = EXPR;`) moves up to the earliest place its operands allow, so the temporaries come before
    the stores m2c interleaved with them. m2c places a temporary where its result is formed in the scheduled code;
    the original wrote the temporaries first, and the scheduler interleaves (docs/drafts.md "Step 92": fn_8001CCD4,
    fn_8001CD64, fn_8001CDF4, fn_8001D254 -> 100%). A temporary stays below a store to the same field it reads
    (`M2C_FIELD(b, T *, off)` with the same base and offset), and below the statement that assigns a local it reads."""
    i = body.find(') {\n')
    j = body.find('\n}', i)
    if i < 0 or j < 0 or '{' in body[i + 4:j] or '}' in body[i + 4:j]:
        return body
    lines = body[i + 4:j].split('\n')
    stmts = [l for l in lines if l.startswith('    ') and not l.startswith('     ') and l.rstrip().endswith(';')
             and ' = ' in l]
    if len(stmts) < 3 or not any(re.match(r'^    var_f\d+ = ', s) for s in stmts):
        return body
    first = lines.index(stmts[0])
    if lines[first:first + len(stmts)] != stmts:
        return body
    field = re.compile(r'M2C_FIELD\((\w+), [\w ]+\*, (\w+)\)')
    out = []
    for s in stmts:
        t = re.match(r'^    (var_f\d+) = (.*);$', s)
        if not t:
            out.append(s)
            continue
        rhs = t.group(2)
        reads = set(field.findall(rhs))
        locs = set(re.findall(r'\b(var_\w+|temp_\w+)\b', rhs))
        k = len(out)
        while k > 0:
            p = out[k - 1]
            lhs = p.split(' = ', 1)[0].strip()
            if lhs in locs or re.search(r'\b%s\b' % t.group(1), p):
                break
            w = field.match(lhs)
            if w and (w.group(1), w.group(2)) in reads:
                break
            if not w:
                break
            k -= 1
        out.insert(k, s)
    if out == stmts:
        return body
    new = lines[:first] + out + lines[first + len(stmts):]
    return body[:i + 4] + '\n'.join(new) + body[j:]


def widen_narrow_params(body):
    """step 93, a variant: the u8 / s8 / u16 / s16 parameters declared s32. m2c types a parameter by its uses (a `sth`
    of it makes it u16); the original passed an int, so the front end converted it at the narrow store and the
    passes between the two schedulings deleted the conversion after it had taken an issue slot in the first pass
    (fn_8001F6BC: `PADControlMotor` arguments after the store, 39 of 39 with `s32 arg1`; docs/drafts.md "Step 93")."""
    i = body.find(') {\n')
    if i < 0:
        return body
    k = body.rfind('(', 0, i)
    if k < 0:
        return body
    sig = re.sub(r'\b(?:u8|s8|u16|s16) (arg\d+)\b', r's32 \1', body[k:i])
    return body[:k] + sig + body[i:]


def masked_index_field(body):
    """step 95, a variant: m2c's `M2C_FIELD(&B[(X * N) & MASK], T, OFF)` (its reading of a `clrlslwi` on a u8 / u16
    index) written as the byte arithmetic `*(T) (B + OFF + X * N)`. The original computed the element's address in
    one expression; the masked index and the field macro give the same words in another order (fn_800B4710: the
    call arguments of HSD_JObjDispAll, 50 -> 56 of 56; docs/drafts.md "Step 95")."""
    body = re.sub(r'M2C_FIELD\(&(\w+)\[\((\w+) \* (\d+)\) & 0x[0-9A-Fa-f]+\], ([^,()]+), (0x[0-9A-Fa-f]+|\d+)\)',
                  r'*(\4) (\1 + \5 + \2 * \3)', body)
    # step 96: the bare form in an address sum, `+ ((X * N) & MASK)` -> `+ (X * N)` (fn_800B497C, 73 of 73)
    return re.sub(r'\(\((\w+) \* (\d+)\) & 0x[0-9A-Fa-f]+\)', r'(\1 * \2)', body)


def byte_compare_casts(body):
    """step 96, a variant: a byte member compared through m2c's `(s32) X` (its reading of `lbz` + `cmpwi`) written
    `(s32) (u8) X`. The second conversion is the front end's `clrlwi`, deleted between the two schedulings after it
    took an issue slot in the first (fn_800B47F0: the `mulli` of the element offset after the `lbz`, 92 -> 99 of 99;
    docs/drafts.md "Step 96")."""
    cmp_ = r' (==|!=|<=|>=|<|>)'
    body = re.sub(r'\(s32\) (M2C_FIELD\([^()]*, u8 \*, [^()]*\))' + cmp_, r'(s32) (u8) \1 \2', body)
    return re.sub(r'\(s32\) (cr_g_\w+(?:\.\w+|\[[^\]()]*\])+)' + cmp_, r'(s32) (u8) \1 \2', body)


def shift_mask_order(body):
    """step 96, a variant: m2c's `(E << N) & M` (its reading of `clrlslwi`) written `(E & (M >> N)) << N` when M is
    a mask shifted by N; with sink_const_global_stores fn_80037894 matches (docs/drafts.md "Step 96")."""
    def f(m):
        n, mask = int(m.group(2)), int(m.group(3), 16)
        if mask & ((1 << n) - 1) or not mask:
            return m.group(0)
        return '(%s & 0x%X) << %d' % (m.group(1), mask >> n, n)
    return re.sub(r'\(([\w.\[\]>-]+) << (\d+)\) & 0x([0-9A-Fa-f]+)', f, body)


def sink_const_global_stores(body):
    """step 96, a variant: a store of a constant to a global (`lbl_X = 0;`, `*lbl_X = 0;`, `lbl_X[0] = 0;`) moved down
    past the simple statements that follow it at the same depth, up to the next control statement or a statement
    that names the global. m2c places the store where its `stw` is in the scheduled code; the original wrote it
    later and the scheduler issued it early (fn_80037894: after the two stores of the mode word; docs/drafts.md
    "Step 96")."""
    lines = body.split('\n')
    store = re.compile(r'^(\s+)\*?(lbl_\w+)(?:\[\d+\])? = (?:0x[0-9A-Fa-f]+|\d+)U?;$')
    k = 0
    while k < len(lines):
        m = store.match(lines[k])
        if not m:
            k += 1
            continue
        ind, g = m.group(1), m.group(2)
        j = k + 1
        while j < len(lines):
            l = lines[j]
            if (store.match(l) or not l.startswith(ind) or l.startswith(ind + ' ') or not l.rstrip().endswith(';')
                    or l.strip().startswith(('return', 'break', 'goto', 'continue')) or re.search(r'\b%s\b' % g, l)
                    or '(' in l.split(' = ', 1)[-1] and not re.match(r'^\s*[\w.\[\]>*-]+ [|&^+-]?= ', l)):
                break
            j += 1
        if j > k + 1:
            lines.insert(j - 1, lines.pop(k))
            k = j
        else:
            k += 1
    return '\n'.join(lines)


def _split_top(s, op):
    """split s at the top-level occurrences of the operator op (' + ', ' * ', ' << ', ' & ', ', ')"""
    parts, depth, k, last = [], 0, 0, 0
    while k < len(s):
        c = s[k]
        if c in '([':
            depth += 1
        elif c in ')]':
            depth -= 1
        elif depth == 0 and s.startswith(op, k):
            parts.append(s[last:k])
            k += len(op)
            last = k
            continue
        k += 1
    parts.append(s[last:])
    return parts


def _strip_parens(s):
    """drop the parentheses that enclose all of s"""
    s = s.strip()
    while s.startswith('(') and s.endswith(')'):
        depth = 0
        for k, c in enumerate(s):
            depth += c == '('
            depth -= c == ')'
            if depth == 0 and k < len(s) - 1:
                return s
        s = s[1:-1].strip()
    return s


_NUM = re.compile(r'0x[0-9A-Fa-f]+|\d+')


def _record_index(addr):
    """`B + ((I * S) + O)`, `B + ((I << N) + O)` or `B + (((I << N) & M) + O)` with 0 < O < S -> (B, I, S, O)"""
    parts = _split_top(_strip_parens(addr), ' + ')
    if len(parts) != 2:
        return None
    base, terms = parts[0].strip(), _split_top(_strip_parens(parts[1]), ' + ')
    if len(terms) != 2 or not _NUM.fullmatch(terms[1].strip()):
        return None
    off, scaled = int(terms[1].strip(), 0), _strip_parens(terms[0])
    masked = _split_top(scaled, ' & ')
    if len(masked) == 2 and _NUM.fullmatch(masked[1].strip()):
        scaled = _strip_parens(masked[0])
    for op in (' * ', ' << '):
        f = _split_top(scaled, op)
        if len(f) == 2 and _NUM.fullmatch(f[1].strip()):
            n = int(f[1].strip(), 0)
            size = n if op == ' * ' else 1 << n
            if size >= 1 and 0 < off and f[0].strip():   # step 108: also an array at an offset past the step (`p->arr[i]`, fn_800A5894 0x90 / 4) and a byte step (fn_8008E158)
                return base, f[0].strip(), size, off
    return None


def record_array_index(body):
    """step 108, a variant: an access at `B + ((I * S) + O)` (O inside the S-byte record) written as a record-array
    element, `*(T *) &((u8 (*)[S]) (B))[I][O]`. The compiler then adds O to the scaled index and loads or stores
    indexed (`addi r0, r4, 0x1b; stbx`), as the original does in fn_8008C3B4, fn_80064150 and fn_8000B144; from the
    byte sum it folds O into the displacement (`add; stb 0x1b(r5)`). indexed_offset leaves record arrays alone because
    others fold in the original (round 8), so this is a variant, tried per draft."""
    field = re.compile(r'M2C_FIELD\(')
    cast = re.compile(r'\(\*\((\w+ \*)\)\(\(s8 \*\)\(')
    out, k = [], 0
    while k < len(body):
        cands = [x for x in (field.search(body, k), cast.search(body, k)) if x]
        if not cands:
            out.append(body[k:])
            break
        m = min(cands, key=lambda x: x.start())
        out.append(body[k:m.start()])
        depth, j = (1 if m.re is field else 3), m.end()
        while j < len(body) and depth:
            depth += body[j] == '('
            depth -= body[j] == ')'
            j += 1
        inner, rep = body[m.end():j - 1], None
        if m.re is field:
            args = _split_top(inner, ', ')
            if len(args) == 3 and args[2].strip() in ('0', '0x0'):
                r = _record_index(args[0])
                if r:
                    rep = '*(%s) &((u8 (*)[0x%X]) (%s))[%s][0x%X]' % (args[1].strip(), r[2], r[0], r[1], r[3])
        else:
            # (*(T *)((s8 *)(( ADDR )) + (0)))
            tail = re.fullmatch(r'(.*)\) \+ \(0\)\)', inner, re.S)
            if tail:
                r = _record_index(tail.group(1))
                if r:
                    rep = '(*(%s) &((u8 (*)[0x%X]) (%s))[%s][0x%X])' % (m.group(1), r[2], r[0], r[1], r[3])
        if rep:
            out.append(rep)
            k = j
        else:   # keep the opener and look inside it (a nested access may match)
            out.append(body[m.start():m.end()])
            k = m.end()
    return ''.join(out)


def _negate_test(test):
    """the opposite of an m2c test, written as the original would: `(u8) X != 0` -> `X == 0`, `A != B` -> `A == B`,
    `A == B` -> `A != B`, else `!(T)`"""
    m = re.fullmatch(r'\((?:u8|s8|u16|s16)\) (.+) != 0', test)
    if m:
        return '%s == 0' % m.group(1)
    for a, b in ((' != ', ' == '), (' == ', ' != ')):
        parts = _split_top(test, a)
        if len(parts) == 2:
            return parts[0] + b + parts[1]
    return '!(%s)' % test


def for_break_loops(body):
    """step 108, a variant: m2c's reading of a search loop,
           loop_N:                              for (; C; I) {
               if (C) {                             if (!T) {
                   if (T) {             ->              break;
                       S...                         }
                       I;                           S...
                       goto loop_N;             }
                   }
               }
    The compiler gives `for` a jump to the test at the bottom (`b test; body: ...; test: cmpwi; blt body`), m2c's form
    tests at the top (fn_80079544, fn_800796EC: 102 -> 106 of 106, 116 -> 120 of 120). Only the exact shape, with the
    label used once; tried per draft."""
    lines = body.split('\n')
    k = 0
    while k < len(lines):
        m = re.fullmatch(r'(loop_\d+):', lines[k].strip())
        if not m or len(re.findall(r'\bgoto %s;' % m.group(1), body)) != 1:
            k += 1
            continue
        ind = re.match(r'\s*', lines[k + 1] if k + 1 < len(lines) else '').group(0)
        c1 = re.fullmatch(re.escape(ind) + r'if \((.+)\) \{', lines[k + 1]) if k + 1 < len(lines) else None
        c2 = re.fullmatch(re.escape(ind) + r'    if \((.+)\) \{', lines[k + 2]) if k + 2 < len(lines) else None
        if not (c1 and c2):
            k += 1
            continue
        j = k + 3
        inner = []
        while j < len(lines) and lines[j].startswith(ind + '        '):
            inner.append(lines[j][8:])
            j += 1
        if (len(inner) < 2 or inner[-1].strip() != 'goto %s;' % m.group(1) or
                j + 1 >= len(lines) or lines[j] != ind + '    }' or lines[j + 1] != ind + '}'):
            k += 1
            continue
        inc = inner[-2].strip()
        if not re.fullmatch(r'\w+ (?:\+=|-=) (?:0x[0-9A-Fa-f]+|\d+);|\w+(?:\+\+|--);', inc) or \
                any(re.match(r'\s*(?:goto|return|break|continue)\b', x) for x in inner[:-2]):
            k += 1
            continue
        new = [ind + 'for (; %s; %s) {' % (c1.group(1), inc[:-1]),
               ind + '    if (%s) {' % _negate_test(c2.group(1)), ind + '        break;', ind + '    }']
        new += [ind + x for x in inner[:-2]] + [ind + '}']
        lines[k:j + 2] = new
        k += len(new)
    return '\n'.join(lines)


def clz_intrinsic(body):
    """step 108: m2c's `CLZ(x)` (its reading of `cntlzw`) is no function of the game: written as mwcc's intrinsic
    `__cntlzw(x)`, which compiles to the one instruction (fn_80038BD8: 67 -> 72 of 72 with the byte test below)"""
    return re.sub(r'\bCLZ\(', '__cntlzw(', body)


def plain_byte_tests(body):
    """step 108, a variant: m2c's `(u8) *(P) != 0` / `== 0` on a byte it loaded with `lbz` written without the cast:
    the cast gives `clrlwi.`, the plain byte `cmplwi r0, 0` (fn_80038BD8, fn_80079544's search loop)"""
    return re.sub(r'\(u8\) (\*\([^;{}]*?\)) ([!=]=) 0\b', r'\1 \2 0', body)


def bitwise_lvalues(body):
    """step 108, a variant: m2c's `M2C_BITWISE(T, G[i])` on a global's element or a global (its reading of a load of
    another type at that address) as the reinterpretation it is, `(*(T *) &G[i])`: cr_game.h's M2C_BITWISE is a value
    cast, which converts (`lfs` + `__cvt_fp2unsigned` where main.dol has one `lwz`: fn_8008CE9C 74 -> 77 of 77)"""
    return re.sub(r'M2C_BITWISE\(([\w ]+\**), ((?:lbl_[0-9A-F]{8}|cr_g_\w+)(?:\[[^\[\]()]+\])?)\)', r'(*(\1 *) &\2)', body)


def void_self(body):
    """step 108, a variant: the drafted function itself as `void`, its `return X;` as `X;` (a call) or `return;`.
    r3_returns gives a function the value its callers read; m2c then also returns r3 from the function's own draft,
    which can add code where the original simply left the value in r3 (fn_8007D17C, fn_80068358: 100% before)."""
    m = re.search(r'^(?!void )([A-Za-z_][\w ]*?\**) ?(\w+)\(([^;{]*)\) \{$', body, re.M)
    if not m:
        return body
    body = body[:m.start()] + 'void %s(%s) {' % (m.group(2), m.group(3)) + body[m.end():]
    return re.sub(r'\breturn ([^;]+);', lambda r: (r.group(1).strip() + ';') if '(' in r.group(1) else 'return;', body)


def byte_loops(body):
    """step 109a, a variant: m2c's reading of a byte copy / fill loop,
           c = 0U;                                  for (c = 0U, p = X; c < N; c += 1) {
           p = X;                                       *p++ = 0;
           while (c < N) {                          }
               c += 1;              ->
               *p = 0;                          (`*d = (u8) (*s); c += 1; s += 1; d += 1;` -> `*d++ = *s++;`)
               p += 1;
           }
    as the source wrote it, the pointer stepped in the store (fn_80078F94: 54 -> 59 of 59, the counter and the pointer
    in r30 / r29 as main.dol has them; with m2c's while they swap). The assignments of the counter and the pointers
    just before the loop go into the `for`; a same-type cast on the copied byte goes. Tried per draft."""
    lines = body.split('\n')
    k = 0
    stmt = re.compile(r'(\w+) \+= 1;|\*(\w+) = (?:\((?:u8|s8)\) \(?\*(\w+)\)?|\*(\w+)|(0x[0-9A-Fa-f]+|\d+));')
    while k < len(lines):
        m = re.fullmatch(r'(\s*)while \((\w+) < ((?:0x[0-9A-Fa-f]+|\d+)U?)\) \{', lines[k])
        if not m or k + 5 >= len(lines):
            k += 1
            continue
        ind, c = m.group(1), m.group(2)
        j = k + 1
        inner = []
        while j < len(lines) and lines[j].startswith(ind + '    '):
            inner.append(lines[j].strip())
            j += 1
        if j >= len(lines) or lines[j] != ind + '}' or not 3 <= len(inner) <= 4:
            k += 1
            continue
        incs, store = [], None
        for s in inner:
            sm = stmt.fullmatch(s)
            if not sm:
                store = False
                break
            if sm.group(1):
                incs.append(sm.group(1))
            elif store is None:
                store = (sm.group(2), sm.group(3) or sm.group(4), sm.group(5))
            else:
                store = False
                break
        if not store or c not in incs:
            k += 1
            continue
        dst, src, const = store
        ptrs = [dst] + ([src] if src else [])
        if sorted(incs) != sorted([c] + ptrs) or len(set(incs)) != len(incs):
            k += 1
            continue
        # the counter's and the pointers' assignments just before the loop
        i = k
        inits = []
        while i > 0:
            am = re.fullmatch(re.escape(ind) + r'(\w+) = ([^;]+);', lines[i - 1])
            if not am or am.group(1) not in [c] + ptrs or am.group(1) in [x[0] for x in inits]:
                break
            inits.insert(0, (am.group(1), am.group(2)))
            i -= 1
        if c not in [x[0] for x in inits]:
            k += 1
            continue
        val = ('*%s++' % src) if src else const
        new = [ind + 'for (%s; %s < %s; %s += 1) {' % (', '.join('%s = %s' % x for x in inits), c, m.group(3), c),
               ind + '    *%s++ = %s;' % (dst, val), ind + '}']
        lines[i:j + 1] = new
        k = i + len(new)
    return '\n'.join(lines)


def local_same_casts(body):
    """step 109a, a variant: `*d = (u8) (*s);` / `x = (s16) *p;` where s / p is a local or parameter declared as a
    pointer to that type: the cast means nothing, and at level 0 it costs a register (fn_80026C50: `lbz r4; stb r4`
    where main.dol has r0). drop_same_assign_casts does the same for globals."""
    decl = {}
    for t, stars, v in re.findall(r'(?:^    |[(,] ?)((?:unsigned |signed )?\w+) (\*+)(\w+)(?=;\n|[,)])', body, re.M):
        decl[v] = (t, len(stars))

    def fix(m):
        t, v = m.group(2), m.group(3) or m.group(4)
        if decl.get(v) != (t, 1):
            return m.group(0)
        return '%s*%s;' % (m.group(1), v)
    return re.sub(r'(= )\((u8|s8|u16|s16|u32|s32|f32)\) (?:\(\*(\w+)\)|\*(\w+));', fix, body)


def record_pointers(body):
    """step 109a, a variant: a pointer into a record array, m2c's `p = B + ((I * S) + O);` or its two statements
    `p = (T *) ((I * S) + O); ... p += (s32) B;`, written as the element's address `p = &((u8 (*)[S]) (B))[I][O];`:
    the compiler then adds O to the scaled index first and the base last (`mulli; addi r30, r5, O; add r30, r3, r30`)
    as main.dol does (fn_80026BF4, fn_80026A7C, fn_800442EC); from the sum it adds the base first. As
    record_array_index for loads and stores; tried per draft."""
    def rec(b, i, s, o):
        return '&((u8 (*)[0x%X]) (%s))[%s][0x%X]' % (s, b, i, o)
    lines = body.split('\n')
    for k, l in enumerate(lines):
        m = re.fullmatch(r'(\s*)(\w+) = (?:\([\w ]+\*\) )?([^;=]+);', l)
        if not m:
            continue
        r = _record_index(m.group(3))
        if r and re.fullmatch(r'\w+', r[0]):
            lines[k] = '%s%s = %s;' % (m.group(1), m.group(2), rec(*r))
            continue
        # the two statements: the offset first, the base added later (nothing between reads or writes p or B)
        parts = _split_top(_strip_parens(m.group(3)), ' + ')
        if len(parts) != 2 or not _NUM.fullmatch(parts[1].strip()):
            continue
        sc = _split_top(_strip_parens(parts[0]), ' * ')
        if len(sc) != 2 or not _NUM.fullmatch(sc[1].strip()) or not re.fullmatch(r'\w+', sc[0].strip()):
            continue
        p = m.group(2)
        for j in range(k + 1, min(k + 8, len(lines))):
            am = re.fullmatch(re.escape(m.group(1)) + r'%s \+= \(s32\) (\w+);' % re.escape(p), lines[j])
            if am:
                b = am.group(1)
                if all(not re.search(r'\b(%s|%s|%s)\b' % (re.escape(p), re.escape(b), re.escape(sc[0].strip())), x)
                       or not re.search(r'\b(%s|%s)\b\s*(?:[-+*/|&^]|<<|>>)?=(?!=)' % (re.escape(p), re.escape(b)), x)
                       for x in lines[k + 1:j]) and not any(re.search(r'\b%s\b' % re.escape(p), x) for x in lines[k + 1:j]):
                    lines[k] = '%s%s = %s;' % (m.group(1), p, rec(b, sc[0].strip(), int(sc[1].strip(), 0), int(parts[1].strip(), 0)))
                    del lines[j]
                break
    return '\n'.join(lines)


def param_retypes(body):
    """step 109a, a variant: `void *argN` copied once among the first statements into a local of another pointer type
    (`u8 *var_r29; ... var_r29 = arg0;`) and used nowhere else: the parameter takes the local's type, so param_copies
    (after this rule) can rename the copy to the parameter; a copy after constant assignments (`i = 0; p = arg0;`),
    which param_copies leaves, is renamed here. With m2c's two types the copy stays and the compiler homes the
    parameter on the stack (`stw r3, 0x8(r1); lwz r29, 0x8(r1)` where main.dol has `mr r29, r3`: fn_800442EC)."""
    sig = re.compile(r'^(\w[^\n;]*?\b\w+)\(([^;{}\n]*)\) \{$', re.M)
    m = sig.search(body)
    if not m:
        return body
    for a in re.findall(r'\bvoid \*(arg\d+)\b', m.group(2)):
        m = sig.search(body)
        if len(re.findall(r'\b%s\b' % a, body)) != 2:
            continue
        cm = re.search(r'^    (\w+) = %s;\n' % a, body, re.M)
        if not cm:
            continue
        v = cm.group(1)
        vd = re.search(r'^    ((?:unsigned |signed )?\w+ \*+)%s;\n' % re.escape(v), body, re.M)
        if not vd or vd.group(1).startswith('void '):
            continue
        params = m.group(2).replace('void *%s' % a, '%s%s' % (vd.group(1), a))
        start = body.find('\n\n', m.end())
        if 0 <= start < cm.start() and not assigned(v, body[:cm.start()] + body[cm.end():]) and \
                re.fullmatch(r'(    \w+ = (?:0x[0-9A-Fa-f]+|-?\d+)U?;\n)*', body[start + 2:cm.start()]):
            # the parameter itself in the copy's register (`mr r29, r3`)
            body = body[:cm.start()] + body[cm.end():]
            body = body[:vd.start()] + body[vd.end():]
            body = re.sub(r'\b%s\b' % re.escape(v), a, body)
        m = sig.search(body)
        body = body[:m.start(2)] + params + body[m.end(2):]
    return body


def wide_return_locals(body):
    """step 109a, a variant: a function returning a byte / halfword whose returned local m2c typed the same width
    (`s8 var_r26; ... return var_r26;`): in main.dol the local was an int, converted at the return (`extsb r3, r26`
    before the epilogue, where the narrow local gives `mr r3, r26`: fn_800EA548, fn_800EA6BC). The local is
    declared s32."""
    m = re.search(r'^(u8|s8|u16|s16) (\w+)\([^;{}\n]*\) \{$', body, re.M)
    if not m:
        return body
    for v in set(re.findall(r'\breturn (\w+);', body)):
        body = re.sub(r'^(    )%s %s;$' % (m.group(1), re.escape(v)), r'\1s32 %s;' % v, body, flags=re.M)
    return body


def advance_copies(body):
    """step 109a, a variant: m2c's `temp = G; G += K; v = temp;` (temp used nowhere else) as `v = G; G += K;`: the
    value read once into v before the advance, the advance reading G again (cr_C_ACT_REPEAT: `lwz r3, PC; lwz r5, PC;
    addi; stw; stw r5, 0x8(r1)` with v on the stack; m2c's temporary takes a saved register, 91% -> 100%)."""
    def fix(m):
        ind, t, g, k, v = m.groups()
        if len(re.findall(r'\b%s\b' % re.escape(t), body)) != 3:
            return m.group(0)
        return '%s%s = %s;\n%s%s += %s;' % (ind, v, g, ind, g, k)
    new = re.sub(r'(?m)^(\s+)(temp_\w+) = ([\w.\->]+);\n\1\3 \+= (\w+);\n\1(\w+) = \2;$', fix, body)
    if new != body:   # the temporaries' declarations that nothing uses any more
        for t in set(re.findall(r'(?m)^\s+(temp_\w+) = [\w.\->]+;\n\s+[\w.\->]+ \+= \w+;\n\s+\w+ = \1;$', body)):
            if len(re.findall(r'\b%s\b' % re.escape(t), new)) == 1:
                new = re.sub(r'(?m)^    [^;\n=()]*\b%s;\n' % re.escape(t), '', new, count=1)
    return new


def unsigned_immediates(body):
    """step 109a, a variant: m2c reads `cmplwi rX, 0xAF30` as a compare with the sign-extended immediate
    (`-0x50D0U`), and `subis r0, r3, 0x20; cmplwi r0, 0xAF30` (mwcc's `x != 0x20AF30`) as
    `(u32) (x + 0xFFE00000) != -0x50D0U`. Written as the unsigned constants: `x != 0x20AF30` (the archive-header
    check of fn_8008E3F8, fn_8008FA8C, fn_8008FC38), `(x & 0x8040) == 0x8040`. A variant: `li r0, -N; cmplw` reads
    the same in m2c's text."""
    def wide(m):
        n = int(m.group(4), 0)
        if not 0 < n <= 0x8000:
            return m.group(0)
        c = ((0x10000 - n) - (int(m.group(2), 16) << 16)) & 0xFFFFFFFF
        return '%s %s 0x%X' % (m.group(1), m.group(3), c)
    # (fn_8000E1E4: `(u32) (x + 0x10000) == -1U`, `addis r0, r3, 1; cmplwi r0, 0xFFFF`: x == 0xFFFFFFFF)
    body = re.sub(r'\(u32\) \(([^()]*(?:\([^()]*(?:\([^()]*\)[^()]*)*\)[^()]*)*) \+ 0x([0-9A-F]{1,4})0000\) ([!=]=) -(0x[0-9A-F]{1,4}|\d+)U\b',
                  wide, body)

    def low(m):
        n = int(m.group(2), 16)
        return m.group(0) if not 0 < n <= 0x8000 else '%s 0x%X' % (m.group(1), 0x10000 - n)
    return re.sub(r'([!=]=|<=|>=|<|>) -0x([0-9A-F]{1,4})U\b', low, body)


def field_pointer_tests(body):
    """step 109a, a variant: a word field m2c typed u32 / s32 only because a loop tests it,
    `while (M2C_FIELD(P, u32 *, N) != 0U)`, as the pointer it is, compared with a null pointer, `M2C_FIELD(P, void **,
    N) != ((void *) 0)`: `li r0, 0; lwzx r3; cmplw r3, r0` (fn_800A44D4's and fn_800BC768's null-terminated joint
    lists), where an integer test gives `cmplwi`. Only in a `while` test: the same functions' other tests are integer
    ones. pointer_tests does it for globals."""
    def fix(m):
        return re.sub(r'M2C_FIELD\(([^;]*?), (?:u32|s32) \*, (0x[0-9A-F]+|\d+)\) ([=!]=) 0U?\b',
                      lambda f: 'M2C_FIELD(%s, void **, %s) %s ((void *) 0)' % (f.group(1), f.group(2), f.group(3))
                      if f.group(1).count('(') == f.group(1).count(')') else f.group(0), m.group(0))
    return re.sub(r'^\s*while \(.*\) \{$', fix, body, flags=re.M)


def signed_temps(body):
    """step 109a, a variant: a local m2c typed u16 / u8 (`u16 temp = M2C_FIELD(p, u16 *, 0x14E);`) that every later
    use converts, `(s16) temp`: the source's local was the signed one, converted once at the load (`lhz r3; extsh
    r26, r3`, the signed value in the saved register; m2c's form keeps the halfword and extends at each use: fn_800A44D4,
    fn_800BC768). The local becomes s16 / s8, its assignment `(s16) LOAD`, its uses plain."""
    for t, s in (('u16', 's16'), ('u8', 's8')):
        for v in re.findall(r'^    %s (\w+);$' % t, body, re.M):
            uses = re.findall(r'\(%s\) %s\b|\b%s\b' % (s, re.escape(v), re.escape(v)), body)
            asg = re.findall(r'^(\s+)%s = ([^;]+);$' % re.escape(v), body, re.M)
            casts = [u for u in uses if u.startswith('(')]
            if len(asg) != 1 or not casts or len(uses) != len(casts) + 2:
                continue
            body = re.sub(r'^    %s %s;$' % (t, re.escape(v)), '    %s %s;' % (s, v), body, flags=re.M)
            body = re.sub(r'^(\s+)%s = ([^;]+);$' % re.escape(v), lambda m: '%s%s = (%s) %s;' % (m.group(1), v, s, m.group(2)), body, flags=re.M)
            body = re.sub(r'\(%s\) %s\b' % (s, re.escape(v)), v, body)
    # m2c's --reg-vars form of the same: the signed local loaded signed (`lha`) -> the unsigned field converted
    for t, s in (('u16', 's16'), ('u8', 's8')):
        for v in re.findall(r'^    %s (\w+);$' % s, body, re.M):
            body = re.sub(r'^(\s+%s = )M2C_FIELD\(([^;]*), %s \*, (\w+)\);$' % (re.escape(v), s),
                          lambda m: '%s(%s) M2C_FIELD(%s, %s *, %s);' % (m.group(1), s, m.group(2), t, m.group(3)), body, flags=re.M)
    return body


def search_loops(body):
    """step 109a, a variant: m2c's reading of a search loop that leaves its result in a variable (an inlined lookup:
    the character / object with an id, the part with a kind),
           loop_N:                                  for (; !A; I) {
           if (A) {                                     if (B) {
               R = X;                                       R = Y;
           } else if (B) {                  ->              goto found_N;
               R = Y;                                   }
           } else {                                 }
               I;                                   R = X;
               goto loop_N;                     found_N:
           }
    The compiler gives `for` its test at the bottom (`b test; body: ...; test: cmpwi; blt body; li r28, 0`), m2c's
    form tests at the top (fn_800AF398 140 -> 144 of 144). The increments I (`v += k;` only) go into the `for`; the
    label is used once."""
    lines = body.split('\n')
    k = 0
    n = 0
    while k < len(lines):
        m = re.fullmatch(r'(\s*)(loop_\d+):', lines[k])
        if not m or k + 9 > len(lines) or len(re.findall(r'\bgoto %s;' % m.group(2), body)) != 1:
            k += 1
            continue
        lab = m.group(2)
        ind = re.match(r'\s*', lines[k + 1]).group(0)
        a = re.fullmatch(re.escape(ind) + r'if \((.+)\) \{', lines[k + 1])
        x = re.fullmatch(re.escape(ind) + r'    (\w+) = ([^;]+);', lines[k + 2])
        b = re.fullmatch(re.escape(ind) + r'\} else if \((.+)\) \{', lines[k + 3])
        y = re.fullmatch(re.escape(ind) + r'    (\w+) = ([^;]+);', lines[k + 4])
        if not (a and x and b and y and x.group(1) == y.group(1) and lines[k + 5] == ind + '} else {'):
            k += 1
            continue
        j = k + 6
        incs = []
        while j < len(lines) and re.fullmatch(re.escape(ind) + r'    \w+ (?:\+=|-=) (?:0x[0-9A-Fa-f]+|\d+);', lines[j]):
            incs.append(lines[j].strip()[:-1])
            j += 1
        if not incs or j + 1 >= len(lines) or lines[j] != ind + '    goto %s;' % lab or lines[j + 1] != ind + '}':
            k += 1
            continue
        n += 1
        found = 'found_%s' % lab[5:]
        test = a.group(1)
        for op, neg in ((' >= ', ' < '), (' <= ', ' > '), (' > ', ' <= '), (' < ', ' >= ')):
            parts = _split_top(test, op)
            if len(parts) == 2 and not any(len(_split_top(test, o)) > 1 for o in (' && ', ' || ')):
                test = parts[0] + neg + parts[1]
                break
        else:
            test = _negate_test(test)
        new = [ind + 'for (; %s; %s) {' % (test, ', '.join(incs)),
               ind + '    if (%s) {' % b.group(1), ind + '        %s = %s;' % (y.group(1), y.group(2)),
               ind + '        goto %s;' % found, ind + '    }', ind + '}', ind + '%s = %s;' % (x.group(1), x.group(2)),
               found + ':']
        lines[k:j + 2] = new
        k += len(new)
    return '\n'.join(lines)


def irregular_switches(body):
    """step 109a, a variant: m2c's `switch (X) { /* irregular */ case A: ... break; ... }` (its reading of a chain of
    compares it could not take as a jump table) as the if / else-if chain the source wrote: main.dol reloads X for each
    test (`lbz r0, 0x4855(r3); cmplwi r0, 0; bne; ... lbz r0, 0x4855(r3); cmplwi r0, 1`), the switch compiles to one
    load and a compare tree (fn_80107310). Only switches whose cases all end in `break;` or `return;`, a `default:`
    last; X loses a cast to the field's own type."""
    lines = body.split('\n')
    k = 0
    while k < len(lines):
        m = re.fullmatch(r'(\s*)switch \((.+)\) \{\s*/\* irregular \*/', lines[k])
        if not m:
            k += 1
            continue
        ind, x = m.group(1), m.group(2)
        cm = re.fullmatch(r'\((u8|s8|u16|s16|s32|u32)\) (M2C_FIELD\(.*, (\w+) \*, \w+\))', x)
        if cm and cm.group(1) == cm.group(3):
            x = cm.group(2)
        j = k + 1
        arms, labels, cur, ok = [], [], None, True
        while j < len(lines) and lines[j] != ind + '}':
            l = lines[j]
            lm = re.fullmatch(re.escape(ind) + r'(case (-?(?:0x[0-9A-Fa-f]+|\d+)U?)|default):', l)
            if lm:
                if cur is not None:
                    ok = False   # a fall-through into the next label
                    break
                labels.append(lm.group(2) if lm.group(2) is not None else None)
            elif (l.startswith(ind + '    ') or not l.strip()) and labels:
                if cur is None:
                    cur = []
                cur.append(l)
                if l == ind + '    break;' or l == ind + '    return;':
                    arms.append((labels, cur))
                    labels, cur = [], None
            else:
                ok = False
                break
            j += 1
        if not ok or labels or cur is not None or j >= len(lines) or not arms or \
                any(None in ls and (len(ls) > 1 or i != len(arms) - 1) for i, (ls, _) in enumerate(arms)):
            k += 1
            continue
        new = []
        for i, (ls, stmts) in enumerate(arms):
            if stmts[-1].strip() == 'break;':
                stmts = stmts[:-1]
            if ls == [None]:
                head = '} else {'
            else:
                test = ' || '.join('(%s == %s)' % (x, v) for v in ls) if len(ls) > 1 else '%s == %s' % (x, ls[0])
                head = ('if (%s) {' if i == 0 else '} else if (%s) {') % test
            new.append(ind + head)
            new += stmts
        new.append(ind + '}')
        lines[k:j + 1] = new
        k += len(new)
    return '\n'.join(lines)


def struct_copy_macro(body):
    """step 109a, a variant: m2c's `M2C_STRUCT_COPY(D, S, N)` (its reading of a struct assignment's copy loop, `mtctr;
    lwz; lwzu; stw; stwu; bdnz`, with both pointers biased by the unit: `(s32) (P - 4) + 4`) as the assignment it is,
    `*(cr_CopyN_U *) (D) = *(cr_CopyN_U *) (S);` through a struct of N bytes of the unit U (4: words, 2: halfwords,
    1: bytes), declared before the function. A macro nothing defines is a call to an unknown function in the draft
    (cr_BattleRobo_InitBody's 0x78-byte body record)."""
    types = {}

    def fix(m):
        args = _split_top(m.group(1), ', ')
        if len(args) != 3 or not _NUM.fullmatch(args[2].strip()):
            return m.group(0)
        n = int(args[2].strip(), 0)
        unit = 4
        um = re.search(r'\) \+ ([124])$', args[0].strip())
        if um:
            unit = int(um.group(1))
        if n % unit:
            unit = 1
        t = 'cr_Copy%X_%d' % (n, unit)
        types[t] = (n, unit)

        def addr(a):
            # `(s32) (P - k) + k` is P itself (m2c's bias on a typed pointer would scale by the element)
            b = re.fullmatch(r'\(s32\) \((.*) - (\d+)\) \+ (\d+)', a.strip())
            return b.group(1) if b and b.group(2) == b.group(3) else a.strip()
        return '*(%s *) (%s) = *(%s *) (%s);' % (t, addr(args[0]), t, addr(args[1]))
    new = re.sub(r'\bM2C_STRUCT_COPY\((.*)\);', fix, body)
    if not types:
        return body
    m = re.search(r'^\w[^;\n]*\)\s*\{$', new, re.M)
    if not m:
        return body
    decl = ''.join('typedef struct %s {\n    %s w[0x%X];\n} %s;\n\n' % (t, {4: 'u32', 2: 'u16', 1: 'u8'}[u], n // u, t)
                   for t, (n, u) in sorted(types.items()))
    return new[:m.start()] + decl + new[m.start():]


def named_param_homes(body):
    """step 109a, a variant: param_homes for a parameter with a name of our prototypes (`u32 frame`, not m2c's argN):
    `sp8 = frame;` among the first statements, sp8 declared with the parameter's type and never assigned again, is the
    parameter's own stack home (level 0 keeps it there: `stw r4, 0x8(r1)`, then `lwz r3, 0x8(r1)` for its use;
    m2c's copy adds a second slot: cr_Motion_Play). sp8 is renamed to the parameter."""
    m = DEF.search(body)
    if not m:
        return body
    params = {}
    for p in m.group(3).split(','):
        pm = re.match(r'^\s*(.*?)\s*\b(\w+)\s*$', p)
        if pm and not re.fullmatch(r'f?arg\d+', pm.group(2)) and pm.group(1):
            params[pm.group(2)] = re.sub(r'\s+', '', pm.group(1))
    start = body.find('\n\n', m.end())
    for a, t in params.items():
        cm = re.search(r'^    (sp[0-9A-F]+) = %s;\n' % re.escape(a), body, re.M)
        if not cm or start < 0 or cm.start() < start or len(re.findall(r'\b%s\b' % re.escape(a), body)) != 2:
            continue
        if not re.fullmatch(r'(    \w+ = \w+;\n)*', body[start + 2:cm.start()]):
            continue
        v = cm.group(1)
        vd = re.search(r'^    ([^;=()\n]*?)\s*\b%s;\n' % v, body, re.M)
        if not vd or re.sub(r'\s+', '', vd.group(1)) != t or assigned(v, body[:cm.start()] + body[cm.end():]):
            continue
        body = body[:cm.start()] + body[cm.end():]
        vd = re.search(r'^    ([^;=()\n]*?)\s*\b%s;\n' % v, body, re.M)
        body = body[:vd.start()] + body[vd.end():]
        body = re.sub(r'\b%s\b' % v, a, body)
    return body


def byte_index_sums(body):
    """step 109a, a variant: record_array_index's form for a byte step, an access at `B + (I + O)` (`(*(T *)((s8 *)((B
    + (I + O))) + (0)))` or `M2C_FIELD((B + (I + O)), T *, 0)`) written `*(T *) &((u8 (*)[1]) (B))[I][O]`: the
    compiler adds O to the index and stores indexed (`addi r0, r30, 0x488a; stbx r3, r31, r0`, fn_80104F70), where
    the sum folds O into the displacement (`add; stb 0x488a(r3)`)."""
    num = r'(?:0x[0-9A-Fa-f]+|\d+)'
    body = re.sub(r'\(\*\((\w+ \*)\)\(\(s8 \*\)\(\((\w+) \+ \((\w+) \+ (%s)\)\)\) \+ \(0\)\)\)' % num,
                  r'(*(\1) &((u8 (*)[1]) (\2))[\3][\4])', body)
    return re.sub(r'M2C_FIELD\(\((\w+) \+ \((\w+) \+ (%s)\)\), (\w+ \*), 0\)' % num, r'(*(\4) &((u8 (*)[1]) (\1))[\2][\3])', body)


def for_break_tails(body):
    """step 109a, a variant: for_break_loops' search loop with the body before the test,
           loop_N:                                  for (; C; I) {
           if (C) {                                     S...
               S...                     ->              if (!T) {
               if (T) {                                     break;
                   I;                                   }
                   goto loop_N;                     }
               }
           }
    (fn_8011725C: the `for` jumps to its test at the bottom, 190 -> 198 of 198). Only with the label used once and S
    free of goto / break / continue / return."""
    lines = body.split('\n')
    k = 0
    while k < len(lines):
        m = re.fullmatch(r'\s*(loop_\d+):', lines[k])
        if not m or k + 6 > len(lines) or len(re.findall(r'\bgoto %s;' % m.group(1), body)) != 1:
            k += 1
            continue
        ind = re.match(r'\s*', lines[k + 1]).group(0)
        c = re.fullmatch(re.escape(ind) + r'if \((.+)\) \{', lines[k + 1])
        if not c:
            k += 1
            continue
        j = k + 2
        while j < len(lines) and (lines[j].startswith(ind + '    ') or not lines[j].strip()):
            j += 1
        # lines[k+2 .. j-1] the block; lines[j] its closing brace
        blk = lines[k + 2:j]
        if j >= len(lines) or lines[j] != ind + '}' or len(blk) < 4:
            k += 1
            continue
        if blk[-1] != ind + '    }' or blk[-2] != ind + '        goto %s;' % m.group(1):
            k += 1
            continue
        t = len(blk) - 3
        incs = []
        while t >= 0 and re.fullmatch(re.escape(ind) + r'        \w+ (?:\+=|-=) (?:0x[0-9A-Fa-f]+|\d+);', blk[t]):
            incs.insert(0, blk[t].strip()[:-1])
            t -= 1
        tm = re.fullmatch(re.escape(ind) + r'    if \((.+)\) \{', blk[t]) if t >= 0 else None
        s = blk[:t] if tm else None
        if not incs or not tm or not s or any(re.search(r'\b(?:goto|break|continue|return)\b', x) for x in s):
            k += 1
            continue
        test = tm.group(1)
        neg = None
        for op, n in ((' >= ', ' < '), (' <= ', ' > '), (' > ', ' <= '), (' < ', ' >= ')):
            parts = _split_top(test, op)
            if len(parts) == 2 and not any(len(_split_top(test, o)) > 1 for o in (' && ', ' || ')):
                neg = parts[0] + n + parts[1]
                break
        new = [ind + 'for (; %s; %s) {' % (c.group(1), ', '.join(incs))] + s + \
              [ind + '    if (%s) {' % (neg or _negate_test(test)), ind + '        break;', ind + '    }', ind + '}']
        lines[k:j + 1] = new
        k += len(new)
    return '\n'.join(lines)


def _wide_locals(body, consts):
    for t, w in (('s16', 's32'), ('s8', 's32'), ('u16', 'u32'), ('u8', 'u32')):
        for v in re.findall(r'^    %s (\w+);$' % t, body, re.M):
            asg = re.findall(r'\b%s\s*(?:[-+*/%%&|^]|<<|>>)?=(?!=)\s*([^;]*);' % re.escape(v), body)
            ok = r'-?(?:0x[0-9A-Fa-f]+|\d+)U?' if consts else r'M2C_FIELD\(.*, %s \*, \w+\)' % t
            if asg and all(re.fullmatch(ok, a) for a in asg):
                body = re.sub(r'^    %s %s;$' % (t, re.escape(v)), '    %s %s;' % (w, v), body, flags=re.M)
    return body


def wide_field_locals(body):
    """step 109a, a variant: a local m2c typed by the field it is loaded from (`s16 v; v = M2C_FIELD(p, s16 *, 2);`)
    as an int: main.dol shifts and masks the loaded halfword directly (`lha r30; srawi r28, r30, 5`), the s16 local
    is extended again at each use (`extsh r3, r30`: cr_C_ACT_CHK_BIT). Every local whose only assignments are loads
    of its own type becomes s32 (s8 / s16) or u32 (u8 / u16)."""
    return _wide_locals(body, False)


def wide_const_locals(body):
    """step 109a, a variant: wide_field_locals for a narrow local that is only set and stepped by constants (`s16 y;
    y = 0xC0; ... y += 0xF;`): an int in main.dol, converted without an `extsh` (`xoris r3, r29, 0x8000` for its
    (f32): fn_800A514C)."""
    return _wide_locals(body, True)


def field_void_null(body):
    """step 109a, a variant: void_null only for a field compared with NULL (`M2C_FIELD(p, T **, 4) == NULL` ->
    `== ((void *) 0)`: `li r0, 0; cmplw`), the globals keeping NULL (`cmplwi`): cr_C_ACT_CHK_BIT tests both."""
    out, i = [], 0
    for m in re.finditer(r' ([=!]=) NULL\b', body):
        j = m.start()
        if j == 0 or body[j - 1] != ')':
            continue
        depth, a = 0, j - 1
        while a >= 0:
            depth += body[a] == ')'
            depth -= body[a] == '('
            if depth == 0:
                break
            a -= 1
        if a >= 9 and body[a - 9:a] == 'M2C_FIELD':
            out.append(body[i:m.start()] + ' %s ((void *) 0)' % m.group(1))
            i = m.end()
    out.append(body[i:])
    return ''.join(out)


def empty_macro_blocks(body):
    """step 109a, a variant: an empty block of m2c (`if (C) {\n\n} else {`, its reading of a test that branches over
    nothing but the jump to the else part) as an empty debug macro, `{ (void) 0; }`: the compiler keeps `beq else;
    b end` for it, where an empty block gives one `bne end` (fn_800F6A44; step 108's hand drafts
    cr_SEQ_MODE_SELECTOR_FrameA and fn_8008C054 are the same macro)."""
    return re.sub(r'(?m)^(\s*)(.*\) \{)\n\n(\1\})', lambda m: '%s%s\n%s    (void) 0;\n%s' % (m.group(1), m.group(2), m.group(1), m.group(3)), body)


def mask_compounds(body):
    """step 109a, a variant: m2c's `L = (s32) (u16) L;` (a field truncated in place) as `L &= 0xFFFF;` (u8: 0xFF): the
    compound assignment computes the field's address once (`lwz r5, 0xc(r5); lwz r0, 0x110(r5); clrlwi; stw r0,
    0x110(r5)`), the plain one twice (fn_8002557C)."""
    return re.sub(r'(?m)^(\s+)(.+) = \((?:s32|u32)\) \((u8|u16)\) (.+);$',
                  lambda m: '%s%s &= %s;' % (m.group(1), m.group(2), '0xFF' if m.group(3) == 'u8' else '0xFFFF')
                  if m.group(2) == m.group(4) else m.group(0), body)


def macro_returns(body):
    """step 109a, a variant: an early return alone in its block, `if (C) { return; }`, with an empty debug macro
    before it, `{ (void) 0; return; }`: the compiler then keeps `beq next; b end` (the return's jump to the epilogue
    behind the test), which the peephole otherwise folds into one `bne end` (fn_8002557C; empty_macro_blocks is the
    same macro in an empty block)."""
    return re.sub(r'(?m)^(\s*)(if \(.*\) \{)\n\1    return;\n\1\}', lambda m: '%s%s\n%s    (void) 0;\n%s    return;\n%s}' % (
        m.group(1), m.group(2), m.group(1), m.group(1), m.group(1)), body)


def refold_shifts(body):
    """step 109a, a variant: `v = K; v = K >> N;` (m2c put the constant it knows into the shift) as the shift of the
    variable, `v = K; v >>= N;` (fn_8000E1E4: `li r31, 0x8000...; srawi r31, r31, 1`)."""
    return re.sub(r'(?m)^(\s+)(\w+) = (-?(?:0x[0-9A-Fa-f]+|\d+)U?);\n\1\2 = \3 >> (\d+|0x[0-9A-Fa-f]+);$',
                  r'\1\2 = \3;\n\1\2 >>= \4;', body)


def postprocess(body, skip=(), add=()):
    """the post-processing rules in order; skip: rules left out, add: OPTIONAL rules applied (one() VARIANTS)"""
    for f in (declare_stack_arrays, declare_unk_stack, drop_param_spills, drop_saved_reg_reads, type_void_stores,
              signed_fields, unsigned_fields, compound_assign, drop_same_casts, drop_same_assign_casts, drop_same_field_casts, while_loops,
              table_call_args, table_call_self, global_call_self, table_call_noargs, narrow_int_casts, minus_one_stores,
              unsigned_const_stores, redundant_returns, widen_masked_bytes, invert_early_returns, chain_assign,
              double_negation, assert_ternary, hsd_inlines, mixed_externs, byte_steps, same_type_copies, param_retypes, param_regs, param_homes, named_param_homes,
              param_copies, param_aliases, inline_volatile_temps, register_order, narrow_minus_one, pointer_tests, void_null,
              float_literals, assign_in_cond, alias_offsets, stack_struct, struct_copies, drop_same_member_casts, expand_ptr_compound,
              script_head_dead, script_counter_ptr, split_slot_structs, unsigned_not_mask, member_array_index, struct_byte_offset, indexed_offset,
              fold_temp_compound, store_literals, hoist_float_temps, widen_narrow_params, masked_index_field, byte_compare_casts, shift_mask_order,
              sink_const_global_stores, record_array_index, for_break_loops, clz_intrinsic, plain_byte_tests, bitwise_lvalues, void_self,
              record_pointers, local_same_casts, byte_loops, wide_return_locals, advance_copies, unsigned_immediates,
              field_pointer_tests, signed_temps, search_loops, irregular_switches, struct_copy_macro, byte_index_sums,
              for_break_tails, wide_field_locals, field_void_null, empty_macro_blocks,
              mask_compounds, macro_returns, wide_const_locals, refold_shifts, cmd_cnt_last):
        if f not in skip and (f not in OPTIONAL or f in add):
            body = f(body)
    return body


# rules only a variant applies, and the variants a near draft is also tried with (one(), step 64), in this order:
# (rules left out, optional rules applied)
OPTIONAL = (void_null, pointer_tests, drop_same_assign_casts, param_homes, float_literals, drop_same_field_casts, table_call_noargs, unsigned_fields, hsd_inlines, stack_struct, alias_offsets,
            redundant_returns, widen_masked_bytes, invert_early_returns, chain_assign, drop_same_member_casts, expand_ptr_compound, store_literals,
            hoist_float_temps, widen_narrow_params, masked_index_field, byte_compare_casts, shift_mask_order, sink_const_global_stores,
            record_array_index, for_break_loops, plain_byte_tests, bitwise_lvalues, void_self, param_retypes, record_pointers,
            local_same_casts, byte_loops, wide_return_locals, advance_copies, unsigned_immediates,
            field_pointer_tests, signed_temps, search_loops, irregular_switches, struct_copy_macro, named_param_homes,
            byte_index_sums, for_break_tails, wide_field_locals, field_void_null, empty_macro_blocks,
            mask_compounds, macro_returns, wide_const_locals, refold_shifts, cmd_cnt_last)
VARIANTS = (((register_order,), ()), ((param_copies,), ()), ((param_copies, register_order), ()), ((), (param_homes,)),
            ((), (void_null,)), ((register_order,), (void_null,)),
            # step 69
            ((inline_volatile_temps,), ()), ((param_aliases,), ()), ((), (float_literals,)), ((), (drop_same_field_casts,)),
            ((), (table_call_noargs,)), ((), (unsigned_fields,)), ((), (redundant_returns,)), ((), (widen_masked_bytes,)),
            ((), (invert_early_returns,)), ((), (chain_assign,)),
            # step 77
            ((), (hsd_inlines,)), ((), (stack_struct,)), ((register_order,), (stack_struct,)), ((), (alias_offsets,)),
            ((), (stack_struct, alias_offsets)), ((), (pointer_tests, void_null)), ((), (drop_same_assign_casts,)),
            # step 79: struct-typed globals (cr_g_Script, cr_g_Scenario, cr_g_GameMode, cr_g_Pads)
            ((), (drop_same_member_casts,)), ((), (expand_ptr_compound,)), ((), (drop_same_member_casts, expand_ptr_compound)),
            # step 80: the script-command head without the counter pointer (the rule is on by default)
            ((script_counter_ptr,), ()),
            # step 110: the head as the original has it (script_head_dead, on by default); the step 80 pointer form as a variant
            ((script_head_dead,), ()), ((), (cmd_cnt_last,)),
            # step 85: the stored float constants as literals, the others externs
            ((), (store_literals,)),
            # step 92: float temporaries before the stores
            ((), (hoist_float_temps,)),
            # step 93: narrow parameters declared int
            ((), (widen_narrow_params,)), ((register_order,), (widen_narrow_params,)),
            # step 95: a masked index into a field as byte arithmetic
            ((), (masked_index_field,)), ((register_order,), (masked_index_field,)),
            # step 96: a byte member compared through (s32) (u8)
            ((), (byte_compare_casts,)), ((register_order,), (byte_compare_casts,)),
            # step 96: the mask before the shift; constant stores to globals later
            ((), (shift_mask_order,)), ((), (sink_const_global_stores,)), ((), (shift_mask_order, sink_const_global_stores)),
            # step 108: a record array's field as an indexed element
            ((), (record_array_index,)), ((register_order,), (record_array_index,)),
            # step 108: m2c's search loops as for / break
            ((), (for_break_loops,)), ((), (for_break_loops, record_array_index)),
            # step 108: byte tests without the (u8) cast
            ((), (plain_byte_tests,)), ((), (plain_byte_tests, for_break_loops)),
            # step 108: M2C_BITWISE on a global as a reinterpretation
            ((), (bitwise_lvalues,)),
            # step 108: the function itself void again (r3_returns)
            ((), (void_self,)), ((register_order,), (void_self,)))
# step 109a: tried only for a draft that is near already (NEAR_MIN, the best so far when they come): they change the
# text of many drafts (loops, switches, returns), and the low ones do not come near 100% with them; the batch
# would otherwise take several times as long
NEAR_MIN = 70
VARIANTS_NEAR = (
            # step 109a: byte loops as `for` with `*p++`, same-type casts of locals, record pointers, retyped parameters
            ((), (byte_loops,)), ((), (local_same_casts,)), ((), (record_pointers,)), ((), (record_pointers, local_same_casts)),
            ((), (param_retypes,)), ((), (param_retypes, record_pointers)),
            # step 109a: a narrow function's returned local as an int
            ((), (wide_return_locals,)),
            # step 109a: a value read before the global it comes from is advanced
            ((), (advance_copies,)),
            # step 109a: cmplwi's immediates unsigned
            ((), (unsigned_immediates,)), ((), (unsigned_immediates, record_array_index)), ((), (unsigned_immediates, masked_index_field)),
            ((), (unsigned_immediates, record_array_index, masked_index_field)),
            # step 109a: a tested word field as a pointer; a halfword local signed at its load
            ((), (field_pointer_tests,)), ((), (signed_temps,)), ((), (signed_temps, field_pointer_tests)),
            # step 109a: a search loop with its result as `for` with a goto past the not-found value
            ((), (search_loops,)), ((register_order,), (search_loops,)), ((), (search_loops, record_array_index)),
            # step 109a: an irregular switch as the if / else-if chain
            ((), (irregular_switches,)), ((register_order,), (irregular_switches,)),
            # step 109a: m2c's struct-copy macro as a struct assignment
            ((), (struct_copy_macro,)), ((), (struct_copy_macro, masked_index_field)), ((), (struct_copy_macro, param_homes)),
            ((), (struct_copy_macro, masked_index_field, param_homes)),
            # step 109a: a named parameter's stack home
            ((), (named_param_homes,)), ((), (named_param_homes, float_literals)), ((), (named_param_homes, float_literals, stack_struct)),
            # step 109a: a byte-step index sum as an indexed element
            ((), (byte_index_sums,)), ((), (byte_index_sums, record_array_index)),
            # step 109a: a search loop with its body before the test
            ((), (for_break_tails,)), ((), (for_break_tails, for_break_loops)), ((), (for_break_tails, search_loops)),
            ((), (for_break_tails, for_break_loops, search_loops)),
            # step 109a: locals loaded from narrow fields as ints
            ((), (wide_field_locals,)), ((), (wide_field_locals, field_void_null)), ((), (field_void_null,)),
            # step 109a: m2c's empty blocks as an empty debug macro
            ((), (empty_macro_blocks,)), ((), (empty_macro_blocks, irregular_switches)),
            # step 109a: a field truncated in place as a compound mask; the debug macro before an early return
            ((), (mask_compounds,)), ((), (macro_returns,)), ((), (mask_compounds, macro_returns)),
            ((), (mask_compounds, macro_returns, stack_struct, float_literals)),
            # step 109a: narrow locals set by constants as ints
            ((), (wide_const_locals,)), ((), (wide_const_locals, float_literals)),
            # step 109a: a shift of a variable m2c folded with its constant
            ((), (refold_shifts,)), ((), (refold_shifts, unsigned_immediates, irregular_switches)))
VARIANTS = VARIANTS + VARIANTS_NEAR


VARIANT_MIN = 0   # the VARIANTS / REG_VARS drafts are tried for every compiled draft below 100% scoring at least this
REG_VARS = ','.join(['r%d' % i for i in range(14, 32)] + ['f%d' % i for i in range(14, 32)])


def regvar_cleanup(body):
    """m2c --reg-vars (REG_VARS) also turns the epilogue's restores it cannot read into statements
    (`var_f31 = M2C_ERROR(/* psq_l unimplemented */);`): they go (the compiler writes its own epilogue)"""
    return re.sub(r'^[ \t]*\w+ = M2C_ERROR\(/\* (?:psq_l|lfd|lwz|lmw) unimplemented \*/\);[ \t]*\n', '', body, flags=re.M)


# ---- the repair loop: one fix per compiler message, checked by compiling again -------------------------------------------
ERR = re.compile(r'^(#\s+(\d+): )(.*)\n#\s+Error:(\s*)\^', re.M)


def first_error(txt):
    """(line number, printed source segment, column in it, message) of the compiler's first error, or None"""
    m = ERR.search(txt)
    if not m:
        return None
    col = len('#   Error:') + len(m.group(4)) - len(m.group(1))
    msg = []
    for l in txt[m.end():].split('\n')[1:]:
        l = l.strip()
        if not l.startswith('#') or 'Too many errors' in l or l.strip('# ') == '':
            break
        msg.append(l.lstrip('# ').strip())
    return int(m.group(2)), m.group(3), col, ' '.join(msg)


def expr_start(s, end):
    """the start of the expression that ends at s[end] (going back over balanced brackets to the operator or bracket
    that begins it: an assignment, a comma, an opening bracket, `return`)"""
    depth = 0
    i = end - 1
    while i >= 0:
        ch = s[i]
        if ch in ')]':
            depth += 1
        elif ch in '([':
            if depth == 0:
                break
            depth -= 1
        elif depth == 0:
            if ch in ',;{}?:':
                break
            if ch == '=' and (i == 0 or s[i - 1] not in '=!<>') and s[i + 1] != '=':
                break
            if s[i - 5:i + 1] == 'return' and (i == 5 or not (s[i - 6].isalnum() or s[i - 6] == '_')):
                break
        i -= 1
    i += 1
    while i < end and s[i] == ' ':
        i += 1
    return i


def split_args(s, i):
    """the top-level arguments of the bracket that opens at s[i]: ([arg texts], index after the closing bracket)"""
    depth, args, start = 0, [], i + 1
    for j in range(i, len(s)):
        ch = s[j]
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
            if depth == 0:
                args.append(s[start:j])
                return args, j + 1
        elif ch == ',' and depth == 1:
            args.append(s[start:j])
            start = j + 1
    return None, None


def expand_fields(s):
    """M2C_FIELD / M2C_BITWISE written out the way the compiler expands them (cr_game.h / m2c_macros.h), so a message
    about a line that uses them can be placed in it (the compiler prints the expanded line)"""
    while True:
        m = re.search(r'\bM2C_(FIELD|BITWISE)\(', s)
        if not m:
            return s
        args, end = split_args(s, m.end() - 1)
        if args is None:
            return s
        if m.group(1) == 'FIELD' and len(args) == 3:
            rep = '(*(%s)((s8 *)(%s) + (%s)))' % (args[1].strip(), args[0].strip(), args[2].strip())
        elif m.group(1) == 'BITWISE' and len(args) == 2:
            rep = '((%s)(%s))' % (args[0].strip(), args[1].strip())
        else:
            return s
        s = s[:m.start()] + rep + s[end:]


def locate(line, seg, col):
    """the index in the draft's line of column col of the segment the compiler printed of it (a window of a long line,
    macros expanded, its own spacing): matched without spaces; None when it is not in the line exactly once"""
    ls = [i for i, ch in enumerate(line) if ch != ' ']
    lz = ''.join(line[i] for i in ls)
    sz = seg.replace(' ', '')
    if not sz:
        return None
    p = lz.find(sz)
    if p < 0 or lz.find(sz, p + 1) >= 0:
        return None
    k = p + len(seg[:col].replace(' ', ''))
    return ls[k] if k < len(ls) else len(line)


CANON = {'u8': 'unsigned char', 's8': 'signed char', 'u16': 'unsigned short', 's16': 'short', 'u32': 'unsigned long',
         's32': 'long', 'f32': 'float', 'f64': 'double', 'vu8': 'unsigned char', 'vu16': 'unsigned short', 'vu32': 'unsigned long',
         'vs32': 'long', 'size_t': 'unsigned long', 'BOOL': 'int', 'M2C_UNK': 'long', 'M2C_UNK8': 'unsigned char',
         'M2C_UNK16': 'unsigned short', 'M2C_UNK32': 'unsigned long'}
DRAFT = {'unsigned char': 'u8', 'signed char': 's8', 'unsigned short': 'u16', 'short': 's16', 'unsigned long': 'u32',
         'long': 's32', 'unsigned int': 'u32', 'int': 's32', 'float': 'f32', 'double': 'f64', 'char': 'char', 'void': 'void'}


def canon(t):
    """a type the way the compiler prints it, without spaces (`u8 *` and `unsigned char *` are the same; the compiler
    prints `struct HSD_JObj *` for the prototype's `HSD_JObj *`: the tag keyword goes, the HSD typedefs carry the tag's
    name)"""
    t = re.sub(r'\b(struct|union)\s+', '', t)
    t = re.sub(r'\b(\w+)\b', lambda m: CANON.get(m.group(1), m.group(1)), t)
    return re.sub(r'\s+', '', t)


def draft_type(t):
    """the draft's spelling of a type the compiler printed (`unsigned char *` -> `u8 *`)"""
    for k in sorted(DRAFT, key=len, reverse=True):
        if t == k or t.startswith(k + ' ') or t.startswith(k + '*'):
            return DRAFT[k] + t[len(k):]
    return t


def protos_map():
    """{name: ([canonical parameter types], variadic)} of every prototype of the context (ctx_pp.c, ctx_sdk.h,
    ctx_protos.h), read once per process"""
    if 'protos' in W:
        return W['protos']
    out = {}
    for f in ('ctx_pp.c', 'ctx_sdk.h', 'ctx_protos.h'):
        try:
            text = open(os.path.join(W['out'], f), encoding='utf-8').read()
        except OSError:
            continue
        for m in re.finditer(r'\b([A-Za-z_]\w*)\s*\(([^(){};]*(?:\([^(){}]*\)[^(){};]*)*)\)\s*;', text):
            name, params = m.group(1), m.group(2).strip()
            if name in out or name in KEYWORDS:
                continue
            ps = [p.strip() for p in split_args('(' + params + ')', 0)[0]] if params else []
            if ps == ['void']:
                ps = []
            variadic = bool(ps) and ps[-1] == '...'
            if variadic:
                ps = ps[:-1]
            cps = []
            for p in ps:
                if '(' in p:
                    p = re.sub(r'\(\s*\*\s*\w+\s*\)', '(*)', p)
                else:
                    w = re.findall(r'\b[A-Za-z_]\w*\b', p)
                    if len(w) >= 2 and w[-1] not in KEYWORDS:
                        p = re.sub(r'\b%s\s*$' % w[-1], '', p.rstrip()).rstrip()   # hmm: `u8 name[4]`? rare in prototypes
                cps.append(canon(p))
            out[name] = (cps, variadic)
    W['protos'] = out
    return out


def call_at(s, at):
    """(callee name, [(start, end)] of the top-level arguments, index of the opening bracket) of the call whose closing
    bracket is at or just before s[at] (the compiler's caret for an argument's conversion), or None"""
    i = at
    if i < len(s) and s[i] == ')':
        i += 1
    while i > 0 and s[i - 1] == ' ':
        i -= 1
    if i == 0 or s[i - 1] != ')':
        return None
    depth, j = 0, i - 1
    while j >= 0:
        depth += 1 if s[j] == ')' else -1 if s[j] == '(' else 0
        if depth == 0:
            break
        j -= 1
    if j < 0:
        return None
    k = j
    while k > 0 and s[k - 1] == ' ':
        k -= 1
    e = k
    while k > 0 and (s[k - 1].isalnum() or s[k - 1] == '_'):
        k -= 1
    name = s[k:e]
    if not name or name[0].isdigit() or name in KEYWORDS or name in ('if', 'while', 'return', 'sizeof'):
        return None
    args, _ = split_args(s, j)
    if args is None:
        return None
    spans, p = [], j + 1
    for a in args:
        spans.append((p, p + len(a)))
        p += len(a) + 1
    return name, spans, j


def wrap(ex):
    return ex if re.match(r'^[\w.]+$', ex) or (ex.startswith('(') and ex.endswith(')') and split_args(ex, 0)[1] == len(ex)) else '(' + ex + ')'


def cast_text(ex, t, deref=False):
    """`(t) ex`, or `*(t *) ex` for a struct the compiler wants by value out of a pointer"""
    return ('*(%s *) ' % t if deref else '(%s) ' % t) + wrap(ex)


def cast_span(s, a, b, t, deref=False):
    """the argument s[a:b] cast to t (its own spacing kept)"""
    arg = s[a:b]
    lead = len(arg) - len(arg.lstrip())
    return s[:a] + arg[:lead] + cast_text(arg.strip(), t, deref) + s[b:]


def has_cast(ex, t):
    m = re.match(r'^\s*\((.+?)\)\s', ex)
    return bool(m) and canon(m.group(1)) == canon(t)


def ptr_names(lines):
    """the locals and parameters a draft declares as pointers"""
    out = set()
    for l in lines:
        m = re.match(r'^\s*(?:const )?[\w ]+?\s*\*+\s*(\w+)(?:\[[^\]]*\])*;', l)
        if m:
            out.add(m.group(1))
        m = DEF.match(l)
        if m:
            out |= set(re.findall(r'\*\s*(\w+)\b', m.group(3)))
    return out


def unbracket(ex):
    """the expression without one pair of outer brackets"""
    ex = ex.strip()
    while ex.startswith('(') and split_args(ex, 0)[1] == len(ex):
        ex = ex[1:-1].strip()
    return ex


def repair(lines, e):
    """apply one fix for the compiler's first error to the draft's lines (1-based line numbers); the fix made, or None.
    The rules are mechanical and local to the line of the message (the rest of the draft is m2c's); each one is named
    in the report's note through the fix count."""
    ln, seg, col, msg = e
    if not 1 <= ln <= len(lines):
        return None
    s = lines[ln - 1]
    at = locate(s, seg, col)
    if at is None and 'M2C_' in s:
        s = lines[ln - 1] = expand_fields(s)
        at = locate(s, seg, col)
    m = re.match(r"illegal implicit conversion from 'int' to 'struct (\w+)'$", msg)
    if m:
        # step 80: `*((cr_Pad *) ((u8 *) p + off)) = 0;` (fn_80020204: a byte store at a variable offset into the typed
        # array, which m2c casts to the element type): the store as a byte at that address (the width is the asm's;
        # stb here)
        st = m.group(1)
        cm = re.search(r'\*\(\(\(%s \*\) (\(.*\))\)\) = ' % re.escape(st), s)
        if cm:
            lines[ln - 1] = s[:cm.start()] + '*((u8 *) %s) = ' % cm.group(1) + s[cm.end():]
            return 'byte store through a struct pointer'
    m = re.match(r"illegal implicit conversion from '(.*)' to '(.*)'$", msg)
    if m and at is not None:
        a_t, t = m.group(1), m.group(2)
        if t == 'void':
            # `*(void *)ADDR = value` / `*M2C_FIELD(x, void **, 8) = value`: a store through an untyped address; the
            # value's type is the store's
            nl = re.sub(r'\*\s*\(void \*\)', '*(%s *)' % a_t, s, count=1)
            if nl == s:
                nl = s.replace('void **', '%s **' % a_t, 1)
            if nl == s:
                return None
            lines[ln - 1] = nl
            return 'store through a %s *' % a_t
        deref = False
        if re.match(r'^(struct|union)\s*\*$', t):
            t = 'void *'          # an anonymous struct's pointer: void * converts to it
        elif re.match(r'^(struct|union) \w+$', t) and a_t.endswith('*'):
            deref = True          # a struct by value where m2c passes its address
        elif re.match(r'^(struct|union)\b', a_t) and not a_t.endswith('*') and not re.match(r'^(struct|union)\b', t) \
                and '(' not in t:
            # `x = *(desc + off)` with desc a struct pointer m2c stepped in bytes: the scalar at that byte offset
            st = expr_start(s, at)
            ex = s[st:at].rstrip()
            inner = unbracket(ex)
            if inner.startswith('*('):
                lines[ln - 1] = s[:st] + '*(%s *) ((u8 *) %s)' % (t, unbracket(inner[1:])) + s[st + len(ex):]
                return 'struct %s read as %s' % (a_t.split()[-1], t)
            return None
        elif (re.match(r'^(struct|union)\b', t) and not t.endswith('*')) or (re.match(r'^(struct|union)\b', a_t) and not a_t.endswith('*')):
            return None   # a struct by value: no cast makes that
        if '(' in t and '(*' not in t:
            return None
        # a call's argument: the message points at the call's end; the parameter of that type says which argument
        c = call_at(s, at)
        if c is not None:
            name, spans, _ = c
            protos = protos_map()
            if name in protos:
                params, variadic = protos[name]
                want = canon(t)
                for i in reversed(range(min(len(params), len(spans)))):
                    if canon(params[i]) == want and not has_cast(s[spans[i][0]:spans[i][1]], t):
                        lines[ln - 1] = cast_span(s, spans[i][0], spans[i][1], t, deref)
                        return 'cast argument %d of %s to %s' % (i, name, t)
                if variadic or any(canon(p) == want for p in params):
                    return None
        st = expr_start(s, at)
        # otherwise the last expression before the caret that is not cast to that type already
        for _ in range(12):
            if st >= at:
                return None
            if not s[st:at].startswith('(' + t + ')'):
                break
            k = st - 1
            while k >= 0 and s[k] == ' ':
                k -= 1
            if k < 0 or s[k] != ',':
                return None
            at, st = k, expr_start(s, k)
        ex = s[st:at].rstrip()
        lines[ln - 1] = s[:st] + cast_text(ex, t, deref) + s[st + len(ex):]
        return 'cast to ' + t
    m = re.match(r"illegal explicit conversion from '(.*)' to '(.*)'$", msg)
    if m and at is not None:
        a_t, t = m.group(1), m.group(2)
        st = expr_start(s, at)
        ex = s[st:at].rstrip()
        if not ex.startswith('('):
            return None
        cm = split_args(ex, 0)
        if cm[0] is None:
            return None
        ct, operand = ex[1:cm[1] - 1].strip(), ex[cm[1]:].strip()
        struct_src = re.match(r'^(struct|union)\b', a_t) and not a_t.endswith('*')
        struct_dst = re.match(r'^(struct|union)\b', t) and not t.endswith('*')
        if struct_src:
            inner = unbracket(operand)
            if inner.startswith('*') and not inner.startswith('*('):
                inner = '*(' + inner[1:].strip() + ')'
            if inner.startswith('*('):
                # `(u16) *(base + off)`: a struct by value where the code reads a scalar at that byte offset
                body = unbracket(inner[1:])
                new = '*(%s *) ((u8 *) %s)' % (ct, body)
            elif re.match(r'^\w+$', inner):
                # `(s32) sp48` of a stack struct: its address (a pointer target) or its first scalar
                new = '(%s) &%s' % (ct, inner) if t.endswith('*') else '*(%s *) &%s' % (ct, inner)
            elif re.match(r'^[\w.]+(\[[^\[\]]+\])+$', inner):
                # `(u32) cr_g_BattleRoboStates[i]`: an array element of struct type read as its first scalar
                new = '(%s) &%s' % (ct, inner) if t.endswith('*') else '*(%s *) &%s' % (ct, inner)
            else:
                return None
            lines[ln - 1] = s[:st] + new + s[st + len(ex):]
            return 'struct %s read as %s' % (a_t.split()[-1], ct)
        if struct_dst and a_t.endswith('*'):
            lines[ln - 1] = s[:st] + '*(%s *) %s' % (ct, wrap(operand)) + s[st + len(ex):]
            return 'struct %s by value from its pointer' % ct
        inner = unbracket(operand)
        if canon(a_t) == canon(t) + '*' and inner.startswith('&'):
            # step 64: `(f32) (&lbl_80388284[0x800])`: m2c's address of the element where the code loads it (lfs
            # f1, 0x2000(r3)): the element itself
            lines[ln - 1] = s[:st] + '(%s) %s' % (ct, inner[1:].strip()) + s[st + len(ex):]
            return 'element instead of its address'
        em = re.match(r'^([A-Za-z_]\w*)\[([^\[\]]+)\]$', inner)
        if em and canon(a_t) == 'float' and t.endswith('*'):
            # step 64: `(s8 *) lbl_80388284[0x80C]`: a word of an array dtk typed float that holds a pointer
            # (a struct of mixed fields): the same word read as a pointer
            lines[ln - 1] = s[:st] + '(%s) ((u32 *) %s)[%s]' % (ct, em.group(1), em.group(2)) + s[st + len(ex):]
            return 'float element read as a pointer'
        return None
    if msg == 'call of non-function':
        # `*(table + i * 4)(args)` / `*obj(args)` / `table[i](args)`: a call through a pointer m2c could not type; the
        # address is cast to a pointer to a pointer to an unprototyped function (the arguments pass as they are)
        for m in re.finditer(r'M2C_FIELD\(', s):
            # step 69: `M2C_FIELD(table + i * 4, u32 *, 0)(args)`, the width form of a word read from a byte array
            # (M2C_WIDTH) that is a function pointer: the call through the slot at that address
            e = close_paren(s, m.end() - 1)
            if e < 0 or s[e + 1:e + 2] != '(':
                continue
            args, _ = split_args(s, m.end() - 1)
            if args and len(args) == 3 and args[1].strip() == 'u32 *':
                base, off = args[0].strip(), args[2].strip()
                addr = unbracket(base) if off == '0' else '(u8 *) %s + %s' % (wrap(base), wrap(off))
                lines[ln - 1] = table_call_args(s[:m.start()] + '(*(M2C_UNK (**)()) (' + addr + '))' + s[e + 1:])
                return 'call through a function pointer'
        for m in re.finditer(r'\*\(', s):
            args, end = split_args(s, m.end() - 1)
            if args is not None and end < len(s) and s[end] == '(' and not s[m.start() - 1:m.start()].isalnum():
                lines[ln - 1] = table_call_args(s[:m.start()] + '(*(M2C_UNK (**)()) ' + s[m.end() - 1:end] + ')' + s[end:])
                return 'call through a function pointer'
        m = re.search(r'(?<![\w)\]])\*(\w+)\(', s)
        if m:
            lines[ln - 1] = s[:m.start()] + '(*(M2C_UNK (**)()) %s)(' % m.group(1) + s[m.end():]
            return 'call through a function pointer'
        for m in re.finditer(r'\]\(', s):
            depth, i = 0, m.start()
            while i >= 0:
                depth += 1 if s[i] == ']' else -1 if s[i] == '[' else 0
                if depth == 0:
                    break
                i -= 1
            while i > 0 and (s[i - 1].isalnum() or s[i - 1] in '_.)'):
                if s[i - 1] == ')':
                    d2 = 0
                    while i > 0:
                        i -= 1
                        d2 += 1 if s[i] == ')' else -1 if s[i] == '(' else 0
                        if d2 == 0:
                            break
                else:
                    i -= 1
            if i < m.start() and not s[i:i + 1].isdigit():
                lines[ln - 1] = s[:i] + '((M2C_UNK (*)()) ' + s[i:m.start() + 1] + ')' + s[m.start() + 1:]
                return 'call through a function table'
        return None
    if msg == 'pointer/array required' and at is not None:
        # `*(x + 8)` / `*x` / `**(x + 8)` / `x[i]` where m2c typed x as an integer: the load goes through a pointer to
        # the type of the cast in front of it (`(u16) *(...)`), else s32, with as many levels as there are stars
        end = at
        while end > 0 and s[end - 1] == ' ':
            end -= 1
        if end and s[end - 1] == ']':
            depth, i = 0, end - 1
            while i >= 0:
                depth += 1 if s[i] == ']' else -1 if s[i] == '[' else 0
                if depth == 0:
                    break
                i -= 1
            k = i
            if k > 0 and s[k - 1] == ')':
                depth = 0
                while k > 0:
                    k -= 1
                    depth += 1 if s[k] == ')' else -1 if s[k] == '(' else 0
                    if depth == 0:
                        break
            else:
                while k > 0 and (s[k - 1].isalnum() or s[k - 1] == '_'):
                    k -= 1
            if k >= i or (k > 0 and s[k - 1] in '.>'):
                return None
            if k > 0 and s[k - 1] == '*' and (k < 2 or s[k - 2] in ' (*!~-'):
                # `*x[2]`: the element is a pointer m2c typed as an integer (an array of pointers declared u32[]):
                # the element is cast to a pointer, the load through it keeps the cast in front (else s32)
                cm = re.search(r'\((u8|s8|u16|s16|u32|s32|f32|f64)\) \*$', s[:k])
                t = (cm.group(1) if cm else 's32') + ' *'
                lines[ln - 1] = s[:k] + '((' + t + ') ' + s[k:end] + ')' + s[end:]
                return 'array element read as a ' + t
            if re.match(r'^\(\((?:u8|s8|u16|s16|u32|s32|f32|f64) \*+\) ', s[k:i]):
                return None   # cast once already: the base is not the problem
            cm = re.search(r'\((u8|s8|u16|s16|u32|s32|f32|f64)\) $', s[:k])
            t = (cm.group(1) if cm else 's32') + ' *'
            lines[ln - 1] = s[:k] + '((' + t + ') ' + s[k:i] + ')' + s[i:]
            return 'index through a ' + t
        if end and s[end - 1] == ')':
            depth, i = 0, end - 1
            while i >= 0:
                depth += 1 if s[i] == ')' else -1 if s[i] == '(' else 0
                if depth == 0:
                    break
                i -= 1
            if s[:i].endswith('M2C_FIELD'):
                # `*M2C_FIELD(x, u32 *, 0)`: the field holds a pointer m2c typed as an integer; it becomes a pointer
                # to that type (`u32 **`: the load through it keeps m2c's width)
                fm = re.search(r', ((?:u8|s8|u16|s16|u32|s32|f32|f64) \*+), ([^,()]+)\)$', s[i:end])
                if fm:
                    a = i + fm.start(1)
                    lines[ln - 1] = s[:a] + fm.group(1) + '*' + s[a + len(fm.group(1)):]
                    return 'field read as a pointer to ' + fm.group(1)
        else:
            i = end
            while i > 0 and (s[i - 1].isalnum() or s[i - 1] == '_'):
                i -= 1
        j = i
        while j > 0 and s[j - 1] == ' ':
            j -= 1
        stars = 0
        while j > 0 and s[j - 1] == '*':
            j -= 1
            stars += 1
        if stars == 0 or (j > 0 and (s[j - 1].isalnum() or s[j - 1] == ')')):
            return None
        cm = re.search(r'\((u8|s8|u16|s16|u32|s32|f32|f64)\) $', s[:j])
        t = (cm.group(1) if cm else 's32') + ' ' + '*' * stars
        lines[ln - 1] = s[:j + stars] + '(' + t + ') ' + s[i:]
        return 'load through a ' + t
    if msg in ('illegal type', 'illegal operand'):
        fixed = s.replace('void **', 'u8 **')
        names = set(re.findall(r'\b([A-Za-z_]\w*)\b', s))
        changed = []
        if fixed != s:
            lines[ln - 1] = fixed
            changed.append('void **')
        for i, l in enumerate(lines):
            # (step 64: also `void **p` for `*p += x`, the relocation loops of cr_ModelEntry_LoadAndBuild & co.)
            dm = re.match(r'^(\s*)void (\*+)\s*(\w+)((?:\[\w*\])*);$', l)
            if dm and dm.group(3) in names:
                lines[i] = '%su8 %s%s%s;' % (dm.group(1), dm.group(2), dm.group(3), dm.group(4))
                changed.append(dm.group(3))
            dm = DEF.match(l)
            if dm:
                def param(pm):
                    return 'u8 *' + pm.group(1) if pm.group(1) in names else pm.group(0)
                nl = re.sub(r'void \*(\w+)\b', param, l)
                if nl != l:
                    lines[i] = nl
                    changed.append('param')
        if changed:
            return 'u8 * for ' + ' '.join(changed)
        s = lines[ln - 1]
        # an integer compared with `(void *) (address)`: the address as an integer
        if re.search(r'[=!<>]=?\s*\(void \*\) \(|\(void \*\) \([^;]*\)\s*[=!<>]=', s) and '(void *) (' in s:
            lines[ln - 1] = s.replace('(void *) (', '(u32) (', 1)
            return 'address as u32'
        # pointer + pointer / pointer as an index: the right one as an integer
        ptrs = ptr_names(lines)
        for m in re.finditer(r'([+\-]=?|\[)\s*(?!\(s32\))(\w+)\b(?![\w(])', s):
            if m.group(2) in ptrs:
                lines[ln - 1] = s[:m.start(2)] + '(s32) ' + s[m.start(2):]
                return 'pointer %s as an integer' % m.group(2)
        for m in re.finditer(r'([+\-]=?|\[)\s*M2C_FIELD\(', s):
            args, end = split_args(s, m.end() - 1)
            if args and args[1].strip().endswith('**'):
                lines[ln - 1] = s[:m.end() - 10] + '(s32) ' + s[m.end() - 10:]
                return 'pointer field as an integer'
        # `(u32) x == ptr` / `M2C_FIELD(p, u32 *, 4) == ptr` (an integer field): the pointer as an integer
        intish = '(u32)' in s or re.search(r'M2C_FIELD\([^;]*?,\s*[us](?:8|16|32) \*,', s)
        for m in re.finditer(r'(==|!=)\s*(\w+)\b(?!\s*[(\[.]|\s*->)|(?<![.>\w])\b(\w+)\s*(==|!=)', s):
            n = m.group(2) or m.group(3)
            if n in ptrs and intish:
                st = m.start(2) if m.group(2) else m.start(3)
                if s[max(0, st - 6):st] == '(u32) ':
                    continue
                lines[ln - 1] = s[:st] + '(u32) ' + s[st:]
                return 'pointer %s as an integer' % n
        # `(void *) M2C_FIELD(...) == x` with x an integer: the field as an integer (the same cmplw)
        for m in re.finditer(r'\(void \*\) (M2C_FIELD\()', s):
            args, end = split_args(s, m.end(1) - 1)
            rest = s[end:] if end else ''
            om = re.match(r'\s*(==|!=)\s*(\w+)\b', rest)
            if om and om.group(2) not in ptrs:
                lines[ln - 1] = s[:m.start()] + '(u32) ' + s[m.start(1):]
                return 'pointer field compared with an integer'
        # `M2C_FIELD(p, u8 **, n) == (u32) x`: the pointer field as an integer
        for m in re.finditer(r'(?<!\(u32\) )M2C_FIELD\(', s):
            args, end = split_args(s, m.end() - 1)
            if args and len(args) == 3 and args[1].strip().endswith('*') and args[1].strip().count('*') >= 2:
                if re.match(r'\s*(==|!=)\s*\((u32|s32)\)', s[end:]):
                    lines[ln - 1] = s[:m.start()] + '(u32) ' + s[m.start():]
                    return 'pointer field compared with an integer'
        # `(u8 *) lbl == arg` / `arg == (u8 *) lbl` with arg an integer: the address as an integer
        for m in re.finditer(r'\(\w+ \*\) (\w+)\s*(==|!=)\s*(\w+)\b(?!\s*[(\[])|(\w+)\s*(==|!=)\s*\(\w+ \*\) (\w+)\b', s):
            other = m.group(3) or m.group(4)
            if other and other not in ptrs and not other.isdigit():
                seg = m.group(0)
                lines[ln - 1] = s[:m.start()] + re.sub(r'\(\w+ \*\) ', '(u32) ', seg, 1) + s[m.end():]
                return 'address compared with an integer'
        return None
    m = re.match(r"type mismatch '(.*)' and '(.*)'$", msg)
    if m:
        a_t, b_t = m.group(1), m.group(2)
        da, db_ = draft_type(a_t), draft_type(b_t)
        if a_t.endswith('*') and b_t.endswith('*') and ' - &' in s:
            # step 80: `cr_g_Scenario.part_bits[0] - &cr_g_Scenario` (fn_800EB518): the byte offset of a member, as the
            # difference of two addresses of different types: both as byte pointers (the offset the original adds)
            new = re.sub(r'([\w.>\-]+(?:\[[^\]]*\])*) - (&\w+)', r'(u8 *) \1 - (u8 *) \2', s, 1)
            if new != s:
                lines[ln - 1] = new
                return 'pointer difference in bytes'
        if '(%s)' % da in s and '(' not in db_:
            lines[ln - 1] = s.replace('(%s)' % da, '(%s)' % db_, 1)
            return 'cast %s instead of %s' % (db_, da)
        if a_t.endswith('*') and '(' not in b_t and not b_t.endswith('*'):
            ptrs = ptr_names(lines)
            for cm in re.finditer(r'\b(\w+)\s*(>=|<=|==|!=|<|>)', s):
                if cm.group(1) in ptrs:
                    lines[ln - 1] = s[:cm.start(1)] + '(%s) ' % db_ + s[cm.start(1):]
                    return 'pointer %s as %s' % (cm.group(1), db_)
        return None
    m = re.match(r"function call '(\w+)\((.*)\)' does not match '\1\((.*)\)'$", msg)
    if m:
        # the call passes fewer / more arguments than the callee's prototype (a draft's guess, or a call the compiler
        # declared implicitly before): zeros for the missing ones, the extra ones dropped (it compiles; it will not match)
        name = m.group(1)
        have = split_args('(' + m.group(2) + ')', 0)[0] if m.group(2).strip() else []
        want = split_args('(' + m.group(3) + ')', 0)[0] if m.group(3).strip() else []
        want = [w.strip() for w in want if w.strip() not in ('void', '...')]
        cm = re.search(r'\b%s\s*\(' % re.escape(name), s)
        if not cm:
            return None
        args, end = split_args(s, cm.end() - 1)
        if args is None:
            return None
        if len(args) < len(want) and len(have) == len(args):
            extra = ['0.0f' if w in ('float', 'double') else '0' for w in want[len(args):]]
            if args == [''] and len(args) == 1:
                lines[ln - 1] = s[:cm.end()] + ', '.join(extra) + s[end - 1:]
            else:
                lines[ln - 1] = s[:end - 1] + ', ' + ', '.join(extra) + s[end - 1:]
            return 'call of %s padded to %d arguments' % (name, len(want))
        if len(args) > len(want) and len(have) == len(args):
            keep = args[:len(want)]
            lines[ln - 1] = s[:cm.end()] + ','.join(keep) + s[end - 1:]
            return 'call of %s cut to %d arguments' % (name, len(want))
        return None
    m = re.match(r"undefined identifier '(saved_reg_[rf]\d+)'$", msg)
    if m:
        for i, l in enumerate(lines):
            if DEF.match(l):
                lines[i + 1:i + 1] = ['    %s %s;' % ('f32' if '_f' in m.group(1) else 's32', m.group(1))]
                return 'declared ' + m.group(1)
    return None


def one(job):
    name, addr, size, orig_obj = job
    out = W['out']
    a = os.path.join(out, 'asm', name + '.s')
    c = os.path.join(out, 'c', name + '.c')
    o = os.path.join(out, 'obj', name + '.o')
    lg = os.path.join(out, 'log', name + '.txt')
    def m2c(extra=()):
        return run([sys.executable, '-I', os.path.join(out, 'm2c_run.py'), '-t', 'ppc-mwcc-c', '--context',
                    os.path.join(out, 'ctx_pp.c'), '--valid-syntax', '--deterministic-vars', '--stack-structs']
                   + list(extra) + ['-f', name, a], 120)
    rc, so, se = m2c()
    body = so.replace('\r\n', '\n')
    if rc != 0 or not re.search(r'^\S.*\b%s\s*\(' % re.escape(name), body, re.M) or 'Decompilation failure' in body:
        msg = (se.strip().splitlines() or body.strip().splitlines() or ['no output'])[-1][:90]
        if W.get('pass1'):
            return name, None, msg, 0
        open(lg, 'w', encoding='utf-8').write(so + '\n' + se)
        return name, 'm2c failed', msg, 0
    if W.get('pass1'):
        # the data objects m2c declares itself, and the functions it declares itself (`u8 fn_80099538(M2C_UNK); /*
        # extern */`: the return type this caller's code needs)
        return name, signature(name, body), (re.findall(r'^extern ([\w ]+ \**)(lbl_[0-9A-F]{8});', body, re.M),
                                             re.findall(EXTERN_FN, body) + [('CR_R3', f) for f in re.findall(R3_READ, body)],
                                             re.findall(EXTERN_PARAMS, body)), 0
    m2c_body = body
    orig = orig_elf(orig_obj).func(name)
    W['leaf'] = not re.search(r'\bbl\s', open(a, encoding='utf-8').read())

    def build(body, c, o):
        """compile one post-processed draft (repair loop) and compare: (percent, equal words, fixes) or (None, the
        compiler's text, fixes)"""
        # the context's prototype of the function itself (ctx_protos.h, from its first-pass draft) is renamed away
        # while the context is read: the draft defines the function with the signature of this pass
        lines = ('// m2c draft of %s (0x%08X, %d bytes): decompiled output, gitignored, never commit\n'
                 '#define %s %s__proto\n#include "ctx.h"\n#undef %s\n\n%s' % (name, addr, size, name, name, name, body)).split('\n')
        cmd = mwcc_cmd(W['ver']) + ['-c'] + W['flags'] + CTX_DEFS + ['-DCR_DRAFT_CC', '-i', out.replace(os.sep, '/')] + sum([['-i', i] for i in CTX_INC], []) + ['-o', o, c.replace('\\', '/')]
        fixes = []
        while True:
            open(c, 'w', encoding='utf-8', newline='\n').write('\n'.join(lines))
            rc, so, se = run(cmd, 120, cwd=D)
            txt = so + se
            if rc == 0 and os.path.exists(o):
                break
            e = first_error(txt)
            if e is None or len(fixes) >= 300:
                return None, txt, fixes
            before = '\n'.join(lines)
            f = repair(lines, e)
            if f is None or '\n'.join(lines) == before:
                return None, txt, fixes
            fixes.append(f)
        try:
            mine = Elf(open(o, 'rb').read()).func(name)
        except Exception:
            mine = None
        if mine is None:
            return None, 'no function symbol in the object', fixes
        if orig is None:
            return None, 'function not in the original object', fixes
        p, eq = compare(orig, mine)
        return p, eq, fixes

    body = postprocess(m2c_body)
    # HAND DRAFTS (step 108): tools/hand_drafts/<name>.c, a draft written by hand from the original's asm where no
    # rule of this file gives the source's form (a signed field, `x = x | 2`, an empty debug macro, ...): used instead
    # of m2c's text, compiled and compared like any draft (so batch 12 reports it and promote_drafts.py links it)
    hand = os.path.join(G, 'tools', 'hand_drafts', name + '.c')
    if os.path.exists(hand):
        body = open(hand, encoding='utf-8').read().replace('\r\n', '\n')
        p, eq, fixes = build(body, c, o)
        if p is None:
            open(lg, 'w', encoding='utf-8').write(eq)
            return name, 'compile failed', '[hand] ' + (compile_error(eq) if '\n' in eq or 'Error' in eq else eq), 0
        return name, '%d%%' % p if p < 100 else '100% MATCH', '[hand]', eq * 4
    p, eq, fixes = build(body, c, o)
    if p is None:
        open(lg, 'w', encoding='utf-8').write(eq)
        return name, 'compile failed', compile_error(eq) if '\n' in eq or 'Error' in eq else eq, 0
    # VARIANTS (step 64): the post-processing rules whose evidence is not in the draft itself (which register a local
    # gets, whether the original worked on the parameter or on a copy, ...) are tried both ways when the draft does not
    # match: the variant that scores higher is kept (the first one on a tie: the full rules).
    # REG_VARS (step 69): the same function drafted by m2c with one variable per callee-saved register (m2c's own
    # --reg-vars): at level 0 the compiler gives every C local a register of its own from r31 / f31 down and never
    # shares one, so two m2c temporaries in the same callee-saved register were one variable of the source
    # (fn_80017668: `temp_r31_23` and `temp_r31_76` are the one saved interrupt state; fn_8001D05C: the sum and its
    # square root are one f31). m2c's default output splits them, and folds a value into its single use even when the
    # original kept it in its register first (`f31 = x*x + ...; f31 = sqrt(f31)`). Tried with the same rules and
    # VARIANTS. Then (step 69) a second round from the best one so far: each single VARIANT toggle added to it (two
    # rules a draft needs together: fn_8005C628's widened byte and its table call without arguments).
    tag = ''
    if p < 100 and p >= VARIANT_MIN and not variants_skipped(fixes):
        sources = [('', m2c_body)]
        rc2, so2, _ = m2c(['--reg-vars', REG_VARS])
        rbody = regvar_cleanup(so2.replace('\r\n', '\n'))
        if rc2 == 0 and re.search(r'^\S.*\b%s\s*\(' % re.escape(name), rbody, re.M) and 'Decompilation failure' not in rbody:
            sources.append(('reg-vars', rbody))
        seen = {body}
        best = ('', (), ())
        k = 0

        def attempt(src, skip, add):
            nonlocal body, p, eq, fixes, best, k
            vb = postprocess(dict(sources)[src], skip, add)
            if vb in seen:
                return
            seen.add(vb)
            k += 1
            vc, vo = c[:-2] + '.v%d.c' % k, o[:-2] + '.v%d.o' % k
            vp, veq, vfixes = build(vb, vc, vo)
            if vp is not None and (vp, veq) > (p, eq):
                shutil.copyfile(vc, c)
                shutil.copyfile(vo, o)
                body, p, eq, fixes, best = vb, vp, veq, vfixes, (src, skip, add)
            for f in (vc, vo):
                if os.path.exists(f):
                    os.remove(f)
        for src, _ in sources:
            for skip, add in ((((), ()),) if src else ()) + VARIANTS:
                if p == 100:
                    break
                if p < NEAR_MIN and (skip, add) in VARIANTS_NEAR:
                    continue
                attempt(src, skip, add)
        if 80 <= p < 100 and best != ('', (), ()):
            src, bskip, badd = best
            for skip, add in VARIANTS:
                if p == 100:
                    break
                if len(skip) + len(add) == 1 and not (set(skip) & set(badd)) and not (set(add) & set(bskip)):
                    attempt(src, tuple(dict.fromkeys(bskip + skip)), tuple(dict.fromkeys(badd + add)))
        src, bskip, badd = best
        tag = ' '.join([src] + ['-' + f.__name__ for f in bskip] + ['+' + f.__name__ for f in badd]).strip()
    note = ' '.join(filter(None, ['M2C_ERROR' if 'M2C_ERROR' in body else '', ('%d fixes' % len(fixes)) if fixes else '',
                                  ('[%s]' % tag) if tag else '']))
    return name, '%d%%' % p if p < 100 else '100% MATCH', note, eq * 4


# ---- step 109b: more repair rules for the compile failures, and rules / VARIANTS for the 80-89% drafts ---------------
# One self-contained block (merged with other branches' rules by appending): the repair loop and postprocess are
# extended by wrapping them, VARIANTS / OPTIONAL by concatenation; nothing above is edited.
SCALAR_PTR = re.compile(r'\b(u8|s8|u16|s16|u32|s32|f32|f64) \*')


def _stmt_scalar(s, msg):
    """the scalar type a struct-typed access of this line most likely is: the float of the message, the width of
    another field the same statement reads or writes, a float local, else s32 (the width only decides a cast: these
    drafts compile, they are not matches)"""
    if "'float'" in msg or "'double'" in msg:
        return 'f32'
    m = SCALAR_PTR.search(s)
    if m:
        return m.group(1)
    if re.search(r'\b(?:temp|var)_f\d+|\(f32\)|\blbl_8045\w+', s):
        return 'f32'
    return 's32'


def _ctx_struct_members():
    """{typedef name: set of member names} of the structs the drafts' context defines (ctx_pp.c, the preprocessed
    context: cr_types.h, the HSD and SDK headers), read once per process"""
    if 'ctxmembers' not in W:
        out = {}
        try:
            text = open(os.path.join(W['out'], 'ctx_pp.c'), encoding='utf-8').read()
        except OSError:
            text = ''
        for m in re.finditer(r'^typedef struct \w* ?\{\n((?:[^{}\n]*\n)*?)\} (\w+);', text, re.M):
            out[m.group(2)] = set(re.findall(r'(\w+)(?:\[[^\]]*\])*;\s*$', m.group(1), re.M))
        W['ctxmembers'] = out
    return W['ctxmembers']


def repair_109b(lines, e):
    """step 109b: fixes for messages the rules of repair() leave (the 44 compile failures of step 108); each returns
    the fix made, or None"""
    ln, seg, col, msg = e
    if not 1 <= ln <= len(lines):
        return None
    s = lines[ln - 1]
    # 1. a struct-typed access used as a scalar: `*(((cr_BattleRobo *) ((u8 *) p + off))) |= 1;` (m2c typed the
    #    address by its base; the load is a word / float / half at that byte address)
    if msg in ('illegal operand', 'illegal type') or re.match(r"illegal (?:implicit|explicit) conversion from '.*' to '.*'$", msg):
        for m in re.finditer(r'\*\(\(\((\w+) \*\) ', s):
            if m.group(1) not in SCALARS and m.group(1) not in ('void', 'char'):
                t = _stmt_scalar(s[:m.start()] + s[m.end():], msg)
                lines[ln - 1] = s[:m.start()] + '*(((%s *) ' % t + s[m.end():]
                return 'struct %s access as %s' % (m.group(1), t)
    m = re.match(r"illegal implicit conversion from '(.*)' to '(struct|union) ?(\w*)'$", msg)
    if m:
        # 2. a scalar assigned to a local m2c declared as a struct (stack_struct's `Point3d sp14;` that m2c also reads
        #    as a float): the store through the local's address
        t = draft_type(m.group(1)) if m.group(1) in DRAFT else m.group(1)
    if m or msg == 'illegal operand':
        t = t if m else _stmt_scalar(s, msg)
        text = '\n'.join(lines)
        for dm in re.finditer(r'(?<![\w.&*>])(\w+) (=|\+=|-=) (?!=)', s):
            n = re.escape(dm.group(1))
            if re.search(r'^\s+(?!M2C_|BOOL\b)(?:cr_\w+|[A-Z]\w*|struct \w+) %s(?:\[\w*\])?;|^\s+\w+ %s\[\w*\];' % (n, n), text, re.M):
                lines[ln - 1] = s[:dm.start()] + '*(%s *) &%s %s ' % (t, dm.group(1), dm.group(2)) + s[dm.end():]
                return 'store to struct / array local %s as %s' % (dm.group(1), t)
    if m:
        # `*M2C_FIELD(p, cr_CustomizeState **, 0x5C) = (f32) x`, `*sp12C = (f32) x` (sp12C a cr_CustomizeState *):
        # the pointer to a scalar
        if m.group(3) and '(' not in t:
            ns = re.sub(r'\b%s \*\*' % re.escape(m.group(3)), '%s **' % t, s, count=1)
            if ns == s:
                ns = re.sub(r'^(\s*)\*(\w+) (=|\+=|-=) ', r'\1*(%s *) \2 \3 ' % t, s, count=1)
            if ns != s:
                lines[ln - 1] = ns
                return 'struct %s field as %s' % (m.group(3), t)
    m = re.match(r"'(\w+)' is not a struct/union/class member$", msg) or (msg == 'not a struct/union/class' and True)
    if m:
        # 2b. a member read through a slot typed for another struct (m2c's reused stack slot): the slot cast to the
        #     struct of cr_types.h that has that member
        structs = _ctx_struct_members()
        s = re.sub(r'\(\(__typeof__\([^;]*?\) \+ 0\)\) (\w+)\)', r'\1', s)   # a slot rule cast of the same slot
        # a cast this rule made that still fails (`((OSCalendarTime *) spC4)->min.z`): the next struct with that member
        for cm in re.finditer(r'\(\((\w+) \*\) (&?\w+)\)->(\w+)\b', s):
            owners = [k for k, v in structs.items() if cm.group(3) in v]
            if cm.group(1) in owners and owners.index(cm.group(1)) + 1 < len(owners) and m is True:
                nxt = owners[owners.index(cm.group(1)) + 1]
                lines[ln - 1] = s[:cm.start()] + '((%s *) %s)->%s' % (nxt, cm.group(2), cm.group(3)) + s[cm.end():]
                return 'slot %s read as a %s' % (cm.group(2), nxt)
        for fm in re.finditer(r'(?<![\w.>])(\w+)(\[[^\[\]]*\])?(->|\.)(\w+)\b', s):
            if m is not True and fm.group(4) != m.group(1):
                continue
            owners = [k for k, v in structs.items() if fm.group(4) in v]
            if owners:
                if fm.group(2):
                    rep = '((%s *) %s)%s%s%s' % (owners[0], fm.group(1), fm.group(2), fm.group(3), fm.group(4))
                else:
                    base = fm.group(1) if fm.group(3) == '->' else '&' + fm.group(1)
                    rep = '((%s *) %s)->%s' % (owners[0], base, fm.group(4))
                lines[ln - 1] = s[:fm.start()] + rep + s[fm.end():]
                return 'slot %s read as a %s' % (fm.group(1), owners[0])
    if msg.startswith('type mismatch') and ' - ' in s:
        # 6a. a difference of two pointer types (member addresses): both as byte pointers
        for dm in re.finditer(r' - ', s):
            a = expr_start(s, dm.start())
            depth, b = 0, dm.end()
            while b < len(s) and not (depth == 0 and s[b] in ',;)'):
                depth += s[b] in '(['
                depth -= s[b] in ')]'
                b += 1
            left, right = s[a:dm.start()].strip(), s[dm.end():b].strip()
            if re.match(r"type mismatch '[^']*\*' and '[^']*\*'$", msg) and left and right and not left.startswith('(u8 *)'):
                lines[ln - 1] = s[:a] + '((u8 *) %s - (u8 *) (%s))' % (left, right) + s[b:]
                return 'pointer difference in bytes'
    text = '\n'.join(lines)
    names = set(re.findall(r'\b([A-Za-z_]\w*)\b', s))
    # 3. a local m2c declared `void *` (with m2c's `/* compiler-managed */` comment, which repair()'s own rule does not
    #    read) or as a function pointer (`void (*temp_r31_48)();`, a slot of the script tables) that the line steps or
    #    reads through: a byte pointer
    if msg in ('illegal type', 'illegal operand') or msg.startswith('type mismatch'):
        changed = []
        for i, l in enumerate(lines):
            dm = re.match(r'^(\s*)(?:void \*+\s*(\w+)|void \(\*(\w+)\)\([^()]*\)|\w+ \(\*(\w+)\)\[\w*\]);(\s*/\*.*\*/)?\s*$', l)
            n = dm and (dm.group(2) or dm.group(3) or dm.group(4))
            if n and n in names:
                lines[i] = '%su8 *%s;' % (dm.group(1), n)
                changed.append(n)
        if changed:
            return 'u8 * for ' + ' '.join(changed)
    at = locate(s, seg, col)
    if at is None and 'M2C_' in s:
        s = lines[ln - 1] = expand_fields(s)
        at = locate(s, seg, col)
    decl = {}
    for l in lines:
        dm = re.match(r'^\s+((?:const |unsigned |signed |struct )*\w+)(\s*\**)\s*(\w+)((?:\[\w*\])*);', l)
        if dm:
            decl[dm.group(3)] = (dm.group(1), dm.group(2).strip(), dm.group(4))
    floats = {n for n, d in decl.items() if d[0] in ('f32', 'f64') and not d[1] and not d[2]}
    ptrs = ptr_names(lines)
    if msg == 'illegal operand':
        # 4. a float stack slot m2c reads as an integer (an index, a pointer step, a bit test): the slot as s32
        for n in sorted(floats & names):
            ns = re.sub(r'\[%s\]' % n, '[(s32) %s]' % n, s)
            ns = re.sub(r'(&\w+ [+-] )%s\b' % n, r'\1(s32) %s' % n, ns)
            if ns != s:
                lines[ln - 1] = ns
                return 'float slot %s as an index' % n
            ns = re.sub(r'\(&%s\)\[' % n, '((s32 *) &%s)[' % n, s)
            ns = re.sub(r'\*\(&%s ([+-])' % n, r'*((s32 *) &%s \1' % n, ns)
            ns = re.sub(r'(?<![\w&*.>()])%s (&|\||<<|>>) ' % n, r'*(s32 *) &%s \1 ' % n, ns)
            ns = re.sub(r'(?<![\w&*.>)])%s \* (?=0x|\d)' % n, r'(s32) %s * ' % n, ns)
            if ns == s and re.search(r'&\w+ \+ \(%s [+-] ' % n, s):
                ns = re.sub(r'(&\w+ \+ \()%s ([+-] )' % n, r'\1(s32) %s \2' % n, s)
            if ns != s:
                lines[ln - 1] = ns
                return 'float slot %s as an integer' % n
        # 5. a pointer in a product / shift / mask (m2c's `sp104 * 0x94` of a reused slot): the pointer as an integer
        for m in re.finditer(r'(?<![\w.>*&])(\w+) (\*|<<|>>|&|\||/) |~(\w+)\b|(?:&|\||\^|\*|/) (\w+)\b(?!\s*[(\[.]|\s*->)', s):
            n = m.group(1) or m.group(3) or m.group(4)
            if n in ptrs:
                k = m.start(1) if m.group(1) else m.start(3) if m.group(3) else m.start(4)
                lines[ln - 1] = s[:k] + '(s32) ' + s[k:]
                return 'pointer %s as an integer' % n
        # 5b. a struct pointer slot indexed as bytes (`sp78[3] + 1`, sp78 a cr_BattleLoadout * m2c also stores
        #     lbl_8044ECA8 into): the slot as a byte pointer
        # 5e. a pointer as the first term of a bracketed byte offset (`(u8 *) sp24 + (sp104 + 0x19)`)
        for m in re.finditer(r'[+-] \((\w+) [+-] ', s):
            if m.group(1) in ptrs:
                lines[ln - 1] = s[:m.start(1)] + '(s32) ' + s[m.start(1):]
                return 'pointer %s as an integer' % m.group(1)
        # 5d. a struct pointer dereferenced as a scalar (`*sp20 = (s32) (*sp20 | 0x40000)`, sp20 a cr_CustomizeState *)
        sp = [n for n in re.findall(r'(?<![\w)\]])\*(\w+)\b(?!\s*(?:[\[.(]|->))', s)
              if n in decl and decl[n][1] == '*' and not decl[n][2] and decl[n][0] not in SCALARS + ('void', 'char')]
        if sp:
            t = _stmt_scalar(s, msg)
            lines[ln - 1] = re.sub(r'(?<![\w)\]])\*%s\b(?!\s*(?:[\[.(]|->))' % sp[0], '*(%s *) %s' % (t, sp[0]), s)
            return 'struct pointer %s read as %s' % (sp[0], t)
        # 5c. a struct / array local read as a scalar (`temp_f0_329 = sp14 + x`, sp14 a Point3d): its first element
        for m in re.finditer(r'(?<![\w.>&*])(\w+)\b(?!\s*(?:[\[.(]|->|=[^=]|\+=|-=))', s):
            d = decl.get(m.group(1))
            if d and not d[1] and (d[2] or re.match(r'(?!M2C_|BOOL\b)(?:cr_\w+|[A-Z]\w*|struct \w+)$', d[0])):
                t = _stmt_scalar(s, msg)
                lines[ln - 1] = s[:m.start()] + '*(%s *) &%s' % (t, m.group(1)) + s[m.end():]
                return 'struct / array local %s read as %s' % (m.group(1), t)
    if msg == 'illegal operand' or re.match(r"illegal implicit conversion from '.*' to 'struct \w+'$", msg):
        for m in re.finditer(r'(?<![\w.>])(\w+)\[|\*\((\w+) [+-] ', s):
            n = m.group(1) or m.group(2)
            d = decl.get(n)
            if d and d[1] == '*' and not d[2] and d[0] not in SCALARS and d[0] != 'char':
                k = m.start(1) if m.group(1) else m.start(2)
                lines[ln - 1] = s[:k] + '((u8 *) %s)' % n + s[k + len(n):]
                return 'struct pointer %s indexed in bytes' % n
        for m in re.finditer(r'\*\((\w+) \+ (\w+)\)', s):
            d = decl.get(m.group(2))
            if d and d[1] == '*' and m.group(1) not in ptrs:
                lines[ln - 1] = s[:m.start()] + '*((u8 *) %s + (s32) %s)' % (m.group(1), m.group(2)) + s[m.end():]
                return 'integer plus pointer %s in bytes' % m.group(2)
    if (msg == 'illegal operand' or msg.startswith('type mismatch')) and at is not None:
        # 6. a comparison of a pointer with an integer / a function / another pointer type: both sides as u32
        st = expr_start(s, at)
        ex = s[st:at]
        for op in (' == ', ' != ', ' <= ', ' >= ', ' < ', ' > '):
            parts = _split_top(ex, op)
            if len(parts) == 2 and not ex.startswith('(u32) ('):
                lines[ln - 1] = s[:st] + '(u32) (%s)%s(u32) (%s)' % (parts[0].strip(), op, parts[1].strip()) + s[at:]
                return 'comparison as u32'
    if msg == 'illegal type':
        # 7. a constant address cast to a function pointer and stepped: `(void (*)(void *))0x80390000 + 0x2A44`
        ns = re.sub(r'\([\w ]+\s*\(\*\)\([^()]*\)\)\s*(0x[0-9A-Fa-f]+) \+ ', r'(u8 *) \1 + ', s)
        if ns != s:
            lines[ln - 1] = ns
            return 'function address stepped in bytes'
    m = re.match(r"illegal explicit conversion from '(.*)' to '(.*)'$", msg)
    if m and at is not None:
        a_t, t = m.group(1), m.group(2)
        st = expr_start(s, at)
        ex = s[st:at]
        cm = re.match(r'(\(+)(f32|f64|float|double|u8|s8|u16|s16|u32|s32)\)\s*', ex)
        if cm and a_t.endswith('*') and t in ('float', 'double'):
            # 8. a float cast of a pointer (m2c's f1 after strcpy / strcat, a pointer slot read as a float): through
            #    an integer
            lines[ln - 1] = s[:st + cm.end()] + '(s32) ' + s[st + cm.end():]
            return 'pointer to float through s32'
        cm = re.match(r'(\(+)([\w ]+\*+)\)\s*', ex)
        if cm and a_t in ('float', 'double') and t.endswith('*'):
            # a float slot read as an address (`(s8*) (spF0)`): through an integer
            lines[ln - 1] = s[:st + cm.end()] + '(s32) ' + s[st + cm.end():]
            return 'float to pointer through s32'
        if re.match(r'^(struct|union)\b', a_t) and not a_t.endswith('*'):
            # 9. a struct local cast to a scalar (`(u8) sp8` of a GXColor): its first bytes
            om = re.search(r'\((u8|s8|u16|s16|u32|s32|f32)\) (\w+)\s*$', ex)
            if om:
                lines[ln - 1] = s[:st + om.start()] + '*(%s *) &%s' % (om.group(1), om.group(2)) + s[st + om.end():]
                return 'struct %s read as %s' % (om.group(2), om.group(1))
            om = re.search(r'&\(u8 \*\) ([\w>.\-]+)', ex)
            if om:
                lines[ln - 1] = s[:st + om.start()] + '(u8 *) &' + om.group(1) + s[st + om.end():]
                return 'address of struct member in bytes'
    m = re.match(r"illegal implicit conversion from '(.*)' to '(.*)'$", msg)
    if m and m.group(1) == m.group(2) and '(...)' in m.group(1):
        # `(void (*)(...)) x` stored to a `void (*)()` member: the prototype-less pointer type
        ns = s.replace('(void (*)(...)) ', '(void (*)()) ')
        if ns != s:
            lines[ln - 1] = ns
            return 'function pointer without parameters'
    if m and at is not None and not (re.match(r'^(struct|union)\b', m.group(2)) and not m.group(2).endswith('*')):
        t = draft_type(m.group(2)) if m.group(2) in DRAFT else m.group(2)
        # 10. `x = call(...)` / `x = array;` whose value the target's type does not take: the value cast
        am = re.match(r'^(\s*[^=;]*?[\w\])] = )(.*);$', s)
        if am and '(' not in t.replace('(*)', ''):
            rhs = am.group(2)
            if not rhs.startswith('(%s)' % t):
                lines[ln - 1] = am.group(1) + '(%s) (%s);' % (t, rhs)
                return 'value cast to ' + t
    if msg == 'pointer/array required' and at is not None:
        # 11. `(u8 *)0x801C0000[0x5BD0]`: the cast bracketed
        ns = re.sub(r'&\((\w+ \*)\)(0x[0-9A-Fa-f]+)\[', r'&((\1) \2)[', s)
        if ns != s:
            lines[ln - 1] = ns
            return 'constant address bracketed'
        # 12. an integer slot used as a pointer (`sp90->unk_00`, `sp3C[1]`; m2c reuses one stack slot for a pointer
        #     it stored there as `(long) (&cr_g_BodyData[i])`): the slot cast to the type of what was stored
        for m in re.finditer(r'(?<![\w.>])(\w+)(->|\[)', s):
            n = m.group(1)
            if n in decl and not decl[n][1] and not decl[n][2] and m.start() < at + 2:
                src = None
                for am in re.finditer(r'^\s+%s = \((?:long|s32|u32|unsigned long)\) (.+);$' % n, text, re.M):
                    x = unbracket(am.group(1))
                    if x.startswith('&') or x in ptrs or re.fullmatch(r'(?:lbl_[0-9A-F]{8}|cr_g_\w+)', x):
                        src = x
                        break
                if src is None and m.group(2) == '->':
                    # the same member read through a typed expression elsewhere in the draft
                    fm = re.search(r'([\w]+(?:\[[^\[\]]*\])?)(\.|->)%s\b' % re.escape(s[m.end():].split('-')[0].split('[')[0].split(')')[0].split(' ')[0]), text)
                    if fm and (fm.group(1).split('[')[0] in ptrs or re.match(r'(?:lbl_[0-9A-F]{8}|cr_g_\w+)', fm.group(1))):
                        src = ('&' + fm.group(1)) if fm.group(2) == '.' else fm.group(1)
                cast = '__typeof__((%s) + 0)' % src if src else ('u8 *' if m.group(2) == '[' else None)
                if cast:
                    lines[ln - 1] = s[:m.start()] + '((%s) %s)' % (cast, n) + s[m.end(1):]
                    return 'slot %s as a pointer' % n
    if msg == 'illegal use of keyword' and re.match(r'^\s*(case [^:]+|default):\s*$', s):
        # 13. a `case` / `default` label m2c placed outside its switch (an irregular switch): dropped (the draft
        #     compiles; the jump into it is lost: it does not match)
        lines[ln - 1] = re.sub(r'(case [^:]+|default):', r'/* \1: */', s)
        return 'stray case label dropped'
    return None


_repair_108 = repair


def variants_skipped(fixes):
    """step 109b: a draft that compiles only through more than 3 fixes, some of them repair_109b's (the former compile
    failures: 2-13 KB, up to 160 fixes each; with them a full run took 80 minutes) is not tried with the VARIANTS: each variant repeats the whole repair loop (hours for the batch) and
    none of them is near a match"""
    return len(fixes) > 3 and any(f.startswith('[109b] ') for f in fixes)


def repair(lines, e):
    """repair() of step 108, then repair_109b (whose fixes are marked `[109b] `); for two messages repair_109b's slot /
    member-difference rules first (repair()'s `load through a T` reads `slot->member[i]` as a deref; its step 80 rule
    splits `a[i].m - &a[i].n` inside a member)"""
    if e and (e[3] == 'pointer/array required' or e[3].startswith('type mismatch')):
        keep = list(lines)
        f = repair_109b(lines, e)
        if f is not None and f.startswith(('slot ', 'constant address', 'pointer difference')):
            return '[109b] ' + f
        lines[:] = keep
    f = _repair_108(lines, e)
    if f is None:
        f = repair_109b(lines, e)
        return None if f is None else '[109b] ' + f
    return f


def _local_decls(body):
    """{name: (type, stars, dims)} of the draft's locals and parameters"""
    out = {}
    for m in re.finditer(r'^\s+((?:const |unsigned |signed |struct )*\w+)\s*(\**)\s*(\w+)((?:\[\w*\])*);', body, re.M):
        out[m.group(3)] = (m.group(1), m.group(2), m.group(4))
    dm = DEF.search(body)
    if dm:
        for p in dm.group(3).split(','):
            pm = re.match(r'\s*((?:const |unsigned |signed |struct )*\w+)\s*(\**)\s*(\w+)\s*$', p)
            if pm:
                out[pm.group(3)] = (pm.group(1), pm.group(2), '')
    return out


def stack_param_alias(body):
    """step 109b, a variant: m2c's `sp8 = arg0;` (the parameter's home slot at level 0, which m2c reads as a stack
    local) written as the parameter itself: with the local, the compiler adds a saved register for it (fn_8006E690,
    fn_8006E778: `stw r3, 0x8(r1)` and the reload in the loop come from the parameter alone)"""
    dm = DEF.search(body)
    params = set(re.findall(r'(\w+)\s*(?:,|$)', dm.group(3))) if dm else set()
    for m in list(re.finditer(r'^[ \t]+((?:sp\.)?sp\w+) = (\w+);[ \t]*\n', body, re.M)):
        sp, arg = m.group(1), m.group(2)
        esp = re.escape(sp)
        if arg not in params:
            continue
        if len(re.findall(r'(?<![\w.])%s\s*(?:[-+*/|&^]?=(?!=)|\+\+|--)' % esp, body)) != 1 or re.search(r'&%s\b' % esp, body) \
                or re.search(r'\b%s\s*(?:[-+*/|&^]?=(?!=)|\+\+|--)' % arg, body) or body.find(m.group(0)) < 0:
            continue
        body = body.replace(m.group(0), '', 1)
        if sp.startswith('sp.'):
            # stack_struct's `struct { s32 sp8; } sp;`: the member goes, the struct with it when it was the only one
            body = re.sub(r'^[ \t]+struct \{\n[ \t]+[\w ]+?\**\s*\b%s;\n[ \t]+\} sp;\n' % sp[3:], '', body, count=1, flags=re.M)
            body = re.sub(r'^([ \t]+struct \{\n(?:[^}\n]*\n)*?)[ \t]+[\w ]+?\**\s*\b%s;\n' % sp[3:], r'\1', body, count=1, flags=re.M)
        else:
            body = re.sub(r'^[ \t]+[\w ]+?\**\s*\b%s;[^\n]*\n' % sp, '', body, count=1, flags=re.M)
        body = re.sub(r'(?<![\w.])%s\b' % esp, arg, body)
    return body


def drop_local_deref_casts(body):
    """step 109b, a variant: `*arg1 = (u8) (*arg0);` where arg0 is a `u8 *` (a local's or parameter's own element
    type): the value as it is loaded. drop_same_assign_casts does this for globals; the cast of a local's element costs
    a register of its own (fn_8008E598: `lbz r6` where the original stores r0)"""
    decls = _local_decls(body)

    def fix(m):
        t, x = m.group(2), m.group(3)
        d = decls.get(x)
        if d and draft_type(d[0]) == t and d[1] == '*' and not d[2]:
            return '%s*%s;' % (m.group(1), x)
        return m.group(0)
    body = re.sub(r'(= )\((%s)\) \(?\*(\w+)\)?;' % '|'.join(SCALARS), fix, body)

    def fix2(m):
        t, x, idx = m.group(2), m.group(3), m.group(4)
        d = decls.get(x)
        if d and draft_type(d[0]) == t and ((d[1] == '*' and not d[2]) or (not d[1] and d[2])):
            return '%s%s%s;' % (m.group(1), x, idx)
        return m.group(0)
    return re.sub(r'(= )\((%s)\) (\w+)(\[[^\[\]=;]*\]);' % '|'.join(SCALARS), fix2, body)


def goto_while_loops(body):
    """step 109b, a variant: m2c's reading of a loop whose test it could not put on top,
           loop_N:                      while (C) {
               if (C) {         ->          S...
                   S...                 }
                   goto loop_N;
               }
    (the label used once; S without break / continue, which would change meaning). The compiler gives `while` its test
    at the bottom (`b test`), m2c's `if` tests at the top (fn_800EA33C's cntlzw loop over a bit mask)"""
    lines = body.split('\n')
    k = 0
    while k < len(lines):
        m = re.fullmatch(r'(loop_\d+):', lines[k].strip())
        if not m or len(re.findall(r'\b%s\b' % m.group(1), body)) != 2 or k + 1 >= len(lines):
            k += 1
            continue
        ind = re.match(r'\s*', lines[k + 1]).group(0)
        c1 = re.fullmatch(re.escape(ind) + r'if \((.+)\) \{', lines[k + 1])
        if not c1:
            k += 1
            continue
        j = k + 2
        inner = []
        while j < len(lines) and (lines[j].startswith(ind + '    ') or not lines[j].strip()):
            inner.append(lines[j])
            j += 1
        if (not inner or inner[-1].strip() != 'goto %s;' % m.group(1) or j >= len(lines) or lines[j] != ind + '}'
                or any(re.match(r'\s*(?:break|continue)\b', x) and len(re.match(r'\s*', x).group(0)) == len(ind) + 4
                       for x in inner)):
            k += 1
            continue
        new = [ind + 'while (%s) {' % c1.group(1)] + inner[:-1] + [ind + '}']
        lines[k:j + 1] = new
        body = '\n'.join(lines)
        k += len(new)
    return '\n'.join(lines)


def goto_search_loops(body):
    """step 109b, a variant: m2c's reading of a search loop that stops at the first hit,
           loop_N:                              for (; C; I) {
               if (C) {                             if (T) {
                   if (T) {             ->              S...
                       S...                             break;
                   } else {                         }
                       I;                       }
                       goto loop_N;
                   }
               }
    (the label used once, S without break / continue / goto). The compiler's `for` tests at the bottom; with
    goto_while_loops for the loop around it (fn_80078B60: two searches in a loop over four records)"""
    lines = body.split('\n')
    k = 0
    while k + 2 < len(lines):
        m = re.fullmatch(r'(loop_\d+):', lines[k].strip())
        if not m or len(re.findall(r'\b%s\b' % m.group(1), '\n'.join(lines))) != 2:
            k += 1
            continue
        ind = re.match(r'\s*', lines[k + 1]).group(0)
        c1 = re.fullmatch(re.escape(ind) + r'if \((.+)\) \{', lines[k + 1])
        # simple statements before the test (`x = p->next_value;`, fn_8006E6E8's list walk)
        p = k + 2
        pre = []
        while p < len(lines) and re.fullmatch(re.escape(ind) + r'    (?!if\b|for\b|while\b|switch\b|goto\b|return\b)[^{}]*;', lines[p]):
            pre.append(lines[p])
            p += 1
        c2 = re.fullmatch(re.escape(ind) + r'    if \((.+)\) \{', lines[p]) if p < len(lines) else None
        if not (c1 and c2):
            k += 1
            continue
        j = p + 1
        s_body = []
        while j < len(lines) and lines[j].startswith(ind + '        '):
            s_body.append(lines[j])
            j += 1
        tail = lines[j:j + 6]
        inc = tail[1].strip() if len(tail) > 1 else ''
        if (len(tail) < 6 or tail[0] != ind + '    } else {' or tail[2] != ind + '        goto %s;' % m.group(1)
                or tail[3] != ind + '    }' or tail[4] != ind + '}'
                or not re.fullmatch(r'\w+ (?:\+=|-=) (?:0x[0-9A-Fa-f]+|\d+);|\w+(?:\+\+|--);|\w+ = [^;]+;', inc)
                or not tail[1].startswith(ind + '        ')
                or any(re.match(r'\s*(?:goto|break|continue|loop_\d+:)\b', x) for x in s_body)):
            k += 1
            continue
        new = [ind + 'for (; %s; %s) {' % (c1.group(1), inc[:-1])] + pre + [ind + '    if (%s) {' % c2.group(1)]
        new += s_body
        new += [ind + '        break;', ind + '    }', ind + '}']
        lines[k:j + 5] = new
        k += len(new)
    return '\n'.join(lines)


SEARCH_LOOP = re.compile(
    r'^[ \t]*(loop_\d+):\n'
    r'([ \t]+)if \((\(u32\) )?(\w+) >= (\w+)\) \{\n'
    r'\2    (\w+) = ([^;\n]+);\n'
    r'\2\} else if \(([^\n]+)\) \{\n'
    r'((?:\2    [^\n{}]*;\n)*?)'
    r'\2    \6 = ([^;\n]+);\n'
    r'\2\} else \{\n'
    r'\2    \4 \+= 1;\n'
    r'\2    goto \1;\n'
    r'\2\}\n', re.M)


def inline_search_helpers(body):
    """step 109b, a variant: m2c's reading of an inlined search function (the fn_800FA5AC family: 30 drafts),
           I = 0; ...                           static inline T_R name_search0(params) {
           loop_N:                                  T_I I;
               if (I >= K) {                        for (I = 0; I < K; I += 1) {
                   R = D;              ->               if (C) {
               } else if (C) {                              return I;
                   R = I;                               }
               } else {                             }
                   I += 1;                          return D;
                   goto loop_N;                 }
               }                                ... R = name_search0(args);
    The inlined body keeps its own variables (in the original the search's index and result take saved registers of
    their own: fn_800FA5AC r31 / r29, fn_800AC158, fn_800FBDF8), which no single function's C gives. The locals /
    parameters C reads are passed as parameters of the same names and types; `I = 0;` before it goes when I has no other
    use."""
    decls = _local_decls(body)
    dm = DEF.search(body)
    if not dm:
        return body
    helpers = []
    for k, m in enumerate(list(SEARCH_LOOP.finditer(body))):
        if body.find(m.group(0)) < 0:
            continue
        label, ind, cast, i, lim, r, d, c, pre, val = m.groups()
        if i not in decls or r not in decls or re.search(r'\b(?:goto|return|break|continue)\b', c + pre):
            continue
        used = c + '\n' + pre + '\n' + val
        free = [n for n in dict.fromkeys(re.findall(r'(?<![.>])\b([A-Za-z_]\w*)\b', used)) if n in decls and n not in (i, r)]
        # the found branch may store (`lbl[i] = 1;`) but not into an outer local
        if any(re.search(r'^\s*%s\s*[-+*/|&^]?=(?!=)' % re.escape(n), pre, re.M) for n in free + [r]):
            continue
        name = '%s_search%d' % (dm.group(2), k)
        ti, tr = decls[i], decls[r]
        params = ', '.join('%s %s%s' % (decls[n][0], decls[n][1], n) for n in free) or 'void'
        pre_lines = ''.join('            ' + x.strip() + '\n' for x in pre.split('\n') if x.strip())
        helpers.append('static inline %s %s%s(%s) {\n    %s %s%s;\n\n    for (%s = 0; %s < %s; %s += 1) {\n        if (%s) {\n'
                       '%s            return %s;\n        }\n    }\n    return %s;\n}\n'
                       % (tr[0], tr[1], name, params, ti[0], ti[1], i, i, (cast or '') + i, lim, i, c, pre_lines, val, d))
        call = '%s%s = %s(%s);\n' % (ind, r, name, ', '.join(free))
        after = body[body.find(m.group(0)) + len(m.group(0)):]
        cp = re.match(r'%s(\w+) = %s;\n' % (re.escape(ind), r), after)
        if cp and len(re.findall(r'\b%s\b' % r, body.replace(m.group(0), ''))) == 2:
            # `R = search(); X = R;` with R used nowhere else: the inlined result goes to X (fn_800EAF34: mr. r30, r29)
            body = body.replace(m.group(0) + cp.group(0), '%s%s = %s(%s);\n' % (ind, cp.group(1), name, ', '.join(free)), 1)
            body = re.sub(r'^[ \t]+[\w ]+?\**\s*\b%s;[^\n]*\n' % r, '', body, count=1, flags=re.M)
            call = None
        if call:
            body = body.replace(m.group(0), call, 1)
        # the index's own `I = 0;` and declaration go when nothing else uses it
        others = re.sub(r'^[ \t]+%s = 0U?;\n' % i, '', body, count=1, flags=re.M)
        if not re.search(r'\b%s\b' % i, re.sub(r'^[ \t]+[\w ]+?\**\s*\b%s;[^\n]*\n' % i, '', others, count=1, flags=re.M)):
            body = re.sub(r'^[ \t]+[\w ]+?\**\s*\b%s;[^\n]*\n' % i, '', others, count=1, flags=re.M)
    if helpers:
        dm = DEF.search(body)
        body = body[:dm.start()] + '\n'.join(helpers) + '\n' + body[dm.start():]
    return body


def goto_exit_loops(body):
    """step 109b, a variant: m2c's reading of a loop whose exit test returns,
           loop_N:                          while (!(T)) {
               if (T) {                         S...
                   return X;        ->      }
               }                            return X;
               S...
               goto loop_N;
    (the label used once, at the end of S, no break / continue in S at the loop's level): `while` tests at the bottom
    (fn_800A4784's walk along a linked list: `lwz r31, 0x8(r31); cmplwi r31, 0; bne`)"""
    lines = body.split('\n')
    k = 0
    while k + 3 < len(lines):
        m = re.fullmatch(r'(loop_\d+):', lines[k].strip())
        if not m or len(re.findall(r'\b%s\b' % m.group(1), '\n'.join(lines))) != 2:
            k += 1
            continue
        ind = re.match(r'\s*', lines[k + 1]).group(0)
        c1 = re.fullmatch(re.escape(ind) + r'if \((.+)\) \{', lines[k + 1])
        rt = re.fullmatch(re.escape(ind) + r'    (return(?: [^;]+)?;)', lines[k + 2])
        if not (c1 and rt and lines[k + 3] == ind + '}'):
            k += 1
            continue
        j = k + 4
        while j < len(lines) and lines[j] != ind + 'goto %s;' % m.group(1) and (lines[j].startswith(ind) or not lines[j].strip()):
            j += 1
        s_body = lines[k + 4:j]
        if (j >= len(lines) or any(re.match(re.escape(ind) + r'(?:break|continue)\b', x) for x in s_body)
                or any(re.match(re.escape(ind) + r'\S', x) and x.strip().endswith(':') for x in s_body)):
            k += 1
            continue
        new = [ind + 'while (%s) {' % _negate_test(c1.group(1))] + ['    ' + x if x.strip() else x for x in s_body] + \
              [ind + '}', ind + rt.group(1)]
        lines[k:j + 1] = new
        k += len(new)
    return '\n'.join(lines)


def irregular_switch_ifs(body):
    """step 109b, a variant: a switch m2c marks `/* irregular */` (no jump table: the original compares the value
    case by case in source order, `cmplwi 2; bne; ...; cmplwi 3; beq; cmplwi 4; bne`) written as the if / else-if chain
    it was: the compiler's own switch code is a compare tree (`cmpwi 2; beq; bge; ...`). Only switches whose case groups
    all end in `break;` / `return` and whose default (if any) is last (fn_8000F7A4, fn_8000E3B8)"""
    lines = body.split('\n')
    k = 0
    while k < len(lines):
        m = re.fullmatch(r'(\s*)switch \((.+)\) \{\s*/\* (?:switch \d+; )?irregular \*/', lines[k])
        if not m:
            k += 1
            continue
        ind, ex = m.group(1), m.group(2)
        j = k + 1
        groups, cur = [], None
        ok = True
        while j < len(lines) and lines[j] != ind + '}':
            l = lines[j]
            lm = re.fullmatch(re.escape(ind) + r'(case ([^:]+)|default):\s*(?:/\*.*\*/)?', l)
            if lm:
                if cur is None or cur[1]:
                    cur = [[], []]
                    groups.append(cur)
                cur[0].append(lm.group(2) if lm.group(2) else None)
            elif l.startswith(ind + '    ') and cur is not None:
                cur[1].append(l)
            elif l.strip():
                ok = False
                break
            j += 1
        if not ok or j >= len(lines) or not groups:
            k += 1
            continue
        out = []
        for gi, (labels, stmts) in enumerate(groups):
            last = stmts[-1].strip() if stmts else ''
            if None in labels and gi != len(groups) - 1:
                ok = False
                break
            if last == 'break;':
                stmts = stmts[:-1]
            elif not (last.startswith('return') or (gi == len(groups) - 1)):
                ok = False
                break
            if any(re.match(r'\s*(?:break|continue|case |default:)', x) for x in stmts) or any('switch' in x for x in stmts):
                ok = False
                break
            if None in labels:
                if len(labels) > 1:
                    ok = False
                    break
                out.append(ind + ('} else {' if out else '{'))
            else:
                cond = ' || '.join('(%s == %s)' % (ex, v.strip()) for v in labels) if len(labels) > 1 else '%s == %s' % (ex, labels[0].strip())
                out.append(ind + ('} else if (%s) {' if out else 'if (%s) {') % cond)
            out += stmts
        if not ok or not out:
            k += 1
            continue
        out.append(ind + '}')
        lines[k:j + 1] = out
        k += len(out)
    return '\n'.join(lines)


def record_array_address(body):
    """step 109b, a variant: the address `B + ((I * S) + O)` of a byte pointer B (record_array_index does the loads
    and stores) written `&((u8 (*)[S]) (B))[I][O]`: the compiler adds O to the scaled index before the base (`mulli;
    addi 0x350; add`) as the original does (fn_80026BF4), where the byte sum adds the base first"""
    decls = _local_decls(body)

    def fix(m):
        d = decls.get(m.group(1))
        if d and d[1] == '*' and not d[2] and d[0] in ('u8', 's8', 'void', 'char'):
            return '&((u8 (*)[%s]) (%s))[%s][%s]' % (m.group(3), m.group(1), m.group(2), m.group(4))
        return m.group(0)
    return re.sub(r'(?<![\w.>*])(\w+) \+ \(\((\w+) \* (0x[0-9A-Fa-f]+|\d+)\) \+ (0x[0-9A-Fa-f]+|\d+)\)', fix, body)


def minus_one_compares(body):
    """step 109b (on by default): `addis r0, r3, 1; cmplwi r0, 0xffff` is the compiler's `x == -1` (0xFFFFFFFF); m2c
    prints it `(u32) (x + 0x10000) == -1U` (its immediate sign-extended), which is another value and compiles to an
    extra add (fn_8000F7A4, fn_8000E3B8: lbl_8044F366): written `x == -1U`"""
    return re.sub(r'\(u32\) \(([\w.\[\]>-]+) \+ 0x10000\) ([=!]=) -1U', r'\1 \2 -1U', body)


def self_update_refs(body):
    """step 109b, a variant: m2c folds a local's known value into its next update, `x = 0x80000000; x = 0x80000000 >>
    1;` (two constants), where the original shifted the register (`lis r31, 0x8000; srawi r31, r31, 1`: fn_8000F7A4,
    fn_8000E3B8): the second statement reads the local again, `x = x >> 1;`"""
    return re.sub(r'^([ \t]+)(\w+) = ([^;\n]+);\n\1\2 = \3 (>>|<<|\+|-|\*|&|\||\^) ',
                  lambda m: '%s%s = %s;\n%s%s = %s %s ' % (m.group(1), m.group(2), m.group(3), m.group(1), m.group(2), m.group(2),
                                                            m.group(4)), body, flags=re.M)


def word_table_rows(body):
    """step 109b, a variant: a table of word pairs / rows read by m2c as byte offsets, `M2C_FIELD(T, f32 *, (R * 8) &
    0x7F8)` and `*(f32 *) (T + 4 + R * 8)`, written as the 2-D array it is, `((f32 (*)[2]) T)[R][0]` / `[R][1]`: the
    compiler then forms the row address once and loads at 0 / 4 (fn_800FA5AC's family: the original's `clrlslwi; add;
    lfs 0x0` / `lfs 0x4`, where the masked byte offset gives other code)"""
    def field(m):
        t, r, n = m.group(2), m.group(3), int(m.group(4), 0)
        return '((%s (*)[%d]) %s)[%s][0]' % (t, n // 4, m.group(1), r) if n % 4 == 0 and n >= 4 else m.group(0)
    body = re.sub(r'M2C_FIELD\((\w+), (f32|s32|u32) \*, \((\w+) \* (0x[0-9A-Fa-f]+|\d+)\) & 0x[0-9A-Fa-f]+\)', field, body)

    def elem(m):
        t, base, off, r, n = m.group(1), m.group(2), int(m.group(3), 0), m.group(4), int(m.group(5), 0)
        if n % 4 or off % 4 or off >= n:
            return m.group(0)
        return '((%s (*)[%d]) %s)[%s][%d]' % (t, n // 4, base, r, off // 4)

    def member(m):
        base, r, n, t, off = m.group(1), m.group(2), int(m.group(3), 0), m.group(4), int(m.group(5), 0)
        if n % 4 or off % 4 or off >= n:
            return m.group(0)
        return '((%s (*)[%d]) %s)[%s][%d]' % (t, n // 4, base, r, off // 4)
    body = re.sub(r'M2C_FIELD\(&(\w+)\[\((\w+) \* (0x[0-9A-Fa-f]+|\d+)\) & 0x[0-9A-Fa-f]+\], (f32|s32|u32) \*, '
                  r'(0x[0-9A-Fa-f]+|\d+)\)', member, body)
    return re.sub(r'\*\((f32|s32|u32) \*\) \((\w+) \+ (0x[0-9A-Fa-f]+|\d+) \+ (\w+) \* (0x[0-9A-Fa-f]+|\d+)\)', elem, body)


def stack_element_steps(body):
    """step 109b (on by default): m2c's `*(&sp8 + (I * 2))` on a u16 stack local (a stack array it split into scalars)
    steps in BYTES; C scales it by the element: written `(&sp8)[I]` when the factor is the local's own size, which is
    what the original computes (`clrlslwi 24, 1; lhzx`, fn_800FFA50's four u16 slots) instead of the doubled step"""
    decls = _local_decls(body)
    size = {'u8': 1, 's8': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'f32': 4, 'f64': 8}

    def fix(m):
        cast = m.group(1) or ''
        d = decls.get(m.group(2))
        if d and not d[1] and not d[2] and size.get(d[0]) == int(m.group(4), 0) and size.get(d[0], 1) > 1:
            # a cast to the slot's own type goes too (the `lhzx` value is stored as loaded: fn_800FFA50)
            return '%s(&%s)[%s]' % ('' if cast == '(%s) ' % d[0] else cast, m.group(2), m.group(3))
        return m.group(0)
    # also the masked index of a narrow loop counter, `*(&sp8 + ((var_r31 * 2) & 0x1FE))` (fn_800FB688)
    body = re.sub(r'(\(\w+\) )?\*\(&(sp\w+) \+ \(\((\w+) \* (0x[0-9A-Fa-f]+|\d+)\) & 0x[0-9A-Fa-f]+\)\)', fix, body)
    return re.sub(r'(\(\w+\) )?\*\(&(sp\w+) \+ \((\w+) \* (0x[0-9A-Fa-f]+|\d+)\)\)', fix, body)


RULES_109B = (stack_param_alias, drop_local_deref_casts, inline_search_helpers, goto_search_loops, goto_exit_loops, goto_while_loops,
              word_table_rows, stack_element_steps, self_update_refs, minus_one_compares, irregular_switch_ifs,
              record_array_address)
OPTIONAL = OPTIONAL + (stack_param_alias, drop_local_deref_casts, inline_search_helpers, goto_search_loops, goto_exit_loops, goto_while_loops,
                       word_table_rows, self_update_refs, irregular_switch_ifs, record_array_address)
VARIANTS = VARIANTS + (((), (stack_param_alias,)), ((), (drop_local_deref_casts,)), ((register_order,), (drop_local_deref_casts,)),
                       ((), (goto_while_loops,)), ((register_order,), (goto_while_loops,)),
                       ((), (goto_search_loops,)), ((), (goto_search_loops, goto_while_loops)),
                       ((), (inline_search_helpers,)), ((), (inline_search_helpers, record_array_index)),
                       ((), (word_table_rows,)), ((), (inline_search_helpers, word_table_rows)),
                       ((), (goto_exit_loops,)), ((), (goto_exit_loops, goto_while_loops)),
                       ((), (self_update_refs,)), ((), (irregular_switch_ifs,)), ((), (self_update_refs, irregular_switch_ifs)),
                       ((), (record_array_address,)))
_postprocess_108 = postprocess


def postprocess(body, skip=(), add=()):
    """the rules of step 108's postprocess, then step 109b's (RULES_109B, in order)"""
    body = _postprocess_108(body, skip, add)
    for f in RULES_109B:
        if f not in skip and (f not in OPTIONAL or f in add):
            body = f(body)
    return body


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default=os.path.join(D, 'drafts'))
    ap.add_argument('--sample', type=int, default=0, help='only N functions, evenly spaced in address order')
    ap.add_argument('--only', nargs='*', default=[])
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--no-summary', action='store_true', help='do not write decomp/reports/draft_summary.txt')
    a = ap.parse_args()
    t0 = time.time()
    out = os.path.abspath(a.out)
    for s in ('c', 'asm', 'obj', 'log'):
        shutil.rmtree(os.path.join(out, s), ignore_errors=True)
        os.makedirs(os.path.join(out, s))
    ver, flags = game_flags()
    # inputs, copied once: the other builds may rewrite decomp/build while this runs
    funcs, jt, lib = game_functions(with_lib=True)
    assert funcs and funcs[0][1] == GAME_START, 'game code asm not as expected (run batch 9 first)'
    shutil.rmtree(os.path.join(out, 'orig'), ignore_errors=True)
    os.makedirs(os.path.join(out, 'orig'))
    copies = {}
    for f in funcs:
        if f[4] not in copies:
            copies[f[4]] = os.path.join(out, 'orig', '%03d.o' % len(copies))
            shutil.copyfile(f[4], copies[f[4]])
    funcs = [(n, ad, sz, b, copies[o]) for n, ad, sz, b, o in funcs]
    starts = {n: ad for n, ad, _, _, _ in funcs}
    # the library functions the game code calls (bl targets outside the game range): the ones no header declares get
    # a prototype from the SDK headers (ctx_sdk.h) or, for the unnamed fn_, from their own first-pass draft
    called = callees(funcs)
    game = set(starts)
    libcalled = sorted(n for n in called if n in lib and n not in starts)
    for n in libcalled:
        starts[n] = lib[n][1]

    def jtext(name):
        out = []
        for l in jt[name]:
            m = re.match(r'^\s*\.4byte (\S+)\+0x([0-9A-Fa-f]+)\s*$', l)
            if m and m.group(1) in starts:
                l = '	.4byte .L_%08X' % (starts[m.group(1)] + int(m.group(2), 16))
            out.append(l)
        return ['.obj %s, local' % name] + out + ['.endobj %s' % name]
    for name, ad, sz, b, _ in funcs + [lib[n] for n in libcalled]:
        for t in sorted(set(re.findall(r'jumptable_[0-9A-F]{8}', chr(10).join(b)))):
            if t in jt:
                tx = jtext(t)
                want = set(re.findall(r'\.L_([0-9A-F]{8})', chr(10).join(tx)))
                have = set(re.findall(r'^\.L_([0-9A-F]{8}):', chr(10).join(b), re.M))
                nb = []
                for l in b:   # a jump-table target may have no label yet (nothing else branches there)
                    m = re.match(r'^/\* ([0-9A-F]{8}) ', l)
                    if m and m.group(1) in want - have:
                        nb.append('.L_%s:' % m.group(1))
                    nb.append(l)
                b = nb + [''] + tx
        # dtk prints `cror eq, lt, eq` (x <= y after fcmpo); m2c's FcmpoCrorPattern only knows the numeric form
        # `cror 2, 0, 2`, and without it m2c took the bne after it for `==` (79 functions; step 64)
        b = [CROR.sub(lambda m: 'cror 2, %d, 2' % {'lt': 0, 'gt': 1}[m.group(1)], l) for l in b]
        open(os.path.join(out, 'asm', name + '.s'), 'w', encoding='utf-8', newline='\n').write('\n'.join(b) + '\n')
    jobs = [(n, ad, sz, o) for n, ad, sz, _, o in funcs]
    if a.only:
        jobs = [j for j in jobs if j[0] in a.only]
    elif a.sample:
        step = len(jobs) / a.sample
        jobs = [jobs[int(i * step)] for i in range(a.sample)]
    # the m2c context: headers preprocessed once with the game compiler itself
    # Melee's MSL math.h (through the HSD headers) declares `float sqrt(double)`; the game's sqrt (MSL w_sqrt.c,
    # symbols/cr_types.h since step 65) returns a double (its callers round with frsp), so Melee's is renamed away
    base = ('// context of the m2c drafts: m2c macros + every HSD header + our types (symbols/cr_types.h)\n#include "m2c_macros.h"\n'
            + '#define sqrt melee_math_sqrt\n'
            + ''.join('#include "%s"\n' % h for h in sorted(os.listdir(os.path.join(G, 'tools/melee/src/sysdolphin/baselib'))) if h.endswith('.h'))
            + '#undef sqrt\n#include "../../symbols/cr_types.h"\n' + SECTION_PRELUDE + globals_decls())
    open(os.path.join(out, 'm2c_run.py'), 'w', newline='\n').write(
        "import sys\nsys.path.insert(0, %r)\n%sfrom m2c.main import main\nmain()\n" % (M2C.replace('\\', '/'), M2C_WIDTH))
    protos = os.path.join(out, 'ctx_protos.h')
    sdk = os.path.join(out, 'ctx_sdk.h')

    def context(text, check=True):
        open(os.path.join(out, 'ctx.h'), 'w', newline='\n').write(text)
        ctxin = os.path.join(out, 'ctx_in.h')
        shutil.copyfile(os.path.join(out, 'ctx.h'), ctxin)
        cmd = mwcc_cmd(ver) + ['-E', '-nodefaults', '-proc', 'gekko', '-nosyspath'] + CTX_DEFS \
            + sum([['-i', i] for i in CTX_INC], []) + ['-i', out.replace(os.sep, '/'), ctxin]
        rc, so, se = run(cmd, 120, cwd=D)
        if rc != 0:
            sys.exit('context preprocess failed:\n' + so[:2000] + se[:2000])
        # m2c's context parser reads plain C: our own preprocessed text, with the macros it defines itself dropped
        open(os.path.join(out, 'ctx_pp.c'), 'w', encoding='utf-8', newline='\n').write(so)
        if check:   # the compiler must accept the context as it is (every draft includes it)
            chk = os.path.join(out, 'ctx_check.c')
            open(chk, 'w', newline='\n').write('#include "ctx.h"\n')
            rc, so2, se2 = run(mwcc_cmd(ver) + ['-c'] + flags + CTX_DEFS + ['-DCR_DRAFT_CC', '-i', out.replace(os.sep, '/')] + sum([['-i', i] for i in CTX_INC], [])
                               + ['-o', os.path.join(out, 'ctx_check.o'), chk], 120, cwd=D)
            if rc != 0:
                sys.exit('the context does not compile:\n' + (so2 + se2)[:3000])
        # prime m2c's context cache once (workers then only read it)
        run([sys.executable, '-I', os.path.join(out, 'm2c_run.py'), '-t', 'ppc-mwcc-c', '--context', os.path.join(out, 'ctx_pp.c'),
             '-f', funcs[0][0], os.path.join(out, 'asm', funcs[0][0] + '.s')], 300)
        return so

    full = not a.only and not a.sample
    if full or not os.path.exists(protos) or not os.path.exists(sdk):
        # the SDK / MSL prototypes of the library functions the game calls that the HSD headers do not declare
        ctx_pp = context(base, check=False)
        und = sorted(n for n in called if n not in game and not declared(n, ctx_pp))
        found, tdefs = sdk_protos(und, ctx_pp)
        open(sdk, 'w', encoding='utf-8', newline='\n').write(
            '// prototypes of the library functions the game code calls that the HSD headers do not declare, from the\n'
            '// decomp\'s SDK headers (decomp/include/dolsdk2004, MSL math.h, LIB_PROTOS; tools/draft_batch.py sdk_protos);\n'
            '// gitignored, a full run rewrites it\n'
            + ''.join('%s\n' % td for td in tdefs)
            + ''.join('%s   // %s\n' % (found[n][1], found[n][0]) for n in sorted(found)))
        print('SDK prototypes: %d of %d undeclared library callees (%d unnamed fn_ get theirs from pass 1)' % (
            len(found), len(und), sum(1 for n in und if n in lib and n not in found)))
        # pass 1: every function once, without the data declarations, for the signature of its own draft (the callers
        # then see the callee's parameters) and the pointer types m2c infers for 4-byte data objects; the library
        # functions the game calls that have no prototype yet get theirs the same way
        ctx_pp = context(base + '#include "ctx_sdk.h"\n')
        sigs, ptr, rets, pars = {}, collections.defaultdict(set), collections.defaultdict(set), collections.defaultdict(list)
        p1 = [(n, ad, sz, o) for n, ad, sz, _, o in funcs] + [(n, lib[n][1], lib[n][2], lib[n][4]) for n in libcalled if n in und and n not in found]
        with cf.ProcessPoolExecutor(a.jobs, initializer=init, initargs=(out, ver, flags, True)) as ex:
            for name, sig, ext, _ in pool_map(ex, one, p1):
                if sig:
                    sigs[name] = (starts[name], sig)
                    externs, fdecls, fparams = ext
                    for t, lbl in externs:
                        if '*' in t:
                            ptr[lbl].add(t)
                    for t, fn in fdecls:
                        rets[fn].add(t.strip())
                    for fn, ps in fparams:
                        pars[fn].append(tuple(x.strip() for x in ps.split(',')))
        nr3 = r3_returns(sigs, rets)
        r3_set(sigs)
        narrowed = narrow_returns(sigs, {f: ts - {'CR_R3'} for f, ts in rets.items()})
        nparams = narrow_params(sigs, pars)
        ptrs = {n: (next(iter(ts)) if len(ts) == 1 and ts != {'void *'} else MIXED if len(ts) > 1 else 'u8 *')
                for n, ts in ptr.items()}
        open(protos, 'w', encoding='utf-8', newline='\n').write(
            protos_text(sigs, ctx_pp) + '\n// data objects m2c typed as pointers in that pass (size 4; tools/draft_batch.py data_decls)\n'
            + ''.join('#define CR_PTR_%s %s\n' % (n, ptrs[n]) for n in sorted(ptrs)))
        print('pass 1: %d prototypes (%d return types narrowed by their callers, %d made value-returning by them, %d parameters signed / unsigned by them), %d pointer objects from the drafts (%d s)' % (
            len(sigs), narrowed, nr3, nparams, len(ptrs), time.time() - t0))
    ptrs = dict(re.findall(r'^#define CR_PTR_(lbl_[0-9A-F]{8}) (.*)$', open(protos, encoding='utf-8').read(), re.M))
    context(base + '#include "ctx_sdk.h"\n' + data_decls(ptrs, load_signs(funcs), load_signs.extents) + '\n#include "ctx_protos.h"\n')
    res = {}
    with cf.ProcessPoolExecutor(a.jobs, initializer=init, initargs=(out, ver, flags)) as ex:
        for name, status, note, eqb in pool_map(ex, one, jobs):
            res[name] = (status, note, eqb)
    # reports (address order, no times, no paths)
    cnt = collections.Counter()
    hist = collections.Counter()
    errs = collections.Counter()
    lines = []
    eqb = 0
    for name, ad, sz, _ in jobs:
        status, note, b = res[name]
        eqb += b
        if status == 'm2c failed':
            cnt['m2c failed'] += 1
            errs['m2c: ' + note] += 1
        else:
            cnt['m2c ok'] += 1
            if status == 'compile failed':
                cnt['compile failed'] += 1
                errs['compile: ' + note] += 1
            else:
                cnt['compiled'] += 1
                p = 100 if status.startswith('100') else int(status[:-1])
                hist[min(p // 10, 10)] += 1
                if p == 100:
                    cnt['100% MATCH'] += 1
        lines.append('%08X  %-44s %5d  %-14s %s' % (ad, name, sz, status, note))
    n = len(jobs)
    tot = ['functions %d' % n, 'm2c ok %d' % cnt['m2c ok'], 'm2c failed %d' % cnt['m2c failed'],
           'compile failed %d' % cnt['compile failed'], 'compiled %d' % cnt['compiled'], '100%% MATCH %d' % cnt['100% MATCH'],
           'bytes matched %d of %d (aligned equal words of the compiled drafts, of the bytes of all %d functions)' % (eqb, sum(j[2] for j in jobs), n)]
    hl = ['match %% histogram of the %d compiled drafts (100%% = every word equal, relocation fields masked; below: aligned equal words / words):' % cnt['compiled']]
    for b in range(10):
        hl.append('  %3d-%2d%%  %d' % (b * 10, b * 10 + 9, hist[b]))
    hl.append('  100%%     %d' % hist[10])
    el = ['most common failures:'] + ['  %5d  %s' % (c, m) for m, c in sorted(errs.items(), key=lambda x: (-x[1], x[0]))[:25]]
    head = ['draft and check: tools/draft_batch.py (m2c %s, compiler %s, flags: %s)' % ('pinned in tools/pins.txt', ver, ' '.join(flags[:0]) or 'configure.py game set'),
            'addr      function                                       bytes  status         note']
    open(os.path.join(out, 'draft_report.txt'), 'w', encoding='utf-8', newline='\r\n').write(
        '\n'.join(head + lines + [''] + tot + [''] + hl + [''] + el) + '\n')
    if not a.no_summary and not a.sample and not a.only:
        rp = os.path.join(D, 'reports', 'draft_summary.txt')
        open(rp, 'w', encoding='utf-8', newline='\r\n').write('\n'.join(
            ['draft and check of the game code (tools/draft_batch.py, tools/batches/batch12_drafts.ps1; docs/drafts.md)',
             'm2c drafts of every game function (decomp/drafts/, gitignored), compiled with the game compiler (%s) and compared with the original' % ver,
             'counts only; the drafts themselves are decompiled output and are never committed', ''] + tot + [''] + hl + [''] + el) + '\n')
    print('\n'.join(tot))
    print('\n'.join(hl))
    print('runtime %d s (%d jobs)' % (time.time() - t0, a.jobs))


if __name__ == '__main__':
    main()
