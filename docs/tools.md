# Tools

All in `tools\`. Python tools run with the system Python (3.11+) in isolated mode (`python -I`); `render_preview.py` needs `tools\pyenv` (trimesh, matplotlib). Run commands from the repo root.

## Our tools

| Tool | Purpose | Usage |
|---|---|---|
| `sfd.py` | SFD archives: unpack / pack / byte-identical verify ([format](formats/sfd.md)) | `python -I tools\sfd.py unpack "Custom Robo extracted\files" unpacked`<br>`python -I tools\sfd.py pack <entry_dir> <out.BIN>`<br>`python -I tools\sfd.py verify "Custom Robo extracted\files" unpacked` |
| `crmodel.py` | Model entry ↔ standard .dat ([format](formats/model_entry.md)) | `python -I tools\crmodel.py to-dat <entry.bin> <out.dat>`<br>`python -I tools\crmodel.py from-dat <in.dat> <template_entry.bin> <out_entry.bin>`<br>`python -I tools\crmodel.py roundtrip unpacked\chara unpacked\weapon_gun …` |
| `extract_models.py` | Every known model → `models_dat\<category>\*.dat` | `python -I tools\extract_models.py unpacked "Custom Robo extracted\files" models_dat` |
| `export_models.py` | `models_dat\` → `models_export\<category>\<name>\` via HSDRawViewer (`.glb`, `.dae` fallback); writes `export_log.txt` | `python -I tools\export_models.py models_dat models_export` |
| `make_test_build.py` | Copies the extracted disc to a build folder and rebuilds chosen `chara` entries through the full import path (red/blue texture swap + 0x100 data shift) | `python -I tools\make_test_build.py "Custom Robo extracted" unpacked test_build 0 1 2 3 4` |
| `boot_test.ps1` | Boots a build folder in Dolphin (the path set in the script), captures the window, closes Dolphin. Capture currently comes out black. | `pwsh tools\boot_test.ps1 -Build test_build -Out shot.png -Seconds 40` |
| `render_preview.py` | Front + side flat-shaded PNG of a `.glb` (sanity check) | `tools\pyenv\Scripts\python tools\render_preview.py <model.glb> <out.png>` |
| `cr_model_explore.py` | Prints a model entry's header, reloc stats and pointer graph (research) | `python -I tools\cr_model_explore.py unpacked\chara\000.bin 5` |
| `rebuild_decomp.ps1` | Fresh Ghidra import + GameCube analyzer + names + strings + C export ([decomp](decomp.md)) | `pwsh tools\rebuild_decomp.ps1` |
| `match_hsd_asserts.py` | Matches `__assert` (file, expression) calls against the Melee decomp to propose HSD function names | `python -I tools\match_hsd_asserts.py decompiled\main_dol.c "Custom Robo extracted\sys\main.dol" tools\melee\src\sysdolphin symbols\melee_matches.txt` |
| `verify_symbols.py` | Checks every name in `symbols\symbols.txt` against Melee's verified symbols (existence + size) | `python -I tools\verify_symbols.py symbols\symbols.txt tools\dtk\proj\symbols.txt tools\melee\config\GALE01\symbols.txt > symbols\verify_report.txt` |
| `from_scratch.ps1` | Everything from a fresh clone + your ISO, PASS/FAIL table, writes `BUILD_REPORT.md` ([REPRODUCE](REPRODUCE.md)) |
| `setup.ps1` | Downloads every tool at the version in `pins.txt` (SHA-256 checked), extracts the disc |
| `checksums.ps1` | SHA-256 of every output vs `docs\expected_checksums.txt` (`-Compare`, `-Write`) |
| `batches\run_all.ps1` | Runs every batch in order (1-12) and ends with `checksums.ps1 -Compare`; see "Batches" below |
| `layout_match.py` | Library names by layout proof against Melee, TP, TWW, MKDD, Pikmin 2, TTYD |
| `clean_headers.py` | Declarations-only copy of Melee's HSD/SDK headers for Ghidra's parser; Custom Robo overrides (`HSD_PObj`) |
| `check_class_sizes.py` | Parsed HSD struct sizes vs the sizes Custom Robo registers for each class |
| `lint_descriptive.py` | Checks `symbols\descriptive.txt` and `cr_globals.txt` (cr_ prefix, why note, no clashes) |
| `crosscheck_dtk.py` / `game_map.py` / `build_index.py` | dtk cross-check / game-code map / SQLite search index |
| `find.py` | Search the decompile: `text`, `func [--brief]`, `callers`, `callees`, `refs`, `str`, `hint` |
| `robo_joints.py` | Robo joint roles (blocks B/C): `table`, `roles`, `path`, `selftest` (170/170 byte-identical) |
| `make_mod.py` | Mod folder (models, files, code patches) -> playable ISO + `.crpatch`; syncs main.dol entry tables ([MODDING](MODDING.md)) |
| `iso_patch.py` | `build` / `diff` / `apply` / `info`: modded ISO and data-free patches |
| `robo_swap.py` | Swap two robos completely (`--model-only`, `--keep-motion`, `--size X`) |
| `struct_fields.py` | Finds typed fields inside a `cr_` struct's unknown stretches from the C's accesses (`report` / `apply`) |
| `name_evidence.py` | Harvests the game's own names (pointer tables to strings, identifier / format / source-file strings + their users) -> `docs/name_evidence.md` |
| `match_game_names.py` | Batch 7: applies the game's own names where proven (G1 parallel name/function tables, G2 self-naming messages, G3 source-file module notes), from `main.dol` bytes only -> `symbols/game_real.txt`, leftovers `symbols/game_unnamed_real.txt` |
| `boot_check.ps1` | Boots an ISO (or a folder's `sys\main.dol`) in Dolphin (`-DolphinDir`, default set in the script; used by batch 11), reports warning dialogs (`-Snapshot` saves a picture of one, `-Screenshot` the game window, `-Log` Dolphin's log of that run) |
| `reloc_audit.py` | Library: checks dtk's relocations in a linked `main.elf` against main.dol's bytes (missed pointers, numbers relocated as pointers, glue spans); used by the two below |
| `gen_dtk_relocs.py` | Batch 9: writes the generated relocation fixes into `decomp\config\GXCE01\config.yml`, `--check` checks the build (`decomp\reports\relocs_report.txt`) |
| `shift_test.py` | Batch 9: pads code and data at 30 points (incl. the proven data-object boundaries; `--all-data`: all of them) in a scratch copy of the build, relinks, checks every relocation (`decomp\reports\shift_report.txt`); batch 11 boots the result |

## Naming tools (batches 1, 6, 7, 8)

| Tool | Purpose |
|---|---|
| `layout_evidence.py` | Library module: independent call-graph and string evidence for library names, ours vs the reference decomps' sources (used by `layout_match.py` and `audit_layout.py`) |
| `audit_layout.py` | Audits the layout-proof names in `symbols\symbols.txt` by replaying the layout match; writes `symbols\layout_audit.txt` (`python -I tools\audit_layout.py`) |
| `export_struct.py` | Library module: structure-only view of the Ghidra export (function starts, references, strings by address) for the name matchers |
| `dol_asserts.py` | Library module: assert / panic calls of main.dol from the instruction bytes (function, file string, expression string), used by `match_lib_hsd.py` |
| `match_lib_sdk.py` | Batch 6: names the remaining SDK functions in 0x80131000..0x8015F000 (OS, DVD, VI, PAD, AI/AR/ARQ/DSP, CARD, GX, MTX, TRK) from reference decomps |
| `match_lib_rt.py` | Batch 6: names for the Metrowerks runtime / MSL C / math / EXI / SI range 0x8015F000-0x8016D408 (rules R3, R5, R6) |
| `match_lib_hsd.py` | Batch 6: names the remaining sysdolphin (HSD) functions from 0x8016D408 on, from independent evidence |
| `merge_lib_candidates.py` | Batch 6 helper: merges `symbols\candidates_*.txt` into `symbols.txt` (tagged `lib-candidate`) after checking each line |
| `crosscheck_names.py` | Batch 8: independent accuracy cross-check of every name, library and game (`[--symbols FILE] [--out FILE.txt] [--md FILE.md]`) -> `symbols\names_crosscheck.txt`, `docs\names_crosscheck.md` |

## Phase 4 build tools (batches 9, 10, 12)

| Tool | Purpose | Usage |
|---|---|---|
| `setup_decomp.ps1` | Phase 4 build setup (setup step 8): the pinned `encounter/dtk-template` -> `decomp\` with our configure patch, the pinned build tools, your `main.dol` | run by `setup.ps1` |
| `gen_dtk_symbols.py` | Batch 9: `decomp\config\GXCE01\symbols.txt` = dtk's analysis of main.dol with our verified names -> `decomp\reports\symbols_report.txt` | run by batch 9 |
| `gen_dtk_splits.py` | Batch 9: `decomp\config\GXCE01\splits.txt`, object (`.c` file) boundaries only where proven, incl. data placed by the matched C and the `game/` objects of `decomp\src\game` (method: its docstring) -> `decomp\reports\splits_report.txt` | run by batch 9 |
| `gen_c_symbols.py` | Batch 9: gives the objects built from C their own symbols in the config `symbols.txt` (objdiff pairs data by name) -> `decomp\reports\c_symbols_report.txt` | run by batch 9 |
| `check_c_objects.py` | Batch 9: every object linked from C is 100% code + data in objdiff, every other C file is in `CR_NOT_MATCHING` -> `decomp\reports\progress_report.txt`, `sources_report.txt` | `python -I tools\check_c_objects.py --link-each` (by hand: links one more C object, prints the SHA-1) |
| `check_c_callsites.py` | Batch 9: names from the matched C's call sites and definitions (rules R7 / R8); checks every `// c-callsite` / `// c-define` line -> `decomp\reports\c_callsites_report.txt` | run by batch 9 |
| `import_ref_source.py` | Copies a C source and the headers it includes from a pinned reference decomp into `decomp\src` / `decomp\include`, provenance on line 1 | `python -I tools\import_ref_source.py <ref> <path in ref> src/<object path> --inc <dir>=include/<ref>` ([decomp](decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026)) |
| `make_configure_patch.py` | Rewrites `tools\patches\dtk_template_configure.patch` from the edited `decomp\configure.py` (and checks template + patch == file) | `python -I tools\make_configure_patch.py` |
| `compiler_probe\probe.py` | Batch 10: which CodeWarrior version and flags built each part of main.dol ([compiler](compiler.md)) | `python -I tools\compiler_probe\probe.py --show FUNCTION --version V --flags "..."` |
| `draft_batch.py` | Batch 12: m2c C draft of every game function -> compile with the game compiler -> compare with the original ([drafts](drafts.md)) | `python -I tools\draft_batch.py [--only NAME ...] [--sample N] [--jobs N]` |
| `promote_drafts.py` | Batch 12: puts every 100% draft that can be linked into `decomp\src\game\<function>.c` -> `decomp\reports\promote_report.txt` (method: its docstring) | run by batch 12 |

## Batches (`tools\batches`)

Each can run alone (`pwsh tools\batches\<name>.ps1`); `run_all.ps1` runs all of them in order ([REPRODUCE](REPRODUCE.md)).

| Batch | Purpose |
|---|---|
| `run_all.ps1` | The one script: names (1 + 6 + 7) <-> Ghidra rebuild to a fixpoint -> 5 -> 2 -> 3 -> 4 -> 8 -> 9 -> 10 -> 11 -> 12 -> `checksums.ps1 -Compare` (about 17 min; `-SkipRebuild`, `-MaxPasses N`) |
| `batch1_names.ps1` | Library names by layout proof (naming rule 3), then verify |
| `batch2_crosscheck.ps1` | Compares Ghidra's C export with decomp-toolkit's analysis -> `symbols\crosscheck_report.txt` |
| `batch3_game_map.ps1` | Maps the game's own code (callers, callees, strings, asserts, subsystem hints) -> `docs\game_map.md`, `symbols\game_map.tsv` |
| `batch4_index.ps1` | Search index of the whole decompile and one file per function -> `decompiled\index.db`, `decompiled\funcs\` |
| `batch5_types.ps1` | Checks the types the rebuild applied (`symbols\types_report.txt`, `check_class_sizes.py`) |
| `batch6_lib_candidates.ps1` | Names the remaining library functions (rules R3-R6) with the three `match_lib_*.py`, merged as `lib-candidate` |
| `batch7_game_names.ps1` | Game-code naming stage 1: the game's own names (`match_game_names.py`), merged into `symbols\descriptive.txt` as `game-real` |
| `batch8_crosscheck.ps1` | Independent accuracy cross-check of every name (`crosscheck_names.py`); stops on any FAIL |
| `batch9_decomp_build.ps1` | The Phase 4 build: config, splits, objects from C, main.dol == retail SHA-1, relocation check, static shift test |
| `batch10_compiler_probe.ps1` | Compiler probe (`compiler_probe\probe.py`, about 5 minutes) |
| `batch11_shift_test.ps1` | Boots the shifted build in Dolphin (twice, about 1 minute; don't type while it runs) |
| `batch12_drafts.ps1` | m2c drafts of the game code, compiled and compared, then `promote_drafts.py` (and batch 9 again when `decomp\src\game` changed) |

## Ghidra scripts (`tools\scripts`)

| Script | Role |
|---|---|
| `EnableGCAnalyzer.java` | pre-script: turns on the GameCube loader's "(GameCube/Wii) Program Analyzer" (sets r13/r2), which stays off by default because its format-name check no longer matches the loader |
| `ApplySymbols.java` | applies names from a dtk-style symbols file (skips generic `fn_`/`lbl_` names) |
| `DefineStrings.java` | defines null-terminated ASCII strings (≥ 4 chars) in non-executable memory |
| `ExportDecompiledC.java` | decompiles every function into one C file |
| `CreateDtkFunctions.java` | creates functions at starts decomp-toolkit found and Ghidra missed |
| `ExportXrefs.java` | functions / references / strings as TSV (for the search index) |
| `ParseHeaders.java` | parses HSD/SDK headers + `symbols/cr_types.h`, checks sizes/offsets, applies prototypes and typed globals |

## Third-party tools (installed under `tools\`)

| Tool | Version | Notes |
|---|---|---|
| Ghidra | 12.1.4 (`ghidra_12.1.4_PUBLIC`) | needs Java 21 (installed system-wide) |
| Ghidra GameCube Loader | 1.3.1 (built for 12.1) | `extension.properties` version patched 12.1 → 12.1.4. Headless needs `-loader-autoloadMaps false` or it hangs on a hidden dialog. |
| HSDLib / HSDRawViewer | built from source (`tools\HSDLib`, base commit in `tools\patches\hsdlib_base_commit.txt`) | targets .NET 8; runtimeconfig set to `rollForward: Major` to run on the installed .NET 10. Our patch (`tools\patches\hsdlib_headless_export_resave.patch`) adds `--export <out_dir> <glb|dae|smd> <files…>` and `--resave <in.dat> <out.dat>`. After every rebuild the runtimeconfig roll-forward line must be re-added. |
| decomp-toolkit (dtk) | 1.8.4 (`tools\dtk\dtk.exe`) | analysis project in `tools\dtk\proj` (`symbols.txt`, `splits.txt`) |
| Melee decomp | sparse clone (`tools\melee`) | `config`, `src\sysdolphin`, `src\MSL`, `src\Runtime`, `src\MetroTRK` and the SDK headers (docs/REPRODUCE.md, setup step 3) |
| Other reference decomps | sparse clones (`tools\refs\`) | MKDD, Pikmin 2, TP, TTYD, TWW, KAR (doldecomp and wowjinxy), XD, dolsdk2001 / 2004, AC, SMS, Mario Party 4, Pikmin, BfBB: commits in `tools\pins.txt` |
| m2c | pinned commit (`tools\m2c`) | batch 12 drafts; plain Python |
| Phase 4 build tools | dtk-template, ninja 1.13.2, objdiff-cli 3.6.1, gc-wii-binutils 2.42-2, sjiswrap 1.2.2, wibo 1.0.3, CodeWarrior `compilers_20251118` | `tools\ninja\`, `tools\refs\dtk-template\`, `decomp\build\{tools,binutils,compilers}\` ([INSTALL](INSTALL.md#phase-4-build-tools-decomp-batch-9)) |
| Python venv | `tools\pyenv` | trimesh, matplotlib, numpy, pygltflib, pillow |

## Rebuilding HSDRawViewer

```
cd tools\HSDLib
dotnet build HSDRawViewer\HSDRawViewer.csproj -c Release
# then add  "rollForward": "Major",  under "runtimeOptions" in
# HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.runtimeconfig.json
```
On a fresh clone: `git apply ..\patches\hsdlib_headless_export_resave.patch` first.
