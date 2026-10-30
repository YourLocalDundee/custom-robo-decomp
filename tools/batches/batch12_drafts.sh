#!/usr/bin/env bash
# Batch 12: draft and check the game code (plan.md Phase 4 Milestone 5, docs/drafts.md); batch12_drafts.ps1 for
# Linux / macOS (the game compiler runs through the pinned wibo, or wine).
#   1. needs batch 9 (decomp/build/GXCE01/asm + obj, decomp/build/compilers) and tools/m2c (tools/setup.sh); read only
#   2. python -I tools/draft_batch.py -> decomp/drafts/ (GITIGNORED: drafts, asm, objects, logs, draft_report.txt)
#      and decomp/reports/draft_summary.txt (counts only; committed)
#   3. python -I tools/promote_drafts.py: every 100% draft that can be linked as it is -> decomp/src/game/<function>.c
#      (COMMITTED) and decomp/reports/promote_report.txt. When decomp/src/game changed, batch 9 runs again here.
#      (Not with --sample: a sample's report lists only some functions.)
# Never commit anything in decomp/drafts/. Not in tools/checksums.sh (see docs/drafts.md "Determinism").
# Usage:  tools/batches/batch12_drafts.sh [--sample N] [--jobs N]
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
sample=0; jobs=$(ncpu)
while [ $# -gt 0 ]; do
    case "$1" in
        --sample | -Sample) shift; sample=$1 ;;
        --jobs | -Jobs) shift; jobs=$1 ;;
        *) die 'usage: tools/batches/batch12_drafts.sh [--sample N] [--jobs N]' ;;
    esac
    shift
done
for p in tools/m2c/m2c/main.py decomp/build/GXCE01/config.json decomp/build/GXCE01/asm decomp/build/GXCE01/obj decomp/build/compilers/GC/2.6/mwcceppc.exe tools/melee/src/sysdolphin/baselib; do
    [ -e "$g/$p" ] || die "$p missing: run tools/setup.sh (m2c, Melee headers) and batch 9 first"
done
args=(--jobs "$jobs")
[ "$sample" = 0 ] || args+=(--sample "$sample")
"$PY" -I "$g/tools/draft_batch.py" "${args[@]}" || die 'draft_batch.py failed'
if [ "$sample" = 0 ]; then
    p=$("$PY" -I "$g/tools/promote_drafts.py") || die 'promote_drafts.py failed'
    printf '%s\n' "$p"
    case "$(printf '%s\n' "$p" | tail -n 1)" in
        changed*)
            echo "== decomp/src/game changed: batch 9 again (the build links the promoted files)"
            "$b/batch9_decomp_build.sh" ;;
    esac
fi
echo "drafts: decomp/drafts/draft_report.txt (local); counts: decomp/reports/draft_summary.txt; promoted: decomp/reports/promote_report.txt"
