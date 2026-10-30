#!/usr/bin/env bash
# Batch 5: check the types the rebuild applied (ParseHeaders.java -> symbols/types_report.txt).
# Stops if any header size / field offset is wrong, any prototype or global failed, or any HSD struct size
# differs from the size Custom Robo registers for that class (tools/check_class_sizes.py).
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
report="$g/symbols/types_report.txt"
summary=$(head -n 1 "$report" | nocr)
echo "types: $summary"
if ! printf '%s' "$summary" | grep -E -q 'ASSERT_SIZE ok [0-9]+, wrong 0, not parsed 0; field offsets ok [0-9]+, wrong 0 .*failed 0; globals typed [0-9]+, failed 0'; then
    die "types check failed, see $report"
fi
"$PY" -I "$g/tools/check_class_sizes.py" "$report" "$g/decompiled/main_dol.c" | tail -n 1 ||
    die 'HSD struct sizes differ from the game (run tools/check_class_sizes.py for details)'
