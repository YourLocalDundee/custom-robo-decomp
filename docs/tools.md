# Tools

Everything in `tools/`. Run from the repo root.

- **Python tools** (`tools/*.py`) run with the system Python 3.11+ in isolated mode: on Windows
  `python -I tools\x.py ...`, on Linux / macOS `python3 -I tools/x.py ...`. The tables below give the script and its
  arguments only (`tools/find.py text chara_bin`); put your system's `python -I` / `python3 -I` in front. Paths in
  the arguments are written with `/`, which Python accepts on Windows too. `render_preview.py` needs the venv
  `tools/pyenv` (trimesh, matplotlib): `tools\pyenv\Scripts\python` on Windows, `tools/pyenv/bin/python` on Linux /
  macOS.
- **Orchestration scripts** exist twice, `.ps1` for Windows (PowerShell 7: `pwsh tools\x.ps1`) and `.sh` for Linux /
  macOS (bash: `tools/x.sh`): same steps, same arguments in each shell's spelling (`-Compare` / `--compare`), same
  output files. `tools/common.sh` holds the helpers the `.sh` scripts share (pinned downloads, SHA checks, CRLF output,
  the PowerShell-order sort). The `.ps1`-only tools (`boot_check.ps1`, `boot_test.ps1`, batch 11) drive Dolphin
  through the Win32 API and are Windows-only.

## Our tools

| Tool | Purpose | Usage |
|---|---|---|
| `sfd.py` | SFD archives: unpack / pack / byte-identical verify ([format](formats/sfd.md)) | `tools/sfd.py unpack "Custom Robo extracted/files" unpacked`<br>`tools/sfd.py pack <entry_dir> <out.BIN>`<br>`tools/sfd.py verify "Custom Robo extracted/files" unpacked` |
| `crmodel.py` | Model entry ↔ standard .dat ([format](formats/model_entry.md)) | `tools/crmodel.py to-dat <entry.bin> <out.dat>`<br>`tools/crmodel.py from-dat <in.dat> <template_entry.bin> <out_entry.bin>`<br>`tools/crmodel.py roundtrip unpacked/chara unpacked/weapon_gun …` |
| `extract_models.py` | Every known model → `models_dat/<category>/*.dat` | `tools/extract_models.py unpacked "Custom Robo extracted/files" models_dat` |
| `export_models.py` | `models_dat/` → `models_export/<category>/<name>/` via HSDRawViewer (`.glb`, `.dae` fallback); writes `export_log.txt`. Windows only | `tools/export_models.py models_dat models_export` |
| `make_test_build.py` | Copies the extracted disc to a build folder and rebuilds chosen `chara` entries through the full import path (red/blue texture swap + 0x100 data shift) | `tools/make_test_build.py "Custom Robo extracted" unpacked test_build 0 1 2 3 4` |
| `render_preview.py` | Front + side flat-shaded PNG of a `.glb` (sanity check); runs with the venv's Python (above) | `tools/render_preview.py <model.glb> <out.png>` |
| `cr_model_explore.py` | Prints a model entry's header, reloc stats and pointer graph (research) | `tools/cr_model_explore.py unpacked/chara/000.bin 5` |
| `find.py` | Search the decompile ([workflows](workflows.md#5-read-and-search-the-code)): `text`, `func [--brief]`, `callers`, `callees`, `refs`, `str`, `hint` | `tools/find.py func cr_Archive_OpenEntry --brief` |
| `robo_joints.py` | Robo joint roles (blocks B/C): `table`, `roles`, `path`, `selftest` (170/170 byte-identical) | `tools/robo_joints.py roles unpacked/chara/000.bin` |
| `make_mod.py` | Mod folder (models, files, code patches) → playable ISO + `.crpatch`; syncs main.dol's entry tables ([MODDING](MODDING.md)) | `tools/make_mod.py mods/my_mod [<clean.iso>]` |
| `blender/*.py` | Blender scripts of the [Blender loop](MODDING.md#blender-loop), run by Blender's own Python (not `python -I`): `common.py` (load / save settings), `roundtrip.py` (no edit), `legionnaire.py` (the step-70 test edit) | `blender -b --factory-startup --python tools/blender/roundtrip.py -- <in.glb> <out.glb>` |
| `iso_patch.py` | `build` / `diff` / `apply` / `info`: modded ISO and data-free patches | `tools/iso_patch.py apply <clean.iso> <mod.crpatch> <out.iso>` |
| `robo_swap.py` | Swap two robos completely (`--model-only`, `--keep-motion`, `--size X`) | `tools/robo_swap.py 0 7` |
| `struct_fields.py` | Finds typed fields inside a `cr_` struct's unknown stretches from the C's accesses (`report` → `docs/struct_<struct>.md`, `apply`) | `tools/struct_fields.py cr_Chara report` |
| `name_evidence.py` | Harvests the game's own names (pointer tables to strings, identifier / format / source-file strings + their users) → `docs/name_evidence.md`, `docs/name_tables.tsv` | `tools/name_evidence.py` |
| `check_docs.py` | Checks the docs: links, doc paths named in scripts, the index, CRLF, one OS per command block ([below](#checking-the-docs)) | `tools/check_docs.py` |
| `boot_test.ps1` | Windows: boots a build folder in Dolphin (`CR_DOLPHIN_DIR`, else a local Project Hive build, else the pinned `tools/dolphin`), takes a screenshot after a delay, closes Dolphin. Older than `boot_check.ps1`, which batch 11 uses | `pwsh tools\boot_test.ps1 test_build shot.png 40` |
| `boot_check.ps1` | Windows: boots an ISO (or a folder's `sys\main.dol`) in Dolphin (`-DolphinDir`, else `CR_DOLPHIN_DIR`, else the pinned Dolphin; used by batch 11), reports warning dialogs (`-Snapshot` saves a picture of one, `-Screenshot` the game window, `-Log` Dolphin's log of that run) | `pwsh tools\boot_check.ps1 <game.iso or folder\sys\main.dol> [seconds]` |

## Setup and the full run

| Tool | Purpose | Windows / Linux, macOS |
|---|---|---|
| `model_loop` | [Blender loop](MODDING.md#blender-loop): export -> `tools/blender/<script>.py` -> `HSDRawViewer --import` -> `make_mod.py` -> ISO + `.crpatch` (+ boot). Export / import need Windows | `pwsh tools\model_loop.ps1 -Mod <mod> -Script <script> -Entries 000 [-Boot]` / `tools/model_loop.sh <mod> <script> 000 [--boot]` |
| `from_scratch` | Everything from a fresh clone + your ISO, PASS/FAIL table, writes `BUILD_REPORT.md` ([REPRODUCE](REPRODUCE.md)) | `pwsh tools\from_scratch.ps1 -Iso <iso>` / `tools/from_scratch.sh <iso>` |
| `setup` | Downloads every tool at the version in `tools/pins.txt` (SHA-256 checked), extracts the disc (Windows: DolphinTool; Linux / macOS: the pinned dtk) | `pwsh tools\setup.ps1 -Iso <iso>` / `tools/setup.sh <iso>` |
| `setup_decomp` | Phase 4 build setup (setup step 8): the pinned `encounter/dtk-template` → `decomp/` with our configure patch, the pinned build tools, your `main.dol` | run by `setup` |
| `rebuild_decomp` | Fresh Ghidra import + GameCube analyzer + names + strings + types + C export ([decomp](decomp.md)) | `pwsh tools\rebuild_decomp.ps1` / `tools/rebuild_decomp.sh` |
| `checksums` | SHA-256 of every output vs `docs/expected_checksums.txt` | `pwsh tools\checksums.ps1 -Compare` (`-Write`) / `tools/checksums.sh --compare` (`--write`) |
| `batches/run_all` | Runs every batch in order (1-12) and ends with the checksum compare; see "Batches" below | `pwsh tools\batches\run_all.ps1` / `tools/batches/run_all.sh` |
| `common.sh` | Linux / macOS only: helpers sourced by every `.sh` script: `pinned_file` / `pinned_clone` (tools/pins.txt), SHA-1/256, `crlf`, `ps_sort`, the reference-file order | sourced |
| `ps_sort.py` | Sorts lines like PowerShell's `Sort-Object` on Windows (culture order), so the `.sh` batches write the same report files as the `.ps1` ones | `tools/ps_sort.py [-u] < lines` |

## Naming tools (batches 1, 6, 7, 8)

| Tool | Purpose |
|---|---|
| `layout_match.py` | Batch 1: library names by layout proof against the pinned reference decomps (Melee, TP, TWW, MKDD, Pikmin 2, TTYD, ...) |
| `verify_symbols.py` | Batch 1: checks every name in `symbols/symbols.txt` against the reference decomps' symbols (existence + size; the closest reference wins) → `symbols/verify_report.txt`. Usage: `tools/verify_symbols.py <symbols.txt> <dtk symbols.txt> <reference symbols.txt> [more ...]` (batch 1 passes Melee's and every `tools/refs/*/config/*/symbols.txt`) |
| `layout_evidence.py` | Library module: independent call-graph and string evidence for library names, ours vs the reference decomps' sources (used by `layout_match.py` and `audit_layout.py`) |
| `audit_layout.py` | Audits the layout-proof names in `symbols/symbols.txt` by replaying the layout match → `symbols/layout_audit.txt` (`tools/audit_layout.py`) |
| `export_struct.py` | Library module: structure-only view of the Ghidra export (function starts, references, strings by address) for the name matchers |
| `dol_asserts.py` | Library module: assert / panic calls of main.dol from the instruction bytes (function, file string, expression string), used by `match_lib_hsd.py` |
| `match_lib_sdk.py` | Batch 6: names the remaining SDK functions in 0x80131000..0x8015F000 (OS, DVD, VI, PAD, AI/AR/ARQ/DSP, CARD, GX, MTX, TRK) from reference decomps |
| `match_lib_rt.py` | Batch 6: names for the Metrowerks runtime / MSL C / math / EXI / SI range 0x8015F000-0x8016D408 (rules R3, R5, R6) |
| `match_lib_hsd.py` | Batch 6: names the remaining sysdolphin (HSD) functions from 0x8016D408 on, from independent evidence |
| `merge_lib_candidates.py` | Batch 6 helper: merges `symbols/candidates_*.txt` into `symbols.txt` (tagged `lib-candidate`) after checking each line |
| `match_game_names.py` | Batch 7: applies the game's own names where proven (G1 parallel name/function tables, G2 self-naming messages, G3 source-file module notes), from `main.dol` bytes only → `symbols/game_real.txt`, leftovers `symbols/game_unnamed_real.txt` |
| `lint_descriptive.py` | Checks `symbols/descriptive.txt` and `cr_globals.txt` (cr_ prefix, why note, no clashes) |
| `crosscheck_names.py` | Batch 8: independent accuracy cross-check of every name, library and game (`[--symbols FILE] [--out FILE.txt] [--md FILE.md]`) → `symbols/names_crosscheck.txt`, `docs/names_crosscheck.md` |
| `match_hsd_asserts.py` | The first HSD names (Oct 25, 2026): `__assert` (file, expression) calls against the Melee decomp → `symbols/melee_matches.txt`. Usage: `tools/match_hsd_asserts.py decompiled/main_dol.c "Custom Robo extracted/sys/main.dol" tools/melee/src/sysdolphin symbols/melee_matches.txt` |
| `crosscheck_dtk.py` / `game_map.py` / `build_index.py` | Batches 2 / 3 / 4: dtk cross-check / game-code map / SQLite search index |
| `clean_headers.py` | Declarations-only copy of Melee's HSD/SDK headers for Ghidra's parser; Custom Robo overrides (`HSD_PObj`) |
| `check_class_sizes.py` | Batch 5: parsed HSD struct sizes vs the sizes Custom Robo registers for each class |

## Phase 4 build tools (batches 9, 10, 12)

| Tool | Purpose | Usage |
|---|---|---|
| `gen_dtk_symbols.py` | Batch 9: `decomp/config/GXCE01/symbols.txt` = dtk's analysis of main.dol with our verified names → `decomp/reports/symbols_report.txt` | run by batch 9 |
| `gen_dtk_splits.py` | Batch 9: `decomp/config/GXCE01/splits.txt`, object (`.c` file) boundaries only where proven, incl. data placed by the matched C and the `game/` objects of `decomp/src/game` (method: its docstring) → `decomp/reports/splits_report.txt` | run by batch 9 |
| `gen_c_symbols.py` | Batch 9: gives the objects built from C their own symbols in the config `symbols.txt` (objdiff pairs data by name) → `decomp/reports/c_symbols_report.txt` | run by batch 9 |
| `check_c_objects.py` | Batch 9: every object linked from C is 100% code + data in objdiff, every other C file is in `CR_NOT_MATCHING` → `decomp/reports/progress_report.txt`, `sources_report.txt` | `tools/check_c_objects.py --link-each` (by hand: links one more C object, prints the SHA-1) |
| `check_c_callsites.py` | Batch 9: names from the matched C's call sites and definitions (rules R7 / R8); checks every `// c-callsite` / `// c-define` line → `decomp/reports/c_callsites_report.txt` | run by batch 9 |
| `reloc_audit.py` | Library: checks dtk's relocations in a linked `main.elf` against main.dol's bytes (missed pointers, numbers relocated as pointers, glue spans); used by the two below | — |
| `gen_dtk_relocs.py` | Batch 9: writes the generated relocation fixes into `decomp/config/GXCE01/config.yml`; `--check` checks the build (`decomp/reports/relocs_report.txt`) | run by batch 9 |
| `shift_test.py` | Batch 9: pads code and data at 30 points (incl. the proven data-object boundaries; `--all-data`: all of them) in a scratch copy of the build, relinks, checks every relocation (`decomp/reports/shift_report.txt`); batch 11 boots the result | `tools/shift_test.py [--pad 0x100] [--all-data]` |
| `import_ref_source.py` | Copies a C source and the headers it includes from a pinned reference decomp into `decomp/src` / `decomp/include`, provenance on line 1 | `tools/import_ref_source.py <ref> <path in ref> src/<object path> --inc <dir>=include/<ref>` ([decomp](decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026)) |
| `make_configure_patch.py` | Rewrites `tools/patches/dtk_template_configure.patch` from the edited `decomp/configure.py` (and checks template + patch == file) | `tools/make_configure_patch.py` |
| `compiler_probe/probe.py` | Batch 10: which CodeWarrior version and flags built each part of main.dol ([compiler](compiler.md)) | `tools/compiler_probe/probe.py --show FUNCTION --version V --flags "..."` |
| `draft_batch.py` | Batch 12: m2c C draft of every game function (context: HSD / `cr_types.h` + every `lbl_` data object with its size + the game functions' prototypes from a first pass) → post-processed and repaired on the compiler's messages → compared with the original, aligned ([drafts](drafts.md)) | `tools/draft_batch.py [--only NAME ...] [--sample N] [--jobs N] [--out DIR]` (`--only` / `--sample` reuse `ctx_protos.h` / `ctx_sdk.h` of the output folder, `decomp/drafts` by default; `--out` keeps an experiment apart from the last full run: copy those two files there first) |
| `promote_drafts.py` | Batch 12: puts every 100% draft that can be linked into `decomp/src/game/<function>.c`, and the drafts of a whole file run (functions sharing a constant pool, step 63) into a file object `<first>_<last>.c` → `decomp/reports/promote_report.txt` (method: its docstring; `--check --try-run FIRST LAST` tests a run) | run by batch 12 |

## Batches (`tools/batches`)

Each batch can run alone: `pwsh tools\batches\<name>.ps1` on Windows, `tools/batches/<name>.sh` on Linux / macOS
(every batch except 11, which is Windows-only and reported as skipped by `run_all.sh`). `run_all` runs all of them in
order ([REPRODUCE](REPRODUCE.md)). Options, Windows / Linux, macOS: `run_all` `-SkipRebuild` `-MaxPasses N` /
`--skip-rebuild` `--max-passes N`; `batch10_compiler_probe` `-Jobs N` / `--jobs N`; `batch12_drafts` `-Sample N`
`-Jobs N` / `--sample N` `--jobs N`.

| Batch (`.ps1` / `.sh`) | Purpose |
|---|---|
| `run_all` | The one script: names (1 + 6 + 7) ↔ Ghidra rebuild to a fixpoint → 5 → 2 → 3 → 4 → 8 → 9 → 10 → 11 → 12 → checksum compare (831 s in the last timed run, plan.md step 58, before batch 12 grew in step 59) |
| `batch1_names` | Library names by layout proof (naming rule 3), then verify |
| `batch2_crosscheck` | Compares Ghidra's C export with decomp-toolkit's analysis → `symbols/crosscheck_report.txt` |
| `batch3_game_map` | Maps the game's own code (callers, callees, strings, asserts, subsystem hints) → `docs/game_map.md`, `symbols/game_map.tsv` |
| `batch4_index` | Search index of the whole decompile and one file per function → `decompiled/index.db`, `decompiled/funcs/` |
| `batch5_types` | Checks the types the rebuild applied (`symbols/types_report.txt`, `check_class_sizes.py`) |
| `batch6_lib_candidates` | Names the remaining library functions (rules R3-R6) with the three `match_lib_*.py`, merged as `lib-candidate` |
| `batch7_game_names` | Game-code naming stage 1: the game's own names (`match_game_names.py`), merged into `symbols/descriptive.txt` as `game-real` |
| `batch8_crosscheck` | Independent accuracy cross-check of every name (`crosscheck_names.py`); stops on any FAIL |
| `batch9_decomp_build` | The Phase 4 build: config, splits, objects from C, main.dol == retail SHA-1, relocation check, static shift test |
| `batch10_compiler_probe` | Compiler probe (`compiler_probe/probe.py`, about 5 minutes) |
| `batch11_shift_test` | `.ps1` only (Windows): boots the shifted build in Dolphin (twice, about 1 minute; don't type while it runs) |
| `batch12_drafts` | m2c drafts of the game code, compiled and compared, then `promote_drafts.py` (and batch 9 again when `decomp/src/game` changed) |

## Ghidra scripts (`tools/scripts`)

| Script | Role |
|---|---|
| `EnableGCAnalyzer.java` | pre-script: turns on the GameCube loader's "(GameCube/Wii) Program Analyzer" (sets r13/r2), which stays off by default because its format-name check no longer matches the loader |
| `ApplySymbols.java` | applies names from a dtk-style symbols file (skips generic `fn_`/`lbl_` names) |
| `DefineStrings.java` | defines null-terminated ASCII strings (≥ 4 chars) in non-executable memory |
| `ExportDecompiledC.java` | decompiles every function into one C file |
| `CreateDtkFunctions.java` | creates functions at starts decomp-toolkit found and Ghidra missed |
| `ExportXrefs.java` | functions / references / strings as TSV (for the search index) |
| `ParseHeaders.java` | parses HSD/SDK headers + `symbols/cr_types.h`, checks sizes/offsets, applies prototypes and typed globals |

## Third-party tools (installed under `tools/` by setup)

Versions are pinned in `tools/pins.txt`; what to install yourself per OS: [INSTALL.md](INSTALL.md).

| Tool | Version | Notes |
|---|---|---|
| Ghidra | 12.1.4 (`ghidra_12.1.4_PUBLIC`) | needs Java 21 (system-wide, or a JDK on `PATH`); headless via `support\analyzeHeadless.bat` (Windows) / `support/analyzeHeadless` |
| Ghidra GameCube Loader | 1.3.1 (built for 12.1) | `extension.properties` version patched 12.1 → 12.1.4. Headless needs `-loader-autoloadMaps false` or it hangs on a hidden dialog. |
| HSDLib / HSDRawViewer | built from source on Windows (`tools/HSDLib`, base commit in `tools/patches/hsdlib_base_commit.txt`) | targets .NET 8; runtimeconfig set to `rollForward: Major` so it runs on a newer installed .NET. Our patch (`tools/patches/hsdlib_headless_export_resave.patch`) adds `--export <out_dir> <glb\|dae\|smd> <files…>` and `--resave <in.dat> <out.dat>`. After every rebuild the runtimeconfig roll-forward line must be re-added. |
| decomp-toolkit (dtk) | 1.8.4 (`tools/dtk/dtk.exe` on Windows, `tools/dtk/dtk` on Linux / macOS) | analysis project in `tools/dtk/proj` (`symbols.txt`, `splits.txt`); on Linux / macOS also extracts the disc (`dtk disc extract`, byte-identical to DolphinTool) |
| Dolphin | 2609a, official build (`tools/dolphin/Dolphin-x64`, Windows) | its `DolphinTool` extracts the disc on Windows; batch 11 boots the shifted build with it |
| Melee decomp | sparse clone (`tools/melee`) | `config`, `src/sysdolphin`, `src/MSL`, `src/Runtime`, `src/MetroTRK` and the SDK headers ([REPRODUCE](REPRODUCE.md), setup step 3) |
| Other reference decomps | sparse clones (`tools/refs/`) | MKDD, Pikmin 2, TP, TTYD, TWW, KAR (doldecomp and wowjinxy), XD, dolsdk2001 / 2004, AC, SMS, Mario Party 4, Pikmin, BfBB: commits in `tools/pins.txt` |
| m2c | pinned commit (`tools/m2c`) | batch 12 drafts; plain Python |
| Phase 4 build tools | dtk-template, ninja 1.13.2, objdiff-cli 3.6.1, gc-wii-binutils 2.42-2, sjiswrap 1.2.2, wibo 1.0.3 (Linux x86_64 / macOS), CodeWarrior `compilers_20251118` | `tools/ninja/`, `tools/refs/dtk-template/`, `decomp/build/{tools,binutils,compilers}/` ([INSTALL](INSTALL.md#phase-4-build-tools-decomp-batch-9)) |
| Python venv | `tools/pyenv` | trimesh, matplotlib, numpy, pygltflib, pillow (`tools/requirements-preview.txt`) |

## Rebuilding HSDRawViewer (Windows)

```powershell
cd tools\HSDLib
git apply ..\patches\hsdlib_headless_export_resave.patch     # on a fresh clone only
dotnet build HSDRawViewer\HSDRawViewer.csproj -c Release
# then add  "rollForward": "Major",  under "runtimeOptions" in
# HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.runtimeconfig.json
```

## Checking the docs

`tools/check_docs.py` (Windows `python -I tools\check_docs.py`, Linux / macOS `python3 -I tools/check_docs.py`)
reads every file git tracks and reports, as `file:line: problem`: relative Markdown links whose file or `#heading`
does not exist; `docs/...` paths named in any script, report or doc that do not exist; files in `docs/` missing from
the index ([README.md](README.md)); hand-written `.md` files with LF line ends (the repo uses CRLF); and command
blocks that mix systems (`pwsh` or a `.ps1` in a bash block, a `.sh` in a PowerShell block). It writes nothing,
needs no batch output and no network, and exits 1 on any problem. Run it after moving, renaming or editing a doc.
