#!/usr/bin/env bash
# Batch 9: the Phase 4 build (plan.md Phase 4 Milestones 1 + 2); batch9_decomp_build.ps1 for Linux / macOS, same steps:
#   1. tools/setup_decomp.sh: template files, pinned build tools, your main.dol (skips what is done)
#   2. tools/gen_dtk_symbols.py -> config symbols.txt (+ decomp/reports/symbols_report.txt)
#   3. tools/gen_dtk_splits.py  -> config splits.txt  (+ decomp/reports/splits_report.txt)
#   3b. tools/gen_dtk_relocs.py -> the generated relocation-fix block of config.yml (+ decomp/reports/relocs_report.txt)
#   4. python configure.py; ninja; a second, forced split must leave the config files byte-identical (a fixpoint)
#   4b. tools/gen_c_symbols.py: the objects linked from C put their own symbols into symbols.txt; ninja again
#   5. SHA-1 of decomp/build/GXCE01/main.dol == retail, written to decomp/reports/build_report.txt
#   5b. tools/check_c_objects.py, 5c. tools/check_c_callsites.py, 6. tools/gen_dtk_relocs.py --check, 7. tools/shift_test.py
# Details of every step: tools/batches/batch9_decomp_build.ps1 and docs/REPRODUCE.md section 2b.
# Outside the name fixpoint of run_all.sh: reads the committed name files only, never Ghidra.
# Usage:  tools/batches/batch9_decomp_build.sh
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
d="$g/decomp"
cfg="$d/config/GXCE01"
retail=6a73cd111ef349b20be294511b8c51a8addfb50b
ninja="$g/tools/ninja/ninja"

"$g/tools/setup_decomp.sh"
"$PY" -I "$g/tools/gen_dtk_symbols.py" || die 'gen_dtk_symbols.py failed'
"$PY" -I "$g/tools/gen_dtk_splits.py" || die 'gen_dtk_splits.py failed'
"$PY" -I "$g/tools/gen_dtk_relocs.py" || die 'gen_dtk_relocs.py failed'

hashes() { echo "$(sha256_of "$cfg/symbols.txt") $(sha256_of "$cfg/splits.txt") $(sha256_of "$cfg/config.yml")"; }
(
    cd "$d"
    # always link afresh, so the SHA-1 below is of this run's main.dol
    rm -f build/GXCE01/main.elf build/GXCE01/main.dol
    "$PY" configure.py || die 'configure.py failed'
    # the objects compiled from decomp/src are rebuilt every run: a new override header (decomp/include/cr_*) changes
    # which header an #include finds, and ninja's dependency file of an object compiled before it does not list it
    find build/GXCE01/src -name '*.o' -delete 2>/dev/null || true
    "$ninja" || die 'ninja failed (see above; dtk rejects a bad split here)'
    # the objects linked from C (configure.py CR_MATCHING) give their own symbols to symbols.txt (main.elf has them at
    # the retail addresses): then split again, so objdiff pairs their data by name
    "$PY" -I "$g/tools/gen_c_symbols.py" || die 'gen_c_symbols.py failed'
    "$ninja" || die 'ninja (after gen_c_symbols.py) failed'
    first=$(hashes)
    # force a second split from the normalised files: nothing may change any more
    touch "$cfg/symbols.txt"
    "$ninja" || die 'ninja (second pass) failed'
    [ "$(hashes)" = "$first" ] || die 'dtk changed symbols.txt / splits.txt / config.yml again on the second split: not a fixpoint'
)

sha1=$(sha1_of "$d/build/GXCE01/main.dol")
ok=0; [ "$sha1" = "$retail" ] && ok=1
result=DIFFERENT; [ "$ok" = 1 ] && result=MATCH
# the first line names the .ps1: it is the committed, checksummed report text
{
    echo "decomp build (tools/batches/batch9_decomp_build.ps1): main.dol linked from dtk's split objects and the objects built from C (decomp/reports/progress_report.txt)"
    echo "built  decomp/build/GXCE01/main.dol SHA-1 $sha1"
    echo "retail main.dol (GXCE01 v1.0)       SHA-1 $retail"
    echo "result: $result"
    echo
    head -n 4 "$d/reports/symbols_report.txt" | nocr
    echo
    head -n 4 "$d/reports/splits_report.txt" | nocr
    echo
    grepq '^fixes written' "$d/reports/relocs_report.txt" | nocr | sed 's/^/relocation /'
} | crlf > "$d/reports/build_report.txt"   # CRLF on every OS, like the two generator reports, so the checksum is the same everywhere
sed -n 2,4p "$d/reports/build_report.txt" | nocr
[ "$ok" = 1 ] || die "built main.dol SHA-1 $sha1 is not the retail $retail"
"$PY" -I "$g/tools/check_c_objects.py" || die 'a C object is not 100% in objdiff, or a C file is not accounted for (decomp/reports/progress_report.txt)'
"$PY" -I "$g/tools/check_c_callsites.py" || die 'a // c-callsite name of symbols/symbols.txt is not confirmed by the matched C call sites (decomp/reports/c_callsites_report.txt)'
"$PY" -I "$g/tools/gen_dtk_relocs.py" --check || die 'the built main.elf still has relocation misses / false positives (decomp/reports/relocs_report.txt)'
"$PY" -I "$g/tools/shift_test.py" || die 'static shift test failed (decomp/reports/shift_report.txt)'
echo "batch 9 done: main.dol matches retail, relocations checked, shifted build checked; reports in decomp/reports/"
