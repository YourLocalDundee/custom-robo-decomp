# Rebuild the Ghidra project and decompiled C from scratch, reapplying every saved name.
# Run from anywhere:  pwsh D:\emulators\gameTesting\tools\rebuild_decomp.ps1
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t = $PSScriptRoot
# output folders are not in git (fresh clone): create them
New-Item -ItemType Directory -Force "$g\decompiled", "$g\ghidra_project" | Out-Null

& "$t\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat" "$g\ghidra_project" CustomRobo `
    -import "$g\Custom Robo extracted\sys\main.dol" -overwrite `
    -loader GameCubeLoader -loader-autoloadMaps false `
    -scriptPath "$t\scripts" `
    -preScript EnableGCAnalyzer.java `
    -postScript CreateDtkFunctions.java "$t\dtk\proj\symbols.txt" `
    -postScript ApplySymbols.java "$g\symbols\symbols.txt" `
    -postScript ApplySymbols.java "$g\symbols\descriptive.txt" `
    -postScript DefineStrings.java `
    -postScript ExportXrefs.java "$g\decompiled\xrefs" `
    -postScript ExportDecompiledC.java "$g\decompiled\main_dol.c" *> "$g\decompiled\ghidra_log.txt"

Select-String -LiteralPath "$g\decompiled\ghidra_log.txt" -Pattern 'Created|Applied|Defined|Exported|ERROR'
