#Requires -Version 7
# Batch 1: library names by layout proof (naming policy rule 3), then verify.
# Rerunnable: strips the previous layout-proof and lib-candidate lines from symbols.txt (batch 6 re-adds the latter:
# run it next; batch 6 output is never input here), rematches to a fixpoint, re-verifies.
# Stops if any layout-proof name fails verify_symbols.py. Does not rebuild Ghidra (run_all.ps1 does).
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$sym = "$g\symbols\symbols.txt"

$kept = Get-Content -LiteralPath $sym | Where-Object { $_ -notmatch '// (layout-proof|lib-candidate) ' }
Set-Content -LiteralPath $sym -Value $kept
python -I "$g\tools\layout_match.py"
if ($LASTEXITCODE) { throw 'layout_match.py failed' }
Get-Content -LiteralPath "$g\symbols\layout_candidates.txt" | Where-Object { $_ -notmatch '^//' } |
    ForEach-Object { $_ -replace ' ref:', ' scope:global // layout-proof ref:' } | Add-Content -LiteralPath $sym

# tools\refs\<game>\config\<version>\symbols.txt, found without wildcards so paths with [ ] work
$refs = @("$g\tools\melee\config\GALE01\symbols.txt") + (Get-ChildItem -LiteralPath "$g\tools\refs" -Directory |
    ForEach-Object { Get-ChildItem -LiteralPath "$($_.FullName)\config" -Directory -ErrorAction SilentlyContinue } |
    ForEach-Object { "$($_.FullName)\symbols.txt" } | Where-Object { Test-Path -LiteralPath $_ } | Sort-Object)
python -I "$g\tools\verify_symbols.py" $sym "$g\tools\dtk\proj\symbols.txt" @refs |
    Set-Content -LiteralPath "$g\symbols\verify_report.txt"
$layout = (Select-String -LiteralPath $sym -Pattern '// layout-proof ').Count
$bad = Get-Content -LiteralPath "$g\symbols\layout_candidates.txt" | Where-Object { $_ -notmatch '^//' } | ForEach-Object { ($_ -split ' ')[0] } |
    Where-Object { Select-String -LiteralPath "$g\symbols\verify_report.txt" -Pattern "^(MISMATCH|UNSOURCED|NO-SIZE)\s+\S+\s+$_\s" -Quiet }
"batch1: $layout layout-proof names in symbols.txt; verify: $((Get-Content -LiteralPath "$g\symbols\verify_report.txt")[-1])"
if ($bad) { throw "layout-proof names failing verification: $($bad -join ', ')" }
