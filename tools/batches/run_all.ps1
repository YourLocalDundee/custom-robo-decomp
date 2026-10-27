# Run every batch in order: names -> Ghidra rebuild with types (~3 min) -> type checks -> cross-check -> game map
# -> search index, then a summary.
# Usage:  pwsh D:\emulators\gameTesting\tools\batches\run_all.ps1 [-SkipRebuild]
param([switch]$SkipRebuild)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$t0 = Get-Date
& "$PSScriptRoot\batch1_names.ps1"
python -I "$g\tools\lint_descriptive.py"
if ($LASTEXITCODE) { throw 'symbols\descriptive.txt has problems (see above)' }
if (-not $SkipRebuild) { & "$g\tools\rebuild_decomp.ps1" }
& "$PSScriptRoot\batch5_types.ps1"
& "$PSScriptRoot\batch2_crosscheck.ps1"
& "$PSScriptRoot\batch3_game_map.ps1"
& "$PSScriptRoot\batch4_index.ps1"

$c = "$g\decompiled\main_dol.c"
$funcs = (Select-String -LiteralPath $c -Pattern '^// [0-9a-f]{8}  ').Count
$generic = (Select-String -LiteralPath $c -Pattern '^// [0-9a-f]{8}  FUN_').Count
"== summary ($([int]((Get-Date) - $t0).TotalSeconds) s)"
"functions in C: $funcs, still FUN_: $generic, named: $($funcs - $generic)"
"reports: symbols\verify_report.txt, symbols\crosscheck_report.txt, docs\game_map.md"
"search: python -I tools\find.py text|func|callers|callees|refs|str|hint <arg>   (index: decompiled\index.db)"
