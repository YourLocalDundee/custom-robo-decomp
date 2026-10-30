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
    if not want or s != '*' + var.format(fmt):
        return s
    if not _scalar(self.type):
        # step 69: a word / halfword m2c could not tell from a pointer (its int-or-pointer type) read from a byte array
        # (`(u32) *(lbl_8024E120 + i * 0x33520) != 0`, dtk's untyped data: a lbz where the original has lwz:
        # fn_8000768C, fn_800BC9A4, fn_8010D2D0): the access's own width
        if want in (2, 4) and _target_size(var) == 1:
            return 'M2C_FIELD(%s, %s, 0)' % (var.format(fmt), 'u32 *' if want == 4 else 'u16 *')
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
    return 'M2C_FIELD(%s, %s, 0)' % (var.format(fmt), _t.Type.ptr(self.type).format(fmt))


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
        for l in open(asm, encoding='utf-8', errors='replace').read().split(chr(10)):
            m = HDR.match(l)
            if m:
                addr, size = int(m.group(1), 16), int(m.group(2), 16)
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
                jt[blk[0]] = blk[1]
                blk = None
            elif blk:
                blk[1].append(l)
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
ELEM = {'u8': 1, 'char': 1, 'u16': 2, 'u32': 4, 'f32': 4, 'f64': 8}
SMALL = ('sdata', 'sbss', 'sdata2', 'sbss2')


DECL_SECTION = {'data': '.data', 'bss': '.data', 'rodata': '.rodata', 'sdata': '.sdata', 'sbss': '.sdata',
                'sdata2': '.sdata2', 'sbss2': '.sdata2'}
SECTION_PRELUDE = ('\n// CR_SECTION(s): the section of a data object, so the compiler picks the original data model whatever its\n'
                   '// size (small data = sda21, large = lis/addi); __declspec only when a draft is compiled (-DCR_DRAFT_CC),\n'
                   '// nothing for m2c\'s context (tools/draft_batch.py data_decls)\n'
                   '#ifdef CR_DRAFT_CC\n#undef __declspec\n#define CR_SECTION(s) __declspec(section s)\n'
                   '#else\n#define CR_SECTION(s)\n#endif\n')


def data_decls(ptrs={}):
    """extern declarations of every lbl_ data object of decomp/config/GXCE01/symbols.txt, with the element type dtk
    found (data:byte/2byte/4byte/float/double/string; u8 otherwise), its size, and the right data model: the game is
    compiled with -sdata 8 -sdata2 8, so the compiler takes an extern of at most 8 bytes for small data (sda21) and a
    larger one for large data (lis/addi). An object whose size disagrees with its section (a small-section object of
    more than 8 bytes, a large-section object of at most 8) gets its section named with CR_SECTION (__declspec(section),
    which sets the data model of an extern in GC/2.6; step 62: before, such objects were cut to u8[8] or declared
    u8 name[], which hid their type from m2c: a float in .rodata was read as bytes); a size-0 object of a large section
    stays u8 name[].
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
    for l in open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8'):
        m = re.match(r'^(lbl_[0-9A-F]{8}) = \.(\w+):0x[0-9A-F]+; // type:object size:0x([0-9A-F]+)(.*)$', l)
        if not m:
            continue
        name, sec, size, rest = m.group(1), m.group(2), int(m.group(3), 16), m.group(4)
        d = re.search(r'\bdata:(\w+)', rest)
        t = DATA_TYPES.get(d.group(1) if d else None, 'u8')
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
        if size == 4 and ptrs.get(name) == MIXED:
            out.append('// CR_MIXED %s %s\n' % (name, pre.strip()))
        elif size == 4 and name in ptrs:
            out.append('%sextern %s%s;\n' % (pre, ptrs[name], name))
        elif size == es:
            out.append('%sextern %s %s;\n' % (pre, t, name))
        elif size % es == 0:
            out.append('%sextern %s %s[%d];\n' % (pre, t, name, size // es))
        else:
            out.append('%sextern u8 %s[%d];\n' % (pre, name, size))
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
    """a variant (step 69): a local `u8 v;` (u16 alike) whose every assignment is `v = (u8) (E);` becomes a u32 that
    takes `(E) & 0xFF`: m2c types a register by the `rlwinm` that extracted its byte, but as a byte the compiler masks
    it again where it is used (`rlwinm r0,r30,2,22,29` for `v * 4` where the original has `slwi r0,r30,2`;
    fn_8005C628). The same values: v only ever holds the masked byte."""
    for m in list(re.finditer(r'^    (u8|u16) (\w+);$', body, re.M)):
        t, v = m.group(1), m.group(2)
        n = re.escape(v)
        asg = re.findall(r'^\s+%s = \(%s\) (.+);$' % (n, t), body, re.M)
        if not asg or len(re.findall(r'\b%s\s*(?:[-+*/%%&|^]|<<|>>)?=(?!=)|\b%s\s*(?:\+\+|--)|(?:\+\+|--)\s*%s\b' % (n, n, n), body)) != len(asg):
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


REGVAR = re.compile(r'^\s+[^;=()]*?\b(?:var|temp)_([rf])(\d+)_\w*(?:\[\w*\])*;\s*$')


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
            m = re.match(r'^(lbl_[0-9A-F]{8}) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([48])\b.*\bdata:(float|double)\b', l)
            if not m:
                continue
            a, double = int(m.group(2), 16), m.group(4) == 'double'
            if int(m.group(3)) != (8 if double else 4):
                continue
            b = read(a, 8 if double else 4)
            if b is None:
                continue
            t = float_text(struct.unpack('>d' if double else '>f', b)[0], double)
            if t:
                out[m.group(1)] = t
        W['floats'] = out
    return W['floats']


def float_literals(body):
    """a float / double constant of .sdata2 read by name (`M2C_FIELD(p, f32 *, 0x28) = lbl_80450CB0;`) written as the
    literal it holds (`0.0f`): the source wrote literals, and the compiler allocates a literal's register differently
    from an extern's (three stores of 0.0f load f0, f1, f0 with the second load moved up; three of an extern float f0,
    f0, f0: fn_80079C70, fn_8001CA84; step 69). The literal is the compiler's own pool constant (the file's .sdata2
    entry at that address: the compare masks the relocation); a variant, because the extern form matches elsewhere."""
    fl = pool_floats()
    if not fl:
        return body
    pat = re.compile(r'(?<![&\w])(lbl_[0-9A-F]{8})\b(?!\s*(?:\[|=[^=]|\+=|-=|\*=|/=))')
    return '\n'.join(l if re.search(r'\bextern\b', l) else pat.sub(lambda m: fl.get(m.group(1), m.group(1)), l)
                     for l in body.split('\n'))


def void_null(body):
    """`p != NULL` with the null pointer written `((void *) 0)`: the compiler then loads the 0 into a register
    (`li r0,0; cmplw r3,r0`) where `NULL` (MSL's 0L) gives `cmplwi r3,0`; the game uses both (fn_80077290), so this is
    a variant (one(), step 64)"""
    return re.sub(r'([=!]=) NULL\b', r'\1 ((void *) 0)', body)


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


def postprocess(body, skip=(), add=()):
    """the post-processing rules in order; skip: rules left out, add: OPTIONAL rules applied (one() VARIANTS)"""
    for f in (declare_stack_arrays, declare_unk_stack, drop_param_spills, drop_saved_reg_reads, type_void_stores,
              signed_fields, unsigned_fields, compound_assign, drop_same_casts, drop_same_field_casts, while_loops,
              table_call_args, table_call_self, table_call_noargs, narrow_int_casts, minus_one_stores,
              unsigned_const_stores, redundant_returns, widen_masked_bytes, invert_early_returns, chain_assign,
              double_negation, assert_ternary, mixed_externs, byte_steps, same_type_copies, param_regs, param_homes,
              param_copies, param_aliases, inline_volatile_temps, register_order, narrow_minus_one, void_null,
              float_literals):
        if f not in skip and (f not in OPTIONAL or f in add):
            body = f(body)
    return body


# rules only a variant applies, and the variants a near draft is also tried with (one(), step 64), in this order:
# (rules left out, optional rules applied)
OPTIONAL = (void_null, param_homes, float_literals, drop_same_field_casts, table_call_noargs, unsigned_fields,
            redundant_returns, widen_masked_bytes, invert_early_returns, chain_assign)
VARIANTS = (((register_order,), ()), ((param_copies,), ()), ((param_copies, register_order), ()), ((), (param_homes,)),
            ((), (void_null,)), ((register_order,), (void_null,)),
            # step 69
            ((inline_volatile_temps,), ()), ((param_aliases,), ()), ((), (float_literals,)), ((), (drop_same_field_casts,)),
            ((), (table_call_noargs,)), ((), (unsigned_fields,)), ((), (redundant_returns,)), ((), (widen_masked_bytes,)),
            ((), (invert_early_returns,)), ((), (chain_assign,)))


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
                                             re.findall(EXTERN_FN, body), re.findall(EXTERN_PARAMS, body)), 0
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
    if p < 100 and p >= VARIANT_MIN:
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
            for name, sig, ext, _ in ex.map(one, p1, chunksize=4):
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
        narrowed = narrow_returns(sigs, rets)
        nparams = narrow_params(sigs, pars)
        ptrs = {n: (next(iter(ts)) if len(ts) == 1 and ts != {'void *'} else MIXED if len(ts) > 1 else 'u8 *')
                for n, ts in ptr.items()}
        open(protos, 'w', encoding='utf-8', newline='\n').write(
            protos_text(sigs, ctx_pp) + '\n// data objects m2c typed as pointers in that pass (size 4; tools/draft_batch.py data_decls)\n'
            + ''.join('#define CR_PTR_%s %s\n' % (n, ptrs[n]) for n in sorted(ptrs)))
        print('pass 1: %d prototypes (%d return types narrowed by their callers, %d parameters signed / unsigned by them), %d pointer objects from the drafts (%d s)' % (
            len(sigs), narrowed, nparams, len(ptrs), time.time() - t0))
    ptrs = dict(re.findall(r'^#define CR_PTR_(lbl_[0-9A-F]{8}) (.*)$', open(protos, encoding='utf-8').read(), re.M))
    context(base + '#include "ctx_sdk.h"\n' + data_decls(ptrs) + '\n#include "ctx_protos.h"\n')
    res = {}
    with cf.ProcessPoolExecutor(a.jobs, initializer=init, initargs=(out, ver, flags)) as ex:
        for name, status, note, eqb in ex.map(one, jobs, chunksize=4):
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
