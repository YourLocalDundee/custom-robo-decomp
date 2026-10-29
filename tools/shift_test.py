"""Shift test of the Phase 4 build (plan.md Phase 4 Milestone 2): can code and data move, and does the game survive?

On a scratch copy of batch 9's build (decomp/build/shift/; the real build is never touched):
  1. Insertion points, chosen by rule from the unshifted build (no address is typed in by hand):
       - the start of each section .text .rodata .data .bss .sdata .sbss .sdata2 (inside the first object there);
       - inside the game code: the first function at or after the middle of the game-code object;
       - between library objects: the start of the first split (non-automatic) .text object after the game code;
       - inside every data section (.rodata .data .bss .sdata .sbss .sdata2): the start of the first, the middle
         and the last data piece of a split (non-automatic) object, i.e. the per-object data boundaries that
         tools/gen_dtk_splits.py proved (a wrong boundary shows up as a relocation target moved by the wrong amount);
       - inside .rodata / .data / .bss: the first glue-span start at or after the middle of the section. A glue span
         (tools/reloc_audit.py) begins where the code builds other objects' addresses from one base plus constant
         offsets, which is how CodeWarrior addresses a file's statics (from the start of that file's section): so it
         is a file boundary, and no constant-offset access crosses it. Points inside any glue span are refused.
     Each point gets PAD bytes (.skip, a multiple of every alignment), so everything after it moves by a different
     multiple of PAD: a hard-coded address would point at the wrong thing and the check below sees it.
  2. The objects holding a point are reassembled from dtk's asm (powerpc-eabi-as, the project's flags, dtk elf
     fixup) and everything is relinked with the project's mwld and ldscript, then dtk elf2dol. A control link of the
     same objects reassembled WITHOUT padding must give the retail SHA-1 (so the asm route itself changes nothing).
  3. Static check, shifted against unshifted (both main.elf keep every relocation):
       - every symbol moved by exactly the padding inserted before it;
       - every relocation of the unshifted build exists at the moved site, with the same type and symbol, and its
         target moved by exactly the padding before the target (else a glue span or an end pointer was split);
       - every relocated data word holds its new target, every relocated branch reaches it, every lis/addi pair and
         small-data access makes it (tools/reloc_audit.py consistency on the shifted build);
       - no unrelocated word or lis pair in the shifted build still holds an address that moved, except the
         numbers that tools/gen_dtk_relocs.py blocked on purpose (they must keep their old value).
  Writes decomp/reports/shift_report.txt (CRLF, deterministic) and decomp/build/shift/main.dol (for the boot test).
Usage: python -I tools/shift_test.py [--pad 0x100] [--all-data]   |   --expect (after a run: the values batch 11 checks)
  --all-data: padding at every data-object boundary instead of 3 per section (by hand; report in decomp/build/shift/;
  rerun without it afterwards, batch 11 boots the batch's shifted build)
"""
import bisect
import hashlib
import json
import os
import re
import shutil
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
import reloc_audit as ra  # noqa: E402
import gen_dtk_relocs as gr  # noqa: E402

D = gr.D
B = os.path.join(D, 'build/GXCE01')
S = os.path.join(D, 'build/shift')
REPORT = os.path.join(D, 'reports/shift_report.txt')
SECTIONS = ('.text', '.rodata', '.data', '.bss', '.sdata', '.sbss', '.sdata2')
DATA_SPLIT = ('.rodata', '.data', '.bss', '.sdata', '.sbss', '.sdata2')
AS_FLAGS = ['-mgekko', '--strip-local-absolute', '-I', 'include', '-I', 'build/GXCE01/include',
            '--defsym', 'BUILD_VERSION=0']


def unit_sections(elf):
    """[(object basename, {section: start})] from the linked ELF's FILE / SECTION symbols, in link order"""
    out, cur = [], None
    names = {s[0]: s[1] for s in elf.sections}
    for n, v, sz, t, b, shn in elf.syms:
        if t == 4:
            cur = {}
            out.append((n, cur))
        elif t == 3 and cur is not None and shn in names: cur.setdefault(names[shn], v)
    return out


ALL_DATA = False   # --all-data: pad at EVERY data-object boundary (a by-hand check; the batch uses 3 per section)


def points(elf, objs, c, units, pad):
    """-> sorted [(address, section, unit name, why)]"""
    us = unit_sections(elf)
    secs = {s[1]: (s[3], s[3] + s[4]) for s in elf.alloc}
    # main.elf's FILE symbols are in the units' link order (two units may share a basename: MetroTRK ddh/gdev main.c)
    if len(us) != len(units) or any(n != os.path.basename(u['name']) for (n, _), u in zip(us, units)):
        raise SystemExit('the FILE symbols of main.elf are not the units of config.json in link order: cannot map them')
    where = []    # (start, section, unit name)
    for u, (n, ss) in zip(units, us):
        for sec, a in ss.items():
            where.append((a, sec, u['name']))
    where.sort()

    def unit_at(sec, a):
        best = None
        for start, s, n in where:
            if s == sec and start <= a: best = (start, n)
        return best
    pts = []
    for sec in SECTIONS:
        lo, hi = secs[sec]
        pts.append((lo, sec, unit_at(sec, lo)[1], f'start of {sec}'))
    game = unit_at('.text', secs['.text'][0])
    gstart = game[0]
    gend = min(a for a, s, n in where if s == '.text' and a > gstart)
    fstarts = sorted(v for n, v, sz, t, b, shn in elf.syms if t == 2 and gstart <= v < gend)
    mid = fstarts[bisect.bisect_left(fstarts, (gstart + gend) // 2)]
    pts.append((mid, '.text', game[1], f'inside the game code ({game[1]}), the first function after its middle'))
    lib = next((a, n) for a, s, n in where if s == '.text' and a >= gend and not n.startswith('auto_'))
    pts.append((lib[0], '.text', lib[1], f'between library objects: the start of {lib[1]}'))
    for sec in ('.rodata', '.data', '.bss'):
        lo, hi = secs[sec]
        cand = [g0 for g0, g1 in c['glue'] if g0 >= (lo + hi) // 2 and g1 < hi and objs.is_start(g0)]
        if cand:
            pts.append((cand[0], sec, unit_at(sec, cand[0])[1],
                        f'inside {sec}: the start of the glue span 0x{cand[0]:08X}.. (a file\'s statics base)'))
    # inside every data section, at the boundaries of the split (non-automatic) objects' data pieces
    # (tools/gen_dtk_splits.py proved them): the first, the middle and the last one after the section start
    for sec in DATA_SPLIT:
        lo, hi = secs[sec]
        b = sorted({a for a, s, n in where if s == sec and lo < a < hi and not n.startswith('auto_')})
        pick = b if ALL_DATA else (sorted({b[0], b[len(b) // 2], b[-1]}) if b else [])
        for a in pick:
            pts.append((a, sec, unit_at(sec, a)[1],
                        f'inside {sec}: the data piece of {unit_at(sec, a)[1]} (a proven object boundary)'))
    for a, sec, n, why in pts:
        for g0, g1 in c['glue']:
            if g0 < a <= g1: raise SystemExit(f'insertion point 0x{a:08X} is inside the glue span 0x{g0:08X}..0x{g1:08X}')
        if not objs.is_start(a): raise SystemExit(f'insertion point 0x{a:08X} is not a symbol start')
    seen, out = set(), []
    for x in pts:   # one point per address (a glue-span start may also be an object boundary)
        if x[0] not in seen: seen.add(x[0]); out.append(x)
    return sorted(out)


def insert(lines, a, sec, pad, at_section_start):
    """insert '.skip pad' into the asm lines before the symbol at address a (or after the section's first .balign)"""
    if at_section_start:
        for i, l in enumerate(lines):
            if l.startswith('.balign'):
                lines.insert(i + 1, f'.skip 0x{pad:X}  # shift test padding')
                return
        raise SystemExit('no .balign in asm')
    tag = re.compile(r'^# \S+:0x[0-9A-F]+ \| 0x%08X \|' % a)
    for i, l in enumerate(lines):
        if tag.match(l):
            j = i
            while j and lines[j - 1].startswith('.balign'): j -= 1
            lines.insert(j, f'.skip 0x{pad:X}  # shift test padding')
            return
    raise SystemExit(f'0x{a:08X} not found in the asm')


def assemble(src, obj):
    exe = '.exe' if os.name == 'nt' else ''
    gr.run([os.path.join(D, 'build/binutils', 'powerpc-eabi-as' + exe)] + AS_FLAGS + ['-o', obj, src])
    gr.run([gr.DTK, 'elf', 'fixup', obj, obj])


def build(units, pts, pad, tag):
    """reassemble the objects holding a point (with padding if pad) and relink -> (elf path, dol path, sha1)"""
    od = os.path.join(S, tag)
    os.makedirs(od, exist_ok=True)
    by_unit = {}
    for a, sec, n, why in pts: by_unit.setdefault(n, []).append((a, sec, why))
    objects = []
    for u in units:
        o = u['object']
        if u['name'] in by_unit:
            rel = os.path.relpath(o, os.path.relpath(os.path.join(B, 'obj'), D))[:-2].replace(os.sep, '/')
            lines = open(os.path.join(B, 'asm', rel + '.s'), 'rb').read().decode().split('\n')
            # insert from the highest address down so the line indexes stay valid
            for a, sec, why in sorted(by_unit[u['name']], reverse=True):
                if pad: insert(lines, a, sec, pad, why.startswith('start of'))
            src = os.path.join(od, rel.replace('/', '_') + '.s')
            open(src, 'wb').write('\n'.join(lines).encode())
            obj = os.path.relpath(src[:-2] + '.o', D).replace('\\', '/')
            assemble(os.path.relpath(src, D).replace('\\', '/'), obj)
            o = obj
        objects.append(o)
    rel = os.path.relpath(od, D).replace('\\', '/')
    lcf = os.path.relpath(os.path.join(B, 'ldscript.lcf'), D).replace(os.sep, '/')
    sha = gr.link(objects, lcf, f'{rel}/main.elf', f'{rel}/main.dol')
    return os.path.join(od, 'main.elf'), os.path.join(od, 'main.dol'), sha


ABS = 0xfff1   # linker-made symbols (_SDA_BASE_, _stack_addr, __ArenaLo, ...): checked by name, not by address


def unique_syms(elf):
    cnt = {}
    for n, v, sz, t, b, shn in elf.syms:
        if n and t not in (3, 4) and shn != ABS: cnt.setdefault(n, []).append(v)
    return {n: v[0] for n, v in cnt.items() if len(v) == 1}


def static_check(e0, e1, pts, pad, blocked):
    P = sorted(a for a, s, n, w in pts)

    def delta(a): return pad * bisect.bisect_right(P, a)   # a symbol AT a point moves (padding goes before it)
    res, bad = {}, []
    u0, u1 = unique_syms(e0), unique_syms(e1)
    common = [n for n in u0 if n in u1]
    wrong = [n for n in common if u1[n] - u0[n] != delta(u0[n])]
    res['symbols compared (unique names)'] = len(common)
    res['symbols moved by the wrong amount'] = len(wrong)
    bad += [f'symbol {n}: 0x{u0[n]:08X} -> 0x{u1[n]:08X}, expected +0x{delta(u0[n]):X}' for n in wrong[:20]]
    # relocations: same site (moved), type, symbol; target moved by the padding before the target
    absn = {n for n, v, sz, t, b, shn in e0.syms if shn == ABS and n}
    miss = moved_wrong = 0
    for s, (ty, t, n, add) in e0.relocs.items():
        r = e1.relocs.get(s + delta(s))
        if r is None or r[0] != ty or r[2] != n:
            miss += 1
            if miss <= 20: bad.append(f'relocation at 0x{s:08X} ({n}) missing after the shift')
            continue
        want = e1.sym(n) + add if n in absn else (t + delta(t)) & 0xffffffff
        if r[1] != want:
            moved_wrong += 1
            if moved_wrong <= 20:
                bad.append(f'relocation at 0x{s:08X} -> {n}+0x{add:X}: target 0x{t:08X} -> 0x{r[1]:08X}, '
                           f'expected +0x{delta(t):X} (an insertion point splits the symbol and its target)')
    res['relocations of the unshifted build'] = len(e0.relocs)
    res['relocations missing after the shift'] = miss
    res['relocation targets moved by the wrong amount'] = moved_wrong
    res['relocations in the shifted build'] = len(e1.relocs)
    # values in the shifted build: every relocated value = its target; branches reach their target
    elf, objs, rng, events, raw, rels = ra.audit(e1.path)
    inc = ra.consistency(elf, events, rels)
    br = 0
    for s, (ty, t, n, add) in elf.relocs.items():
        if ty in (ra.R_REL24, ra.R_REL14):
            d = da_branch_target(elf.word(s), s)
            if d != t:
                br += 1
                if br <= 20: bad.append(f'branch at 0x{s:08X} reaches {d}, not {n} 0x{t:08X}')
    sd = {x[1]: x[3] for x in e1.alloc}
    for sym, sec in (('_SDA_BASE_', '.sdata'), ('_SDA2_BASE_', '.sdata2')):
        if e1.sym(sym) != sd[sec] + 0x8000: bad.append(f'{sym} 0x{e1.sym(sym):08X} is not {sec} + 0x8000'); br += 1
    res['relocated values (data words, lis pairs, small-data, loads) different from their target'] = len(inc)
    res['relocated branches that miss their target'] = br
    bad += [f'{k} at 0x{a:08X}: value 0x{v:08X}, target 0x{d.get("target", 0):08X}' for k, a, v, d in inc[:20]]
    # stale addresses: unrelocated words / code addresses in the shifted build that still hold a moved address
    c = ra.classify(elf, objs, events, rels)
    lo0, hi0 = ra.moving_range(e0)
    inv = {s + delta(s): s for s in blocked}   # blocked numbers, at their shifted sites

    def moved(v): return lo0 <= v < hi0 and delta(v) != 0
    stale_d = [(a, v) for a, v in raw if a not in inv and moved(v)]
    stale_c = [(k, a, v) for k, a, v, d in c['misses'] if a not in inv and d.get('hi') not in inv and moved(v)]
    kept = sum(1 for s in blocked if e1.word(s + delta(s)) == e0.word(s))
    fps = len(c['fp_data']) + len(c['fp_code'])
    res['numbers relocated as addresses (audit false positives) in the shifted build'] = fps
    res['unrelocated data words holding a moved address'] = len(stale_d)
    res['unrelocated code addresses (lis pairs, small-data) holding a moved address'] = len(stale_c)
    res[f'blocked numbers (gen_dtk_relocs) still holding their old value'] = f'{kept} of {len(blocked)}'
    bad += [f'stale data word at 0x{a:08X}: 0x{v:08X}' for a, v in stale_d[:20]]
    bad += [f'stale {k} at 0x{a:08X}: 0x{v:08X}' for k, a, v in stale_c[:20]]
    ok = not (wrong or miss or moved_wrong or inc or br or stale_d or stale_c or fps) and kept == len(blocked)
    return ok, res, bad


def da_branch_target(w, at):
    op = w >> 26
    if op == 18:
        t = w & 0x3fffffc
        t = t - 0x4000000 if t & 0x2000000 else t
        return t if w & 2 else (at + t) & 0xffffffff
    if op == 16:
        t = w & 0xfffc
        t = t - 0x10000 if t & 0x8000 else t
        return t if w & 2 else (at + t) & 0xffffffff
    return None


def expect():
    """--expect: what batch 11 checks in Dolphin's log (OSInit prints the arena start = _stack_addr), as JSON"""
    e0, e1 = ra.Elf(os.path.join(B, 'main.elf')), ra.Elf(os.path.join(S, 'shifted/main.elf'))
    sha = hashlib.sha1(open(os.path.join(S, 'main.dol'), 'rb').read()).hexdigest()
    print(json.dumps({'arena_control': f'0x{e0.sym("_stack_addr"):08x}', 'arena_shifted': f'0x{e1.sym("_stack_addr"):08x}',
                      'sha1_shifted': sha}))
    return 0


def main(argv):
    if '--expect' in argv: return expect()
    global ALL_DATA, REPORT
    if '--all-data' in argv:
        ALL_DATA = True
        REPORT = os.path.join(S, 'shift_report_all_data.txt')   # scratch only: the committed report is the batch's
    pad = int(argv[argv.index('--pad') + 1], 0) if '--pad' in argv else 0x100
    if pad % 0x100: raise SystemExit('--pad must be a multiple of 0x100 (keeps every alignment)')
    elf0_path = os.path.join(B, 'main.elf')
    if not os.path.exists(elf0_path): raise SystemExit('no decomp/build/GXCE01/main.elf: run batch 9 first')
    if os.path.exists(S): shutil.rmtree(S)
    os.makedirs(S)
    units = json.load(open(os.path.join(B, 'config.json')))['units']
    elf, objs, rng, events, raw, rels = ra.audit(elf0_path)
    c = ra.classify(elf, objs, events, rels)
    pts = points(elf, objs, c, units, pad)
    e0p, d0p, sha0 = build(units, pts, 0, 'control')
    if sha0 != gr.RETAIL: raise SystemExit(f'control link (asm route, no padding) SHA-1 {sha0} is not retail')
    e1p, d1p, sha1 = build(units, pts, pad, 'shifted')
    e0, e1 = ra.Elf(e0p), ra.Elf(e1p)
    e0.path, e1.path = e0p, e1p
    blocked = gr.blocked_sites()
    ok, res, bad = static_check(e0, e1, pts, pad, blocked)
    shutil.copyfile(d1p, os.path.join(S, 'main.dol'))
    secs0 = {s[1]: (s[3], s[4]) for s in e0.alloc}
    secs1 = {s[1]: (s[3], s[4]) for s in e1.alloc}
    lines = [
        'shift test (tools/shift_test.py, method: its docstring): padding inserted into a scratch copy of batch 9\'s',
        'build, relinked, and checked against the unshifted build.',
        f'padding per point: 0x{pad:X} bytes; points: {len(pts)}',
    ]
    for i, (a, sec, n, why) in enumerate(pts):
        lines.append(f'  {i + 1:2}. 0x{a:08X} {sec:8} {why} -> everything from here moves by +0x{pad * (i + 1):X}')
    lines += [
        f'control (same objects reassembled without padding): SHA-1 {sha0} = retail',
        f'shifted main.dol: SHA-1 {sha1} ({os.path.getsize(d1p)} bytes, retail {os.path.getsize(d0p)})',
        'sections, unshifted -> shifted:',
    ]
    for n in sorted(secs0, key=lambda x: secs0[x][0]):
        a0, s0 = secs0[n]
        a1, s1 = secs1[n]
        lines.append(f'  {n:8} 0x{a0:08X} size 0x{s0:06X} -> 0x{a1:08X} size 0x{s1:06X}')
    for k in ('_SDA_BASE_', '_SDA2_BASE_', '_stack_addr', '__ArenaLo'):
        lines.append(f'  {k:12} 0x{e0.sym(k):08X} -> 0x{e1.sym(k):08X}')
    lines.append('static check:')
    for k, v in res.items(): lines.append(f'  {k}: {v}')
    lines += [f'  ! {b}' for b in bad]
    lines.append(f'result: {"PASS" if ok else "FAIL"} (static); boot it with tools/batches/batch11_shift_test.ps1')
    with open(REPORT, 'wb') as f: f.write(''.join(x + '\r\n' for x in lines).encode())
    print('\n'.join(lines[-(len(res) + len(bad) + 2):]))
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
