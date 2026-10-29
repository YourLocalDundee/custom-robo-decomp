"""Name the remaining sysdolphin (HSD) library functions (0x8016D408 .. end of .text) from independent evidence.

References (read only, never built or run):
  melee/GALE01  tools/melee: config/GALE01/symbols.txt + splits.txt (sizes, translation units),
                src/sysdolphin/baselib/*.c,*.h (asserts, string literals, call lists through header inlines/macros)
  kar/GKYE01    Kirby Air Ride, which links the SAME newer HSD revision as Custom Robo (many sizes identical):
                tools/refs/kar (doldecomp/kar 3ed51a1a): asm/**/*.s = exact code, calls, assert/string data;
                src/sysdolphin/*.c = C for the functions not in the asm; tools/refs/kar_wowjinxy (wowjinxy/KAR
                cb94b157) config/GKYE01/symbols.txt = complete map of the same binary (sizes, more names).
  xd/GXXE01     tools/refs/xd (TeamOrre/xd-decomp 4989794e) config map: layout proof only. Its HSD build differs
                (HSD_SetupPEMode 0x4E0 vs ours 0x174) and its particle code is Genius Sonority's C++.
  Names: only Melee names are ever proposed (Melee symbols, or functions defined in a Melee .c). KAR and XD names
  are used only when they are also Melee names: both KAR projects contain invented names (HSD_StateSetColorDither
  for Melee's HSD_StateSetDither, gobj_allocate, ...). Header inlines (ref_INC, HSD_IDGetData) are never proposed:
  their out-of-line copies exist once per translation unit. Placeholders (fn_, func_, lbl_, kar_, *_<address>) never.
Our side, structure only (by address; no name Ghidra applied is used as a name, the decompiled C is not read):
asserts from main.dol's instruction bytes at the calls to __assert / HSD_Panic (tools/dol_asserts.py),
decompiled/xrefs/{functions,refs,strings}.tsv (calls, string refs, computed jumps),
tools/dtk/proj/symbols.txt (sizes), symbols/symbols.txt (verified names = neighbours; lines tagged
"// lib-candidate" in this range are this script's own earlier output and are ignored), main.dol (string bytes).

Rules (each candidate line says which one fired and against which reference):
  R3 layout proof: between two functions that are consecutive anchors in our binary (named in ours, same name in
     the reference, same order), our gap and the reference gap have the same number of functions and every size
     is within 10%; names are taken only where the reference gap entry has a (Melee) name.
  R4 assert-sequence: the function's own-file __assert / HSD_Panic (file, expression-or-message) set is non-empty and
     equal to that of exactly one reference function, no other function of ours has the same set, and the
     reference address forms no inversion with any other named function of that reference file in our binary (at
     least one such name must exist). Line numbers are ignored. Sets of only '0' / '' also need a same-file named
     neighbour right next to it and a size within 0.75..1.34.
  R5 call-graph: the function's verified callees and callers (>= 2 distinct verified neighbours, not counting
     __assert/OSReport/memcpy/... and runtime save/restore helpers) equal exactly one reference function's callees
     and callers projected onto the verified names (Melee: C call lists expanded through inlines/macros; KAR: exact
     asm/C calls, translated through functions named the same in both), no other function of ours has the same
     projection, the reference function is in the same reference file as the nearest named function before or after
     ours, and no order inversion.
  R6 unique string: the function references a string literal (format / print / error / class-name text, not an
     assert expression or a file name, >= 6 chars) that exactly one reference function uses and exactly one of our
     functions uses; no order inversion against the reference file's named functions.
  Game strings (coordinator request): every range function's own strings, docs/name_evidence.md and
     docs/name_tables.tsv were also checked for the game naming the function itself. A printed prefix
     ("Name: ...") is NOT accepted as a name: the reference shows prefixes naming another function
     ("psInitDataBanks: unknown version" is printed by psInitDataBankLoad), so such prefixes are listed in
     lib_unnamed_hsd.txt as hints only.
  Sanity for every name: size ratio ours / that reference within 0.5 .. 2.0 (R3: 0.9 .. 1.1); the name occurs once
  in the reference; never C++ mangled; never already used in symbols/symbols.txt; one name -> one address; two
  references proposing different names for one function = no name.
Order independence: proposals of all rules and references are collected per round before any is accepted; after
the fixpoint every name is re-checked against the FINAL name set (it must still be proposed by some rule/reference
with the uniqueness checks above), unsupported names are dropped and dropped names that are supported again are
restored, until stable. Permuting the rule or reference order gives byte-identical output.

Writes symbols/candidates_hsd.txt (symbols.txt format) and symbols/lib_unnamed_hsd.txt (every function of the
range still unnamed, with the reason). Deterministic; reads repository / reference files only.
usage: python -I tools/match_lib_hsd.py
"""
import os, re, struct, sys
from collections import defaultdict

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
import dol_asserts  # noqa: E402  assert calls from the instruction bytes

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
P = lambda *a: os.path.join(G, *a)
LO = 0x8016D408
MEL = P('tools/melee')
MEL_SRC = P('tools/melee/src/sysdolphin/baselib')
REF = 'melee/GALE01'
MANGLED = re.compile(r'^[^_].*__\w')
GENERIC = ('fn_', 'lbl_', 'jumptable_', '@', 'gap_', 'FUN_', 'func_', 'sub_')
KEYWORDS = {'if', 'for', 'while', 'switch', 'return', 'sizeof', 'do', 'else', 'case', 'defined', 'ATTRIBUTE_NORETURN'}


# ---------------------------------------------------------------- our side
class Dol:
    def __init__(self, path):
        self.d = open(path, 'rb').read()
        self.off = struct.unpack_from('>18I', self.d, 0)
        self.adr = struct.unpack_from('>18I', self.d, 0x48)
        self.sz = struct.unpack_from('>18I', self.d, 0x90)

    def cstr(self, addr):
        for o, a, s in zip(self.off, self.adr, self.sz):
            if s and a <= addr < a + s:
                f = o + addr - a
                e = self.d.find(b'\0', f)
                return self.d[f:e].decode('latin-1')
        return None


def norm(s): return re.sub(r'\s+', '', s)


def load_syms(path):
    out = []
    for l in open(path, encoding='utf-8'):
        m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function(?: size:0x([0-9A-Fa-f]+))?', l)
        if m: out.append((int(m.group(2), 16), int(m.group(3), 16) if m.group(3) else 0, m.group(1)))
    return sorted(out)


dol = Dol(P('tools/dtk/proj/main.dol'))
dtk_size = {a: s for a, s, n in load_syms(P('tools/dtk/proj/symbols.txt'))}
verified = {}  # addr -> name
for l in open(P('symbols/symbols.txt'), encoding='utf-8'):
    # our own earlier output, once merged into symbols.txt, is not evidence for itself
    m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function', l)
    if m and not ('// lib-candidate' in l and int(m.group(2), 16) >= LO): verified[int(m.group(2), 16)] = m.group(1)
used_names = set(verified.values())
funcs = []  # (addr, size) all functions of ours
for l in open(P('decompiled/xrefs/functions.tsv'), encoding='utf-8'):
    a, n, s = l.rstrip('\n').split('\t')[:3]
    a = int(a, 16); funcs.append((a, dtk_size.get(a) or int(s)))
funcs.sort()
fstart = {a for a, s in funcs}
fsize = dict(funcs)
range_funcs = [a for a, s in funcs if a >= LO]

str_addrs = set()
for l in open(P('decompiled/xrefs/strings.tsv'), encoding='utf-8'):
    str_addrs.add(int(l.split('\t')[0], 16))
callees, callers = defaultdict(set), defaultdict(set)
strs_of = defaultdict(set)  # func -> string addrs
for l in open(P('decompiled/xrefs/refs.tsv'), encoding='utf-8'):
    f, at, to, kind = l.rstrip('\n').split('\t')[:4]
    f, to = int(f, 16), int(to, 16)
    if kind in ('UNCONDITIONAL_CALL', 'UNCONDITIONAL_JUMP', 'CONDITIONAL_CALL') and to in fstart and to != f:
        callees[f].add(to); callers[to].add(f)
    elif to in str_addrs:
        strs_of[f].add(to)

# asserts from the instruction bytes (tools/dol_asserts.py): every call to the __assert / HSD_Panic addresses (their
# names in symbols/symbols.txt; call sites from refs.tsv), file = r3 and expression = r5 at the call by constant
# propagation over main.dol. Never the decompiled C, so nothing here depends on the names / prototypes a rebuild applied.
asserts = defaultdict(set)  # func -> {(file, expr)}
assert_strs = set()  # string addresses used as assert file / expression
ASSERT_FUNCS = ('__assert', 'HSD_Panic')
assert_at = set()
for l in open(P('symbols/symbols.txt'), encoding='utf-8'):
    m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function', l)
    if m and m.group(1) in ASSERT_FUNCS: assert_at.add(int(m.group(2), 16))
init_regs = None
for l in open(P('tools/dtk/proj/symbols.txt'), encoding='utf-8'):
    m = re.match(r'__init_registers = \.init:0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)', l)
    if m: init_regs = (int(m.group(1), 16), int(m.group(2), 16))
if len(assert_at) != len(ASSERT_FUNCS) or not init_regs:
    sys.exit('match_lib_hsd.py: __assert / HSD_Panic (symbols/symbols.txt) or __init_registers (dtk symbols) not found')
call_sites, computed_jumps = defaultdict(list), defaultdict(list)
for l in open(P('decompiled/xrefs/refs.tsv'), encoding='utf-8'):
    f, at, to, kind = l.rstrip('\n').split('\t')[:4]
    if kind == 'UNCONDITIONAL_CALL' and int(to, 16) in assert_at: call_sites[int(f, 16)].append(int(at, 16))
    elif kind == 'COMPUTED_JUMP': computed_jumps[int(at, 16)].append(int(to, 16))
adol = dol_asserts.Dol(P('tools/dtk/proj/main.dol'))
for f, fa, ea in sorted(dol_asserts.assert_calls(adol, fsize, call_sites, computed_jumps,
                                                 dol_asserts.small_data_bases(adol, init_regs))):
    asserts[f].add((dol.cstr(fa), norm(dol.cstr(ea)))); assert_strs.update((fa, ea))

# ---------------------------------------------------------------- reference side (Melee)
mel = load_syms(P('tools/melee/config/GALE01/symbols.txt'))
mcount = defaultdict(int)
for a, s, n in mel: mcount[n] += 1
msym = {n: (a, s) for a, s, n in mel}
tus = []
for l in open(P('tools/melee/config/GALE01/splits.txt'), encoding='utf-8'):
    if re.match(r'^\S.*:\s*$', l): tu = l.strip()[:-1]
    m = re.match(r'\s+\.text\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)', l)
    if m: tus.append((int(m.group(1), 16), int(m.group(2), 16), tu.split('/')[-1]))


def tu_of(addr):
    for s, e, t in tus:
        if s <= addr < e: return t
    return None


TOK = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*', re.S)


def strip(text):
    lits = []

    def rep(m):
        t = m.group(0)
        if t.startswith('/'): return ' ' if t.startswith('/*') else ''
        if t.startswith('"'):
            lits.append(t[1:-1]); return '"@%d@"' % (len(lits) - 1)
        return '0'
    return TOK.sub(rep, text), lits


def unescape(s):
    return re.sub(r'\\(x[0-9a-fA-F]{1,2}|[0-7]{1,3}|.)', lambda m: {'n': '\n', 't': '\t', 'r': '\r', '0': '\0', '\\': '\\',
                  '"': '"', "'": "'"}.get(m.group(1), chr(int(m.group(1)[1:], 16)) if m.group(1)[0] == 'x' else
                  chr(int(m.group(1), 8)) if m.group(1)[0] in '01234567' else m.group(1)), s)


CALL = re.compile(r'\b([A-Za-z_]\w*)\s*\(')
AMAC = re.compile(r'\b(HSD_ASSERT|HSD_ASSERTMSG|HSD_ASSERTREPORT|HSD_ASSERT2|__assert|HSD_Panic)\s*\(')


def split_args(s, i):
    """s[i] == '(' -> (args, end index)"""
    depth, args, cur = 0, [], ''
    for j in range(i, len(s)):
        c = s[j]
        if c == '(':
            depth += 1
            if depth == 1: continue
        elif c == ')':
            depth -= 1
            if depth == 0: args.append(cur); return args, j
        elif c == ',' and depth == 1:
            args.append(cur); cur = ''; continue
        cur += c
    return args, len(s)


def parse_c(src_dir):
    """C sources -> (functions: name -> file, raw call ids, literals, asserts; function-like macros)"""
    rfuncs = {}   # name -> dict(file, calls(raw ids), strs, asserts)
    macros = {}   # name -> raw call ids
    for fn in sorted(os.listdir(src_dir)):
        if not fn.endswith(('.c', '.h')): continue
        raw = open(os.path.join(src_dir, fn), encoding='utf-8', errors='replace').read()
        raw = raw.replace('\\\r\n', ' ').replace('\\\n', ' ')
        code, lits = strip(raw)
        lines = []
        for ln in code.split('\n'):
            m = re.match(r'\s*#\s*define\s+(\w+)\(([^)]*)\)(.*)', ln)
            if m: macros.setdefault(m.group(1), set(CALL.findall(m.group(3))) - KEYWORDS)
            lines.append('' if ln.lstrip().startswith('#') else ln)
        code = '\n'.join(lines)
        depth, last, i = 0, 0, 0
        while i < len(code):
            c = code[i]
            if c == '{' and depth == 0:
                hdr = code[last:i]
                m = re.search(r'\b([A-Za-z_]\w*)\s*\(([^{};]*)\)\s*$', hdr)
                start = i; d = 0
                while i < len(code):
                    if code[i] == '{': d += 1
                    elif code[i] == '}':
                        d -= 1
                        if d == 0: break
                    i += 1
                body = code[start:i + 1]
                if m and '=' not in hdr.split(m.group(1))[0] and not re.match(r'\s*(typedef|struct|union|enum)\b', hdr):
                    name = m.group(1)
                    ent = {'file': fn, 'calls': set(CALL.findall(body)) - KEYWORDS, 'strs': set(), 'asserts': set()}
                    for am in AMAC.finditer(body):
                        args, _ = split_args(body, am.end() - 1)
                        k = am.group(1)
                        if k in ('HSD_ASSERT', 'assert_line', 'HSD_ASSERTREPORT') and len(args) >= 2:
                            e = args[1]
                            e = re.sub(r'"@(\d+)@"', lambda x: '"' + lits[int(x.group(1))] + '"', e)
                            ent['asserts'].add((fn, norm(e)))
                        elif len(args) >= 3:
                            e = args[2] if k != 'HSD_ASSERT2' else args[1]
                            mm = re.fullmatch(r'\s*"@(\d+)@"\s*', e)
                            if mm: ent['asserts'].add((fn, norm(unescape(lits[int(mm.group(1))]))))
                    for sm in re.finditer(r'"@(\d+)@"', body):
                        ent['strs'].add(unescape(lits[int(sm.group(1))]))
                    if name not in rfuncs or fn.endswith('.c'): rfuncs[name] = ent
                last = i + 1
            elif c == ';' and depth == 0:
                last = i + 1
            i += 1

    return rfuncs, macros


rfuncs, macros = parse_c(MEL_SRC)

real = {n for n in msym}  # names with an address in the reference


def expand(name, seen=None, rfuncs=rfuncs, macros=macros, real=real):
    """real callees of a reference function / inline / macro"""
    seen = seen if seen is not None else set()
    out, extra_asserts, extra_strs = set(), set(), set()
    ids = rfuncs[name]['calls'] if name in rfuncs else macros.get(name, set())
    for c in ids:
        if c in seen or c == name: continue
        if c in real: out.add(c); continue
        if c in rfuncs or c in macros:
            seen.add(c)
            o, a, s = expand(c, seen, rfuncs, macros, real)
            out |= o; extra_asserts |= a; extra_strs |= s
            if c in rfuncs:
                extra_asserts |= rfuncs[c]['asserts']; extra_strs |= rfuncs[c]['strs']
    return out, extra_asserts, extra_strs


ref = {}  # real reference function name -> info
for n, e in rfuncs.items():
    if n not in real or not e['file'].endswith('.c'): continue
    a, s = msym[n]
    t = tu_of(a)
    if t is None or t != e['file']: continue  # definition must sit in its own translation unit
    c, xa, xs = expand(n)
    ref[n] = {'addr': a, 'size': s, 'tu': t, 'callees': c, 'own_asserts': e['asserts'],
              'strs': e['strs'] | xs, 'own_strs': e['strs']}
rcallers = defaultdict(set)
for n, r in ref.items():
    for c in r['callees']: rcallers[c].add(n)
str_users = defaultdict(set)  # literal -> reference functions using it directly
for n, r in ref.items():
    for s in r['own_strs']: str_users[s].add(n)




# ---------------------------------------------------------------- reference databases
# Every reference is a DB: name -> {addr, size, tu, callees (names), own_asserts, own_strs}, plus 'seq' (all
# functions in address order, real name or None) for layout proof.
# Melee symbols, plus functions defined in a Melee .c (file statics); never header inlines (ref_INC, HSD_IDGetData,
# ...): their out-of-line copies exist once per translation unit, so the name does not identify one function
MELEE_NAMES = {n for n in msym if mcount[n] == 1} | {n for n, e in rfuncs.items() if mcount.get(n, 0) <= 1 and e['file'].endswith('.c')}
MELEE_NAMES = {n for n in MELEE_NAMES if not MANGLED.match(n) and not n.startswith(GENERIC)}


def make_db(tag, f, seq):
    db = {'tag': tag, 'f': f, 'seq': seq, 'callers': defaultdict(set), 'str_users': defaultdict(set),
          'idx': {n: i for i, (a, s, n) in enumerate(seq) if n}}
    for n, r in f.items():
        for c in r['callees']: db['callers'][c].add(n)
        for s in r['own_strs']: db['str_users'][s].add(n)
    return db


DBS = [make_db(REF, ref, [(a, s, n if (mcount[n] == 1 and n in MELEE_NAMES) else None) for a, s, n in mel])]
DBS[0]['size'] = {n: r['size'] for n, r in ref.items()}
for a, s, n in mel:
    if n in MELEE_NAMES and mcount[n] == 1: DBS[0]['size'].setdefault(n, s)

# --- Kirby Air Ride (doldecomp/kar asm = exact code + calls; wowjinxy/KAR map = names for the same addresses).
# KAR links the same (newer) HSD revision as Custom Robo. A KAR name is used only when it is also a Melee name
# (doldecomp/kar and wowjinxy/KAR both contain invented names, e.g. HSD_StateSetColorDither for Melee's
# HSD_StateSetDither); two different Melee names for one KAR address = no name.
KAR_ASM = P('tools/refs/kar/asm')
KAR_WJ = P('tools/refs/kar_wowjinxy/config/GKYE01/symbols.txt')


def parse_kar():
    lab = re.compile(r'^([A-Za-z_@$][\w@$.]*):\s*$')
    ins = re.compile(r'^/\* ([0-9A-F]{8}) [0-9A-F]{8}  [0-9A-F ]{11} \*/\s*(\w[\w.+-]*)\s*(.*)$')
    data, fl = {}, []
    for root, ds, fs in sorted(os.walk(KAR_ASM)):
        ds.sort()
        for fn in sorted(fs):
            if not fn.endswith('.s'): continue
            path = os.path.join(root, fn)
            sec, cur, buf, gl = None, None, None, set()
            for line in open(path, encoding='latin-1'):
                s = line.strip()
                if s.startswith('.section'): sec = s.split()[1].rstrip(','); cur = None; continue
                if s.startswith('.global'): gl.add(s.split()[1]); continue
                m = lab.match(s)
                if m:
                    nm = m.group(1); text = bool(sec and sec.startswith('.text'))
                    if text and nm in gl and not nm.startswith('lbl_'):
                        cur = {'name': nm, 'file': os.path.relpath(path, KAR_ASM).replace(os.sep, '/'), 'start': None,
                               'end': None, 'ins': []}
                        fl.append(cur)
                    mm = re.match(r'lbl_([0-9A-F]{8})$', nm)
                    buf = None
                    if mm and not text: buf = bytearray(); data[int(mm.group(1), 16)] = buf
                    continue
                m = ins.match(s)
                if m and cur is not None:
                    a = int(m.group(1), 16)
                    if cur['start'] is None: cur['start'] = a
                    cur['end'] = a + 4; cur['ins'].append((m.group(2), m.group(3)))
                    continue
                if buf is not None:
                    try:
                        if s.startswith(('.asciz', '.string')):
                            t = s.split(None, 1)[1].strip()[1:-1]
                            buf += t.encode('latin-1').decode('unicode_escape').encode('latin-1') + b'\0'
                        elif s.startswith('.4byte'):
                            for v in s.split(None, 1)[1].split(','):
                                v = v.strip()
                                buf += struct.pack('>I', int(v, 16)) if v.startswith('0x') else b'\xff' * 4
                        elif s.startswith('.byte'):
                            for v in s.split(None, 1)[1].split(','): buf += bytes([int(v.strip(), 0) & 0xff])
                        elif s.startswith('.2byte'):
                            for v in s.split(None, 1)[1].split(','): buf += struct.pack('>H', int(v.strip(), 0) & 0xffff)
                    except (ValueError, IndexError):
                        buf = None
    fl = [x for x in fl if x['start'] is not None]

    def cstr(addr):
        for base in (addr,):
            if base in data:
                b = data[base]; e = b.find(b'\0')
                if e > 0: return b[:e].decode('latin-1')
        return None
    return fl, cstr


kar_db = None
KAR_SRC = P('tools/refs/kar/src/sysdolphin')
if os.path.isdir(KAR_ASM):
    kfl, kcstr = parse_kar()
    asm_at = {x['start']: x for x in kfl}
    # function universe: wowjinxy's complete map of the same binary (sizes), plus asm functions it lacks
    univ = {}
    if os.path.exists(KAR_WJ):
        for a, sz, n in load_syms(KAR_WJ):
            if sz: univ[a] = [sz, n]
    for x in kfl: univ.setdefault(x['start'], [x['end'] - x['start'], x['name']])
    kc = defaultdict(int)
    for x in kfl: kc['asm:' + x['name']] += 1
    for a, (sz, n) in univ.items(): kc['wj:' + n] += 1
    addr_of = {}
    for a, (sz, n) in univ.items():
        if kc['wj:' + n] == 1: addr_of[n] = a
    for x in kfl:
        if kc['asm:' + x['name']] == 1: addr_of.setdefault(x['name'], x['start'])
    kname = {}
    for a, (sz, n) in univ.items():
        c = set()
        if kc['wj:' + n] == 1: c.add(n)
        if a in asm_at and kc['asm:' + asm_at[a]['name']] == 1: c.add(asm_at[a]['name'])
        c = {x for x in c if x in MELEE_NAMES}
        kname[a] = c.pop() if len(c) == 1 else None
    kseq = [(a, univ[a][0], kname[a]) for a in sorted(univ)]
    # KAR C sources (functions not in the asm): calls from the C, expanded through KAR + Melee inlines/macros
    kr, km = parse_c(KAR_SRC) if os.path.isdir(KAR_SRC) else ({}, {})
    kmac = dict(macros); kmac.update(km)
    kreal = set(addr_of)
    info = {}  # addr -> callees_a, own asserts, strings, tu
    for x in kfl:
        callees_a, regs, asr, ss = set(), {}, set(), set()
        for op, args in x['ins']:
            if op in ('bl', 'b'):
                t = args.split(',')[0].strip()
                ta = addr_of.get(t)
                if ta is None:
                    mm = re.match(r'func_([0-9A-F]{8})$', t)
                    ta = int(mm.group(1), 16) if mm else None
                if t in ('__assert', 'HSD_Panic') and 'r3' in regs and 'r5' in regs:
                    fs_, es_ = kcstr(regs['r3']), kcstr(regs['r5'])
                    if fs_ and es_ is not None: asr.add((fs_, norm(es_)))
                if ta is not None and ta != x['start'] and ta in univ: callees_a.add(ta)
                continue
            mm = re.search(r'lbl_([0-9A-F]{8})@(l|sda21)', args)
            if mm:
                r = args.split(',')[0].strip(); la = int(mm.group(1), 16); regs[r] = la
                t = kcstr(la)
                if t and len(t) >= 2 and all(32 <= ord(ch) < 127 or ch in '\n\t' for ch in t): ss.add(t)
        info[x['start']] = (callees_a, {(f_, e) for f_, e in asr if f_.endswith('.c')},
                            ss - {e for _, e in asr} - {f_ for f_, _ in asr}, 'asm/' + x['file'])
    for n, e in kr.items():
        if not e['file'].endswith('.c') or n not in addr_of: continue
        a = addr_of[n]
        if a in info: continue
        c, xa, xs = expand(n, None, kr, kmac, kreal)
        info[a] = ({addr_of[x] for x in c if x in addr_of and addr_of[x] != a}, set(e['asserts']),
                   set(e['strs']) - {ee for _, ee in e['asserts']}, 'src/' + e['file'])
    kf, ksize = {}, {}
    for a, (sz, n) in univ.items():
        nm = kname[a]
        if not nm: continue
        ksize[nm] = sz
        if a not in info: continue
        ca, own, ss, tu = info[a]
        kf[nm] = {'addr': a, 'size': sz, 'tu': tu, 'callees_a': ca, 'callees': {kname[c] for c in ca if kname.get(c)},
                  'own_asserts': own, 'own_strs': ss}
    kar_db = make_db('kar/GKYE01', kf, kseq)
    kar_db['size'] = ksize
    kar_db['kname'] = kname
    kar_db['kcallers_a'] = defaultdict(set)
    for a, (ca, own, ss, tu) in info.items():
        for c in ca: kar_db['kcallers_a'][c].add(a)
    DBS.append(kar_db)

# --- Pokemon XD map: layout proof only (no HSD source in that repo). XD's HSD build differs (HSD_SetupPEMode 0x4E0
# vs ours 0x174) and its particle code is Genius Sonority's C++ (GSAPI/particle/psdisp.cpp), so only Melee names
# are taken from it and only from 1:1 gaps with every size within 10%.
XD_MAP = P('tools/refs/xd/config/GXXE01/symbols.txt')
if os.path.exists(XD_MAP):
    xs = load_syms(XD_MAP); xc = defaultdict(int)
    for a, sz, n in xs: xc[n] += 1
    xseq = [(a, sz, n if (xc[n] == 1 and n in MELEE_NAMES) else None) for a, sz, n in xs if sz]
    xdb = make_db('xd/GXXE01', {}, xseq)
    xdb['size'] = {n: sz for a, sz, n in xseq if n}
    DBS.append(xdb)

# ---------------------------------------------------------------- matching state
names = dict(verified)  # addr -> name (verified + accepted)
accepted = {}  # addr -> (name, rule, db tag, why)
banned = set()  # (addr, name) dropped by a final check
dropinfo = {}
order_cache = None


def named_set(): return set(names.values())


def sane(db, name, addr, tol=(0.5, 2.0)):
    if name not in MELEE_NAMES or MANGLED.match(name) or name.startswith(GENERIC): return False
    if name in used_names or (addr, name) in banned: return False
    sz = db['size'].get(name)
    return bool(sz) and tol[0] <= fsize[addr] / sz <= tol[1]


def no_inversion(db, addr, name, need_neighbour, nm):
    r = db['f'][name]; t, ra = r['tu'], r['addr']
    others = [(a, db['f'][n]['addr']) for a, n in nm.items() if a != addr and n in db['f'] and db['f'][n]['tu'] == t]
    if need_neighbour and not others: return False
    return all((a < addr) == (x < ra) for a, x in others)


ours_by_text = defaultdict(set)
for f_, ss in strs_of.items():
    for s in ss:
        if s not in assert_strs: ours_by_text[dol.cstr(s)].add(f_)
own_asserts_of = {a: {x for x in asserts.get(a, ()) if x[0].endswith('.c')} for a in range_funcs}

RUNTIME = re.compile(r'^(_savegpr|_restgpr|_savefpr|_restfpr|__save_|__restore_|__cvt|__div|__mod|__shr|__shl|__ptmf|__va_arg|__construct|__destroy)')
# helpers called from almost everywhere: they still have to agree, but do not count towards the 2 neighbours
COMMON = {'__assert', 'HSD_Panic', 'OSReport', 'OSPanic', 'memcpy', 'memset', 'OSDisableInterrupts',
          'OSRestoreInterrupts', 'OSRoundUp32B', 'OSRoundDown32B', 'strcmp', 'strlen', 'sprintf', 'printf'}


def our_nb(a):
    ce = {names[x] for x in callees.get(a, ()) if x in names and not RUNTIME.match(names[x])}
    cr = {names[x] for x in callers.get(a, ()) if x in names}
    return ce, cr


def side_tus(db, a):
    import bisect
    order = sorted(names); i = bisect.bisect_left(order, a)
    return {db['f'][names[order[j]]]['tu'] for j in (i - 1, i) if 0 <= j < len(order) and names[order[j]] in db['f']}


def r4(db):
    by_tu = defaultdict(list)
    for n, r in db['f'].items():
        if r['own_asserts']: by_tu[frozenset(f_ for f_, _ in r['own_asserts'])].append(n)
    out = {}
    for a in range_funcs:
        if a in names: continue
        own = own_asserts_of[a]
        files = {f_ for f_, e in own}
        if not own or len(files) != 1: continue
        c = [n for n in by_tu.get(frozenset(files), ()) if db['f'][n]['own_asserts'] == own]
        if len(c) != 1: continue
        weak = all(len(e) <= 1 or e in ('NULL', 'FALSE') for _, e in own)
        if weak:  # only generic expressions ('0', ''): also need a same-file named neighbour next to it, size 0.75..1.34
            if db['f'][c[0]]['tu'] not in side_tus(db, a) or not sane(db, c[0], a, (0.75, 1.34)): continue
        f_ = next(iter(files))
        out[a] = (c[0], 'R4', 'asserts %s: %s' % (f_, ' | '.join(sorted(repr(e) for _, e in own))[:160]))
    return out


def r6(db):
    out = {}
    for a in range_funcs:
        if a in names: continue
        hits = []
        for s in strs_of.get(a, ()):
            if s in assert_strs: continue
            t = dol.cstr(s)
            if t is None or len(t.strip()) < 6 or re.fullmatch(r'\w+\.[ch]', t): continue
            if len(ours_by_text.get(t, ())) != 1: continue
            u = db['str_users'].get(t, set())
            if len(u) == 1: hits.append((next(iter(u)), s, t))
        if len({h[0] for h in hits}) == 1:
            n, s, t = hits[0]
            out[a] = (n, 'R6', 'string 0x%08X %r used only by %s in %s' % (s, t[:60], n, db['f'][n]['tu']))
    return out


def kar_corr():
    """our addr <-> KAR addr for every function named the same in both (KAR name = Melee name)"""
    k2o = {}
    kn = {n: a for a, n in kar_db['kname'].items() if n}
    for a, n in names.items():
        if n in kn: k2o[kn[n]] = a
    return k2o


def ref_projection(db, n, vn, k2o=None):
    if db is kar_db:  # KAR asm calls are exact: translate through name correspondence (incl. unnamed KAR SDK code)
        r = db['f'][n]
        ce = {names[k2o[c]] for c in r['callees_a'] if c in k2o and not RUNTIME.match(names[k2o[c]])}
        cr = {names[k2o[c]] for c in kar_db['kcallers_a'][r['addr']] if c in k2o}
        return ce, cr
    r = db['f'][n]
    return {x for x in r['callees'] if x in vn}, {x for x in db['callers'][n] if x in vn}


def r5_matches(db, a, vn, k2o):
    ce, cr = our_nb(a)
    return [n for n in db['f'] if ref_projection(db, n, vn, k2o) == (ce, cr)]


def r5(db):
    vn = named_set(); k2o = kar_corr() if db is kar_db else None
    proj = {n: ref_projection(db, n, vn, k2o) for n in db['f']}
    inv = defaultdict(list)
    for n, p in proj.items(): inv[(frozenset(p[0]), frozenset(p[1]))].append(n)
    out = {}
    for a in range_funcs:
        if a in names: continue
        ce, cr = our_nb(a)
        if len((ce | cr) - COMMON) < 2: continue
        m = inv.get((frozenset(ce), frozenset(cr)), [])
        if len(m) == 1 and db['f'][m[0]]['tu'] in side_tus(db, a):
            out[a] = (m[0], 'R5', 'callees {%s} callers {%s} = %s in %s' % (','.join(sorted(ce)), ','.join(sorted(cr)), m[0], db['f'][m[0]]['tu']))
    return out


def r3(db):
    """layout proof: between two consecutive (in our binary) functions known in both, the same number of
    functions in both, every size within 10%; names only where the reference has a real (Melee) name"""
    seq, idx = db['seq'], db['idx']
    ours = [a for a, s in funcs]
    pos = {a: i for i, a in enumerate(ours)}
    anchors = [a for a in ours if a in names and names[a] in idx]
    out = {}
    for x, y in zip(anchors, anchors[1:]):
        i, j = idx[names[x]], idx[names[y]]
        og = ours[pos[x] + 1:pos[y]]
        rg = seq[i + 1:j]
        if not og or len(og) != len(rg) or j < i: continue
        if any(not (0.9 * rs <= fsize[o] <= 1.1 * rs) for o, (ra, rs, rn) in zip(og, rg)): continue
        if any(o in names and rn != names[o] for o, (ra, rs, rn) in zip(og, rg)): continue
        if any(rn and rn in used_names and names.get(o) != rn for o, (ra, rs, rn) in zip(og, rg)): continue
        for o, (ra, rs, rn) in zip(og, rg):
            if o >= LO and o not in names and rn:
                out[o] = (rn, 'R3', 'layout %s..%s (%d in gap), ref size 0x%X' % (names[x], names[y], len(og), rs))
    return out


RULES = [('R4', r4), ('R6', r6), ('R5', r5), ('R3', r3)]


RULE_PRI = {'R4': 0, 'R6': 1, 'R5': 2, 'R3': 3}  # only picks which evidence is quoted; all are collected first


def fixpoint():
    while True:
        props = defaultdict(list)
        for db in DBS:
            for k, rule in RULES:
                for a, (n, kk, why) in rule(db).items():
                    if sane(db, n, a, (0.9, 1.1) if kk == 'R3' else (0.5, 2.0)):
                        props[a].append((RULE_PRI[kk], db['tag'], n, kk, why, db))
        new = {a: sorted(v, key=lambda x: x[:2]) for a, v in props.items() if len({x[2] for x in v}) == 1}
        cnt = defaultdict(int)
        for a, v in new.items(): cnt[v[0][2]] += 1
        new = {a: v for a, v in new.items() if cnt[v[0][2]] == 1 and v[0][2] not in used_names}
        trial = dict(names); trial.update({a: v[0][2] for a, v in new.items()})
        keep = {}
        for a, v in new.items():
            ok = [x for x in v if x[3] == 'R3' or no_inversion(x[5], a, x[2], x[3] in ('R4', 'R5'), trial)]
            if ok: keep[a] = ok[0]
        if not keep: return
        for a, (pri, tag, n, kk, why, db) in keep.items():
            names[a] = n; accepted[a] = (n, kk, tag, why, db); used_names.add(n)


def support(a, n):
    """evidence for name n at a against the current name set (a itself treated as unnamed): the first rule /
    reference that proposes exactly n here, with the name unique on our side too (no other function of ours has
    the same call graph (R5) or the same assert set (R4)). None = no evidence."""
    had = names.pop(a, None)
    if had: used_names.discard(had)
    ban = (a, n) in banned
    banned.discard((a, n))
    try:
        vn = named_set(); k2o = kar_corr() if kar_db else None
        for k, rule in sorted(RULES, key=lambda r: RULE_PRI[r[0]]):
            for db in sorted(DBS, key=lambda d: d['tag']):
                res = rule(db).get(a)
                if not res or res[0] != n: continue
                if not sane(db, n, a, (0.9, 1.1) if k == 'R3' else (0.5, 2.0)): continue
                if k == 'R5':
                    p_ = ref_projection(db, n, vn, k2o if db is kar_db else None)
                    if [b for b in range_funcs if b not in verified and b != a and our_nb(b) == p_]: continue
                if k == 'R4':
                    own = db['f'][n]['own_asserts']
                    if [b for b in range_funcs if b not in verified and b != a and own_asserts_of[b] == own]: continue
                if k != 'R3':
                    trial = dict(names); trial[a] = n
                    if not no_inversion(db, a, n, k in ('R4', 'R5'), trial): continue
                return (n, k, db['tag'], res[2], db)
        return None
    finally:
        if ban: banned.add((a, n))
        if had: names[a] = had; used_names.add(had)


# order independence: after the greedy fixpoint, every name must have evidence against the FINAL name set; names
# without it are dropped, names dropped earlier that have evidence again are restored; repeat until stable.
for it in range(20):
    fixpoint()
    changed = False
    for a in sorted(accepted):
        if a not in accepted: continue
        n = accepted[a][0]
        sp = support(a, n)
        if sp is None:
            print('dropped (no evidence against the final names): %s 0x%08X (was %s %s)' % (n, a, accepted[a][1], accepted[a][2]))
            banned.add((a, n)); dropinfo[a] = '%s (was %s %s): no unique evidence against the final name set' % (n, accepted[a][1], accepted[a][2])
            used_names.discard(names.pop(a)); del accepted[a]; changed = True
        else:
            accepted[a] = sp
    for a, n in sorted(banned):
        if a in names or n in used_names: continue
        sp = support(a, n)
        if sp:
            print('restored: %s 0x%08X (%s %s)' % (n, a, sp[1], sp[2]))
            banned.discard((a, n)); dropinfo.pop(a, None)
            names[a] = n; used_names.add(n); accepted[a] = sp; changed = True
    if not changed: break

# ---------------------------------------------------------------- output
with open(P('symbols/candidates_hsd.txt'), 'w', encoding='utf-8', newline='\n') as w:
    for a in sorted(accepted):
        n, k, tag, why, db = accepted[a]
        w.write('%s = .text:0x%08X; // type:function size:0x%X scope:global // %s ref:%s ref_size:0x%X why: %s\n'
                % (n, a, fsize[a], k, tag, db['size'][n], why.replace('\n', '\\n')))


def game_self_name():
    """printed prefixes "Name: ..." (hints only)"""
    pat = re.compile(r'^\s*([A-Za-z_][A-Za-z0-9_]{3,})\s*(?:\(\s*\))?\s*:')
    printed = defaultdict(set)
    for f_, ss in strs_of.items():
        for s in ss:
            m = pat.match(dol.cstr(s) or '')
            if m: printed[m.group(1)].add(f_)
    return {next(iter(fs)): nm for nm, fs in printed.items() if len(fs) == 1}


def reason(a):
    own = own_asserts_of[a]
    allown = asserts.get(a, set())
    ce, cr = our_nb(a)
    ss = [dol.cstr(s) for s in strs_of.get(a, ()) if s not in assert_strs]
    bits = []
    if allown:
        f = sorted({x[0] for x in allown})
        cands = sorted({'%s:%s' % (db['tag'].split('/')[0], n) for db in DBS for n, r in db['f'].items()
                        if r['own_asserts'] and own and r['own_asserts'] == own})
        if not own: bits.append('only header asserts (%s)' % ','.join(f))
        elif not cands: bits.append('assert set (%s) matches no reference function' % ','.join(f))
        elif len(cands) > 1: bits.append('assert set matches %d: %s' % (len(cands), ', '.join(cands[:4])))
        else: bits.append('assert match %s rejected (size/order/neighbour/conflict)' % cands[0])
    else:
        bits.append('no asserts')
    bits.append('strings: %s' % ('none' if not ss else 'not unique in references (%d)' % len(ss)))
    nb = len((ce | cr) - COMMON)
    bits.append('%d specific verified neighbours' % nb + ('' if nb >= 2 else ' (<2)'))
    if nb >= 2:
        vn = named_set(); k2o = kar_corr() if kar_db else None
        m = sorted({'%s:%s' % (db['tag'].split('/')[0], n) for db in DBS for n in r5_matches(db, a, vn, k2o)})
        bits.append('call graph: %s' % ('no reference match' if not m else '%d candidates: %s' % (len(m), ', '.join(m[:4]))))
    if a in dropinfo: bits.append('dropped by final uniqueness check: ' + dropinfo[a])
    h = hints.get(a)
    bits.append('layout: no 1:1 gap with sizes within 10% in Melee/KAR/XD; game strings/name tables checked: '
                + ('printed prefix %r (hint only, not proof)' % h if h else 'none name it'))
    if ss: bits.append('own strings: ' + ' | '.join(sorted(repr(x[:40]) for x in ss if x)[:3]))
    return '; '.join(bits)


hints = game_self_name()
left = [a for a in range_funcs if a not in names]
with open(P('symbols/lib_unnamed_hsd.txt'), 'w', encoding='utf-8', newline='\n') as w:
    w.write('# functions 0x%08X..end of .text still unnamed after tools/match_lib_hsd.py (R3/R4/R5/R6 fixpoint,\n' % LO)
    w.write('# references: Melee src+map, KAR asm+map; XD not used, see script header)\n'
            '# also checked for each: game strings naming a function ("Name: ..."), docs/name_evidence.md, docs/name_tables.tsv\n')
    for a in left:
        w.write('0x%08X size:0x%X  %s\n' % (a, fsize[a], reason(a)))
cnt = defaultdict(int)
for a, v in accepted.items(): cnt[v[1] + ' ' + v[2]] += 1
print('reference functions: %s; range functions: %d; accepted: %d %s; still unnamed: %d'
      % ({db['tag']: len(db['f']) for db in DBS}, len(range_funcs), len(accepted), dict(sorted(cnt.items())), len(left)))
