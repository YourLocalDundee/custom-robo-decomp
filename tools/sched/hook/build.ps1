# Builds the scheduler hook (schedhook.c -> lmgr326b.dll) with the Visual Studio x86 compiler and installs it in the
# analysis copy of the GC/2.6 compiler folder, tools/sched/bin (gitignored; never decomp/build/compilers).
# Usage: pwsh tools/sched/hook/build.ps1
$ErrorActionPreference = 'Stop'
$here = $PSScriptRoot
$bin = Join-Path (Split-Path $here -Parent) 'bin'
$g = Split-Path (Split-Path (Split-Path $here -Parent) -Parent) -Parent
$vs = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vs) { throw 'Visual Studio (C++ tools) not found' }
$vcvars = Join-Path $vs 'VC\Auxiliary\Build\vcvars32.bat'
New-Item -ItemType Directory -Force $bin | Out-Null
foreach ($f in 'mwcceppc.exe') {
    Copy-Item (Join-Path $g "decomp/build/compilers/GC/2.6/$f") (Join-Path $bin $f) -Force
}
$out = Join-Path $bin 'lmgr326b.dll'
$obj = Join-Path $env:TEMP 'schedhook.obj'
cmd /c "`"$vcvars`" >nul && cl /nologo /LD /O1 /W3 /MT /D_CRT_SECURE_NO_WARNINGS `"$here\schedhook.c`" /Fo`"$obj`" /Fe`"$out`" /link /DEF:`"$here\schedhook.def`" kernel32.lib"
if ($LASTEXITCODE) { throw 'build failed' }
Remove-Item (Join-Path $bin 'lmgr326b.lib'), (Join-Path $bin 'lmgr326b.exp') -ErrorAction SilentlyContinue
"built $out"
