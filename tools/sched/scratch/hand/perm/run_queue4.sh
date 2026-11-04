#!/bin/bash
# the fourth permuter queue (Nov 4, 2026): the near misses left after the hand pass of queue 3's results
# (docs/drafts.md, "Nov 4, 2026: the queue 3 near misses by hand"); bases in perm/q4/ (the improved hand forms
# for fn_80067778 89% and fn_80044A90 96%, the queue 3 bases otherwise). fn_80080C7C (the last function of the file
# 0x80080A94-0x800811E8) first, with double time.
S="$(dirname "$0")/q4"
G=/d/emulators/gameTesting
cd "$G"
M=${MINUTES:-20}
run() {
  echo "== $1 $(date +%T)"
  python -I tools/sched/permuter_setup.py "$1" "$S/$1.c" --run 8 --minutes "$2" 2>&1 | tr '\r' '\n' | grep -E "set up|base score|best:|rror" | tail -4
}
run fn_80080C7C $((M * 2))
for f in fn_80044A90 fn_80067778 cr_Text_GetBodyName cr_Text_GetPartDescription fn_8001EA48 fn_8003B1BC fn_800389C8 fn_800192C4 fn_80023CBC; do
  run "$f" "$M"
done
echo "== queue4 done $(date +%T)"
