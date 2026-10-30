#Requires -Version 7
# Rebuild the Ghidra project and decompiled C from scratch, reapplying every saved name.
# Run from anywhere:  pwsh tools/rebuild_decomp.ps1
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t = $PSScriptRoot
# output folders are not in git (fresh clone): create them
New-Item -ItemType Directory -Force "$g/decompiled", "$g/ghidra_project" | Out-Null
# Ghidra must not be running on this project already (an earlier run left behind would keep writing the export)
if (Test-Path -LiteralPath "$g/ghidra_project/CustomRobo.lock") {
    throw "ghidra_project\CustomRobo.lock exists: another Ghidra is using the project (close it, or delete the lock if none runs)"
}
# a failed rebuild must not leave the previous export behind for the name batches to read
Remove-Item -LiteralPath "$g/decompiled/main_dol.c", "$g/decompiled/xrefs/functions.tsv", "$g/decompiled/xrefs/refs.tsv",
    "$g/decompiled/xrefs/strings.tsv" -ErrorAction SilentlyContinue
# declarations-only copy of the Melee headers for Ghidra's C parser (see tools\clean_headers.py)
python -I "$t/clean_headers.py" "$t/melee" "$g/build/headers"
if ($LASTEXITCODE) { throw 'clean_headers.py failed' }

$headless = "$t/ghidra_12.1.4_PUBLIC/support/analyzeHeadless" + $(if ($IsWindows) { '.bat' } else { '' })
& $headless "$g/ghidra_project" CustomRobo `
    -import "$g/Custom Robo extracted/sys/main.dol" -overwrite `
    -loader GameCubeLoader -loader-autoloadMaps false `
    -scriptPath "$t/scripts" `
    -preScript EnableGCAnalyzer.java `
    -postScript CreateDtkFunctions.java "$t/dtk/proj/symbols.txt" `
    -postScript ApplySymbols.java "$g/symbols/symbols.txt" `
    -postScript ApplySymbols.java "$g/symbols/descriptive.txt" `
    -postScript DefineStrings.java `
    -postScript ParseHeaders.java "$g/build/headers" "$g/symbols/verify_report.txt" "$g/symbols/types_report.txt" `
        "$g/symbols/cr_types.h" "$g/symbols/cr_globals.txt" `
    -postScript ExportXrefs.java "$g/decompiled/xrefs" `
    -postScript ExportDecompiledC.java "$g/decompiled/main_dol.c" *> "$g/decompiled/ghidra_log.txt"

$headlessExit = $LASTEXITCODE
Select-String -LiteralPath "$g/decompiled/ghidra_log.txt" -Pattern 'Created|Applied|Defined|Parsed|Exported|ERROR'
# the rebuild is only usable when Ghidra finished both exports and the C has every exported function
$log = Get-Content -LiteralPath "$g/decompiled/ghidra_log.txt" -Raw
$nf = [regex]::Match("$log", 'Exported (\d+) functions, \d+ references').Groups[1].Value
$nc = [regex]::Match("$log", 'Exported (\d+) functions \(0 failed\) to').Groups[1].Value
$heads = if (Test-Path -LiteralPath "$g/decompiled/main_dol.c") { (Select-String -LiteralPath "$g/decompiled/main_dol.c" -Pattern '^// [0-9a-f]{8}  ').Count } else { 0 }
if ($headlessExit -or -not $nf -or $nf -ne $nc -or [int]$nc -ne $heads) {
    throw "Ghidra rebuild incomplete (exit $headlessExit; xrefs '$nf', C '$nc' functions, $heads in main_dol.c): see decompiled\ghidra_log.txt"
}
