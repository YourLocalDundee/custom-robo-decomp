#Requires -Version 7
# The ONE script that runs everything, in order, stopping at the first failure (user rule: one sequenced batch):
#   batches 1 + 6 + 7 names <-> Ghidra rebuild to a fixpoint -> 5 types -> 2 dtk cross-check -> 3 game map
#   -> 4 search index -> 8 name cross-check -> 9 decomp build (retail SHA-1, relocations, static shift test)
#   -> 10 compiler probe (~5 min) -> 11 shifted build booted in Dolphin (~1 min; do not type while Dolphin is open)
#   -> checksums.ps1 -Compare (every output byte-identical to docs/expected_checksums.txt), then a summary.
# tools/from_scratch.ps1 wraps it: setup -> this -> archives / models / mod build.
# Usage:  pwsh tools/batches/run_all.ps1 [-SkipRebuild] [-MaxPasses 5]
#
# Reproducible from any starting point. The name batches (1 = layout proof, then 6 = lib candidates, which builds on
# batch 1, then 7 = the game's own names, from main.dol only) take our names only from the committed symbol files
# and the Ghidra export only for structure (function starts, calls, strings, assert calls, by address:
# tools\export_struct.py), and that structure does not depend on the names applied. So the run
#   1. starts from the committed names: strips every layout-proof / lib-candidate line from symbols\symbols.txt and
#      empties symbols\candidates_{sdk,rt,hsd}.txt (candidates_audit.txt is an input and stays);
#   2. runs batches 1 + 6 + 7 against whatever export exists (none on a fresh setup: forced layout pairs only;
#      batch 7 needs no export) - batch 7 replaces its own "// game-real" lines in symbols\descriptive.txt;
#   3. rebuilds Ghidra and reruns batches 1 + 6 + 7 against the fresh export; if any name file changed, rebuilds again.
# It stops when a rebuild leaves every name unchanged (the decompiled C is then exactly the C of the final names):
# 1 rebuild when the previous export exists, 2 on a fresh setup. Fails after -MaxPasses rebuilds.
# -SkipRebuild: step 2 only, no Ghidra.
param([switch]$SkipRebuild, [int]$MaxPasses = 5)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$t0 = Get-Date
$sym = "$g/symbols/symbols.txt"
# everything batches 1 + 6 + 7 write that the next pass or the rebuild reads (ParseHeaders.java reads verify_report.txt,
# ApplySymbols.java descriptive.txt)
$nameFiles = 'symbols.txt', 'layout_candidates.txt', 'candidates_sdk.txt', 'candidates_rt.txt', 'candidates_hsd.txt',
    'verify_report.txt', 'lib_candidates_report.txt', 'descriptive.txt', 'game_real.txt', 'game_unnamed_real.txt' |
    ForEach-Object { "$g/symbols/$_" }

function Snapshot {
    ($nameFiles | ForEach-Object { if (Test-Path -LiteralPath $_) { (Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash } else { 'missing' } }) -join ' '
}
function Names($label) {
    "== names ($label)"
    & "$PSScriptRoot/batch1_names.ps1"
    & "$PSScriptRoot/batch6_lib_candidates.ps1"
    & "$PSScriptRoot/batch7_game_names.ps1"
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
# Phase 4 build: outside the name fixpoint (reads the final name files, never Ghidra). Stops the run unless the built
# main.dol is the retail one.
& "$PSScriptRoot/batch9_decomp_build.ps1"
& "$PSScriptRoot/batch10_compiler_probe.ps1"
if ($IsWindows) { "== batch 11: Dolphin opens twice for about a minute - do not type or click (a key mapped to Start skews the check)" }
& "$PSScriptRoot/batch11_shift_test.ps1"
if ($LASTEXITCODE) { throw 'batch 11 (shifted build boot) failed: decomp/build/shift/boot/boot_result.txt' }
& "$PSScriptRoot/batch12_drafts.ps1"   # m2c drafts of the game code, compiled and compared (decomp/drafts/ stays local); the linkable 100% ones -> decomp/src/game (tools/promote_drafts.py), batch 9 again if that changed
& "$g/tools/checksums.ps1" -Compare
if ($LASTEXITCODE) { throw "checksums differ from docs/expected_checksums.txt (docs/REPRODUCE.md, 'If a checksum differs')" }

$c = "$g/decompiled/main_dol.c"
$funcs = (Select-String -LiteralPath $c -Pattern '^// [0-9a-f]{8}  ').Count
$generic = (Select-String -LiteralPath $c -Pattern '^// [0-9a-f]{8}  FUN_').Count
"== summary ($([int]((Get-Date) - $t0).TotalSeconds) s)"
"fixpoint: names unchanged after $passes Ghidra rebuild(s)"
"functions in C: $funcs, still FUN_: $generic, named: $($funcs - $generic)"
"reports: symbols\verify_report.txt, symbols\crosscheck_report.txt, docs\game_map.md"
"decomp build: main.dol == retail SHA-1 (decomp\reports\build_report.txt, splits_report.txt, symbols_report.txt)"
"compiler probe: tools\compiler_probe\out\summary.txt (docs\compiler.md); shifted boot: decomp\build\shift\boot\boot_result.txt"
"checksums: all outputs match docs\expected_checksums.txt"
"search: python -I tools\find.py text|func|callers|callees|refs|str|hint <arg>   (index: decompiled\index.db)"
