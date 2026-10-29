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
    'symbols\descriptive.txt'                 # game names: hand lines + the "// game-real" lines of batch 7
    'symbols\game_real.txt'                   # batch 7 (tools\match_game_names.py): G1 / G2 names, G3 module notes
    'symbols\game_unnamed_real.txt'           # batch 7 leftovers
    'symbols\names_crosscheck.txt'            # name cross-check (batch 8)
    'symbols\verify_report.txt'
    'symbols\types_report.txt'                # header types, prototypes, globals (ParseHeaders.java)
    'symbols\crosscheck_report.txt'
    'symbols\game_map.tsv'
    'decompiled\main_dol.c'                   # Ghidra's C output
    'decompiled\xrefs\functions.tsv'
    'decompiled\xrefs\refs.tsv'
    'decompiled\xrefs\strings.tsv'
    'decomp\config\GXCE01\symbols.txt'     # Phase 4 build config (batch 9): dtk analysis + our names, dtk-normalised
    'decomp\config\GXCE01\splits.txt'      # proven object boundaries (tools\gen_dtk_splits.py)
    'decomp\reports\symbols_report.txt'
    'decomp\reports\splits_report.txt'
    'decomp\reports\build_report.txt'       # built main.dol SHA-1 == retail
    'decomp\config\GXCE01\config.yml'      # + the generated relocation fixes (tools\gen_dtk_relocs.py, Milestone 2)
    'decomp\reports\relocs_report.txt'      # relocation audit: what dtk missed / invented + the check of the build
    'decomp\reports\shift_report.txt'       # static shift test (tools\shift_test.py): padded relink, all checked
    'decomp\reports\progress_report.txt'    # Milestone 4: objects built from C, bytes per library (tools\check_c_objects.py)
    'decomp\reports\sources_report.txt'     # provenance of every file in decomp\src + decomp\include
    'decomp\reports\c_symbols_report.txt'   # the C objects' own symbols put into symbols.txt (tools\gen_c_symbols.py)
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
