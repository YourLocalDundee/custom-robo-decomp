#!/usr/bin/env bash
# Blender-to-game loop for one model edit (docs/MODDING.md "Blender loop"); bash twin of tools/model_loop.ps1.
#   tools/model_loop.sh <mod> <script> <entries> [--boot] [--fresh] [--iso <clean.iso>] [--category chara]
#   tools/model_loop.sh ray_legionnaire legionnaire 000,001,002,003,004
#   tools/model_loop.sh rt_test roundtrip 000                        (no edit: the pipeline itself)
# For each entry: export (HSDRawViewer --export, once) -> Blender (tools/blender/<script>.py) -> HSDRawViewer --import
# over the original .dat -> mods/<mod>/models/<category>/<name>.dat; then tools/make_mod.py -> ISO + .crpatch.
# HSDRawViewer is a Windows program: under Git Bash on Windows every step runs; on Linux / macOS the export and import
# steps are skipped (the Blender step still runs on an export copied from Windows into
# build/model_loop/<mod>/export/<name>/root0_joint.glb; bring the edited glb back to Windows for --import, or put
# finished .dat files into mods/<mod>/models/ and this script builds the ISO and patch). --boot needs Windows
# (tools/boot_check.ps1). Blender: $BLENDER, else blender on PATH, else the usual install folders.
set -euo pipefail
t=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$t")
. "$t/common.sh"
[ $# -ge 3 ] || die "usage: tools/model_loop.sh <mod> <script> <entries, e.g. 000,001> [--boot] [--fresh] [--iso X] [--category chara]"
mod=$1; script=$2; entries=$3; shift 3
boot=0; fresh=0; iso=""; category=chara; seconds=40
while [ $# -gt 0 ]; do
    case "$1" in
        --boot | -Boot) boot=1 ;;
        --fresh | -Fresh) fresh=1 ;;
        --iso | -Iso) iso=$2; shift ;;
        --category | -Category) category=$2; shift ;;
        --seconds | -Seconds) seconds=$2; shift ;;
        *) die "unknown option $1" ;;
    esac
    shift
done
py="$t/blender/$script.py"
[ -f "$py" ] || die "no Blender script tools/blender/$script.py"
exe="$t/HSDLib/HSDRawViewer/bin/Release/net8.0-windows7.0/HSDRawViewer.exe"
windows=0
case "$(uname -s)" in MINGW* | MSYS* | CYGWIN*) windows=1 ;; esac
hsd=0
if [ $windows = 1 ] && [ -f "$exe" ]; then hsd=1; fi
[ $hsd = 1 ] || echo "model_loop: HSDRawViewer (Windows only) not available here: skipping the export and import steps"
blender=${BLENDER:-}
[ -n "$blender" ] || blender=$(command -v blender || true)
if [ -z "$blender" ]; then
    for c in /Applications/Blender.app/Contents/MacOS/Blender "/c/Program Files/Blender Foundation"/Blender*/blender.exe; do
        [ -x "$c" ] && blender=$c
    done
fi
[ -n "$blender" ] || die "Blender not found: set BLENDER=<path to blender>"
work="$g/build/model_loop/$mod"
out="$g/mods/$mod/models/$category"
mkdir -p "$work" "$out"
if [ ! -f "$g/mods/$mod/mod.json" ]; then
    printf '{"name": "%s", "author": "model_loop", "description": "tools/blender/%s.py on %s %s"}\r\n' "$mod" "$script" "$category" "$entries" > "$g/mods/$mod/mod.json"
fi
# Windows programs get Windows paths (cygpath under Git Bash), everything else the plain ones
winpath() { if [ $windows = 1 ]; then cygpath -w "$1"; else printf '%s' "$1"; fi; }
for e in ${entries//,/ }; do
    name="${category}_$e"
    dat="$g/models_dat/$category/$name.dat"
    glb="$work/export/$name/root0_joint.glb"
    # 1. export, once
    if [ $hsd = 1 ] && { [ $fresh = 1 ] || [ ! -f "$glb" ]; }; then
        [ -f "$dat" ] || die "no $dat (run the one-time build first: README quick start)"
        "$exe" --export "$(winpath "$work/export")" glb "$(winpath "$dat")" | grepq -E '^(ok|fail) ' || die "export of $name failed"
    fi
    [ -f "$glb" ] || die "no export $glb (export it on Windows: pwsh tools/model_loop.ps1, or HSDRawViewer --export)"
    # 2. roles of the original entry, for the script
    roles="$work/$name.roles.json"
    if [ "$category" = chara ]; then
        "$PY" -I "$t/robo_joints.py" roles "$g/unpacked/chara/$e.bin" | crlf > "$roles"
    else
        printf '{"C": {}}\r\n' > "$roles"
    fi
    # 3. Blender
    edited="$work/$name.edited.glb"
    rm -f "$edited"
    "$blender" -b --factory-startup --python-exit-code 1 --python "$(winpath "$py")" -- \
        "$(winpath "$glb")" "$(winpath "$edited")" "$(winpath "$roles")" 2>&1 | grepq -E '^(legionnaire|blender|[a-z_]+:)|Error|Traceback' ||
        die "Blender script $script failed for $name"
    [ -f "$edited" ] || die "Blender script $script wrote no $edited"
    # 4. import over the original .dat
    if [ $hsd = 1 ]; then
        "$exe" --import "$(winpath "$dat")" "$(winpath "$edited")" "$(winpath "$out/$name.dat")" | grepq -v flags ||
            die "HSDRawViewer --import failed for $name"
    else
        echo "model_loop: $name: edited glb at $edited; import it on Windows (HSDRawViewer --import) into $out/$name.dat"
    fi
done
# 5. ISO + patch from whatever models the mod folder has
ls "$out"/*.dat >/dev/null 2>&1 || die "no models in $out yet: nothing to build"
if [ -n "$iso" ]; then "$PY" -I "$t/make_mod.py" "$g/mods/$mod" "$iso"; else "$PY" -I "$t/make_mod.py" "$g/mods/$mod"; fi
name=$("$PY" -I -c "import json,re,sys; print(re.sub(r'[^A-Za-z0-9_.-]+', '_', json.load(open(sys.argv[1], encoding='utf-8'))['name']))" "$g/mods/$mod/mod.json")
echo "ISO:     $g/build/mods/$name/$name.iso"
echo "crpatch: $g/mods/$mod/$name.crpatch"
# 6. optional boot (Windows: tools/boot_check.ps1 drives Dolphin's windows)
if [ $boot = 1 ]; then
    [ $windows = 1 ] || die "--boot needs Windows (tools/boot_check.ps1)"
    pwsh -NoProfile -File "$(winpath "$t/boot_check.ps1")" "$(winpath "$g/build/mods/$name/$name.iso")" "$seconds"
fi
