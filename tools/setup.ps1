#Requires -Version 7
# One-time setup: download every third-party tool and reference at the exact version in tools\pins.txt
# (each download is SHA-256 checked), then extract YOUR copy of the game. Every step: docs\REPRODUCE.md.
# Usage:  pwsh tools\setup.ps1 -Iso "C:\path\to\Custom Robo (USA).iso" [-Dolphin "C:\your\Dolphin folder"]
#   -Dolphin is optional: without it, the official Dolphin 2609a build is downloaded just for DolphinTool.exe.
# Needs on PATH: git, python (3.11+), java (21+), dotnet (SDK 8+) and tar (built in); see docs/INSTALL.md.
# Windows script (PowerShell 7). Linux / macOS: tools/setup.sh (bash; no PowerShell needed there).
# Re-running skips finished steps.
param([Parameter(Mandatory)][string]$Iso, [string]$Dolphin)
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t = $PSScriptRoot
$dl = "$t/downloads"

# 0. Prerequisites: print every tool's version, collect everything that is missing or too old, stop once with the list.
$problems = @()
function ToolVersion([string]$cmd, [string[]]$a) {
    $old = $ErrorActionPreference; $ErrorActionPreference = 'Continue'
    try { (& $cmd @a 2>&1 | Select-Object -First 1 | ForEach-Object { "$_" }) } finally { $ErrorActionPreference = $old }
}
function NeedTool([string]$cmd, [string[]]$a, [scriptblock]$ok, [string]$want, [string]$hint) {
    if (-not (Get-Command $cmd -ErrorAction SilentlyContinue)) { "  MISSING  $cmd (need $want)"; $script:problems += "${cmd}: $hint"; return }
    $v = ToolVersion $cmd $a
    if (& $ok $v) { "  ok       {0,-8} {1}" -f $cmd, $v } else { "  TOO OLD  {0,-8} {1} (need $want)" -f $cmd, $v; $script:problems += "${cmd}: $hint" }
}
"checking tools:"
if ($PSVersionTable.PSVersion.Major -ge 7) { "  ok       pwsh     $($PSVersionTable.PSVersion)" } else { "  TOO OLD  PowerShell $($PSVersionTable.PSVersion) (need 7+)"; $problems += 'PowerShell 7+: https://aka.ms/powershell' }
NeedTool git @('--version') { param($v) $v -match 'git version' } 'any' 'install git'
NeedTool python @('--version') { param($v) $v -match 'Python 3\.(\d+)' -and [int]$Matches[1] -ge 11 } '3.11+' 'install Python 3.11+ and make the command "python" work (Debian/Ubuntu: python-is-python3; macOS: add brew python''s libexec/bin to PATH)'
NeedTool java @('-version') { param($v) $v -match '"(\d+)' -and [int]$Matches[1] -ge 21 } '21+' 'install a JDK 21+ (Ghidra 12.1 needs it)'
if ($IsWindows) {
    NeedTool dotnet @('--version') { param($v) $v -match '^(\d+)\.' -and [int]$Matches[1] -ge 8 } 'SDK 8+' 'install the .NET SDK 8 or newer (builds HSDRawViewer)'
    NeedTool tar @('--version') { param($v) $true } 'any (built into Windows 10/11)' 'tar.exe ships with Windows 10/11'
} else {
    NeedTool unzip @('-v') { param($v) $true } 'any' 'install unzip'
    "  note     model export (HSDRawViewer) is Windows-only; the decompile/naming/mod pipeline works here"
}
if ($problems) { throw ("missing tools, install them (README: Install the tools) and re-run:`n  " + ($problems -join "`n  ")) }
New-Item -ItemType Directory -Force $dl | Out-Null

# tools\pins.txt: "kind name source version-or-sha256"
$pins = @{}
Get-Content -LiteralPath "$t/pins.txt" | Where-Object { $_ -notmatch '^\s*(#|$)' } | ForEach-Object {
    $k, $n, $src, $ver = -split $_
    $pins[$n] = @{ kind = $k; src = $src; ver = $ver }
}

function PinnedClone($name, $dir, [string[]]$sparse) {
    $p = $pins[$name]
    if (Test-Path -LiteralPath $dir) {
        # an existing clone gets this script's sparse paths (a list widened later, e.g. tp / pikmin2 in step 68)
        if ($sparse) { git -C $dir sparse-checkout set --no-cone @sparse }
        "skip $dir (exists)"; return
    }
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

# unzip a .zip: Windows' tar reads zips; elsewhere use unzip (keeps the executable bits Ghidra's scripts need)
function ExtractZip($zip, $dest) {
    if ($IsWindows) { tar -xf $zip -C $dest } else { unzip -q -o $zip -d $dest }
    if ($LASTEXITCODE) { throw "extracting $zip failed" }
}

# 1. Check the ISO is the clean US retail dump (GXCE01 v1.0) before touching it.
$sha1 = (Get-FileHash -LiteralPath $Iso -Algorithm SHA1).Hash.ToLower()
if ($sha1 -ne 'fff6e74590366a5c0c5ba0d40c8ccf94770bcb16') { throw "ISO SHA-1 $sha1 is not the GXCE01 v1.0 retail dump" }
"ISO ok (GXCE01 v1.0)"

# 2. Extract the disc with Dolphin's own DolphinTool (yours, or on Windows the official pinned build).
# $dtool = the command to run (array: a flatpak wrapper needs several words).
$dtool = $null
if ($Dolphin) {
    if (Test-Path -LiteralPath $Dolphin -PathType Leaf) { $dtool = @($Dolphin) }
    else {
        $c = 'DolphinTool.exe', 'DolphinTool', 'dolphin-tool', 'Contents/MacOS/DolphinTool' | ForEach-Object { Join-Path $Dolphin $_ } | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } | Select-Object -First 1
        if (-not $c) { throw "no DolphinTool found in '$Dolphin'" }
        $dtool = @($c)
    }
} elseif ($IsWindows) {
    $exe = "$t/dolphin/Dolphin-x64/DolphinTool.exe"
    if (-not (Test-Path -LiteralPath $exe) -and -not (Test-Path -LiteralPath "$g/Custom Robo extracted")) {
        PinnedFile dolphin.7z "$dl/dolphin.7z"
        New-Item -ItemType Directory -Force "$t/dolphin" | Out-Null
        tar -xf "$dl/dolphin.7z" -C "$t/dolphin"
    }
    $dtool = @($exe)
} else {
    # Linux / macOS: the Dolphin you installed (package manager, or flatpak). See docs/INSTALL.md.
    $c = Get-Command dolphin-tool, DolphinTool -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($c) { $dtool = @($c.Source) }
    elseif (Test-Path -LiteralPath '/Applications/Dolphin.app/Contents/MacOS/DolphinTool') { $dtool = @('/Applications/Dolphin.app/Contents/MacOS/DolphinTool') }
    elseif ((Get-Command flatpak -ErrorAction SilentlyContinue) -and (flatpak info org.DolphinEmu.dolphin-emu 2>$null)) {
        $dtool = @('flatpak', 'run', '--filesystem=host', '--command=dolphin-tool', 'org.DolphinEmu.dolphin-emu')
    }
    if (-not $dtool -and -not (Test-Path -LiteralPath "$g/Custom Robo extracted")) {
        throw "dolphin-tool not found. Install Dolphin (docs/INSTALL.md) or pass -Dolphin <folder or file of DolphinTool>"
    }
}
if (-not (Test-Path -LiteralPath "$g/Custom Robo extracted")) {
    & $dtool[0] @($dtool | Select-Object -Skip 1) extract -i $Iso -o "$g/Custom Robo extracted"
    if ($LASTEXITCODE) { throw 'DolphinTool extract failed' }
}

# 3. Reference symbol lists (names + sizes only) from public GameCube decomps.
# Melee also: its HSD library + SDK headers (types and prototypes, parsed by ParseHeaders.java)
PinnedClone melee "$t/melee" @('/config/', '/src/sysdolphin/', '/src/Runtime/', '/src/MSL/', '/src/MetroTRK/',
    '/libs/doldecomp/include/', '/libs/dolphin/include/')
# (MetroTRK/: the TRK sources, and MSL's math.h includes MetroTRK/intrinsics.h; tools/compiler_probe compiles them.
#  An older clone without it: git -C tools/melee sparse-checkout add /src/MetroTRK/)
foreach ($r in 'mkdd', 'ttyd', 'tww') {
    PinnedClone $r "$t/refs/$r" @('/config/*/symbols.txt', '/config/*/splits.txt')
}
# Twilight Princess / Pikmin 2: also their MetroTRK v2.6, MSL and SDK sources (docs/compiler.md "Source hunt"; the
# MetroTRK objects and FILE_POS.c are imported from them by tools/import_ref_source.py, tools/compiler_probe/sources.py)
PinnedClone tp "$t/refs/tp" @('/config/*/symbols.txt', '/config/*/splits.txt', '/include/', '/libs/TRK_MINNOW_DOLPHIN/',
    '/libs/PowerPC_EABI_Support/', '/libs/dolphin/', '/configure.py')
PinnedClone pikmin2 "$t/refs/pikmin2" @('/config/*/symbols.txt', '/config/*/splits.txt', '/include/', '/src/Dolphin/',
    '/configure.py')
# Library-naming references (tools/match_lib_*.py): names, sizes, and source text for call graphs / asserts / strings.
# Only text is read; nothing from these repos is built or run.
foreach ($r in 'kar_wowjinxy', 'xd', 'dolsdk2001', 'dolsdk2004', 'ac-decomp', 'sms', 'marioparty4', 'pikmin', 'bfbb') {
    PinnedClone $r "$t/refs/$r" @('/config/', '/src/', '/include/', '/libs/')
}
# doldecomp/kar: its asm/ is exact Kirby Air Ride code (calls, assert and string data) for HSD functions without C yet
PinnedClone kar "$t/refs/kar" @('/config/', '/src/', '/include/', '/libs/', '/asm/')

# 4. HSDLib (HSDRawViewer) at the pinned commit, plus our headless export/resave patch (readable: tools\patches\).
if (-not $IsWindows) {
    "skip HSDLib/HSDRawViewer: it is a Windows program (model export to glTF needs Windows; everything else works here)"
} elseif (-not (Test-Path -LiteralPath "$t/HSDLib")) {
    PinnedClone HSDLib "$t/HSDLib" $null
    git -C "$t/HSDLib" apply "$t/patches/hsdlib_headless_export_resave.patch"
    # RollForward=Major: HSDRawViewer targets .NET 8; this lets it run on any newer installed .NET too
    dotnet build "$t/HSDLib/HSDRawViewer" -c Release -v quiet -nologo -p:RollForward=Major
    if ($LASTEXITCODE) { throw 'HSDRawViewer build failed' }
}

# 4b. m2c (matt-kempster/m2c) at the pinned commit: C drafts of the game code (tools/draft_batch.py, batch 12). Plain Python, no install.
PinnedClone m2c "$t/m2c" $null

# 5. decomp-toolkit, hash-checked; its own analysis of main.dol (function boundaries, SDK signature names).
# the pin for this OS: dtk.exe (Windows x64), dtk-linux-x86_64 / dtk-linux-aarch64, dtk-macos-x86_64 / dtk-macos-arm64
$cpu = [System.Runtime.InteropServices.RuntimeInformation]::OSArchitecture.ToString()
if ($IsWindows) { $dtkPin = 'dtk.exe'; $dtk = "$t/dtk/dtk.exe"; if ($cpu -ne 'X64') { throw "no pinned dtk for Windows $cpu" } }
else {
    $os = if ($IsMacOS) { 'macos' } else { 'linux' }
    $a = switch ($cpu) { 'X64' { 'x86_64' } 'Arm64' { if ($IsMacOS) { 'arm64' } else { 'aarch64' } } default { throw "no pinned dtk for $os $cpu" } }
    $dtkPin = "dtk-$os-$a"; $dtk = "$t/dtk/dtk"
}
New-Item -ItemType Directory -Force "$t/dtk/proj" | Out-Null
PinnedFile $dtkPin $dtk
if (-not $IsWindows) { chmod +x $dtk }
Copy-Item -LiteralPath "$g/Custom Robo extracted/sys/main.dol" "$t/dtk/proj/main.dol" -Force
if (-not (Test-Path -LiteralPath "$t/dtk/proj/symbols.txt")) {
    Push-Location "$t/dtk/proj"
    try { & $dtk dol split config.yml out } finally { Pop-Location }
    if ($LASTEXITCODE) { throw 'dtk dol split failed' }
}

# 6. Python environment for model previews, exact package versions.
if (-not (Test-Path -LiteralPath "$t/pyenv")) {
    python -m venv "$t/pyenv"
    $vpy = if ($IsWindows) { "$t/pyenv/Scripts/python" } else { "$t/pyenv/bin/python" }
    & $vpy -m pip install -q --disable-pip-version-check -r "$t/requirements-preview.txt"
    if ($LASTEXITCODE) { throw 'pip install failed' }
}

# 7. Ghidra 12.1.4 (hash published by the Ghidra project) + GameCube loader 1.3.1.
if (-not (Test-Path -LiteralPath "$t/ghidra_12.1.4_PUBLIC")) {
    PinnedFile ghidra.zip "$dl/ghidra.zip"
    ExtractZip "$dl/ghidra.zip" $t
}
$ext = "$t/ghidra_12.1.4_PUBLIC/Ghidra/Extensions"
if (-not (Test-Path -LiteralPath "$ext/GameCubeLoader")) {
    PinnedFile gcloader.zip "$dl/gcloader.zip"
    ExtractZip "$dl/gcloader.zip" $ext
    # the loader zip is built for Ghidra 12.1; Ghidra refuses it on 12.1.4 unless the version line matches
    $props = "$ext/GameCubeLoader/extension.properties"
    (Get-Content -LiteralPath $props) -replace '^version=.*', 'version=12.1.4' | Set-Content -LiteralPath $props
}
# 8. Phase 4 build (decomp/): dtk-template, ninja, objdiff, binutils, the CodeWarrior compilers, your main.dol.
& "$t/setup_decomp.ps1"
"setup done. Next:  pwsh tools\batches\run_all.ps1   then   pwsh tools\checksums.ps1 -Compare"
