#Requires -Version 7
# Batch 12: draft and check the game code (plan.md Phase 4 Milestone 5, docs/drafts.md).
# For each of the ~1,750 game functions (decomp/config/GXCE01/splits.txt: 0x800055E0 up to the first library object; dtk's automatic objects there and the game/ objects linked from C; i.e. not
# in a library object): m2c (tools/m2c, pinned in tools/pins.txt) writes a C draft with our names and our types
# (HSD headers + symbols/cr_types.h) as context, the draft is compiled with the game compiler and flags (GC/2.6,
# configure.py CR_LIBS "game"), and the compiled function is compared with the original one in dtk's split object
# (relocation fields masked). About 5 minutes on 8 cores, parallel (multiprocessing), deterministic.
#   1. needs batch 9 (decomp/build/GXCE01/asm + obj, decomp/build/compilers) and tools/m2c (tools/setup.ps1); read only
#   2. python -I tools/draft_batch.py -> decomp/drafts/ (GITIGNORED: drafts, asm, objects, logs, draft_report.txt)
#      and decomp/reports/draft_summary.txt (counts only; committed)
#   3. python -I tools/promote_drafts.py: every 100% draft that can be linked as it is (no data of its own, no padding,
#      every relocation to the same address, compiles the same behind the committed decomp/include/game/cr_game.h)
#      -> decomp/src/game/<function>.c (COMMITTED: matched C, one function per object, plan.md step 58) and
#      decomp/reports/promote_report.txt. Rerunnable; nothing changes when the drafts did not. When decomp/src/game
#      changed, batch 9 runs again here, so the build (retail SHA-1, objdiff) and its reports include the new files.
#      (Not with -Sample: a sample's report lists only some functions.)
# Never commit anything in decomp/drafts/. Not in tools/checksums.ps1 (see docs/drafts.md "Determinism").
# Usage:  pwsh tools/batches/batch12_drafts.ps1 [-Sample N] [-Jobs N]
param([int]$Sample = 0, [int]$Jobs = [Environment]::ProcessorCount)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
foreach ($p in 'tools/m2c/m2c/main.py', 'decomp/build/GXCE01/config.json', 'decomp/build/GXCE01/asm', 'decomp/build/GXCE01/obj', 'decomp/build/compilers/GC/2.6/mwcceppc.exe', 'tools/melee/src/sysdolphin/baselib') {
    if (-not (Test-Path -LiteralPath "$g/$p")) { throw "$p missing: run tools/setup.ps1 (m2c, Melee headers) and batch 9 first" }
}
$a = @('--jobs', $Jobs)
if ($Sample) { $a += @('--sample', $Sample) }
python -I "$g/tools/draft_batch.py" @a
if ($LASTEXITCODE) { throw 'draft_batch.py failed' }
if (-not $Sample) {
    $p = python -I "$g/tools/promote_drafts.py"
    if ($LASTEXITCODE) { throw 'promote_drafts.py failed' }
    $p
    if ($p[-1] -like 'changed*') {
        "== decomp/src/game changed: batch 9 again (the build links the promoted files)"
        & "$PSScriptRoot/batch9_decomp_build.ps1"
    }
}
"drafts: decomp/drafts/draft_report.txt (local); counts: decomp/reports/draft_summary.txt; promoted: decomp/reports/promote_report.txt"
