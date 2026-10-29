#Requires -Version 7
# Run every batch in order: names -> Ghidra rebuild with types (~3 min) -> names again, to a fixpoint -> type checks
# -> cross-check -> game map -> search index -> name cross-check, then a summary.
# Usage:  pwsh D:\emulators\gameTesting\tools\batches\run_all.ps1 [-SkipRebuild] [-MaxPasses 5]
#
# Reproducible from any starting point. The name batches (1 = layout proof, then 6 = lib candidates, which builds on
# batch 1) take our names only from the committed symbol files and the Ghidra export only for structure (function
# starts, calls, strings, assert calls, by address: tools\export_struct.py), and that structure does not depend on
# the names applied. So the run
#   1. starts from the committed names: strips every layout-proof / lib-candidate line from symbols\symbols.txt and
#      empties symbols\candidates_{sdk,rt,hsd}.txt (candidates_audit.txt is an input and stays);
#   2. runs batches 1 + 6 against whatever export exists (none on a fresh setup: forced layout pairs only);
#   3. rebuilds Ghidra and reruns batches 1 + 6 against the fresh export; if any name file changed, rebuilds again.
# It stops when a rebuild leaves every name unchanged (the decompiled C is then exactly the C of the final names):
# 1 rebuild when the previous export exists, 2 on a fresh setup. Fails after -MaxPasses rebuilds.
# -SkipRebuild: step 2 only, no Ghidra.
param([switch]$SkipRebuild, [int]$MaxPasses = 5)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$t0 = Get-Date
$sym = "$g/symbols/symbols.txt"
# everything batches 1 + 6 write that the next pass or the rebuild reads (ParseHeaders.java reads verify_report.txt)
$nameFiles = 'symbols.txt', 'layout_candidates.txt', 'candidates_sdk.txt', 'candidates_rt.txt', 'candidates_hsd.txt',
    'verify_report.txt', 'lib_candidates_report.txt' | ForEach-Object { "$g/symbols/$_" }

function Snapshot {
    ($nameFiles | ForEach-Object { if (Test-Path -LiteralPath $_) { (Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash } else { 'missing' } }) -join ' '
}
function Names($label) {
    "== names ($label)"
    & "$PSScriptRoot/batch1_names.ps1"
    & "$PSScriptRoot/batch6_lib_candidates.ps1"
    python -I "$g/tools/lint_descriptive.py"
    if ($LASTEXITCODE) { throw 'symbols\descriptive.txt has problems (see above)' }
}

# 1. the committed starting point
Set-Content -LiteralPath $sym -Value (Get-Content -LiteralPath $sym | Where-Object { $_ -notmatch '// (layout-proof|lib-candidate) ' })
foreach ($m in 'sdk', 'rt', 'hsd') {
    Set-Content -LiteralPath "$g/symbols/candidates_$m.txt" -Value "// all rounds of batch6_lib_candidates.ps1 (tools\match_lib_$m.py)"
}
# 2. names against the existing export
Names $(if (Test-Path -LiteralPath "$g/decompiled/xrefs/refs.tsv") { 'previous export' } else { 'no export yet' })
# 3. rebuild <-> names to a fixpoint
$passes = 0
if (-not $SkipRebuild) {
    while ($true) {
        if ($passes -ge $MaxPasses) { throw "names did not converge in $MaxPasses Ghidra rebuilds (symbols\symbols.txt still changing)" }
        $before = Snapshot
        $passes++
        "== Ghidra rebuild, pass $passes"
        & "$g/tools/rebuild_decomp.ps1"
        Names "against rebuild $passes"
        if ((Snapshot) -eq $before) { break }
        "== rebuild ${passes}: names changed, rebuilding"
    }
    "== names converged: unchanged after Ghidra rebuild $passes ($passes rebuild(s))"
}
& "$PSScriptRoot/batch5_types.ps1"
& "$PSScriptRoot/batch2_crosscheck.ps1"
& "$PSScriptRoot/batch3_game_map.ps1"
& "$PSScriptRoot/batch4_index.ps1"
& "$PSScriptRoot/batch8_crosscheck.ps1"   # needs the fresh index.db from batch 4

$c = "$g/decompiled/main_dol.c"
$funcs = (Select-String -LiteralPath $c -Pattern '^// [0-9a-f]{8}  ').Count
$generic = (Select-String -LiteralPath $c -Pattern '^// [0-9a-f]{8}  FUN_').Count
"== summary ($([int]((Get-Date) - $t0).TotalSeconds) s)"
"fixpoint: names unchanged after $passes Ghidra rebuild(s)"
"functions in C: $funcs, still FUN_: $generic, named: $($funcs - $generic)"
"reports: symbols\verify_report.txt, symbols\crosscheck_report.txt, docs\game_map.md"
"search: python -I tools\find.py text|func|callers|callees|refs|str|hint <arg>   (index: decompiled\index.db)"
