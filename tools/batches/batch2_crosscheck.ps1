#Requires -Version 7
# Batch 2: Phase 0 step 4 - compare Ghidra's C export with decomp-toolkit's analysis.
# Needs a current decompiled\main_dol.c. Report: symbols\crosscheck_report.txt
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
python -I "$g/tools/crosscheck_dtk.py"
if ($LASTEXITCODE) { throw 'crosscheck_dtk.py failed' }
