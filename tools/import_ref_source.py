"""Copy a C source (and the headers it includes) from a pinned reference decomp into decomp/ (plan.md Phase 4 M4).

Only the community's own matching C of the pinned references (tools/pins.txt) is copied; never decompiler output.
Each copied file gets ONE provenance comment prepended to its first line (same line, so __LINE__ and every line
number stay those of the reference):
    /* Custom Robo: from <repo> @ <commit>, <path> */ <original first line>
tools/check_c_objects.py lists every such file in decomp/reports/sources_report.txt (and whether it was changed
since: the rest of the file is compared with the reference).
Headers: every #include of the copied file (recursively) is looked up like the compiler would, first next to the
including file inside the reference, then in the given include roots (--inc REFDIR=DESTDIR, relative to the reference
and to decomp/); found ones are copied the same way (next to the source -> next to the destination; under REFDIR ->
under DESTDIR). A file already in decomp/ is never overwritten (it may have been adapted), unless --force.
Usage:
  python -I tools/import_ref_source.py REF REFPATH DEST [--inc REFDIR=DESTDIR ...] [--force] [--no-headers]
  e.g. python -I tools/import_ref_source.py dolsdk2004 src/os/OSArena.c src/dolphin/os/OSArena.c \\
           --inc include=include/dolsdk2004 --inc include/libc=include/dolsdk2004/libc
REF: a name in tools/pins.txt (melee = tools/melee, the others tools/refs/<name>).
"""
import os
import re
import subprocess
import sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = os.path.join(G, 'decomp')
TAG = b'/* Custom Robo: from '
INC = re.compile(rb'^\s*#\s*include\s*([<"])([^>"]+)[>"]', re.M)


def pins():
    out = {}
    for ln in open(os.path.join(G, 'tools', 'pins.txt'), encoding='utf-8'):
        f = ln.split()
        if len(f) >= 4 and f[0] == 'git':
            out[f[1]] = (f[2], f[3])
    return out


def ref_dir(name):
    return os.path.join(G, 'tools', 'melee') if name == 'melee' else os.path.join(G, 'tools', 'refs', name)


def repo_name(url):
    m = re.search(r'github\.com/(.+?)(?:\.git)?$', url)
    return m[1] if m else url


def provenance(ref, refpath):
    url, commit = pins()[ref]
    head = subprocess.run(['git', '-C', ref_dir(ref), 'rev-parse', 'HEAD'], capture_output=True, text=True).stdout.strip()
    if head != commit:
        sys.exit(f'{ref_dir(ref)} is at {head}, tools/pins.txt says {commit}')
    return TAG + f'{repo_name(url)} @ {commit}, {refpath} */ '.encode()


def copy(ref, refpath, dest, force):
    src = os.path.join(ref_dir(ref), refpath)
    dst = os.path.join(D, dest)
    data = open(src, 'rb').read()
    if os.path.exists(dst) and not force:
        return data, False
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    open(dst, 'wb').write(provenance(ref, refpath) + data)
    return data, True


def main(argv):
    force = '--force' in argv
    headers = '--no-headers' not in argv
    incs = []
    pos = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == '--inc':
            r, d = argv[i + 1].split('=', 1)
            incs.append((r.strip('/'), d.strip('/')))
            i += 2
            continue
        if not a.startswith('--'):
            pos.append(a)
        i += 1
    if len(pos) != 3:
        sys.exit(__doc__)
    ref, refpath, dest = pos
    root = ref_dir(ref)
    todo = [(refpath.replace('\\', '/'), dest.replace('\\', '/'))]
    seen = set()
    while todo:
        rp, dp = todo.pop()
        if rp in seen:
            continue
        seen.add(rp)
        data, new = copy(ref, rp, dp, force)
        print(f'{"copied " if new else "kept   "} {ref}:{rp} -> decomp/{dp}')
        if not headers:
            continue
        for kind, name in INC.findall(data):
            name = name.decode()
            cands = []
            if kind == b'"':
                cands.append((os.path.dirname(rp) + '/' + name, os.path.dirname(dp) + '/' + name))
            for r, d in incs:
                cands.append((r + '/' + name, d + '/' + name))
            for crp, cdp in cands:
                crp = os.path.normpath(crp).replace('\\', '/')
                if os.path.isfile(os.path.join(root, crp)):
                    todo.append((crp, os.path.normpath(cdp).replace('\\', '/')))
                    break
            else:
                print(f'  note: {name} (included by {rp}) not found in {ref}')


if __name__ == '__main__':
    main(sys.argv[1:])
