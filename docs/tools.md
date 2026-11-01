# Tools

Everything in `tools/`, grouped by the workflow it belongs to ([workflows.md](workflows.md)). Run from the repo root.
Durations are from the development desktop (8 cores, Oct 31, 2026); "quick" is under a minute.

- **Python tools** (`tools/*.py`) run with the system Python 3.11+ in isolated mode: on Windows
  `python -I tools\x.py ...`, on Linux / macOS `python3 -I tools/x.py ...`. The tables below give the script and its
  arguments only (`tools/find.py text chara_bin`); put your system's `python -I` / `python3 -I` in front. Paths in
  the arguments are written with `/`, which Python accepts on Windows too. `render_preview.py` needs the venv
  `tools/pyenv` (trimesh, matplotlib): `tools\pyenv\Scripts\python` on Windows, `tools/pyenv/bin/python` on Linux /
  macOS. The Blender scripts (`tools/blender/*.py`) run inside Blender, never with `python -I`.
- **Orchestration scripts** exist twice, `.ps1` for Windows (PowerShell 7: `pwsh tools\x.ps1`) and `.sh` for Linux /
  macOS (bash: `tools/x.sh`): same steps, same arguments in each shell's spelling (`-Compare` / `--compare`), same
  output files. `tools/common.sh` holds the helpers the `.sh` scripts share (pinned downloads, SHA checks, CRLF output,
  the PowerShell-order sort). The `.ps1`-only tools (`boot_check.ps1`, `boot_test.ps1`, batch 11) drive Dolphin
  through the Win32 API and are Windows-only. **The `.sh` twins are not maintained at the moment** (last run Oct 30,
  2026); Windows is the verified system.
- Every script's own header (the `#` block of a `.ps1`, the docstring of a `.py`) is the authoritative usage text;
  `--help` is only where a script uses argparse (`draft_batch.py`, `compiler_probe/probe.py`, `revisions.py`, `sources.py`).

## Setup and the full run

| Tool | Purpose | Usage (Windows / Linux, macOS) | Duration |
|---|---|---|---|
| `from_scratch` | Everything from a fresh clone + your ISO: setup, `run_all`, archives, models, glTF, a preview, the mod test build and a patch round trip; PASS/FAIL table, writes `BUILD_REPORT.md` ([REPRODUCE](REPRODUCE.md)) | `pwsh tools\from_scratch.ps1 -Iso <iso> [-Dolphin <folder>]` / `tools/from_scratch.sh <iso>` | about an hour |
| `setup` | Checks the tools on `PATH`, checks the ISO's SHA-1, downloads every tool at the version in `tools/pins.txt` (SHA-256 checked), extracts the disc (Windows: DolphinTool; Linux / macOS: the pinned dtk), clones the references, HSDLib (patched, built with `-p:RollForward=Major`) and m2c, runs dtk's analysis, makes the venv, unpacks Ghidra, then `setup_decomp`. Rerunnable: finished steps are skipped | `pwsh tools\setup.ps1 -Iso <iso> [-Dolphin <folder>]` / `tools/setup.sh <iso> [DolphinTool]` | 5-15 min (downloads) |
| `setup_decomp` | Phase 4 build setup (setup step 8): the pinned `encounter/dtk-template` -> `decomp/` with our configure patch (`decomp/configure.py` is regenerated on every run), the pinned build tools, your `main.dol` -> `decomp/orig` | `pwsh tools\setup_decomp.ps1` / `tools/setup_decomp.sh` (run by `setup` and batches 9 / 10) | quick after the first download |
| `check_refs.py` | The reference checkouts (`tools/melee`, `tools/refs/*`, `tools/m2c`, `tools/HSDLib`) are the setup's: pinned commit and exactly the sparse paths of `tools/setup.ps1` / `setup.sh` (a clone made before a list was widened gives other names, plan.md step 68b) | `tools/check_refs.py` (run by `run_all` and batch 9) | quick |
| `rebuild_decomp` | Fresh Ghidra import + GameCube analyzer + names + strings + types + C export ([decomp](decomp.md)); deletes the old export first and stops unless both exports completed. **Only in the main checkout** (a worktree's `decompiled/` is a junction to it) | `pwsh tools\rebuild_decomp.ps1` / `tools/rebuild_decomp.sh` | about 4 min |
| `batches/run_all` | Runs every batch in order (1-12) and ends with the checksum compare; see "Batches" below | `pwsh tools\batches\run_all.ps1 [-SkipRebuild] [-MaxPasses 5]` / `tools/batches/run_all.sh [--skip-rebuild] [--max-passes 5]` | 36 min (2,138 s, 1 Ghidra rebuild, Oct 31, 2026) |
| `checksums` | SHA-256 of the 33 outputs vs `docs/expected_checksums.txt` (the list with the batch that writes each file is in the script) | `pwsh tools\checksums.ps1 [-Compare \| -Write]` / `tools/checksums.sh [--compare \| --write]` | quick |
| `check_docs.py` | Checks the docs: links and anchors, `docs/...` paths named in scripts, the index, CRLF, one OS per command block ([below](#checking-the-docs)) | `tools/check_docs.py` (before every doc commit) | quick |
| `common.sh` | Linux / macOS only: helpers sourced by every `.sh` script: `pinned_file` / `pinned_clone` (tools/pins.txt), SHA-1/256, `crlf`, `ps_sort`, the reference-file order | sourced | |
| `ps_sort.py` | Sorts lines like PowerShell's `Sort-Object` on Windows (culture order), so the `.sh` batches write the same report files as the `.ps1` ones | `tools/ps_sort.py [-u] < lines` | |

## Naming (batches 1, 6, 7, 8 and the hand tools)

| Tool | Purpose | Usage |
|---|---|---|
| `layout_match.py` | Batch 1: library names by layout proof against the pinned reference decomps (Melee, TP, TWW, MKDD, Pikmin 2, TTYD, ...) -> `symbols/layout_candidates.txt` | run by batch 1 |
| `verify_symbols.py` | Batch 1: checks every name in `symbols/symbols.txt` against the reference decomps' symbols (existence + size; the closest reference wins) -> `symbols/verify_report.txt` | `tools/verify_symbols.py <symbols.txt> <dtk symbols.txt> <reference symbols.txt> [more ...]` (batch 1 passes Melee's and every `tools/refs/*/config/*/symbols.txt`) |
| `layout_evidence.py` | Library module: independent call-graph and string evidence for library names, ours vs the reference decomps' sources (used by `layout_match.py` and `audit_layout.py`) | module |
| `audit_layout.py` | Audits the layout-proof names in `symbols/symbols.txt` by replaying the layout match -> `symbols/layout_audit.txt` | `tools/audit_layout.py` |
| `export_struct.py` | Library module: structure-only view of the Ghidra export (function starts, references, strings by address) for the name matchers | module |
| `dol_asserts.py` | Library module: assert / panic calls of main.dol from the instruction bytes (function, file string, expression string), used by `match_lib_hsd.py`, `match_game_names.py`, `reloc_audit.py` | module |
| `match_lib_sdk.py` | Batch 6: names the remaining SDK functions in 0x80131000..0x8015F000 (OS, DVD, VI, PAD, AI/AR/ARQ/DSP, CARD, GX, MTX, TRK) from reference decomps -> `symbols/candidates_sdk.txt` | run by batch 6 |
| `match_lib_rt.py` | Batch 6: the Metrowerks runtime / MSL C / math / EXI / SI range 0x8015F000-0x8016D408 (rules R3, R5, R6) -> `symbols/candidates_rt.txt`, `lib_unnamed_rt.txt` | run by batch 6 |
| `match_lib_hsd.py` | Batch 6: the remaining sysdolphin (HSD) functions from 0x8016D408 on, from independent evidence (Melee's sources, KAR's asm) -> `symbols/candidates_hsd.txt` | run by batch 6 |
| `merge_lib_candidates.py` | Batch 6 helper: merges `symbols/candidates_*.txt` into `symbols.txt` (tagged `lib-candidate`) after checking each line | `tools/merge_lib_candidates.py <accepted out> <replaced out> <report part out>` (run by batch 6) |
| `match_game_names.py` | Batch 7: the game's own names where proven (G1 parallel name/function tables, G2 self-naming messages, G3 source-file module notes), from `main.dol` bytes only -> `symbols/game_real.txt`, leftovers `symbols/game_unnamed_real.txt` | run by batch 7 |
| `lint_descriptive.py` | Checks `symbols/descriptive.txt` and `cr_globals.txt` (cr_ prefix, why note, no clashes, function starts) | `tools/lint_descriptive.py` |
| `crosscheck_names.py` | Batch 8: independent accuracy cross-check of every name, library and game -> `symbols/names_crosscheck.txt`, `docs/names_crosscheck.md`; exit 1 on any FAIL | `tools/crosscheck_names.py [--symbols FILE] [--out FILE.txt] [--md FILE.md]` |
| `base_scan.py` | The base-register scan of the game code (steps 79, 81, 90) from batch 9's asm: per data object the loads / stores through a register holding its address (or, `*SYM`, through a pointer variable's value): offset, width, sign, functions | `tools/base_scan.py top 40 --default`, `tools/base_scan.py fields lbl_80329B60 '*lbl_8044F348'` |
| `name_evidence.py` | Harvests the game's own names (pointer tables to strings, identifier / format / source-file strings + their users, the body codes) and the hand-kept leads -> `docs/name_evidence.md`, `docs/name_tables.tsv` (by hand, not a batch) | `tools/name_evidence.py` |
| `struct_fields.py` | Finds typed fields inside a `cr_` struct's unknown stretches from the C's accesses (`report` -> `docs/struct_<struct>.md`, `apply` rewrites `cr_types.h`) | `tools/struct_fields.py cr_Chara report` |
| `match_hsd_asserts.py` | The first HSD names (Oct 25, 2026): `__assert` (file, expression) calls against the Melee decomp -> `symbols/melee_matches.txt` (historic) | `tools/match_hsd_asserts.py decompiled/main_dol.c "Custom Robo extracted/sys/main.dol" tools/melee/src/sysdolphin symbols/melee_matches.txt` |
| `crosscheck_dtk.py` / `game_map.py` / `build_index.py` | Batches 2 / 3 / 4: dtk cross-check (`symbols/crosscheck_report.txt`) / game-code map (`docs/game_map.md`, `symbols/game_map.tsv`) / SQLite search index (`decompiled/index.db`, `decompiled/funcs/`) | run by the batches |
| `find.py` | Search the decompile ([workflows](workflows.md#5-read-and-search-the-code)): `text`, `func [--brief]`, `callers`, `callees`, `refs`, `str`, `hint` | `tools/find.py func cr_Archive_OpenEntry --brief` |
| `clean_headers.py` | Declarations-only copy of Melee's HSD/SDK headers for Ghidra's parser; Custom Robo overrides (`HSD_PObj`) -> `build/headers` (run by `rebuild_decomp`) | `tools/clean_headers.py <melee dir> <out dir>` |
| `check_class_sizes.py` | Batch 5: parsed HSD struct sizes vs the sizes Custom Robo registers for each class | `tools/check_class_sizes.py <ParseHeaders report> [decompiled/main_dol.c]` |

## The Phase 4 build (batch 9) and the compiler probe (batch 10)

| Tool | Purpose | Usage |
|---|---|---|
| `gen_dtk_symbols.py` | Batch 9: `decomp/config/GXCE01/symbols.txt` = dtk's analysis of main.dol with our verified names (only names change; plus the cuts of `symbols/data_splits.txt`, step 90: a dtk data object ends at the cut and `lbl_<address>` takes the rest) -> `decomp/reports/symbols_report.txt` | run by batch 9 |
| `sync_game_names.py` | Batch 9, before the link: a promoted game file (`decomp/src/game`, marker line of `promote_drafts.py`) that still calls a function by the name it had before this run's `gen_dtk_symbols.py` gets the new name (plan.md step 68b); batch 12 writes the file again | `tools/sync_game_names.py <previous config symbols.txt>` (run by batch 9) |
| `gen_dtk_splits.py` | Batch 9: `decomp/config/GXCE01/splits.txt`, object (`.c` file) boundaries only where proven, incl. data placed by the matched C, the `game/` objects of `decomp/src/game`, the file objects' pools and the data-only game units of `decomp/src/game/data` (method: its docstring) -> `decomp/reports/splits_report.txt` | run by batch 9 |
| `gen_data_c.py` | Writes a data-only game unit `decomp/src/game/data/<first>_<last>.c` (step 86, [decomp.md "Game data objects"](decomp.md#game-data-objects-step-86-oct-31-2026)): the C definitions of contiguous game globals of `symbols/cr_globals.txt` with their `cr_types.h` types, initialisers generated from main.dol's bytes (numbers, floats, pointers as the names at the relocation targets, `&name[i]` for an element of a typed global of ours; step 90), declared in the order the compiler lays the section out; every definition under `#pragma force_active` (mwld strips data that nothing names); compiled and placed with `gen_dtk_splits.py`'s rule before it is written. A new type in `cr_types.h` needs a fresh context first (`tools/draft_batch.py --out decomp/build/ctxgen --sample 1 --no-summary`, then copy its `ctx_pp.c`). Needs the drafts' context (`decomp/drafts/ctx_pp.c`, batch 12) | `tools/gen_data_c.py NAME ...` (one unit), `--all` (every unit of its UNITS list again, after a type or name change), `--check` (verify, write nothing) |
| `gen_dtk_relocs.py` | Batch 9: writes the generated relocation fixes into `decomp/config/GXCE01/config.yml`; `--check` checks the build -> `decomp/reports/relocs_report.txt` | `tools/gen_dtk_relocs.py [--check]` (run by batch 9) |
| `reloc_audit.py` | Library: checks dtk's relocations in a linked `main.elf` against main.dol's bytes (missed pointers, numbers relocated as pointers, glue spans); used by `gen_dtk_relocs.py` and `shift_test.py` | module |
| `gen_c_symbols.py` | Batch 9: gives the objects built from C their own symbols in the config `symbols.txt` (objdiff pairs data by name) -> `decomp/reports/c_symbols_report.txt` | run by batch 9 |
| `check_c_objects.py` | Batch 9: every object linked from C is 100% code + data in objdiff, every other C file is in `CR_NOT_MATCHING` with its reason -> `decomp/reports/progress_report.txt` (the per-library numbers quoted everywhere), `sources_report.txt` (provenance) | `tools/check_c_objects.py [--link-each]` (`--link-each`, by hand: links one more C object and prints the SHA-1) |
| `check_c_callsites.py` | Batch 9: names from the matched C's call sites and definitions (rules R7 / R8); checks every `// c-callsite` / `// c-define` line -> `decomp/reports/c_callsites_report.txt` | run by batch 9 |
| `shift_test.py` | Batch 9 (last step): pads code and data at 30+ points (incl. the proven data-object boundaries and the start of every data-only game unit outside a glue span; `--all-data`: all of them) in a scratch copy of the build, relinks, checks every relocation -> `decomp/reports/shift_report.txt`, `decomp/build/shift/main.dol`; `--expect` prints what batch 11 must see | `tools/shift_test.py [--pad 0x100] [--all-data] [--expect]` |
| `import_ref_source.py` | Copies a C source and the headers it includes from a pinned reference decomp into `decomp/src` / `decomp/include`, provenance on line 1 | `tools/import_ref_source.py <ref> <path in ref> src/<object path> --inc <dir>=include/<ref> [--force] [--no-headers]` ([decomp](decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026)) |
| `make_configure_patch.py` | Rewrites `tools/patches/dtk_template_configure.patch` from the edited `decomp/configure.py` (and checks template + patch == file). Run after every edit of `configure.py` (the file itself is gitignored) | `tools/make_configure_patch.py` |
| `compiler_probe/probe.py` | Batch 10 (about 5 min): which CodeWarrior version and flags built each part of main.dol ([compiler](compiler.md)) -> `tools/compiler_probe/out/{summary.txt,results.tsv}` (gitignored) | `tools/compiler_probe/probe.py [--jobs N] [--lib ...] [--keep 0.5]`; one function: `--show FUNCTION --version V --flags "..."` |
| `compiler_probe/revisions.py` | Which library source revisions built main.dol: version strings, every reference's copy of each file compiled and compared, HSD vs KAR's asm ([compiler.md "Source revisions"](compiler.md#source-revisions-oct-30-2026)) -> `tools/compiler_probe/out/revisions.txt` | `tools/compiler_probe/revisions.py [--jobs N] [--part strings,sdk,msl,trk,hsd]` (by hand, about a minute) |
| `compiler_probe/sources.py` | Source hunt for MetroTRK / MSL / Runtime: which public source compiles to the exact bytes, with the per-file flag variants ([compiler.md "Source hunt"](compiler.md#source-hunt-oct-30-2026)) -> `out/sources.txt`; `--fetch` widens an old clone | `tools/compiler_probe/sources.py [--fetch] [--jobs N] [--part sdk,trk,msl]` (by hand, about a minute) |
| `compiler_probe/hsd_hunt.py` | Source hunt for HSD: every function of the HSD range against our ports, Melee, wowjinxy/KAR, doldecomp/kar (C and asm), classified by the kind of difference -> `out/hsd_hunt.txt` | `tools/compiler_probe/hsd_hunt.py` (by hand, about a minute) |
| `compiler_probe/game/*.c` | Hand-written C for a few tiny game functions, probed like the libraries (`fn_XXXXXXXX` = the address) | read by `probe.py` |

## Game-code drafts (batch 12)

| Tool | Purpose | Usage |
|---|---|---|
| `draft_batch.py` | Batch 12 (about 26 min): m2c C draft of every game function (context: HSD / `cr_types.h` + every `lbl_` data object with its size + the game functions' prototypes from a first pass; m2c runs through a small wrapper that adds the load width to the fields it cannot place) -> post-processed by the generator's rules, repaired on the compiler's messages, compiled with the game flags of `decomp/configure.py`, compared with the original, aligned; every draft below 100% also with `--reg-vars` and the variants ([drafts](drafts.md)) -> `decomp/drafts/` (gitignored), `decomp/reports/draft_summary.txt` | `tools/draft_batch.py [--only NAME ...] [--sample N] [--jobs N] [--out DIR] [--no-summary]` (`--only` / `--sample` reuse `ctx_protos.h` / `ctx_sdk.h` of the output folder, `decomp/drafts` by default; `--out` keeps an experiment apart from the last full run: copy those two files there first; `--no-summary` leaves `draft_summary.txt` alone) |
| `promote_drafts.py` | Batch 12: puts every 100% draft that can be linked into `decomp/src/game/<function>.c`, and the drafts of a whole file run (functions sharing a constant pool, step 63) into a file object `<first>_<last>.c`; keeps a file whose draft fell below 100% while it links, drops it when its name or address is no longer its function's -> `decomp/reports/promote_report.txt` (method: its docstring) | `tools/promote_drafts.py [--check --try-run FIRST LAST]` (run by batch 12; `--try-run` tests one run) |

## The scheduler replay (docs/sched_model.md)

| Tool | Purpose | Usage |
|---|---|---|
| `sched/corpus.py` | Compiles the scheduler test programs (tools/sched/scratch: hd/m/*.c, hd/base.c, hd/v0.c, fr/v13.c) with GC/2.6 and the game flags of `decomp/configure.py`, with and without `schedule`, disassembles both with dtk and keeps the listings per function plus the original words of cr_CM_DUMMY_008 / fn_8006EEF8 -> `tools/sched/corpus/` (gitignored, `manifest.json`) | `tools/sched/corpus.py [--only ID ...]` (about 30 s) |
| `sched/replay.py` | The GC/2.6 scheduler replay (DAG, heights, the urgency / release count / height / rank pick, the 750 and 603 machine models read from the compiler binary, both passes); `--corpus` measures it against the corpus -> `tools/sched/corpus_report.txt`; `--trace ID` prints every first-pass pick of one corpus function | `tools/sched/replay.py --corpus` / `--trace ID [FUNCTION] [--model 603]` (quick) |
| `sched/ghidra_scripts/SchedExport.java` | Names the scheduler functions in an analysis Ghidra project of the compiler (`tools/sched/ghidra/`) and exports their decompiled C to `tools/sched/ghidra_export/` (both gitignored) | headless: `-postScript SchedExport.java <out.c> <addr:name> ...` |

## Models, archives and mods (Phase 1)

| Tool | Purpose | Usage |
|---|---|---|
| `sfd.py` | SFD archives: unpack / pack / byte-identical verify ([format](formats/sfd.md)) | `tools/sfd.py unpack "Custom Robo extracted/files" unpacked`<br>`tools/sfd.py pack <entry_dir> <out.BIN>`<br>`tools/sfd.py verify "Custom Robo extracted/files" unpacked` |
| `crmodel.py` | Model entry <-> standard .dat ([format](formats/model_entry.md)) | `tools/crmodel.py to-dat <entry.bin> <out.dat>`<br>`tools/crmodel.py from-dat <in.dat> <template_entry.bin> <out_entry.bin>`<br>`tools/crmodel.py roundtrip unpacked/chara unpacked/weapon_gun ...` |
| `extract_models.py` | Every known model -> `models_dat/<category>/*.dat` (1,006) | `tools/extract_models.py unpacked "Custom Robo extracted/files" models_dat` |
| `export_models.py` | `models_dat/` -> `models_export/<category>/<name>/` via HSDRawViewer (`.glb`, `.dae` fallback); writes `export_log.txt`. Windows only; the longest step of `from_scratch` | `tools/export_models.py models_dat models_export` |
| `render_preview.py` | Front + side flat-shaded PNG of a `.glb` (sanity check); runs with the venv's Python (above) | `tools/render_preview.py <model.glb> <out.png>` |
| `cr_model_explore.py` | Prints a model entry's header, reloc stats and pointer graph (research) | `tools/cr_model_explore.py unpacked/chara/000.bin 5` |
| `robo_joints.py` | Robo joint roles (blocks B/C): `table`, `roles`, `path`, `selftest` (170/170 byte-identical) | `tools/robo_joints.py roles unpacked/chara/000.bin` |
| `model_loop` | [Blender loop](MODDING.md#blender-loop): export -> `tools/blender/<script>.py` -> `HSDRawViewer --import` -> `make_mod.py` -> ISO + `.crpatch` (+ boot). Export / import need Windows (HSDRawViewer); Blender from `-Blender`, `$env:BLENDER`, `PATH`, or the newest `C:\Program Files\Blender Foundation\Blender *` | `pwsh tools\model_loop.ps1 -Mod <mod> -Script <script> -Entries 000[,001] [-Category bg_data [-Models 0,3]] [-Iso <iso>] [-Blender <exe>] [-Fresh] [-Boot] [-Seconds 40]` / `tools/model_loop.sh <mod> <script> 000 [--category bg_data] [--models 0,3] [--boot]` |
| `blender/*.py` | Blender scripts of the loop, run by Blender's own Python: `common.py` (load / save settings), `roundtrip.py` (no edit), `legionnaire.py` (the step-70 test edit), `legionnaire_v2.py` (step 73 / 74: a complete procedural legionnaire with 3 new bones, baked textures packed into one atlas, smooth skinning; `legionnaire_v2_template.py` the same on the template skeleton), `stage_pillar.py` (step 75: a pillar on Basic Arena, model 0 of `bg_data_000`), `render.py` (multi-view preview PNG of a glb) | `blender -b --factory-startup --python tools/blender/roundtrip.py -- <in.glb> <out.glb> [<roles.json>]`<br>`blender -b --factory-startup --python tools/blender/render.py -- <in.glb> <out.png> [pose.json] [joints.json]` |
| `make_mod.py` | Mod folder (models, files, code patches) -> playable ISO + `.crpatch`; syncs main.dol's entry tables (bg_data has none: the game reads its header), checks the body (0x3A000) and stage (0x100000 / 0x8000) load-buffer sizes ([MODDING](MODDING.md)) | `tools/make_mod.py mods/my_mod [<clean.iso>]` |
| `iso_patch.py` | `build` / `diff` / `apply` / `info`: modded ISO and data-free patches | `tools/iso_patch.py apply <clean.iso> <mod.crpatch> <out.iso>` |
| `robo_swap.py` | Swap two robos completely (`--model-only`, `--keep-motion`, `--size X`): writes `mods/swap_robo<A>_robo<B>/` and builds it | `tools/robo_swap.py <A> <B> [<clean.iso>] [--model-only] [--keep-motion] [--size X]` |
| `make_test_build.py` | Copies the extracted disc to a build folder and rebuilds chosen `chara` entries through the full import path (red/blue texture swap + 0x100 data shift); `from_scratch` runs it for entries 0-4 | `tools/make_test_build.py "Custom Robo extracted" unpacked test_build 0 1 2 3 4` |
| `boot_check.ps1` | Windows: boots an ISO (or a folder's `sys\main.dol`) in Dolphin (`-DolphinDir`, else `CR_DOLPHIN_DIR`, else the pinned `tools/dolphin/Dolphin-x64` that `setup.ps1` downloads; used by batch 11 and `model_loop -Boot`), reports warning dialogs, exit 1 on one (`-Snapshot` saves a picture of the dialog, `-Screenshot` the game window, `-Log` Dolphin's log of that run, `-PressA 16` the A button every 2 s from 16 s until the title file is read: for a Dolphin without a Custom Robo save). The first-start usage-statistics prompt is answered through command-line config | `pwsh tools\boot_check.ps1 <game.iso or folder\sys\main.dol> [seconds] [-Snapshot x.png] [-Screenshot x.png] [-Log x.log] [-DolphinDir <folder>] [-PressA 16]` |
| `boot_test.ps1` | Windows: boots a build folder in Dolphin (`CR_DOLPHIN_DIR`, else the pinned `tools/dolphin`, else the Project Hive build on this desktop), takes a screenshot after a delay, closes Dolphin. Older than `boot_check.ps1` | `pwsh tools\boot_test.ps1 test_build shot.png 40` |

## Batches (`tools/batches`)

Each batch can run alone: `pwsh tools\batches\<name>.ps1` on Windows, `tools/batches/<name>.sh` on Linux / macOS
(every batch except 11, which is Windows-only and reported as skipped by `run_all.sh`). `run_all` runs all of them in
order ([REPRODUCE](REPRODUCE.md)); the short check of a checkout is batch 9 + batch 8 + `checksums -Compare`
([REPRODUCE, "Verify this checkout"](REPRODUCE.md#verify-this-checkout)). Options, Windows / Linux, macOS: `run_all`
`-SkipRebuild` `-MaxPasses N` / `--skip-rebuild` `--max-passes N`; `batch10_compiler_probe` `-Jobs N` / `--jobs N`;
`batch11_shift_test` `-Seconds 22`; `batch12_drafts` `-Sample N` `-Jobs N` / `--sample N` `--jobs N`.

| Batch (`.ps1` / `.sh`) | Purpose | Duration |
|---|---|---|
| `run_all` | The one script: names (1 + 6 + 7) <-> Ghidra rebuild to a fixpoint -> 5 -> 2 -> 3 -> 4 -> 8 -> 9 -> 10 -> 11 -> 12 -> checksum compare | 36 min with one rebuild (2,138 s on Oct 31, 2026, batch 12 1,552 s of it) |
| `batch1_names` | Library names by layout proof (naming rule 3), then verify | quick |
| `batch2_crosscheck` | Compares Ghidra's C export with decomp-toolkit's analysis -> `symbols/crosscheck_report.txt` | quick |
| `batch3_game_map` | Maps the game's own code (callers, callees, strings, asserts, subsystem hints) -> `docs/game_map.md`, `symbols/game_map.tsv` | quick |
| `batch4_index` | Search index of the whole decompile and one file per function -> `decompiled/index.db`, `decompiled/funcs/` | about a minute |
| `batch5_types` | Checks the types the rebuild applied (`symbols/types_report.txt`, `check_class_sizes.py`) | quick |
| `batch6_lib_candidates` | Names the remaining library functions (rules R3-R6) with the three `match_lib_*.py`, merged as `lib-candidate` | a few minutes |
| `batch7_game_names` | Game-code naming stage 1: the game's own names (`match_game_names.py`), merged into `symbols/descriptive.txt` as `game-real` | quick |
| `batch8_crosscheck` | Independent accuracy cross-check of every name (`crosscheck_names.py`); stops on any FAIL | 10 s |
| `batch9_decomp_build` | The Phase 4 build: `setup_decomp`, `check_refs`, config (symbols, splits, relocs), objects from C, `main.dol` == retail SHA-1, C symbols, fixpoint, the C checks, relocation check, static shift test | about a minute (47 s; 50 s in a fresh worktree, building every object) |
| `batch10_compiler_probe` | Compiler probe (`compiler_probe/probe.py`) | about 5 min |
| `batch11_shift_test` | `.ps1` only (Windows): boots the shifted build in Dolphin (twice; don't type while it runs) -> `decomp/build/shift/boot/` | 1 min (58 s); 2 min (104 s) when the Dolphin has no Custom Robo save (32 s boots with the A button pressed) |
| `batch12_drafts` | m2c drafts of the game code, compiled and compared, then `promote_drafts.py` (and batch 9 again when `decomp/src/game` changed; not with `-Sample`) | about 26 min (1,552 s on Oct 31, 2026; + batch 9) |

## Ghidra scripts (`tools/scripts`)

| Script | Role |
|---|---|
| `EnableGCAnalyzer.java` | pre-script: turns on the GameCube loader's "(GameCube/Wii) Program Analyzer" (sets r13/r2), which stays off by default because its format-name check no longer matches the loader |
| `ApplySymbols.java` | applies names from a dtk-style symbols file (skips generic `fn_`/`lbl_` names) |
| `DefineStrings.java` | defines null-terminated ASCII strings (>= 4 chars) in non-executable memory |
| `ExportDecompiledC.java` | decompiles every function into one C file |
| `CreateDtkFunctions.java` | creates functions at starts decomp-toolkit found and Ghidra missed |
| `ExportXrefs.java` | functions / references / strings as TSV (for the search index) |
| `ParseHeaders.java` | parses HSD/SDK headers + `symbols/cr_types.h`, checks sizes/offsets, applies prototypes and typed globals -> `symbols/types_report.txt` |

## Third-party tools (installed under `tools/` by setup)

Versions are pinned in `tools/pins.txt`; what to install yourself and the versions in use: [INSTALL.md](INSTALL.md).

| Tool | Version | Notes |
|---|---|---|
| Ghidra | 12.1.4 (`tools/ghidra_12.1.4_PUBLIC`) | needs Java 21 (system-wide, or a JDK on `PATH`); headless via `support\analyzeHeadless.bat` (Windows) / `support/analyzeHeadless` |
| Ghidra GameCube Loader | 1.3.1 (built for 12.1) | `extension.properties` version patched 12.1 -> 12.1.4. Headless needs `-loader-autoloadMaps false` or it hangs on a hidden dialog. |
| HSDLib / HSDRawViewer | built from source on Windows (`tools/HSDLib`, base commit 85567e4 in `tools/patches/hsdlib_base_commit.txt`) | targets .NET 8; built with `-p:RollForward=Major` so it runs on the installed .NET 10. Our patch (`tools/patches/hsdlib_headless_export_resave.patch`) adds `--export <out_dir> <glb\|dae\|smd> <files...>`, `--resave <in.dat> <out.dat>`, `--import <template.dat> <edited.glb> <out.dat> [--replace] [--skeleton template\|glb] [--roles f] [--envelope-joint J] [--model N]` (`--model N`: model N of a `scene_data` root, written back in the template's own file layout) and `--info <file.dat>` (joint roots and scene_data models; docs/MODDING.md "Blender loop", "Stages"). Rebuild: below. |
| decomp-toolkit (dtk) | 1.8.4 (`tools/dtk/dtk.exe` on Windows, `tools/dtk/dtk` on Linux / macOS) | analysis project in `tools/dtk/proj` (`symbols.txt`, `splits.txt`); on Linux / macOS also extracts the disc (`dtk disc extract`, byte-identical to DolphinTool) |
| Dolphin | 2609a, official build (`tools/dolphin/Dolphin-x64`, Windows; downloaded by `setup.ps1` whenever it is missing; portable: its `User\` folder, memory card and log stay inside) | its `DolphinTool` extracts the disc on Windows; batch 11 and `boot_check.ps1` boot builds with it (or `-DolphinDir` / `CR_DOLPHIN_DIR`). Stock settings, no cheats, no save: batch 11 presses A past the no-save message |
| Melee decomp | sparse clone (`tools/melee`) | `config`, `src/sysdolphin`, `src/MSL`, `src/Runtime`, `src/MetroTRK` and the SDK headers ([REPRODUCE](REPRODUCE.md), setup step 3) |
| Other reference decomps | sparse clones (`tools/refs/`) | MKDD, Pikmin 2, TP, TTYD, TWW, KAR (doldecomp and wowjinxy), XD, dolsdk2001 / 2004, AC, SMS, Mario Party 4, Pikmin, BfBB: commits in `tools/pins.txt`; `tools/check_refs.py` checks them |
| m2c | pinned commit c632946 (`tools/m2c`) | batch 12 drafts; plain Python, run through `draft_batch.py`'s wrapper (the clone is never changed) |
| Phase 4 build tools | dtk-template, ninja 1.13.2, objdiff-cli 3.6.1, gc-wii-binutils 2.42-2, sjiswrap 1.2.2, wibo 1.0.3 (Linux x86_64 / macOS), CodeWarrior `compilers_20251118` | `tools/ninja/`, `tools/refs/dtk-template/`, `decomp/build/{tools,binutils,compilers}/` ([INSTALL](INSTALL.md#phase-4-build-tools-decomp-batch-9)) |
| Python venv | `tools/pyenv` (Python 3.13 on the development machine) | trimesh, matplotlib, numpy, pygltflib, pillow (`tools/requirements-preview.txt`) |

## Rebuilding HSDRawViewer (Windows)

`setup.ps1` builds it on a fresh clone. After the patch changed (setup only patches a fresh clone), or by hand:

```powershell
cd tools\HSDLib
# an EXISTING clone: back to the pinned tree first
git reset -q; git checkout -q -- .; git clean -fq HSDRawViewer\BatchExport.cs HSDRawViewer\ModelImport.cs HSDRawViewer\ModelInfo.cs
git apply ..\patches\hsdlib_headless_export_resave.patch
dotnet build HSDRawViewer -c Release -v quiet -nologo -p:RollForward=Major
cd ..\..
```

`-p:RollForward=Major` puts `"rollForward": "Major"` into `HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.runtimeconfig.json`
(the .NET 8 program then runs on the installed .NET 10); without the flag the line must be added by hand after every
build. To change the patch: edit the sources in `tools\HSDLib`, then `git -C tools\HSDLib diff > tools\patches\hsdlib_headless_export_resave.patch`
(the patch is what is committed; `tools\HSDLib` is a gitignored clone shared by every worktree).

## Checking the docs

`tools/check_docs.py` (Windows `python -I tools\check_docs.py`, Linux / macOS `python3 -I tools/check_docs.py`)
reads every file git tracks and reports, as `file:line: problem`: relative Markdown links whose file or `#heading`
does not exist; `docs/...` paths named in any script, report or doc that do not exist; files in `docs/` missing from
the index ([README.md](README.md)); hand-written `.md` files with LF line ends (the repo uses CRLF); and command
blocks that mix systems (`pwsh` or a `.ps1` in a bash block, a `.sh` in a PowerShell block). It writes nothing,
needs no batch output and no network, and exits 1 on any problem. Run it after moving, renaming or editing a doc;
it must report 0 problems before a commit ([CONTRIBUTING.md](../CONTRIBUTING.md#rules-for-everything)).
