"""Rewrite tools/patches/dtk_template_configure.patch from decomp/configure.py (plan.md Phase 4).

decomp/configure.py is the pinned dtk-template's configure.py (tools/refs/dtk-template, commit in tools/pins.txt) plus
our patch; tools/setup_decomp.ps1 recreates it from the template + patch on every run. To change it: edit
decomp/configure.py, run this script, then run tools/setup_decomp.ps1 (or batch 9) once: the patch must reproduce the
file byte for byte (this script checks that, with git apply on a scratch copy).
Usage: python -I tools/make_configure_patch.py
"""
import os
import shutil
import subprocess
import sys
import tempfile

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TPL = os.path.join(G, 'tools', 'refs', 'dtk-template')
SRC = os.path.join(G, 'decomp', 'configure.py')
PATCH = os.path.join(G, 'tools', 'patches', 'dtk_template_configure.patch')


def main():
    a = os.path.join(TPL, 'configure.py')
    r = subprocess.run(['git', 'diff', '--no-index', '--no-color', '--full-index', a, SRC], capture_output=True)
    if r.returncode not in (0, 1):
        sys.exit(r.stderr.decode())
    lines = r.stdout.split(b'\n')
    out = []
    for ln in lines:
        if ln.startswith(b'diff --git '):
            ln = b'diff --git a/configure.py b/configure.py'
        elif ln.startswith(b'--- '):
            ln = b'--- a/configure.py'
        elif ln.startswith(b'+++ '):
            ln = b'+++ b/configure.py'
        elif ln.startswith(b'index '):
            continue   # the blob ids are not needed by git apply (and differ per checkout)
        out.append(ln)
    patch = b'\n'.join(out)
    # check: the template + this patch == decomp/configure.py
    with tempfile.TemporaryDirectory() as t:
        shutil.copy(a, os.path.join(t, 'configure.py'))
        open(os.path.join(t, 'p.patch'), 'wb').write(patch)
        subprocess.run(['git', 'init', '-q', t], check=True)
        subprocess.run(['git', '-C', t, '-c', 'core.autocrlf=false', 'apply', 'p.patch'], check=True)
        if open(os.path.join(t, 'configure.py'), 'rb').read() != open(SRC, 'rb').read():
            sys.exit('the patch does not reproduce decomp/configure.py')
    open(PATCH, 'wb').write(patch)
    print(f'wrote {os.path.relpath(PATCH, G)} ({len(patch)} bytes); template + patch == decomp/configure.py')


if __name__ == '__main__':
    main()
