#!/usr/bin/env bash
# Batch 8: independent accuracy cross-check of EVERY name (symbols.txt, candidates_*.txt, descriptive.txt, cr_globals.txt).
# Re-derives evidence per name (size vs every reference, call graph vs reference sources / KAR asm, asserts and strings,
# uniqueness; for game names the table entries / strings / references cited in the why:) instead of trusting the tag.
# Needs a current decompiled/index.db (batch 4, after the Ghidra rebuild). Stops the run when any name FAILs.
# Reports: symbols/names_crosscheck.txt (FAILs first), docs/names_crosscheck.md. Method: tools/crosscheck_names.py docstring.
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
rc=0
"$PY" -I "$g/tools/crosscheck_names.py" || rc=$?
[ "$rc" != 1 ] || die 'crosscheck_names.py: some names FAIL (see symbols/names_crosscheck.txt, FAILs first)'
[ "$rc" = 0 ] || die 'crosscheck_names.py failed (inputs missing? run batch 4 / the Ghidra rebuild first)'
