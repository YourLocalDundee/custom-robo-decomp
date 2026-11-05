#!/bin/bash
# the fifth permuter queue (Nov 4, 2026): the register-allocation pool of structscore.py (unmatched drafts whose
# instructions are >= 90% right with the registers masked), biggest first, plus the two big 85% ones. Written to run
# on Linux (cr-linux: about 2.4x the Windows tries per minute, docs/INSTALL.md "Speed") or Windows: the repo is found
# from this script's place. Source per function: perm/q5/FUNC.c when it exists (a hand base), else the draft
# decomp/drafts/c/FUNC.c (both gitignored: decompiled output). Log: perm/queue5.log (run it with > queue5.log).
P="$(cd "$(dirname "$0")" && pwd)"
G="$(cd "$P/../../../../.." && pwd)"
cd "$G"
M=${MINUTES:-20}
PY=python3; command -v python3 >/dev/null || PY=python
run() {
  echo "== $1 $(date +%T)"
  src="$P/q5/$1.c"; [ -f "$src" ] || src="$G/decomp/drafts/c/$1.c"
  $PY -I tools/sched/permuter_setup.py "$1" "$src" --run 8 --minutes "$2" 2>&1 | tr '\r' '\n' | grep -E "pruned|set up|base score|best:|rror" | tail -5
}
for f in fn_8003C4F0 fn_80039FAC fn_80045FEC fn_800381D0 fn_800177F0 fn_801166E4 fn_80101284 fn_800B1B2C fn_80090298 fn_801314D4 fn_801019D8 fn_8008538C fn_80089550 fn_800B7D8C fn_8000E64C cr_SEQ_BATTLE_FrameA; do
  run "$f" "$M"
done
echo "== queue5 done $(date +%T)"
