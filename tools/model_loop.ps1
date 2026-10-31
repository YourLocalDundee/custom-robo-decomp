#Requires -Version 7
# Blender-to-game loop for one model edit (docs/MODDING.md "Blender loop"): for each entry
#   export (HSDRawViewer --export, once) -> Blender script (tools/blender/<Script>.py) -> HSDRawViewer --import over the
#   original .dat -> mods/<Mod>/models/<Category>/<name>.dat
# then python -I tools/make_mod.py mods/<Mod> -> the ISO and the .crpatch, and optionally a 40 s boot in Dolphin.
#   pwsh tools\model_loop.ps1 -Mod ray_legionnaire -Script legionnaire -Entries 000,001,002,003,004 [-Boot]
#   pwsh tools\model_loop.ps1 -Mod rt_test -Script roundtrip -Entries 000          (no edit: the pipeline itself)
# Options: -Category chara (default; the only one with a roles file for the scripts), -Iso <clean.iso> (default: the
#   repo's "Custom Robo (USA).iso"), -Blender <blender.exe> (default: $env:BLENDER, then Blender on PATH, then the
#   newest "C:\Program Files\Blender Foundation\Blender *\blender.exe"), -Fresh (export again even if it exists),
#   -Seconds 40 (boot time). Work files: build/model_loop/<Mod>/ (the export, the edited glb, the roles file).
# Windows only for the export / import steps (HSDRawViewer is a Windows program); tools/model_loop.sh is the bash twin.
param([Parameter(Mandatory)][string]$Mod, [Parameter(Mandatory)][string]$Script, [Parameter(Mandatory)][string[]]$Entries,
      [string]$Category = 'chara', [string]$Iso, [string]$Blender, [switch]$Fresh, [switch]$Boot, [int]$Seconds = 40)
$ErrorActionPreference = 'Stop'
$Entries = @($Entries -split ',' | ForEach-Object { $_.Trim() } | Where-Object { $_ })   # 000,001 from pwsh -File too
$g = Split-Path $PSScriptRoot -Parent
$exe = Join-Path $g 'tools/HSDLib/HSDRawViewer/bin/Release/net8.0-windows7.0/HSDRawViewer.exe'
if (-not (Test-Path -LiteralPath $exe)) { throw "HSDRawViewer not built: $exe (pwsh tools\setup.ps1)" }
$py = Join-Path $g "tools/blender/$Script.py"
if (-not (Test-Path -LiteralPath $py)) { throw "no Blender script tools/blender/$Script.py" }
if (-not $Blender) { $Blender = $env:BLENDER }
if (-not $Blender) { $Blender = (Get-Command blender -ErrorAction SilentlyContinue)?.Source }
if (-not $Blender) {
    $Blender = Get-ChildItem 'C:\Program Files\Blender Foundation\Blender *\blender.exe' -ErrorAction SilentlyContinue |
        Sort-Object { [version]($_.Directory.Name -replace '[^0-9.]', '') } | Select-Object -Last 1 -ExpandProperty FullName
}
if (-not $Blender -or -not (Test-Path -LiteralPath $Blender)) { throw 'Blender not found: pass -Blender <blender.exe> or set BLENDER' }
$work = Join-Path $g "build/model_loop/$Mod"
$out = Join-Path $g "mods/$Mod/models/$Category"
New-Item -ItemType Directory -Force $work, $out | Out-Null
if (-not (Test-Path -LiteralPath (Join-Path $g "mods/$Mod/mod.json"))) {
    @{ name = $Mod; author = 'model_loop'; description = "tools/blender/$Script.py on $Category $($Entries -join ', ')" } |
        ConvertTo-Json | Set-Content -LiteralPath (Join-Path $g "mods/$Mod/mod.json")
}
foreach ($e in $Entries) {
    $name = "${Category}_$e"
    $dat = Join-Path $g "models_dat/$Category/$name.dat"
    if (-not (Test-Path -LiteralPath $dat)) { throw "no $dat (run the one-time build first: README quick start)" }
    # 1. export (the original .dat -> glb), once
    $glb = Join-Path $work "export/$name/root0_joint.glb"
    if ($Fresh -or -not (Test-Path -LiteralPath $glb)) {
        & $exe --export (Join-Path $work 'export') glb $dat | Where-Object { $_ -match '^(ok|fail) ' }
        if (-not (Test-Path -LiteralPath $glb)) { throw "export of $name failed" }
    }
    # 2. roles of the original entry (head, arms, legs as joint numbers), for the script
    $roles = Join-Path $work "$name.roles.json"
    if ($Category -eq 'chara') {
        python -I (Join-Path $g 'tools/robo_joints.py') roles (Join-Path $g "unpacked/chara/$e.bin") | Set-Content -LiteralPath $roles
        if ($LASTEXITCODE) { throw "robo_joints.py roles failed for $e" }
    } else { '{"C": {}}' | Set-Content -LiteralPath $roles }
    # 3. Blender
    $edited = Join-Path $work "$name.edited.glb"
    & $Blender -b --factory-startup --python-exit-code 1 --python $py -- $glb $edited $roles 2>&1 |
        Where-Object { $_ -match '^(legionnaire|blender|[a-z_]+:)|Error|Traceback' }
    if ($LASTEXITCODE -or -not (Test-Path -LiteralPath $edited)) { throw "Blender script $Script failed for $name" }
    # 4. import over the original .dat (same skeleton) -> the mod's model
    $target = Join-Path $out "$name.dat"
    & $exe --import $dat $edited $target | Where-Object { $_ -notmatch 'flags' }
    if ($LASTEXITCODE) { throw "HSDRawViewer --import failed for $name" }
}
# 5. ISO + patch
$args5 = @((Join-Path $g 'tools/make_mod.py'), (Join-Path $g "mods/$Mod"))
if ($Iso) { $args5 += $Iso }
python -I @args5
if ($LASTEXITCODE) { throw 'make_mod.py failed' }
$modName = (Get-Content -LiteralPath (Join-Path $g "mods/$Mod/mod.json") -Raw | ConvertFrom-Json).name -replace '[^A-Za-z0-9_.-]+', '_'
$isoOut = Join-Path $g "build/mods/$modName/$modName.iso"
"ISO:     $isoOut"
"crpatch: $(Join-Path $g "mods/$Mod/$modName.crpatch")"
# 6. optional boot: Dolphin must still run after $Seconds s with no warning dialog (the picture is the tester's job)
if ($Boot) {
    pwsh -NoProfile -File (Join-Path $g 'tools/boot_check.ps1') $isoOut $Seconds
    if ($LASTEXITCODE) { throw "boot check failed (see above)" }
}
