#Requires -Version 7
# Batch 11: boot the SHIFTED build in Dolphin (plan.md Phase 4 Milestone 2). run_all.ps1 runs it after batch 10 (or run it
# alone). It starts Dolphin twice (about 1 minute) and needs a desktop session; don't type while it runs.
#   1. tools/shift_test.py (static shift test, also the last step of batch 9) -> decomp/build/shift/main.dol, a main.dol
#      with 0x100 bytes of padding at 30 points (code, game code, library objects, .rodata .data .bss .sdata .sbss
#      .sdata2, and inside each data section at proven data-object boundaries), relinked from dtk's split objects, every relocation checked (decomp/reports/shift_report.txt).
#   2. decomp/build/shift/disc/: a scratch copy of "Custom Robo extracted" (hard links, so no disk space and no copy
#      time; the folder itself is never written) with the shifted sys/main.dol in it (a real file, not a link).
#   3. tools/boot_check.ps1 boots the unmodified extracted folder (the control) and then the scratch copy, each for
#      -Seconds, with Dolphin's log for that run (OS reports, files read from the disc) and a screenshot.
#   4. Pass when, for the shifted build: Dolphin is still running with no warning dialog; the OS reports its arena
#      starting at the SHIFTED _stack_addr (the control must show the retail one: proves OSInit used the moved linker
#      symbols); no error-level line in the log; and it read the same disc files in the same order as the control,
#      up to and including the title screen's (sprite/us_title.tpl, TITLE_MODEL.dat).
# Writes decomp/build/shift/boot/{control,shifted}.{log,png} and boot_result.txt (timing-dependent: not checksummed).
# Usage:  pwsh tools/batches/batch11_shift_test.ps1 [-Seconds 22]
param([int]$Seconds = 22)   # the title screen shows from ~13 s; at ~40 s the attract video starts (black between)
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$src = "$g/Custom Robo extracted"
$s = "$g/decomp/build/shift"
if (-not (Test-Path -LiteralPath "$g/decomp/build/GXCE01/main.elf")) { throw 'no decomp/build/GXCE01/main.elf: run tools/batches/batch9_decomp_build.ps1 first' }
if (-not (Test-Path -LiteralPath "$src/sys/main.dol")) { throw "missing $src (tools/setup.ps1 extracts it from your ISO)" }

python -I "$g/tools/shift_test.py"
if ($LASTEXITCODE) { throw 'static shift test failed (decomp/reports/shift_report.txt)' }
$want = python -I "$g/tools/shift_test.py" --expect
if ($LASTEXITCODE) { throw 'shift_test.py --expect failed' }
$exp = $want | ConvertFrom-Json

# scratch copy of the extracted game: hard links (same drive) or copies, then the shifted main.dol as a real file
$disc = "$s/disc"
if (Test-Path -LiteralPath $disc) { Remove-Item -LiteralPath $disc -Recurse -Force }
$srcFull = (Resolve-Path -LiteralPath $src).Path
$linked = 0; $copied = 0
foreach ($f in Get-ChildItem -LiteralPath $srcFull -Recurse -File) {
    $rel = $f.FullName.Substring($srcFull.Length + 1)
    $dst = Join-Path $disc $rel
    $null = New-Item -ItemType Directory -Force -Path (Split-Path $dst -Parent)
    if ($rel -eq (Join-Path 'sys' 'main.dol')) { continue }
    try { $null = New-Item -ItemType HardLink -Path $dst -Target $f.FullName; $linked++ }
    catch { Copy-Item -LiteralPath $f.FullName -Destination $dst; $copied++ }
}
Copy-Item -LiteralPath "$s/main.dol" -Destination (Join-Path $disc 'sys/main.dol')
"scratch game folder: $linked files hard-linked, $copied copied, shifted sys/main.dol"

$b = "$s/boot"
$null = New-Item -ItemType Directory -Force -Path $b
function Boot($dol, $name) {
    $out = & "$g/tools/boot_check.ps1" -Iso $dol -Seconds $Seconds -Log "$b/$name.log" -Screenshot "$b/$name.png"
    $code = $LASTEXITCODE
    $log = if (Test-Path -LiteralPath "$b/$name.log") { Get-Content -LiteralPath "$b/$name.log" } else { @() }
    $files = $log | Where-Object { $_ -match 'W\[FileMon\]:\s+[\d,]+ kB (\S.*)$' } | ForEach-Object { $Matches[1].Trim() }
    $arena = ($log | Where-Object { $_ -match 'OSREPORT\]: Arena : (0x[0-9a-f]+)' } | Select-Object -First 1) -replace '.*Arena : (0x[0-9a-f]+).*', '$1'
    $errors = @($log | Where-Object { $_ -match ' E\[' })
    [pscustomobject]@{ Name = $name; Out = "$out"; Code = $code; Files = @($files); Arena = $arena; Errors = $errors }
}
$c = Boot "$srcFull/sys/main.dol" 'control'
$t = Boot (Join-Path $disc 'sys/main.dol') 'shifted'

$title = 'sprite/us_title.tpl', 'TITLE_MODEL.dat'
$checks = [ordered]@{
    'control boots with no warning'                     = $c.Code -eq 0
    "control arena = retail _stack_addr $($exp.arena_control)" = $c.Arena -eq $exp.arena_control
    'control reaches the title screen files'            = -not ($title | Where-Object { $_ -notin $c.Files })
    'shifted boots, still running, no warning dialog'   = $t.Code -eq 0
    "shifted arena = shifted _stack_addr $($exp.arena_shifted)" = $t.Arena -eq $exp.arena_shifted
    'shifted log has no error-level line'               = $t.Errors.Count -eq 0
    'shifted reads the title screen files'              = -not ($title | Where-Object { $_ -notin $t.Files })
    'shifted reads the same files in the same order as the control' = ($c.Files -join '|') -eq ($t.Files -join '|')
}
$ok = -not ($checks.Values -contains $false)
$lines = @("batch 11 (shift test, Dolphin boot, $Seconds s each): shifted main.dol SHA-1 $($exp.sha1_shifted)", $c.Out, $t.Out)
$lines += $checks.GetEnumerator() | ForEach-Object { "  $(if ($_.Value) { 'ok  ' } else { 'FAIL' }) $($_.Key)" }
$lines += "  control files read: $($c.Files -join ', ')"
$lines += "  shifted files read: $($t.Files -join ', ')"
$lines += $t.Errors | ForEach-Object { "  shifted log error: $_" }
$lines += "screenshots: $b/control.png, $b/shifted.png"
$lines += "result: $(if ($ok) { 'PASS (boots to the title screen)' } else { 'FAIL' })"
[IO.File]::WriteAllText("$b/boot_result.txt", (($lines -join "`r`n") + "`r`n"))
$lines
if (-not $ok) { exit 1 }
