"""local_init.py LIST SRCDIRS KEEPDIR [JOBS]: m2c's stack words copied from a .rodata label = a local with an initialiser.

    sp18 = M2C_FIELD(lbl_801C3758, u32 *, 0);            f32 t0[4] = { 0.0f, 640.0f, 0.0f, 480.0f };
    sp1C = lbl_801C3758[1];                       ->
    sp20 = lbl_801C3758[2];                              fn(..., t0, ...);
    sp24 = lbl_801C3758[3];
    fn(..., &sp18, ...);

The compiler keeps the initialiser of a local array / struct as an anonymous constant in .rodata and copies it to
the stack word by word; m2c prints the copy (Nov 13, 2026: fn_8005CF0C, fn_800B4208, fn_80069108, cr_E_VIEWIN).
For each function of LIST: the runs of such assignments are grouped into tables (stack offset and source address
both contiguous), the values read from main.dol (tools/dol_vals.py), the assignments removed and every use of a
member renamed to `tN[i]`.  The tables are declared in the order of their source addresses (the compiler emits
the constants in declaration order, so no other order can give the original's .rodata).  Three placements are
scored (hd.py, equal words) and the best kept in KEEPDIR when it beats the draft:
    top    the declarations first in the function
    last   after the other locals
    block  a nested block from the first copied word to the end of its enclosing block (the original copies the
           table where the block starts: cr_E_VIEWIN)
When the first table does not start at its label, the file has constants before it and the function reads its
own through the pool's base with that offset (fn_8008C430: 0x10): a scoring-only function `cr_Pool<label>_Order`
with a local of that many bytes (the original's values) is written in front.  Such a draft scores, but cannot be
promoted alone (promote_drafts.py: one function per object); it waits for its file.
A word table whose non-zero values all look like floats is declared f32, other tables keep the type m2c gave
the stack words.  The result is a starting point: the types of the uses usually still need a hand.
SRCDIRS: folders with comma, first that has FUNC.c wins; then tools/hand_drafts, decomp/drafts/c."""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(G)
sys.path.insert(0, os.path.join(G, 'tools'))
import dol_vals  # noqa: E402

NL, TAB = chr(10), chr(9)
RODATA = (0x801B6620, 0x801C40D8)
SIZE = {'u32': 4, 's32': 4, 'f32': 4, 'u16': 2, 's16': 2, 'u8': 1, 's8': 1, 'f64': 8, 'int': 4, 'long': 4}
NUM = r'(0x[0-9A-Fa-f]+|\d+)'
ASSIGN = re.compile(r'^(\s+)(sp\.)?(sp[0-9A-F]+) = (?:\([\w ]+\) ?)?(?:M2C_FIELD\((lbl_[0-9A-F]{8}), (\w+) \*, ' + NUM +
                    r'\)|(lbl_[0-9A-F]{8})\[' + NUM + r'\]);$')
DECL = re.compile(r'^\s+(\w+) (sp[0-9A-F]+);$')
CTX = None


def elem_type(label):
    global CTX
    if CTX is None:
        CTX = open('decomp/drafts/ctx.h', encoding='utf-8', errors='replace').read()
    m = re.search(r'extern (?:CR_RO |const )*(\w+) ' + label + r'\[', CTX)
    return m.group(1) if m else None


def score(f, text, keep, tag):
    tmp = os.path.join(keep, f'{f}_{tag}.tmp.c')
    open(tmp, 'w', encoding='utf-8', newline=NL).write(text)
    try:
        r = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/hd.py', 'decomp/drafts', f, tmp],
                           capture_output=True, text=True, timeout=300).stdout.split(NL)[0]
    except subprocess.TimeoutExpired:
        r = ''
    try:
        os.remove(tmp)
    except OSError:
        pass
    m = re.search(r'(\d+)%\s+\((\d+) equal', r)
    return (int(m.group(2)), int(m.group(1))) if m else (-1, -1)


def looks_float(v):
    e = (v >> 23) & 0xFF
    return v == 0 or v == 0x80000000 or 0x60 <= e <= 0x9F


def find_tables(lines):
    """-> (tables, assignment line numbers); a table: dict(stack, src, typ, n, names{name: index})"""
    decl = {}
    for l in lines:
        m = DECL.match(l)
        if m:
            decl[m.group(2)] = m.group(1)
    ent = []
    for i, l in enumerate(lines):
        m = ASSIGN.match(l)
        if not m:
            continue
        name = m.group(3)
        if m.group(4):
            lab, typ, off = m.group(4), m.group(5), int(m.group(6), 0)
        else:
            lab = m.group(7)
            typ = elem_type(lab)
            if typ not in SIZE:
                continue
            off = int(m.group(8), 0) * SIZE[typ]
        if typ not in SIZE:
            continue
        src = int(lab[4:], 16) + off
        if not (RODATA[0] <= src < RODATA[1]):
            continue
        dt = decl.get(name, typ)
        if SIZE.get(dt) != SIZE[typ]:
            dt = typ
        ent.append(dict(line=i, name=name, stack=int(name[2:], 16), src=src, typ=dt, size=SIZE[typ], lab=int(lab[4:], 16)))
    ent.sort(key=lambda e: e['stack'])
    groups, cur = [], []
    for e in ent:
        if cur and (cur[-1]['stack'] + cur[-1]['size'] == e['stack'] and cur[-1]['src'] + cur[-1]['size'] == e['src'] and
                    cur[-1]['size'] == e['size']):
            cur.append(e)
        else:
            if len(cur) >= 2:
                groups.append(cur)
            cur = [e]
    if len(cur) >= 2:
        groups.append(cur)
    groups.sort(key=lambda g: g[0]['src'])
    tables, gone = [], set()
    for k, g in enumerate(groups):
        typ = g[0]['typ']
        if any(e['typ'] != typ for e in g):
            typ = {4: 'u32', 2: 'u16', 1: 'u8', 8: 'f64'}[g[0]['size']]
        if g[0]['size'] == 4 and typ in ('u32', 's32'):
            raw = [int(v, 0) & 0xFFFFFFFF for v in dol_vals.values(g[0]['src'], len(g), 'u32')]
            if any(raw) and all(looks_float(v) for v in raw):
                typ = 'f32'
        vals = dol_vals.values(g[0]['src'], len(g), typ)
        tables.append(dict(name=f't{k}', typ=typ, n=len(g), vals=vals, first=min(e['line'] for e in g),
                           names={e['name']: i for i, e in enumerate(g)}, src=g[0]['src'], lab=g[0]['lab']))
        gone.update(e['line'] for e in g)
    return tables, gone


def rewrite(text, mode):
    lines = text.split(NL)
    start = next((i for i, l in enumerate(lines) if l and not l[0].isspace() and l.rstrip().endswith('{') and '(' in l), None)
    if start is None:
        return None
    tables, gone = find_tables(lines)
    if not tables:
        return None
    names = {}
    for t in tables:
        for n, i in t['names'].items():
            names[n] = f"{t['name']}[{i}]"
    decls = [f"{t['typ']} {t['name']}[{t['n']}] = {{ {', '.join(t['vals'])} }};" for t in tables]
    out = []
    for i, l in enumerate(lines):
        if i in gone:
            out.append(None)
            continue
        m = DECL.match(l)
        if m and m.group(2) in names:
            out.append(None)
            continue
        if i > start:
            l = re.sub(r'\b(?:sp\.)?(sp[0-9A-F]+)\b', lambda m: names.get(m.group(1), m.group(0)), l)
        out.append(l)
    # an emptied `struct { ... } sp;`
    for i, l in enumerate(out):
        if l is not None and l.strip() == 'struct {':
            j = i + 1
            while j < len(out) and (out[j] is None or out[j].strip() != '} sp;'):
                j += 1
            if j < len(out) and not any(o is not None and re.search(r'\bsp[0-9A-F]+;', o) for o in out[i + 1:j]):
                for k in range(i, j + 1):
                    out[k] = None
    # the file's earlier constants: the label starts before our first table (the pool of the file's .rodata,
    # fn_8008C430), so a scoring-only function in front brings that many bytes
    gap = tables[0]['src'] - tables[0]['lab']
    if gap > 0 and gap % 4 == 0:
        pre = dol_vals.values(tables[0]['lab'], gap // 4, 'u32')
        out[start] = (f"extern void cr_Pool_Use(void *);{NL}void cr_Pool{tables[0]['lab']:08X}_Order(void) {{{NL}"
                      f"    u32 before[{gap // 4}] = {{ {', '.join(pre)} }};{NL}{NL}    cr_Pool_Use(before);{NL}}}{NL}{NL}" + out[start])
    if mode == 'top':
        out[start] = out[start] + NL + NL.join('    ' + d for d in decls)
    elif mode == 'last':
        j = start + 1
        last = start
        depth = 0
        while j < len(out):
            l = out[j]
            if l is not None and l.strip():
                s = l.strip()
                if s == 'struct {':
                    depth = 1
                elif depth and s.startswith('}'):
                    depth = 0
                    last = j
                elif depth:
                    pass
                elif re.match(r'^[\w ]+[ \*]+\w+(\[\w+\])*;$', s) and '=' not in s and not s.startswith('return'):
                    last = j
                else:
                    break
            j += 1
        out[last] = out[last] + NL + NL.join('    ' + d for d in decls)
    else:
        first = min(t['first'] for t in tables)
        ind = len(lines[first]) - len(lines[first].lstrip())
        end = first + 1
        while end < len(lines) and not (lines[end].strip() and len(lines[end]) - len(lines[end].lstrip()) < ind):
            end += 1
        pad = ' ' * ind
        for k in range(first + 1, end):
            if out[k] is not None and out[k].strip():
                out[k] = '    ' + out[k]
        out[first] = pad + '{' + NL + NL.join(pad + '    ' + d for d in decls) + NL
        out[end - 1] = (out[end - 1] + NL if out[end - 1] is not None else '') + pad + '}'
    return NL.join(l for l in out if l is not None)


def main():
    lst, srcdirs, keep = sys.argv[1:4]
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    funcs = [l.strip() for l in open(lst, encoding='utf-8') if l.strip()] if os.path.exists(lst) else lst.split(',')
    os.makedirs(keep, exist_ok=True)

    def one(f):
        src = None
        for d in srcdirs.split(',') + ['tools/hand_drafts', 'decomp/drafts/c']:
            if os.path.exists(f'{d}/{f}.c'):
                src = f'{d}/{f}.c'
                break
        if src is None:
            return f + TAB + 'no draft'
        s = open(src, encoding='utf-8').read().replace(chr(13) + NL, NL)
        try:
            var = {m: rewrite(s, m) for m in ('top', 'last', 'block')}
        except Exception as e:  # a draft the line patterns do not fit
            return f + TAB + 'error ' + repr(e)
        if var['top'] is None:
            return f + TAB + 'no table'
        base = score(f, s, keep, 'base')
        sc = {m: score(f, t, keep, m) for m, t in var.items()}
        best = max(sc, key=lambda m: sc[m])
        if sc[best] > base:
            open(os.path.join(keep, f + '.c'), 'w', encoding='utf-8', newline=NL).write(var[best])
        return TAB.join([f, f'{base[0]}/{base[1]}'] + [f'{m}={sc[m][0]}/{sc[m][1]}' for m in var] +
                        [('kept ' + best) if sc[best] > base else '-'])

    with ThreadPoolExecutor(jobs) as ex:
        for line in ex.map(one, funcs):
            print(line, flush=True)


if __name__ == '__main__':
    main()
