#Requires -Version 7
# Phase 4 build setup (plan.md Phase 4, docs/REPRODUCE.md "Build main.dol from asm"): makes decomp/ buildable.
#   1. encounter/dtk-template at the commit pinned in tools/pins.txt -> tools/refs/dtk-template; its configure.py and
#      tools/*.py are copied into decomp/ byte for byte and configure.py gets tools/patches/dtk_template_configure.patch.
#      (Only decomp/config/ is ours and committed; the copied files are gitignored and rewritten on every run.)
#   2. The build tools the template names, at its versions, SHA-256 checked (pins.txt): ninja -> tools/ninja/,
#      objdiff-cli, sjiswrap (and wibo on Linux / macOS) -> decomp/build/tools/, binutils -> decomp/build/binutils/,
#      the CodeWarrior compilers -> decomp/build/compilers/ (GC/1.0 .. 3.0a5.2, Wii, ProDG). dtk is tools/dtk/ (setup.ps1).
#   3. YOUR main.dol: "Custom Robo extracted/sys/main.dol" (setup.ps1 step 2), SHA-1 checked -> decomp/orig/GXCE01/sys/.
# Needs setup.ps1 to have run (disc extracted, dtk downloaded). Rerunnable: downloads and extractions are skipped
# when done; the template files and the DOL copy are refreshed every time.
# Usage:  pwsh tools/setup_decomp.ps1
$ErrorActionPreference = 'Stop'
$g = Split-Path $PSScriptRoot -Parent
$t = $PSScriptRoot
$dl = "$t/downloads"
$d = "$g/decomp"
New-Item -ItemType Directory -Force $dl, "$d/build/tools", "$d/orig/GXCE01/sys" | Out-Null

$pins = @{}
Get-Content -LiteralPath "$t/pins.txt" | Where-Object { $_ -notmatch '^\s*(#|$)' } | ForEach-Object {
    $k, $n, $src, $ver = -split $_
    $pins[$n] = @{ kind = $k; src = $src; ver = $ver }
}
function PinnedFile($name, $out) {
    $p = $pins[$name]
    if (-not $p) { throw "no pin '$name' in tools/pins.txt" }
    if (-not (Test-Path -LiteralPath $out)) { "downloading $name"; Invoke-WebRequest $p.src -OutFile $out }
    $h = (Get-FileHash -LiteralPath $out -Algorithm SHA256).Hash.ToLower()
    if ($h -ne $p.ver) { Remove-Item -LiteralPath $out; throw "$name sha256 $h does not match tools/pins.txt (deleted, re-run to retry)" }
    "$name sha256 ok"
}
function ExtractZip($zip, $dest) {
    New-Item -ItemType Directory -Force $dest | Out-Null
    if ($IsWindows) { & "$env:SystemRoot/System32/tar.exe" -xf $zip -C $dest }   # Windows' own tar (reads zip), not an MSYS one else { unzip -q -o $zip -d $dest }
    if ($LASTEXITCODE) { throw "extracting $zip failed" }
}
# a pinned single-file tool, copied into place (and made executable off Windows)
function PinnedTool($name, $out) {
    PinnedFile $name "$dl/$name"
    if (-not (Test-Path -LiteralPath $out)) { Copy-Item -LiteralPath "$dl/$name" $out; if (-not $IsWindows) { chmod +x $out } }
}
# a pinned zip, extracted once into $dest (marker file = done)
function PinnedZip($name, $dest) {
    PinnedFile $name "$dl/$name"
    if (-not (Test-Path -LiteralPath "$dest/.pinned_$name")) {
        ExtractZip "$dl/$name" $dest
        if (-not $IsWindows) { Get-ChildItem -LiteralPath $dest -Recurse -File | ForEach-Object { chmod +x $_.FullName } }
        Set-Content -LiteralPath "$dest/.pinned_$name" -Value $pins[$name].ver
    }
}

# 1. the template, checked out without line-ending conversion (the patch is against the exact bytes)
$tpl = "$t/refs/dtk-template"
$p = $pins['dtk-template']
if (-not (Test-Path -LiteralPath $tpl)) {
    git init -q $tpl
    git -C $tpl config core.autocrlf false
    git -C $tpl config core.eol lf   # the template's .gitattributes marks *.py as text: without this Windows checks it out CRLF
    git -C $tpl remote add origin $p.src
    git -C $tpl fetch -q --depth 1 origin $p.ver
    git -C $tpl checkout -q FETCH_HEAD
    if ($LASTEXITCODE) { throw 'dtk-template checkout failed' }
}
$head = git -C $tpl rev-parse HEAD
if ($head -ne $p.ver) { throw "tools/refs/dtk-template is at $head, pins.txt says $($p.ver): delete tools/refs/dtk-template and re-run" }
New-Item -ItemType Directory -Force "$d/tools" | Out-Null
foreach ($f in (git -C $tpl ls-files configure.py LICENSE 'tools/*.py')) {
    $dst = if ($f -eq 'LICENSE') { "$d/tools/LICENSE.dtk-template" } else { "$d/$f" }
    Copy-Item -LiteralPath "$tpl/$f" $dst -Force
}
Push-Location $d
# the template clone as git dir and decomp/ as work tree: paths in the patch are then relative to decomp/
try { git --git-dir="$tpl/.git" --work-tree=. apply "$t/patches/dtk_template_configure.patch" } finally { Pop-Location }
if ($LASTEXITCODE) { throw 'tools/patches/dtk_template_configure.patch does not apply to the pinned template' }
if (-not (Select-String -LiteralPath "$d/configure.py" -Pattern 'Custom Robo' -Quiet)) { throw 'configure.py was not patched' }
"dtk-template at $head, configure.py patched"

# 2. build tools for this OS
$cpu = [System.Runtime.InteropServices.RuntimeInformation]::OSArchitecture.ToString()
if ($IsWindows) {
    if ($cpu -ne 'X64') { throw "no pinned build tools for Windows $cpu" }
    $ninja = 'ninja-win.zip'; $objdiff = 'objdiff-cli-windows-x86_64.exe'; $binutils = 'binutils-windows-x86_64.zip'; $wibo = $null
} elseif ($IsMacOS) {
    $ninja = 'ninja-mac.zip'; $objdiff = if ($cpu -eq 'Arm64') { 'objdiff-cli-macos-arm64' } else { 'objdiff-cli-macos-x86_64' }
    $binutils = 'binutils-macos-universal.zip'; $wibo = 'wibo-macos'
} else {
    switch ($cpu) {
        'X64' { $ninja = 'ninja-linux.zip'; $objdiff = 'objdiff-cli-linux-x86_64'; $binutils = 'binutils-linux-x86_64.zip'; $wibo = 'wibo-x86_64' }
        'Arm64' { $ninja = 'ninja-linux-aarch64.zip'; $objdiff = 'objdiff-cli-linux-aarch64'; $binutils = 'binutils-linux-aarch64.zip'; $wibo = $null }
        default { throw "no pinned build tools for Linux $cpu" }
    }
}
$exe = if ($IsWindows) { '.exe' } else { '' }
PinnedZip $ninja "$t/ninja"
PinnedTool $objdiff "$d/build/tools/objdiff-cli$exe"
PinnedTool 'sjiswrap.exe' "$d/build/tools/sjiswrap.exe"
if ($wibo) { PinnedTool $wibo "$d/build/tools/wibo" }
elseif (-not $IsWindows) { "note: no pinned wibo for $cpu; the compilers will run through wine (install it, docs/INSTALL.md)" }
PinnedZip $binutils "$d/build/binutils"
PinnedZip 'compilers_20251118.zip' "$d/build/compilers"
$dtk = "$t/dtk/dtk$exe"
if (-not (Test-Path -LiteralPath $dtk)) { throw "$dtk missing: run tools/setup.ps1 first" }

# 3. your main.dol
$src = "$g/Custom Robo extracted/sys/main.dol"
if (-not (Test-Path -LiteralPath $src)) { throw "$src missing: run tools/setup.ps1 -Iso <your ISO> first" }
$sha1 = (Get-FileHash -LiteralPath $src -Algorithm SHA1).Hash.ToLower()
if ($sha1 -ne '6a73cd111ef349b20be294511b8c51a8addfb50b') { throw "main.dol SHA-1 $sha1 is not the GXCE01 v1.0 retail one" }
Copy-Item -LiteralPath $src "$d/orig/GXCE01/sys/main.dol" -Force
"main.dol ok -> decomp/orig/GXCE01/sys/main.dol"
"decomp setup done. Build:  cd decomp; python configure.py; ../tools/ninja/ninja$exe   (or pwsh tools/batches/batch9_decomp_build.ps1)"
