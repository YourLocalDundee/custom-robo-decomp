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
arithmetic on `void *`), each fix checked by compiling again.
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
MELEE = '../tools/melee'
# the same defines tools/scripts/ParseHeaders.java uses: no __MWERKS__ (inline asm drops out), M2CTX = the plain-C paths
CTX_DEFS = ['-DM2CTX', '-D__attribute__(x)=', '-D__declspec(x)=']
CTX_INC = [MELEE + '/src', MELEE + '/src/sysdolphin/baselib', MELEE + '/src/MSL', MELEE + '/src/Runtime',
           MELEE + '/libs/doldecomp/include', MELEE + '/libs/dolphin/include', MELEE + '/libs/dolphin/include/libc',
           '../tools/m2c']
FN = re.compile(r'^\.fn (\S+), \w+')
ENDFN = re.compile(r'^\.endfn (\S+)')
HDR = re.compile(r'^# \.text:0x[0-9A-Fa-f]+ \| 0x([0-9A-F]{8}) \| size: 0x([0-9A-F]+)')


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


def game_functions():
    """[(name, address, size, asm lines, original object)] of every game function, in address order, and the jump
    tables {name: lines} of the whole split (from dtk's asm of every object)"""
    end = game_end()
    funcs, jt = [], {}
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
    return funcs, jt


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


def data_decls(ptrs={}):
    """extern declarations of every lbl_ data object of decomp/config/GXCE01/symbols.txt, with the element type dtk
    found (data:byte/2byte/4byte/float/double/string; u8 otherwise), its size, and the right data model: the game is
    compiled with -sdata 8 -sdata2 8, so the compiler takes an extern of at most 8 bytes for small data (sda21) and a
    larger one for large data (lis/addi). An object of a small section is therefore declared with at most 8 bytes
    (the few larger ones are cut to u8[8]; the draft still reaches past it through M2C_FIELD) and an object of a
    large section of at most 8 bytes as an array of unknown size (u8 name[]: the compiler takes it for large data).
    ptrs: {name: pointer type} for the 4-byte objects m2c itself typed as pointers in the first pass (`u8 *` when its
    drafts disagree or say `void *`, which the compiler cannot do arithmetic on)."""
    out = ['\n// every lbl_ data object of decomp/config/GXCE01/symbols.txt with its size, so the compiler picks the\n'
           '// same data model as the original (small data only for the small sections; tools/draft_batch.py data_decls)\n']
    for l in open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8'):
        m = re.match(r'^(lbl_[0-9A-F]{8}) = \.(\w+):0x[0-9A-F]+; // type:object size:0x([0-9A-F]+)(.*)$', l)
        if not m:
            continue
        name, sec, size, rest = m.group(1), m.group(2), int(m.group(3), 16), m.group(4)
        d = re.search(r'\bdata:(\w+)', rest)
        t = DATA_TYPES.get(d.group(1) if d else None, 'u8')
        if sec in SMALL:
            if size > 8:
                t, size = 'u8', 8
            if size == 0:
                size = 1
        elif size <= 8:
            out.append('extern u8 %s[];\n' % name)
            continue
        es = ELEM[t]
        if size == 4 and name in ptrs:
            out.append('extern %s%s;\n' % (ptrs[name], name))
        elif size == es:
            out.append('extern %s %s;\n' % (t, name))
        elif size % es == 0:
            out.append('extern %s %s[%d];\n' % (t, name, size // es))
        else:
            out.append('extern u8 %s[%d];\n' % (name, size))
    return ''.join(out)


DEF = re.compile(r'^([A-Za-z_][\w \t\*]*?)\s*\**\b(\w+)\(([^()]*)\) \{$', re.M)


def signature(name, body):
    """the prototype of a draft's own definition line (`void fn_X(void *arg0, s32 arg1) {`), or None"""
    for m in DEF.finditer(body):
        if m.group(2) == name:
            line = body[m.start():m.end()]
            return line[:-2].rstrip() + ';'
    return None


def protos_text(sigs, ctx_pp):
    """ctx_protos.h: the signatures of the game functions from their own drafts, for the ones the context does not
    declare already (symbols/cr_types.h and the HSD / SDK headers win)"""
    out = ['// prototypes of the game functions from their own m2c drafts (first pass of tools/draft_batch.py);\n'
           '// gitignored like the drafts; a full run rewrites it, --only / --sample reuse it\n']
    for name in sorted(sigs, key=lambda n: sigs[n][0]):
        if not re.search(r'\b%s\s*\(' % re.escape(name), ctx_pp):
            out.append(sigs[name][1] + '\n')
    return ''.join(out)


def globals_decls():
    """extern declarations of our typed globals (symbols/cr_globals.txt: cr_g_Name = .sec:0xADDR; // type:object datatype:T*[N])"""
    out = ['\n// our typed globals (symbols/cr_globals.txt)\n']
    for l in open(os.path.join(G, 'symbols/cr_globals.txt'), encoding='utf-8'):
        m = re.match(r'^(\w+) = \.\w+:0x[0-9A-Fa-f]+; // type:object datatype:(\w+)(\**)((?:\[\d+\])*)(?=\s|;|/)', l)
        if m:
            out.append('extern %s%s %s%s;\n' % (m.group(2), m.group(3), m.group(1), m.group(4)))
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


def postprocess(body):
    return type_void_stores(drop_saved_reg_reads(drop_param_spills(declare_stack_arrays(body))))


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


def repair(lines, e):
    """apply one fix for the compiler's first error to the draft's lines (1-based line numbers); the fix made, or None"""
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
        t = m.group(2)
        if t == 'void':
            # `*(void *)ADDR = value`: a store through an untyped address; the value's type is the store's
            nl = re.sub(r'\*\s*\(void \*\)', '*(%s *)' % m.group(1), s, count=1)
            if nl == s:
                return None
            lines[ln - 1] = nl
            return 'store through a %s *' % m.group(1)
        if '(' in t and '(*)' not in t:
            return None
        st = expr_start(s, at)
        # the message for a call's argument points at the call's end: the argument to cast is the last one that is
        # not cast to that type already
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
        wrap = ex if re.match(r'^[\w.]+$', ex) or (ex.startswith('(') and ex.endswith(')')) else '(' + ex + ')'
        lines[ln - 1] = s[:st] + '(' + t + ') ' + wrap + s[st + len(ex):]
        return 'cast to ' + t
    if msg == 'call of non-function':
        # `*(table + i * 4)(args)`: a call through a pointer m2c could not type; cast the address to a pointer to a
        # pointer to an unprototyped function (its arguments are passed as they are, like the original's)
        for m in re.finditer(r'\*\(', s):
            args, end = split_args(s, m.end() - 1)
            if args is not None and end < len(s) and s[end] == '(' and not s[m.start() - 1:m.start()].isalnum():
                lines[ln - 1] = s[:m.start()] + '(*(M2C_UNK (**)()) ' + s[m.end() - 1:end] + ')' + s[end:]
                return 'call through a function pointer'
        return None
    if msg == 'pointer/array required' and at is not None:
        # `*(x + 8)` / `*x` where m2c typed x as an integer: the load's type is the cast in front of it (`(u16) *(...)`)
        # or s32
        end = at
        while end > 0 and s[end - 1] == ' ':
            end -= 1
        if end and s[end - 1] == ')':
            depth, i = 0, end - 1
            while i >= 0:
                depth += 1 if s[i] == ')' else -1 if s[i] == '(' else 0
                if depth == 0:
                    break
                i -= 1
        else:
            i = end
            while i > 0 and (s[i - 1].isalnum() or s[i - 1] == '_'):
                i -= 1
        j = i
        while j > 0 and s[j - 1] == ' ':
            j -= 1
        if j == 0 or s[j - 1] != '*' or (j >= 2 and s[j - 2] == '*'):
            return None
        cm = re.search(r'\((u8|s8|u16|s16|u32|s32|f32|f64)\) $', s[:j - 1])
        t = (cm.group(1) if cm else 's32') + ' *'
        lines[ln - 1] = s[:j] + '(' + t + ') ' + s[i:]
        return 'load through a ' + t
    if msg in ('illegal type', 'illegal operand'):
        fixed = s.replace('void **', 'u8 **')
        names = set(re.findall(r'\b([A-Za-z_]\w*)\b', s))
        changed = []
        if fixed != s:
            lines[ln - 1] = fixed
            changed.append('void **')
        for i, l in enumerate(lines):
            dm = re.match(r'^(\s*)void \*(\w+);$', l)
            if dm and dm.group(2) in names:
                lines[i] = '%su8 *%s;' % (dm.group(1), dm.group(2))
                changed.append(dm.group(2))
            dm = DEF.match(l)
            if dm:
                def param(pm):
                    return 'u8 *' + pm.group(1) if pm.group(1) in names else pm.group(0)
                nl = re.sub(r'void \*(\w+)\b', param, l)
                if nl != l:
                    lines[i] = nl
                    changed.append('param')
        return 'u8 * for ' + ' '.join(changed) if changed else None
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
    rc, so, se = run([sys.executable, '-I', os.path.join(out, 'm2c_run.py'), '-t', 'ppc-mwcc-c', '--context',
                      os.path.join(out, 'ctx_pp.c'), '--valid-syntax', '--deterministic-vars', '--stack-structs', '-f', name, a], 120)
    body = so.replace('\r\n', '\n')
    if rc != 0 or not re.search(r'^\S.*\b%s\s*\(' % re.escape(name), body, re.M) or 'Decompilation failure' in body:
        msg = (se.strip().splitlines() or body.strip().splitlines() or ['no output'])[-1][:90]
        if W.get('pass1'):
            return name, None, msg, 0
        open(lg, 'w', encoding='utf-8').write(so + '\n' + se)
        return name, 'm2c failed', msg, 0
    if W.get('pass1'):
        return name, signature(name, body), re.findall(r'^extern ([\w ]+ \**)(lbl_[0-9A-F]{8});', body, re.M), 0
    body = postprocess(body)
    # the context's prototype of the function itself (ctx_protos.h, from its first-pass draft) is renamed away while
    # the context is read: the draft defines the function with the signature of this pass
    lines = ('// m2c draft of %s (0x%08X, %d bytes): decompiled output, gitignored, never commit\n'
             '#define %s %s__proto\n#include "ctx.h"\n#undef %s\n\n%s' % (name, addr, size, name, name, name, body)).split('\n')
    cmd = mwcc_cmd(W['ver']) + ['-c'] + W['flags'] + CTX_DEFS + ['-i', 'drafts'] + sum([['-i', i] for i in CTX_INC], []) + ['-o', o, c.replace('\\', '/')]
    fixes = []
    last = None
    while True:
        open(c, 'w', encoding='utf-8', newline='\n').write('\n'.join(lines))
        rc, so, se = run(cmd, 120, cwd=D)
        txt = so + se
        if rc == 0 and os.path.exists(o):
            break
        e = first_error(txt)
        if e is None or len(fixes) >= 300:
            open(lg, 'w', encoding='utf-8').write(txt)
            return name, 'compile failed', compile_error(txt), 0
        before = '\n'.join(lines)
        f = repair(lines, e)
        if f is None or '\n'.join(lines) == before:
            open(lg, 'w', encoding='utf-8').write(txt)
            return name, 'compile failed', compile_error(txt), 0
        fixes.append(f)
    try:
        mine = Elf(open(o, 'rb').read()).func(name)
    except Exception as e:
        mine = None
    if mine is None:
        return name, 'compile failed', 'no function symbol in the object', 0
    orig = orig_elf(orig_obj).func(name)
    if orig is None:
        return name, 'compile failed', 'function not in the original object', 0
    p, eq = compare(orig, mine)
    note = ' '.join(filter(None, ['M2C_ERROR' if 'M2C_ERROR' in body else '', ('%d fixes' % len(fixes)) if fixes else '']))
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
    funcs, jt = game_functions()
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

    def jtext(name):
        out = []
        for l in jt[name]:
            m = re.match(r'^\s*\.4byte (\S+)\+0x([0-9A-Fa-f]+)\s*$', l)
            if m and m.group(1) in starts:
                l = '	.4byte .L_%08X' % (starts[m.group(1)] + int(m.group(2), 16))
            out.append(l)
        return ['.obj %s, local' % name] + out + ['.endobj %s' % name]
    for name, ad, sz, b, _ in funcs:
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
        open(os.path.join(out, 'asm', name + '.s'), 'w', encoding='utf-8', newline='\n').write('\n'.join(b) + '\n')
    jobs = [(n, ad, sz, o) for n, ad, sz, _, o in funcs]
    if a.only:
        jobs = [j for j in jobs if j[0] in a.only]
    elif a.sample:
        step = len(jobs) / a.sample
        jobs = [jobs[int(i * step)] for i in range(a.sample)]
    # the m2c context: headers preprocessed once with the game compiler itself
    base = ('// context of the m2c drafts: m2c macros + every HSD header + our types (symbols/cr_types.h)\n#include "m2c_macros.h"\n'
            + ''.join('#include "%s"\n' % h for h in sorted(os.listdir(os.path.join(G, 'tools/melee/src/sysdolphin/baselib'))) if h.endswith('.h'))
            + '#include "../../symbols/cr_types.h"\n' + globals_decls())
    open(os.path.join(out, 'm2c_run.py'), 'w', newline='\n').write(
        "import sys\nsys.path.insert(0, %r)\nfrom m2c.main import main\nmain()\n" % M2C.replace('\\', '/'))
    protos = os.path.join(out, 'ctx_protos.h')

    def context(text):
        open(os.path.join(out, 'ctx.h'), 'w', newline='\n').write(text)
        ctxin = os.path.join(out, 'ctx_in.h')
        shutil.copyfile(os.path.join(out, 'ctx.h'), ctxin)
        cmd = mwcc_cmd(ver) + ['-E', '-nodefaults', '-proc', 'gekko', '-nosyspath'] + CTX_DEFS \
            + sum([['-i', i] for i in CTX_INC], []) + ['-i', 'drafts', ctxin]
        rc, so, se = run(cmd, 120, cwd=D)
        if rc != 0:
            sys.exit('context preprocess failed:\n' + so[:2000] + se[:2000])
        # m2c's context parser reads plain C: our own preprocessed text, with the macros it defines itself dropped
        open(os.path.join(out, 'ctx_pp.c'), 'w', encoding='utf-8', newline='\n').write(so)
        # prime m2c's context cache once (workers then only read it)
        run([sys.executable, '-I', os.path.join(out, 'm2c_run.py'), '-t', 'ppc-mwcc-c', '--context', os.path.join(out, 'ctx_pp.c'),
             '-f', funcs[0][0], os.path.join(out, 'asm', funcs[0][0] + '.s')], 300)
        return so

    full = not a.only and not a.sample
    if full or not os.path.exists(protos):
        # pass 1: every function once, without the data declarations, for the signature of its own draft (the callers
        # then see the callee's parameters) and the pointer types m2c infers for 4-byte data objects
        ctx_pp = context(base)
        sigs, ptr = {}, collections.defaultdict(set)
        with cf.ProcessPoolExecutor(a.jobs, initializer=init, initargs=(out, ver, flags, True)) as ex:
            for name, sig, externs, _ in ex.map(one, [(n, ad, sz, o) for n, ad, sz, _, o in funcs], chunksize=4):
                if sig:
                    sigs[name] = (starts[name], sig)
                    for t, lbl in externs:
                        if '*' in t:
                            ptr[lbl].add(t)
        ptrs = {n: (next(iter(ts)) if len(ts) == 1 and ts != {'void *'} else 'u8 *') for n, ts in ptr.items()}
        open(protos, 'w', encoding='utf-8', newline='\n').write(
            protos_text(sigs, ctx_pp) + '\n// data objects m2c typed as pointers in that pass (size 4; tools/draft_batch.py data_decls)\n'
            + ''.join('#define CR_PTR_%s %s\n' % (n, ptrs[n]) for n in sorted(ptrs)))
        print('pass 1: %d prototypes, %d pointer objects from the drafts (%d s)' % (len(sigs), len(ptrs), time.time() - t0))
    ptrs = dict(re.findall(r'^#define CR_PTR_(lbl_[0-9A-F]{8}) (.*)$', open(protos, encoding='utf-8').read(), re.M))
    context(base + data_decls(ptrs) + '\n#include "ctx_protos.h"\n')
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
