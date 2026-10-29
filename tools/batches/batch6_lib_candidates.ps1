#Requires -Version 7
# Batch 6: name the remaining library functions (naming policy rules R3 layout / R4 asserts / R5 call graph / R6 string).
# Runs the three matchers (tools\match_lib_sdk.py 0x80131000-0x8015F000, match_lib_rt.py 0x8015F000-0x8016D408,
# match_lib_hsd.py 0x8016D408-), merges what they find into symbols.txt tagged "lib-candidate", and repeats until no new
# name appears (max 10 rounds), because every matcher reads symbols.txt: names found by one are verified neighbours for the others.
# Then runs verify_symbols.py and stops if any merged name fails. Details: symbols\lib_candidates_report.txt.
# Rerunnable: strips the previously merged lines first. Run right after batch 1 (which rewrites the layout-proof lines).
# A candidate with "replaces:OLD" in its why: removes the layout-proof line OLD at that address (a run of same-size
# functions that the size-only layout match had shifted by one). Does not rebuild Ghidra (run_all.ps1 does).
# The matchers read the Ghidra export decompiled\xrefs\*.tsv for structure only - calls, string refs, computed jumps by address,
# never a name Ghidra applied (tools\export_struct.py) - and main.dol (asserts from the instruction bytes, tools\dol_asserts.py),
# never the decompiled C; our names come from the symbol files. When the
# export does not exist yet (fresh setup) the matchers are skipped and symbols\candidates_*.txt are merged as they are
# (run_all.ps1 empties them first, then reruns this batch after every Ghidra rebuild until the names stop changing).
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$sym = "$g\symbols\symbols.txt"
$tmp = $env:TEMP
$acc = "$tmp\lib_candidates_accepted.txt"
$rep = "$tmp\lib_candidates_replaced.txt"
$part = "$tmp\lib_candidates_part.txt"

$kept = Get-Content -LiteralPath $sym | Where-Object { $_ -notmatch '// lib-candidate ' }
Set-Content -LiteralPath $sym -Value $kept

$export = "$g\decompiled\xrefs\functions.tsv", "$g\decompiled\xrefs\refs.tsv", "$g\decompiled\xrefs\strings.tsv"
$haveIndex = -not ($export | Where-Object { -not (Test-Path -LiteralPath $_) })
if (-not $haveIndex) { "batch6: Ghidra export (decompiled\xrefs) missing - matchers skipped, merging symbols\candidates_*.txt as they are" }
$matchers = 'sdk', 'rt', 'hsd'
$parts = @()
$accum = @{ sdk = @(); rt = @(); hsd = @() }
$rounds = 0
for ($round = 1; $round -le 10; $round++) {
    $counts = @()
    if ($haveIndex) {
        foreach ($m in $matchers) {
            python -I "$g\tools\match_lib_$m.py" | Out-Null
            if ($LASTEXITCODE) { throw "match_lib_$m.py failed" }
            $lines = @(Get-Content -LiteralPath "$g\symbols\candidates_$m.txt" | Where-Object { $_ -match '^\S+ = \.text:' })
            $accum[$m] += $lines
            $n = $lines.Count
            $counts += "$m=$n"
        }
    }
    $r = python -I "$g\tools\merge_lib_candidates.py" $acc $rep $part
    if ($LASTEXITCODE) { throw 'merge_lib_candidates.py failed' }
    $nums = ("$r").Trim().Split(' ')
    $replaced = @{}
    Get-Content -LiteralPath $rep | ForEach-Object { $p = $_.Split(' '); $replaced["$($p[0]) $($p[1].ToUpper().Replace('0X', '0x'))"] = 1 }
    if ($replaced.Count) {
        $kept = Get-Content -LiteralPath $sym | Where-Object {
            $m = [regex]::Match($_, '^(\S+) = \.text:0x([0-9A-Fa-f]{8});')
            -not ($m.Success -and $_ -match '// layout-proof ' -and $replaced.ContainsKey($m.Groups[1].Value + ' 0x' + $m.Groups[2].Value.ToUpper()))
        }
        Set-Content -LiteralPath $sym -Value $kept
    }
    Get-Content -LiteralPath $acc | Add-Content -LiteralPath $sym
    $parts += , (Get-Content -LiteralPath $part)
    $rounds = $round
    "batch6 round ${round}: candidate lines $($counts -join ' ') read=$($nums[0]) accepted=$($nums[1]) rejected=$($nums[2]) layout-proof replaced=$($nums[3])"
    if ([int]$nums[1] -eq 0) { break }
    if (-not $haveIndex) { break }   # nothing changes between rounds without matchers
}

# the matchers overwrite their file every round: leave the union of all rounds in symbols\candidates_<matcher>.txt
if ($haveIndex) {
    foreach ($m in $matchers) {
        Set-Content -LiteralPath "$g\symbols\candidates_$m.txt" -Value (@("// all rounds of batch6_lib_candidates.ps1 (tools\match_lib_$m.py)") +
            ($accum[$m] | Sort-Object { [Convert]::ToInt64(([regex]::Match($_, '\.text:0x([0-9A-Fa-f]{8})').Groups[1].Value), 16) } -Unique))
    }
}

# report: unique REJECT / REPLACED lines, every ACCEPT line
$all = $parts | ForEach-Object { $_ } | Where-Object { $_ }
$acceptL = @($all | Where-Object { $_ -like 'ACCEPT *' } | Sort-Object -Unique)
$rejectL = @($all | Where-Object { $_ -like 'REJECT *' } | Sort-Object -Unique)
$replL = @($all | Where-Object { $_ -like 'REPLACED *' } | Sort-Object -Unique)
$byRule = ($acceptL | ForEach-Object { $_.Split(' ')[3] } | Group-Object | Sort-Object Name | ForEach-Object { "$($_.Name):$($_.Count)" }) -join ', '
$head = "lib candidates: $($acceptL.Count) accepted ($byRule), $($rejectL.Count) rejected, $($replL.Count) layout-proof names replaced, $rounds round(s)"
Set-Content -LiteralPath "$g\symbols\lib_candidates_report.txt" -Value (@($head) + $rejectL + $replL + $acceptL)

$refs = @("$g\tools\melee\config\GALE01\symbols.txt") + (Get-ChildItem -LiteralPath "$g\tools\refs" -Directory |
    ForEach-Object { Get-ChildItem -LiteralPath "$($_.FullName)\config" -Directory -ErrorAction SilentlyContinue } |
    ForEach-Object { "$($_.FullName)\symbols.txt" } | Where-Object { Test-Path -LiteralPath $_ } | Sort-Object)
python -I "$g\tools\verify_symbols.py" $sym "$g\tools\dtk\proj\symbols.txt" @refs |
    Set-Content -LiteralPath "$g\symbols\verify_report.txt"
$merged = Get-Content -LiteralPath $sym | Where-Object { $_ -match '// lib-candidate ' } | ForEach-Object { ($_ -split ' ')[0] }
$bad = $merged | Where-Object { Select-String -LiteralPath "$g\symbols\verify_report.txt" -Pattern "^(MISMATCH|UNSOURCED|NO-SIZE)\s+\S+\s+$([regex]::Escape($_))\s" -Quiet }
"batch6: $head; verify: $((Get-Content -LiteralPath "$g\symbols\verify_report.txt")[-1])"
if ($bad) { throw "lib-candidate names failing verification ($($bad.Count)): $($bad -join ', ')" }
