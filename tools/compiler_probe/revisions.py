"""Which library SOURCE REVISIONS built main.dol (docs/compiler.md "Source revisions"; plan.md step 66).

1. Version strings: every "<< Dolphin SDK - ... >>" string, the OS kernel date, the MetroTRK banner, the Metrowerks
   runtime strings and the game's own build stamp in main.dol (file offset, address), plus the apploader's.
2. Cross-reference: compiles the same library file from every reference project in tools/refs (and tools/melee) with
   the library's pinned compiler / flags (docs/compiler.md) and compares every function with main.dol's bytes (probe.py's
   masked compare). Per function: which references reproduce it exactly.
3. HSD: our sysdolphin / HSD audio functions against doldecomp/kar's asm (Kirby Air Ride), masked the same way on both
   sides (call targets, @ha/@l halves of lis pairs, r2/r13 small-data offsets); "li-only" = equal but for li immediates
   (assert line numbers).

Inputs : decomp/orig/GXCE01/sys/main.dol, Custom Robo extracted/sys/apploader.img (optional), decomp/config/GXCE01,
         tools/refs/*, tools/melee, decomp/build/compilers/GC (probe.py's)
Output : tools/compiler_probe/out/revisions.txt (gitignored), also printed
Usage  : python -I tools/compiler_probe/revisions.py [--jobs N] [--part strings,sdk,msl,trk,hsd]
"""
import argparse
import concurrent.futures as cf
import os
import re
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import probe as P   # noqa: E402

G, R, M = P.G, P.G / 'tools' / 'refs', P.MELEE
APPLOADER = G / 'Custom Robo extracted' / 'sys' / 'apploader.img'


def I(*ps, r=()):
    return ['-I-'] + sum([['-i', str(p)] for p in ps], []) + sum([['-ir', str(p)] for p in r], [])


# ---- reference projects per library: name -> (source root, include flags, defines) ----------------------------------
SDK_REFS = {
    'dolsdk2004': (R / 'dolsdk2004/src', I(R / 'dolsdk2004/include', R / 'dolsdk2004/include/libc', r=[R / 'dolsdk2004/src']),
                   ['-D__GEKKO__', '-DSDK_REVISION=0', '-DRELEASE', '-DNDEBUG']),
    'dolsdk2001': (R / 'dolsdk2001/src', I(R / 'dolsdk2001/include', R / 'dolsdk2001/include/libc', r=[R / 'dolsdk2001/src']),
                   ['-D__GEKKO__', '-DRELEASE', '-DNDEBUG']),
    'xd': (R / 'xd/src/dolphin', I(R / 'xd/include', R / 'xd/include/libc', r=[R / 'xd/src/dolphin']),
           ['-D__GEKKO__', '-DSDK_REVISION=0', '-DRELEASE', '-DNDEBUG']),
    # BfBB's MATCHING picks OSErrorLine(line, ...) = OSPanic(__FILE__, line, ...) (VIConfigure's line 1908)
    'bfbb': (R / 'bfbb/src/dolphin/src', I(R / 'bfbb/src/dolphin/include', R / 'bfbb/include',
                                           R / 'bfbb/src/PowerPC_EABI_Support/include', r=[R / 'bfbb/src/dolphin/src']),
             ['-D__GEKKO__', '-DNDEBUG', '-DMATCHING']),
    'mp4': (R / 'marioparty4/src/dolphin', I(R / 'marioparty4/include', R / 'marioparty4/include/dolphin',
                                             r=[R / 'marioparty4/src/dolphin']), ['-D__GEKKO__', '-DNDEBUG', '-DVERSION=0']),
    'ac': (R / 'ac-decomp/src/static/dolphin', I(R / 'ac-decomp/include', R / 'ac-decomp/include/libc',
                                                 r=[R / 'ac-decomp/src/static/dolphin']), ['-D__GEKKO__', '-DNDEBUG']),
    'sms': (R / 'sms/libs/dolphin/src', I(R / 'sms/libs/dolphin/include', R / 'sms/include',
                                          R / 'sms/libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common',
                                          r=[R / 'sms/libs/dolphin/src', R / 'sms/libs']), ['-D__GEKKO__', '-DNDEBUG']),
    'pikmin': (R / 'pikmin/src', I(R / 'pikmin/include', R / 'pikmin/include/stl'), ['-D__GEKKO__', '-DNDEBUG']),
}
SDK_FLAGS = {   # GC/1.2.5n, docs/compiler.md; per-file variants seen in the SDK
    'base': ['-O4,p', '-inline', 'auto', '-char', 'unsigned', '-fp_contract', 'off'],
    'O3': ['-O3,p', '-inline', 'auto', '-char', 'unsigned', '-fp_contract', 'off'],
    'signed': ['-O4,p', '-inline', 'auto', '-char', 'signed', '-fp_contract', 'off'],
}
SDK_FILES = ['os/OS.c', 'os/OSAlloc.c', 'os/OSFont.c', 'os/OSReboot.c', 'os/OSReset.c', 'gx/GXAttr.c', 'gx/GXGeometry.c',
             'gx/GXInit.c', 'gx/GXLight.c', 'gx/GXTev.c', 'gx/GXPixel.c', 'gx/GXFrameBuf.c', 'ax/AXVPB.c',
             'card/CARDBios.c', 'card/CARDMount.c', 'card/CARDRdwr.c', 'vi/vi.c', 'exi/EXIBios.c', 'exi/EXIUart.c',
             'thp/THPDec.c', 'thp/THPAudio.c']


def lib_sets(lib):
    """MSL / Runtime and MetroTRK sources of every reference that has them: name -> ([files], include flags)"""
    if lib == 'trk':
        return {
            'mp4': (sorted((R / 'marioparty4/src/TRK_MINNOW_DOLPHIN').glob('*.c')),
                    I(R / 'marioparty4/include', R / 'marioparty4/include/dolphin')),
            'melee': (sorted((M / 'src/MetroTRK').glob('*.c')), P.MELEE_INC),
            'ac': (sorted((R / 'ac-decomp/src/static/TRK_MINNOW_DOLPHIN').glob('*.c')),
                   I(R / 'ac-decomp/include', R / 'ac-decomp/include/libc')),
            'pikmin': (sorted((R / 'pikmin/src/TRK_MINNOW_DOLPHIN').glob('*.c')),
                       I(R / 'pikmin/include', R / 'pikmin/include/stl')),
            'sms': (sorted((R / 'sms/libs/TRK_MINNOW_DOLPHIN').rglob('*.c')),
                    I(R / 'sms/libs/TRK_MINNOW_DOLPHIN/include', R / 'sms/libs/dolphin/include',
                      R / 'sms/libs/PowerPC_EABI_Support/include',
                      R / 'sms/libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common',
                      R / 'sms/include')),
            'bfbb': (sorted((R / 'bfbb/src/runtime_libs/debugger').rglob('*.c')),
                     I(R / 'bfbb/src/runtime_libs/debugger/include', R / 'bfbb/src/runtime_libs/debugger/embedded/MetroTRK',
                       R / 'bfbb/src/dolphin/include', R / 'bfbb/include', R / 'bfbb/src/PowerPC_EABI_Support/include')),
            'kar': ([R / 'kar_wowjinxy/src/metrotrk.c', R / 'kar_wowjinxy/src/metrotrk_bridge.c'],
                    I(R / 'kar_wowjinxy/include')),
        }
    return {
        'mp4': (sorted((R / 'marioparty4/src/MSL_C.PPCEABI.bare.H').glob('*.c'))
                + sorted((R / 'marioparty4/src/Runtime.PPCEABI.H').glob('*.c')),
                I(R / 'marioparty4/include', R / 'marioparty4/include/dolphin')),
        'melee': (sorted((M / 'src/MSL').glob('*.c')) + sorted((M / 'src/Runtime').glob('*.c')), P.MELEE_INC),
        'ac': (sorted((R / 'ac-decomp/src/static/MSL_C.PPCEABI.bare.H').glob('*.c'))
               + sorted((R / 'ac-decomp/src/static/Runtime.PPCEABI.H').glob('*.c')),
               I(R / 'ac-decomp/include', R / 'ac-decomp/include/libc', R / 'ac-decomp/include/MSL_C')),
        'pikmin': (sorted((R / 'pikmin/src/MSL_C').rglob('*.c')) + sorted((R / 'pikmin/src/Runtime').rglob('*.c')),
                   I(R / 'pikmin/include', R / 'pikmin/include/stl')),
        'sms': (sorted((R / 'sms/libs/PowerPC_EABI_Support/src').rglob('*.c')),
                I(R / 'sms/libs/PowerPC_EABI_Support/include',
                  R / 'sms/libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common',
                  R / 'sms/libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded',
                  R / 'sms/libs/dolphin/include', R / 'sms/include')),
        'bfbb': (sorted((R / 'bfbb/src/PowerPC_EABI_Support/src').rglob('*.[cC]')),
                 I(R / 'bfbb/src/PowerPC_EABI_Support/include', R / 'bfbb/src/dolphin/include', R / 'bfbb/include')),
        'kar': ([R / 'kar_wowjinxy/src/runtime_libc.c', R / 'kar_wowjinxy/src/runtime.c',
                 R / 'kar_wowjinxy/src/runtime_support.c'], I(R / 'kar_wowjinxy/include')),
    }


# ---- helpers ---------------------------------------------------------------------------------------------------------
def objects():
    """[(start, end, object)] of every .text / .init range in splits.txt"""
    rng, cur = [], None
    for line in (G / 'decomp/config/GXCE01/splits.txt').read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(':'): cur = line[:-1]
        m = re.search(r'\.(?:text|init)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)', line)
        if m and cur: rng.append((int(m[1], 16), int(m[2], 16), cur))
    return rng


def obj_of(rng, a):
    return next((o for s, e, o in rng if s <= a < e), None)


def strings(data, pat):
    return [(m.start(), m.group().decode()) for m in re.finditer(rb'[\x20-\x7e\t]{5,}', data) if re.search(pat, m.group())]


def compile_compare(job):
    """job = (ref, src, version, flags, includes, defines) -> (ref, src, {function: (exact, score)} or None)"""
    ref, src, ver, flags, inc, defs = job
    with tempfile.TemporaryDirectory(dir=P.OUT) as tmp:
        funcs, err = P.compile_one(ver, flags, ('x', ref, src, inc, defs, []), tmp)
    if err: return ref, src, None
    out = {}
    for name, (code, masks) in funcs.items():
        if name in OURS:
            a, s = OURS[name]
            out[name] = P.compare(code, masks, DOL.read(a, s))
    return ref, src, out


def cross_reference(jobs, nproc, keep):
    """per function: {ref: (exact, best score, file)} over all jobs"""
    res = {}
    with cf.ThreadPoolExecutor(nproc) as ex:
        for ref, src, out in ex.map(compile_compare, jobs):
            for name, (e, sc) in (out or {}).items():
                if not keep(OURS[name][0]): continue
                d = res.setdefault(name, {})
                if (e, sc) > d.get(ref, (False, 0.0, ''))[:2]: d[ref] = (e, sc, src.name)
    return res


def report_xref(title, res, rng, pred, current):
    L = [f'== {title}']
    fs = sorted((a, n) for n, (a, s) in OURS.items() if pred(obj_of(rng, a) or ''))
    count = {}
    for a, n in fs:
        d = res.get(n, {})
        exs = sorted(k for k, v in d.items() if v[0])
        if any(k in current for k in exs): key = 'exact with the pinned reference'
        elif exs: key = 'exact only with ' + '+'.join(exs)
        elif d: key = 'no reference exact'
        else: key = 'no reference source'
        count[key] = count.get(key, 0) + 1
        if key != 'exact with the pinned reference':
            best = ' '.join(f'{k}:{"EXACT" if v[0] else f"{v[1] * 100:.0f}%"}'
                            for k, v in sorted(d.items(), key=lambda x: (-x[1][0], -x[1][1]))[:4])
            L.append(f'  {a:08X} {n:34s} {obj_of(rng, a):34s} {best}')
    L.append('  totals: ' + ', '.join(f'{k} {v}' for k, v in sorted(count.items(), key=lambda x: -x[1])))
    return L


# ---- HSD vs doldecomp/kar asm ----------------------------------------------------------------------------------------
def mask_words(words):
    out, lis = [], set()
    for w in words:
        op, rd, ra = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        m = w
        if op == 18: m = w & 0xFC000003                                   # b / bl target
        elif op == 15 and ra == 0: m = w & 0xFFFF0000; lis.add(rd)        # lis rX, sym@ha
        elif op in (12, 13, 14) + tuple(range(32, 56)) and (ra in (2, 13) or ra in lis):
            m = w & 0xFFFF0000                                            # sym@l(rX), sda21
        elif rd in lis and op in (14, 15, 21, 24, 31, 32, 33, 34, 35, 40, 41, 42, 43):
            lis.discard(rd)
        out.append(m)
    return tuple(out)


def no_li(t):
    return tuple((w & 0xFFFF0000) if w >> 26 == 14 and (w >> 16) & 31 == 0 else w for w in t)


def hsd_vs_kar(rng):
    kf = {}
    for f in (R / 'kar' / 'asm').rglob('*.s'):
        cur, words = None, []
        for line in f.read_text(errors='replace').splitlines():
            g = re.match(r'^\.global\s+(\S+)', line)
            if g:
                if cur and words: kf[(f.stem, cur)] = words
                cur, words = g[1], []
                continue
            h = re.match(r'/\* [0-9A-F]{8} [0-9A-F]{8}  ([0-9A-F]{2}) ([0-9A-F]{2}) ([0-9A-F]{2}) ([0-9A-F]{2}) \*/', line)
            if h and cur: words.append(int(''.join(h.groups()), 16))
        if cur and words: kf[(f.stem, cur)] = words
    exact, soft = set(), set()
    for w in kf.values():
        t = mask_words(w); exact.add(t); soft.add(no_li(t))
    parts = [('sysdolphin core (0x80186490-0x801AE9B4)', 0x80186490, 0x801AE9B4),
             ('HSD audio: axdriver (0x8016D42C-0x80170ECC)', 0x8016D42C, 0x80170ECC),
             ('HSD audio: devcom (0x80170ECC-0x80172DD8)', 0x80170ECC, 0x80172DD8),
             ('HSD audio: synth (0x80172DD8-0x80179C14)', 0x80172DD8, 0x80179C14),
             ('particle and the rest up to the core (0x80179C14-0x80186490)', 0x80179C14, 0x80186490)]
    L = ['== HSD vs doldecomp/kar asm (functions / bytes identical, + identical but for li immediates)']
    for title, lo, hi in parts:
        n = [0, 0, 0]; b = [0, 0, 0]
        for name, (a, s) in OURS.items():
            if not lo <= a < hi: continue
            data = DOL.read(a, s)
            t = mask_words([struct.unpack_from('>I', data, i)[0] for i in range(0, s, 4)])
            k = 0 if t in exact else 1 if no_li(t) in soft else 2
            n[k] += 1; b[k] += s
        tot = max(1, sum(b))
        L.append(f'  {title}: {sum(n)} functions: {n[0]} identical, {n[1]} li-only, {n[2]} differ; bytes '
                 f'{b[0] * 100 // tot}% / {b[1] * 100 // tot}% / {b[2] * 100 // tot}%')
    return L


def main():
    global OURS, DOL
    ap = argparse.ArgumentParser()
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--part', default='strings,sdk,msl,trk,hsd')
    a = ap.parse_args()
    parts = a.part.split(',')
    P.OUT.mkdir(parents=True, exist_ok=True)
    OURS, DOL = P.our_functions(), P.Dol(P.DOL)
    rng = objects()
    L = []
    if 'strings' in parts:
        L.append('== version strings in main.dol (file offset, address)')
        for off, s in strings(DOL.d, rb'Dolphin SDK|MetroTRK for|Metrowerks|Kernel built|^\[[A-Z][a-z]{2} [ 0-9]\d 20\d\d|'
                                         rb'^(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec) [ 0-9]\d 20\d\d$|'
                                         rb'^\d\d:\d\d:\d\d$|^20\d\d/\d\d/\d\d'):
            adr = next((b + off - o for o, b, z in zip(DOL.off, DOL.adr, DOL.sz) if z and o <= off < o + z), 0)
            L.append(f'  0x{off:06X} 0x{adr:08X} {s!r}')
        if APPLOADER.exists():
            L.append('== apploader.img')
            for off, s in strings(APPLOADER.read_bytes(), rb'Dolphin SDK|release build|^20\d\d/|built|Sep|Revision'):
                L.append(f'  0x{off:05X} {s!r}')
    if 'sdk' in parts:
        jobs = []
        for ref, (root, inc, defs) in SDK_REFS.items():
            for f in SDK_FILES:
                src = root / f
                if not src.exists():
                    c = [p for p in root.rglob(Path(f).name)]
                    if not c: continue
                    src = c[0]
                for fl in SDK_FLAGS.values(): jobs.append((ref, src, '1.2.5n', fl, inc, defs))
        res = cross_reference(jobs, a.jobs, lambda adr: True)
        L += report_xref('Dolphin SDK: functions of the probed objects not reproduced by dolsdk2004', res, rng,
                         lambda o: o.startswith('dolphin/') and any(o.endswith(x) for x in
                                                                     [Path(f).name for f in SDK_FILES]), ('dolsdk2004',))
    for lib, title, pred in (('msl', 'MSL + Runtime', lambda o: o.startswith(('MSL/', 'Runtime/'))),
                             ('trk', 'MetroTRK', lambda o: o.startswith('MetroTRK/'))):
        if lib not in parts: continue
        flags = [P.LIBS['trk']['base']] if lib == 'trk' else [P.LIBS['msl']['base'] + ['-char', 'signed'],
                                                               P.LIBS['runtime']['base']]
        jobs = [(ref, s, '2.6', fl, inc, ['-DNDEBUG=1']) for ref, (srcs, inc) in lib_sets(lib).items()
                for s in srcs for fl in flags]
        res = cross_reference(jobs, a.jobs, lambda adr: True)
        L += report_xref(f'{title}: functions not reproduced by Mario Party 4 / Melee (the pinned references)', res,
                         rng, pred, ('mp4', 'melee'))
    if 'hsd' in parts:
        L += hsd_vs_kar(rng)
    text = '\n'.join(L) + '\n'
    (P.OUT / 'revisions.txt').write_bytes(text.encode())
    print(text)
    print(f'wrote {P.OUT / "revisions.txt"}')


if __name__ == '__main__':
    main()
