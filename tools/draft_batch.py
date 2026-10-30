"""Draft and check the game code: m2c C draft -> compile with the game compiler -> compare with the original.

For every function of the game code (0x800055E0 up to the first library object of decomp/config/GXCE01/splits.txt,
~1,751 functions; dtk's automatic objects there and the game objects linked from C, decomp/src/game):
  1. its asm comes from dtk's split (the asm of every object of decomp/build/GXCE01/config.json in that range, read
     only; dtk writes asm for the objects linked from C too);
  2. m2c (tools/m2c, pinned in tools/pins.txt) writes a C draft, with our names (they are already in the asm) and
     our types as context: the HSD headers + symbols/cr_types.h, preprocessed once with the game compiler's own -E;
  3. the draft is compiled with the game-code compiler and flags (docs/compiler.md; the flags are read from
     decomp/configure.py CR_LIBS["game"] and cflags_cr, so they cannot drift);
  4. the compiled function is compared with the original function in dtk's split object (decomp/build/GXCE01/obj/
     <object>.o, the object that holds it) word by word. Fields a relocation fills in (call targets, @ha / @l halves, small-data
     offsets) are masked on both sides, the rest must be equal. match % = equal words / max(words of both).
     (This is the same comparison docs/compiler.md "Method" uses; objdiff-cli is not needed.)
Everything goes into decomp/drafts/ (gitignored: decompiled output must never be committed):
  c/<name>.c  the drafts      asm/<name>.s  the input   obj/  compiled objects   log/  m2c / compiler messages of failures
  draft_report.txt  one line per function + totals     ctx_pp.c  the m2c context     orig/  copies of the original objects
Also writes decomp/reports/draft_summary.txt (counts only, no code; committed).
Deterministic: functions in address order, m2c --deterministic-vars, no times or paths in the reports.
Reads decomp/build as input only and never runs ninja. Usage:
  python -I tools/draft_batch.py [--sample N] [--only NAME ...] [--jobs J] [--out DIR] [--no-summary]
"""
import argparse, collections, concurrent.futures as cf, os, re, shutil, struct, subprocess, sys, time

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = os.path.join(G, 'decomp')
GAME_START = 0x800055E0
B = os.path.join(D, 'build/GXCE01')
MWCC = None
M2C = os.path.join(G, 'tools/m2c')
MELEE = '../tools/melee'
# the same defines tools/scripts/ParseHeaders.java uses: no __MWERKS__ (inline asm drops out), M2CTX = the plain-C paths
CTX_DEFS = ['-DM2CTX', '-D__attribute__(x)=', '-D__declspec(x)=']
CTX_INC = [MELEE + '/src', MELEE + '/src/sysdolphin/baselib', MELEE + '/src/MSL', MELEE + '/src/Runtime',
           MELEE + '/libs/doldecomp/include', MELEE + '/libs/dolphin/include', MELEE + '/libs/dolphin/include/libc',
           '../tools/m2c']
FN = re.compile(r'^\.fn (\S+), \w+')
ENDFN = re.compile(r'^\.endfn (\S+)')
HDR = re.compile(r'^# \.text:0x[0-9A-Fa-f]+ \| 0x([0-9A-F]{8}) \| size: 0x([0-9A-F]+)')


def game_end():
    """the end of the game code: the first .text of a library object (not game/) of splits.txt after GAME_START"""
    end, path = None, None
    for l in open(os.path.join(D, 'config/GXCE01/splits.txt'), encoding='utf-8'):
        if l.strip() and not l[0].isspace():
            path = l.split(':')[0].strip()
        m = re.match(r'^\s+\.text\s+start:0x([0-9A-Fa-f]+)', l)
        if m and path and not path.startswith('game/') and not path.startswith('Sections'):
            a = int(m.group(1), 16)
            if a >= GAME_START and (end is None or a < end):
                end = a
    return end


def unit_files():
    """[(asm path, object path)] of every object dtk split (decomp/build/GXCE01/config.json), in its order"""
    import json
    out = []
    for u in json.load(open(os.path.join(B, 'config.json')))['units']:
        o = os.path.join(D, u['object'])
        a = os.path.join(B, 'asm', os.path.relpath(o, os.path.join(B, 'obj')))[:-2] + '.s'
        if os.path.exists(a):
            out.append((a, o))
    return out


def game_functions():
    """[(name, address, size, asm lines, original object)] of every game function, in address order, and the jump
    tables {name: lines} of the whole split (from dtk's asm of every object)"""
    end = game_end()
    funcs, jt = [], {}
    for asm, obj in unit_files():
        cur, buf, addr, size, blk = None, [], 0, 0, None
        for l in open(asm, encoding='utf-8', errors='replace').read().split(chr(10)):
            m = HDR.match(l)
            if m:
                addr, size = int(m.group(1), 16), int(m.group(2), 16)
            m = FN.match(l)
            if m:
                cur, buf = m.group(1), [l]
                continue
            if cur:
                buf.append(l)
                if ENDFN.match(l):
                    if GAME_START <= addr < end:
                        funcs.append((cur, addr, size, buf, obj))
                    cur = None
                continue
            m = re.match(r'^\.obj (jumptable_[0-9A-F]{8}),', l)
            if m:
                blk = [m.group(1), []]
            elif blk and l.startswith('.endobj'):
                jt[blk[0]] = blk[1]
                blk = None
            elif blk:
                blk[1].append(l)
    funcs.sort(key=lambda f: f[1])
    return funcs, jt


def game_flags():
    """cflags of configure.py: cflags_cr + CR_LIBS['game'] (read from the source so the draft check cannot drift)."""
    src = open(os.path.join(D, 'configure.py'), encoding='utf-8').read()
    m = re.search(r'^cflags_cr = \[(.*?)^\]', src, re.S | re.M)
    base = re.findall(r'^\s+f?(["\'])(.*)\1,', m.group(1), re.M)
    base = [b[1] for b in base]
    g = re.search(r'^\s+"game": \("([^"]+)", \[\*cflags_cr, (.*)\]\),', src, re.M)
    extra = [x[1] for x in re.findall(r'(["\'])(.*?)\1', g.group(2))]
    ver = g.group(1)
    flags = []
    for f in base + extra:
        f = f.replace('{version_num}', '0').replace('{config.version}', 'GXCE01')   # VERSIONS.index('GXCE01') == 0
        if f.startswith('-') and ' ' in f:
            k, v = f.split(' ', 1)
            flags += [k, v.strip('"')]
        else:
            flags.append(f)
    return ver, flags


# ---- ELF32 big-endian (relocatable objects only) ---------------------------------------------------------------------
class Elf:
    def __init__(self, data):
        self.d = data
        shoff, = struct.unpack('>I', data[0x20:0x24])
        shentsize, shnum, shstrndx = struct.unpack('>HHH', data[0x2E:0x34])
        self.sec = []
        for i in range(shnum):
            o = shoff + i * shentsize
            self.sec.append(struct.unpack('>IIIIIIIIII', data[o:o + 40]))   # name type flags addr off size link info align entsize
        stro = self.sec[shstrndx][4]
        self.secname = [self._cstr(stro + s[0]) for s in self.sec]
        self.syms = {}
        self.symlist = []
        for i, s in enumerate(self.sec):
            if s[1] == 2:   # SHT_SYMTAB
                strt = self.sec[s[6]][4]
                for j in range(s[5] // 16):
                    n, v, sz, info, oth, shn = struct.unpack('>IIIBBH', data[s[4] + j * 16:s[4] + j * 16 + 16])
                    name = self._cstr(strt + n)
                    self.symlist.append((name, v, sz, info, shn))
                    if (info & 0xF) == 2 and name not in self.syms:
                        self.syms[name] = (v, sz, shn)
        self.relocs = collections.defaultdict(dict)   # section index -> {offset: type}
        for i, s in enumerate(self.sec):
            if s[1] in (4, 9):   # RELA / REL
                ent = 12 if s[1] == 4 else 8
                for j in range(s[5] // ent):
                    r = struct.unpack('>III', data[s[4] + j * ent:s[4] + j * ent + 12]) if ent == 12 else struct.unpack('>II', data[s[4] + j * 8:s[4] + j * 8 + 8])
                    self.relocs[s[7]][r[0]] = r[1] & 0xFF

    def _cstr(self, o):
        e = self.d.index(b'\0', o)
        return self.d[o:e].decode('latin-1')

    def func(self, name):
        """(words, {word index: reloc type}) of a function, or None."""
        s = self.syms.get(name)
        if not s:
            return None
        v, sz, shn = s
        off = self.sec[shn][4]
        raw = self.d[off + v:off + v + sz]
        words = struct.unpack('>%dI' % (sz // 4), raw)
        rel = {(o - v) // 4: t for o, t in self.relocs[shn].items() if v <= o < v + sz}
        return words, rel


MASK = {1: 0xFFFFFFFF, 2: 0x03FFFFFC, 4: 0xFFFF, 5: 0xFFFF, 6: 0xFFFF, 7: 0xFFFC, 8: 0xFFFC, 9: 0xFFFC,
        10: 0x03FFFFFC, 11: 0xFFFC, 109: 0x001FFFFF}


def compare(a, b):
    """percent (int, 100 only if every word equal) of words equal after masking relocation fields."""
    wa, ra = a
    wb, rb = b
    n = max(len(wa), len(wb))
    if n == 0:
        return 100
    eq = 0
    for i in range(min(len(wa), len(wb))):
        m = MASK.get(ra.get(i), 0xFFFF) if i in ra else 0
        m |= MASK.get(rb.get(i), 0xFFFF) if i in rb else 0
        if (wa[i] & ~m & 0xFFFFFFFF) == (wb[i] & ~m & 0xFFFFFFFF):
            eq += 1
    return 100 if eq == n and len(wa) == len(wb) else min(99, eq * 100 // n)


def globals_decls():
    """extern declarations of our typed globals (symbols/cr_globals.txt: cr_g_Name = .sec:0xADDR; // type:object datatype:T*[N])"""
    out = ['\n// our typed globals (symbols/cr_globals.txt)\n']
    for l in open(os.path.join(G, 'symbols/cr_globals.txt'), encoding='utf-8'):
        m = re.match(r'^(\w+) = \.\w+:0x[0-9A-Fa-f]+; // type:object datatype:(\w+)(\**)((?:\[\d+\])*)\b', l)
        if m:
            out.append('extern %s%s %s%s;\n' % (m.group(2), m.group(3), m.group(1), m.group(4)))
    return ''.join(out)



# ---- one function --------------------------------------------------------------------------------------------------------
W = {}   # per-process state


def init(out, ver, flags):
    W.update(out=out, ver=ver, flags=flags, orig={})


def orig_elf(path):
    if path not in W['orig']:
        W['orig'][path] = Elf(open(path, 'rb').read())
    return W['orig'][path]


def run(cmd, timeout, cwd=None, env=None):
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout, cwd=cwd, env=env, errors='replace')
        return r.returncode, r.stdout, r.stderr
    except subprocess.TimeoutExpired:
        return -9, '', 'timeout'


ERRMSG = re.compile(r'^#\s+Error:\s*\^?\s*$')


def compile_error(text):
    """first compiler message, with identifiers/numbers normalised (so messages can be counted)."""
    lines = text.splitlines()
    for i, l in enumerate(lines):
        if re.match(r'^#\s+Error:', l):
            msg = l.split('Error:', 1)[1].strip(' ^')
            j = i + 1
            while not msg and j < len(lines):
                msg = lines[j].lstrip('# ').strip()
                j += 1
            msg = re.sub(r"'[^']*'", "'*'", msg)
            msg = re.sub(r'\b\d+\b', 'N', msg)
            return msg[:90]
    return (text.strip().splitlines() or ['no message'])[-1][:90]


def one(job):
    name, addr, size, orig_obj = job
    out = W['out']
    a = os.path.join(out, 'asm', name + '.s')
    c = os.path.join(out, 'c', name + '.c')
    o = os.path.join(out, 'obj', name + '.o')
    lg = os.path.join(out, 'log', name + '.txt')
    rc, so, se = run([sys.executable, '-I', os.path.join(out, 'm2c_run.py'), '-t', 'ppc-mwcc-c', '--context',
                      os.path.join(out, 'ctx_pp.c'), '--valid-syntax', '--deterministic-vars', '-f', name, a], 120)
    body = so
    if rc != 0 or not re.search(r'^\S.*\b%s\s*\(' % re.escape(name), body, re.M) or 'Decompilation failure' in body:
        msg = (se.strip().splitlines() or body.strip().splitlines() or ['no output'])[-1][:90]
        open(lg, 'w', encoding='utf-8').write(so + '\n' + se)
        return name, 'm2c failed', msg
    open(c, 'w', encoding='utf-8', newline='\n').write(
        '// m2c draft of %s (0x%08X, %d bytes): decompiled output, gitignored, never commit\n#include "ctx.h"\n\n%s' % (name, addr, size, body))
    cmd = [MWCC, '-c'] + W['flags'] + CTX_DEFS + ['-i', 'drafts'] + sum([['-i', i] for i in CTX_INC], []) + ['-o', o, c.replace('\\', '/')]
    cmd[0] = os.path.join(D, 'build/compilers', W['ver'], 'mwcceppc.exe')
    rc, so, se = run(cmd, 120, cwd=D)
    txt = so + se
    if rc != 0 or not os.path.exists(o):
        open(lg, 'w', encoding='utf-8').write(txt)
        return name, 'compile failed', compile_error(txt)
    try:
        mine = Elf(open(o, 'rb').read()).func(name)
    except Exception as e:
        mine = None
    if mine is None:
        return name, 'compile failed', 'no function symbol in the object'
    orig = orig_elf(orig_obj).func(name)
    if orig is None:
        return name, 'compile failed', 'function not in the original object'
    p = compare(orig, mine)
    note = 'M2C_ERROR' if 'M2C_ERROR' in body else ''
    return name, '%d%%' % p if p < 100 else '100% MATCH', note


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default=os.path.join(D, 'drafts'))
    ap.add_argument('--sample', type=int, default=0, help='only N functions, evenly spaced in address order')
    ap.add_argument('--only', nargs='*', default=[])
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    ap.add_argument('--no-summary', action='store_true', help='do not write decomp/reports/draft_summary.txt')
    a = ap.parse_args()
    t0 = time.time()
    out = os.path.abspath(a.out)
    for s in ('c', 'asm', 'obj', 'log'):
        shutil.rmtree(os.path.join(out, s), ignore_errors=True)
        os.makedirs(os.path.join(out, s))
    ver, flags = game_flags()
    # inputs, copied once: the other builds may rewrite decomp/build while this runs
    funcs, jt = game_functions()
    assert funcs and funcs[0][1] == GAME_START, 'game code asm not as expected (run batch 9 first)'
    shutil.rmtree(os.path.join(out, 'orig'), ignore_errors=True)
    os.makedirs(os.path.join(out, 'orig'))
    copies = {}
    for f in funcs:
        if f[4] not in copies:
            copies[f[4]] = os.path.join(out, 'orig', '%03d.o' % len(copies))
            shutil.copyfile(f[4], copies[f[4]])
    funcs = [(n, ad, sz, b, copies[o]) for n, ad, sz, b, o in funcs]
    starts = {n: ad for n, ad, _, _, _ in funcs}

    def jtext(name):
        out = []
        for l in jt[name]:
            m = re.match(r'^\s*\.4byte (\S+)\+0x([0-9A-Fa-f]+)\s*$', l)
            if m and m.group(1) in starts:
                l = '	.4byte .L_%08X' % (starts[m.group(1)] + int(m.group(2), 16))
            out.append(l)
        return ['.obj %s, local' % name] + out + ['.endobj %s' % name]
    for name, ad, sz, b, _ in funcs:
        for t in sorted(set(re.findall(r'jumptable_[0-9A-F]{8}', chr(10).join(b)))):
            if t in jt:
                tx = jtext(t)
                want = set(re.findall(r'\.L_([0-9A-F]{8})', chr(10).join(tx)))
                have = set(re.findall(r'^\.L_([0-9A-F]{8}):', chr(10).join(b), re.M))
                nb = []
                for l in b:   # a jump-table target may have no label yet (nothing else branches there)
                    m = re.match(r'^/\* ([0-9A-F]{8}) ', l)
                    if m and m.group(1) in want - have:
                        nb.append('.L_%s:' % m.group(1))
                    nb.append(l)
                b = nb + [''] + tx
        open(os.path.join(out, 'asm', name + '.s'), 'w', encoding='utf-8', newline='\n').write('\n'.join(b) + '\n')
    jobs = [(n, ad, sz, o) for n, ad, sz, _, o in funcs]
    if a.only:
        jobs = [j for j in jobs if j[0] in a.only]
    elif a.sample:
        step = len(jobs) / a.sample
        jobs = [jobs[int(i * step)] for i in range(a.sample)]
    # the m2c context: headers preprocessed once with the game compiler itself
    open(os.path.join(out, 'ctx.h'), 'w', newline='\n').write(
        '// context of the m2c drafts: m2c macros + every HSD header + our types (symbols/cr_types.h)\n#include "m2c_macros.h"\n'
        + ''.join('#include "%s"\n' % h for h in sorted(os.listdir(os.path.join(G, 'tools/melee/src/sysdolphin/baselib'))) if h.endswith('.h'))
        + '#include "../../symbols/cr_types.h"\n' + globals_decls())
    ctxin = os.path.join(out, 'ctx_in.h')
    shutil.copyfile(os.path.join(out, 'ctx.h'), ctxin)
    cmd = [os.path.join(D, 'build/compilers', ver, 'mwcceppc.exe'), '-E', '-nodefaults', '-proc', 'gekko', '-nosyspath'] + CTX_DEFS \
        + sum([['-i', i] for i in CTX_INC], []) + ['-i', 'drafts', ctxin]
    rc, so, se = run(cmd, 120, cwd=D)
    if rc != 0:
        sys.exit('context preprocess failed:\n' + so[:2000] + se[:2000])
    # m2c's context parser reads plain C: our own preprocessed text, with the macros it defines itself dropped
    open(os.path.join(out, 'ctx_pp.c'), 'w', encoding='utf-8', newline='\n').write(so)
    open(os.path.join(out, 'm2c_run.py'), 'w', newline='\n').write(
        "import sys\nsys.path.insert(0, %r)\nfrom m2c.main import main\nmain()\n" % M2C.replace('\\', '/'))
    # prime m2c's context cache once (workers then only read it)
    run([sys.executable, '-I', os.path.join(out, 'm2c_run.py'), '-t', 'ppc-mwcc-c', '--context', os.path.join(out, 'ctx_pp.c'),
         '-f', funcs[0][0], os.path.join(out, 'asm', funcs[0][0] + '.s')], 300)
    res = {}
    with cf.ProcessPoolExecutor(a.jobs, initializer=init, initargs=(out, ver, flags)) as ex:
        for name, status, note in ex.map(one, jobs, chunksize=4):
            res[name] = (status, note)
    # reports (address order, no times, no paths)
    cnt = collections.Counter()
    hist = collections.Counter()
    errs = collections.Counter()
    lines = []
    for name, ad, sz, _ in jobs:
        status, note = res[name]
        if status == 'm2c failed':
            cnt['m2c failed'] += 1
            errs['m2c: ' + note] += 1
        else:
            cnt['m2c ok'] += 1
            if status == 'compile failed':
                cnt['compile failed'] += 1
                errs['compile: ' + note] += 1
            else:
                cnt['compiled'] += 1
                p = 100 if status.startswith('100') else int(status[:-1])
                hist[min(p // 10, 10)] += 1
                if p == 100:
                    cnt['100% MATCH'] += 1
        lines.append('%08X  %-44s %5d  %-14s %s' % (ad, name, sz, status, note))
    n = len(jobs)
    tot = ['functions %d' % n, 'm2c ok %d' % cnt['m2c ok'], 'm2c failed %d' % cnt['m2c failed'],
           'compile failed %d' % cnt['compile failed'], 'compiled %d' % cnt['compiled'], '100%% MATCH %d' % cnt['100% MATCH']]
    hl = ['match %% histogram of the %d compiled drafts (100%% = every word equal, relocation fields masked):' % cnt['compiled']]
    for b in range(10):
        hl.append('  %3d-%2d%%  %d' % (b * 10, b * 10 + 9, hist[b]))
    hl.append('  100%%     %d' % hist[10])
    el = ['most common failures:'] + ['  %5d  %s' % (c, m) for m, c in sorted(errs.items(), key=lambda x: (-x[1], x[0]))[:25]]
    head = ['draft and check: tools/draft_batch.py (m2c %s, compiler %s, flags: %s)' % ('pinned in tools/pins.txt', ver, ' '.join(flags[:0]) or 'configure.py game set'),
            'addr      function                                       bytes  status         note']
    open(os.path.join(out, 'draft_report.txt'), 'w', encoding='utf-8', newline='\r\n').write(
        '\n'.join(head + lines + [''] + tot + [''] + hl + [''] + el) + '\n')
    if not a.no_summary and not a.sample and not a.only:
        rp = os.path.join(D, 'reports', 'draft_summary.txt')
        open(rp, 'w', encoding='utf-8', newline='\r\n').write('\n'.join(
            ['draft and check of the game code (tools/draft_batch.py, tools/batches/batch12_drafts.ps1; docs/drafts.md)',
             'm2c drafts of every game function (decomp/drafts/, gitignored), compiled with the game compiler (%s) and compared with the original' % ver,
             'counts only; the drafts themselves are decompiled output and are never committed', ''] + tot + [''] + hl + [''] + el) + '\n')
    print('\n'.join(tot))
    print('\n'.join(hl))
    print('runtime %d s (%d jobs)' % (time.time() - t0, a.jobs))


if __name__ == '__main__':
    main()
