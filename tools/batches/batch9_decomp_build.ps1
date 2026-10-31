#Requires -Version 7
# Batch 9: the Phase 4 build (plan.md Phase 4 Milestones 1 + 2). Regenerates decomp/config/GXCE01/{symbols,splits}.txt from
# dtk's analysis + our verified names + the reference layouts, configures and builds main.dol from dtk's split objects
# (the configure.py CR_MATCHING objects compiled from decomp/src, every other one from asm), and checks the built main.dol is the retail one (SHA-1 6a73cd11...fb50b).
#   1. tools/setup_decomp.ps1: template files, pinned build tools, your main.dol (skips what is done)
#   2. tools/gen_dtk_symbols.py -> config symbols.txt (+ decomp/reports/symbols_report.txt)
#   2a. tools/sync_game_names.py: a promoted game file (decomp/src/game) calls a function whose name changed since the
#      previous symbols.txt by its old name: renamed, so the link finds it (batch 12 writes the file again)
#   3. tools/gen_dtk_splits.py  -> config splits.txt  (+ decomp/reports/splits_report.txt): code objects from the
#      reference layouts + library sources, data pieces per object from a sections-only scratch link's relocations
#      (decomp/build/splitdata/, SHA-1 retail) checked against the references' piece sizes
#   3b. tools/gen_dtk_relocs.py -> the generated relocation-fix block of config.yml (+ decomp/reports/relocs_report.txt):
#      dtk's own relocations (an unfixed split + link in decomp/build/relocs/) audited from main.dol's bytes; the
#      pointers dtk missed become add_relocations, the non-pointers it relocated block_relocations (Milestone 2)
#   4. python configure.py; ninja. dtk's first split normalises both files (symbols.txt: its re-analysis written
#      back, align: / data: attributes and a few sizes, names never change; splits.txt: objects in dtk's resolved link
#      order, ranges unchanged); a second, forced split must then leave both byte-identical (a fixpoint). The
#      committed files are these normalised ones.
#   4b. tools/gen_c_symbols.py: the objects linked from C (configure.py CR_MATCHING, Milestone 4) put their own symbols
#      (main.elf's, at the retail addresses) into symbols.txt; ninja again (a fresh split), so objdiff pairs by name.
#   5. SHA-1 of decomp/build/GXCE01/main.dol == retail, written to decomp/reports/build_report.txt.
#   5b. tools/check_c_objects.py: every C object linked is 100% code and data in objdiff; every other C file is listed
#      with its reason; per-library progress -> decomp/reports/progress_report.txt, provenance -> sources_report.txt.
#   5c. tools/check_c_callsites.py: every name of symbols/symbols.txt tagged "c-callsite" (rule R7) is re-derived from the
#      matched C: the relocation of a 100% function at that call site and main.elf's relocation there agree on the
#      address; other names the matched C gives to dtk default names are listed -> decomp/reports/c_callsites_report.txt.
#   6. tools/gen_dtk_relocs.py --check: the built main.elf has no missed pointer, no false one, and every relocated
#      value equals its target (appended to relocs_report.txt).
#   7. tools/shift_test.py: the static shift test (Milestone 2) on a scratch copy of the build (decomp/build/shift/):
#      padding at 30 points (incl. data-object boundaries), relinked, every relocation / symbol / address checked -> decomp/reports/shift_report.txt.
#      Booting the shifted main.dol in Dolphin is batch 11 (tools/batches/batch11_shift_test.ps1, by hand).
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
python -I "$g/tools/check_refs.py"   # gen_dtk_splits.py reads the reference sources: the setup's checkouts only
if ($LASTEXITCODE) { throw 'reference checkouts differ from tools/setup.ps1 (see above)' }
# the previous symbols.txt: the promoted game files (decomp/src/game) take the names that changed since (2a)
$before = "$g/decomp/build/symbols_before_batch9.txt"
New-Item -ItemType Directory -Force "$g/decomp/build" | Out-Null
if (Test-Path -LiteralPath "$g/decomp/config/GXCE01/symbols.txt") { Copy-Item -LiteralPath "$g/decomp/config/GXCE01/symbols.txt" $before -Force }
else { Remove-Item -LiteralPath $before -Force -ErrorAction SilentlyContinue }
python -I "$g/tools/gen_dtk_symbols.py"
if ($LASTEXITCODE) { throw 'gen_dtk_symbols.py failed' }
python -I "$g/tools/sync_game_names.py" $before
if ($LASTEXITCODE) { throw 'sync_game_names.py failed' }
python -I "$g/tools/gen_dtk_splits.py"
if ($LASTEXITCODE) { throw 'gen_dtk_splits.py failed' }
python -I "$g/tools/gen_dtk_relocs.py"
if ($LASTEXITCODE) { throw 'gen_dtk_relocs.py failed' }

function Hashes { ('symbols.txt', 'splits.txt', 'config.yml' | ForEach-Object { (Get-FileHash -LiteralPath "$cfg/$_" -Algorithm SHA256).Hash }) -join ' ' }
Push-Location $d
try {
    # always link afresh, so the SHA-1 below is of this run's main.dol
    Remove-Item -LiteralPath build/GXCE01/main.elf, build/GXCE01/main.dol -ErrorAction SilentlyContinue
    python configure.py
    if ($LASTEXITCODE) { throw 'configure.py failed' }
    # the objects compiled from decomp/src are rebuilt every run: a new override header (decomp/include/cr_*) changes
    # which header an #include finds, and ninja's dependency file of an object compiled before it does not list it
    Get-ChildItem -LiteralPath build/GXCE01/src -Recurse -Filter *.o -ErrorAction SilentlyContinue | Remove-Item -Force
    & $ninja
    if ($LASTEXITCODE) { throw 'ninja failed (see above; dtk rejects a bad split here)' }
    # the objects linked from C (configure.py CR_MATCHING) give their own symbols to symbols.txt (main.elf has them at
    # the retail addresses): then split again, so objdiff pairs their data by name
    python -I "$g/tools/gen_c_symbols.py"
    if ($LASTEXITCODE) { throw 'gen_c_symbols.py failed' }
    & $ninja
    if ($LASTEXITCODE) { throw 'ninja (after gen_c_symbols.py) failed' }
    $first = Hashes
    # force a second split from the normalised files: nothing may change any more
    (Get-Item -LiteralPath "$cfg/symbols.txt").LastWriteTime = Get-Date
    & $ninja
    if ($LASTEXITCODE) { throw 'ninja (second pass) failed' }
    if ((Hashes) -ne $first) { throw 'dtk changed symbols.txt / splits.txt / config.yml again on the second split: not a fixpoint' }
} finally { Pop-Location }

$sha1 = (Get-FileHash -LiteralPath "$d/build/GXCE01/main.dol" -Algorithm SHA1).Hash.ToLower()
$sp = Get-Content -LiteralPath "$d/reports/splits_report.txt" -TotalCount 4
$sy = Get-Content -LiteralPath "$d/reports/symbols_report.txt" -TotalCount 4
$rl = Get-Content -LiteralPath "$d/reports/relocs_report.txt" | Select-String -Pattern '^fixes written' | ForEach-Object { "relocation $($_.Line)" }
$ok = $sha1 -eq $retail
$report = @(
    'decomp build (tools/batches/batch9_decomp_build.ps1): main.dol linked from dtk''s split objects and the objects built from C (decomp/reports/progress_report.txt)'
    "built  decomp/build/GXCE01/main.dol SHA-1 $sha1"
    "retail main.dol (GXCE01 v1.0)       SHA-1 $retail"
    "result: $(if ($ok) { 'MATCH' } else { 'DIFFERENT' })"
    ''
) + $sy + '' + $sp + '' + $rl
# CRLF on every OS, like the two generator reports, so the checksum is the same everywhere
[IO.File]::WriteAllText("$d/reports/build_report.txt", (($report -join "`r`n") + "`r`n"))
$report[1..3]
if (-not $ok) { throw "built main.dol SHA-1 $sha1 is not the retail $retail" }
python -I "$g/tools/check_c_objects.py"
if ($LASTEXITCODE) { throw 'a C object is not 100% in objdiff, or a C file is not accounted for (decomp/reports/progress_report.txt)' }
python -I "$g/tools/check_c_callsites.py"
if ($LASTEXITCODE) { throw 'a // c-callsite name of symbols/symbols.txt is not confirmed by the matched C call sites (decomp/reports/c_callsites_report.txt)' }
python -I "$g/tools/gen_dtk_relocs.py" --check
if ($LASTEXITCODE) { throw 'the built main.elf still has relocation misses / false positives (decomp/reports/relocs_report.txt)' }
python -I "$g/tools/shift_test.py"
if ($LASTEXITCODE) { throw 'static shift test failed (decomp/reports/shift_report.txt)' }
"batch 9 done: main.dol matches retail, relocations checked, shifted build checked; reports in decomp/reports/"
