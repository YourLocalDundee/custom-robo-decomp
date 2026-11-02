#Requires -Version 7
# The ONE script that runs everything, in order, stopping at the first failure (user rule: one sequenced batch):
#   Ghidra rebuild -> batches 1 + 6 + 7 as a CHECK of the committed names -> 5 types -> 2 dtk cross-check -> 3 game map
#   -> 4 search index -> 8 name cross-check -> 9 decomp build (retail SHA-1, relocations, static shift test)
#   -> 10 compiler probe (~5 min) -> 11 shifted build booted in Dolphin (~1 min; do not type while Dolphin is open)
#   -> 12 m2c drafts of the game code, compiled and compared, the linkable 100% ones promoted into decomp/src/game
#      (~26 min; batch 9 again when that folder changed)
#   -> checksums.ps1 -Compare (every output byte-identical to docs/expected_checksums.txt), then a summary.
# docs/REPRODUCE.md "Verify this checkout". tools/from_scratch.ps1 wraps it: setup -> this -> archives / models / mods.
# Usage:  pwsh tools/batches/run_all.ps1 [-SkipRebuild] [-ApplyNames [-MaxPasses 5]]
#
# The committed name files (symbols\*.txt) are the truth (plan.md step 110). By default the run rebuilds Ghidra once
# from them, runs the name batches (1 = layout proof, 6 = lib candidates, 7 = the game's own names) against that
# export, writes what they would change to symbols\name_proposals.txt and puts the committed files back
# (tools\name_check.py): a verification run never renames anything.
# -ApplyNames: take the batches' names. Every layout-proof / lib-candidate line is stripped, the batches run against
# the existing export, then Ghidra rebuild <-> batches until no name file changes (fails after -MaxPasses rebuilds),
# and the run stops if a name the C sources in decomp\src use was dropped or moved (tools\name_check.py guard).
# -SkipRebuild: no Ghidra rebuild (the existing export is used).
param([switch]$SkipRebuild, [switch]$ApplyNames, [int]$MaxPasses = 5)
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

# 0. the reference checkouts are the setup's: their sources are read as text, other files give other names (step 68b)
python -I "$g/tools/check_refs.py"
if ($LASTEXITCODE) { throw 'reference checkouts differ from tools/setup.ps1 (see above)' }
$snap = "$g/build/name_snapshot"
python -I "$g/tools/name_check.py" snapshot $snap
if ($LASTEXITCODE) { throw 'name_check.py snapshot failed' }
$passes = 0
if ($ApplyNames) {
    # 1. the committed starting point without the derived lines
    Set-Content -LiteralPath $sym -Value (Get-Content -LiteralPath $sym | Where-Object { $_ -notmatch '// (layout-proof|lib-candidate) ' })
    foreach ($m in 'sdk', 'rt', 'hsd') {
        Set-Content -LiteralPath "$g/symbols/candidates_$m.txt" -Value "// all rounds of batch6_lib_candidates.ps1 (tools\match_lib_$m.py)"
    }
    # 2. names against the existing export
    Names $(if (Test-Path -LiteralPath "$g/decompiled/xrefs/refs.tsv") { 'previous export' } else { 'no export yet' })
    # 3. rebuild <-> names to a fixpoint
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
    python -I "$g/tools/name_check.py" guard $snap
    if ($LASTEXITCODE) { throw 'the name batches dropped names the C sources use (see above): fix the batches or keep those lines by hand' }
} else {
    if (-not $SkipRebuild) {
        $passes = 1
        "== Ghidra rebuild (committed names)"
        & "$g/tools/rebuild_decomp.ps1"
    }
    Names 'check against the export'
    python -I "$g/tools/name_check.py" report $snap "$g/symbols/name_proposals.txt"
    if ($LASTEXITCODE) { throw 'name_check.py report failed' }
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
$(if ($ApplyNames) { "fixpoint: names unchanged after $passes Ghidra rebuild(s)" } else { "names: committed names checked, proposals in symbols\name_proposals.txt ($passes Ghidra rebuild(s))" })
"functions in C: $funcs, still FUN_: $generic, named: $($funcs - $generic)"
"reports: symbols\verify_report.txt, symbols\crosscheck_report.txt, docs\game_map.md"
"decomp build: main.dol == retail SHA-1 (decomp\reports\build_report.txt, splits_report.txt, symbols_report.txt)"
"compiler probe: tools\compiler_probe\out\summary.txt (docs\compiler.md); shifted boot: decomp\build\shift\boot\boot_result.txt"
"checksums: all outputs match docs\expected_checksums.txt"
"search: python -I tools\find.py text|func|callers|callees|refs|str|hint <arg>   (index: decompiled\index.db)"
