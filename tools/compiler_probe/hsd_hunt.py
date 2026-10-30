"""Source hunt, HSD (sysdolphin + HSD audio + particle): for every function of main.dol's HSD range, the closest
public reference and what kind of difference separates it (docs/compiler.md "Source hunt"; plan.md step 67).

References
  C, compiled with GC/1.3.2 and our pinned sysdolphin flags (docs/compiler.md), compared by name:
    ours      decomp/src/sysdolphin/baselib/*.c (the step 61 ports; their own header roots from decomp/configure.py)
    melee     doldecomp/melee (tools/melee, 24e685d)
    melee_latest  the 7 sysdolphin .c files doldecomp/melee changed up to 598987c (only .c files changed), downloaded
              from raw.githubusercontent.com into tools/refs_new/melee_latest/src/sysdolphin/baselib (optional)
    wj_testing    wowjinxy/KAR's older 'testing' branch: `git archive origin/testing src/sysdolphin include` of a clone,
              unpacked into tools/refs_new/kar_wj_testing (optional)
    wowjinxy  wowjinxy/KAR (tools/refs/kar_wowjinxy, main) src/sysdolphin/*.c, -DVERSION_GKYP01 (KAR PAL)
    kar       doldecomp/kar (tools/refs/kar) src/sysdolphin/*.c
  asm: doldecomp/kar's asm (Kirby Air Ride GKYE01), named through wowjinxy/KAR's complete GKYE01 symbol map; our fn_
  functions without a name are paired with the most similar KAR function of their own file (asm file = our split
  object; unsplit audio / particle: every KAR function within 0.6..1.6 of its size).
Compare: probe.py's masked compare (call targets, address halves, small-data offsets masked) on the C; for the asm,
revisions.py's masking on both sides.

Per function the report gives: closest reference and its score, KAR asm status (identical / li-only = assert line
numbers / differs), and the kinds of difference against the closest code (classify()):
  lines       only li immediates differ (assert / panic line numbers)
  regs        same instructions but register numbers (allocation: declaration / expression order)
  offsets     same load / store but another displacement (struct field moved, struct size changed)
  consts      other immediates (li / addi / cmpwi / rlwinm masks: changed constants or flags)
  +check      ours has extra compare-and-branch (an added NULL / range check)
  -check      the reference has a check ours lacks
  +call/-call calls only ours / only the reference makes (helper not inlined / inlined, extra call)
  +N/-N       instructions only in ours / only in the reference (other code)
  rewrite     under 50% of the words align: a different implementation
Output: tools/compiler_probe/out/hsd_hunt.txt (table) and hsd_hunt.tsv, gitignored; also printed (summary)
Usage : python -I tools/compiler_probe/hsd_hunt.py [--jobs N]
"""
import argparse
import concurrent.futures as cf
import difflib
import os
import re
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import probe as P       # noqa: E402
import revisions as Rv  # noqa: E402

G, R, N = P.G, P.G / 'tools' / 'refs', P.G / 'tools' / 'refs_new'
D = G / 'decomp'
HSD_LO, HSD_HI = 0x8016D42C, 0x801AEB40
PARTS = [('axdriver', 0x8016D42C, 0x80170ECC), ('devcom', 0x80170ECC, 0x80172DD8), ('synth', 0x80172DD8, 0x80179C14),
         ('particle+', 0x80179C14, 0x80186490), ('core', 0x80186490, HSD_HI)]
HSD_FLAGS = P.LIBS['hsd']['base']
COMMON_DEFS = ['-DBUILD_VERSION=0', '-DNDEBUG=1']


def melee_root():
    return P.MELEE


# doldecomp/melee's newest sysdolphin files that changed since tools/melee (24e685d): only .c files changed up to
# 598987c (aobj fobj fog jobj pobj robj tobj), fetched raw into tools/refs_new/melee_latest, compiled with tools/melee's
# headers
MELEE_LATEST = N / 'melee_latest/src/sysdolphin/baselib'


def git_head(p):
    try:
        import subprocess
        return subprocess.run(['git', '-C', str(p), 'rev-parse', '--short', 'HEAD'], capture_output=True,
                              text=True).stdout.strip()
    except OSError:
        return '?'


def c_sets():
    """name -> [(file, include flags, defines)]"""
    M = melee_root()
    minc = ['-cwd', 'source'] + sum([['-i', str(M / p)] for p in
                                     ('src', 'libs/doldecomp/include', 'src/MSL', 'libs/dolphin/include',
                                      'src/MSL/include', 'libs/runtime/include', 'libs/MSL/include')
                                     if (M / p).exists()], [])
    ours_def = ['-i', str(D / 'src'), '-i', str(D / 'include/melee/doldecomp'), '-i', str(D / 'include/melee/MSL'),
                '-i', str(D / 'include/melee/dolphin')]
    ours_kar = ['-cwd', 'explicit', '-i', str(D / 'include/cr_kar'), '-i', str(D / 'include/kar'),
                '-i', str(D / 'include/kar/sysdolphin')]
    cfg = (D / 'configure.py').read_text(encoding='utf-8')
    ours = []
    for f in sorted((D / 'src/sysdolphin/baselib').glob('*.c')):
        key = f'"sysdolphin/baselib/{f.name}": ["-cwd explicit"'
        inc = (ours_kar if key in cfg else ['-cwd', 'source'] + ours_def)
        ours.append((f, inc, ['-DVERSION_GXCE01', '-DMUST_MATCH'] + COMMON_DEFS))
    wj = R / 'kar_wowjinxy'
    mlinc = minc + ['-i', str(M / 'src/sysdolphin/baselib')]
    return {
        'melee_latest': [(f, mlinc, ['-DVERSION_GALE01', '-DMUST_MATCH', '-DBUILD_VERSION=2', '-DNDEBUG=1'])
                         for f in sorted(MELEE_LATEST.glob('*.c'))],
        'ours': ours,
        'melee': [(f, minc, ['-DVERSION_GALE01', '-DMUST_MATCH', '-DBUILD_VERSION=2', '-DNDEBUG=1'])
                  for f in sorted((M / 'src/sysdolphin/baselib').glob('*.c'))],
        'wowjinxy': [(f, ['-cwd', 'source', '-I-', '-i', str(wj / 'include')],
                      ['-DVERSION_GKYP01', '-DBUILD_VERSION=0', '-DNDEBUG=1'])
                     for f in sorted((wj / 'src/sysdolphin').glob('*.c'))],
        # wowjinxy/KAR's older 'testing' branch (git archive into tools/refs_new/kar_wj_testing), when present
        'wj_testing': [(f, ['-cwd', 'source', '-I-', '-i', str(N / 'kar_wj_testing/include')],
                        ['-DVERSION_GKYP01', '-DBUILD_VERSION=0', '-DNDEBUG=1'])
                       for f in sorted((N / 'kar_wj_testing/src/sysdolphin').glob('*.c'))],
        'kar': [(f, ['-cwd', 'source', '-I-', '-i', str(R / 'kar/include'), '-i', str(R / 'kar/src')],
                 ['-DVERSION_GKYE01', '-DBUILD_VERSION=0', '-DNDEBUG=1'])
                for f in sorted((R / 'kar/src/sysdolphin').glob('*.c'))],
    }


def compile_job(job):
    ref, src, inc, defs = job
    with tempfile.TemporaryDirectory(dir=P.OUT) as tmp:
        funcs, err = P.compile_one('1.3.2', HSD_FLAGS, ('x', ref, src, inc, defs, []), tmp)
    return job, funcs or {}, err


# ---- KAR asm ---------------------------------------------------------------------------------------------------------
def kar_asm():
    """[(file stem, address, name, words)] of every .text function in doldecomp/kar's asm"""
    wjmap = {}
    for line in (R / 'kar_wowjinxy/config/GKYE01/symbols.txt').read_text().splitlines():
        m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]+);', line)
        if m: wjmap[int(m[2], 16)] = m[1]
    out = []
    for f in sorted((R / 'kar/asm').rglob('*.s')):
        cur, adr, words = None, None, []
        for line in f.read_text(errors='replace').splitlines():
            g = re.match(r'^\.global\s+(\S+)', line)
            if g:
                if cur and words: out.append((f.stem, adr, cur, words))
                cur, adr, words = g[1], None, []
                continue
            h = re.match(r'/\* ([0-9A-F]{8}) [0-9A-F]{8}  ([0-9A-F]{2}) ([0-9A-F]{2}) ([0-9A-F]{2}) ([0-9A-F]{2}) \*/', line)
            if h and cur:
                if adr is None: adr = int(h[1], 16)
                words.append(int(''.join(h.groups()[1:]), 16))
        if cur and words: out.append((f.stem, adr, cur, words))
    return [(st, a, wjmap.get(a, n), w) for st, a, n, w in out]


# ---- classification --------------------------------------------------------------------------------------------------
def fields(w):
    return w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF


def is_cmp(w): return w >> 26 in (10, 11) or (w >> 26 == 31 and ((w >> 1) & 0x3FF) in (0, 32))
def is_bc(w): return w >> 26 == 16
def is_bl(w): return w >> 26 == 18 and w & 1


def classify(ours, ref):
    """masked word lists -> (kinds, equal fraction, detail)"""
    if ours == ref: return ['identical'], 1.0, ''
    if Rv.no_li(tuple(ours)) == Rv.no_li(tuple(ref)): return ['lines'], 1.0, ''
    sm = difflib.SequenceMatcher(None, ref, ours, autojunk=False)
    eq = sum(b.size for b in sm.get_matching_blocks())
    frac = eq / max(len(ours), len(ref))
    kinds, det = {}, []
    add = lambda k, n=1: kinds.__setitem__(k, kinds.get(k, 0) + n)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == 'equal': continue
        a, b = ref[i1:i2], ours[j1:j2]
        if op == 'replace' and len(a) == len(b):
            for x, y in zip(a, b):
                fx, fy = fields(x), fields(y)
                if fx[0] != fy[0]: add('other'); continue
                op_ = fx[0]
                if 32 <= op_ <= 55 and fx[1:3] == fy[1:3] and fx[3] != fy[3]:
                    add('offsets'); det.append(f'{op_ in (36, 37, 38, 44, 45, 52, 53, 54, 55) and "st" or "ld"}'
                                               f' +0x{fx[3]:X}->+0x{fy[3]:X}')
                elif op_ == 14 and fx[2] == 0 and fx[1] == fy[1]:
                    add('lines' if (fx[3] < 0x1000 and fy[3] < 0x1000) else 'consts')
                elif op_ in (10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 21) and fx[1:3] == fy[1:3]:
                    add('consts'); det.append(f'imm 0x{fx[3]:X}->0x{fy[3]:X}')
                elif (x ^ y) & 0x03FFF800 and not (x ^ y) & ~0x03FFF800 & 0xFFFFFFFF:
                    add('regs')
                elif op_ == 16 or op_ == 18:
                    add('branches')
                else:
                    add('other')
            continue
        if any(is_cmp(w) for w in b) and any(is_bc(w) for w in b) and len(b) <= 4 + len(a): add('+check')
        if any(is_cmp(w) for w in a) and any(is_bc(w) for w in a) and len(a) <= 4 + len(b): add('-check')
        nb, na = sum(map(is_bl, b)), sum(map(is_bl, a))
        if nb > na: add('+call', nb - na)
        if na > nb: add('-call', na - nb)
        if len(b) > len(a): add('+N', len(b) - len(a))
        if len(a) > len(b): add('-N', len(a) - len(b))
        if len(a) == len(b): add('other', len(a))
    if frac < 0.5: return ['rewrite'], frac, ''
    order = ['lines', 'regs', 'offsets', 'consts', '+check', '-check', '+call', '-call', 'branches', '+N', '-N', 'other']
    ks = [f'{k}' + (f'({kinds[k]})' if kinds[k] > 1 else '') for k in order if k in kinds]
    return ks, frac, ' '.join(dict.fromkeys(det))[:120]


def suggest(kinds, ref_is_c, best_c):
    k = ' '.join(kinds)
    start = f' (start from {best_c})' if best_c else ''
    src = 'the C' if ref_is_c else 'C written from the KAR asm' + start
    if kinds == ['identical']: return 'write C to the KAR asm, no edit needed' + start
    if kinds == ['lines']: return f'{src}, with our assert line numbers'
    if kinds == ['rewrite']: return 'write from our asm (no close reference)'
    edits = []
    if 'lines' in k: edits.append('assert lines')
    if 'regs' in k: edits.append('declaration / expression order (register allocation)')
    if 'offsets' in k: edits.append('struct layout (field offsets)')
    if 'consts' in k: edits.append('constants / flag masks')
    if '+check' in k: edits.append('add the extra check')
    if '-check' in k: edits.append('drop a check')
    if '+call' in k or '-call' in k: edits.append('call / inline changes')
    if '+N' in k or '-N' in k or 'other' in k or 'branches' in k: edits.append('code changes')
    return f'{src}, then: ' + ', '.join(edits)


BUCKETS = ['KAR asm identical', 'assert lines only', 'small edit (>=90%)', 'moderate (70-90%)', 'rewrite (<70%)',
           'no reference']


def bucket(r):
    status, kinds, frac = r[6], r[8], r[9]
    if status == 'none': return 'no reference'
    if kinds == 'identical': return 'KAR asm identical'
    if kinds == 'lines': return 'assert lines only'
    if kinds == 'rewrite' or frac < 0.7: return 'rewrite (<70%)'
    return 'small edit (>=90%)' if frac >= 0.9 else 'moderate (70-90%)'


# ---- main ------------------------------------------------------------------------------------------------------------
def masked_ours(a, s):
    data = Rv.DOL.read(a, s)
    return list(Rv.mask_words([struct.unpack_from('>I', data, i)[0] for i in range(0, s, 4)]))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    a = ap.parse_args()
    P.OUT.mkdir(parents=True, exist_ok=True)
    Rv.DOL = P.Dol(P.DOL)
    rng = Rv.objects()
    import sources as S
    ours = [(n, ad, s) for n, ad, s in S.all_functions() if HSD_LO <= ad < HSD_HI]
    by_name = {}
    for n, ad, s in ours: by_name.setdefault(n, []).append((ad, s))

    # C references
    sets = c_sets()
    jobs = [(ref, f, inc, defs) for ref, lst in sets.items() for f, inc, defs in lst]
    cres, errs = {}, []
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        for (ref, src, inc, defs), funcs, err in ex.map(compile_job, jobs):
            if err: errs.append(f'{ref} {src.name}: {err}')
            for name, (code, masks) in funcs.items():
                for ad, sz in by_name.get(name, []):
                    e, sc = P.compare(code, masks, Rv.DOL.read(ad, sz))
                    d = cres.setdefault(ad, {})
                    if (e, sc) > d.get(ref, (False, -1.0))[:2]:
                        d[ref] = (e, sc, src.name, len(code), code)

    # KAR asm
    kf = kar_asm()
    kmask = [(st, ka, kn, list(Rv.mask_words(w))) for st, ka, kn, w in kf]
    sys_stems = {f.stem for f in (R / 'kar/asm/sysdolphin').glob('*.s')}
    kname = {}
    for st, ka, kn, mw in kmask: kname.setdefault(kn, []).append((st, ka, mw))
    kexact, ksoft = {}, {}
    for st, ka, kn, mw in kmask:
        kexact.setdefault(tuple(mw), (st, ka, mw)); ksoft.setdefault(Rv.no_li(tuple(mw)), (st, ka, mw))
    placeholder = re.compile(r'^(fn|func|lbl|kar|zz)_|_[0-9A-Fa-f]{8}$|__near_')

    rows, L = [], []
    heads = {'melee': git_head(melee_root()) + (', melee_latest 598987c (7 files)' if MELEE_LATEST.exists() else ''), 'wowjinxy': git_head(R / 'kar_wowjinxy'), 'kar': git_head(R / 'kar')}
    for n, ad, s in sorted(ours, key=lambda x: x[1]):
        obj = Rv.obj_of(rng, ad) or ''
        part = next(p for p, lo, hi in PARTS if lo <= ad < hi)
        mo = masked_ours(ad, s)
        kstat = 'identical' if tuple(mo) in kexact else 'li-only' if Rv.no_li(tuple(mo)) in ksoft else 'differs'
        # KAR asm partner: same name, else most similar in the same file / size band
        cand = [] if placeholder.search(n) else kname.get(n, [])
        how = 'name'
        if kstat != 'differs' and not any(tuple(mw) == tuple(mo) or Rv.no_li(tuple(mw)) == Rv.no_li(tuple(mo))
                                          for *_, mw in cand):
            cand = [kexact.get(tuple(mo)) or ksoft[Rv.no_li(tuple(mo))]]
            how = 'bytes'
        if not cand:
            stem = Path(obj).stem if obj else None
            pool = [(st, ka, mw) for st, ka, kn, mw in kmask
                    if (st == stem if stem in sys_stems else st in sys_stems or st == 'text_804452A0')
                    and 0.6 <= len(mw) / max(1, len(mo)) <= 1.6]
            best = None
            for st, ka, mw in pool:
                sm = difflib.SequenceMatcher(None, mw, mo, autojunk=False)
                if sm.real_quick_ratio() < (best[0] if best else 0.3) or sm.quick_ratio() < (best[0] if best else 0.3):
                    continue
                r = sm.ratio()
                if not best or r > best[0]: best = (r, st, ka, mw)
            if not best or best[0] < 0.6:     # anywhere in KAR's code
                for st, ka, kn, mw in kmask:
                    if not 0.6 <= len(mw) / max(1, len(mo)) <= 1.6: continue
                    sm = difflib.SequenceMatcher(None, mw, mo, autojunk=False)
                    lim = best[0] if best else 0.3
                    if sm.real_quick_ratio() < lim or sm.quick_ratio() < lim: continue
                    r = sm.ratio()
                    if not best or r > best[0]: best = (r, st, ka, mw)
            cand = [best[1:]] if best else []
            how = 'similar'
        kar_best = None
        for st, ka, mw in cand:
            ks, fr, det = classify(mo, mw)
            if not kar_best or fr > kar_best[2]: kar_best = (st, ka, fr, ks, det)
        d = cres.get(ad, {})
        exact_refs = sorted(k for k, v in d.items() if v[0])
        best_c = max(d.items(), key=lambda x: (x[1][0], x[1][1]), default=None)
        if exact_refs:
            status, closest, kinds, frac, det = 'exact C', '+'.join(f'{k}:{d[k][2]}' for k in exact_refs), [], 1.0, ''
        else:
            # closest: the best C (score) or the KAR asm, whichever aligns more words
            cscore = best_c[1][1] if best_c else -1
            kscore = kar_best[2] if kar_best else -1
            if kar_best and kscore >= cscore:
                status = 'asm'
                kn = next((kn for st, ka, kn, mw in kmask if ka == kar_best[1]), '?')
                closest = f'kar-asm:{kar_best[0]}.s:{kn}@{kar_best[1]:08X}' + ('' if how == 'name' else f'({how})')
                kinds, frac, det = kar_best[3], kar_best[2], kar_best[4]
            elif best_c:
                status = 'C'
                closest = f'{best_c[0]}:{best_c[1][2]}'
                cw = list(Rv.mask_words([struct.unpack_from('>I', best_c[1][4], i)[0]
                                         for i in range(0, len(best_c[1][4]), 4)]))
                kinds, frac, det = classify(mo, cw)
                det = (det + f' C size 0x{best_c[1][3]:X}').strip()
                if kar_best: det += f'; kar asm {kar_best[2] * 100:.0f}% {" ".join(kar_best[3])}'
            else:
                status, closest, kinds, frac, det = 'none', '-', ['no reference'], 0.0, ''
        cs = ' '.join(f'{k}:{"EXACT" if v[0] else f"{v[1] * 100:.0f}%"}' for k, v in sorted(d.items()))
        bc = f'{best_c[0]}:{best_c[1][2]} {best_c[1][1] * 100:.0f}%' if best_c and best_c[1][1] >= 0.5 else ''
        if status == 'asm' and frac < 0.5: status, closest, kinds = 'none', '-', ['no reference']
        sug = ('import' if status == 'exact C' else suggest(kinds, status == 'C', bc) if status != 'none'
               else 'write from our asm' + (f' (nearest C {bc})' if bc else ''))
        rows.append((part, obj, n, ad, s, kstat, status, closest, ' '.join(kinds), frac, det, cs, sug))

    # report
    L.append(f'references: melee {melee_root().relative_to(G).as_posix()} @ {heads["melee"]}, wowjinxy/KAR main @ '
             f'{heads["wowjinxy"]}, doldecomp/kar @ {heads["kar"]} (asm + src), ours decomp/src/sysdolphin')
    if errs: L += ['compile errors (those files give no functions):'] + ['  ' + e for e in errs]
    L.append('== summary per part: functions; exact C (any reference incl. ours / new: not in ours); KAR asm '
             'identical / li-only; the rest by distance from the closest reference')
    for p, lo, hi in PARTS:
        rs = [r for r in rows if r[0] == p]
        ex = [r for r in rs if r[6] == 'exact C']
        new = sum('ours:' not in r[7] for r in ex)
        kid = sum(r[5] == 'identical' for r in rs); kli = sum(r[5] == 'li-only' for r in rs)
        b = {}
        for r in rs:
            if r[6] != 'exact C': b[bucket(r)] = b.get(bucket(r), 0) + 1
        L.append(f'  {p:10s} {len(rs):4d} functions: exact C {len(ex)} (new {new}); KAR asm identical {kid} + li-only '
                 f'{kli}; not exact: ' + ', '.join(f'{k} {b[k]}' for k in BUCKETS if k in b))
    L.append('== per function (not exact in C): part, object, name, address, size, KAR asm status, closest reference, '
             'kinds of difference, aligned words, detail | C scores | suggestion')
    for r in rows:
        if r[6] == 'exact C': continue
        L.append(f'  {r[0]:9s} {Path(r[1]).name or "-":14s} {r[2]:36s} {r[3]:08X} 0x{r[4]:<5X} {r[5]:9s} {r[7]} '
                 f'[{r[8]}] {r[9] * 100:.0f}% {r[10]} | {r[11]} | {r[12]}')
    L.append('== exact C (the reference to import from)')
    for r in rows:
        if r[6] == 'exact C': L.append(f'  {r[0]:9s} {Path(r[1]).name or "-":14s} {r[2]:36s} {r[3]:08X} {r[7]}')
    text = '\n'.join(L) + '\n'
    (P.OUT / 'hsd_hunt.txt').write_bytes(text.encode())
    with open(P.OUT / 'hsd_hunt.tsv', 'w', encoding='utf-8', newline='\n') as f:
        f.write('part\tobject\tname\taddress\tsize\tkar_asm\tstatus\tclosest\tkinds\taligned\tdetail\tc_scores\tsuggestion\n')
        for r in rows:
            f.write('\t'.join([r[0], r[1], r[2], f'{r[3]:08X}', f'0x{r[4]:X}', r[5], r[6], r[7], r[8], f'{r[9]:.3f}',
                               r[10], r[11], r[12]]) + '\n')
    print('\n'.join(L[:12]))
    print(f'wrote {P.OUT / "hsd_hunt.txt"}')


if __name__ == '__main__':
    main()
