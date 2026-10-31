#!/usr/bin/env bash
# One-time setup (Linux / macOS; Windows: tools/setup.ps1): download every third-party tool and reference at the exact
# version in tools/pins.txt (each download is SHA-256 checked), then extract YOUR copy of the game. Every step:
# docs/REPRODUCE.md. Same steps and outputs as setup.ps1; the disc is extracted by the pinned decomp-toolkit
# (byte-identical to DolphinTool's output), so Dolphin is not needed.
# Usage:  tools/setup.sh "/path/to/Custom Robo (USA).iso" [DolphinTool file, or a Dolphin folder holding one]
#   The second argument is optional: with it, that DolphinTool extracts the disc instead of dtk.
# Needs on PATH: git, python3 (3.11+), java (21+), curl, unzip (see docs/INSTALL.md). Re-running skips finished steps.
set -euo pipefail
t=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$t")
. "$t/common.sh"
dl="$t/downloads"
[ $# -ge 1 ] || die 'usage: tools/setup.sh "/path/to/Custom Robo (USA).iso" [DolphinTool]'
iso=$1
dolphin=${2:-}

# 0. Prerequisites: print every tool's version, collect everything that is missing or too old, stop once with the list.
problems=()
tool_version() { "$@" 2>&1 | head -n 1 || true; }
need_tool() {   # need_tool <cmd> <version args> <want> <hint> <check: a regex the version line must match>
    local cmd=$1 args=$2 want=$3 hint=$4 check=$5 v
    if ! command -v "$cmd" >/dev/null 2>&1; then echo "  MISSING  $cmd (need $want)"; problems+=("$cmd: $hint"); return; fi
    # shellcheck disable=SC2086
    v=$(tool_version "$cmd" $args)
    if printf '%s' "$v" | grep -E -q "$check"; then printf '  ok       %-8s %s\n' "$cmd" "$v"
    else printf '  TOO OLD  %-8s %s (need %s)\n' "$cmd" "$v" "$want"; problems+=("$cmd: $hint"); fi
}
echo "checking tools:"
printf '  ok       bash     %s\n' "$BASH_VERSION"
need_tool git --version any 'install git' 'git version'
need_tool "$PY" --version '3.11+' 'install Python 3.11+ (python3)' 'Python 3\.([1-9][1-9]|[2-9][0-9])'
need_tool java -version '21+' 'install a JDK 21+ (Ghidra 12.1 needs it; docs/INSTALL.md)' '"(2[1-9]|[3-9][0-9])'
need_tool curl --version any 'install curl' 'curl'
need_tool unzip -v any 'install unzip' '.'
echo "  note     model export (HSDRawViewer) is Windows-only; the decompile/naming/mod pipeline works here"
if [ ${#problems[@]} -gt 0 ]; then
    printf 'missing tools, install them (README: Install the tools) and re-run:\n' >&2
    printf '  %s\n' "${problems[@]}" >&2
    exit 1
fi
mkdir -p "$dl"

# 1. Check the ISO is the clean US retail dump (GXCE01 v1.0) before touching it.
sha1=$(sha1_of "$iso")
[ "$sha1" = fff6e74590366a5c0c5ba0d40c8ccf94770bcb16 ] || die "ISO SHA-1 $sha1 is not the GXCE01 v1.0 retail dump"
echo "ISO ok (GXCE01 v1.0)"

# 2. decomp-toolkit, hash-checked (the pin for this OS / CPU: dtk-linux-x86_64, dtk-linux-aarch64, dtk-macos-x86_64,
# dtk-macos-arm64); it extracts the disc here (step 2b) and analyses main.dol (step 5).
dtk="$t/dtk/dtk"
mkdir -p "$t/dtk/proj"
pinned_file "$(dtk_pin)" "$dtk"
chmod +x "$dtk"

# 2b. Extract the disc: dtk's "disc extract" writes the same sys/ + files/ layout as DolphinTool, byte for byte
# (checked: all 920 files identical, Oct 29, 2026). With a DolphinTool given, that one is used instead, as setup.ps1.
ext="$g/Custom Robo extracted"
if [ ! -d "$ext" ]; then
    if [ -n "$dolphin" ]; then
        if [ -f "$dolphin" ]; then dtool=("$dolphin")
        else
            dtool=()
            for c in DolphinTool.exe DolphinTool dolphin-tool Contents/MacOS/DolphinTool; do
                if [ -f "$dolphin/$c" ]; then dtool=("$dolphin/$c"); break; fi
            done
            [ ${#dtool[@]} -gt 0 ] || die "no DolphinTool found in '$dolphin'"
        fi
        "${dtool[@]}" extract -i "$iso" -o "$ext" || die 'DolphinTool extract failed'
    else
        rm -rf "$ext.part"
        if "$dtk" disc extract -q "$iso" "$ext.part"; then mv "$ext.part" "$ext"
        else
            # fallback: Dolphin's own tool, if installed (package, Dolphin.app, or the flatpak)
            rm -rf "$ext.part"
            dtool=()
            if command -v dolphin-tool >/dev/null 2>&1; then dtool=("$(command -v dolphin-tool)")
            elif command -v DolphinTool >/dev/null 2>&1; then dtool=("$(command -v DolphinTool)")
            elif [ -x /Applications/Dolphin.app/Contents/MacOS/DolphinTool ]; then dtool=(/Applications/Dolphin.app/Contents/MacOS/DolphinTool)
            elif command -v flatpak >/dev/null 2>&1 && flatpak info org.DolphinEmu.dolphin-emu >/dev/null 2>&1; then
                dtool=(flatpak run --filesystem=host --command=dolphin-tool org.DolphinEmu.dolphin-emu)
            fi
            [ ${#dtool[@]} -gt 0 ] || die "dtk disc extract failed and no dolphin-tool found (docs/INSTALL.md), or pass a DolphinTool as the second argument"
            echo "dtk disc extract failed: extracting with ${dtool[*]}"
            "${dtool[@]}" extract -i "$iso" -o "$ext" || die 'DolphinTool extract failed'
        fi
    fi
fi
[ -f "$ext/sys/main.dol" ] || die "$ext/sys/main.dol missing after extraction"

# 3. Reference symbol lists (names + sizes only) from public GameCube decomps.
# Melee also: its HSD library + SDK headers (types and prototypes, parsed by ParseHeaders.java)
pinned_clone melee "$t/melee" /config/ /src/sysdolphin/ /src/Runtime/ /src/MSL/ /src/MetroTRK/ \
    /libs/doldecomp/include/ /libs/dolphin/include/
# (MetroTRK/: the TRK sources, and MSL's math.h includes MetroTRK/intrinsics.h; tools/compiler_probe compiles them.
#  An older clone without it: git -C tools/melee sparse-checkout add /src/MetroTRK/)
for r in mkdd ttyd tww; do
    pinned_clone "$r" "$t/refs/$r" '/config/*/symbols.txt' '/config/*/splits.txt'
done
# Twilight Princess / Pikmin 2: also their MetroTRK v2.6, MSL and SDK sources (docs/compiler.md "Source hunt"; the
# MetroTRK objects and FILE_POS.c are imported from them by tools/import_ref_source.py, tools/compiler_probe/sources.py)
pinned_clone tp "$t/refs/tp" '/config/*/symbols.txt' '/config/*/splits.txt' /include/ /libs/TRK_MINNOW_DOLPHIN/ \
    /libs/PowerPC_EABI_Support/ /libs/dolphin/ /configure.py
pinned_clone pikmin2 "$t/refs/pikmin2" '/config/*/symbols.txt' '/config/*/splits.txt' /include/ /src/Dolphin/ /configure.py
# Library-naming references (tools/match_lib_*.py): names, sizes, and source text for call graphs / asserts / strings.
# Only text is read; nothing from these repos is built or run.
for r in kar_wowjinxy xd dolsdk2001 dolsdk2004 ac-decomp sms marioparty4 pikmin bfbb; do
    pinned_clone "$r" "$t/refs/$r" /config/ /src/ /include/ /libs/
done
# doldecomp/kar: its asm/ is exact Kirby Air Ride code (calls, assert and string data) for HSD functions without C yet
pinned_clone kar "$t/refs/kar" /config/ /src/ /include/ /libs/ /asm/

# 4. HSDLib (HSDRawViewer): a Windows program (model export to glTF needs Windows; everything else works here).
echo "skip HSDLib/HSDRawViewer: it is a Windows program (model export to glTF needs Windows; everything else works here)"

# 4b. m2c (matt-kempster/m2c) at the pinned commit: C drafts of the game code (tools/draft_batch.py, batch 12). Plain Python, no install.
pinned_clone m2c "$t/m2c"

# 5. dtk's own analysis of main.dol (function boundaries, SDK signature names).
cp -f "$ext/sys/main.dol" "$t/dtk/proj/main.dol"
if [ ! -f "$t/dtk/proj/symbols.txt" ]; then
    (cd "$t/dtk/proj" && "$dtk" dol split config.yml out) || die 'dtk dol split failed'
fi

# 6. Python environment for model previews, exact package versions.
if [ ! -d "$t/pyenv" ]; then
    "$PY" -m venv "$t/pyenv"
    "$t/pyenv/bin/python" -m pip install -q --disable-pip-version-check -r "$t/requirements-preview.txt" || die 'pip install failed'
fi

# 7. Ghidra 12.1.4 (hash published by the Ghidra project) + GameCube loader 1.3.1.
if [ ! -d "$t/ghidra_12.1.4_PUBLIC" ]; then
    pinned_file ghidra.zip "$dl/ghidra.zip"
    extract_zip "$dl/ghidra.zip" "$t"
fi
ext_dir="$t/ghidra_12.1.4_PUBLIC/Ghidra/Extensions"
if [ ! -d "$ext_dir/GameCubeLoader" ]; then
    pinned_file gcloader.zip "$dl/gcloader.zip"
    extract_zip "$dl/gcloader.zip" "$ext_dir"
    # the loader zip is built for Ghidra 12.1; Ghidra refuses it on 12.1.4 unless the version line matches
    sed -i.bak 's/^version=.*/version=12.1.4/' "$ext_dir/GameCubeLoader/extension.properties"
    rm -f "$ext_dir/GameCubeLoader/extension.properties.bak"
fi
# 8. Phase 4 build (decomp/): dtk-template, ninja, objdiff, binutils, the CodeWarrior compilers, your main.dol.
"$t/setup_decomp.sh"
echo "setup done. Next:  tools/batches/run_all.sh   then   tools/checksums.sh --compare"
