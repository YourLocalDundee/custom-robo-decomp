# One-time setup: download every third-party tool and reference at the exact version in tools\pins.txt
# (each download is SHA-256 checked), then extract YOUR copy of the game. Every step: docs\REPRODUCE.md.
# Usage:  pwsh tools\setup.ps1 -Iso "C:\path\to\Custom Robo (USA).iso" [-Dolphin "C:\your\Dolphin folder"]
#   -Dolphin is optional: without it, the official Dolphin 2609a build is downloaded just for DolphinTool.exe.
# Needs on PATH: git, python (3.11+), dotnet (SDK 8+), java (21+), tar (built into Windows 10/11).
# Re-running skips finished steps.
param([Parameter(Mandatory)][string]$Iso, [string]$Dolphin)
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t = $PSScriptRoot
$dl = "$t\downloads"

# 0. Prerequisites
foreach ($c in 'git', 'python', 'dotnet', 'java', 'tar') {
    if (-not (Get-Command $c -ErrorAction SilentlyContinue)) { throw "'$c' not found on PATH (see README, Quick start)" }
}
New-Item -ItemType Directory -Force $dl | Out-Null

# tools\pins.txt: "kind name source version-or-sha256"
$pins = @{}
Get-Content -LiteralPath "$t\pins.txt" | Where-Object { $_ -notmatch '^\s*(#|$)' } | ForEach-Object {
    $k, $n, $src, $ver = -split $_
    $pins[$n] = @{ kind = $k; src = $src; ver = $ver }
}

function PinnedClone($name, $dir, [string[]]$sparse) {
    $p = $pins[$name]
    if (Test-Path -LiteralPath $dir) { "skip $dir (exists)"; return }
    git init -q $dir
    git -C $dir remote add origin $p.src
    if ($sparse) { git -C $dir sparse-checkout set --no-cone @sparse }
    git -C $dir fetch -q --depth 1 --filter=blob:none origin $p.ver
    git -C $dir checkout -q FETCH_HEAD
    "$name at $($p.ver)"
}

# download once into tools\downloads\, then always verify the SHA-256 from pins.txt
function PinnedFile($name, $out) {
    $p = $pins[$name]
    if (-not (Test-Path -LiteralPath $out)) { "downloading $name"; Invoke-WebRequest $p.src -OutFile $out }
    $h = (Get-FileHash -LiteralPath $out -Algorithm SHA256).Hash.ToLower()
    if ($h -ne $p.ver) { Remove-Item -LiteralPath $out; throw "$name sha256 $h does not match tools\pins.txt (deleted, re-run to retry)" }
    "$name sha256 ok"
}

# 1. Check the ISO is the clean US retail dump (GXCE01 v1.0) before touching it.
$sha1 = (Get-FileHash -LiteralPath $Iso -Algorithm SHA1).Hash.ToLower()
if ($sha1 -ne 'fff6e74590366a5c0c5ba0d40c8ccf94770bcb16') { throw "ISO SHA-1 $sha1 is not the GXCE01 v1.0 retail dump" }
"ISO ok (GXCE01 v1.0)"

# 2. Extract the disc with Dolphin's own DolphinTool (yours, or the official pinned build).
if (-not $Dolphin) {
    $Dolphin = "$t\dolphin\Dolphin-x64"
    if (-not (Test-Path -LiteralPath "$Dolphin\DolphinTool.exe")) {
        PinnedFile dolphin.7z "$dl\dolphin.7z"
        New-Item -ItemType Directory -Force "$t\dolphin" | Out-Null
        tar -xf "$dl\dolphin.7z" -C "$t\dolphin"
    }
}
if (-not (Test-Path -LiteralPath "$g\Custom Robo extracted")) {
    & "$Dolphin\DolphinTool.exe" extract -i $Iso -o "$g\Custom Robo extracted"
    if ($LASTEXITCODE) { throw 'DolphinTool extract failed' }
}

# 3. Reference symbol lists (names + sizes only) from public GameCube decomps.
# Melee also: its HSD library + SDK headers (types and prototypes, parsed by ParseHeaders.java)
PinnedClone melee "$t\melee" @('/config/', '/src/sysdolphin/', '/src/Runtime/', '/src/MSL/',
    '/libs/doldecomp/include/', '/libs/dolphin/include/')
foreach ($r in 'mkdd', 'pikmin2', 'ttyd', 'tww', 'tp') {
    PinnedClone $r "$t\refs\$r" @('/config/*/symbols.txt', '/config/*/splits.txt')
}

# 4. HSDLib (HSDRawViewer) at the pinned commit, plus our headless export/resave patch (readable: tools\patches\).
if (-not (Test-Path -LiteralPath "$t\HSDLib")) {
    PinnedClone HSDLib "$t\HSDLib" $null
    git -C "$t\HSDLib" apply "$t\patches\hsdlib_headless_export_resave.patch"
    # RollForward=Major: HSDRawViewer targets .NET 8; this lets it run on any newer installed .NET too
    dotnet build "$t\HSDLib\HSDRawViewer" -c Release -v quiet -nologo -p:RollForward=Major
    if ($LASTEXITCODE) { throw 'HSDRawViewer build failed' }
}

# 5. decomp-toolkit, hash-checked; its own analysis of main.dol (function boundaries, SDK signature names).
PinnedFile dtk.exe "$t\dtk\dtk.exe"
Copy-Item -LiteralPath "$g\Custom Robo extracted\sys\main.dol" "$t\dtk\proj\main.dol" -Force
if (-not (Test-Path -LiteralPath "$t\dtk\proj\symbols.txt")) {
    Push-Location "$t\dtk\proj"
    try { & "$t\dtk\dtk.exe" dol split config.yml out } finally { Pop-Location }
    if ($LASTEXITCODE) { throw 'dtk dol split failed' }
}

# 6. Python environment for model previews, exact package versions.
if (-not (Test-Path -LiteralPath "$t\pyenv")) {
    python -m venv "$t\pyenv"
    & "$t\pyenv\Scripts\python" -m pip install -q --disable-pip-version-check -r "$t\requirements-preview.txt"
    if ($LASTEXITCODE) { throw 'pip install failed' }
}

# 7. Ghidra 12.1.4 (hash published by the Ghidra project) + GameCube loader 1.3.1.
if (-not (Test-Path -LiteralPath "$t\ghidra_12.1.4_PUBLIC")) {
    PinnedFile ghidra.zip "$dl\ghidra.zip"
    tar -xf "$dl\ghidra.zip" -C $t
}
$ext = "$t\ghidra_12.1.4_PUBLIC\Ghidra\Extensions"
if (-not (Test-Path -LiteralPath "$ext\GameCubeLoader")) {
    PinnedFile gcloader.zip "$dl\gcloader.zip"
    tar -xf "$dl\gcloader.zip" -C $ext
    # the loader zip is built for Ghidra 12.1; Ghidra refuses it on 12.1.4 unless the version line matches
    $props = "$ext\GameCubeLoader\extension.properties"
    (Get-Content -LiteralPath $props) -replace '^version=.*', 'version=12.1.4' | Set-Content -LiteralPath $props
}
"setup done. Next:  pwsh tools\batches\run_all.ps1   then   pwsh tools\checksums.ps1 -Compare"
