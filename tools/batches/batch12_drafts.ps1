#Requires -Version 7
# Batch 12: draft and check the game code (plan.md Phase 4 Milestone 5, docs/drafts.md).
# For each of the ~1,750 game functions (decomp/config/GXCE01/splits.txt: the automatic object from 0x800055E0, i.e. not
# in a library object): m2c (tools/m2c, pinned in tools/pins.txt) writes a C draft with our names and our types
# (HSD headers + symbols/cr_types.h) as context, the draft is compiled with the game compiler and flags (GC/2.6,
# configure.py CR_LIBS "game"), and the compiled function is compared with the original one in dtk's split object
# (relocation fields masked). About 5 minutes on 8 cores, parallel (multiprocessing), deterministic.
#   1. needs batch 9 (decomp/build/GXCE01/asm + obj, decomp/build/compilers) and tools/m2c (tools/setup.ps1); read only
#   2. python -I tools/draft_batch.py -> decomp/drafts/ (GITIGNORED: drafts, asm, objects, logs, draft_report.txt)
#      and decomp/reports/draft_summary.txt (counts only; committed)
# Never commit anything in decomp/drafts/. Not in tools/checksums.ps1 (see docs/drafts.md "Determinism").
# Usage:  pwsh tools/batches/batch12_drafts.ps1 [-Sample N] [-Jobs N]
param([int]$Sample = 0, [int]$Jobs = [Environment]::ProcessorCount)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
foreach ($p in 'tools/m2c/m2c/main.py', 'decomp/build/GXCE01/asm/auto_01_800055E0_text.s', 'decomp/build/GXCE01/obj/auto_01_800055E0_text.o', 'decomp/build/compilers/GC/2.6/mwcceppc.exe', 'tools/melee/src/sysdolphin/baselib') {
    if (-not (Test-Path -LiteralPath "$g/$p")) { throw "$p missing: run tools/setup.ps1 (m2c, Melee headers) and batch 9 first" }
}
$a = @('--jobs', $Jobs)
if ($Sample) { $a += @('--sample', $Sample) }
python -I "$g/tools/draft_batch.py" @a
if ($LASTEXITCODE) { throw 'draft_batch.py failed' }
"drafts: decomp/drafts/draft_report.txt (local); counts: decomp/reports/draft_summary.txt"
