"""Independent accuracy cross-check of EVERY name (library and game), rerunnable, read-only on all inputs.

The check re-derives evidence for each name from scratch instead of trusting the tag that produced it.

Library names (symbols/symbols.txt, symbols/candidates_{hsd,sdk,rt}.txt):
  size     our size (tools/dtk/proj/symbols.txt) vs every reference symbol map that has the name (ratio reported;
           support: within 10% of some reference; contradiction: function outside 0.5 .. 2.0 of every reference)
  callees  our named callees (decompiled/index.db references, dtk function bounds; names = the merged name set below)
           vs the reference definitions of the name: C/C++ sources (identifiers used in the body) and KAR asm
           (bl / b / @ha / @l operands). Inlining allowed: support = every named callee is used by one definition;
           a callee is still "explained" when it is used by a function the definition calls (two levels, same
           reference project); contradiction = in every definition less than half of our named callees are explained
  callers  for every caller with a library name that has reference definitions: does one of them use the name
           (support: directly; no contradiction: within two levels; contradiction: none of them uses it)
  asserts  file names (*.c) our function passes to __assert / OSPanic vs the source file(s) defining the name
  strings  every other string our function uses vs the string literals and assert-macro arguments of the definitions
           (support: equal after removing whitespace; contradiction: a string of >= 6 characters that exactly one
           OTHER reference function uses, the name's definitions (two levels) never use, and the name has C sources)
  unique   our side: one name at two addresses, or two names at one address (FAIL); reference side: a name defined
           twice in every reference map that has it is reported AMBIG and its size / definitions count as no support
  Producing evidence (not counted again): dtk -> size; Melee-assert -> asserts+size; layout-proof / R3 / hand-identified
  -> size; R4 -> asserts; R5 -> callees+callers; R6 -> strings; C-callsite (R7, matched C's call sites, checked by
  tools/check_c_callsites.py) -> callers; C-define (R8, the definition of a C file that matches its whole object, also checked by
  tools/check_c_callsites.py) -> size.
  Verdict: FAIL = any contradiction; OK = at least one independent supporting kind; WEAK = only the producing evidence.
  Merged name set used for callees / callers: symbols.txt, overridden per address by candidates_*.txt (a candidate
  "replaces:" the shifted layout-proof name there), plus descriptive.txt (game code). Objects / labels: size + unique.

Game names (symbols/descriptive.txt, symbols/cr_globals.txt): the `why:` text is parsed for claims that can be
checked mechanically against main.dol (tools/dtk/proj/main.dol) and the index:
  "entry N [slot S] of <table|cr_g_Global> 0xT"  -> the table word at that index is this function
  "<NAME> is entry N of the game's name table 0xT" / section header "name table 0xT" + the name's game word
                                                 -> the string at table[N] is that word (and the name contains it)
  "called by X" / "called from X" / "called per ... by X"    -> X calls this function (index references)
  "calls X" / "address stored ... by X"           -> this function references X / X references this function
  "slot K of N modes in T" / "also slot K of modes A and B"  -> count / positions in the table
  quoted text "..."                               -> a string in main.dol (and, when the why names a function using
                                                    it, that function references it)
  every 0x8xxxxxxx address / FUN_ / cr_ name mentioned -> exists (function start, named function or mapped section)
  cr_globals datatype T*[N] / T[N]                -> for code-pointer and char* tables: all N words are code / strings
  `// game-real G1 names:0xT[i]@0xS funcs:0xF[i] "STR"` (lines merged by batch 7, tools/match_game_names.py)
                                                 -> the name table word T+4i still points at S, S is still the string
                                                    STR, the function table word F+4i is still this function, the name
                                                    is exactly cr_STR, and T / F are adjacent runs of pointers
  `// game-real G2 string:0xS "STR"`             -> S is still STR, this function references S, the name is cr_<the
                                                    identifier STR starts with>
  (the tag is checked on its own; the rest of the why: by the rules above)
Verdict: FAIL = a checked claim is false; OK = >= 1 substantive claim checked true, none false; UNCHECKABLE = prose only.

Outputs symbols/names_crosscheck.txt (summary line, then one row per name, FAILs first) and docs/names_crosscheck.md.
Exit code 1 when any name FAILs (tools/batches/batch8_crosscheck.ps1 stops the run), 2 when inputs are missing.
usage: python -I tools/crosscheck_names.py [--symbols FILE] [--out FILE.txt] [--md FILE.md]
  --symbols  check another copy of symbols.txt (e.g. `git show HEAD:symbols/symbols.txt`), default symbols/symbols.txt
  --out/--md report paths (default symbols/names_crosscheck.txt, docs/names_crosscheck.md)
"""
import bisect, glob, os, re, sqlite3, struct, sys, warnings
from collections import defaultdict

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
P = lambda *a: os.path.join(G, *a)


def _arg(flag, default):
    return os.path.abspath(sys.argv[sys.argv.index(flag) + 1]) if flag in sys.argv else default


SYMBOLS = _arg('--symbols', P('symbols/symbols.txt'))
OUT_TXT = _arg('--out', P('symbols/names_crosscheck.txt'))
OUT_MD = _arg('--md', P('docs/names_crosscheck.md'))

# called through assert / report / panic macros, or emitted by the compiler: never evidence either way
MACRO_ONLY = {'OSPanic', 'OSReport', '__assert', 'OSHalt', '__OSPanic', 'OSFatal', 'printf', 'OSVReport', 'HSD_Panic',
              '__assert_fail', 'OSErrorLine', 'OSAssert', 'HSD_Report'}
HELPERS = {'__div2i', '__div2u', '__mod2i', '__mod2u', '__shl2i', '__shr2i', '__shr2u', '__cvt_fp2unsigned',
           '__cvt_dbl_usll', '__cvt_sll_flt', '__cvt_ull_flt', '__cvt_dbl_ull', '__cvt_ull_dbl', '__cvt_sll_dbl',
           '__cvt_dbl_ll', '__save_gpr', '__restore_gpr', '__save_fpr', '__restore_fpr', '__ptmf_scall', '__va_arg',
           'memset', 'memcpy', '__fill_mem', '__copy', '_savegpr_14', '_restgpr_14', 'memmove', 'MWTRACE'}
# infrastructure calls that newer library revisions add around the same code (interrupt guards, MSL locks): their
# presence or absence says nothing about which function this is
GUARDS = {'OSDisableInterrupts', 'OSRestoreInterrupts', 'OSEnableInterrupts', '__begin_critical_region',
          '__end_critical_region', '__kill_critical_regions', 'OSLockMutex', 'OSUnlockMutex'}
IGNORE = MACRO_ONLY | HELPERS | GUARDS
GAME_SPECIFIC = {'main'}   # one per game: its body is the game's own code
KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'do', 'else', 'case', 'defined', 'asm', '__asm',
            'catch', 'typeof', '__typeof__', 'alignof', '__alignof__', 'offsetof', 'decltype', 'static_cast',
            'reinterpret_cast', 'const_cast', 'dynamic_cast', 'new', 'delete', 'throw', 'goto', 'break', 'continue'}
PLACEHOLDER = re.compile(r'^(fn_|lbl_|func_|kar_\w*__|kar_\w*_[0-9a-fA-F]{8}$|FUN_|sub_|zz_|unk_)')
GENERIC = re.compile(r'^(fn_|lbl_|jumptable_|@|gap_|FUN_|func_|sub_|zz_|LAB_|DAT_)')
SRC_EXT = ('.c', '.cpp', '.cp', '.h', '.hpp', '.inc')

# producing evidence per tag (kinds: size callees callers asserts strings)
PRODUCER = {'dtk': {'size'}, 'Melee-assert': {'asserts', 'size'}, 'hand': {'size'}, 'layout-proof': {'size'},
            'R3': {'size'}, 'R4': {'asserts'}, 'R5': {'callees', 'callers'}, 'R6': {'strings'},
            'C-callsite': {'callers'}, 'C-define': {'size'}}


def norm(s): return re.sub(r'\s+', '', s)


# ================================================================ our side
class Dol:
    def __init__(self, path):
        self.d = open(path, 'rb').read()
        off = struct.unpack_from('>18I', self.d, 0)
        adr = struct.unpack_from('>18I', self.d, 0x48)
        sz = struct.unpack_from('>18I', self.d, 0x90)
        self.secs = [(a, s, o, i < 7) for i, (o, a, s) in enumerate(zip(off, adr, sz)) if s]
        self.bss = struct.unpack_from('>2I', self.d, 0xD8)

    def _sec(self, addr):
        for a, s, o, text in self.secs:
            if a <= addr < a + s: return a, s, o, text
        return None

    def mapped(self, addr):
        b, n = self.bss
        return self._sec(addr) is not None or b <= addr < b + n

    def is_text(self, addr):
        s = self._sec(addr)
        return bool(s and s[3])

    def word(self, addr):
        s = self._sec(addr)
        if not s or addr + 4 > s[0] + s[1]: return None
        return struct.unpack_from('>I', self.d, s[2] + addr - s[0])[0]

    def cstr(self, addr, maxlen=512):
        s = self._sec(addr)
        if not s or s[3]: return None
        f = s[2] + addr - s[0]
        e = self.d.find(b'\0', f, f + maxlen)
        if e < 0: return None
        b = self.d[f:e]
        if not b or any(c < 0x20 and c not in (9, 10, 13) or c > 0x7E for c in b): return None
        return b.decode('ascii')


SYMLINE = re.compile(r'^(\S+) = \.(\w+):0x([0-9A-Fa-f]{8}); // type:(\w+)(?: size:0x([0-9A-Fa-f]+))?(.*)$')


def parse_tag(rest, section_hdr):
    """-> (tag, ref) from the trailing comment of a symbols.txt / candidates line."""
    m = re.search(r'// (?:lib-candidate )?(R[3-6]) ref:(\S+)', rest)
    if m: return m.group(1), m.group(2)
    m = re.search(r'// layout-proof ref:(\S+)', rest)
    if m: return 'layout-proof', m.group(1)
    if '// c-callsite ' in rest: return 'C-callsite', ''
    if '// c-define ' in rest: return 'C-define', ''
    if 'melee-assert-match' in rest: return 'Melee-assert', 'melee/GALE01'
    if section_hdr == 'hand': return 'hand', 'melee/GALE01'
    return 'dtk', ''


def load_names():
    recs = []   # dict(name addr kind size src tag ref line why)
    hdr = 'dtk'
    for l in open(SYMBOLS, encoding='utf-8'):
        if l.startswith('// ===='):
            hdr = 'hand' if 'Hand-identified' in l else 'other'
        m = SYMLINE.match(l.rstrip('\n'))
        if not m: continue
        tag, ref = parse_tag(m.group(6), hdr)
        src = 'symbols'
        recs.append(dict(name=m.group(1), sec=m.group(2), addr=int(m.group(3), 16), kind=m.group(4),
                         size=int(m.group(5), 16) if m.group(5) else None, src=src, tag=tag, ref=ref,
                         why=m.group(6), lib=True, local='scope:local' in m.group(6)))
    for p in sorted(glob.glob(P('symbols/candidates_*.txt'))):
        f = os.path.basename(p)[len('candidates_'):-4]
        for l in open(p, encoding='utf-8'):
            m = SYMLINE.match(l.rstrip('\n'))
            if not m: continue
            tag, ref = parse_tag(m.group(6), 'other')
            recs.append(dict(name=m.group(1), sec=m.group(2), addr=int(m.group(3), 16), kind=m.group(4),
                             size=int(m.group(5), 16) if m.group(5) else None, src='cand_' + f, tag=tag, ref=ref,
                             why=m.group(6), lib=True, local='scope:local' in m.group(6)))
    # a symbols.txt line merged by batch6 ("lib-candidate") is the same claim as its candidates line: keep one record
    cand = {(r['name'], r['addr']) for r in recs if r['src'].startswith('cand_')}
    recs = [r for r in recs if not (r['src'] == 'symbols' and 'lib-candidate' in r['why'] and (r['name'], r['addr']) in cand)]
    game = []
    for fn, src in (('symbols/descriptive.txt', 'descriptive'), ('symbols/cr_globals.txt', 'cr_globals')):
        section = ''
        seen = set()
        for l in open(P(fn), encoding='utf-8'):
            if l.startswith('// ----'): section = l
            elif l.startswith('//      ') and section: section += l
            m = SYMLINE.match(l.rstrip('\n'))
            if not m: continue
            key = (m.group(1), m.group(3))
            if key in seen: continue   # an exact duplicate line is one claim
            seen.add(key)
            w = re.search(r'// why: (.*)$', m.group(6))
            dt = re.search(r'datatype:(\S+)', m.group(6))
            game.append(dict(name=m.group(1), sec=m.group(2), addr=int(m.group(3), 16), kind=m.group(4), src=src,
                             why=w.group(1) if w else '', datatype=dt.group(1) if dt else '', section=section,
                             lib=False))
    return recs, game


class Ours:
    def __init__(self):
        self.funcs = {}
        self.objs = {}
        for l in open(P('tools/dtk/proj/symbols.txt'), encoding='utf-8'):
            m = SYMLINE.match(l.rstrip('\n'))
            if not m or not m.group(5): continue
            a, s = int(m.group(3), 16), int(m.group(5), 16)
            (self.funcs if m.group(4) == 'function' else self.objs)[a] = s
        self.starts = sorted(self.funcs)
        con = sqlite3.connect(P('decompiled/index.db'))
        self.gfuncs = {a: (n, s) for a, n, s in con.execute('SELECT addr, name, size FROM funcs')}
        self.gstr = dict(con.execute('SELECT addr, value FROM strings'))
        self.out = defaultdict(set)      # func -> referenced function starts (calls, jumps, pointers)
        self.calls = defaultdict(set)    # func -> called (CALL / JUMP) function starts
        self.inc = defaultdict(set)      # func -> callers
        self.refd = defaultdict(set)     # target -> functions referencing it (any kind)
        self.reft = defaultdict(set)     # func -> every referenced target
        self.strs = defaultdict(set)     # func -> string addresses
        self.rows = []
        for at, tgt, kind in con.execute('SELECT at, target, kind FROM refs ORDER BY at, target, kind'):
            f = self.owner(at)
            if f is None: continue
            self.reft[f].add(tgt)
            self.refd[tgt].add(f)
            if tgt in self.funcs and tgt != f:
                self.out[f].add(tgt)
                if 'CALL' in kind or 'JUMP' in kind:
                    self.calls[f].add(tgt); self.inc[tgt].add(f)
            elif tgt in self.gstr:
                self.strs[f].add(tgt)
        con.close()

    def owner(self, at):
        i = bisect.bisect_right(self.starts, at) - 1
        if i < 0: return None
        a = self.starts[i]
        return a if at < a + self.funcs[a] else None


# ================================================================ reference side
TOKEN = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*|^[ \t]*#[^\n]*(?:\\\n[^\n]*)*',
                   re.S | re.M)
IDENT = re.compile(r'[A-Za-z_]\w*')
CALL = re.compile(r'([A-Za-z_]\w*)\s*\(')
STRREF = re.compile(r'\x01(\d+)\x02')
ASSERTCALL = re.compile(r'\b(\w*(?:assert|ASSERT|Assert)\w*)\s*\(')
PP = re.compile(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|elif|else|endif)\b')
IF0 = re.compile(r'^[ \t]*#[ \t]*if[ \t]+0\b')


def unescape(s):
    try:
        with warnings.catch_warnings():
            warnings.simplefilter('ignore')
            return bytes(s, 'latin-1', 'replace').decode('unicode_escape', 'replace')
    except Exception: return s


def first_branch(text):
    out, stack = [], []
    for l in text.split('\n'):
        m = PP.match(l) if '#' in l else None
        if m:
            k = m.group(1)
            if k.startswith('if'): stack.append(not IF0.match(l))
            elif k == 'else' and stack: stack[-1] = not stack[-1]
            elif k == 'elif' and stack: stack[-1] = False
            elif k == 'endif' and stack: stack.pop()
            out.append('')
            continue
        out.append(l if all(stack) else '')
    return '\n'.join(out)


def def_name(header):
    h = re.sub(r'\)\s*(const|throw\s*\(\s*\))\s*$', ')', header.rstrip())
    if not h.endswith(')'): return None
    d, i = 0, len(h) - 1
    while i >= 0:
        if h[i] == ')': d += 1
        elif h[i] == '(':
            d -= 1
            if d == 0: break
        i -= 1
    if i <= 0: return None
    pre = h[:i].rstrip()
    m = re.search(r'([A-Za-z_]\w*)$', pre)
    if not m or m.group(1) in KEYWORDS: return None
    if '=' in re.sub(r'\([^()]*\)', '', pre): return None
    if not pre[:m.start()].strip() and not re.match(r'^[A-Za-z_]\w*$', pre): return None
    return m.group(1)


def macro_args(text, i):
    """text[i] is just after '(' -> list of top-level argument texts."""
    d, start, args = 1, i, []
    j = i
    n = len(text)
    while j < n and d:
        c = text[j]
        if c in '([{': d += 1
        elif c in ')]}':
            d -= 1
            if d == 0: args.append(text[start:j]); break
        elif c == ',' and d == 1:
            args.append(text[start:j]); start = j + 1
        j += 1
    return args


class Def:
    __slots__ = ('ids', 'lits', 'asserts', 'file', 'root', 'asm', 'partial')

    def __init__(self, ids, lits, asserts, file, root, asm):
        self.ids, self.lits, self.asserts, self.file, self.root, self.asm = ids, lits, asserts, file, root, asm
        # can support a name but never contradict one: asm (KAR names its callees with placeholders or its own
        # invented names), C that still calls placeholder-named functions, and empty / stub bodies
        self.partial = asm or not ids or any(PLACEHOLDER.match(x) for x in ids)


def parse_c(text, defs, rel, root):
    strings = []
    text = first_branch(text)

    def sub(m):
        t = m.group(0)
        if t[0] == '"':
            strings.append(unescape(t[1:-1])); return f'\x01{len(strings) - 1}\x02'
        if t[0] == "'": return '0'
        return '\n' if t.startswith('//') or t.lstrip().startswith('#') else ' '
    text = TOKEN.sub(sub, text)
    blocks(text, 0, len(text), strings, defs, rel, root, 0)


def blocks(text, lo, hi, strings, defs, rel, root, depth):
    pos, start = lo, lo
    brace = re.compile(r'[{};]')
    while True:
        m = brace.search(text, pos, hi)
        if not m: return
        if m.group(0) != '{':
            pos = start = m.end(); continue
        hdr0 = text[start:m.start()].strip()
        if hdr0.endswith('=') and depth < 3:
            # initialised global (dispatch / callback tables): its identifiers count for whoever uses the variable
            d, k = 1, m.end()
            while d:
                n = brace.search(text, k, hi)
                if not n: return
                d += 1 if n.group(0) == '{' else (-1 if n.group(0) == '}' else 0)
                k = n.end()
            vm = re.search(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*=$', hdr0)
            if vm:
                ids = set(IDENT.findall(text[m.end():k - 1])) - KEYWORDS
                defs[vm.group(1)].append(Def(frozenset(ids), frozenset(), frozenset(), rel, root, False))
            pos = start = k
            continue
        d, k = 1, m.end()
        while d:
            n = brace.search(text, k, hi)
            if not n: return
            d += 1 if n.group(0) == '{' else (-1 if n.group(0) == '}' else 0)
            k = n.end()
        header = text[start:m.start()].strip()
        body = text[m.end():k - 1]
        if depth < 3 and (re.search(r'\bnamespace\b[\w\s]*$', header) or re.search(r'\bextern\s*\x01\d+\x02$', header)):
            blocks(text, m.end(), k - 1, strings, defs, rel, root, depth + 1)
        elif header.endswith(')') or re.search(r'\)\s*(const|throw\s*\(\s*\))\s*$', header):
            name = def_name(header)
            if name:
                ids = set(IDENT.findall(body)) - KEYWORDS
                lits = {norm(strings[int(i)]) for i in STRREF.findall(body)}
                asserts = set()
                for am in ASSERTCALL.finditer(body):
                    for a in macro_args(body, am.end()):
                        a = STRREF.sub(lambda x: strings[int(x.group(1))], a)
                        asserts.add(norm(a))
                defs[name].append(Def(frozenset(ids), frozenset(lits), frozenset(asserts), rel, root, False))
        pos = start = k


ASM_LABEL = re.compile(r'^([A-Za-z_][\w$.@]*):\s*$')
ASM_FN = re.compile(r'^\.fn\s+([A-Za-z_][\w$.@]*)')
ASM_OPS = re.compile(r'\b(?:bl|b|bne|beq|blt|bgt|ble|bge|lis|addi|ori|li|la|lwz|stw)\b[^\n]*?([A-Za-z_][\w$.]*)(?:@ha|@l|@h)?\s*$')
ASM_SYM = re.compile(r'([A-Za-z_][\w$.]*)(?:@ha|@l|@h|@sda21)?')


def parse_asm(text, defs, rel, root):
    cur, ids = None, set()

    def flush():
        if cur and not GENERIC.match(cur):
            defs[cur].append(Def(frozenset(ids), frozenset(), frozenset(), rel, root, True))
    for l in text.split('\n'):
        m = ASM_LABEL.match(l) or ASM_FN.match(l)
        if m and not m.group(1).startswith(('lbl_', '.L', 'jumptable_', '@')):
            flush(); cur, ids = m.group(1), set(); continue
        if cur is None: continue
        if '*/' in l:
            ins = l.split('*/', 1)[1].strip()
            if not ins: continue
            parts = ins.split(None, 1)
            if len(parts) < 2: continue
            op, args = parts
            if op in ('bl', 'b') or op.startswith(('lis', 'addi', 'ori', 'lwz', 'stw', 'lfs', 'lfd', 'la', 'li')):
                for s in ASM_SYM.findall(args):
                    if not re.match(r'^(r\d+|f\d+|cr\d|lbl_|jumptable_|x[0-9a-fA-F]+$)', s) and not GENERIC.match(s):
                        ids.add(s)
    flush()


class Refs:
    def __init__(self):
        self.maps = []   # (tag, {name: [sizes]})
        paths = [P('tools/melee/config/GALE01/symbols.txt')] + sorted(glob.glob(P('tools/refs/*/config/*/symbols.txt')))
        for p in paths:
            if not os.path.exists(p): continue
            game, cfg = os.path.relpath(p, G).replace(os.sep, '/').split('/config/')
            tag = game.split('/')[-1] + '/' + cfg.split('/')[0]
            mp = defaultdict(list)
            for l in open(p, encoding='utf-8', errors='replace'):
                m = SYMLINE.match(l.rstrip('\n'))
                if m and m.group(5):
                    mp[m.group(1)].append((m.group(4), int(m.group(5), 16)))
            self.maps.append((tag, mp))
        self.defs = defaultdict(list)
        roots = [P('tools/melee')] + sorted(glob.glob(P('tools/refs/*')))
        for r in roots:
            rootname = os.path.basename(r)
            for dp, dn, fn in os.walk(r):
                dn.sort()
                rel = os.path.relpath(dp, G).replace(os.sep, '/')
                if '/config' in '/' + os.path.relpath(dp, r).replace(os.sep, '/') + '/' and False: pass
                for f in sorted(fn):
                    path = os.path.join(dp, f)
                    if f.endswith(SRC_EXT):
                        try: t = open(path, encoding='utf-8', errors='replace').read()
                        except OSError: continue
                        if len(t) > 3_000_000: continue
                        parse_c(t, self.defs, rel + '/' + f, rootname)
                    elif f.endswith('.s') and '/asm' in rel.replace(os.sep, '/'):
                        try: t = open(path, encoding='utf-8', errors='replace').read()
                        except OSError: continue
                        parse_asm(t, self.defs, rel + '/' + f, rootname)
        self.by_root = defaultdict(dict)   # root -> name -> [Def]
        for n, ds in self.defs.items():
            for d in ds: self.by_root[d.root].setdefault(n, []).append(d)
        # string -> reference function names using it (literal or assert argument)
        self.str_users = defaultdict(set)
        for n, ds in self.defs.items():
            for d in ds:
                for s in d.lits | d.asserts:
                    if len(s) >= 6: self.str_users[s].add(n)
        self._exp = {}

    def sizes(self, name, kind):
        out = []
        for tag, mp in self.maps:
            if name in mp:
                xs = [s for k, s in mp[name] if (k == 'function') == (kind == 'function')]
                if xs: out.append((tag, xs))
        return out

    def is_func(self, name):
        return any(name in mp and any(k == 'function' for k, s in mp[name]) for t, mp in self.maps)

    def expand(self, name):
        """per definition: (direct ids, ids within 2 levels of called definitions in the same project, def)"""
        if name in self._exp: return self._exp[name]
        r = []
        for d in self.defs.get(name, ()):
            own = self.by_root[d.root]
            d1 = alias(set(d.ids))
            e = set(d1)
            lv = set(d.ids)
            for _ in range(2):
                nxt = set()
                for h in lv:
                    if h == name: continue
                    for d2 in own.get(h, ()):
                        nxt |= d2.ids
                nxt -= e
                e |= nxt
                lv = nxt
                if len(e) > 20000: break
            r.append((d1, alias(e), d))
        self._exp[name] = r
        return r

    def expand_strs(self, name):
        """strings (literals + assert args) of every definition of name and the definitions it calls (2 levels)"""
        out = []
        for d1, e, d in self.expand(name):
            own = self.by_root[d.root]
            s = set(d.lits | d.asserts)
            for h in e:
                for d2 in own.get(h, ()):
                    s |= d2.lits | d2.asserts
            out.append(s)
        return out


MATHF = {'sin', 'cos', 'tan', 'asin', 'acos', 'atan', 'atan2', 'exp', 'log', 'log10', 'pow', 'sqrt', 'fmod', 'floor',
         'ceil', 'fabs', 'frexp', 'ldexp', 'modf'}
_MTX = re.compile(r'^(?:PS|C_)?(MTX|VEC|QUAT)(\w+)$')


def alias(ids):
    extra = set()
    for x in ids:
        if len(x) > 3 and x.endswith('f') and x[:-1] in MATHF: extra.add(x[:-1])
        m = _MTX.match(x)
        if m: extra |= {m.group(1) + m.group(2), 'PS' + m.group(1) + m.group(2), 'C_' + m.group(1) + m.group(2)}
    return ids | extra


# ================================================================ library check
FILE_RE = re.compile(r'^[\w./\\-]+\.(c|cpp|cp)$')
NAME_RE = re.compile(r'^[\w./\\-]+\.(c|cpp|cp|h|hpp)$')   # any source file name (assert / panic __FILE__)


local_at = {}
claims_at = defaultdict(list)   # addr -> [(name, source, why)] of every library claim


def named_callees(ours, names_at, addr, self_name):
    out = {}
    for t in sorted(ours.out.get(addr, ())):
        n = names_at.get(t)
        if not n or n == self_name or n in IGNORE or n.startswith('cr_') or GENERIC.match(n): continue
        out[n] = t
    return out


def names_at_addr(addrs_of, name):
    s = addrs_of.get(name)
    return next(iter(sorted(s))) if s else None


def def_kind(refs, d1, e, ours_set, self_name, ours, names_at, addrs_of):
    """one reference definition vs our named callees:
      explains  every named callee of ours is used (within two levels)
      addition  shares at least one named callee with ours: another revision of the same function (MetroTRK, MSL)
      swap      shares none, and calls a function we have named that ours neither calls nor inlines (or, for a one-call
                wrapper, calls a function whose reference size cannot be the one ours calls): the shape of a wrong name
      unrelated anything else (another implementation, a C++ overload, no named callees)"""
    if ours_set and ours_set <= e: return 'explains'
    if ours_set & e: return 'addition'   # shares a callee: another revision of the same function (MetroTRK, MSL)
    miss = missing_calls(refs, d1, ours_set, self_name, ours, names_at, addrs_of)
    if miss: return 'swap'
    # a one-call wrapper whose reference callee we have not named: compare that callee's size with the one we call
    # (asin -> __ieee754_asin 0x238 vs ours -> __ieee754_fmod 0x33C is fine; log10 -> __ieee754_log10 0x110 is not)
    if len(ours_set) == 1:
        fx = sorted(x for x in d1 if x != self_name and x not in IGNORE and refs.is_func(x))
        if len(fx) == 1:
            x = next(iter(ours_set))
            a = next(iter(sorted(addrs_of.get(x, ()))), None)
            ys = [sz for t, xs in refs.sizes(fx[0], 'function') for sz in xs]
            if a in ours.funcs and ys and not any(0.5 <= ours.funcs[a] / y <= 2.0 for y in ys if y):
                return 'swap'
    return 'unrelated'


def missing_calls(refs, d1, ours_set, self_name, ours, names_at, addrs_of):
    """functions the reference definition calls directly that we have named (library names) but our function
    neither calls nor contains inlined (none of their own named callees are called either)"""
    miss = []
    for x in sorted(d1):
        if x == self_name or x in IGNORE or x in ours_set or x not in addrs_of or x.startswith('cr_'): continue
        a = next(iter(sorted(addrs_of[x])))
        if a not in ours.funcs: continue
        me = names_at_addr(addrs_of, self_name)
        too_big = me is not None and me in ours.funcs and ours.funcs[a] > 2 * ours.funcs[me]
        own = set(named_callees(ours, names_at, a, x))
        for y in sorted(own):   # two levels, like the reference side
            b = next(iter(sorted(addrs_of.get(y, ()))), None)
            if b is not None: own |= set(named_callees(ours, names_at, b, y))
        if not too_big and (not own or own & ours_set): continue   # may be an inlined copy (a leaf leaves no trace)
        miss.append(x)
    return miss


def check_lib(r, ours, refs, names_at, addrs_of, dol):
    name, addr = r['name'], r['addr']
    sup, con, notes = {}, [], []
    # ---- (d) uniqueness, our side
    others = sorted(a for a in addrs_of.get(name, ()) if a != addr)
    if others and r['local'] and all(local_at.get(a) for a in others):
        notes.append('file-static name, also at ' + ','.join(f'{a:08X}' for a in others) + ' (scope:local in different units)')
        others = []
    if others: con.append('unique: name also at ' + ','.join(f'{a:08X}' for a in others))
    # two names claimed for one address: a symbols.txt name disputed by a candidate, or two candidates disagreeing
    # (a candidate that says "replaces:<this symbols.txt name>" is not itself in conflict)
    for n2, src2, why2 in claims_at.get(addr, ()):
        if n2 == name or src2 == r['src'] and n2 == name: continue
        if r['src'] != 'symbols' and src2 == 'symbols' and f'replaces:{n2}' in r['why']: continue
        if r['kind'] != 'function': continue
        con.append(f'unique: {src2} names this address {n2}')
    # ---- (a) size
    kind = r['kind']
    our_size = ours.funcs.get(addr) if kind == 'function' else ours.objs.get(addr, r['size'])
    if kind == 'function' and our_size is None:
        con.append('not a function start in dtk analysis')
    rs = refs.sizes(name, kind)
    amb = bool(rs) and all(len(xs) > 1 for t, xs in rs)
    if amb: notes.append('AMBIG: defined more than once in every reference map (' + ','.join(t for t, x in rs[:4]) + ')')
    if rs and our_size:
        best = min(((t, s) for t, xs in rs for s in xs), key=lambda ts: abs(our_size / ts[1] - 1) if ts[1] else 9)
        ratio = our_size / best[1] if best[1] else 0
        notes.append(f'size 0x{our_size:X}/{best[0]} 0x{best[1]:X}={ratio:.2f} ({len(rs)} refs)')
        if not amb:
            if abs(ratio - 1) <= 0.10 and (kind != 'function' or our_size >= 0x40 or r['tag'] in ('dtk',)):
                sup['size'] = f'size within 10% of {best[0]}'
            elif kind == 'function' and not (0.5 <= ratio <= 2.0) and our_size > 8:
                con.append(f'size: 0x{our_size:X} vs nearest reference {best[0]} 0x{best[1]:X} (ratio {ratio:.2f})')
    elif not rs:
        notes.append('no reference map has this name')
    if kind != 'function':
        return sup, con, notes
    ex = [] if amb else refs.expand(name)
    has_c = any(not d.asm for _, _, d in ex)
    # ---- (b) callees
    callees = named_callees(ours, names_at, addr, name)
    exc = [x for x in ex if not x[2].partial]   # only complete C bodies can contradict
    if callees and ex and name not in GAME_SPECIFIC:
        cs = set(callees)
        if any(cs <= d1 for d1, e, d in ex):
            sup['callees'] = 'callees ' + ','.join(sorted(cs))
        elif exc:
            fr = max(len(cs & e) / len(cs) for d1, e, d in exc)
            # a newer library revision may ADD calls; a wrong name shows a SWAP: the reference calls a function we have
            # named that ours does not call (nor its callees, as an inlined copy would)
            kinds = [def_kind(refs, d1, e, cs, name, ours, names_at, addrs_of) for d1, e, d in exc]
            if fr < 0.5 and 'swap' in kinds and 'addition' not in kinds:
                miss = sorted(cs - max((e for d1, e, d in exc), key=lambda e: len(cs & e)))
                con.append(f'callees: no reference {name} uses ' + ','.join(f'{m}@{callees[m]:08X}' for m in miss[:4]))
            else:
                notes.append(f'callees partly explained ({fr:.2f}), no swapped callee')
    # ---- (b) callers
    # a caller contradicts only when EVERY caller that has complete reference C contradicts (one caller of another
    # library revision that inlines differently is not enough while another caller agrees)
    bad_callers, good_callers = [], 0
    for f in sorted(ours.inc.get(addr, ())):
        fn = names_at.get(f)
        if not fn or fn == name or fn.startswith('cr_') or GENERIC.match(fn) or name in IGNORE or fn in GAME_SPECIFIC: continue
        fx = refs.expand(fn)
        if not fx: continue
        if any(name in d1 for d1, e, d in fx):
            sup['callers'] = f'caller {fn}'; good_callers += 1
        elif any(name in e for d1, e, d in fx): good_callers += 1
        elif any(not d.partial for d1, e, d in fx):
            fc = set(named_callees(ours, names_at, f, fn))
            kinds = [def_kind(refs, d1, e, fc, fn, ours, names_at, addrs_of) for d1, e, d in fx if not d.partial]
            if 'swap' in kinds and 'addition' not in kinds and 'explains' not in kinds:
                bad_callers.append(f'{fn}@{f:08X}')
            else:
                good_callers += 1   # another revision of the caller: neither for nor against
                notes.append(f'caller {fn} not explained (no swapped callee)')
    if bad_callers and not good_callers:
        con.append(f'callers: ' + ','.join(bad_callers[:4]) + f' call it, no reference definition of them uses {name}')
    elif bad_callers:
        notes.append('callers not explained: ' + ','.join(bad_callers[:4]))
    # ---- (c) asserts / strings
    mystr = sorted({ours_str(ours, dol, s) for s in ours.strs.get(addr, ())} - {None})
    files = {os.path.basename(s) for s in mystr if FILE_RE.match(s)}
    if files and ex:
        deffiles = {os.path.basename(d.file) for _, _, d in ex if not d.asm and d.file.endswith(('.c', '.cpp', '.cp'))}
        if deffiles and not (files & deffiles):
            con.append(f'asserts: file {",".join(sorted(files))} but reference defines {name} in {",".join(sorted(deffiles)[:3])}')
        elif files & deffiles:
            sup['asserts'] = 'assert file ' + ','.join(sorted(files & deffiles))
    if ex:
        xs = refs.expand_strs(name)
        direct = [d.lits | d.asserts for _, _, d in ex]
        for s in mystr:
            if NAME_RE.match(s): continue
            ns = norm(s)
            if len(ns) < 3: continue
            if any(ns in st for st in direct):
                kind_s = 'asserts' if any(ns in d.asserts for _, _, d in ex) else 'strings'
                sup.setdefault(kind_s, f'{kind_s[:-1]} "{s[:40]}"')
            elif has_c and len(ns) >= 6 and not any(ns in st for st in xs):
                users = refs.str_users.get(ns, set()) - {name}
                # a revised message of the name's own (same first 12 characters, e.g. Melee "audio heap overflow.\n"
                # vs ours "audio heap overflow. size = %u\n") is the same text, not another function's
                revised = len(ns) >= 12 and any(x[:12] == ns[:12] for st in xs for x in st)
                if len(users) == 1 and not revised:
                    u = next(iter(users))
                    if not PLACEHOLDER.match(u) and not any(u in e for _, e, _ in ex):
                        con.append(f'strings: "{s[:40]}" is used only by reference {u}')
    return sup, con, notes


def ours_str(ours, dol, a):
    s = dol.cstr(a)
    return s if s is not None else ours.gstr.get(a)


def lib_verdict(r, sup, con):
    prod = PRODUCER.get(r['tag'], set())
    indep = {k: v for k, v in sup.items() if k not in prod}
    if con: return 'FAIL', indep
    return ('OK' if indep else 'WEAK'), indep


# ================================================================ game check
HEXA = re.compile(r'0x(8[0-9A-Fa-f]{7})\b')
FUNREF = re.compile(r'\bFUN_([0-9a-fA-F]{8})\b')
CRREF = re.compile(r'\b(cr_[A-Za-z0-9_]+)')


def check_game_real(tag, name, addr, dol, ours):
    """the `// game-real` tag of a batch-7 line -> (ok, bad)"""
    ok, bad = [], []
    m = re.fullmatch(r'G1 names:0x([0-9A-F]{8})\[(\d+)\]@0x([0-9A-F]{8}) funcs:0x([0-9A-F]{8})\[(\d+)\] "(.*)"', tag)
    if m:
        t, i, sa, f, j, word = int(m.group(1), 16), int(m.group(2)), int(m.group(3), 16), int(m.group(4), 16), int(m.group(5)), m.group(6)
        if i != j: bad.append(f'game-real: name index {i} != function index {j}')
        w = dol.word(t + 4 * i)
        (ok if w == sa else bad).append(f'game-real: 0x{t:08X}[{i}] = ' + (f'{w:08X}' if w is not None else 'unreadable') + ('' if w == sa else f' not {sa:08X}'))
        sv = dol.cstr(sa)
        (ok if sv == word else bad).append(f'game-real: string 0x{sa:08X} = "{sv}"' + ('' if sv == word else f' not "{word}"'))
        (ok if name == 'cr_' + word else bad).append(f'game-real: name {name} ' + ('= cr_ + the string' if name == 'cr_' + word else f'is not cr_{word}'))
        w = dol.word(f + 4 * j)
        (ok if w == addr else bad).append(f'game-real: 0x{f:08X}[{j}] = ' + (f'{w:08X}' if w is not None else 'unreadable') + ('' if w == addr else f' not {addr:08X}'))
        # adjacency: the word right after the name table run / before it is the function table (or the reverse)
        def run_end(base, step, pred):
            a = base
            while pred(dol.word(a + step)): a += step
            return a
        is_str = lambda x: x is not None and dol.cstr(x) is not None
        is_fn = lambda x: x is not None and dol.is_text(x)
        s_lo, s_hi = run_end(t, -4, is_str), run_end(t, 4, is_str)
        f_lo, f_hi = run_end(f, -4, is_fn), run_end(f, 4, is_fn)
        adj = s_hi + 4 == f_lo or f_hi + 4 == s_lo
        same = (s_hi - s_lo) == (f_hi - f_lo) and s_lo == t and f_lo == f
        (ok if adj and same else bad).append(f'game-real: tables 0x{t:08X} ({(s_hi - s_lo) // 4 + 1}) and 0x{f:08X} ({(f_hi - f_lo) // 4 + 1}) '
                                             + ('adjacent, same length' if adj and same else 'not adjacent runs of the same length'))
        return ok, bad
    m = re.fullmatch(r'G2 string:0x([0-9A-F]{8}) "(.*)"', tag)
    if m:
        sa, text = int(m.group(1), 16), m.group(2)
        sv = dol.cstr(sa)
        shown = sv.replace('\n', '\\n').replace('\t', '\\t').replace('\r', '\\r') if sv else None
        (ok if shown and shown.startswith(text) else bad).append(f'game-real: string 0x{sa:08X} ' + ('matches' if shown and shown.startswith(text) else f'= "{shown}"'))
        (ok if sa in ours.reft.get(addr, ()) else bad).append(f'game-real: {addr:08X} ' + ('references' if sa in ours.reft.get(addr, ()) else 'does not reference') + f' 0x{sa:08X}')
        ident = re.match(r'(?:Warning: |Error: )?([A-Za-z_]\w*)\(\)', text)
        (ok if ident and name == 'cr_' + ident.group(1) else bad).append(f'game-real: name {name} vs string ' + (ident.group(1) if ident else '(no identifier)'))
        return ok, bad
    return ok, [f'game-real: unreadable tag "{tag[:60]}"']


def check_game(r, ours, dol, all_names, glob_addr, names_at):
    why, name, addr = r['why'], r['name'], r['addr']
    ok, bad, notes = [], [], []
    isfunc = r['kind'] == 'function'
    gr = None
    if '// game-real ' in why:
        why, gr = why.split('// game-real ', 1)
        why = why.rstrip()
        o, b = check_game_real(gr.strip(), name, addr, dol, ours)
        ok += o; bad += b

    def resolve(tok):
        """FUN_x / 0x8... / cr_name / lib name -> address"""
        m = re.match(r'^FUN_([0-9a-fA-F]{8})$', tok)
        if m: return int(m.group(1), 16)
        m = re.match(r'^0x(8[0-9A-Fa-f]{7})$', tok)
        if m: return int(m.group(1), 16)
        return all_names.get(tok)

    # existence of everything mentioned
    for m in HEXA.finditer(why):
        a = int(m.group(1), 16)
        if not dol.mapped(a): bad.append(f'0x{a:08X} is not in any main.dol section')
    for m in FUNREF.finditer(why):
        a = int(m.group(1), 16)
        if a not in ours.funcs and a not in ours.gfuncs: bad.append(f'FUN_{m.group(1)} is not a function')
    for m in CRREF.finditer(why):
        t = m.group(1).rstrip('_')
        if t not in all_names and t not in TYPE_NAMES and not re.match(r'^cr_g_\w+$', t) and not t.endswith('_'):
            if not any(k.startswith(t) for k in all_names): bad.append(f'{t} is not a known name')
    if isfunc and addr not in ours.funcs and addr not in ours.gfuncs:
        bad.append('address is not a function start')

    # table entries
    for m in re.finditer(r'\bentry (\d+)(?: \(([A-Z_0-9]+)\))?(?: slot (\d+))? of (?:the [\w\s-]*?table (0x[0-9A-Fa-f]{8})|(cr_g_\w+))', why):
        n, word, slot, tab, g = int(m.group(1)), m.group(2), m.group(3), m.group(4), m.group(5)
        t = int(tab, 16) if tab else glob_addr.get(g)
        if t is None: bad.append(f'table {g} unknown'); continue
        idx = n * 2 + int(slot) if slot is not None else n
        w = dol.word(t + 4 * idx)
        what = f'{tab or g}[{idx}]'
        if 'name table' in m.group(0):
            s = dol.cstr(w) if w else None
            continue   # handled below with the quoted word
        if w == addr: ok.append(f'{what} = {addr:08X}')
        else: bad.append(f'{what} = {w:08X} not {addr:08X}' if w is not None else f'{what} unreadable')
        after = re.match(r'\s*\(([A-Z][A-Z_0-9]+)\)', why[m.end():])   # "entry 3 slot 0 of cr_g_CamModeFuncs (CAMMODE_OPENING)"
        if not word and after: word = after.group(1)
        if word:
            nt = section_name_table(r['section'])
            if nt is not None:
                s = dol.cstr(dol.word(nt + 4 * n) or 0)
                if s == word: ok.append(f'name table 0x{nt:08X}[{n}] = "{word}"')
                else: bad.append(f'name table 0x{nt:08X}[{n}] = "{s}" not "{word}"')
            if word not in name: bad.append(f'name {name} does not contain {word}')
    for m in re.finditer(r'(?:command )?name ([A-Z_0-9]+) is entry (\d+) of the game\'s name table (0x[0-9A-Fa-f]{8})', why):
        word, n, t = m.group(1), int(m.group(2)), int(m.group(3), 16)
        w = dol.word(t + 4 * n)
        s = dol.cstr(w) if w else None
        if s == word: ok.append(f'0x{t:08X}[{n}] = "{word}"')
        else: bad.append(f'0x{t:08X}[{n}] = "{s}" not "{word}"')
        if word not in name: bad.append(f'name {name} does not contain {word}')
    m = re.search(r'\bslot (\d) of (\d+) modes in (cr_g_\w+)', why)
    if m:
        k, cnt, t = int(m.group(1)), int(m.group(2)), glob_addr.get(m.group(3))
        n = glob_len(r_global(m.group(3)))
        if t and n:
            c = sum(1 for i in range(k, n, 2) if dol.word(t + 4 * i) == addr)
            (ok if c == cnt else bad).append(f'{m.group(3)}: {c} modes have it in slot {k} (claimed {cnt})')
    m = re.search(r'also slot (\d) of modes? ([\d ,and]+)', why)
    if m:
        t = glob_addr.get('cr_g_CamModeFuncs') if 'CamMode' in why or 'cr_g_CamModeFuncs' in why else None
        if t:
            k = int(m.group(1))
            for mode in map(int, re.findall(r'\d+', m.group(2))):
                w = dol.word(t + 4 * (mode * 2 + k))
                (ok if w == addr else bad).append(f'cr_g_CamModeFuncs[{mode}*2+{k}] = {w:08X}' + ('' if w == addr else f' not {addr:08X}'))

    # call relations
    if isfunc:
        for m in re.finditer(r'\bcalled (?:per [\w ]+ |twice |every frame, )?(?:by|from) ((?:FUN_[0-9a-fA-F]{8}|0x8[0-9A-Fa-f]{7}|cr_\w+|[A-Z]\w+)(?:\s*(?:/|,|and)\s*(?:FUN_[0-9a-fA-F]{8}|0x8[0-9A-Fa-f]{7}|cr_\w+))*)', why):
            for tok in re.findall(r'FUN_[0-9a-fA-F]{8}|0x8[0-9A-Fa-f]{7}|cr_\w+|[A-Z]\w+', m.group(1)):
                a = resolve(tok)
                if a is None: continue
                if a in ours.inc.get(addr, ()) or addr in ours.out.get(a, ()) or a in ours.refd.get(addr, ()):
                    ok.append(f'{tok} references it')
                else: bad.append(f'{tok} does not reference {addr:08X}')
        for m in re.finditer(r'\b(?:two calls of|calls|calling|thin wrapper \(calls) ((?:FUN_[0-9a-fA-F]{8}|cr_\w+))', why):
            a = resolve(m.group(1))
            if a is None: continue
            (ok if a in ours.out.get(addr, ()) else bad).append(f'calls {m.group(1)}' + ('' if a in ours.out.get(addr, ()) else ': no reference'))
        for m in re.finditer(r'address stored as a callback by (FUN_[0-9a-fA-F]{8}|0x8[0-9A-Fa-f]{7})', why):
            a = resolve(m.group(1))
            (ok if addr in ours.reft.get(a, ()) else bad).append(f'{m.group(1)} references {addr:08X}' + ('' if addr in ours.reft.get(a, ()) else ': no'))
    else:
        # a global: the functions named in the why reference it
        size = glob_bytes(r)
        hi = addr + max(size or 4, 4)
        users = set()
        for t in range(addr, hi, 1) if hi - addr <= 0x40 else ():
            users |= ours.refd.get(t, set())
        if hi - addr > 0x40:
            for t, fs in ours.refd.items():
                if addr <= t < hi: users |= fs
        for tok in sorted(set(re.findall(r'\b(FUN_[0-9a-fA-F]{8}|cr_[A-Z][A-Za-z0-9]*_[A-Za-z0-9_]+)\b', why))):
            if tok.startswith('cr_g_'): continue
            a = resolve(tok)
            if a is None or a not in ours.funcs: continue
            if a in users: ok.append(f'{tok} references {r["name"]}')
            else: notes.append(f'{tok}: no direct reference into it in the index (computed address?)')
        # tables of code pointers / strings
        m = re.match(r'^(\w+)(\*?)\[(\d+)\]$', r['datatype'])
        if m:
            base, ptr, n = m.group(1), m.group(2), int(m.group(3))
            words = [dol.word(addr + 4 * i) for i in range(n)]
            if base.endswith('Func') and not ptr:
                nb = sum(1 for w in words if not (w and dol.is_text(w)))
                (ok if not nb else bad).append(f'{n} words are code pointers' if not nb else f'{nb} of {n} words are not code pointers')
            elif base == 'char' and ptr:
                nb = sum(1 for w in words if not (w and dol.cstr(w)))
                (ok if not nb else bad).append(f'{n} words point to strings' if not nb else f'{nb} of {n} words are not strings')
    # a function's why: the globals / data addresses / functions it cites are referenced by it or call it
    if isfunc:
        mine = ours.reft.get(addr, set())
        for tok in sorted(set(re.findall(r'\b(cr_g_\w+|0x8[0-9A-Fa-f]{7}|FUN_[0-9a-fA-F]{8}|cr_[A-Z]\w+)\b', why))):
            a = resolve(tok) if not tok.startswith('cr_g_') else glob_addr.get(tok)
            if a is None or a == addr: continue
            if re.search(r'(?:table|of) ' + re.escape(tok) + r'\b', why): continue   # a table cited for its entry, checked above
            if a in ours.funcs or a in ours.gfuncs:
                if a in ours.out.get(addr, ()) or addr in ours.out.get(a, ()) or a in mine:
                    ok.append(f'{tok} is a caller / callee / referenced')
                else:
                    notes.append(f'{tok}: no call / reference between them (prose?)')
            elif dol.mapped(a):
                g = _GLOBALS.get(tok) if tok.startswith('cr_g_') else next(
                    (x for x in _GLOBALS.values() if x['addr'] <= a < x['addr'] + (glob_bytes(x) or 4)), None)
                lo = g['addr'] if g else a
                n = (glob_bytes(g) if g else None) or 0x40   # a bare address: the table / record starting there
                if any(lo <= t < lo + max(n, 4) for t in mine): ok.append(f'references {tok}')
                else: notes.append(f'{tok}: not referenced directly (computed address?)')
    # quoted strings
    for m in re.finditer(r'"([^"]{3,})"', why):
        q = m.group(1)
        if q in ('%s',): continue
        users = [a for a, v in ours.gstr.items() if v and norm(q) in norm(v)]
        if not users: bad.append(f'string "{q}" not found in the game'); continue
        fs = set()
        for a in users: fs |= ours.refd.get(a, set())
        named = [resolve(t) for t in re.findall(r'FUN_[0-9a-fA-F]{8}|cr_\w+', why[max(0, m.start() - 60): m.end() + 60])]
        named = [a for a in named if a]
        if named and any(a in fs for a in named + [addr]): ok.append(f'"{q[:30]}" used by the function named with it')
        elif addr in fs: ok.append(f'"{q[:30]}" used by it')
        else: ok.append(f'"{q[:30]}" exists') if False else None
    # the game's own word in a name: cr_<WORD> with WORD from the section's name table (sequence / camera mode)
    v = 'FAIL' if bad else ('OK' if ok else 'UNCHECKABLE')
    return v, ok, bad + ['(note) ' + n for n in notes]


_GLOBALS = {}


def r_global(n): return _GLOBALS.get(n)


def glob_len(r):
    if not r: return None
    m = re.search(r'\[(\d+)\]$', r['datatype'])
    return int(m.group(1)) if m else None


SIZES = {'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'f32': 4}


def glob_bytes(r):
    m = re.match(r'^(\w+)(\*?)(?:\[(\d+)\])?$', r['datatype'])
    if not m: return None
    n = int(m.group(3)) if m.group(3) else 1
    if m.group(2) or m.group(1).endswith('Func'): return 4 * n
    s = SIZES.get(m.group(1)) or STRUCT_SIZES.get(m.group(1))
    return s * n if s else None


STRUCT_SIZES = {}
TYPE_NAMES = set()


def section_name_table(section):
    m = re.search(r'name table (0x[0-9A-Fa-f]{8})', section)
    return int(m.group(1), 16) if m else None


# ================================================================ main
def main():
    need = ['decompiled/index.db', 'tools/dtk/proj/symbols.txt', 'tools/dtk/proj/main.dol', SYMBOLS]
    miss = [n for n in need if not os.path.exists(P(n))]
    if miss: print('crosscheck_names: missing ' + ', '.join(miss)); return 2
    for l in open(P('symbols/cr_types.h'), encoding='utf-8'):
        m = re.search(r'ASSERT_SIZE\((\w+),\s*0x([0-9A-Fa-f]+)\)', l)
        if m: STRUCT_SIZES[m.group(1)] = int(m.group(2), 16)
        TYPE_NAMES.update(re.findall(r'\b(cr_[A-Z]\w*)', l))
    recs, game = load_names()
    ours = Ours()
    dol = Dol(P('tools/dtk/proj/main.dol'))
    refs = Refs()
    # merged name set: symbols.txt, overridden by candidates, plus game names where unnamed
    names_at = {}
    for r in recs:
        if r['src'] == 'symbols': names_at[r['addr']] = r['name']
    for r in recs:
        if r['src'] != 'symbols': names_at[r['addr']] = r['name']
    for r in game:
        names_at.setdefault(r['addr'], r['name'])
    addrs_of = defaultdict(set)
    for a, n in names_at.items(): addrs_of[n].add(a)
    for r in recs:
        if r['local']: local_at[r['addr']] = True
        if r['kind'] == 'function': claims_at[r['addr']].append((r['name'], r['src'], r['why']))
    # one name at two addresses within the claims themselves (not just in the merged view)
    claim_addrs = defaultdict(set)
    for r in recs + game:
        claim_addrs[r['name']].add(r['addr'])
    for n, s in claim_addrs.items():
        if len(s) > 1: addrs_of[n] |= {a for a in s if names_at.get(a) == n}
    all_names = {}
    for a, n in sorted(names_at.items()): all_names.setdefault(n, a)
    for r in game:
        if r['src'] == 'cr_globals': _GLOBALS[r['name']] = r
    glob_addr = {r['name']: r['addr'] for r in game if r['src'] == 'cr_globals'}
    for n, a in glob_addr.items(): all_names.setdefault(n, a)
    for a, (n, s) in ours.gfuncs.items(): all_names.setdefault(n, a)

    rows = []
    counts = defaultdict(lambda: defaultdict(int))
    for r in sorted(recs, key=lambda r: (r['addr'], r['src'], r['name'])):
        sup, con, notes = check_lib(r, ours, refs, names_at, addrs_of, dol)
        v, indep = lib_verdict(r, sup, con)
        counts[r['src']][v] += 1
        ev = '; '.join(f'{k}: {s}' for k, s in sorted(sup.items()))
        detail = ' | '.join(x for x in [('AGAINST ' + ' ; '.join(con)) if con else '',
                                         ('independent: ' + ', '.join(sorted(indep))) if indep else 'independent: none',
                                         ('evidence: ' + ev) if ev else '', ' ; '.join(notes)] if x)
        rows.append((v, r['addr'], r['name'], r['src'], r['tag'] + (' ' + r['ref'] if r['ref'] else ''), detail))
    for r in sorted(game, key=lambda r: (r['addr'], r['src'], r['name'])):
        v, ok, bad = check_game(r, ours, dol, all_names, glob_addr, names_at)
        dup = sorted(a for a in claim_addrs[r['name']] if a != r['addr'])
        if dup: v, bad = 'FAIL', bad + ['unique: name also at ' + ','.join(f'{a:08X}' for a in dup)]
        other = [x for x in recs if x['addr'] == r['addr'] and x['kind'] == 'function' and r['kind'] == 'function']
        if other: v, bad = 'FAIL', bad + ['address also named ' + ','.join(x['name'] for x in other)]
        counts[r['src']][v] += 1
        notes_g = [b[7:] for b in bad if b.startswith('(note)')]
        bad = [b for b in bad if not b.startswith('(note)')]
        detail = ' | '.join(x for x in [('AGAINST ' + ' ; '.join(bad)) if bad else '',
                                         ('checked: ' + ' ; '.join(ok)) if ok else 'checked: nothing mechanical',
                                         ('unconfirmed: ' + ' ; '.join(notes_g)) if notes_g else ''] if x)
        rows.append((v, r['addr'], r['name'], r['src'], 'why', detail))
    order = {'FAIL': 0, 'WEAK': 1, 'UNCHECKABLE': 1, 'OK': 2}
    rows.sort(key=lambda x: (order[x[0]], x[3] not in ('symbols',), x[3], x[1], x[2]))
    srcs = ['symbols', 'cand_hsd', 'cand_sdk', 'cand_rt', 'descriptive', 'cr_globals']
    tot = defaultdict(int)
    for s in counts:
        for v, n in counts[s].items(): tot[v] += n
    summary = (f'names_crosscheck: {sum(tot.values())} names; FAIL {tot["FAIL"]}, WEAK {tot["WEAK"]}, '
               f'UNCHECKABLE {tot["UNCHECKABLE"]}, OK {tot["OK"]}; ' +
               ', '.join(f'{s} ' + '/'.join(f'{v}={counts[s][v]}' for v in ('OK', 'WEAK', 'UNCHECKABLE', 'FAIL') if counts[s][v])
                         for s in srcs if s in counts))
    with open(OUT_TXT, 'w', encoding='utf-8', newline='\n') as f:
        f.write(summary + '\n')
        f.write('// tools/crosscheck_names.py: VERDICT ADDR NAME SOURCE TAG | AGAINST (contradictions) | independent kinds | evidence | notes\n')
        for v, a, n, s, t, d in rows:
            d = d.replace('\r', '\\r').replace('\n', '\\n')
            f.write(f'{v:11} {a:08X} {n:36} {s:11} {t:28} {d}\n')
    fails = [x for x in rows if x[0] == 'FAIL']
    with open(OUT_MD, 'w', encoding='utf-8', newline='\n') as f:
        f.write('# Name cross-check\n\n')
        f.write('Generated by `python -I tools/crosscheck_names.py` (batch 8, after the Ghidra rebuild and index). '
                'Every name is re-checked with evidence other than the rule that produced it; method in the script '
                'docstring. Full table: `symbols/names_crosscheck.txt`.\n\n')
        f.write('| source | OK | WEAK | UNCHECKABLE | FAIL |\n|---|---|---|---|---|\n')
        for s in srcs:
            if s in counts:
                c = counts[s]
                f.write(f'| {s} | {c["OK"]} | {c["WEAK"]} | {c["UNCHECKABLE"]} | {c["FAIL"]} |\n')
        f.write('\nVerdicts: library names - OK = at least one independent agreeing kind of evidence (size, callees, '
                'callers, asserts, strings) beyond the producing rule and no contradiction; WEAK = only the producing '
                'evidence; FAIL = a contradiction. Game names - OK = a mechanically checkable claim of the `why:` '
                'holds; UNCHECKABLE = prose only; FAIL = a claim is false.\n\n')
        f.write(f'## FAIL ({len(fails)})\n\n')
        if not fails: f.write('None.\n')
        for v, a, n, s, t, d in fails:
            against = d.split(' | ')[0].replace('AGAINST ', '').replace('\n', '\\n').replace('|', '/')
            f.write(f'- `{a:08X}` **{n}** ({s}, {t}): {against}\n')
    print(summary)
    return 1 if fails else 0


if __name__ == '__main__':
    sys.exit(main())
