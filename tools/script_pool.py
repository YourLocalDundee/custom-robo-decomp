"""script_pool.py LIST SRCDIRS KEEPDIR [JOBS]: m2c drafts on a file's .bss pool rewritten onto defined objects.

A function that touches three or more objects of its file's .bss addresses them from the file's first object
(docs/drafts.md, Nov 13, 2026); m2c then prints every access as `BASE + 0x...` and the draft, which only has
the extern array BASE, can never compile to that code. For each function of LIST whose draft has such a base:
  1. every `BASE + 0xNNNN` is resolved to the symbol that holds it (decomp/config/GXCE01/symbols.txt) and the
     access rewritten onto that object:
       M2C_FIELD((BASE + 0xN), T *, off)   ->  ((struct cr_PF_.. *) &OBJ)->v     a member: `addi rX, base, obj`
                                                                                 then `off(rX)` (off 0: folded)
       M2C_FIELD(BASE, T *, 0xN)           ->  *(T *) ((u8 *) &OBJ + in)         folded into one displacement
       (BASE + 0xN)[k]                     ->  the byte member k of OBJ
       BASE + 0xN  (anything else)         ->  ((u8 *) &OBJ + in)
  2. the objects are defined in front of the function at their offsets (tools/pool_prelude.py: the
     `// also defines:` line, fillers, the order function), with the context's types;
  3. for the script file (BASE lbl_803B8C00) the command prologue, which m2c prints interleaved with the
     command's own statements, is replaced by its source form (cr_E_CIN, cr_E_CLOCKIN, cr_MAP_LINK):
         pc = cr_g_ScriptPC;
         cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
         cr_g_Script.history_index += 1;
         cr_g_Script.history_index &= 0x3F;
         cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
         cr_g_ScriptPC += N;
         if (0) { (*cmd_cnt)++; }
     (the cast and the dead block are both needed: each is one temporary / one variable of the original)
     and the argument reads become pc[k].
The result is scored (hd.py, equal words) and written to KEEPDIR when it beats the draft; a 100% result is
checked with relcheck.py and reported as MATCH. Statement order and types usually still need a hand
(tools/order_climb.py on the result).
SRCDIRS: folders with comma, first that has FUNC.c wins; then decomp/drafts/c."""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
sys.path.insert(0, os.path.join(G, 'tools'))
import pool_prelude  # noqa: E402

NL, TAB = chr(10), chr(9)
HEX = r'(0x[0-9A-Fa-f]+|\d+)'
TYP = r'([\w ]+? ?\**?) ?\*'
SYM = None
SCRIPT_BASE = 'lbl_803B8C00'


def symbols():
    global SYM
    if SYM is None:
        SYM = pool_prelude.symbols()
    return SYM


def owner(sec, addr):
    best = None
    for n, (s, a, size) in symbols().items():
        if s == sec and a <= addr < a + max(size, 1) and (best is None or a > best[1]):
            best = (n, a, size)
    return best


def score(f, text, keep, tag, tool='hd.py'):
    tmp = os.path.join(keep, f'{f}_{tag}.tmp.c')
    open(tmp, 'w', encoding='utf-8', newline=NL).write(text)
    try:
        r = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/' + tool, 'decomp/drafts', f, tmp],
                           capture_output=True, text=True, timeout=300).stdout.strip().split(NL)
    except subprocess.TimeoutExpired:
        r = ['']
    try:
        os.remove(tmp)
    except OSError:
        pass
    if tool != 'hd.py':
        return r[-1]
    m = re.search(r'(\d+)%\s+\((\d+) equal', r[0])
    return (int(m.group(2)), int(m.group(1))) if m else (-1, -1)


def find_base(text):
    cnt = {}
    for m in re.finditer(r'\b(lbl_[0-9A-F]{8}|cr_g_\w+) \+ (0x[0-9A-Fa-f]+)', text):
        cnt.setdefault(m.group(1), set()).add(m.group(2))
    cand = [(len(v), k) for k, v in cnt.items() if k in symbols() and symbols()[k][0] == 'bss']
    return max(cand)[1] if cand else None


def prologue(body, casts=False):
    """the script command prologue -> its source form; returns (body, ok).  casts: the argument reads stay
    M2C_FIELD(pc, T *, k) (one use of pc each; as pc[k] they count two in the register order).  A command without
    arguments has no pc variable in the m2c draft (the command word is read through cr_g_ScriptPC itself)."""
    m = re.search(r'(?m)^\s+(\w+) = cr_g_ScriptPC;$', body)
    adv = re.search(r'(?m)^\s+cr_g_ScriptPC \+= (\d+|0x[0-9A-Fa-f]+);$', body)
    hist = re.search(r'(?m)^\s+M2C_FIELD\(\(' + SCRIPT_BASE + r' \+ 0x5758 \+ \(\(M2C_FIELD\(\(' + SCRIPT_BASE +
                     r' \+ 0x5758\), s32 \*, 8\) \* 4\) \+ 0x4C\)\), u32 \*, 0\) = [^;]*;$', body)
    if not (adv and hist) or (m is None and 'cr_g_ScriptPC;' not in hist.group(0)):
        return body, False
    pc = m.group(1) if m else '__no_pc_variable__'
    temps = set(re.findall(r'(?m)^\s+(\w+) = ' + SCRIPT_BASE + r' \+ 0x5758;$', body))
    dep = set(re.findall(r'(?m)^\s+(\w+) = \(cr_g_ScriptDepth \* 4\) \+ 0xC;$', body))
    out, dropped = [], 0
    for l in body.split(NL):
        s = l.strip()
        if (s == f'{pc} = cr_g_ScriptPC;' or s == adv.group(0).strip() or s == hist.group(0).strip()
                or re.fullmatch(r'(\w+) = ' + SCRIPT_BASE + r' \+ 0x5758;', s)
                or re.fullmatch(r'(\w+) = \(cr_g_ScriptDepth \* 4\) \+ 0xC;', s)
                or re.fullmatch(r'M2C_FIELD\((?:\(\(u8 \*\) \()?\(?(\w+|' + SCRIPT_BASE + r' \+ 0x5758)\)?\)?\)?, s32 \*, 8\) (\+= 1|&= 0x3F);', s)
                or re.fullmatch(r'M2C_FIELD\(\((\w+) \+ (\w+)\), u32 \*, 0\) \+= 1;', s)):
            dropped += 1
            continue
        d = re.fullmatch(r'[\w ]+[ \*](\w+);', s)
        if d and (d.group(1) in temps or d.group(1) in dep or d.group(1) == pc):
            continue
        out.append(l)
    if dropped < (6 if m else 5):
        return body, False
    body = NL.join(out)
    if not casts:
        body = re.sub(r'M2C_FIELD\(' + pc + r', s16 \*, ' + HEX + r'\)', lambda q: f'pc[{int(q.group(1), 0) // 2}]', body)
        body = re.sub(r'M2C_FIELD\(' + pc + r', u16 \*, ' + HEX + r'\)', lambda q: f'((u16 *) pc)[{int(q.group(1), 0) // 2}]', body)
        body = re.sub(r'M2C_FIELD\(' + pc + r', ([us]8) \*, ' + HEX + r'\)', lambda q: f'(({q.group(1)} *) pc)[{int(q.group(2), 0)}]', body)
    body = re.sub(r'\b' + pc + r'\b', 'pc', body)
    canon = NL.join(['    s16 *pc;', '    u32 *cmd_cnt;', '', '    pc = cr_g_ScriptPC;',
                     '    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];',
                     '    cr_g_Script.history_index += 1;', '    cr_g_Script.history_index &= 0x3F;',
                     '    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;', f'    cr_g_ScriptPC += {adv.group(1)};',
                     '    if (0) {', '        (*cmd_cnt)++;', '    }'])
    # after the declarations of the function (the first blank line of its body)
    lines = body.split(NL)
    k = 1
    while k < len(lines) and lines[k].strip():
        k += 1
    decls = [l for l in lines[1:k]]
    body = NL.join([lines[0]] + decls + canon.split(NL) + lines[k + 1:]) if decls else NL.join([lines[0]] + canon.split(NL) + lines[1:])
    return body, True


def rewrite(text, casts=False):
    text = text.replace(chr(13) + NL, NL)
    base = find_base(text)
    if base is None:
        return None, 'no pool base'
    sec, b0, _ = symbols()[base]
    lines = [l for l in text.split(NL) if not (l.startswith('#define ') or l.startswith('#include "ctx.h"') or l.startswith('#undef ')
                                                or l.startswith('// m2c draft'))]
    start = next((k for k, l in enumerate(lines) if l and not l[0].isspace() and l.rstrip().endswith('{') and '(' in l
                  and not l.startswith(('static inline', 'typedef', 'struct'))), None)
    if start is None:
        return None, 'no function'
    head, body = NL.join(lines[:start]), NL.join(lines[start:])
    names, structs = {}, {}
    script = False
    if base == SCRIPT_BASE:
        body, script = prologue(body, casts)
        if script:
            names['cr_g_Script'] = None

    def obj(off):
        o = owner(sec, b0 + off)
        if o is None:
            raise ValueError(f'no symbol at {base} + 0x{off:X}')
        names[o[0]] = None
        return o[0], b0 + off - o[1]

    def member(n, total, typ):
        typ = typ.strip()
        if total == 0:
            return f'(*({typ} *) &{n})'
        tag = 'cr_PF_%X_%s' % (total, re.sub(r'\W+', '_', typ.replace('*', 'p')).strip('_'))
        structs[tag] = f'struct {tag} {{\n    u8 pad[0x{total:X}];\n    {typ} v;\n}};'
        return f'((struct {tag} *) &{n})->v'

    def p1(m):
        n, inner = obj(int(m.group(1), 16))
        return member(n, inner + int(m.group(3), 0), m.group(2))

    def p2(m):
        n, inner = obj(int(m.group(2), 0))
        return f'(*({m.group(1).strip()} *) &{n})' if inner == 0 else f'(*({m.group(1).strip()} *) ((u8 *) &{n} + 0x{inner:X}))'

    def p4(m):
        n, inner = obj(int(m.group(1), 16))
        return member(n, inner + int(m.group(2), 0), 'u8')

    def p5(m):
        n, inner = obj(int(m.group(1), 16))
        return f'((u8 *) &{n})' if inner == 0 else f'((u8 *) &{n} + 0x{inner:X})'

    B = re.escape(base)
    try:
        body = re.sub(r'M2C_FIELD\(\(\(u8 \*\) \(' + B + r' \+ (0x[0-9A-Fa-f]+)\)\), ' + TYP + r', ' + HEX + r'\)', p1, body)
        body = re.sub(r'M2C_FIELD\(\(' + B + r' \+ (0x[0-9A-Fa-f]+)\), ' + TYP + r', ' + HEX + r'\)', p1, body)
        body = re.sub(r'M2C_FIELD\(' + B + r', ' + TYP + r', ' + HEX + r'\)', p2, body)
        body = re.sub(r'\(' + B + r' \+ (0x[0-9A-Fa-f]+)\)\[' + HEX + r'\]', p4, body)
        body = re.sub(B + r' \+ (0x[0-9A-Fa-f]+)', p5, body)
    except ValueError as e:
        return None, str(e)
    if re.search(r'\b' + B + r'\b', body):
        names[base] = None
    order = sorted(names, key=lambda n: symbols()[n][1])
    pre = pool_prelude.build(base, order).split(NL)
    out = NL.join([pre[0]] + ([head.strip(NL)] if head.strip() else []) + [''] + list(structs.values()) + [''] + pre[1:] + ['', body])
    return out, ('script ' if script else '') + f'{base}: ' + ' '.join(order)


def main():
    lst, srcdirs, keep = sys.argv[1:4]
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
    os.makedirs(keep, exist_ok=True)

    def one(f):
        src = None
        for d in srcdirs.split(',') + ['decomp/drafts/c']:
            if os.path.exists(f'{d}/{f}.c'):
                src = f'{d}/{f}.c'
                break
        if src is None:
            return f + TAB + 'no draft'
        s = open(src, encoding='utf-8').read()
        if '// also defines:' in s:
            return f + TAB + 'already on defined objects'
        try:
            new, info = rewrite(s)
        except Exception as e:  # a draft the patterns do not fit
            return f + TAB + 'error ' + repr(e)
        if new is None:
            return f + TAB + info
        base = score(f, s, keep, 'base')
        sc = score(f, new, keep, 'new')
        if info.startswith('script '):
            # the same with the argument reads left as casts: kept when it scores higher
            new2, _ = rewrite(s, True)
            if new2 and new2 != new:
                sc2 = score(f, new2, keep, 'new2')
                if sc2 > sc:
                    new, sc, info = new2, sc2, info.replace('script ', 'script (casts) ', 1)
        note = '-'
        if sc > base:
            open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(new)
            note = 'kept'
            if sc[1] == 100:
                note = 'MATCH' if score(f, new, keep, 'rel', 'relcheck.py').startswith('relocations equal') else 'kept (100%, relocations differ)'
        return TAB.join([f, f'{base[0]}/{base[1]}', f'{sc[0]}/{sc[1]}', note, info])

    with ThreadPoolExecutor(jobs) as ex:
        for line in ex.map(one, funcs):
            print(line, flush=True)


if __name__ == '__main__':
    main()
