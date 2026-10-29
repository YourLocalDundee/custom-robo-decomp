#Requires -Version 7
# SHA-256 of every file the pipeline produces from the ISO, so anyone can check their rebuild matches ours.
# Only hashes are published (docs\expected_checksums.txt), never the files themselves.
#   pwsh tools\checksums.ps1            print hashes of your outputs
#   pwsh tools\checksums.ps1 -Compare   compare with docs\expected_checksums.txt (exit 1 on any difference)
#   pwsh tools\checksums.ps1 -Write     maintainers: refresh docs\expected_checksums.txt after an intended change
param([switch]$Compare, [switch]$Write)
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$files = @(
    'Custom Robo extracted\sys\main.dol'      # input: must match before anything else can
    'tools\dtk\proj\symbols.txt'              # dtk analysis (setup step 5)
    'tools\dtk\proj\splits.txt'
    'symbols\symbols.txt'                     # verified names after batches 1 + 6 reach their fixpoint (run_all.ps1)
    'symbols\layout_candidates.txt'
    'symbols\candidates_sdk.txt'              # lib candidates (batch 6), after the names <-> Ghidra fixpoint
    'symbols\candidates_rt.txt'
    'symbols\candidates_hsd.txt'
    'symbols\lib_candidates_report.txt'
    'symbols\names_crosscheck.txt'            # name cross-check (batch 8)
    'symbols\verify_report.txt'
    'symbols\types_report.txt'                # header types, prototypes, globals (ParseHeaders.java)
    'symbols\crosscheck_report.txt'
    'symbols\game_map.tsv'
    'decompiled\main_dol.c'                   # Ghidra's C output
    'decompiled\xrefs\functions.tsv'
    'decompiled\xrefs\refs.tsv'
    'decompiled\xrefs\strings.tsv'
)
$now = foreach ($f in $files) {
    $p = Join-Path $g ($f -replace '\\', '/')   # names in the list use backslashes
    $h = if (Test-Path -LiteralPath $p) { (Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash.ToLower() } else { 'MISSING'.PadRight(64) }
    "$h  $f"
}
$expected = "$g/docs/expected_checksums.txt"
if ($Write) { $now | Set-Content -LiteralPath $expected; "wrote $expected"; return }
if (-not $Compare) { $now; return }
$want = Get-Content -LiteralPath $expected | Where-Object { $_ -notmatch '^#' }
$bad = 0
foreach ($line in $now) {
    $f = $line.Substring(66)
    $w = $want | Where-Object { $_.Substring(66) -eq $f }
    if ($w -eq $line) { "OK        $f" } elseif ($line.StartsWith('MISSING')) { "MISSING   $f"; $bad++ } else { "DIFFERENT $f"; $bad++ }
}
if ($bad) { "$bad file(s) differ (see docs\REPRODUCE.md, 'If a checksum differs')"; exit 1 }
"all $($now.Count) files match"
