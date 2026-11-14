"""pool_prelude.py BASE NAME [NAME ...]: the scoring-only definitions that give a draft its file's .bss pool.

A function that touches three or more objects of its own file's .bss addresses them all from one base register,
the file's first .bss object (cr_E_CLOCKIN: `lis r3, lbl_803B8C00@ha` / `addi r31, r3, lbl_803B8C00@l`, then
`addi r3, r31, 0x5758` for cr_g_Script; m2c prints `lbl_803B8C00 + 0x5758`). A draft that only declares the
objects extern can never give that code; one that DEFINES them, at the original's offsets from the base, does
(docs/drafts.md, Nov 13, 2026; the Nov 6 pool.inc of tools/file_drafts/battle_9B60 inside the draft).

BASE is the pool's first object (the label the original loads), each NAME an object of the pool the draft uses
by name (a symbol of decomp/config/GXCE01/symbols.txt at or after BASE; `NAME=TYPE` or `NAME=TYPE[N]` to give
it a type other than the context's, e.g. `lbl_803BA350=cr_Fade`). Printed, to paste at the top of the draft:

    // also defines: NAME ...           hd.py / draft_batch.py rename the context's extern declarations
    u8 cr_s_<base>_<offset>[gap];       fillers between the named objects
    <type> NAME;                        the named objects, with the context's types
    void cr_Pool<BASE>_Order(void)      references everything in address order: MWCC lays a file's data out in
                                        the order of first reference, so this fixes the offsets

relcheck.py maps our `...bss.0` one to one onto BASE. Such a draft scores and counts, but its object holds the
order function and the definitions, so promote_drafts.py leaves it out until its file is one object.
"""
import os
import re
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NL = chr(10)


def symbols():
    out = {}
    for l in open(os.path.join(G, 'decomp', 'config', 'GXCE01', 'symbols.txt'), encoding='utf-8'):
        m = re.match(r'(\S+) = \.(\w+):0x([0-9A-Fa-f]{8});.*?size:0x([0-9A-Fa-f]+)', l)
        if m:
            out[m.group(1)] = (m.group(2), int(m.group(3), 16), int(m.group(4), 16))
    return out


def ctx_decl(name):
    t = open(os.path.join(G, 'decomp', 'drafts', 'ctx.h'), encoding='utf-8', errors='replace').read()
    m = re.search(r'(?m)^(?:CR_SECTION\("[^"]*"\) )?extern (?:CR_RO )?([^;]*?\b' + re.escape(name) + r'(?:\[[^;]*\])?);', t)
    return m.group(1) if m else None


def build(base, args):
    sym = symbols()
    sec, b0, _ = sym[base]
    items = []
    for a in args:
        name, _, typ = a.partition('=')
        s, addr, size = sym[name]
        if s != sec or addr < b0:
            raise SystemExit(f"{name} is not in the section of {base}, or lies before it")
        if typ:
            m = re.match(r'(.*?)(\[.*\])?$', typ)
            decl = f'{m.group(1)} {name}{m.group(2) or ""}'
        else:
            decl = ctx_decl(name) or f'u8 {name}[0x{size:X}]'
        items.append((addr, size, name, decl))
    items.sort()
    tag = base[-4:] if base.startswith('lbl_') else base
    out = ['// also defines: ' + ' '.join(i[2] for i in items),
           f"/* the file's .bss from its first object ({base}): scoring only (tools/pool_prelude.py) */"]
    order, pos = [], b0
    for addr, size, name, decl in items:
        if addr > pos:
            f = f'cr_s_{tag}_{pos - b0:04X}'
            out.append(f'u8 {f}[0x{addr - pos:X}];')
            order.append(f)
        elif addr < pos:
            raise SystemExit(f'{name} overlaps the object before it')
        out.append(decl + ';')
        order.append(name if '[' in decl else '&' + name)
        pos = addr + size
    out.append('extern void cr_Pool_Use(void *);')
    out.append(f'void cr_Pool{base[4:] if base.startswith("lbl_") else base}_Order(void) {{')
    out += [f'    cr_Pool_Use({o});' for o in order]
    out.append('}')
    return NL.join(out)


def main():
    print(build(sys.argv[1], sys.argv[2:]))


if __name__ == '__main__':
    main()
