# Batch 3: map the game's own code (callers, callees, strings, asserts, subsystem hints).
# Behaviour notes only, nothing is applied. Output: docs\game_map.md, symbols\game_map.tsv
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
python -I "$g\tools\game_map.py"
if ($LASTEXITCODE) { throw 'game_map.py failed' }
