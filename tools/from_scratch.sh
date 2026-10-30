#!/usr/bin/env bash
# Everything from a fresh clone + your ISO, in order, stopping at the first failure, ending in PASS / FAIL
# (tools/from_scratch.ps1 for Linux / macOS, same steps):
#   1. tools/setup.sh            download + hash-check all tools, extract the disc
#   2. tools/batches/run_all.sh  every batch in order (1-12: names, Ghidra, checks, map, index, decomp build,
#                                compiler probe, drafts; 11 is Windows-only), ending in checksums.sh --compare
#   4. archives + models         SFD repack and model round trip byte-identical; all 1,006 models to .dat
#                                (glTF and the preview render need HSDRawViewer: Windows-only, skipped here)
#   5. mod test build            test_build/ = the game with robo 0 recoloured, bootable in Dolphin;
#                                then as ISO -> .crpatch -> applied to the clean ISO, SHA-1 checked
#   6. BUILD_REPORT.md           what was made and where everything is
# Usage:  tools/from_scratch.sh "/path/to/Custom Robo (USA).iso" [DolphinTool file or Dolphin folder]
set -euo pipefail
t=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$t")
. "$t/common.sh"
[ $# -ge 1 ] || die 'usage: tools/from_scratch.sh "/path/to/Custom Robo (USA).iso" [DolphinTool]'
iso=$1
dolphin=${2:-}
t0=$(date +%s)
names=(); values=()
errfile=$(mktemp)
trap 'rm -f "$errfile"' EXIT

report() {
    local mins ok=1 i rows=""
    mins=$(( ($(date +%s) - t0) / 60 ))
    for i in "${!names[@]}"; do case "${values[$i]}" in FAIL*) ok=0 ;; esac; done
    echo
    echo "=== result ($mins min)"
    for i in "${!names[@]}"; do printf '%-28s %s\n' "${names[$i]}" "${values[$i]}"; rows="$rows| ${names[$i]} | ${values[$i]} |"$'\n'; done
    if [ "$ok" = 1 ]; then echo 'PASS: fresh build matches the published reference'; else echo FAIL; fi
    local verdict=FAIL; [ "$ok" = 1 ] && verdict=PASS
    {
        echo '# Build report'
        echo
        echo "Built $(date '+%Y-%m-%d %H:%M') from \`$iso\` in $mins min: **$verdict**"
        echo
        echo '| Step | Result |'
        echo '|---|---|'
        printf '%s' "$rows"
        cat <<'EOF'

## Where everything is

| What | Where | How to use it |
|---|---|---|
| Decompiled game (C) | `decompiled/main_dol.c`, one file per function in `decompiled/funcs/` | any text editor |
| Search everything | `decompiled/index.db` | `python3 -I tools/find.py text <word>` (see README, "Reading the code") |
| Ghidra project | `ghidra_project/CustomRobo.gpr` | `tools/ghidra_12.1.4_PUBLIC/ghidraRun`, File > Open Project |
| Names | `symbols/symbols.txt` (verified), `symbols/descriptive.txt` (ours, `cr_`) | add names: CONTRIBUTING.md |
| Game code map | `docs/game_map.md` | which functions to name next |
| Disc contents | `Custom Robo extracted/` | raw files from your disc |
| Archive entries | `unpacked/` | `tools/sfd.py` unpack / pack |
| Models as HSD .dat | `models_dat/<category>/` | open in HSDRawViewer (Windows) |
| Models for Blender | glTF export is Windows-only (HSDRawViewer); run `tools/export_models.py` there | Blender: File > Import > glTF |
| Mod test build | `test_build/` | Dolphin: open `test_build/sys/main.dol`; robo 0 has red/blue swapped |
| Make your own mod | `docs/MODDING.md` | `python3 -I tools/make_mod.py mods/<your mod>` -> playable ISO + shareable .crpatch |
| How it all works | `docs/` (start at docs/README.md), `plan.md` | |
EOF
    } | crlf > "$g/BUILD_REPORT.md"
    echo "report: $g/BUILD_REPORT.md"
}

# step <name> <function>: the function runs in its own shell with errexit, so its first failing command ends the step
step() {
    local name=$1 fn=$2 rc msg
    echo
    echo "=== $name"
    : > "$errfile"
    set +e
    ( set -euo pipefail; "$fn" )
    rc=$?
    set -e
    names+=("$name")
    if [ "$rc" = 0 ]; then values+=(PASS)
    else
        msg=$(cat "$errfile"); [ -n "$msg" ] || msg="exit code $rc"
        values+=("FAIL: $msg"); report; exit 1
    fi
}
fail() { printf '%s\n' "$*" | tee "$errfile" >&2; exit 1; }

s_setup() { if [ -n "$dolphin" ]; then "$t/setup.sh" "$iso" "$dolphin"; else "$t/setup.sh" "$iso"; fi; }
s_run_all() { "$t/batches/run_all.sh"; }
s_sfd() {
    "$PY" -I "$t/sfd.py" unpack "$g/Custom Robo extracted/files" "$g/unpacked" || fail 'unpack failed'
    "$PY" -I "$t/sfd.py" verify "$g/Custom Robo extracted/files" "$g/unpacked"
}
s_roundtrip() {
    local dirs=() x
    for x in chara weapon_gun weapon_bom weapon_pod weapon_leg scenario_us/snr_chara scenario_jp/snr_chara; do
        [ -d "$g/unpacked/$x" ] && dirs+=("$g/unpacked/$x")
    done
    "$PY" -I "$t/crmodel.py" roundtrip "${dirs[@]}" || fail 'crmodel roundtrip failed'
    "$PY" -I "$t/robo_joints.py" selftest "$g/unpacked/chara"
}
s_models_dat() { "$PY" -I "$t/extract_models.py" "$g/unpacked" "$g/Custom Robo extracted/files" "$g/models_dat"; }
s_gltf() { echo 'skipped: HSDRawViewer is Windows-only'; }
s_preview() { echo 'skipped: needs the glTF export (Windows-only)'; }
s_test_build() { "$PY" -I "$t/make_test_build.py" "$g/Custom Robo extracted" "$g/unpacked" "$g/test_build" 0 1 2 3 4; }
s_patch() {
    # the mod test build as an ISO -> .crpatch -> applied to the clean ISO must give the same SHA-1 (apply checks)
    local d="$g/build/patch_test" rc=0
    mkdir -p "$d"
    "$PY" -I "$t/iso_patch.py" build "$iso" "$g/test_build" "$d/mod.iso" || { rm -f "$d/mod.iso"; fail 'iso_patch build failed'; }
    "$PY" -I "$t/iso_patch.py" diff "$iso" "$d/mod.iso" "$d/test.crpatch" || { rm -f "$d/mod.iso"; fail 'iso_patch diff failed'; }
    "$PY" -I "$t/iso_patch.py" apply "$iso" "$d/test.crpatch" "$d/applied.iso" || rc=$?
    rm -f "$d/mod.iso" "$d/applied.iso"
    return $rc
}

step 'setup' s_setup
step 'all batches + checksums (run_all)' s_run_all
step 'SFD unpack + repack' s_sfd
step 'model round trip' s_roundtrip
step 'models to .dat' s_models_dat
step 'models to glTF' s_gltf
step 'preview render' s_preview
step 'mod test build' s_test_build
step 'patch round trip' s_patch
report
