#!/usr/bin/env bash
# SHA-256 of every file the pipeline produces from the ISO, so anyone can check their rebuild matches ours.
# Only hashes are published (docs/expected_checksums.txt), never the files themselves. Same files, same lines and the
# same comparison as tools/checksums.ps1 (the names keep their backslashes: they are the lines of the published file).
#   tools/checksums.sh             print hashes of your outputs
#   tools/checksums.sh --compare   compare with docs/expected_checksums.txt (exit 1 on any difference)
#   tools/checksums.sh --write     maintainers: refresh docs/expected_checksums.txt after an intended change
set -euo pipefail
t=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$t")
. "$t/common.sh"
compare=0; write=0
for a in "$@"; do
    case "$a" in
        --compare | -Compare) compare=1 ;;
        --write | -Write) write=1 ;;
        *) die "usage: tools/checksums.sh [--compare | --write]" ;;
    esac
done
files=(
    'Custom Robo extracted\sys\main.dol'      # input: must match before anything else can
    'tools\dtk\proj\symbols.txt'              # dtk analysis (setup step 5)
    'tools\dtk\proj\splits.txt'
    'symbols\symbols.txt'                     # verified names after batches 1 + 6 reach their fixpoint (run_all)
    'symbols\layout_candidates.txt'
    'symbols\candidates_sdk.txt'              # lib candidates (batch 6), after the names <-> Ghidra fixpoint
    'symbols\candidates_rt.txt'
    'symbols\candidates_hsd.txt'
    'symbols\lib_candidates_report.txt'
    'symbols\descriptive.txt'                 # game names: hand lines + the "// game-real" lines of batch 7
    'symbols\game_real.txt'                   # batch 7 (tools/match_game_names.py): G1 / G2 names, G3 module notes
    'symbols\game_unnamed_real.txt'           # batch 7 leftovers
    'symbols\names_crosscheck.txt'            # name cross-check (batch 8)
    'symbols\verify_report.txt'
    'symbols\types_report.txt'                # header types, prototypes, globals (ParseHeaders.java)
    'symbols\crosscheck_report.txt'
    'symbols\game_map.tsv'
    'decompiled\main_dol.c'                   # Ghidra's C output
    'decompiled\xrefs\functions.tsv'
    'decompiled\xrefs\refs.tsv'
    'decompiled\xrefs\strings.tsv'
    'decomp\config\GXCE01\symbols.txt'     # Phase 4 build config (batch 9): dtk analysis + our names, dtk-normalised
    'decomp\config\GXCE01\splits.txt'      # proven object boundaries (tools/gen_dtk_splits.py)
    'decomp\reports\symbols_report.txt'
    'decomp\reports\splits_report.txt'
    'decomp\reports\build_report.txt'       # built main.dol SHA-1 == retail
    'decomp\config\GXCE01\config.yml'      # + the generated relocation fixes (tools/gen_dtk_relocs.py, Milestone 2)
    'decomp\reports\relocs_report.txt'      # relocation audit: what dtk missed / invented + the check of the build
    'decomp\reports\shift_report.txt'       # static shift test (tools/shift_test.py): padded relink, all checked
    'decomp\reports\progress_report.txt'    # Milestone 4: objects built from C, bytes per library (tools/check_c_objects.py)
    'decomp\reports\sources_report.txt'     # provenance of every file in decomp/src + decomp/include
    'decomp\reports\c_symbols_report.txt'   # the C objects' own symbols put into symbols.txt (tools/gen_c_symbols.py)
    'decomp\reports\c_callsites_report.txt' # names from the matched C's call sites, rule R7 (tools/check_c_callsites.py)
)
now=()
for f in "${files[@]}"; do
    p="$g/${f//\\//}"   # names in the list use backslashes
    if [ -f "$p" ]; then h=$(sha256_of "$p"); else h=$(printf '%-64s' MISSING); fi
    now+=("$h  $f")
done
expected="$g/docs/expected_checksums.txt"
if [ "$write" = 1 ]; then printf '%s\n' "${now[@]}" | crlf > "$expected"; echo "wrote $expected"; exit 0; fi
if [ "$compare" = 0 ]; then printf '%s\n' "${now[@]}"; exit 0; fi
want=$(nocr < "$expected" | grepq -v '^#')
bad=0
for line in "${now[@]}"; do
    f=${line:66}
    w=$(printf '%s\n' "$want" | while IFS= read -r x; do [ "${x:66}" = "$f" ] && printf '%s\n' "$x"; done; true)
    if [ "$w" = "$line" ]; then echo "OK        $f"
    elif [ "${line#MISSING}" != "$line" ]; then echo "MISSING   $f"; bad=$((bad + 1))
    else echo "DIFFERENT $f"; bad=$((bad + 1)); fi
done
if [ "$bad" != 0 ]; then echo "$bad file(s) differ (see docs/REPRODUCE.md, 'If a checksum differs')"; exit 1; fi
echo "all ${#now[@]} files match"
