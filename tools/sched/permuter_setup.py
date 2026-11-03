"""Set up a decomp-permuter run for one function (docs/sched_model.md sections 19-20).

usage: python -I tools/sched/permuter_setup.py FUNCTION SOURCE.c [--run JOBS] [--minutes M]

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


def main(a):
    if len(a) < 3:
        raise SystemExit(__doc__)
    name, src = a[1], os.path.abspath(a[2])
    out = os.path.join(HERE, 'permuter', name)
    os.makedirs(out, exist_ok=True)
    dec = os.path.join(G, 'decomp')
    # base.c: preprocessed like a draft compile (cwd decomp/, the draft include paths)
    incs = ['-I', os.path.join(dec, 'drafts'), '-I', os.path.dirname(src)]
    for i in db.CTX_INC:
        incs += ['-I', os.path.normpath(os.path.join(dec, i))]
    defs = ['-DM2CTX', '-D__attribute__(x)=', '-D__declspec(x)=', '-DCR_DRAFT_CC']
    pp = subprocess.run([CPP, '-P', '-nostdinc'] + defs + incs + [src], capture_output=True, text=True, cwd=dec)
    if pp.returncode:
        raise SystemExit('cpp failed: ' + pp.stderr[-2000:])
    base = re.sub(r'__declspec\(section "[^"]*"\)\s*', '', pp.stdout)
    # an mwcc-only fixed-address declaration (`T x : 0xADDR;`) as a macro the parser reads
    base = re.sub(r'^(?:volatile\s+)?(\w+)\s+(\w+)\s*:\s*(0x[0-9A-Fa-f]+);$', r'#define \2 (*(volatile \1 *) \3)', base, flags=re.M)
    open(os.path.join(out, 'base.c'), 'w', encoding='utf-8').write(base)
    # target.o from the original's asm
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
    mw = fwd(os.path.join(dec, 'build', 'compilers', 'GC', '2.6', 'mwcceppc.exe'))
    open(os.path.join(out, 'compile.sh'), 'w', newline='\n').write(
        '#!/bin/bash\n# permuter compile step: GC/2.6 with the game flags (configure.py); written by permuter_setup.py\n'
        '"%s" %s -c "$(cygpath -w "$1")" -o "$(cygpath -w "$3")" >/dev/null 2>&1\n' % (mw, FLAGS))
    open(os.path.join(out, 'settings.toml'), 'w').write(
        'func_name = "%s"\ncompiler_type = "mwcc"\nobjdump_command = "%s -dr -EB -mpowerpc -M broadway"\n' % (name, fwd(OBJDUMP)))
    print('set up', fwd(out))
    if '--run' in a:
        jobs = a[a.index('--run') + 1]
        minutes = int(a[a.index('--minutes') + 1]) if '--minutes' in a else 60
        env = dict(os.environ, PATH=os.path.dirname(CPP) + os.pathsep + os.environ.get('PATH', ''))
        try:
            subprocess.run([PY, '-I', os.path.join(HERE, 'run_permuter.py'), name, '-j', jobs],
                           cwd=os.path.join(HERE, 'permuter'), env=env, timeout=60 * minutes)
        except subprocess.TimeoutExpired:
            pass
        best = sorted((d for d in os.listdir(out) if d.startswith('output-')), key=lambda d: int(d.split('-')[1]))
        print('best:', best[:3])


if __name__ == '__main__':
    main(sys.argv)
