#!/usr/bin/env bash
# Batch 10: which CodeWarrior compiler + flags built each part of main.dol (plan.md Phase 4 Milestone 3, docs/compiler.md).
# batch10_compiler_probe.ps1 for Linux / macOS (the compilers run through the pinned wibo, or wine).
#   1. tools/setup_decomp.sh: compilers, binutils, your main.dol (skips what is done)
#   2. Melee's src/MetroTRK/ in the sparse checkout (added for this batch; an older clone gets it here)
#   3. python -I tools/compiler_probe/probe.py -> tools/compiler_probe/out/{results.tsv,summary.txt} (gitignored)
# Reads only main.dol, decomp/config/GXCE01/symbols.txt and the pinned references; writes nothing outside out/.
# Usage:  tools/batches/batch10_compiler_probe.sh [--jobs N]
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
jobs=$(ncpu)
while [ $# -gt 0 ]; do
    case "$1" in
        --jobs | -Jobs) shift; jobs=$1 ;;
        *) die 'usage: tools/batches/batch10_compiler_probe.sh [--jobs N]' ;;
    esac
    shift
done

"$g/tools/setup_decomp.sh"
if [ ! -d "$g/tools/melee/src/MetroTRK" ]; then
    git -C "$g/tools/melee" sparse-checkout add /src/MetroTRK/ || die 'adding src/MetroTRK/ to the Melee checkout failed'
fi
for r in dolsdk2004 marioparty4; do
    [ -d "$g/tools/refs/$r/src" ] || die "tools/refs/$r missing: run tools/setup.sh first"
done

"$PY" -I "$g/tools/compiler_probe/probe.py" --jobs "$jobs" || die 'probe.py failed'

# the headline per library: the best version + flags and its matched / compared functions
nocr < "$g/tools/compiler_probe/out/summary.txt" | awk '
    /^==== / { print }
    best { print; best = 0 }
    /^best 25/ { best = 1 }
    /^functions compared/ { print }'
echo "batch 10 done: details in tools/compiler_probe/out/summary.txt, every compile in results.tsv; read with docs/compiler.md"
