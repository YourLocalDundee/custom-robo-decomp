#!/usr/bin/env bash
# Phase 4 build setup (plan.md Phase 4, docs/REPRODUCE.md "Build main.dol from asm"): makes decomp/ buildable.
# Linux / macOS version of tools/setup_decomp.ps1, same steps:
#   1. encounter/dtk-template at the commit pinned in tools/pins.txt -> tools/refs/dtk-template; its configure.py and
#      tools/*.py are copied into decomp/ byte for byte and configure.py gets tools/patches/dtk_template_configure.patch.
#      (Only decomp/config/ is ours and committed; the copied files are gitignored and rewritten on every run.)
#   2. The build tools the template names, at its versions, SHA-256 checked (pins.txt): ninja -> tools/ninja/,
#      objdiff-cli, sjiswrap and wibo (Linux x86_64 / macOS: runs the Windows-only CodeWarrior tools) -> decomp/build/tools/,
#      binutils -> decomp/build/binutils/, the CodeWarrior compilers -> decomp/build/compilers/. dtk is tools/dtk/ (setup.sh).
#   3. YOUR main.dol: "Custom Robo extracted/sys/main.dol" (setup.sh step 2), SHA-1 checked -> decomp/orig/GXCE01/sys/.
# Needs setup.sh to have run (disc extracted, dtk downloaded). Rerunnable: downloads and extractions are skipped
# when done; the template files and the DOL copy are refreshed every time.
# Usage:  tools/setup_decomp.sh
set -euo pipefail
t=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$t")
. "$t/common.sh"
dl="$t/downloads"
d="$g/decomp"
mkdir -p "$dl" "$d/build/tools" "$d/orig/GXCE01/sys"

# a pinned single-file tool, copied into place and made executable
pinned_tool() {
    pinned_file "$1" "$dl/$1"
    if [ ! -f "$2" ]; then cp "$dl/$1" "$2"; chmod +x "$2"; fi
}
# a pinned zip, extracted once into $2 (marker file = done); every file executable, as the Windows zips carry no bits
pinned_zip() {
    pinned_file "$1" "$dl/$1"
    if [ ! -f "$2/.pinned_$1" ]; then
        extract_zip "$dl/$1" "$2"
        find "$2" -type f -exec chmod +x {} +
        pin_field "$1" 3 > "$2/.pinned_$1"
    fi
}

# 1. the template, checked out without line-ending conversion (the patch is against the exact bytes)
tpl="$t/refs/dtk-template"
ver=$(pin_field dtk-template 3)
if [ ! -d "$tpl" ]; then
    git init -q "$tpl"
    git -C "$tpl" config core.autocrlf false
    git -C "$tpl" config core.eol lf
    git -C "$tpl" remote add origin "$(pin_field dtk-template 2)"
    git -C "$tpl" fetch -q --depth 1 origin "$ver"
    git -C "$tpl" checkout -q FETCH_HEAD || die 'dtk-template checkout failed'
fi
head=$(git -C "$tpl" rev-parse HEAD)
[ "$head" = "$ver" ] || die "tools/refs/dtk-template is at $head, pins.txt says $ver: delete tools/refs/dtk-template and re-run"
mkdir -p "$d/tools"
git -C "$tpl" ls-files configure.py LICENSE 'tools/*.py' | while IFS= read -r f; do
    if [ "$f" = LICENSE ]; then dst="$d/tools/LICENSE.dtk-template"; else dst="$d/$f"; fi
    cp -f "$tpl/$f" "$dst"
done
# the template clone as git dir and decomp/ as work tree: paths in the patch are then relative to decomp/
(cd "$d" && git --git-dir="$tpl/.git" --work-tree=. apply "$t/patches/dtk_template_configure.patch") ||
    die 'tools/patches/dtk_template_configure.patch does not apply to the pinned template'
grep -q 'Custom Robo' "$d/configure.py" || die 'configure.py was not patched'
echo "dtk-template at $head, configure.py patched"

# 2. build tools for this OS
cpu=$(uname -m)
if is_macos; then
    ninja=ninja-mac.zip; binutils=binutils-macos-universal.zip; wibo=wibo-macos
    case "$cpu" in arm64) objdiff=objdiff-cli-macos-arm64 ;; *) objdiff=objdiff-cli-macos-x86_64 ;; esac
else
    case "$cpu" in
        x86_64 | amd64) ninja=ninja-linux.zip; objdiff=objdiff-cli-linux-x86_64; binutils=binutils-linux-x86_64.zip; wibo=wibo-x86_64 ;;
        aarch64 | arm64) ninja=ninja-linux-aarch64.zip; objdiff=objdiff-cli-linux-aarch64; binutils=binutils-linux-aarch64.zip; wibo= ;;
        *) die "no pinned build tools for Linux $cpu" ;;
    esac
fi
pinned_zip "$ninja" "$t/ninja"
pinned_tool "$objdiff" "$d/build/tools/objdiff-cli"
pinned_tool sjiswrap.exe "$d/build/tools/sjiswrap.exe"
if [ -n "$wibo" ]; then pinned_tool "$wibo" "$d/build/tools/wibo"
else echo "note: no pinned wibo for $cpu; the compilers will run through wine (install it, docs/INSTALL.md)"; fi
pinned_zip "$binutils" "$d/build/binutils"
pinned_zip compilers_20251118.zip "$d/build/compilers"
[ -f "$t/dtk/dtk" ] || die "$t/dtk/dtk missing: run tools/setup.sh first"

# 3. your main.dol
src="$g/Custom Robo extracted/sys/main.dol"
[ -f "$src" ] || die "$src missing: run tools/setup.sh <your ISO> first"
sha1=$(sha1_of "$src")
[ "$sha1" = 6a73cd111ef349b20be294511b8c51a8addfb50b ] || die "main.dol SHA-1 $sha1 is not the GXCE01 v1.0 retail one"
cp -f "$src" "$d/orig/GXCE01/sys/main.dol"
echo "main.dol ok -> decomp/orig/GXCE01/sys/main.dol"
echo "decomp setup done. Build:  cd decomp; $PY configure.py; ../tools/ninja/ninja   (or tools/batches/batch9_decomp_build.sh)"
