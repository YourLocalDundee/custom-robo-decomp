# Everything from a fresh clone + your ISO, in order, stopping at the first failure, ending in PASS / FAIL:
#   1. tools\setup.ps1            download + hash-check all tools, extract the disc
#   2. tools\batches\run_all.ps1  names, Ghidra decompile, cross-check, game map, search index
#   3. tools\checksums.ps1        outputs must be byte-identical to docs\expected_checksums.txt
#   4. archives + models          SFD repack and model round trip byte-identical; all 1,006 models to .dat,
#                                 then glTF (.glb) + textures for Blender; one preview render
#   5. mod test build             test_build\ = the game with robo 0 recoloured, bootable in Dolphin;
#                                 then as ISO -> .crpatch -> applied to the clean ISO, SHA-1 checked
#   6. BUILD_REPORT.md            what was made and where everything is
# Usage:  pwsh tools\from_scratch.ps1 -Iso "C:\path\to\Custom Robo (USA).iso" [-Dolphin "C:\your\Dolphin folder"]
param([Parameter(Mandatory)][string]$Iso, [string]$Dolphin)
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t0 = Get-Date
$results = [ordered]@{}

function Step($name, [scriptblock]$body) {
    "`n=== $name"
    try {
        & $body
        if ($LASTEXITCODE) { throw "exit code $LASTEXITCODE" }
        $results[$name] = 'PASS'
    } catch {
        $results[$name] = "FAIL: $_"
        Report; exit 1
    }
}

function Report {
    $mins = [int]((Get-Date) - $t0).TotalMinutes
    $ok = -not ($results.Values -match '^FAIL')
    "`n=== result ($mins min)"
    $results.GetEnumerator() | ForEach-Object { '{0,-28} {1}' -f $_.Key, $_.Value }
    if ($ok) { 'PASS: fresh build matches the published reference' } else { 'FAIL' }
    $rows = $results.GetEnumerator() | ForEach-Object { "| $($_.Key) | $($_.Value) |" }
    @"
# Build report

Built $(Get-Date -Format 'yyyy-MM-dd HH:mm') from ``$Iso`` in $mins min: **$(if ($ok) { 'PASS' } else { 'FAIL' })**

| Step | Result |
|---|---|
$($rows -join "`n")

## Where everything is

| What | Where | How to use it |
|---|---|---|
| Decompiled game (C) | ``decompiled\main_dol.c``, one file per function in ``decompiled\funcs\`` | any text editor |
| Search everything | ``decompiled\index.db`` | ``python -I tools\find.py text <word>`` (see README, "Reading the code") |
| Ghidra project | ``ghidra_project\CustomRobo.gpr`` | ``tools\ghidra_12.1.4_PUBLIC\ghidraRun.bat``, File > Open Project |
| Names | ``symbols\symbols.txt`` (verified), ``symbols\descriptive.txt`` (ours, ``cr_``) | add names: CONTRIBUTING.md |
| Game code map | ``docs\game_map.md`` | which functions to name next |
| Disc contents | ``Custom Robo extracted\`` | raw files from your disc |
| Archive entries | ``unpacked\`` | ``tools\sfd.py`` unpack / pack |
| Models as HSD .dat | ``models_dat\<category>\`` | open in ``tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe`` |
| Models for Blender | ``models_export\<category>\<name>\*.glb`` + textures | Blender: File > Import > glTF |
| Preview render | ``previews\`` | sanity check |
| Mod test build | ``test_build\`` | Dolphin: open ``test_build\sys\main.dol``; robo 0 has red/blue swapped |
| Make your own mod | ``docs\MODDING.md`` | ``python -I tools\make_mod.py mods\<your mod>`` -> playable ISO + shareable .crpatch |
| How it all works | ``docs\`` (start at docs\README.md), ``plan.md`` | |
"@ | Set-Content -LiteralPath "$g\BUILD_REPORT.md"
    "report: $g\BUILD_REPORT.md"
}

$setupArgs = @{ Iso = $Iso }
if ($Dolphin) { $setupArgs.Dolphin = $Dolphin }
Step 'setup' { & "$g\tools\setup.ps1" @setupArgs }
Step 'decompile (run_all)' { & "$g\tools\batches\run_all.ps1" }
Step 'checksums' { & "$g\tools\checksums.ps1" -Compare }
Step 'SFD unpack + repack' {
    python -I "$g\tools\sfd.py" unpack "$g\Custom Robo extracted\files" "$g\unpacked"
    if ($LASTEXITCODE) { throw 'unpack failed' }
    python -I "$g\tools\sfd.py" verify "$g\Custom Robo extracted\files" "$g\unpacked"
}
Step 'model round trip' {
    $dirs = 'chara', 'weapon_gun', 'weapon_bom', 'weapon_pod', 'weapon_leg', 'scenario_us\snr_chara', 'scenario_jp\snr_chara' |
        ForEach-Object { "$g\unpacked\$_" } | Where-Object { Test-Path -LiteralPath $_ }
    python -I "$g\tools\crmodel.py" roundtrip @dirs
}
Step 'models to .dat' { python -I "$g\tools\extract_models.py" "$g\unpacked" "$g\Custom Robo extracted\files" "$g\models_dat" }
Step 'models to glTF' { python -I "$g\tools\export_models.py" "$g\models_dat" "$g\models_export" }
Step 'preview render' {
    New-Item -ItemType Directory -Force "$g\previews" | Out-Null
    $glb = Get-ChildItem -LiteralPath "$g\models_export\chara" -Recurse -Filter *.glb | Sort-Object FullName | Select-Object -First 1
    & "$g\tools\pyenv\Scripts\python" "$g\tools\render_preview.py" $glb.FullName "$g\previews\chara_first.png"
}
Step 'mod test build' {
    python -I "$g\tools\make_test_build.py" "$g\Custom Robo extracted" "$g\unpacked" "$g\test_build" 0 1 2 3 4
}
Step 'patch round trip' {
    # the mod test build as an ISO -> .crpatch -> applied to the clean ISO must give the same SHA-1 (apply checks)
    $d = "$g\build\patch_test"
    New-Item -ItemType Directory -Force $d | Out-Null
    try {
        python -I "$g\tools\iso_patch.py" build $Iso "$g\test_build" "$d\mod.iso"
        if ($LASTEXITCODE) { throw 'iso_patch build failed' }
        python -I "$g\tools\iso_patch.py" diff $Iso "$d\mod.iso" "$d\test.crpatch"
        if ($LASTEXITCODE) { throw 'iso_patch diff failed' }
        python -I "$g\tools\iso_patch.py" apply $Iso "$d\test.crpatch" "$d\applied.iso"
    } finally {
        Remove-Item -LiteralPath "$d\mod.iso", "$d\applied.iso" -ErrorAction SilentlyContinue
    }
}
Report
