#!/bin/bash
# the third permuter queue (Nov 3): the two functions that hold back the file run 0x80080A94-0x80081430 first
# (double time), then the register-only near misses of the draftscan triage and the hand drafts left at 85-98%
S="$(dirname "$0")"
G=/d/emulators/gameTesting
cd "$G"
M=${MINUTES:-10}
run() {
  echo "== $1 $(date +%T)"
  python -I tools/sched/permuter_setup.py "$1" "$S/$1.c" --run 8 --minutes "$2" 2>&1 | tr '\r' '\n' | grep -E "set up|base score|best:|rror" | tail -4
}
for f in fn_80080C7C fn_80080E24; do
  run "$f" $((M * 2))
done
for f in fn_8001E488 fn_8001EA48 fn_800699E0 fn_8000AFBC cr_Vec3_RotateY fn_800389C8 fn_800192C4 fn_800506DC fn_80075950 fn_80023CBC fn_8003B1BC fn_80070130 fn_80066EEC fn_80067778 cr_Archive_GetEntrySize cr_SEQ_TITLE_Leave cr_SEQ_STAGE_SELECTOR_Leave cr_Text_GetBodyName cr_Text_GetPartDescription fn_800677C4 cr_Save_ApplyUnlocks fn_80044A90 cr_CAMMODE_SNR_AUTO_Y_ANG_Frame fn_8003FB00 cr_Vec3_RotateAboutAxis fn_8006975C fn_8004FF64; do
  run "$f" "$M"
done
# fn_8006EEF8 (74%) last: its queue 2 run did not start (a one-off failure, the run works by hand)
run fn_8006EEF8 $((M * 2))
echo "== queue3 done $(date +%T)"
