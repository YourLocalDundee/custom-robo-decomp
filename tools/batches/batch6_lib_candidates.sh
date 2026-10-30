#!/usr/bin/env bash
# Batch 6: name the remaining library functions (naming policy rules R3 layout / R4 asserts / R5 call graph / R6 string).
# batch6_lib_candidates.ps1 for Linux / macOS: same rounds, same merge, same report, byte for byte.
# Runs the three matchers (tools/match_lib_sdk.py 0x80131000-0x8015F000, match_lib_rt.py 0x8015F000-0x8016D408,
# match_lib_hsd.py 0x8016D408-), merges what they find into symbols.txt tagged "lib-candidate", and repeats until no new
# name appears (max 10 rounds), because every matcher reads symbols.txt: names found by one are verified neighbours for the others.
# Then runs verify_symbols.py and stops if any merged name fails. Details: symbols/lib_candidates_report.txt.
# Rerunnable: strips the previously merged lines first. Run right after batch 1 (which rewrites the layout-proof lines).
# A candidate with "replaces:OLD" in its why: removes the layout-proof line OLD at that address (a run of same-size
# functions that the size-only layout match had shifted by one). Does not rebuild Ghidra (run_all.sh does).
# The matchers read the Ghidra export decompiled/xrefs/*.tsv for structure only - calls, string refs, computed jumps by address,
# never a name Ghidra applied (tools/export_struct.py) - and main.dol (asserts from the instruction bytes, tools/dol_asserts.py),
# never the decompiled C; our names come from the symbol files. When the
# export does not exist yet (fresh setup) the matchers are skipped and symbols/candidates_*.txt are merged as they are
# (run_all.sh empties them first, then reruns this batch after every Ghidra rebuild until the names stop changing).
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
sym="$g/symbols/symbols.txt"
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
acc="$tmp/lib_candidates_accepted.txt"
rep="$tmp/lib_candidates_replaced.txt"
part="$tmp/lib_candidates_part.txt"
parts="$tmp/lib_candidates_parts.txt"   # every round's part file, appended
: > "$parts"

grepq -v '// lib-candidate ' "$sym" | crlf > "$sym.tmp" && mv -f "$sym.tmp" "$sym"

have_index=1
for f in functions.tsv refs.tsv strings.tsv; do [ -f "$g/decompiled/xrefs/$f" ] || have_index=0; done
[ "$have_index" = 1 ] || echo "batch6: Ghidra export (decompiled/xrefs) missing - matchers skipped, merging symbols/candidates_*.txt as they are"
matchers=(sdk rt hsd)
for m in "${matchers[@]}"; do : > "$tmp/accum_$m.txt"; done
rounds=0
for round in 1 2 3 4 5 6 7 8 9 10; do
    counts=""
    if [ "$have_index" = 1 ]; then
        for m in "${matchers[@]}"; do
            "$PY" -I "$g/tools/match_lib_$m.py" > /dev/null || die "match_lib_$m.py failed"
            n=$(grepq -E '^[^[:space:]]+ = \.text:' "$g/symbols/candidates_$m.txt" | nocr | tee -a "$tmp/accum_$m.txt" | wc -l | tr -d ' ')
            counts="$counts${counts:+ }$m=$n"
        done
    fi
    r=$("$PY" -I "$g/tools/merge_lib_candidates.py" "$acc" "$rep" "$part") || die 'merge_lib_candidates.py failed'
    # shellcheck disable=SC2206
    nums=($r)
    # a candidate that replaces a layout-proof name: that layout-proof line (same name and address) leaves symbols.txt
    if [ -s "$rep" ]; then
        "$PY" -I - "$sym" "$rep" <<'EOF'
import re, sys
sym, rep = sys.argv[1:3]
replaced = set()
for l in open(rep, encoding='utf-8'):
    p = l.rstrip('\r\n').split(' ')
    replaced.add(p[0] + ' ' + p[1].upper().replace('0X', '0x'))
out = []
for l in open(sym, encoding='utf-8'):
    l = l.rstrip('\r\n')
    m = re.match(r'^(\S+) = \.text:0x([0-9A-Fa-f]{8});', l)
    if m and '// layout-proof ' in l and (m.group(1) + ' 0x' + m.group(2).upper()) in replaced:
        continue
    out.append(l)
open(sym, 'w', encoding='utf-8', newline='\r\n').write(''.join(x + '\n' for x in out))
EOF
    fi
    nocr < "$acc" | crlf >> "$sym"
    nocr < "$part" >> "$parts"
    rounds=$round
    echo "batch6 round $round: candidate lines $counts read=${nums[0]} accepted=${nums[1]} rejected=${nums[2]} layout-proof replaced=${nums[3]}"
    [ "${nums[1]}" != 0 ] || break
    [ "$have_index" = 1 ] || break   # nothing changes between rounds without matchers
done

# the matchers overwrite their file every round: leave the union of all rounds in symbols/candidates_<matcher>.txt
if [ "$have_index" = 1 ]; then
    for m in "${matchers[@]}"; do
        "$PY" -I - "$tmp/accum_$m.txt" "$g/symbols/candidates_$m.txt" "$m" "$g/tools" <<'EOF'
import re, sys
src, dst, m, tools = sys.argv[1:5]
sys.path.insert(0, tools)
import ps_sort
# sorted by address, one line per address, exactly the line Sort-Object { address } -Unique keeps (tools/ps_sort.py)
rows = [l.rstrip('\r\n') for l in open(src, encoding='utf-8')]
addr = lambda l: int(re.search(r'\.text:0x([0-9A-Fa-f]{8})', l).group(1), 16)
out = ['// all rounds of batch6_lib_candidates.ps1 (tools\\match_lib_%s.py)' % m] + ps_sort.unique_by(rows, addr)
open(dst, 'w', encoding='utf-8', newline='\r\n').write(''.join(x + '\n' for x in out))
EOF
    done
fi

# report: unique REJECT / REPLACED lines, every ACCEPT line
accept_l=$(grepq '^ACCEPT ' "$parts" | ps_sort -u)
reject_l=$(grepq '^REJECT ' "$parts" | ps_sort -u)
repl_l=$(grepq '^REPLACED ' "$parts" | ps_sort -u)
count() { if [ -z "$1" ]; then echo 0; else printf '%s\n' "$1" | wc -l | tr -d ' '; fi; }
by_rule=$(printf '%s\n' "$accept_l" | awk 'NF { n[$4]++ } END { for (r in n) print r ":" n[r] }' | ps_sort | paste -sd ',' - | sed 's/,/, /g')
head_l="lib candidates: $(count "$accept_l") accepted ($by_rule), $(count "$reject_l") rejected, $(count "$repl_l") layout-proof names replaced, $rounds round(s)"
{
    printf '%s\n' "$head_l"
    [ -z "$reject_l" ] || printf '%s\n' "$reject_l"
    [ -z "$repl_l" ] || printf '%s\n' "$repl_l"
    [ -z "$accept_l" ] || printf '%s\n' "$accept_l"
} | crlf > "$g/symbols/lib_candidates_report.txt"

refs=()
while IFS= read -r r; do refs+=("$r"); done < <(ref_symbol_files)
"$PY" -I "$g/tools/verify_symbols.py" "$sym" "$g/tools/dtk/proj/symbols.txt" "${refs[@]}" | crlf > "$g/symbols/verify_report.txt"
grepq '// lib-candidate ' "$sym" | nocr | cut -d' ' -f1 > "$tmp/merged_names.txt"
bad=$("$PY" -I - "$g/symbols/verify_report.txt" "$tmp/merged_names.txt" <<'EOF'
import re, sys
report = open(sys.argv[1], encoding='utf-8').read()
bad = [n for n in (l.strip() for l in open(sys.argv[2], encoding='utf-8')) if n and
       re.search(r'^(MISMATCH|UNSOURCED|NO-SIZE)\s+\S+\s+' + re.escape(n) + r'\s', report, re.M)]
print(', '.join(bad))
EOF
)
echo "batch6: $head_l; verify: $(nocr < "$g/symbols/verify_report.txt" | tail -n 1)"
[ -z "$bad" ] || die "lib-candidate names failing verification ($(printf '%s' "$bad" | awk -F', ' '{ print NF }')): $bad"
