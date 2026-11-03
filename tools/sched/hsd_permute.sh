#!/bin/bash
# step 110: the permuter on the HSD near misses, one at a time (tools/sched/permuter_setup.py --object)
cd "$(dirname "$0")"
for x in "HSD_TExpSetReg texp" "mkBillBoardMtx displayfunc" "mkHBillBoardMtx displayfunc" "HSD_LObjSetupInit lobj" "PObjLoad pobj" "TExp2TevDesc texp" "HSD_JObjDispAll jobj" "drawShapeAnim pobj"; do
  set -- $x
  echo "== $1 ($2) $(date +%T)"
  python -I permuter_setup.py "$1" "../../decomp/src/sysdolphin/baselib/$2.c" --object "sysdolphin/baselib/$2" --run 3 --minutes "${MINUTES:-20}" 2>&1 | tr '\r' '\n' | grep -E "set up|base score|best:" | tail -3
done
echo "== done $(date +%T)"
