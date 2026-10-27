# Rebuild the Ghidra project and decompiled C from scratch, reapplying every saved name.
# Run from anywhere:  pwsh D:\emulators\gameTesting\tools\rebuild_decomp.ps1
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t = $PSScriptRoot
# output folders are not in git (fresh clone): create them
New-Item -ItemType Directory -Force "$g\decompiled", "$g\ghidra_project" | Out-Null
# declarations-only copy of the Melee headers for Ghidra's C parser (see tools\clean_headers.py)
python -I "$t\clean_headers.py" "$t\melee" "$g\build\headers"
if ($LASTEXITCODE) { throw 'clean_headers.py failed' }

& "$t\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat" "$g\ghidra_project" CustomRobo `
    -import "$g\Custom Robo extracted\sys\main.dol" -overwrite `
    -loader GameCubeLoader -loader-autoloadMaps false `
    -scriptPath "$t\scripts" `
    -preScript EnableGCAnalyzer.java `
    -postScript CreateDtkFunctions.java "$t\dtk\proj\symbols.txt" `
    -postScript ApplySymbols.java "$g\symbols\symbols.txt" `
    -postScript ApplySymbols.java "$g\symbols\descriptive.txt" `
    -postScript DefineStrings.java `
    -postScript ParseHeaders.java "$g\build\headers" "$g\symbols\verify_report.txt" "$g\symbols\types_report.txt" `
        "$g\symbols\cr_types.h" "$g\symbols\cr_globals.txt" `
    -postScript ExportXrefs.java "$g\decompiled\xrefs" `
    -postScript ExportDecompiledC.java "$g\decompiled\main_dol.c" *> "$g\decompiled\ghidra_log.txt"

Select-String -LiteralPath "$g\decompiled\ghidra_log.txt" -Pattern 'Created|Applied|Defined|Parsed|Exported|ERROR'
