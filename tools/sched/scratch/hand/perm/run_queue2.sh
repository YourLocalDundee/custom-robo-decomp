#!/bin/bash
# the second permuter queue of Nov 3: the new hand drafts, then the HSD near misses
S="$(dirname "$0")"
G=/d/emulators/gameTesting
cd "$G"
M=${MINUTES:-12}
for n in fn_80075258:$M fn_800057B8:$M fn_8000860C:$M fn_80038CF8:$M fn_8001838C:$M fn_8000E3B8:$M fn_8006EEF8:$((M * 2)); do
  f=${n%%:*}; m=${n##*:}
  echo "== $f $(date +%T)"
  python -I tools/sched/permuter_setup.py "$f" "$S/$f.c" --run 8 --minutes "$m" 2>&1 | tr '\r' '\n' | grep -E "set up|base score|best:|rror" | tail -4
done
cd tools/sched
LIST="HSD_ShadowSetViewingRect:shadow mkBillBoardMtx:displayfunc HSD_LObjSetupInit:lobj PObjLoad:pobj TExp2TevDesc:texp HSD_JObjDispAll:jobj drawShapeAnim:pobj HSD_TExpSetReg:texp" MINUTES=$M bash hsd_permute.sh
echo "== queue done $(date +%T)"
