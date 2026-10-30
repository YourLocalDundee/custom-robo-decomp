#!/usr/bin/env bash
# Batch 2: Phase 0 step 4 - compare Ghidra's C export with decomp-toolkit's analysis.
# Needs a current decompiled/main_dol.c. Report: symbols/crosscheck_report.txt
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
"$PY" -I "$g/tools/crosscheck_dtk.py" || die 'crosscheck_dtk.py failed'
