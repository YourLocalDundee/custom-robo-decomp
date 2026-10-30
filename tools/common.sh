# Shared helpers for the bash scripts (tools/*.sh, tools/batches/*.sh): sourced, never run. The caller sets g (repo root)
# first. Everything the PowerShell scripts do with cmdlets is here as a small function, so each .sh reads like its .ps1.
# Output files are written exactly as the Windows run writes them (CRLF where Set-Content wrote CRLF), so
# docs/expected_checksums.txt holds on every OS.

# python: "python3", or "python" where that is the only name (it must be Python 3.11+; the tools run it with -I)
PY=python3
command -v python3 >/dev/null 2>&1 || PY=python

die() { printf '%s\n' "$*" >&2; exit 1; }
is_macos() { [ "$(uname -s)" = Darwin ]; }
ncpu() { nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4; }

sha1_of() { if command -v sha1sum >/dev/null 2>&1; then sha1sum "$1"; else shasum -a 1 "$1"; fi | cut -d' ' -f1; }
sha256_of() { if command -v sha256sum >/dev/null 2>&1; then sha256sum "$1"; else shasum -a 256 "$1"; fi | cut -d' ' -f1; }

# stdin lines -> stdout with CRLF line ends (what PowerShell's Set-Content / Add-Content write on Windows)
crlf() { awk '{ sub(/\r$/, ""); printf "%s\r\n", $0 }'; }
# stdin -> stdout without CR (what Get-Content reads)
nocr() { tr -d '\r'; }
# grep that is not an error when nothing matches (PowerShell's Where-Object / Select-String just return nothing)
grepq() { grep "$@" || [ $? -eq 1 ]; }
# lines sorted like PowerShell's Sort-Object (culture order; -u = -Unique): tools/ps_sort.py
ps_sort() { "$PY" -I "$g/tools/ps_sort.py" "$@"; }

# tools/pins.txt: "kind name source version-or-sha256"; pin_field <name> <2 = source | 3 = version>
pin_field() {
    awk -v n="$1" -v i="$2" '{ sub(/\r$/, "") } /^[[:space:]]*(#|$)/ { next } $2 == n { print $(i + 1); exit }' "$g/tools/pins.txt"
}

# download once into tools/downloads/, then always verify the SHA-256 from pins.txt
pinned_file() {
    local name=$1 out=$2 src ver h
    src=$(pin_field "$name" 2); ver=$(pin_field "$name" 3)
    [ -n "$src" ] || die "no pin '$name' in tools/pins.txt"
    if [ ! -f "$out" ]; then
        echo "downloading $name"
        curl -fsSL --retry 3 -o "$out.part" "$src" && mv -f "$out.part" "$out"
    fi
    h=$(sha256_of "$out")
    if [ "$h" != "$ver" ]; then rm -f "$out"; die "$name sha256 $h does not match tools/pins.txt (deleted, re-run to retry)"; fi
    echo "$name sha256 ok"
}

# a sparse git clone at the pinned commit: pinned_clone <name> <dir> [sparse paths...]
pinned_clone() {
    local name=$1 dir=$2 src ver
    shift 2
    src=$(pin_field "$name" 2); ver=$(pin_field "$name" 3)
    [ -n "$src" ] || die "no pin '$name' in tools/pins.txt"
    if [ -e "$dir" ]; then
        # an existing clone gets the caller's sparse paths (a list widened later, e.g. tp / pikmin2 in step 68)
        if [ $# -gt 0 ]; then git -C "$dir" sparse-checkout set --no-cone "$@"; fi
        echo "skip $dir (exists)"; return 0
    fi
    git init -q "$dir"
    git -C "$dir" remote add origin "$src"
    if [ $# -gt 0 ]; then git -C "$dir" sparse-checkout set --no-cone "$@"; fi
    git -C "$dir" fetch -q --depth 1 --filter=blob:none origin "$ver"
    git -C "$dir" checkout -q FETCH_HEAD
    echo "$name at $ver"
}

# unzip keeps the executable bits Ghidra's scripts need
extract_zip() { mkdir -p "$2"; unzip -q -o "$1" -d "$2" || die "extracting $1 failed"; }

# the pinned decomp-toolkit for this OS and CPU: dtk_pin -> pin name (tools/pins.txt)
dtk_pin() {
    local os cpu
    os=linux; is_macos && os=macos
    case "$(uname -m)" in
        x86_64 | amd64) cpu=x86_64 ;;
        aarch64 | arm64) if is_macos; then cpu=arm64; else cpu=aarch64; fi ;;
        *) die "no pinned dtk for $os $(uname -m)" ;;
    esac
    echo "dtk-$os-$cpu"
}

# the reference symbols.txt files (Melee first, then tools/refs/<game>/config/<version>/symbols.txt), in the order the
# Windows scripts pass them to tools/verify_symbols.py (Sort-Object on the Windows paths)
ref_symbol_files() {
    local d
    echo "$g/tools/melee/config/GALE01/symbols.txt"
    for d in "$g"/tools/refs/*/config/*/; do
        [ -f "$d/symbols.txt" ] && printf '%s\n' "${d#"$g/"}symbols.txt"
    done | tr '/' '\\' | ps_sort | tr '\\' '/' | sed "s|^|$g/|"
}
