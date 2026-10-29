#Requires -Version 7
# Batch 9: the Phase 4 build (plan.md Phase 4 Milestone 1). Regenerates decomp/config/GXCE01/{symbols,splits}.txt from
# dtk's analysis + our verified names + the reference layouts, configures and builds main.dol from dtk's split objects
# (every object NonMatching), and checks the built main.dol is the retail one (SHA-1 6a73cd11...fb50b).
#   1. tools/setup_decomp.ps1: template files, pinned build tools, your main.dol (skips what is done)
#   2. tools/gen_dtk_symbols.py -> config symbols.txt (+ decomp/reports/symbols_report.txt)
#   3. tools/gen_dtk_splits.py  -> config splits.txt  (+ decomp/reports/splits_report.txt)
#   4. python configure.py; ninja. dtk's first split normalises both files (symbols.txt: its re-analysis written
#      back, align: / data: attributes and a few sizes, names never change; splits.txt: objects in dtk's resolved link
#      order, ranges unchanged); a second, forced split must then leave both byte-identical (a fixpoint). The
#      committed files are these normalised ones.
#   5. SHA-1 of decomp/build/GXCE01/main.dol == retail, written to decomp/reports/build_report.txt.
# Outside the name fixpoint of run_all.ps1: reads the committed name files only, never Ghidra.
# Usage:  pwsh tools/batches/batch9_decomp_build.ps1
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$d = "$g/decomp"
$cfg = "$d/config/GXCE01"
$retail = '6a73cd111ef349b20be294511b8c51a8addfb50b'
$exe = if ($IsWindows) { '.exe' } else { '' }
$ninja = "$g/tools/ninja/ninja$exe"

& "$g/tools/setup_decomp.ps1"
python -I "$g/tools/gen_dtk_symbols.py"
if ($LASTEXITCODE) { throw 'gen_dtk_symbols.py failed' }
python -I "$g/tools/gen_dtk_splits.py"
if ($LASTEXITCODE) { throw 'gen_dtk_splits.py failed' }

function Hashes { "$((Get-FileHash -LiteralPath "$cfg/symbols.txt" -Algorithm SHA256).Hash) $((Get-FileHash -LiteralPath "$cfg/splits.txt" -Algorithm SHA256).Hash)" }
Push-Location $d
try {
    # always link afresh, so the SHA-1 below is of this run's main.dol
    Remove-Item -LiteralPath build/GXCE01/main.elf, build/GXCE01/main.dol -ErrorAction SilentlyContinue
    python configure.py
    if ($LASTEXITCODE) { throw 'configure.py failed' }
    & $ninja
    if ($LASTEXITCODE) { throw 'ninja failed (see above; dtk rejects a bad split here)' }
    $first = Hashes
    # force a second split from the normalised files: nothing may change any more
    (Get-Item -LiteralPath "$cfg/symbols.txt").LastWriteTime = Get-Date
    & $ninja
    if ($LASTEXITCODE) { throw 'ninja (second pass) failed' }
    if ((Hashes) -ne $first) { throw 'dtk changed symbols.txt / splits.txt again on the second split: not a fixpoint' }
} finally { Pop-Location }

$sha1 = (Get-FileHash -LiteralPath "$d/build/GXCE01/main.dol" -Algorithm SHA1).Hash.ToLower()
$sp = Get-Content -LiteralPath "$d/reports/splits_report.txt" -TotalCount 4
$sy = Get-Content -LiteralPath "$d/reports/symbols_report.txt" -TotalCount 4
$ok = $sha1 -eq $retail
$report = @(
    'decomp build (tools/batches/batch9_decomp_build.ps1): main.dol linked from dtk''s split objects, every object NonMatching'
    "built  decomp/build/GXCE01/main.dol SHA-1 $sha1"
    "retail main.dol (GXCE01 v1.0)       SHA-1 $retail"
    "result: $(if ($ok) { 'MATCH' } else { 'DIFFERENT' })"
    ''
) + $sy + '' + $sp
# CRLF on every OS, like the two generator reports, so the checksum is the same everywhere
[IO.File]::WriteAllText("$d/reports/build_report.txt", (($report -join "`r`n") + "`r`n"))
$report[1..3]
if (-not $ok) { throw "built main.dol SHA-1 $sha1 is not the retail $retail" }
"batch 9 done: main.dol matches retail; reports in decomp/reports/"
