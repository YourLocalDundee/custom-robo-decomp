#Requires -Version 7
# Batch 10: which CodeWarrior compiler + flags built each part of main.dol (plan.md Phase 4 Milestone 3, docs/compiler.md).
# Compiles reference C from the pinned decomps (dolsdk2004, Melee sysdolphin / MSL / Runtime / MetroTRK, Mario Party 4
# MSL / Runtime / TRK) and our hand-written C for small game functions (tools/compiler_probe/game/) with every pinned GC
# compiler (decomp/build/compilers/GC) and per-library flag sets, and compares each function with main.dol's bytes
# (relocated fields masked). Slow-ish (about 5 minutes, ~50,000 compiles), so not part of run_all.ps1.
#   1. tools/setup_decomp.ps1: compilers, binutils, your main.dol (skips what is done)
#   2. Melee's src/MetroTRK/ in the sparse checkout (added for this batch; an older clone gets it here)
#   3. python -I tools/compiler_probe/probe.py -> tools/compiler_probe/out/{results.tsv,summary.txt} (gitignored)
# Reads only main.dol, decomp/config/GXCE01/symbols.txt and the pinned references; writes nothing outside out/.
# Usage:  pwsh tools/batches/batch10_compiler_probe.ps1 [-Jobs N]
param([int]$Jobs = [Environment]::ProcessorCount)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent

& "$g/tools/setup_decomp.ps1"
if (-not (Test-Path -LiteralPath "$g/tools/melee/src/MetroTRK")) {
    git -C "$g/tools/melee" sparse-checkout add /src/MetroTRK/
    if ($LASTEXITCODE) { throw 'adding src/MetroTRK/ to the Melee checkout failed' }
}
foreach ($r in 'dolsdk2004', 'marioparty4') {
    if (-not (Test-Path -LiteralPath "$g/tools/refs/$r/src")) { throw "tools/refs/$r missing: run tools/setup.ps1 first" }
}

python -I "$g/tools/compiler_probe/probe.py" --jobs $Jobs
if ($LASTEXITCODE) { throw 'probe.py failed' }

# the headline per library: the best version + flags and its matched / compared functions
$out = "$g/tools/compiler_probe/out/summary.txt"
$lines = Get-Content -LiteralPath $out
for ($i = 0; $i -lt $lines.Count; $i++) {
    if ($lines[$i] -like '==== *') { $lines[$i] }
    if ($lines[$i] -like 'best 25*') { $lines[$i + 1] }
    if ($lines[$i] -like 'functions compared*') { $lines[$i] }
}
"batch 10 done: details in tools/compiler_probe/out/summary.txt, every compile in results.tsv; read with docs/compiler.md"
