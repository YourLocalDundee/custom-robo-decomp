"""Independent evidence for library function names: call graph and strings, ours vs the reference decomps' sources.

Used by tools/layout_match.py (ambiguity rule) and tools/audit_layout.py. Library module, no command line.
  Ours (decompiled/xrefs, the Ghidra export, structure only via tools/export_struct.py - no Ghidra names): for every
  function (tools/dtk/proj/symbols.txt bounds)
    the functions it references (calls, tail calls, pointers), the functions that call it, the strings it uses.
  References (tools/melee, tools/refs/*: .c .cpp .cp .h): every function definition -> identifiers used in its body,
    the functions it calls, string literals. Parsed with a brace matcher, approximate but deterministic.
Evidence for "the function at A is X" (score, each item counts 1):
  callee  - the trusted functions A references (minus macro-only ones like OSPanic) are all used in a definition of X
  caller  - a trusted caller F of A has a definition that uses X
  string  - a string A uses is a literal in a definition of X
A name is only CONFIRMED by tools that compare it with its alternatives: it must score strictly higher than every one.
"""
import bisect, glob, os, re

import export_struct

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# called through assert / report / panic macros: a reference body does not show them by name
MACRO_ONLY = {'OSPanic', 'OSReport', '__assert', 'OSHalt', '__OSPanic', 'OSFatal', 'printf', 'OSVReport', 'HSD_Panic',
              '__assert_fail', 'OSErrorLine'}
KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'do', 'else', 'case', 'defined', 'asm', '__asm',
            'catch', 'typeof', '__typeof__', 'alignof', '__alignof__', 'offsetof', 'decltype', 'static_cast',
            'reinterpret_cast', 'const_cast', 'dynamic_cast', 'new', 'delete', 'throw', 'goto', 'break', 'continue'}
# emitted by the compiler (integer / float helpers, struct copy / clear, register save) or by trace macros: never
# evidence either way
HELPERS = {'__div2i', '__div2u', '__mod2i', '__mod2u', '__shl2i', '__shr2i', '__shr2u', '__cvt_fp2unsigned',
           '__cvt_dbl_usll', '__cvt_sll_flt', '__cvt_ull_flt', '__cvt_dbl_ull', '__cvt_ull_dbl', '__cvt_sll_dbl',
           '__cvt_dbl_ll', '__save_gpr', '__restore_gpr', '__save_fpr', '__restore_fpr', '__ptmf_scall', '__va_arg',
           'memset', 'memcpy', 'MWTRACE', '__fill_mem', '__copy'}
SRC_EXT = ('.c', '.cpp', '.cp', '.h')
TOKEN = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*|^[ \t]*#[^\n]*(?:\\\n[^\n]*)*',
                   re.S | re.M)
IDENT = re.compile(r'[A-Za-z_]\w*')
CALL = re.compile(r'([A-Za-z_]\w*)\s*\(')
STRREF = re.compile(r'\x01(\d+)\x02')


def _unescape(s):
    try: return bytes(s, 'latin-1', 'replace').decode('unicode_escape', 'replace')
    except Exception: return s


PP = re.compile(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|elif|else|endif)\b')
IF0 = re.compile(r'^[ \t]*#[ \t]*if[ \t]+0\b')


def _first_branch(text):
    """Keep only the first branch of every #if / #else (the #else branch after #if 0), so K&R alternatives and
    duplicate headers do not break the brace matching."""
    out, stack = [], []   # stack: is the current branch kept
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


def _parse_file(text, defs):
    strings = []
    text = _first_branch(text)

    def sub(m):
        t = m.group(0)
        if t[0] == '"':
            strings.append(_unescape(t[1:-1])); return f'\x01{len(strings) - 1}\x02'
        if t[0] == "'": return '0'
        return '\n' if t.startswith('//') or t.lstrip().startswith('#') else ' '
    text = TOKEN.sub(sub, text)
    _blocks(text, 0, len(text), strings, defs, 0)


def _blocks(text, lo, hi, strings, defs, depth):
    """Top-level { } blocks in text[lo:hi]; function definitions go to defs, namespaces / extern "C" recurse."""
    pos, start = lo, lo
    brace = re.compile(r'[{};]')
    while True:
        m = brace.search(text, pos, hi)
        if not m: return
        c = m.group(0)
        if c != '{':
            pos = start = m.end(); continue
        # matching close brace
        d, k = 1, m.end()
        while d:
            n = brace.search(text, k, hi)
            if not n: return
            if n.group(0) == '{': d += 1
            elif n.group(0) == '}': d -= 1
            k = n.end()
        header = text[start:m.start()].strip()
        body = text[m.end():k - 1]
        if depth < 3 and (re.search(r'\bnamespace\b[\w\s]*$', header) or re.search(r'\bextern\s*\x01\d+\x02$', header)):
            _blocks(text, m.end(), k - 1, strings, defs, depth + 1)
        elif header.endswith(')') or re.search(r'\)\s*(const|throw\s*\(\s*\))\s*$', header):
            name = _def_name(header)
            if name:
                ids = set(IDENT.findall(body))
                calls = {x for x in CALL.findall(body) if x not in KEYWORDS}
                lits = {strings[int(i)] for i in STRREF.findall(body)}
                defs.setdefault(name, []).append((frozenset(ids), frozenset(calls), frozenset(lits)))
        pos = start = k


def _def_name(header):
    h = header.rstrip()
    h = re.sub(r'\)\s*(const|throw\s*\(\s*\))\s*$', ')', h)
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
    if '=' in re.sub(r'\([^()]*\)', '', pre): return None   # initializer, not a function
    before = pre[:m.start()].strip()
    if not before and not re.match(r'^[A-Za-z_]\w*$', pre): return None
    return m.group(1)


_SRC = None


def source_defs():
    """name -> [(identifiers, calls, string literals)] over every reference source file."""
    global _SRC
    if _SRC is not None: return _SRC
    roots = [os.path.join(G, 'tools/melee')] + sorted(glob.glob(os.path.join(G, 'tools/refs/*')))
    files = []
    for r in roots:
        for dp, dn, fn in os.walk(r):
            dn.sort()
            for f in sorted(fn):
                if f.endswith(SRC_EXT): files.append(os.path.join(dp, f))
    defs = {}
    for f in files:
        try: t = open(f, encoding='utf-8', errors='replace').read()
        except OSError: continue
        if len(t) > 3_000_000: continue
        _parse_file(t, defs)
    _SRC = defs
    return defs


class Ours:
    """Custom Robo call graph and strings by address (needs the Ghidra export decompiled/xrefs)."""

    def __init__(self):
        self.ok = export_struct.ok()
        self.funcs = []
        for l in open(os.path.join(G, 'tools/dtk/proj/symbols.txt'), encoding='utf-8'):
            m = re.match(r'\S+ = \.(?:text|init):0x([0-9A-F]{8}); // type:function size:0x([0-9A-F]+)', l)
            if m: self.funcs.append((int(m.group(1), 16), int(m.group(2), 16)))
        self.funcs.sort()
        self.starts = [a for a, s in self.funcs]
        self.size = dict(self.funcs)
        self.out, self.inc, self.strs = {}, {}, {}
        if not self.ok: return
        con = export_struct.connect()
        strings = dict(con.execute('SELECT addr, value FROM strings'))
        for at, tgt, kind in con.execute('SELECT at, target, kind FROM refs ORDER BY at, target, kind'):
            f = self.owner(at)
            if f is None: continue
            if tgt in self.size and tgt != f:
                self.out.setdefault(f, set()).add(tgt)
                if 'CALL' in kind or 'JUMP' in kind: self.inc.setdefault(tgt, set()).add(f)
            elif tgt in strings and strings[tgt]:
                self.strs.setdefault(f, set()).add(strings[tgt])

    def owner(self, at):
        i = bisect.bisect_right(self.starts, at) - 1
        if i < 0: return None
        a = self.starts[i]
        return a if at < a + self.size[a] else None


def pending_candidates(verified):
    """addr -> name of the lib matchers' pending lines (symbols/candidates_*.txt, merged by batch6), used as call-graph
    evidence only: addresses not in verified, one name per address and one address per name, name not used in
    verified. candidates_audit.txt is left out (it is this audit's own output)."""
    by_a, by_n = {}, {}
    used = set(verified.values())
    for f in sorted(glob.glob(os.path.join(G, 'symbols/candidates_*.txt'))):
        if os.path.basename(f) == 'candidates_audit.txt': continue
        for l in open(f, encoding='utf-8'):
            m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]{8}); // type:function .* why: ', l)
            if not m: continue
            a, n = int(m.group(2), 16), m.group(1)
            if a in verified or n in used: continue
            by_a.setdefault(a, set()).add(n); by_n.setdefault(n, set()).add(a)
    return {a: next(iter(n)) for a, n in sorted(by_a.items()) if len(n) == 1 and len(by_n[next(iter(n))]) == 1}


_MTX = re.compile(r'^(?:PS|C_)?(MTX|VEC|QUAT)(\w+)$')


def _aliases(ids):
    """MTXConcat in a source is a macro for PSMTXConcat (C_MTXConcat in debug builds): count all spellings."""
    extra = set()
    for x in ids:
        m = _MTX.match(x)
        if m: extra |= {m.group(1) + m.group(2), 'PS' + m.group(1) + m.group(2), 'C_' + m.group(1) + m.group(2)}
    return ids | extra


_EXP = {}


def _ids(defs, name):
    """[(direct identifiers, identifiers including one level of called definitions)] per definition of name."""
    if name in _EXP: return _EXP[name]
    r = []
    for ids, calls, lits in defs.get(name, ()):
        d = _aliases(set(ids))
        e = set(d)
        for h in calls:
            if h != name:
                for ids2, c2, l2 in defs.get(h, ()): e |= ids2
        r.append((d, _aliases(e), lits))
    _EXP[name] = r
    return r


def score(ours, defs, addr, name, trusted):
    """(support reasons, contradiction reasons) for "the function at addr is name".
    trusted: addr -> name we trust. A contradiction needs the name to have reference definitions and is only counted
    when one level of called definitions (inline helpers) does not explain it either."""
    ds = _ids(defs, name)
    if not ds: return [], []
    sup, con = [], []
    callees = {trusted[t] for t in ours.out.get(addr, ()) if t in trusted and trusted[t] != name} - MACRO_ONLY - HELPERS
    if callees:
        if any(callees <= d for d, e, l in ds): sup.append('callees ' + ','.join(sorted(callees)))
        elif not any(callees <= e for d, e, l in ds):
            con.append('no definition uses all of ' + ','.join(sorted(callees)))
    for f in sorted(ours.inc.get(addr, ())):
        fn = trusted.get(f)
        if not fn or fn == name or fn in MACRO_ONLY or name in HELPERS: continue
        fd = _ids(defs, fn)
        if not fd: continue
        if any(name in d for d, e, l in fd): sup.append(f'caller {fn}')
        elif not any(name in e for d, e, l in fd): con.append(f'caller {fn} does not use it')
    for s in sorted(ours.strs.get(addr, ())):
        if len(s) >= 4 and any(s in l for d, e, l in ds): sup.append(f'string "{s[:40]}"')
    return sup, con


def judge(ours, defs, addr, name, pool, trusted, amb=False, taken=None):
    """Compare name with the alternative names in pool (names trusted at another address are no alternative).
    -> (status, right name or None, why); status CONFIRMED | WRONG | CONTRADICTED | UNPROVEN
      CONFIRMED    strongly supported, not contradicted, and no uncontradicted alternative has as much support
      WRONG        contradicted (amb: or without any reference definition), and an uncontradicted alternative is
                   strongly supported (right = the unique best one)
      strong = callees or a string, or at least two callers (one caller also uses the neighbours of a name)
      CONTRADICTED contradicted, no supported alternative
      UNPROVEN     anything else (no evidence, or a tie)"""
    s0, c0 = score(ours, defs, addr, name, trusted)
    # one caller using a name is weak (a caller uses its siblings too): strong = callees or a string, or 2+ callers
    strong = lambda s: any(not x.startswith('caller ') for x in s) or len(s) >= 2
    taken = {v for k, v in (trusted if taken is None else taken).items() if k != addr}
    alts = []
    for x in sorted(set(pool) - {name} - taken):
        s, c = score(ours, defs, addr, x, trusted)
        if s and not c and strong(s): alts.append((len(s), x, s))
    best = max((a[0] for a in alts), default=0)
    fmt = lambda s, c: '; '.join(s + ['AGAINST ' + x for x in c])
    if s0 and not c0 and strong(s0) and len(s0) > best:
        return 'CONFIRMED', None, fmt(s0, c0) + (f' (best alternative {best})' if alts else '')
    if (c0 or (amb and not _ids(defs, name))) and alts:
        top = [a for a in alts if a[0] == best]
        right = top[0][1] if len(top) == 1 else None
        return 'WRONG', right, (f'{name}: {fmt(s0, c0) or "no reference definition"} | ' + ' | '.join(f'{x}: {"; ".join(s)}' for n, x, s in top))
    if c0: return 'CONTRADICTED', None, fmt(s0, c0)
    if s0: return 'UNPROVEN', None, f'tie: {name} ({fmt(s0, c0)}) vs ' + ', '.join(x for n, x, s in alts if n >= len(s0))
    return 'UNPROVEN', None, 'no call-graph / string evidence'
