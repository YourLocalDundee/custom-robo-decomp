"""The reference checkouts are the ones tools/setup.ps1 / tools/setup.sh make: pinned commit and sparse paths.

The name batches and gen_dtk_splits.py read the reference sources as text (tools/layout_evidence.py, match_lib_*.py,
crosscheck_names.py walk every .c / .h of tools/melee and tools/refs/*), so their output depends on WHICH files a
clone has checked out. setup widens an existing clone's sparse paths when its list grows (pinned_clone /
PinnedClone), but a clone made before the change and never set up again keeps the old, narrower file set and
silently gives other names than a fresh setup (plan.md step 68b: tp / pikmin2 widened in step 68, the Windows clones
still config-only, a Linux setup from scratch got different names and lost dolphin/dvd/dvdfs.c).

Reads the PinnedClone calls of tools/setup.ps1 and the pinned_clone calls of tools/setup.sh (both must name the same
paths: the twins kept in step), and checks every clone that setup makes and that exists here: HEAD is the commit of
tools/pins.txt and `git sparse-checkout list` is exactly the setup's list. A missing clone is setup's business (the
tools that need it fail on their own); HSDLib / m2c (no sparse list) are checked for their commit only.
Exit 1 with what to run when anything differs.
usage: python -I tools/check_refs.py
"""
import os, re, subprocess, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def pins():
    out = {}
    for l in open(os.path.join(G, 'tools/pins.txt'), encoding='utf-8'):
        f = l.split()
        if len(f) >= 4 and not f[0].startswith('#') and f[0] == 'git': out[f[1]] = f[3]
    return out


def ps1_clones():
    """tools/setup.ps1: PinnedClone <name> "<dir>" @('..', ..) | $null, also inside foreach ($r in 'a', 'b') { }"""
    text = open(os.path.join(G, 'tools/setup.ps1'), encoding='utf-8').read()
    text = re.sub(r'(?m)^\s*#.*$', '', text)
    out = {}
    call = re.compile(r'PinnedClone\s+(\$r|[\w-]+)\s+"\$t/([^"]+)"\s+(\$null|@\(([^)]*)\))')
    loops = [(m.start(), m.end(), re.findall(r"'([^']+)'", m.group(1)))
             for m in re.finditer(r'foreach\s*\(\$r in ([^)]*)\)\s*\{[^}]*\}', text)]
    for m in call.finditer(text):
        if text[max(0, m.start() - 9):m.start()].endswith('function '): continue
        paths = re.findall(r"'([^']+)'", m.group(4) or '')
        if m.group(1) == '$r':
            names = next(n for a, b, n in loops if a <= m.start() < b)
            for n in names: out[n] = (m.group(2).replace('$r', n), paths)
        else: out[m.group(1)] = (m.group(2), paths)
    return out


def sh_clones():
    """tools/setup.sh: pinned_clone <name> "$t/<dir>" paths... (continued with \\), also inside for r in a b; do .. done"""
    text = open(os.path.join(G, 'tools/setup.sh'), encoding='utf-8').read().replace('\\\n', ' ')
    lines = [l for l in text.split('\n') if not l.lstrip().startswith('#')]
    out, loop = {}, None
    for l in lines:
        s = l.strip()
        m = re.match(r'for r in ([^;]+); do$', s)
        if m: loop = m.group(1).split(); continue
        if s == 'done': loop = None; continue
        m = re.match(r'pinned_clone\s+("\$r"|[\w-]+)\s+"\$t/([^"]+)"(.*)$', s)
        if not m: continue
        paths = [p.strip("'\"") for p in m.group(3).split()]
        if m.group(1) == '"$r"':
            for n in loop: out[n] = (m.group(2).replace('$r', n), paths)
        else: out[m.group(1)] = (m.group(2), paths)
    return out


def git(d, *args):
    r = subprocess.run(['git', '-C', d] + list(args), stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True)
    return r.stdout if r.returncode == 0 else None


def main():
    pin = pins()
    ps1, sh = ps1_clones(), sh_clones()
    errors = []
    for n in sorted(set(ps1) | set(sh)):
        if n == 'HSDLib': continue   # Windows-only (setup.sh skips it)
        if ps1.get(n) != sh.get(n):
            errors.append(f'{n}: tools/setup.ps1 clones {ps1.get(n)}, tools/setup.sh {sh.get(n)} (keep the twins in step)')
    checked = 0
    for n, (rel, paths) in sorted(ps1.items()):
        d = os.path.join(G, 'tools', rel)
        if not os.path.isdir(os.path.join(d, '.git')): continue
        checked += 1
        head = (git(d, 'rev-parse', 'HEAD') or '').strip()
        if n in pin and head != pin[n]:
            errors.append(f'tools/{rel}: HEAD {head or "?"} is not the pinned {pin[n]} (tools/pins.txt)')
        if not paths: continue
        have = [l.strip() for l in (git(d, 'sparse-checkout', 'list') or '').splitlines() if l.strip()]
        if have != paths:
            errors.append(f'tools/{rel}: sparse paths {have} are not the setup\'s {paths}')
    for e in errors: print('  ' + e)
    if errors:
        print('reference checkouts differ from the setup: run tools/setup.ps1 (Windows) or tools/setup.sh again '
              '(it updates existing clones), then this run')
        sys.exit(1)
    print(f'reference checkouts: {checked} clones at their pinned commits with the setup\'s sparse paths')


if __name__ == '__main__':
    main()
