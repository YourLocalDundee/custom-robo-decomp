#Requires -Version 7
# Batch 12: draft and check the game code (plan.md Phase 4 Milestone 5, docs/drafts.md).
# For each of the ~1,750 game functions (decomp/config/GXCE01/splits.txt: 0x800055E0 up to the first library object; dtk's automatic objects there and the game/ objects linked from C; i.e. not
# in a library object): m2c (tools/m2c, pinned in tools/pins.txt) writes a C draft with our names and our types
# (HSD headers + symbols/cr_types.h) as context, the draft is compiled with the game compiler and flags (GC/2.6,
# configure.py CR_LIBS "game"), and the compiled function is compared with the original one in dtk's split object
# (relocation fields masked). About 26 minutes on this desktop (1,552 s on Oct 31, 2026: two m2c passes, the repair loop and the variants of every
# draft below 100%, plan.md steps 77-83; -Sample 200 for a quick look), parallel (multiprocessing), deterministic.
#   1. needs batch 9 (decomp/build/GXCE01/asm + obj, decomp/build/compilers) and tools/m2c (tools/setup.ps1); read only
#   2. python -I tools/draft_batch.py -> decomp/drafts/ (GITIGNORED: drafts, asm, objects, logs, draft_report.txt)
#      and decomp/reports/draft_summary.txt (counts only; committed)
#   3. python -I tools/promote_drafts.py: every 100% draft that can be linked as it is (no data of its own, no padding,
#      every relocation to the same address, compiles the same behind the committed decomp/include/game/cr_game.h)
#      -> decomp/src/game/<function>.c (COMMITTED: matched C, one function per object, plan.md step 58) and
#      decomp/reports/promote_report.txt. Rerunnable; nothing changes when the drafts did not. When decomp/src/game
#      changed, batch 9 runs again here, so the build (retail SHA-1, objdiff) and its reports include the new files.
#      (Not with -Sample: a sample's report lists only some functions.)
#   4. the data-only game units (docs/decomp.md "Game data objects", step 111), from the batch 9 build before this
#      batch and this batch's context (decomp/drafts/ctx_pp.c, the types of symbols/cr_types.h):
#      python -I tools/gen_data_c.py --scan-untyped (symbols/bss_untyped.txt: the untyped .bss / .sbss runs),
#      python -I tools/scan_lbl_types.py (symbols/lbl_types.txt: the typed families under dtk names; each family checks
#      itself and stops the batch on anything else), python -I tools/gen_data_c.py --all (decomp/src/game/data:
#      every unit written again, compiled and placed before it is written; a unit no list produces is removed).
#      COMMITTED: the two lists and decomp/src/game/data. A new cut in symbols/data_splits.txt needs one batch 9 before
#      this step sees the object it makes (the full run's batch 9 -> batch 12 order does that). Not with -Sample.
#      When decomp/src/game or decomp/src/game/data changed, batch 9 runs again.
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
    $changed = $p[-1] -like 'changed*'
    "== data units (step 4): untyped .bss / .sbss runs, typed lbl_ families, decomp/src/game/data"
    python -I "$g/tools/gen_data_c.py" --scan-untyped
    if ($LASTEXITCODE) { throw 'gen_data_c.py --scan-untyped failed' }
    python -I "$g/tools/scan_lbl_types.py"
    if ($LASTEXITCODE) { throw 'scan_lbl_types.py failed (a family did not check; see its message)' }
    $d = python -I "$g/tools/gen_data_c.py" --all
    if ($LASTEXITCODE) { $d; throw 'gen_data_c.py --all failed' }
    $du = @($d | Where-Object { $_ -match ': (written|rewritten|removed)' })
    $du
    "data units: $(@($d).Count - $du.Count) unchanged, $($du.Count) written or removed"
    if ($du.Count) { $changed = $true }
    if ($changed) {
        "== decomp/src/game or decomp/src/game/data changed: batch 9 again (the build links the new files)"
        & "$PSScriptRoot/batch9_decomp_build.ps1"
    }
}
"drafts: decomp/drafts/draft_report.txt (local); counts: decomp/reports/draft_summary.txt; promoted: decomp/reports/promote_report.txt"
