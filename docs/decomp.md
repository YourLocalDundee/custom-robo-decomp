# Decompile

## Output

| | |
|---|---|
| C file | `decompiled\main_dol.c` — 3,416 functions, 222,965 lines, 0 decompile failures |
| Ghidra project | `ghidra_project\CustomRobo` (open in the Ghidra GUI to browse / rename) |
| Log | `decompiled\ghidra_log.txt` |
| Rebuild | `pwsh tools\rebuild_decomp.ps1` (~2 min) |
| Names applied | 347 function names (348 listed; see below) + 203 data labels from dtk |
| Strings | 513 distinct labelled strings in the C (`s_…`) |
| `unaff_r13` / `unaff_r2` | 0 / 852 (in 74 functions) |

Function names in the C that are not `FUN_…`: 378 (our 347 plus names Ghidra generates itself, e.g. entry points).

## Pipeline (`tools\rebuild_decomp.ps1`)

1. `analyzeHeadless` imports `Custom Robo extracted\sys\main.dol` with the GameCube loader (`-loader GameCubeLoader -loader-autoloadMaps false`).
2. Pre-script `EnableGCAnalyzer.java` turns on the loader's "(GameCube/Wii) Program Analyzer", which sets r13 = 0x804543C0 and r2 = 0x804581A0 (small-data bases). It is off by default because its enablement check compares the program format with "Nintendo GameCube/Wii Binary" while the loader now reports "Nintendo GameCube/Wii Binary (Executable)" — **verified**: enabling it took `unaff_r13` from 22,062 to 0.
3. Post-scripts: `ApplySymbols.java symbols\symbols.txt`, `DefineStrings.java`, `ExportDecompiledC.java decompiled\main_dol.c`.

## Naming policy

A name goes into `symbols\symbols.txt` only with an independent source:
1. **decomp-toolkit signature match** — SDK / Metrowerks runtime. Sizes can differ from Melee's because Custom Robo (2004) links a newer SDK than Melee (2001).
2. **Melee decomp match** — the function's `__assert` (file, expression) pairs match a Melee function **and** its size is within 10% of Melee's verified size (`tools\melee\config\GALE01\symbols.txt`).
3. **Layout proof** — same neighbouring functions and same / near size as Melee's verified symbol.

Before rejecting a name for "not in Melee's symbols", search all of Melee (`config` + `src`): a `static` / `inline` function is folded into its caller and never appears on its own. Then check the caller instead.

Descriptive guesses go to `symbols\unverified.txt` (never applied). The report of the last check is `symbols\verify_report.txt`.

## Current names (348 listed)

| Source | Count | Notes |
|---|---|---|
| decomp-toolkit 1.8.4 signatures | 298 | OS, DVD, GX (part), PAD, VI, TRK debugger, C runtime (`memset`, `memcpy`, `__start`, `main`, …) |
| Melee assert match, verified | 46 | sysdolphin (HSD) library: `HSD_JObjAlloc`, `HSD_DObjLoadDesc`, `HSD_PObjLoadDesc`, `HSD_TObjSetup`, `HSD_ByteCodeEval`, … |
| Layout proof | 4 | `__assert` (0x801AEA4C), `HSD_Panic` (0x801AEAA8), `OSPanic` (0x80134874), `HSD_IDInsertToTable` (0x801A29E8) |

Rejected and kept only in `unverified.txt`: 7 Melee matches whose size differs > 10% (e.g. `HSD_JObjReparent`: 0x4D4 here vs 0x110 in Melee), plus `FObjLoadWait` at 0x80192F74 (static in Melee, folded into `FObjUpdateAnim`, which is 0x1C0 in Melee vs 0x6D4 here — not verified).

Ghidra applies 347: `__save_fpr`, `__restore_fpr`, `__save_gpr`, `__restore_gpr` are runtime fragments Ghidra folds into neighbouring functions; `OSDefaultExceptionHandler` did not get its own function header (not investigated).

Removed earlier: descriptive names we had invented (`File_ReadEntry`, `Viewer_LoadModel`, `gArchiveTable`, `Game_Panic`, …). `Game_Panic` turned out to be `OSPanic`.

## Findings in the code (game functions keep `FUN_` names)

| Address | What it does (read from the C) |
|---|---|
| `0x801F1200` (data) | pointer list to archive descriptors; descriptors are 16 bytes `{char* name, ?, ?, ?}` from `0x801F21E0`. Index 0 `logo_read_us.dbn`, 1 `chara.bin`, 2 `scenario/snr_t_01.bin`, 3 `scenario_us/snr_chara.bin`, 4 `snr_map.bin`, 5 `rpg_t_models.bin`, 6 `rpg_f_models.bin`, 7 `snr_motion.bin` |
| `FUN_80020a70(archive, entry)` | returns the entry's disc offset |
| `FUN_800202ec(archive, entry)` | returns the entry's size |
| `FUN_80020e44` | reads an archive entry into memory |
| `FUN_8002036c` | opens an archive entry (slot table at `0x80320184`) |
| `FUN_800a63c0` | loads `chara` entry robo_id × 5, and archives 8 / 9 / 10 / 11 by a category byte at `DAT_803bfc95` |
| `FUN_800747b0`, `FUN_800748a8` | robo / story-character entry loaders ([model_entry.md](formats/model_entry.md)) |
| `FUN_80073cb8` | weapon entry loader |
| `FUN_801988fc` | called with a root JObjDesc pointer by the loaders (loads a joint tree) |
| `FUN_80196e0c` | called with (joint, joint animation, 0, 0) by the loaders (attaches an animation) |

The game's assert strings name its HSD source files (`jobj.c`, `pobj.c`, `dobj.c`, `mobj.c`, `tobj.c`, `cobj.c`, `lobj.c`, `robj.c`, `fobj.c`, `aobj.c`, `wobj.c`, `texp.c`, `bytecode.c`, `shadow.c`, …) and a few game files (`seq_movie.c`, `sprite.c`, `background.c`).

## Not done

- Roadmap Phase 0 step 4: compare Ghidra's references with dtk's relocation / SDA analysis.
- Audio (AX) and most GX functions are not named by dtk's signatures (reason not checked).
- `symbols\` is not under version control yet.

## Batch pipeline and search (Oct 27, 2026)

`pwsh tools\batches\run_all.ps1` (about 4 min per rebuild: 1 rebuild with an export present, 2 on a fresh setup; `-SkipRebuild` to reuse the current Ghidra output) runs:
batch 1 layout-proof names (`tools\layout_match.py`) + batch 6 lib candidates + batch 7 the game's own names (`tools\match_game_names.py`, plan.md step 48) + lint ->
Ghidra rebuild (also `CreateDtkFunctions.java`, `ExportXrefs.java`) -> batches 1 + 6 + 7 again, rebuilding until no name file changes (plan.md step 47) -> batch 5 types -> batch 2 dtk cross-check (`symbols\crosscheck_report.txt`) ->
batch 3 game map (`docs\game_map.md`) -> batch 4 search index (`decompiled\index.db`, `decompiled\funcs\`) -> batch 8 name cross-check (`docs\names_crosscheck.md`; stops on any FAIL) -> batch 9 decomp build (retail SHA-1, relocations, static shift test)
-> batch 10 compiler probe -> batch 11 shifted build booted in Dolphin -> `tools\checksums.ps1 -Compare`. One script runs it all, in order, stopping at the first failure.

Search examples:
```
python -I tools\find.py text chara_bin          full-text over all C
python -I tools\find.py func FUN_800747b0       print one function
python -I tools\find.py callers 800747b0        direct calls + places that take its address
python -I tools\find.py refs 801F1200           every instruction touching an address
python -I tools\find.py str snr_motion          strings + the functions using them
python -I tools\find.py hint anim               game functions reaching animation code
```

## Types (Oct 27, 2026)

Library structs and prototypes come from Melee's headers (`ParseHeaders.java`), checked three ways: the headers' own ASSERT_SIZE lines, their `/* +XX */` field offsets, and the object sizes Custom Robo registers for each HSD class (`tools/check_class_sizes.py`). Custom Robo's differences are overrides in `tools/clean_headers.py`. Our own types: `symbols/cr_types.h`, globals: `symbols/cr_globals.txt`. Report: `symbols/types_report.txt`. Details: plan.md step 23.

## Robo objects (Oct 27, 2026)

`cr_Chara` (0x19A8 bytes, 24 in `cr_g_Charas` at 0x803BFE88) is the robo in play; `cr_g_CurrentChara` points at the one being processed. Known fields and their evidence are in `symbols/cr_types.h`; plan.md step 30.

## Real-name evidence (Oct 27, 2026)

`tools/name_evidence.py` lists every name the game itself still holds (enum name tables, identifier strings, debug formats, source files) with the code that uses them: `docs/name_evidence.md`, `docs/name_tables.tsv`. Rule: use these before any `cr_` name (CONTRIBUTING.md). First result: the motion name table (0x80206DB0) gives the 17 field-character motion slots their real names (plan.md step 40).

## Game sequences (Oct 27, 2026)

The game runs one sequence at a time (`cr_Seq`: SEQ_LOGO, SEQ_MOVIE, SEQ_TITLE, SEQ_FIELD, SEQ_BATTLE, ... - the game's own names). `cr_g_SeqCurrent` picks the handlers in five tables: `cr_g_SeqEnter` / `cr_g_SeqLeave` on a change, `cr_g_SeqFrameA/B/C` every frame. Handlers are named `cr_SEQ_BATTLE_Enter` etc. To find a mode's code, start at its handlers. plan.md step 42.

## Script commands (Oct 28, 2026)

The game's script interpreter dispatches through `cr_g_ScrCmdFuncs` (0x8021C3F4, 300 handlers), indexed by command number; the matching names (`SYS_SET_SCR`, ... ) are in `cr_g_ScrCmdNames` (0x8021BF44), directly before it. Handlers are named `cr_<COMMAND NAME>`. plan.md step 44.

## Building library objects from C (Phase 4 Milestone 4, Oct 29, 2026)

Not the Ghidra C above (that is never copied anywhere): the community's own matching C from the pinned reference
decomps, compiled by the Phase 4 build (`decomp/`). Status: `decomp/reports/progress_report.txt`; provenance of every
file: `decomp/reports/sources_report.txt`.

How to add a C file (one object of `decomp/config/GXCE01/splits.txt`):
1. Copy it with its headers, never by hand:
   `python -I tools/import_ref_source.py <ref> <path in ref> src/<object path> --inc <ref include dir>=include/<ref> ...`
   - SDK: `dolsdk2004 src/os/OSCache.c src/dolphin/os/OSCache.c --inc include=include/dolsdk2004 --inc include/libc=include/dolsdk2004/libc`
   - MSL / Runtime / TRK: `marioparty4 src/MSL_C.PPCEABI.bare.H/mem.c src/MSL/mem.c --inc include=include/marioparty4`
   - HSD: `melee src/sysdolphin/baselib/util.c src/sysdolphin/baselib/util.c --inc src=src --inc libs/doldecomp/include=include/melee/doldecomp --inc src/MSL=include/melee/MSL --inc libs/dolphin/include=include/melee/dolphin`
   The provenance comment goes on line 1 (same line), so line numbers stay the reference's.
2. `cd decomp; python configure.py; ..\tools\ninja\ninja.exe` compiles it (for objdiff only); then
   `python -I tools\check_c_objects.py --link-each` links the build with it taken from C: `retail` = it can be linked.
   The check stops until the file is listed: in `CR_NOT_MATCHING` (decomp/configure.py) with its reason, or in
   `CR_MATCHING`.
3. If functions differ, adapt the source (objdiff GUI on `decomp/`, or `objdiff-cli diff -p decomp -u main/<object>`).
   A `fn_...` at 0% usually exists in the C under its real name (the link test shows it).
   If the link fails on an undefined name (the C calls / uses something our symbols leave as `fn_` / `lbl_`):
   `decomp/reports/c_callsites_report.txt` (`tools/check_c_callsites.py`, batch 9) lists every name the matched C's
   call sites give to a dtk default name, with its address (`UNNAMED` lines). Add it to `symbols/symbols.txt` in the
   "Matched-C call sites" block, tagged `// c-callsite obj:<C file> at:<function>+0x<offset>`, with `ref_size:` and a
   `why:` (reference size, place in the reference file); batch 9 re-derives it every run and stops if it disagrees.
   Not when another C object linked from C defines that name (it would be defined twice): that is a data split.
4. In `CR_MATCHING`: `python -I tools\make_configure_patch.py`, then batch 9. `tools/gen_c_symbols.py` gives the object
   the C's own symbols (objdiff pairs data by name), `tools/check_c_objects.py` must report 100% code + data, main.dol
   must stay retail. Then the full `run_all.ps1`, and `checksums.ps1 -Write` for the intended changes.
