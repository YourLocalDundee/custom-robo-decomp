#!/bin/bash
# the sixth permuter queue (Nov 5, 2026): the next of structscore.py's register-allocation pool (noreg >= 85%, not
# run in queues 2-5), biggest first. Written to run
# on Linux (cr-linux: about 2.4x the Windows tries per minute, docs/INSTALL.md "Speed") or Windows: the repo is found
# from this script's place. Source per function: perm/q6/FUNC.c when it exists (a hand base), else the draft
# decomp/drafts/c/FUNC.c (both gitignored: decompiled output). Log: perm/queue6.log (run it with > queue5.log).
P="$(cd "$(dirname "$0")" && pwd)"
G="$(cd "$P/../../../../.." && pwd)"
cd "$G"
M=${MINUTES:-20}
PY=python3; command -v python3 >/dev/null || PY=python
run() {
  echo "== $1 $(date +%T)"
  src="$P/q6/$1.c"; [ -f "$src" ] || src="$G/decomp/drafts/c/$1.c"
  $PY -I tools/sched/permuter_setup.py "$1" "$src" --run 8 --minutes "$2" 2>&1 | tr '\r' '\n' | grep -E "pruned|set up|base score|best:|rror" | tail -5
}
for f in fn_8004DC78 fn_800F7BF8 cr_SEQ_SCENARIO_FrameB fn_80123584 fn_80009018 cr_ModelEntry_LoadAndBuild fn_80105288 cr_Game_Update fn_80119F34 cr_CHAR_SET_WMOT_Y cr_CHAR_SET_WMOT fn_80108290 fn_80089140 cr_CHAR_MOVE_REL fn_8001C5AC fn_800FE954 cr_Ending_LoadPartPictures; do
  run "$f" "$M"
done
echo "== queue6 done $(date +%T)"
