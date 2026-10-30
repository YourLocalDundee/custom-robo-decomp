#!/usr/bin/env bash
# Rebuild the Ghidra project and decompiled C from scratch, reapplying every saved name (tools/rebuild_decomp.ps1 for
# Linux / macOS: the same headless run with the same scripts and arguments).
# Run from anywhere:  tools/rebuild_decomp.sh
set -euo pipefail
t=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$t")
. "$t/common.sh"
# output folders are not in git (fresh clone): create them
mkdir -p "$g/decompiled" "$g/ghidra_project"
# Ghidra must not be running on this project already (an earlier run left behind would keep writing the export)
if [ -e "$g/ghidra_project/CustomRobo.lock" ]; then
    die "ghidra_project/CustomRobo.lock exists: another Ghidra is using the project (close it, or delete the lock if none runs)"
fi
# a failed rebuild must not leave the previous export behind for the name batches to read
rm -f "$g/decompiled/main_dol.c" "$g/decompiled/xrefs/functions.tsv" "$g/decompiled/xrefs/refs.tsv" "$g/decompiled/xrefs/strings.tsv"
# declarations-only copy of the Melee headers for Ghidra's C parser (see tools/clean_headers.py)
"$PY" -I "$t/clean_headers.py" "$t/melee" "$g/build/headers" || die 'clean_headers.py failed'

headless="$t/ghidra_12.1.4_PUBLIC/support/analyzeHeadless"
[ -x "$headless" ] || die "$headless missing or not executable: run tools/setup.sh first"
set +e
"$headless" "$g/ghidra_project" CustomRobo \
    -import "$g/Custom Robo extracted/sys/main.dol" -overwrite \
    -loader GameCubeLoader -loader-autoloadMaps false \
    -scriptPath "$t/scripts" \
    -preScript EnableGCAnalyzer.java \
    -postScript CreateDtkFunctions.java "$t/dtk/proj/symbols.txt" \
    -postScript ApplySymbols.java "$g/symbols/symbols.txt" \
    -postScript ApplySymbols.java "$g/symbols/descriptive.txt" \
    -postScript DefineStrings.java \
    -postScript ParseHeaders.java "$g/build/headers" "$g/symbols/verify_report.txt" "$g/symbols/types_report.txt" \
        "$g/symbols/cr_types.h" "$g/symbols/cr_globals.txt" \
    -postScript ExportXrefs.java "$g/decompiled/xrefs" \
    -postScript ExportDecompiledC.java "$g/decompiled/main_dol.c" > "$g/decompiled/ghidra_log.txt" 2>&1
headless_exit=$?
set -e
grepq -E 'Created|Applied|Defined|Parsed|Exported|ERROR' "$g/decompiled/ghidra_log.txt"
# the rebuild is only usable when Ghidra finished both exports and the C has every exported function
nf=$(grepq -o -E 'Exported [0-9]+ functions, [0-9]+ references' "$g/decompiled/ghidra_log.txt" | head -n 1 | awk '{ print $2 }')
nc=$(grepq -o -E 'Exported [0-9]+ functions \(0 failed\) to' "$g/decompiled/ghidra_log.txt" | head -n 1 | awk '{ print $2 }')
heads=0
[ -f "$g/decompiled/main_dol.c" ] && heads=$(grep -c -E '^// [0-9a-f]{8}  ' "$g/decompiled/main_dol.c" || true)
if [ "$headless_exit" != 0 ] || [ -z "$nf" ] || [ "$nf" != "$nc" ] || [ "$nc" != "$heads" ]; then
    die "Ghidra rebuild incomplete (exit $headless_exit; xrefs '$nf', C '$nc' functions, $heads in main_dol.c): see decompiled/ghidra_log.txt"
fi
