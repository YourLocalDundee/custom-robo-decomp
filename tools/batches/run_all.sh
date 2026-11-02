#!/usr/bin/env bash
# The ONE script that runs everything, in order, stopping at the first failure (tools/batches/run_all.ps1 for
# Linux / macOS, same order, same outputs):
#   Ghidra rebuild -> batches 1 + 6 + 7 as a CHECK of the committed names -> 5 types -> 2 dtk cross-check -> 3 game map
#   -> 4 search index -> 8 name cross-check -> 9 decomp build (retail SHA-1, relocations, static shift test)
#   -> 10 compiler probe (~5 min) -> 11 (Windows-only: skipped here) -> 12 drafts
#   -> checksums.sh --compare (every output byte-identical to docs/expected_checksums.txt), then a summary.
# tools/from_scratch.sh wraps it: setup -> this -> archives / models / mod build.
# Usage:  tools/batches/run_all.sh [--skip-rebuild] [--apply-names [--max-passes 5]]
#
# The committed name files are the truth (plan.md step 110): by default the name batches only report what they would
# change (symbols/name_proposals.txt, tools/name_check.py) and the committed files are put back. --apply-names takes
# their names (rebuild <-> batches to a fixpoint) and stops if a name the C sources use was dropped or moved.
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
skip_rebuild=0; apply_names=0; max_passes=5
while [ $# -gt 0 ]; do
    case "$1" in
        --skip-rebuild | -SkipRebuild) skip_rebuild=1 ;;
        --apply-names | -ApplyNames) apply_names=1 ;;
        --max-passes | -MaxPasses) shift; max_passes=$1 ;;
        *) die 'usage: tools/batches/run_all.sh [--skip-rebuild] [--apply-names [--max-passes N]]' ;;
    esac
    shift
done
t0=$(date +%s)
sym="$g/symbols/symbols.txt"
# everything batches 1 + 6 + 7 write that the next pass or the rebuild reads (ParseHeaders.java reads verify_report.txt,
# ApplySymbols.java descriptive.txt)
name_files=(symbols.txt layout_candidates.txt candidates_sdk.txt candidates_rt.txt candidates_hsd.txt
    verify_report.txt lib_candidates_report.txt descriptive.txt game_real.txt game_unnamed_real.txt)

snapshot() {
    local f out=""
    for f in "${name_files[@]}"; do
        if [ -f "$g/symbols/$f" ]; then out="$out $(sha256_of "$g/symbols/$f")"; else out="$out missing"; fi
    done
    echo "$out"
}
names() {
    echo "== names ($1)"
    "$b/batch1_names.sh"
    "$b/batch6_lib_candidates.sh"
    "$b/batch7_game_names.sh"
    "$PY" -I "$g/tools/lint_descriptive.py" || die 'symbols/descriptive.txt has problems (see above)'
}

# 0. the reference checkouts are the setup's: their sources are read as text, other files give other names (step 68b)
"$PY" -I "$g/tools/check_refs.py" || die 'reference checkouts differ from tools/setup.sh (see above)'
# 1. the committed starting point
snap="$g/build/name_snapshot"
"$PY" -I "$g/tools/name_check.py" snapshot "$snap" || die 'name_check.py snapshot failed'
passes=0
if [ "$apply_names" = 1 ]; then
    grepq -v -E '// (layout-proof|lib-candidate) ' "$sym" | crlf > "$sym.tmp" && mv -f "$sym.tmp" "$sym"
    for m in sdk rt hsd; do
        printf '%s\n' "// all rounds of batch6_lib_candidates.ps1 (tools\match_lib_$m.py)" | crlf > "$g/symbols/candidates_$m.txt"
    done
    if [ -f "$g/decompiled/xrefs/refs.tsv" ]; then names 'previous export'; else names 'no export yet'; fi
    if [ "$skip_rebuild" = 0 ]; then
        while :; do
            [ "$passes" -lt "$max_passes" ] || die "names did not converge in $max_passes Ghidra rebuilds (symbols/symbols.txt still changing)"
            before=$(snapshot)
            passes=$((passes + 1))
            echo "== Ghidra rebuild, pass $passes"
            "$g/tools/rebuild_decomp.sh"
            names "against rebuild $passes"
            [ "$(snapshot)" != "$before" ] || break
            echo "== rebuild $passes: names changed, rebuilding"
        done
        echo "== names converged: unchanged after Ghidra rebuild $passes ($passes rebuild(s))"
    fi
    "$PY" -I "$g/tools/name_check.py" guard "$snap" || die 'the name batches dropped names the C sources use (see above)'
else
    if [ "$skip_rebuild" = 0 ]; then
        passes=1
        echo "== Ghidra rebuild (committed names)"
        "$g/tools/rebuild_decomp.sh"
    fi
    names 'check against the export'
    "$PY" -I "$g/tools/name_check.py" report "$snap" "$g/symbols/name_proposals.txt" || die 'name_check.py report failed'
fi
"$b/batch5_types.sh"
"$b/batch2_crosscheck.sh"
"$b/batch3_game_map.sh"
"$b/batch4_index.sh"
"$b/batch8_crosscheck.sh"   # needs the fresh index.db from batch 4
# Phase 4 build: outside the name fixpoint (reads the final name files, never Ghidra). Stops the run unless the built
# main.dol is the retail one.
"$b/batch9_decomp_build.sh"
"$b/batch10_compiler_probe.sh"
echo 'batch 11 skipped: the Dolphin boot check (tools/boot_check.ps1) uses Windows window APIs; run it on Windows (the static shift test ran in batch 9)'
"$b/batch12_drafts.sh"   # m2c drafts of the game code, compiled and compared (decomp/drafts/ stays local); the linkable 100% ones -> decomp/src/game (tools/promote_drafts.py), batch 9 again if that changed
"$g/tools/checksums.sh" --compare || die "checksums differ from docs/expected_checksums.txt (docs/REPRODUCE.md, 'If a checksum differs')"

c="$g/decompiled/main_dol.c"
funcs=$(grep -c -E '^// [0-9a-f]{8}  ' "$c" || true)
generic=$(grep -c -E '^// [0-9a-f]{8}  FUN_' "$c" || true)
echo "== summary ($(( $(date +%s) - t0 )) s)"
if [ "$apply_names" = 1 ]; then echo "fixpoint: names unchanged after $passes Ghidra rebuild(s)"; else echo "names: committed names checked, proposals in symbols/name_proposals.txt ($passes Ghidra rebuild(s))"; fi
echo "functions in C: $funcs, still FUN_: $generic, named: $((funcs - generic))"
echo "reports: symbols/verify_report.txt, symbols/crosscheck_report.txt, docs/game_map.md"
echo "decomp build: main.dol == retail SHA-1 (decomp/reports/build_report.txt, splits_report.txt, symbols_report.txt)"
echo "compiler probe: tools/compiler_probe/out/summary.txt (docs/compiler.md); shifted boot (batch 11): Windows only"
echo "checksums: all outputs match docs/expected_checksums.txt"
echo "search: $PY -I tools/find.py text|func|callers|callees|refs|str|hint <arg>   (index: decompiled/index.db)"
