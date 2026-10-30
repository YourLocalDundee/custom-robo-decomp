#!/usr/bin/env bash
# Batch 1: library names by layout proof (naming policy rule 3), then verify (batch1_names.ps1 for Linux / macOS).
# Rerunnable: strips the previous layout-proof and lib-candidate lines from symbols.txt (batch 6 re-adds the latter:
# run it next; batch 6 output is never input here), rematches to a fixpoint, re-verifies.
# Stops if any layout-proof name fails verify_symbols.py. Does not rebuild Ghidra (run_all.sh does).
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
sym="$g/symbols/symbols.txt"

grepq -v -E '// (layout-proof|lib-candidate) ' "$sym" | crlf > "$sym.tmp" && mv -f "$sym.tmp" "$sym"
"$PY" -I "$g/tools/layout_match.py" || die 'layout_match.py failed'
grepq -v '^//' "$g/symbols/layout_candidates.txt" | sed 's| ref:| scope:global // layout-proof ref:|g' | crlf >> "$sym"

# tools/refs/<game>/config/<version>/symbols.txt, in the Windows order (tools/common.sh)
refs=()
while IFS= read -r r; do refs+=("$r"); done < <(ref_symbol_files)
"$PY" -I "$g/tools/verify_symbols.py" "$sym" "$g/tools/dtk/proj/symbols.txt" "${refs[@]}" | crlf > "$g/symbols/verify_report.txt"
layout=$(grep -c '// layout-proof ' "$sym" || true)
# layout-proof names failing verification
names=$(mktemp)
trap 'rm -f "$names"' EXIT
grepq -v '^//' "$g/symbols/layout_candidates.txt" | nocr | cut -d' ' -f1 > "$names"
bad=$("$PY" -I - "$g/symbols/verify_report.txt" "$names" <<'EOF'
import re, sys
report = open(sys.argv[1], encoding='utf-8').read()
bad = [n for n in (l.strip() for l in open(sys.argv[2], encoding='utf-8')) if n and
       re.search(r'^(MISMATCH|UNSOURCED|NO-SIZE)\s+\S+\s+' + re.escape(n) + r'\s', report, re.M)]
print(', '.join(bad))
EOF
)
echo "batch1: $layout layout-proof names in symbols.txt; verify: $(nocr < "$g/symbols/verify_report.txt" | tail -n 1)"
[ -z "$bad" ] || die "layout-proof names failing verification: $bad"
