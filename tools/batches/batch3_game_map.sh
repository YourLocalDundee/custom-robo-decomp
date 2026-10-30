#!/usr/bin/env bash
# Batch 3: map the game's own code (callers, callees, strings, asserts, subsystem hints).
# Behaviour notes only, nothing is applied. Output: docs/game_map.md, symbols/game_map.tsv
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
"$PY" -I "$g/tools/game_map.py" || die 'game_map.py failed'
