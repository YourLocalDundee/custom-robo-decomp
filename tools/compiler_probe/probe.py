"""Phase 4 Milestone 3: which Metrowerks CodeWarrior (mwcceppc) version and flags built each part of main.dol.

Method: compile reference C (the pinned decomps) with every GC compiler in decomp/build/compilers/GC and a set of
candidate flag sets per library, take every function of the object whose name is also a function in our
decomp/config/GXCE01/symbols.txt, and compare its code with the retail bytes (read from main.dol through the DOL
header, like tools/dol_asserts.py). Instruction fields that a relocation fills in (call targets, address halves,
small-data offsets + base register) are masked on both sides. A function "matches" when it has the same size and
every masked word is equal; otherwise it scores the fraction of equal words (difflib on the word lists when the sizes
differ).

Stage 1: every compiler version with the library's base flags (the flags its reference project uses).
Stage 2: every version that matched at least half as many functions as the best one in stage 1 (--keep), with
every flag variant of the library, so versions are always compared over the same flag sets.
A function that matches under every candidate tells nothing; the report lists the discriminating ones.

The game code has no source: tools/compiler_probe/game/*.c are hand-written C for a few tiny game functions
(name fn_XXXXXXXX = the address), probed the same way.

Inputs : decomp/orig/GXCE01/sys/main.dol, decomp/config/GXCE01/symbols.txt, decomp/build/compilers/GC/*,
         tools/melee, tools/refs/{dolsdk2004,dolsdk2001,marioparty4}, tools/compiler_probe/game/*.c
Outputs: tools/compiler_probe/out/ (gitignored): results.tsv (every case x candidate x function), summary.txt
Usage  : python -I tools/compiler_probe/probe.py [--jobs N] [--lib NAME ...] [--keep F]
         python -I tools/compiler_probe/probe.py --show FUNCTION --version 1.3.2 --flags "-O4,p -inline auto"
         (one function word by word against retail; relocated fields shown as '.')
"""
import argparse
import concurrent.futures as cf
import difflib
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

G = Path(__file__).resolve().parents[2]
OUT = G / 'tools' / 'compiler_probe' / 'out'
GAME_C = G / 'tools' / 'compiler_probe' / 'game'
CC_DIR = G / 'decomp' / 'build' / 'compilers' / 'GC'
DOL = G / 'decomp' / 'orig' / 'GXCE01' / 'sys' / 'main.dol'
# off Windows the compilers run through the pinned wibo (tools/setup_decomp.ps1), else wine
WRAP = str(G / 'decomp' / 'build' / 'tools' / 'wibo') if (G / 'decomp/build/tools/wibo').exists() else 'wine'
SYMS = G / 'decomp' / 'config' / 'GXCE01' / 'symbols.txt'
MELEE = G / 'tools' / 'melee'
SDK04 = G / 'tools' / 'refs' / 'dolsdk2004'
SDK01 = G / 'tools' / 'refs' / 'dolsdk2001'
MP4 = G / 'tools' / 'refs' / 'marioparty4'

# every GC compiler of the pinned compilers zip (decomp/build/compilers/info.txt), oldest first
VERSIONS = ['1.0', '1.1', '1.1p1', '1.2.5', '1.2.5n', '1.3', '1.3.2', '1.3.2r', '2.0', '2.0p1', '2.5', '2.6', '2.7',
            '3.0a3', '3.0a3.2', '3.0a3.3', '3.0a3.4', '3.0a5', '3.0a5.2']

# ---- flag sets ------------------------------------------------------------------------------------------------------
# Common to every case: no default include paths, Gekko, hardware floats, no C++ exceptions, warnings off.
COMMON = ['-nodefaults', '-proc', 'gekko', '-fp', 'hard', '-Cpp_exceptions', 'off', '-enum', 'int', '-w', 'off',
          '-maxerrors', '1', '-nosyspath', '-pragma', 'cats off', '-align', 'powerpc']

# Per library: base flags (from the reference project that builds this source) and variants for stage 2.
# Variants are alternatives seen in 2001-2005 GC decomps (Melee, KAR, MKDD, Pikmin 2, XD, MP4, dolsdk Makefiles).
O_VARIANTS = [['-O4,p'], ['-O4,s'], ['-O3,p'], ['-O2']]
INL_VARIANTS = [['-inline', 'auto'], ['-inline', 'auto,deferred'], ['-inline', 'all'], ['-inline', 'off'],
                ['-inline', 'noauto']]
FPC_VARIANTS = [['-fp_contract', 'on'], ['-fp_contract', 'off']]
# 'extra': further single changes to the base flags (appended; mwcc takes the last setting), also run in stage 2
LIBS = {
    # dolsdk2004 Makefile (release): -O4,p -inline auto -char unsigned; Melee DolphinLib: -fp_contract off
    'sdk': dict(base=['-O4,p', '-inline', 'auto', '-char', 'unsigned', '-fp_contract', 'off'],
                axes=[O_VARIANTS, INL_VARIANTS, FPC_VARIANTS],
                extra=[['-char', 'signed'], ['-use_lmw_stmw', 'on'], ['-sdata', '0', '-sdata2', '0']]),
    # Melee cflags_base + cflags_optimized (sysdolphin): -O4,p -inline auto -fp_contract on -str reuse
    'hsd': dict(base=['-O4,p', '-inline', 'auto', '-fp_contract', 'on', '-str', 'reuse'],
                axes=[O_VARIANTS, INL_VARIANTS, FPC_VARIANTS],
                extra=[['-char', 'signed'], ['-char', 'unsigned'], ['-use_lmw_stmw', 'on'],
                       ['-sdata', '0', '-sdata2', '0']]),
    # Melee cflags_libc + Libc(inline="deferred,auto"); MP4 cflags_msl: -char signed, inline auto,deferred
    'msl': dict(base=['-O4,p', '-inline', 'auto,deferred', '-use_lmw_stmw', 'on', '-str', 'pool,readonly',
                      '-common', 'off', '-fp_contract', 'on'],
                axes=[O_VARIANTS, INL_VARIANTS, FPC_VARIANTS],
                extra=[['-char', 'signed'], ['-use_lmw_stmw', 'off'], ['-sdata', '0', '-sdata2', '0']]),
    # Melee cflags_runtime: -use_lmw_stmw on -str reuse,pool,readonly -common off, -inline auto
    'runtime': dict(base=['-O4,p', '-inline', 'auto', '-use_lmw_stmw', 'on', '-str', 'reuse,pool,readonly',
                          '-common', 'off', '-fp_contract', 'on'],
                    axes=[O_VARIANTS, INL_VARIANTS, FPC_VARIANTS],
                    extra=[['-use_lmw_stmw', 'off'], ['-sdata', '0', '-sdata2', '0']]),
    # MP4 cflags_trk: -use_lmw_stmw on -sdata 0 -sdata2 0 -inline auto,deferred (MP4's -enum min loses 7 functions
    # here: ours is -enum int)
    'trk': dict(base=['-O4,p', '-inline', 'auto,deferred', '-use_lmw_stmw', 'on', '-str', 'reuse,readonly',
                      '-common', 'off', '-sdata', '0', '-sdata2', '0', '-fp_contract', 'on'],
                axes=[O_VARIANTS, INL_VARIANTS, FPC_VARIANTS],
                extra=[['-enum', 'min'], ['-sdata', '8', '-sdata2', '8'], ['-use_lmw_stmw', 'off'],
                       ['-pool', 'off', '-rostr']]),
    # game: no reference; candidates range from -O0 to -O4,p (stage 1 uses the first)
    # (",speed": with the default ",space" level 0 saves 3+ registers through _savegpr_NN calls; the game stores them.
    #  -fp_contract on: the game's a += b * s is one fmadds even at level 0, where the default is off)
    'game': dict(base=['-opt', 'level=0,peephole,schedule,speed', '-fp_contract', 'on'],
                 axes=[[['-opt', 'level=0,peephole,schedule,speed'], ['-opt', 'level=0,peephole,schedule,space'],
                        ['-O0'], ['-O0,p'], ['-opt', 'level=0,peephole,speed'], ['-opt', 'level=0,schedule,speed'],
                        ['-opt', 'level=1,peephole,schedule,speed'], ['-opt', 'level=2,peephole,schedule,speed'],
                        ['-O4,p'], ['-O4,s'], ['-O2']], FPC_VARIANTS],
                 extra=[['-sdata', '0', '-sdata2', '0'], ['-use_lmw_stmw', 'on']]),
}

# ---- cases: (library, label, source, include dirs, defines, extra flags) -------------------------------------------
SDK04_INC = ['-I-', '-i', str(SDK04 / 'include'), '-i', str(SDK04 / 'include' / 'libc'), '-ir', str(SDK04 / 'src')]
SDK04_DEF = ['-D__GEKKO__', '-DSDK_REVISION=0', '-DRELEASE', '-DNDEBUG']
MELEE_INC = ['-cwd', 'source'] + sum([['-i', str(MELEE / p)] for p in
                           ('src', 'libs/doldecomp/include', 'src/MSL', 'libs/dolphin/include')], [])
MELEE_DEF = ['-DBUILD_VERSION=2', '-DVERSION_GALE01', '-DNDEBUG=1', '-DMUST_MATCH']
MP4_INC = ['-cwd', 'source', '-i', str(MP4 / 'include'), '-i', str(MP4 / 'include' / 'dolphin')]
MP4_DEF = ['-DVERSION=0', '-DNDEBUG=1', '-DMUSY_TARGET=MUSY_TARGET_DOLPHIN']


def _cases():
    c = []
    for f, signed in [('os/OSAlarm.c', 0), ('os/OSAlloc.c', 0), ('os/OSThread.c', 0), ('os/OSRtc.c', 0),
                      ('os/OSFont.c', 0), ('os/OSReset.c', 0), ('os/OSMutex.c', 0), ('gx/GXAttr.c', 0),
                      ('gx/GXTev.c', 0), ('gx/GXLight.c', 0), ('gx/GXTransform.c', 0), ('gx/GXGeometry.c', 0),
                      ('gx/GXMisc.c', 0), ('gx/GXTexture.c', 0), ('gx/GXFrameBuf.c', 0), ('gx/GXPixel.c', 0),
                      ('vi/vi.c', 0), ('ai/ai.c', 0), ('ar/arq.c', 0), ('si/SIBios.c', 0), ('pad/Pad.c', 0),
                      ('dvd/dvdqueue.c', 1), ('dvd/dvd.c', 1), ('card/CARDStat.c', 0), ('card/CARDBios.c', 0),
                      ('mtx/mtx.c', 1), ('exi/EXIBios.c', 0), ('dsp/dsp.c', 0), ('ax/AXVPB.c', 0)]:
        c.append(('sdk', 'dolsdk2004/' + f, SDK04 / 'src' / f, SDK04_INC, SDK04_DEF,
                  ['-char', 'signed'] if signed else []))
    for f in ('tobj', 'mobj', 'fog', 'object', 'util', 'state', 'jobj', 'aobj', 'lobj', 'cobj', 'dobj', 'pobj',
              'list', 'memory', 'random', 'mtx', 'displayfunc', 'wobj', 'fobj', 'robj', 'id', 'class', 'objalloc',
              'gobj', 'gobjproc', 'gobjplink', 'archive', 'texp', 'tev', 'initialize', 'video', 'quatlib',
              'spline', 'shadow', 'bytecode', 'controller', 'rumble'):
        c.append(('hsd', f'melee/sysdolphin/baselib/{f}.c', MELEE / 'src/sysdolphin/baselib' / f'{f}.c',
                  MELEE_INC, MELEE_DEF, []))
    for f in ('axdriver', 'synth', 'devcom'):
        c.append(('hsd_audio', f'melee/sysdolphin/baselib/{f}.c', MELEE / 'src/sysdolphin/baselib' / f'{f}.c',
                  MELEE_INC, MELEE_DEF, []))
    msl = MP4 / 'src' / 'MSL_C.PPCEABI.bare.H'
    for f in ('string', 'mem', 'mem_funcs', 'ctype', 'ansi_fp', 'buffer_io', 'direct_io', 'file_io', 'misc_io',
              'mbstring', 'alloc', 'abort_exit', 'ansi_files', 'wchar_io', 'e_fmod', 'e_pow', 'e_rem_pio2',
              'k_cos', 'k_sin', 'k_tan', 'k_rem_pio2', 's_atan', 's_floor', 's_frexp', 's_ldexp', 's_modf',
              's_copysign', 's_sin', 's_cos', 's_tan', 'w_fmod', 'w_pow', 'e_acos', 'e_asin', 'e_atan2',
              'printf', 'FILE_POS', 'uart_console_io'):
        c.append(('msl', f'mp4/MSL/{f}.c', msl / f'{f}.c', MP4_INC, MP4_DEF, ['-char', 'signed']))
    for f in ('string', 'mem', 'mem_funcs', 'ctype', 'ansi_fp', 'buffer_io', 'direct_io', 'misc_io', 'mbstring',
              'abort_exit', 'ansi_files', 'wchar_io', 'trigf', 'math', 'printf', 'strtoul'):
        c.append(('msl', f'melee/MSL/{f}.c', MELEE / 'src/MSL' / f'{f}.c', MELEE_INC, MELEE_DEF, []))
    rt = MP4 / 'src' / 'Runtime.PPCEABI.H'
    for f in ('__va_arg', 'global_destructor_chain', 'runtime', 'GCN_Mem_Alloc', '__mem'):
        c.append(('runtime', f'mp4/Runtime/{f}.c', rt / f'{f}.c', MP4_INC, MP4_DEF, []))
    for f in ('__va_arg', 'global_destructor_chain', 'runtime', '__mem'):
        c.append(('runtime', f'melee/Runtime/{f}.c', MELEE / 'src/Runtime' / f'{f}.c', MELEE_INC, MELEE_DEF, []))
    trk = MP4 / 'src' / 'TRK_MINNOW_DOLPHIN'
    for p in sorted(trk.glob('*.c')):
        c.append(('trk', f'mp4/TRK/{p.name}', p, MP4_INC, MP4_DEF, []))
    for p in sorted((MELEE / 'src' / 'MetroTRK').glob('*.c')):   # an older TRK (2001): closer to ours in places
        c.append(('trk', f'melee/MetroTRK/{p.name}', p, MELEE_INC, MELEE_DEF, []))
    for p in sorted(GAME_C.glob('*.c')):
        c.append(('game', f'game/{p.name}', p, [], [], []))
    return [x for x in c if x[2].exists()]


# ---- retail bytes ---------------------------------------------------------------------------------------------------
class Dol:
    def __init__(self, path):
        self.d = path.read_bytes()
        self.off = struct.unpack_from('>18I', self.d, 0)
        self.adr = struct.unpack_from('>18I', self.d, 0x48)
        self.sz = struct.unpack_from('>18I', self.d, 0x90)

    def read(self, a, n):
        for o, b, s in zip(self.off, self.adr, self.sz):
            if s and b <= a and a + n <= b + s: return self.d[o + a - b:o + a - b + n]
        return None


def our_functions():
    f = {}
    for line in SYMS.read_text().splitlines():
        m = re.match(r'(\S+) = \.(?:text|init):0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)', line)
        if m: f[m[1]] = (int(m[2], 16), int(m[3], 16))
    return f


# ---- minimal ELF32 big-endian reader (mwcc objects) -------------------------------------------------------------------
def elf_functions(data):
    """{name: (code bytes, [word masks])} for every sized function symbol in an executable section"""
    shoff, = struct.unpack_from('>I', data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('>HHH', data, 0x2E)
    sh = [struct.unpack_from('>IIIIIIIIII', data, shoff + i * shentsize) for i in range(shnum)]
    strs = lambda sec, o: data[sh[sec][4] + o:data.index(b'\0', sh[sec][4] + o)].decode('latin-1')
    symtab = next((i for i, s in enumerate(sh) if s[1] == 2), None)
    if symtab is None: return {}
    syms = []
    for k in range(sh[symtab][5] // 16):
        nm, val, size, info, oth, shn = struct.unpack_from('>IIIBBH', data, sh[symtab][4] + k * 16)
        syms.append((strs(sh[symtab][6], nm) if nm else '', val, size, info & 15, shn))
    masks = {}   # section index -> {offset: mask}
    for s in sh:
        if s[1] == 4:   # SHT_RELA
            tgt, m = s[7], masks.setdefault(s[7], {})
            for k in range(s[5] // 12):
                off, info, _ = struct.unpack_from('>IIi', data, s[4] + k * 12)
                t = info & 0xFF
                mk = {10: 0x03FFFFFC, 2: 0x03FFFFFC, 4: 0xFFFF, 5: 0xFFFF, 6: 0xFFFF, 11: 0xFFFC, 7: 0xFFFC,
                      109: 0x001FFFFF}.get(t, 0xFFFFFFFF)
                m[off & ~3] = m.get(off & ~3, 0) | mk
    out = {}
    for name, val, size, typ, shn in syms:
        if typ != 2 or not size or shn == 0 or shn >= len(sh): continue
        if not sh[shn][2] & 4: continue   # SHF_EXECINSTR
        code = data[sh[shn][4] + val:sh[shn][4] + val + size]
        m = masks.get(shn, {})
        out[name] = (code, [m.get(val + i, 0) for i in range(0, size, 4)])
    return out


def compare(code, masks, target):
    """(exact, score 0..1) of compiled code (relocated fields masked) against the retail bytes"""
    a = [struct.unpack_from('>I', code, i)[0] & ~masks[i // 4] for i in range(0, len(code) & ~3, 4)]
    nb = min(len(target), len(code)) & ~3
    b = [struct.unpack_from('>I', target, i)[0] & ~(masks[i // 4] if i // 4 < len(masks) else 0)
         for i in range(0, len(target) & ~3, 4)]
    if len(a) == len(b):
        eq = sum(x == y for x, y in zip(a, b))
        return eq == len(a), eq / max(1, len(a))
    return False, difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


# ---- running ----------------------------------------------------------------------------------------------------------
def compile_one(version, flags, case, tmp):
    lib, label, src, inc, defs, extra = case
    exe = CC_DIR / version / 'mwcceppc.exe'
    obj = Path(tmp) / f'{abs(hash((version, tuple(flags), label)))}.o'
    cmd = [str(exe), '-c'] + COMMON + flags + extra + inc + defs + [str(src), '-o', str(obj)]
    if os.name != 'nt': cmd = [WRAP] + cmd
    r = subprocess.run(cmd, capture_output=True, cwd=str(src.parent), timeout=120)
    if r.returncode or not obj.exists():
        err = (r.stdout + r.stderr).decode('latin-1').strip().splitlines()
        # mwcc prints '#   Error: ^' and the message on the next line
        msg = [err[i + 1].strip('# ').strip() for i, l in enumerate(err[:-1]) if 'Error:' in l]
        return None, ' | '.join(msg)[:200] or 'compile failed'
    data = obj.read_bytes()
    obj.unlink()
    return elf_functions(data), None


def run(jobs_list, ours, dol, tmp, nproc):
    """jobs_list: [(version, flags tuple, case)] -> [(case label, lib, version, flags str, func, addr, exact, score)]"""
    rows, errors = [], {}
    with cf.ThreadPoolExecutor(nproc) as ex:
        futs = {ex.submit(compile_one, v, list(f), c, tmp): (v, f, c) for v, f, c in jobs_list}
        for fu in cf.as_completed(futs):
            v, f, c = futs[fu]
            funcs, err = fu.result()
            if err:
                errors[(c[1], v, ' '.join(f))] = err
                continue
            for name, (code, masks) in funcs.items():
                if name not in ours: continue
                addr, size = ours[name]
                tgt = dol.read(addr, size)
                if tgt is None: continue
                exact, score = compare(code, masks, tgt)
                rows.append((c[1], c[0], v, ' '.join(f), name, addr, exact, score, len(code) == size))
    return rows, errors


def flag_variants(lib):
    base = LIBS[lib]['base']
    out = [tuple(base)]
    for axis in LIBS[lib]['axes']:
        new = []
        for opt in axis:
            # replace the base's setting of this option (by its option word) with the variant's
            key = opt[0] if opt[0] in ('-inline', '-fp_contract', '-opt') else None
            for f in out:
                f = list(f)
                if key:
                    if key in f:
                        i = f.index(key); f[i:i + 2] = opt
                    else:
                        f += opt
                else:   # an -O level: replace any -O* / -opt pair
                    f = [x for x in f if not x.startswith('-O')]
                    if '-opt' in f:
                        i = f.index('-opt'); del f[i:i + 2]
                    f = opt + f
                new.append(tuple(f))
        out = list(dict.fromkeys(new))
    return list(dict.fromkeys([tuple(base)] + out + [tuple(base + e) for e in LIBS[lib].get('extra', [])]))


def summarize(rows, lib, base=''):
    """per (version, flags): matched functions, functions seen, mean score"""
    agg = {}
    for label, l, v, f, name, addr, exact, score, samesize in rows:
        if l != lib: continue
        a = agg.setdefault((v, f), [0, 0, 0.0])
        a[0] += exact; a[1] += 1; a[2] += score
    # ties in a fixed order (the base flags first, then oldest version, then flags), so the report is the same every run
    return sorted(((k, a[0], a[1], a[2] / max(1, a[1])) for k, a in agg.items()),
                  key=lambda x: (-x[1], -round(x[3], 6), x[0][1] != base, VERSIONS.index(x[0][0]), x[0][1]))


def show(name, version, flags, ours, dol):
    """print one function word by word: compiled (masked fields as ....) vs retail, '*' where they differ"""
    addr, size = ours[name]
    tgt = dol.read(addr, size)
    with tempfile.TemporaryDirectory(dir=OUT) as tmp:
        for c in _cases():
            funcs, err = compile_one(version, flags, c, tmp)
            if not funcs or name not in funcs: continue
            code, masks = funcs[name]
            exact, score = compare(code, masks, tgt)
            print(f'{c[1]}: {name} @0x{addr:08X} GC/{version} {" ".join(flags)}: exact={exact} '
                  f'score={score * 100:.2f}% size {len(code):#x} vs {size:#x}')
            for i in range(0, max(len(code), size), 4):
                a = struct.unpack_from('>I', code, i)[0] if i < len(code) else None
                b = struct.unpack_from('>I', tgt, i)[0] if i < size else None
                m = masks[i // 4] if i < len(code) else 0
                sa = '--------' if a is None else ''.join('.' if (m >> (28 - 4 * k)) & 15 else f'{a:08x}'[k]
                                                         for k in range(8))
                sb = '--------' if b is None else f'{b:08x}'
                d = a is None or b is None or (a & ~m) != (b & ~m)
                print(f'  +{i:04x}  {sa}  {sb}  {"*" if d else ""}')
            return
    print(f'{name}: no case compiles it')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--lib', nargs='*', default=list(LIBS) + ['hsd_audio'])
    ap.add_argument('--keep', type=float, default=0.5,
                    help='stage 2 takes every version matching at least this fraction of the best stage-1 count')
    ap.add_argument('--show', metavar='FUNCTION', help='only print one function vs retail (with --version, --flags)')
    ap.add_argument('--version', default='1.3.2')
    ap.add_argument('--flags', default='-O4,p -inline auto')
    a = ap.parse_args()
    for p in (DOL, SYMS, CC_DIR):
        if not p.exists(): sys.exit(f'missing {p} (run tools/setup_decomp.ps1 / batch 9 first)')
    OUT.mkdir(parents=True, exist_ok=True)
    ours, dol = our_functions(), Dol(DOL)
    if a.show:
        # '-pragma "cats off"' style quoting is not needed here: split on spaces
        return show(a.show, a.version, a.flags.split(), ours, dol)
    cases = [c for c in _cases() if c[0] in a.lib]
    versions = [v for v in VERSIONS if (CC_DIR / v / 'mwcceppc.exe').exists()]
    all_rows, all_err, lines = [], {}, []
    with tempfile.TemporaryDirectory(dir=OUT) as tmp:
        for lib in a.lib:
            lc = [c for c in cases if c[0] == lib]
            if not lc: continue
            flib = 'hsd' if lib == 'hsd_audio' else lib
            base = tuple(LIBS[flib]['base'])
            print(f'[{lib}] stage 1: {len(lc)} files x {len(versions)} versions', flush=True)
            rows, err = run([(v, base, c) for v in versions for c in lc], ours, dol, tmp, a.jobs)
            s1 = summarize(rows, lib, ' '.join(base))
            # every version near the best: comparing versions is only fair when all got the same flag sets
            top = max((m for _, m, _, _ in s1), default=0)
            best = [k[0] for k, m, _, _ in s1 if m >= a.keep * top and m]
            variants = [f for f in flag_variants(flib) if f != base]
            print(f'[{lib}] stage 2: versions {best} x {len(variants)} flag sets', flush=True)
            rows2, err2 = run([(v, f, c) for v in best for f in variants for c in lc], ours, dol, tmp, a.jobs)
            rows += rows2; err.update(err2)
            all_rows += rows; all_err.update(err)
            lines += report(lib, rows, err, base, lc)
    with open(OUT / 'results.tsv', 'wb') as fh:
        fh.write(b'case\tlib\tversion\tflags\tfunction\taddress\texact\tscore\tsame_size\n')
        for r in sorted(all_rows, key=lambda r: (r[1], r[0], r[4], r[2], r[3])):
            fh.write(('\t'.join([r[0], r[1], r[2], r[3], r[4], f'0x{r[5]:08X}', str(int(r[6])), f'{r[7]:.4f}',
                                 str(int(r[8]))]) + '\n').encode())
    (OUT / 'summary.txt').write_bytes(('\n'.join(lines) + '\n').encode())
    print(f'wrote {OUT / "results.tsv"} and {OUT / "summary.txt"}')


def report(lib, rows, err, base, cases):
    L = [f'==== {lib}: {len(cases)} source files, base flags: {" ".join(base)}']
    s = summarize(rows, lib, ' '.join(base))
    L.append('stage 1, base flags, every version: version matched/seen mean%')
    for (v, f), m, n, sc in sorted((x for x in s if x[0][1] == ' '.join(base)), key=lambda x: VERSIONS.index(x[0][0])):
        L.append(f'      {v:9s} {m:4d}/{n:<4d}     {sc * 100:6.2f}')
    L.append('best 25 (stage 1 + 2): version matched/seen mean% flags')
    for (v, f), m, n, sc in s[:25]:
        L.append(f'      {v:9s} {m:4d}/{n:<4d}     {sc * 100:6.2f}  {f}')
    # discriminating functions: per function, which candidates match exactly
    cands = {(v, f) for (v, f), *_ in s}
    per = {}
    for label, l, v, f, name, addr, exact, score, ss in rows:
        if l != lib: continue
        d = per.setdefault((name, addr, label), {})
        d[(v, f)] = max(d.get((v, f), 0), score + (1 if exact else 0))
    top = s[0][0] if s else None
    disc, never, alln = [], [], 0
    for (name, addr, label), d in sorted(per.items(), key=lambda x: x[0][1]):
        ok = {k for k, x in d.items() if x >= 2}
        if not ok: never.append((name, addr, label, max(d.values()))); continue
        if ok == set(d): alln += 1; continue
        disc.append((name, addr, label, ok))
    L.append(f'functions compared: {len(per)}; match under every candidate (no evidence): {alln}; '
             f'discriminating: {len(disc)}; never exact: {len(never)}')
    if top:
        L.append(f'best candidate: {top[0]} {top[1]}')
        tm = [x for x in disc if top in x[3]]
        L.append(f'discriminating functions matched by the best candidate: {len(tm)}')
        for name, addr, label, ok in tm[:60]:
            vs = sorted({v for v, f in ok}, key=VERSIONS.index)
            L.append(f'   0x{addr:08X} {name:32s} {label:40s} versions {",".join(vs)}')
    L.append(f'never exact ({len(never)}), best score:')
    for name, addr, label, b in never[:40]:
        L.append(f'   0x{addr:08X} {name:32s} {label:40s} {min(b, 1) * 100:6.2f}%')
    if err:
        L.append(f'compile errors: {len(err)} (first 10)')
        for k, e in list(err.items())[:10]: L.append(f'   {k}: {e}')
    L.append('')
    return L


if __name__ == '__main__':
    main()
