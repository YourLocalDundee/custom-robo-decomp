"""Source hunt: for every MetroTRK / MSL / Runtime function, which public source compiles to main.dol's exact bytes
(docs/compiler.md "Source hunt"; plan.md step 67). Extends revisions.py (step 66) with the 2004-2006 references that
have the CodeWarrior 2.6-era TRK / MSL source: Twilight Princess (zeldaret/tp) and Pikmin 2 (doldecomp/pikmin2),
cloned into the gitignored tools/refs_new by --fetch (shallow, sparse: only their TRK / MSL / Runtime folders and
headers).

Every source is compiled with GC/2.6 and our pinned flags for the library (docs/compiler.md table) plus the per-file
variants the reference itself uses (-char signed, -sdata 8, -rostr, -str reuse,nopool), and every function is compared
with the retail bytes (probe.py's masked compare). A function counts as exact when one variant reproduces it.

Inputs : as revisions.py, plus tools/refs_new/{tp,pikmin2}
Output : tools/compiler_probe/out/sources.txt (gitignored), also printed
Usage  : python -I tools/compiler_probe/sources.py [--fetch] [--jobs N] [--part sdk,trk,msl]
"""
import argparse
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import probe as P       # noqa: E402
import revisions as Rv  # noqa: E402

N = P.G / 'tools' / 'refs_new'
FETCH = {   # name: (url, commit tested, sparse paths)
    'tp': ('https://github.com/zeldaret/tp', 'c8fa8c9',
           ['/include/', '/libs/TRK_MINNOW_DOLPHIN/', '/libs/PowerPC_EABI_Support/', '/libs/dolphin/include/',
            '/libs/dolphin/src/', '/configure.py']),
    'pikmin2': ('https://github.com/doldecomp/pikmin2', '999140f', ['/include/', '/src/Dolphin/', '/configure.py']),
}
I = Rv.I
TP, PK = N / 'tp', N / 'pikmin2'
TP_INC = I(TP / 'include', TP / 'libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include',
           TP / 'libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Include',
           TP / 'libs/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include', TP / 'libs/PowerPC_EABI_Support/Runtime/Inc',
           TP / 'libs/PowerPC_EABI_Support/MetroTRK', TP / 'libs/dolphin/include', TP / 'libs/dolphin/include/dolphin')
TP_DEF = ['-DVERSION=0', '-D__GEKKO__', '-DNDEBUG=1']
PK_INC = I(PK / 'include', PK / 'include/stl')
PK_DEF = ['-DVERSION_GPVE01', '-DNDEBUG=1']
# our unnamed function -> the reference's name, by position and size in XD / TP / Pikmin 2 (step 66); compiled under
# the reference's name and compared with our bytes
ALIAS = {'fn_8015E09C': 'TRK__write_aram', 'fn_8015E288': 'TRK__read_aram', 'fn_800053E0': '__TRK_reset'}
# SDK: by position (the first function of reverb_std.c in dolsdk2004's order), scored under that name
SDK_ALIAS = {'fn_80148998': 'ReverbSTDCreate'}
# assembler sources (.s) of the references: function -> instruction words, compared directly
ASM = {n: [0x0FE00000, 0x4E800020] for n in ('TRKAccessFile', 'TRKOpenFile', 'TRKCloseFile', 'TRKPositionFile')}
ASM_SRC = 'tp/pikmin2 targsupp.s'


def fetch():
    N.mkdir(exist_ok=True)
    env = dict(os.environ, MSYS_NO_PATHCONV='1')
    for name, (url, _, paths) in FETCH.items():
        d = N / name
        if not d.exists():
            subprocess.run(['git', 'clone', '-q', '--depth', '1', '--filter=blob:none', '--no-checkout', url, str(d)],
                           check=True)
        subprocess.run(['git', '-C', str(d), 'sparse-checkout', 'set', '--no-cone'] + paths, check=True, env=env)


def sets(lib):
    """name -> ([files], includes, defines)"""
    if lib == 'trk':
        new = {'tp': (sorted((TP / 'libs/TRK_MINNOW_DOLPHIN').rglob('*.c')), TP_INC, TP_DEF),
               'pikmin2': (sorted((PK / 'src/Dolphin/TRK_MINNOW_DOLPHIN').rglob('*.c'))
                           + sorted((PK / 'src/Dolphin/TRK_MINNOW_DOLPHIN').rglob('*.cpp')), PK_INC, PK_DEF)}
    else:
        new = {'tp': (sorted((TP / 'libs/PowerPC_EABI_Support/MSL/MSL_C').rglob('*.c'))
                      + sorted((TP / 'libs/PowerPC_EABI_Support/Runtime/Src').glob('*.c')), TP_INC, TP_DEF),
               'pikmin2': (sorted((PK / 'src/Dolphin/MSL_C').rglob('*.c'))
                           + sorted((PK / 'src/Dolphin/Runtime').rglob('*.c')), PK_INC, PK_DEF)}
    old = {k: (v[0], v[1], ['-DNDEBUG=1']) for k, v in Rv.lib_sets(lib).items()}
    return {**old, **new}


def flag_sets(lib):
    if lib == 'trk':
        b = P.LIBS['trk']['base']
        return [b, b + ['-char', 'signed'], b + ['-sdata', '8'], b + ['-char', 'signed', '-sdata', '8'],
                b + ['-char', 'signed', '-rostr']]
    m, r = P.LIBS['msl']['base'] + ['-char', 'signed'], P.LIBS['runtime']['base']
    return [m, r, r + ['-char', 'signed'], m + ['-inline', 'deferred,auto'], r + ['-str', 'reuse,nopool,readonly'],
            r + ['-char', 'signed', '-inline', 'deferred,auto', '-str', 'reuse,nopool,readonly']]


def cross_reference(jobs, nproc):
    """like revisions.cross_reference, but remembers the first flag set that made each function exact:
    {function: {ref: (exact, score, file, extra flags)}}"""
    import concurrent.futures as cf
    res = {}
    with cf.ThreadPoolExecutor(nproc) as ex:
        for job, (ref, src, out) in zip(jobs, ex.map(Rv.compile_compare, jobs)):
            for name, (e, sc) in (out or {}).items():
                d = res.setdefault(name, {})
                if (e, sc) > d.get(ref, (False, 0.0))[:2]: d[ref] = (e, sc, src.name, flag_tag(job[3]))
    return res


def flag_tag(fl):
    """the flag set's difference from the library's pinned flags ('' = pinned; 'runtime' = Runtime's pinned set
    used on an MSL file or the other way round)"""
    m, r, t = P.LIBS['msl']['base'] + ['-char', 'signed'], P.LIBS['runtime']['base'], P.LIBS['trk']['base']
    for b, tag in ((t, ''), (m, ''), (r, 'runtime')):
        if fl[:len(b)] == b: return ' '.join(([tag] if tag else []) + fl[len(b):])
    return ' '.join(fl)


# ---- Dolphin SDK: every function of the SDK's address ranges against every file of BfBB's and dolsdk2004's SDK ------
SDK_RANGES = [(0x80003100, 0x80003458), (0x80131C08, 0x80159530), (0x801AEB40, 0x80200000)]


# Twilight Princess's SDK (2004-2006): its configure.py cflags_dolphin with our dolphin flags
TP_SDK = (TP / 'libs/dolphin/src', TP_INC + ['-ir', str(TP / 'libs/dolphin/src')],
          ['-D__GEKKO__', '-DSDK_REVISION=2', '-DVERSION=0', '-DNDEBUG'])


def compile_funcs(job):
    ref, src, ver, flags, inc, defs = job
    import tempfile
    with tempfile.TemporaryDirectory(dir=P.OUT) as tmp:
        funcs, err = P.compile_one(ver, flags, ('x', ref, src, inc, defs, []), tmp)
    return job, funcs or {}


def all_functions():
    """[(name, address, size)] of every function in symbols.txt, duplicate local names kept (probe.our_functions keeps
    only the last of each name)"""
    import re
    out = []
    for line in P.SYMS.read_text().splitlines():
        m = re.match(r'(\S+) = \.(?:text|init):0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)', line)
        if m: out.append((m[1], int(m[2], 16), int(m[3], 16)))
    return out


def sdk_part(nproc, rng):
    import concurrent.futures as cf
    jobs = []
    for ref in ('bfbb', 'dolsdk2004', 'dolsdk2001', 'tp'):
        root, inc, defs = TP_SDK if ref == 'tp' else Rv.SDK_REFS[ref]
        for src in sorted(root.rglob('*.c')):
            for tag, fl in Rv.SDK_FLAGS.items(): jobs.append((ref, src, '1.2.5n', fl, inc, defs))
    ours = [(n, a, s) for n, a, s in all_functions() if any(lo <= a < hi for lo, hi in SDK_RANGES)]
    by_name, by_size = {}, {}
    for n, a, s in ours:
        by_name.setdefault(SDK_ALIAS.get(n, n), []).append((a, s))
        by_size.setdefault(s, []).append((n, a))
    res, bytes_hit = {}, {}     # keyed by address
    with cf.ThreadPoolExecutor(nproc) as ex:
        for (ref, src, ver, fl, inc, defs), funcs in ex.map(compile_funcs, jobs):
            tag = next(k for k, v in Rv.SDK_FLAGS.items() if v == fl)
            rel = src.relative_to((TP_SDK if ref == 'tp' else Rv.SDK_REFS[ref])[0]).as_posix()
            for name, (code, masks) in funcs.items():
                for a, sz in by_name.get(name, []):
                    e, sc = P.compare(code, masks, Rv.DOL.read(a, sz))
                    d = res.setdefault(a, {})
                    if (e, sc) > d.get(ref, (False, 0.0))[:2]: d[ref] = (e, sc, rel, '' if tag == 'base' else tag)
                for other, a in by_size.get(len(code), []):     # unnamed: by bytes
                    if not other.startswith('fn_'): continue
                    if P.compare(code, masks, Rv.DOL.read(a, len(code)))[0]:
                        bytes_hit.setdefault(a, set()).add(f'{ref}:{rel}:{name}')
    L = ['== Dolphin SDK: every function in the SDK ranges (named: compiled under its name; fn_: equal bytes)']
    count = {}
    for n, a, s in sorted(ours, key=lambda x: x[1]):
        d = res.get(a, {})
        exs = sorted(k for k, v in d.items() if v[0])
        if exs:
            key = 'exact'
            txt = ' '.join(f'{k}:EXACT({d[k][2]}{"; " + d[k][3] if d[k][3] else ""})' for k in exs)
        elif a in bytes_hit:
            names = sorted({h.rsplit(':', 1)[1] for h in bytes_hit[a]})
            key = 'exact by bytes (one name)' if len(names) == 1 else 'exact by bytes (several names)'
            txt = ' '.join(sorted(bytes_hit[a])[:4])
        else:
            key = 'no reference exact' if d else 'no reference source'
            txt = ' '.join(f'{k}:{v[1] * 100:.0f}%({v[2]})' for k, v in d.items()) or '-'
        count[key] = count.get(key, 0) + 1
        L.append(f'  {a:08X} {n:34s} {(Rv.obj_of(rng, a) or "(automatic)"):34s} {key:30s} {txt}')
    L.append('  totals: ' + ', '.join(f'{k} {v}' for k, v in sorted(count.items(), key=lambda x: -x[1])))
    return L


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--fetch', action='store_true')
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--part', default='sdk,trk,msl')
    a = ap.parse_args()
    if a.fetch: fetch()
    P.OUT.mkdir(parents=True, exist_ok=True)
    Rv.OURS, Rv.DOL = P.our_functions(), P.Dol(P.DOL)
    for ours, ref in ALIAS.items():
        if ours in Rv.OURS: Rv.OURS[ref] = Rv.OURS[ours]
    rng = Rv.objects()
    L = []
    if 'sdk' in a.part.split(','): L += sdk_part(a.jobs, rng)
    for lib, title, pred in (('trk', 'MetroTRK', lambda o: o.startswith('MetroTRK/')),
                             ('msl', 'MSL + Runtime', lambda o: o.startswith(('MSL/', 'Runtime/')))):
        if lib not in a.part.split(','): continue
        jobs = [(ref, s, '2.6', fl, inc, defs) for ref, (srcs, inc, defs) in sets(lib).items()
                for s in srcs for fl in flag_sets(lib)]
        res = cross_reference(jobs, a.jobs)
        L.append(f'== {title}: every function, best reference (exact first)')
        fs = sorted((ad, n) for n, (ad, s) in Rv.OURS.items()
                    if (pred(Rv.obj_of(rng, ad) or '') or (lib == 'trk' and n == 'fn_800053E0'))
                    and n not in ALIAS.values())
        count = {}
        for ad, n in fs:
            d = dict(res.get(ALIAS.get(n, n), {}))
            if n in ASM:
                words = [int.from_bytes(Rv.DOL.read(ad, Rv.OURS[n][1])[i:i + 4], 'big') for i in range(0, 8, 4)]
                d[ASM_SRC] = (words == ASM[n], 1.0 if words == ASM[n] else 0.0, 'asm', '')
            exs = sorted(k for k, v in d.items() if v[0])
            key = ('exact: ' + '+'.join(exs)) if exs else ('no reference exact' if d else 'no reference source')
            k2 = 'exact' if exs else key
            count[k2] = count.get(k2, 0) + 1
            best = ' '.join(f'{k}:{"EXACT" if v[0] else f"{v[1] * 100:.0f}%"}({v[2]}{"; " + v[3] if v[3] else ""})'
                            for k, v in sorted(d.items(), key=lambda x: (-x[1][0], -x[1][1]))[:5])
            L.append(f'  {ad:08X} {n:34s} {(Rv.obj_of(rng, ad) or "(automatic)"):34s} {best or "-"}')
        L.append('  totals: ' + ', '.join(f'{k} {v}' for k, v in sorted(count.items(), key=lambda x: -x[1])))
        L.append(f'== {title}: per object, the references that reproduce EVERY function of it (file; extra flags)')
        objs = {}
        for ad, n in fs: objs.setdefault(Rv.obj_of(rng, ad) or f"(automatic) {n}", []).append(n)
        for o, names in objs.items():
            ds = [dict(res.get(ALIAS.get(n, n), {})) for n in names]
            if all(n in ASM for n in names): whole = [ASM_SRC]
            else:
                whole = [k for k in ds[0] if all(k in d and d[k][0] for d in ds)]
            fl = {k: sorted({d[k][3] for d in ds if d[k][3]}) for k in whole if k != ASM_SRC}
            txt = ' | '.join(f'{k} ({ds[0][k][2]}{"; " + ", ".join(fl[k]) if fl.get(k) else ""})' if k != ASM_SRC
                             else k for k in whole)
            L.append(f'  {o:36s} {len(names):3d}  {txt or "NONE"}')
    text = '\n'.join(L) + '\n'
    (P.OUT / 'sources.txt').write_bytes(text.encode())
    print(text)
    print(f'wrote {P.OUT / "sources.txt"}')


if __name__ == '__main__':
    main()
