"""Set up a decomp-permuter run for one function (docs/sched_model.md sections 19-20).

usage: python -I tools/sched/permuter_setup.py FUNCTION SOURCE.c [--object OBJSTEM] [--run JOBS] [--minutes M]
  --object sysdolphin/baselib/displayfunc: a library function: the object's own cflags and compiler from
  decomp/build.ninja, the include paths of those cflags, the target from decomp/build/GXCE01/asm/<OBJSTEM>.s

Writes tools/sched/permuter/<FUNCTION>/ (untracked: the files hold this machine's absolute paths):
  base.c        SOURCE preprocessed the way the drafts compile (tools/draft_batch.py's defines and include paths,
                with `__declspec(section ...)` removed, which the permuter's C parser does not read)
  target.o      the original function, assembled from decomp/drafts/asm/<FUNCTION>.s with GNU as (relocations kept)
  compile.sh    GC/2.6 with the game flags of configure.py
  settings.toml the function name and the PowerPC objdump
--run JOBS starts the permuter (tools/sched/permuter/run_permuter.py) with that many workers for --minutes (default
60); its better sources land in <FUNCTION>/output-<score>-<n>/source.c. Score a result with
`python -I tools/sched/scratch/hd/pr.py FUNCTION <file.c>`.
Needs (tools/downloads, set up once by hand, docs/sched_model.md section 19): decomp-permuter_dl, permuter_venv,
binutils_ppc, cpp_shim/cpp.exe (a copy of LLVM's clang-cpp.exe).
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
G = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(G, 'tools'))
import draft_batch as db  # noqa: E402

DL = os.path.join(G, 'tools', 'downloads')
CPP = os.path.join(DL, 'cpp_shim', 'cpp.exe')
AS = os.path.join(DL, 'binutils_ppc', 'powerpc-eabi-as.exe')
OBJDUMP = os.path.join(DL, 'binutils_ppc', 'powerpc-eabi-objdump.exe')
PY = os.path.join(DL, 'permuter_venv', 'Scripts', 'python.exe')
MACROS = os.path.join(G, 'decomp', 'build', 'GXCE01', 'include', 'macros.inc')
FLAGS = ('-nodefaults -proc gekko -align powerpc -enum int -fp hardware -Cpp_exceptions off -pragma "cats off" '
         '-pragma "warn_notinlined off" -maxerrors 1 -nosyspath -multibyte -W off '
         '-opt level=0,peephole,schedule,speed -use_lmw_stmw on -fp_contract on -str reuse,readonly')


def fwd(p):
    return p.replace(os.sep, '/')


def object_flags(dec, obj):
    """(cflags for the compile step, mw version, include dirs, -D defines) of decomp/build.ninja's src/<obj>.c"""
    import shlex
    nin = open(os.path.join(dec, 'build.ninja'), encoding='utf-8').read().replace('$\n', '')
    bs = chr(92)
    key = 'build build' + bs + 'GXCE01' + bs + 'src' + bs + obj.replace('/', bs) + '.o: mwcc'
    i = nin.index(key)
    blk = nin[i:nin.index('\nbuild ', i + 10)]
    cflags = re.search(r'\n  cflags = (.*)', blk).group(1)
    ver = re.search(r'mw_version = (\S+)', blk).group(1).replace(bs, '/').split('/')[-1]
    args = shlex.split(cflags.replace(bs, '/'), posix=True)
    incs = [args[k + 1] for k, x in enumerate(args) if x == '-i']
    defs = [x for x in args if x.startswith('-D')]
    keep = []
    k = 0
    while k < len(args):   # the compile step gets the preprocessed source: no include paths, no -cwd
        if args[k] in ('-i', '-cwd'):
            k += 2
            continue
        keep.append(args[k])
        k += 1
    flags = ' '.join('"%s"' % x if ' ' in x else x for x in keep)
    return flags, ver, incs, defs


def main(a):
    if len(a) < 3:
        raise SystemExit(__doc__)
    name, src = a[1], os.path.abspath(a[2])
    obj = a[a.index('--object') + 1] if '--object' in a else None
    out = os.path.join(HERE, 'permuter', name)
    os.makedirs(out, exist_ok=True)
    dec = os.path.join(G, 'decomp')
    # base.c: preprocessed like a draft compile (cwd decomp/, the draft include paths)
    flags, ver = FLAGS, '2.6'
    if obj:
        flags, ver, inc_dirs, defs = object_flags(dec, obj)
        # mwcc searches the -i paths before the source's own folder (lobj.c's "lobj.h" is include/cr_kar/lobj.h)
        incs = sum((['-I', os.path.join(dec, d)] for d in inc_dirs), []) + ['-I', os.path.dirname(src), '-I', os.path.join(dec, 'src')]
        defs = defs + ['-D__declspec(x)=', '-D__attribute__(x)=', '-D__MWERKS__=0x2301', '-D__PPCGEKKO__']   # the headers' mwcc-only parts (platform.h's MWERKS_GEKKO: math_ppc.h's inline sqrtf) as mwcc sees them
    else:
        incs = ['-I', os.path.join(dec, 'drafts'), '-I', os.path.dirname(src)]
        for i in db.CTX_INC:
            incs += ['-I', os.path.normpath(os.path.join(dec, i))]
        defs = ['-DM2CTX', '-D__attribute__(x)=', '-D__declspec(x)=', '-DCR_DRAFT_CC']
    feed = src
    if obj:
        # the library asserts pass __FILE__: a #line keeps the name the original was compiled under ("texp.c"),
        # else the preprocessed source carries this machine's path and the assert strings (and their pool) differ
        feed = os.path.join(out, '_line_' + os.path.basename(src))
        body = open(src, encoding='utf-8', errors='replace').read()
        open(feed, 'w', encoding='utf-8').write('#line 1 "%s"\n' % os.path.basename(src) + body)
    pp = subprocess.run([CPP, '-P', '-nostdinc'] + defs + incs + [feed], capture_output=True, text=True, cwd=dec)
    if pp.returncode:
        raise SystemExit('cpp failed: ' + pp.stderr[-2000:])
    base = re.sub(r'__declspec\(section "[^"]*"\)\s*', '', pp.stdout)
    # __FILE__ of a header's inline (jobj.h's asserts): cpp writes the path it opened, mwcc the bare name ("jobj.h",
    # 7 bytes: small data, one `li r3,x@sda21` where the path costs lis/addi)
    base = re.sub(r'"[A-Za-z]:[^"]*?([\w.]+)"', r'"\1"', base)
    # an mwcc-only fixed-address declaration (`T x : 0xADDR;`) as a macro the parser reads
    base = re.sub(r'^(?:volatile\s+)?(\w+)\s+(\w+)\s*:\s*(0x[0-9A-Fa-f]+);$', r'#define \2 (*(volatile \1 *) \3)', base, flags=re.M)
    open(os.path.join(out, 'base.c'), 'w', encoding='utf-8').write(base)
    # target.o from the original's asm
    if obj:
        whole = open(os.path.join(dec, 'build', 'GXCE01', 'asm', obj + '.s'), encoding='utf-8').read().splitlines()
        i0 = next(k for k, l in enumerate(whole) if l.startswith('.fn %s,' % name))
        i1 = next(k for k in range(i0, len(whole)) if whole[k].startswith('.endfn %s' % name))
        asm = whole[i0:i1 + 1]
    else:
        asm = open(os.path.join(dec, 'drafts', 'asm', name + '.s'), encoding='utf-8').read().splitlines()
    lines = ['.include "%s"' % fwd(MACROS), '.section .text']
    for l in asm:
        l = re.sub(r'/\*.*?\*/', '', l).strip()
        if not l:
            continue
        m = re.match(r'\.fn (\S+),', l)
        if m:
            lines += ['.globl ' + m.group(1), m.group(1) + ':']
            continue
        if l.startswith('.endfn'):
            break
        lines.append(l if l.endswith(':') else '    ' + l)
    open(os.path.join(out, 'target.s'), 'w', encoding='utf-8').write('\n'.join(lines) + '\n')
    r = subprocess.run([AS, '-mgekko', '-o', os.path.join(out, 'target.o'), os.path.join(out, 'target.s')],
                       capture_output=True, text=True)
    if r.returncode:
        raise SystemExit('as failed: ' + r.stderr[-2000:])
    mw = fwd(os.path.join(dec, 'build', 'compilers', 'GC', ver, 'mwcceppc.exe'))
    open(os.path.join(out, 'compile.sh'), 'w', newline='\n').write(
        '#!/bin/bash\n# permuter compile step: GC/2.6 with the game flags (configure.py); written by permuter_setup.py\n'
        '"%s" %s -c "$(cygpath -w "$1")" -o "$(cygpath -w "$3")" >/dev/null 2>&1\n' % (mw, flags))
    open(os.path.join(out, 'settings.toml'), 'w').write(
        'func_name = "%s"\ncompiler_type = "mwcc"\nobjdump_command = "%s -dr -EB -mpowerpc -M broadway"\n' % (name, fwd(OBJDUMP)))
    print('set up', fwd(out))
    if '--run' in a:
        jobs = a[a.index('--run') + 1]
        minutes = int(a[a.index('--minutes') + 1]) if '--minutes' in a else 60
        env = dict(os.environ, PATH=os.path.dirname(CPP) + os.pathsep + os.environ.get('PATH', ''))
        p = subprocess.Popen([PY, '-I', os.path.join(HERE, 'run_permuter.py'), name, '-j', jobs],
                             cwd=os.path.join(HERE, 'permuter'), env=env)
        try:
            p.wait(timeout=60 * minutes)
        except subprocess.TimeoutExpired:
            # the permuter's multiprocessing workers outlive their parent (and keep any pipe open): kill the tree
            if os.name == 'nt':
                subprocess.run(['taskkill', '/T', '/F', '/PID', str(p.pid)], capture_output=True)
            else:
                p.kill()
            p.wait()
        best = sorted((d for d in os.listdir(out) if d.startswith('output-')), key=lambda d: int(d.split('-')[1]))
        print('best:', best[:3])


if __name__ == '__main__':
    main(sys.argv)
