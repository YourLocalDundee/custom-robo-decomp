"""Check the objects built from C and write the progress / provenance reports (plan.md Phase 4 Milestone 4).

Run by batch 9 after ninja (ninja has compiled every decomp/src file and written objdiff's build/GXCE01/report.json):
  1. Every object in configure.py's CR_MATCHING (the objects linked from C), and every game/ object of splits.txt
     (configure.py cr_matching(): decomp/src/game, tools/promote_drafts.py), has its source and objdiff reports 100%
     code AND data for it, every function matched. (The retail SHA-1 of the main.dol linked WITH them is batch 9's
     own check.)
  2. Every other C file in decomp/src is an object of splits.txt listed in CR_NOT_MATCHING with the reason it is not
     linked (it is compiled for objdiff only; that object is linked from dtk's split object, built from main.dol).
  3. decomp/reports/progress_report.txt: per library (top folder of splits.txt) the objects, how many are linked from
     C, and the code / data bytes of those (objdiff's sizes of the target objects), plus objdiff's matched bytes;
     then one line per C object. Deterministic (CRLF).
  4. decomp/reports/sources_report.txt: every file in decomp/src and decomp/include with its provenance (the
     '/* Custom Robo: from <repo> @ <commit>, <path> */' comment tools/import_ref_source.py put on its first line),
     checked against tools/pins.txt, and whether it still equals the reference file (else how many lines differ).
Options (development, not used by the batch):
  --link-each   for every C object at 100% / 100% in objdiff: link the current build with that object ALSO taken from
                C (decomp/build/ccheck/) and print the SHA-1: a C object whose data is not all in its split object
                (a static or string that dtk left in an automatic object) is 100% in objdiff but changes main.dol.
Usage: python -I tools/check_c_objects.py [--link-each]
"""
import ast
import difflib
import json
import os
import re
import struct
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(G, 'tools'))
D = os.path.join(G, 'decomp')
B = os.path.join(D, 'build', 'GXCE01')
REPORTS = os.path.join(D, 'reports')
TAG = re.compile(rb'^/\* Custom Robo: from (\S+) @ ([0-9a-f]{40}), (\S+) \*/ ')


def configure_lists():
    """CR_MATCHING (list) and CR_NOT_MATCHING (dict) from decomp/configure.py, read as literals"""
    tree = ast.parse(open(os.path.join(D, 'configure.py'), 'rb').read())
    out = {}
    for node in tree.body:
        tgt = node.targets[0] if isinstance(node, ast.Assign) else node.target if isinstance(node, ast.AnnAssign) else None
        if isinstance(tgt, ast.Name) and tgt.id in ('CR_MATCHING', 'CR_NOT_MATCHING'):
            out[tgt.id] = ast.literal_eval(node.value)
    matching, not_matching = out.get('CR_MATCHING', []), out.get('CR_NOT_MATCHING', {})
    # configure.py cr_matching(): every game/ object of splits.txt (decomp/src/game) is linked unless CR_NOT_MATCHING has it
    matching = matching + [o for o in split_objects() if o.startswith('game/') and o not in not_matching and o not in matching]
    return matching, not_matching


def split_objects():
    out = []
    for ln in open(os.path.join(D, 'config', 'GXCE01', 'splits.txt'), encoding='utf-8'):
        if ln.strip() and not ln[0].isspace() and not ln.startswith('Sections:'):
            out.append(ln.split(':')[0].strip())
    return out


def pins():
    out = {}
    for ln in open(os.path.join(G, 'tools', 'pins.txt'), encoding='utf-8'):
        f = ln.split()
        if len(f) >= 4 and f[0] == 'git':
            m = re.search(r'github\.com/(.+?)(?:\.git)?$', f[2])
            out[m[1] if m else f[2]] = (f[1], f[3])
    return out


def ref_dir(name):
    return os.path.join(G, 'tools', 'melee') if name == 'melee' else os.path.join(G, 'tools', 'refs', name)


def ref_file(name, refpath):
    """the reference file's real path: the provenance line was written on Windows, where the file system ignores case
    (GCN_mem_alloc.c finds GCN_Mem_Alloc.c), so each component is matched without case when it is not found as is"""
    p = ref_dir(name)
    for part in refpath.replace('\\', '/').split('/'):
        q = os.path.join(p, part)
        if not os.path.exists(q) and os.path.isdir(p):
            hit = [e for e in os.listdir(p) if e.lower() == part.lower()]
            if len(hit) == 1:
                q = os.path.join(p, hit[0])
        p = q
    return p


def elf_sections(path):
    """{name: size} of the allocated sections of an ELF object"""
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('>I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('>HHH', d, 0x2E)
    sh = [struct.unpack_from('>10I', d, shoff + i * shentsize) for i in range(shnum)]
    so = sh[shstrndx][4]
    out = {}
    for s in sh:
        if s[2] & 2 and s[5]:   # SHF_ALLOC
            out[d[so + s[0]:d.index(b'\0', so + s[0])].decode()] = s[5]
    return out


def sources_report():
    p = pins()
    rows, errors = [], []
    for top in ('src', 'include'):
        for dp, dn, fn in os.walk(os.path.join(D, top)):
            dn.sort()
            for f in sorted(fn):
                path = os.path.join(dp, f)
                rel = os.path.relpath(path, D).replace('\\', '/')
                data = open(path, 'rb').read()
                m = TAG.match(data)
                if not m:
                    if data.startswith(b'/* Custom Robo game code, promoted by tools/promote_drafts.py'):
                        rows.append(f'{rel}  ours: a matching m2c draft of the game code, promoted by tools/promote_drafts.py')
                    else:
                        rows.append(f'{rel}  ours (no reference)')
                    continue
                repo, commit, refpath = m[1].decode(), m[2].decode(), m[3].decode()
                if repo not in p:
                    errors.append(f'{rel}: {repo} is not a pinned reference (tools/pins.txt)')
                    continue
                name, pin = p[repo]
                if commit != pin:
                    errors.append(f'{rel}: {repo} @ {commit}, but tools/pins.txt pins {pin}')
                ref = ref_file(name, refpath)
                body = data[m.end():]
                if not os.path.exists(ref):
                    state = 'reference file missing locally (run tools/setup.ps1)'
                else:
                    want = open(ref, 'rb').read()
                    # line ends aside: the references check out CRLF on Windows (git autocrlf) and LF on Linux / macOS
                    if body.replace(b'\r\n', b'\n') == want.replace(b'\r\n', b'\n'):
                        state = 'identical'
                    else:
                        a, b = want.decode('utf-8', 'replace').splitlines(), body.decode('utf-8', 'replace').splitlines()
                        n = sum(1 for t in difflib.ndiff(a, b) if t[:1] in '+-')
                        state = f'changed ({n} lines added / removed)'
                rows.append(f'{rel}  {repo} @ {commit[:12]} {refpath}  {state}')
    head = ['C sources and headers in decomp/src and decomp/include (tools/check_c_objects.py)',
            'Each file is copied from a pinned reference decomp (the community\'s matching C; never decompiler '
            'output) by tools/import_ref_source.py, which puts the provenance on its first line. The game files '
            '(decomp/src/game) are ours: m2c drafts that compile to the retail bytes (tools/promote_drafts.py).',
            'Columns: path in decomp/, reference repo @ commit (tools/pins.txt), path in the reference, state '
            '(identical = byte for byte the reference file after the provenance comment).',
            f'files: {len(rows)}', '']
    return head + rows, errors


def link_each(cands, units, complete):
    import gen_dtk_relocs as gr
    out = os.path.join(D, 'build', 'ccheck')
    os.makedirs(out, exist_ok=True)
    cfg = json.load(open(os.path.join(B, 'config.json')))
    base_of = {u['target_path']: u.get('base_path') for u in units}
    for name in cands:
        tp = f'build/GXCE01/obj/{os.path.splitext(name)[0]}.o'
        objs = []
        for u in cfg['units']:
            o = u['object']
            if (base_of.get(o) and (o in complete)) or o == tp:
                o = base_of[o]
            objs.append(o)
        try:
            sha = gr.link(objs, 'build/GXCE01/ldscript.lcf', 'build/ccheck/main.elf', 'build/ccheck/main.dol')
            res = 'retail' if sha == gr.RETAIL else 'DIFFERENT ' + sha
        except SystemExit:
            log = gr.run.__defaults__   # noqa (the linker output was printed above by gen_dtk_relocs.run)
            res = 'LINK FAILED (linker output above)'
        print(f'{name:45} linked from C with the current build: {res}', flush=True)


def main(argv):
    matching, not_matching = configure_lists()
    objects = split_objects()
    od = json.load(open(os.path.join(D, 'objdiff.json')))
    rep = json.load(open(os.path.join(B, 'report.json')))
    runits = {u['name']: u for u in rep['units']}
    errors = []
    rows = []
    libs = {}
    complete = set()
    cands = []
    for u in od['units']:
        name = u['name'][len('main/'):]
        r = runits[u['name']]
        m = r['measures']
        auto = u['metadata'].get('auto_generated')
        lib = 'automatic objects (unsplit)' if auto else name.split('/')[0]
        L = libs.setdefault(lib, dict(objects=0, c=0, code=0, data=0, ccode=0, cdata=0, mcode=0, mdata=0, fn=0, cfn=0))
        tc, td = int(m.get('total_code', 0)), int(m.get('total_data', 0))
        mc, md = int(m.get('matched_code', 0)), int(m.get('matched_data', 0))
        L['objects'] += 1
        L['code'] += tc
        L['data'] += td
        L['mcode'] += mc
        L['mdata'] += md
        L['fn'] += m.get('total_functions', 0)
        if auto:
            continue
        cname = None
        for ext in ('.c', '.cpp', '.cp'):
            if name + ext in objects:
                cname = name + ext
        if cname is None:
            errors.append(f'{name}: not an object of splits.txt')
            continue
        src = u['metadata'].get('source_path')
        has_src = 'base_path' in u
        full = has_src and mc == tc and md == td and m.get('matched_functions', 0) == m.get('total_functions', 0)
        if cname in matching:
            if not has_src:
                errors.append(f'{cname}: in CR_MATCHING but decomp/src/{cname} is missing')
            elif not full:
                errors.append(f'{cname}: in CR_MATCHING but objdiff reports code {mc}/{tc}, data {md}/{td}')
            elif not u['metadata'].get('complete'):
                errors.append(f'{cname}: in CR_MATCHING but not marked complete in objdiff.json (configure.py?)')
            else:
                complete.add(u['target_path'])
                L['c'] += 1
                L['ccode'] += tc
                L['cdata'] += td
                L['cfn'] += m.get('total_functions', 0)
        elif has_src:
            if cname not in not_matching:
                errors.append(f'{cname}: decomp/src has it, but it is neither in CR_MATCHING nor in CR_NOT_MATCHING')
            cands.append(cname)   # (a function dtk left unnamed, fn_..., is 0% until the C gives it its name)
        if has_src:
            hint = ''
            if os.path.exists(os.path.join(D, u['base_path'])) and os.path.exists(os.path.join(D, u['target_path'])):
                bs, ts = elf_sections(os.path.join(D, u['base_path'])), elf_sections(os.path.join(D, u['target_path']))
                extra = [f'{s} 0x{bs[s]:X}' for s in sorted(bs) if s not in ts and s != '.text']
                if extra:
                    hint = '  (C has data the split object lacks: ' + ', '.join(extra) + ')'
            state = 'linked from C' if cname in matching else 'not linked: ' + not_matching.get(cname, '(no reason given)')
            rows.append((cname, f'{cname:45} code 0x{mc:X}/0x{tc:X}  data 0x{md:X}/0x{td:X}  functions '
                                f'{m.get("matched_functions", 0)}/{m.get("total_functions", 0)}  {state}{hint if cname not in matching else ""}'))
    for name in not_matching:
        if name not in objects:
            errors.append(f'CR_NOT_MATCHING names {name}, not an object of splits.txt')
    for dp, dn, fn in os.walk(os.path.join(D, 'src')):
        for f in fn:
            if f.endswith(('.c', '.cpp', '.cp')):
                rel = os.path.relpath(os.path.join(dp, f), os.path.join(D, 'src')).replace('\\', '/')
                if rel not in objects:
                    errors.append(f'decomp/src/{rel} is not an object of splits.txt')

    def pc(a, b):
        return f'{100.0 * a / b:6.2f}%' if b else '   -   '
    out = ['Phase 4 progress: objects built from C (tools/check_c_objects.py, from objdiff-cli\'s report of the '
           'batch 9 build)',
           'Per library (top folder of splits.txt): split objects, how many are linked from C (configure.py '
           'CR_MATCHING: 100% code and data in objdiff, retail SHA-1 with them linked), and their bytes.',
           'Code / data bytes are the sizes of the split (target) objects; "of all" = share of that library\'s '
           'split bytes now built from C.', '']
    out.append(f'{"library":28} {"objects":>7} {"from C":>6}   {"code from C":>21}   {"data from C":>21}   '
               f'{"functions":>11}')
    order = [x for x in ('dolphin', 'MSL', 'Runtime', 'MetroTRK', 'sysdolphin', 'game') if x in libs] + \
        sorted(x for x in libs if x not in ('dolphin', 'MSL', 'Runtime', 'MetroTRK', 'sysdolphin', 'game'))
    tot = dict(objects=0, c=0, code=0, data=0, ccode=0, cdata=0, fn=0, cfn=0)
    for lib in order:
        L = libs[lib]
        for k in tot:
            tot[k] += L[k]
        out.append(f'{lib:28} {L["objects"]:7} {L["c"]:6}   0x{L["ccode"]:06X}/0x{L["code"]:06X} {pc(L["ccode"], L["code"])}'
                   f'   0x{L["cdata"]:06X}/0x{L["data"]:06X} {pc(L["cdata"], L["data"])}   {L["cfn"]:4}/{L["fn"]:5}')
    out.append(f'{"main.dol (all units)":28} {tot["objects"]:7} {tot["c"]:6}   0x{tot["ccode"]:06X}/0x{tot["code"]:06X} '
               f'{pc(tot["ccode"], tot["code"])}   0x{tot["cdata"]:06X}/0x{tot["data"]:06X} {pc(tot["cdata"], tot["data"])}'
               f'   {tot["cfn"]:4}/{tot["fn"]:5}')
    # the game code: .text from 0x800055E0 up to the first library object (its objects: game/ and the automatic ones there)
    gstart, gend, cur = 0x800055E0, None, None
    for ln in open(os.path.join(D, 'config', 'GXCE01', 'splits.txt'), encoding='utf-8'):
        if ln.strip() and not ln[0].isspace():
            cur = ln.split(':')[0].strip()
        mm = re.match(r'^\s+\.text\s+start:0x([0-9A-Fa-f]+)', ln)
        if mm and cur and not cur.startswith('game/') and int(mm.group(1), 16) >= gstart:
            gend = min(gend or 1 << 32, int(mm.group(1), 16))
    gl = libs.get('game', dict(c=0, ccode=0))
    if gend:
        out += ['', f'game code (.text 0x{gstart:08X}-0x{gend:08X}, 0x{gend - gstart:X} bytes): {gl["c"]} objects from C '
                    f'(one function each, or one file of consecutive functions with their own data: tools/promote_drafts.py), 0x{gl["ccode"]:X} bytes = '
                    f'{pc(gl["ccode"], gend - gstart).strip()} of the game code, {pc(gl["ccode"], tot["code"]).strip()} of all code']
    out += ['', '== C objects (decomp/src): objdiff matched bytes, and whether linked']
    out += [r for _, r in sorted(rows)]
    srows, serr = sources_report()
    errors += serr
    os.makedirs(REPORTS, exist_ok=True)
    open(os.path.join(REPORTS, 'progress_report.txt'), 'wb').write(('\r\n'.join(out) + '\r\n').encode())
    open(os.path.join(REPORTS, 'sources_report.txt'), 'wb').write(('\r\n'.join(srows) + '\r\n').encode())
    print('\n'.join(out[4:4 + len(order) + 2]))
    print(f'C objects linked: {tot["c"]}; C sources tried, not linked: {len(rows) - tot["c"]}')
    if '--link-each' in argv:
        link_each(cands + sorted(matching), od['units'], complete)
    if errors:
        print('\n'.join('ERROR ' + e for e in errors))
        sys.exit(1)


if __name__ == '__main__':
    main(sys.argv[1:])
