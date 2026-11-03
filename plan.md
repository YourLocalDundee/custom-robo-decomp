# Custom Robo (GC) — Model Extraction & Decompile

Goal: get viewable/exportable 3D models out of Custom Robo (USA) for GameCube, and a decompiled copy of the game code.

This file is the project log and roadmap. Reference documentation (formats, tools, workflows, decompile) is in [`docs\`](docs/README.md).

## Fresh session: start here

This file is the carry-over between sessions. Read, in this order: (1) [Resume here](#resume-here) below: the first bullet is the current queue, in the owner's order; (2) the docs those bullets link to, only the ones the task needs (the index of every doc: [docs/README.md](docs/README.md); the status numbers: [README.md](README.md) and `decomp/reports/progress_report.txt` of the last full run); (3) the numbered step log and every earlier hand-off only when a bullet points at a step: [docs/history.md](docs/history.md). The worktree recipe and the verification steps of a merge: [docs/workflows.md](docs/workflows.md#14-work-in-a-worktree-and-merge-it-back), [docs/REPRODUCE.md](docs/REPRODUCE.md).

## File sources

| What | Path |
|---|---|
| **Original ISO (untouched)** | `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Games\Custom Robo (USA).iso` |
| Working copy of ISO | `D:\emulators\gameTesting\Custom Robo (USA).iso` |
| Dolphin | the pinned official build, downloaded by `tools\setup.ps1` to `tools\dolphin\Dolphin-x64` (the Project Hive build in `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]` is not used for checks any more: it runs Gecko / AR codes, docs/INSTALL.md) |

> Note: the ISO came from the Project Hive folder and is a clean US retail dump (GXCE01, v1.0). Verified Oct 25, 2026: MD5 `546e5108173bceb4d5c8ca27b3fcfc96`, SHA-1 `fff6e74590366a5c0c5ba0d40c8ccf94770bcb16` match GameTDB's GXCE01 entry. Patches can be built against it.

## Folder layout (`D:\emulators\gameTesting`)

```
gameTesting\
├─ Custom Robo (USA).iso        copy of the source ISO
├─ plan.md                      this file (log + roadmap)
├─ docs\                        reference docs: README, workflows, tools, decomp, formats\ (sfd, model_entry, containers)
├─ Custom Robo extracted\       raw disc contents (DolphinTool extract)
│  ├─ sys\main.dol              game executable (code) — 2.3 MB
│  └─ files\                    game assets (.BIN archives, sprites, sound, etc.)
├─ unpacked\                    all 22 SFD archives split into entries 000.bin… (tools\sfd.py unpack), incl.
│                               scenario_us\ and scenario_jp\ (snr_chara, snr_obj, snr_map, rpg_t/f_models)
├─ models_dat\<category>\       1,006 models as standard HSD .dat (tools\extract_models.py)
│                               chara 170, weapon_gun 64, weapon_bom 52, weapon_pod 57, weapon_leg 29,
│                               snr_chara 56, bg_data 40, leg_effect 34, snr_obj 89, attack_effect 68,
│                               bomb_effect 104, gun_effect 128, pod_effect 114, title 1
├─ models_export\<category>\<name>\   every model exported: *.glb (or .dae for NaN cases) + textures .png
│  └─ export_log.txt            per-root result (joints / dobjs / pobjs)
├─ previews\                    sanity-check renders (tools\render_preview.py)
├─ test_build\                  full disc folder copy with a modified robo 0 (tools\make_test_build.py); boot in Dolphin
├─ symbols\
│  ├─ symbols.txt               SOURCE OF TRUTH for names; only names with an independent source
│  ├─ verify_report.txt         every applied name checked against Melee's verified symbols
│  ├─ unverified.txt            rejected / unsourced candidates + behaviour notes (NOT applied)
│  └─ melee_matches.txt         raw output of the assert matcher (before verification)
├─ decompiled\
│  ├─ main_dol.c                decompiled C of every function (output of Ghidra, verified names applied)
│  └─ ghidra_log.txt            headless analysis log
├─ ghidra_project\CustomRobo    Ghidra project — open in Ghidra GUI to browse/rename
└─ tools\
   ├─ rebuild_decomp.ps1        ONE command: fresh import + analyzer + names + strings + C export
   ├─ sfd.py                    SFD archives: unpack / pack / verify (byte-identical repack of all 22)
   ├─ crmodel.py                model entry <-> standard .dat (to-dat / from-dat / roundtrip), per the game's loaders
   ├─ extract_models.py         unpacked\ -> models_dat\ (all categories)
   ├─ export_models.py          models_dat\ -> models_export\ (glb, dae fallback)
   ├─ make_test_build.py        builds test_build\ through the full import path (recolour + 0x100 data shift)
   ├─ boot_test.ps1             boots a build folder in the Project Hive Dolphin, screenshots, closes (capture is black - see step 20)
   ├─ cr_model_explore.py       walks a model entry's pointer graph (format research)
   ├─ match_hsd_asserts.py      names HSD functions by matching __assert calls vs Melee decomp
   ├─ verify_symbols.py         checks every name vs Melee verified symbols (sizes)
   ├─ render_preview.py         renders a .glb to PNG (uses tools\pyenv)
   ├─ scripts\                  Ghidra scripts: EnableGCAnalyzer, ApplySymbols, DefineStrings, ExportDecompiledC
   ├─ ghidra_12.1.4_PUBLIC\     Ghidra (+ GameCubeLoader extension installed)
   ├─ HSDLib\                   HSDRawViewer built from source + our patch: headless --export (BatchExport.cs), --resave (Program.cs), --import (ModelImport.cs)
   ├─ patches\                  our tool fixes as patches: hsdlib_headless_export_resave.patch (+ base commit)
   ├─ dtk\                      decomp-toolkit 1.8.4 + its analysis project (proj\symbols.txt)
   ├─ melee\                    sparse clone of doldecomp/melee (sysdolphin source + verified symbols)
   └─ pyenv\                    Python venv (trimesh, matplotlib) for previews
```

## Resume here

- **WHERE TO CONTINUE (Nov 2, 2026, step 110; the owner's order: 1 scheduler wall follow-ups, then the unsplit objects, the drafts below 100%, typed data, HSD).** State: see the last commit and `decomp/reports/progress_report.txt`; step 110 is in [docs/history.md](docs/history.md) once closed.
  1. **The scheduler wall is broken (step 110)**: cr_CM_DUMMY_008 compiles to the original's 38 of 38 words ([docs/sched_model.md](docs/sched_model.md) section 20: decomp-permuter found the order, a counter pointer used only in dead code keeps r30). The generator rule `script_head_dead` writes that head for every handler. **Next**: (a) the handlers still below 100% after the full batch 12 (pr.py diff per group: the gap register's position, other head spellings); (b) the same idea on fn_8006EEF8 (no local where the original had none; dead-code locals for unused saved registers), on the ~200 gap-register functions and on the HSD near misses (lobj, texp, pobj, jobj); (c) run the permuter on the next stubborn shape (tools/sched/permuter, docs/sched_model.md sections 19-20 for the setup).
  2. **The unsplit automatic objects** (302 units; the deep dive of Nov 2 is in [docs/roadblock_solutions.md](docs/roadblock_solutions.md) section 9). The first fix found there: promote_drafts.py's file runs counted a global the original references (cr_g_BattleRobos) as a draft's own data, which chained one run over 1,739 functions; with that fixed the runs are file-sized again and their 100% drafts can be linked together (file objects).
  3. **The game functions below 100%**: the step 108/109 method (pr.py diff, hand drafts in tools/hand_drafts, a draft_batch.py rule when a fix repeats), smallest first; names and types first where a struct is still u8[].
  4. **Typed data**: the step 90 \"left\" list in docs/decomp.md, the struct fields still unk_, the motion tables 0x801F9B00 / 0x801F9D74 / 0x801F9D84 / 0x801F9D94 (docs/formats/motion_playback.md).
  5. **HSD to 100%** (sysdolphin 27 of 36 files): shadow.c, jobj.c, lobj.c, displayfunc.c, psdisptev.c with item 1's method; archive.c's loader, synth.c and the particle family as hand ports. Measured Nov 2 (objdiff report.json): each unlinked file is held by one or two functions: jobj.c HSD_JObjDispAll 97.2% (8 bytes larger, one more saved register; declarations, loop forms and reusing the jobj parameter tried, tools/sched/scratch/hsd110/jv.py: no change), lobj.c HSD_LObjSetupInit 99.1%, shadow.c HSD_ShadowSetViewingRect 99.8%, displayfunc.c mkBillBoardMtx / mkHBillBoardMtx 99.9% (two float temporaries swapped around `-az.y / mag`; the unused `f32 t` tried in 4 places: no change), pobj.c PObjLoad 99.1% / drawShapeAnim 97.1%, texp.c TExp2TevDesc 98.0% / HSD_TExpSetReg 99.6%. These are GC/1.3.2 -O4 register allocation: next, permuter_setup.py with the object's own build.ninja flags (an --object option) and the permuter on each.
  6. **The animation format: done on the tool side**, docs/formats/motion.md and motion_playback.md. Open: a human test in the game (an imported motion in a Dolphin battle); then tell the owner, who tells the lead.
  Review of symbols/name_proposals.txt (Nov 2): accept HSD_IDGetData (0x80198DC4, 0x2C = KAR's, layout next to the C-proven HSD_JObjDispAll), HSD_DevComARAMCallback (0x80170F48, 0x1A4) and HSD_DevComARAMWakeUp (0x801710EC, 0x4D8; KAR sizes, between named DevCom neighbours); reject hsd_8039DAD4 (Melee's placeholder for an unnamed function, not a name); setupInstanceMtx stays (used by jobj.c, no longer proven by batch 1: a c-callsite line once jobj.c's HSD_JObjDispAll matches).
  How to work (owner, Nov 2): **no subagents**: the main session does every item itself (the worktrees gameTesting-hsd / -hsd2 and branches hand109a / hand109b are merged and removed; gameTesting-data is old and merged). A partial `draft_batch.py --only` run uses the last full run's ctx_protos.h (a function renamed since then fails to compile in it only); the full batch 12 is the test (docs/drafts.md, main-session note). After a merge that brings promoted files, rename against the branch's symbols (docs/workflows.md section 14). No personal names or handles in any doc or commit.
  Name batches (fixed Nov 2, step 110): `run_all` no longer rewrites names. It rebuilds Ghidra once from the committed name files, runs batches 1 / 6 / 7 as a check, writes what they would change to symbols\name_proposals.txt and restores the files (tools\name_check.py). `-ApplyNames` keeps the old fixpoint and stops if a name the C sources use is dropped or moved. Why: since steps 103-109 named things by hand, batch 1 from scratch reaches other answers and dropped HSD_JObjDispAll, which the linked C calls, so batch 9 could not link. Open in the proposals: batch 1 no longer proves HSD_JObjDispAll and setupInstanceMtx (both used by C, both kept); 4 additions to review (HSD_DevComARAMCallback / WakeUp, HSD_IDGetData, hsd_8039DAD4).
  Then: source files and data in their files, the fresh-clone run, the clean-up sweep, the final scripts run.
- **Plan, once stages and bodies both import: an interactive modding tool (user request, Oct 30, 2026).** A desktop app (or Blender add-on + a small GUI) over the same scripts, nothing new underneath: pick a robo or stage, export to Blender with one click, re-import with the skeleton / roles / texture choices as checkboxes, preview the result (the `render.py` views), build the ISO or `.crpatch`, boot it in Dolphin, and a patch manager to apply others' patches. Steps: (1) finish the import paths (bodies done, stages next, then weapons and effects); (2) a Python core module that wraps model_loop / make_mod / iso_patch with a stable API and progress callbacks; (3) a GUI on it (Python + a toolkit that runs on Windows and Linux, or a Blender N-panel add-on for the model side and a separate small app for patches); (4) HSDRawViewer stays the Windows-only import/export engine until the import is ported into Python (IONET/HSDRaw logic), which also makes the whole tool cross-platform; (5) release with the mod docs and a sample mod (the legionnaire).
- **Waiting on the user:** (0) the in-game human test of the animation format: `build/mods/atlas_ninja_run/atlas_ninja_run.iso` (mods/atlas_ninja_run: Atlas, the legionnaire body on robo 0, with a ninja run on every *_normal_run motion and the "butt shot" on every gun motion: back to the target, bent over, the gun hand at the butt; made by mods/atlas_ninja_run/make_motion.py through tools/crmotion.py): run in a battle, shoot, and say whether the poses show and where the bullets come from; (1) the legionnaire v2 ATLAS build in Customize and a battle (step 74: `build/mods/ray_legionnaire/ray_legionnaire.iso`; the skeleton is already proven by the Oct 30 photos); (2) a battle in the shifted build (docs/REPRODUCE.md 2c); (3) **Owner: the first edited arena**, `build/mods/basic_arena_pillar/basic_arena_pillar.iso` (or `mods/basic_arena_pillar/basic_arena_pillar.crpatch`): pick Basic Arena (first stage of the select screen) in a versus battle; screenshots of (a) the stage select preview and (b) the battle, from a view that shows the floor at the right of the centre: a golden 1.2 x 2.6 box should stand on the floor, with the floor's tile pattern on its sides; robos walk through it (no collision); note any crash, missing floor or black / white box; (c) which button switches Basic Arena to Practice Stage at the stage select (docs/stage_tables.md). Project Hive's `.asm` sources are public (step 72, project-hive); the player-struct sheet is still worth asking for. Lesson of Oct 30, 2026: tools/HSDLib is an untracked clone, so after a worktree changes the HSDLib patch, re-apply it on the main clone (docs/tools.md).
- **Rules:** no wrong names, real names first; every step a script in run_all; docs after every task; never commit game data / asm / raw decompiler output (Ghidra C, m2c drafts) - matched C in decomp/src is committed; no co-author lines; every batch has a PowerShell 7 script for Windows and a bash twin for Linux / macOS (tools/*.sh), kept in step; CRLF in .md/.txt.


## Naming policy (applies to symbols/symbols.txt)

A name is applied only with an independent source:
1. **decomp-toolkit signature match** (SDK / runtime). Sizes can differ from Melee because Custom Robo uses a newer SDK.
2. **Melee decomp match**: assert (file, expression) match **and** function size within 10% of Melee's verified size.
3. **Layout proof (R3)**: between two verified anchors that appear in the same order in a reference, the unnamed functions align to the reference by size (within 10%). Since Oct 28, 2026 (step 47) a name is only emitted when its alignment is unique, or call graph / strings confirm it against every shifted alternative (`tools/layout_evidence.py`); equal-size runs (e.g. 0x20 math wrappers) are never named by layout alone. Names from the wowjinxy Kirby Air Ride map count only when another reference has the same name (it invents some).
4. **Assert sequence (R4)**: same assert source file and expression(s) as exactly one reference function, in an order consistent with the reference file; line numbers may differ.
5. **Call graph (R5)**: a unique match of the function's verified callees and callers (at least 2 distinct verified neighbours, common helpers like `__assert`/`OSReport`/`memcpy` not counted) to one reference function in the same reference unit; re-checked against the final name set.
6. **Unique string (R6)**: references a string literal that one reference function uses alone, or a game string that names the function.
7. **Matched-C call site (R7, Phase 4, step 55)**: a C file of `decomp/src` whose calling function is 100% in objdiff calls / uses the name (another file's, or since step 56 one of its own globals), and main.dol's own relocation at that instruction points at the address; every such call site must agree. Lines in the "Matched-C call sites" block of symbols.txt, tagged `// c-callsite obj:<C file> at:<function>+0x<offset> ref_size:` with the reference size and place in a `why:`; re-derived by `tools/check_c_callsites.py` on every batch 9 (stops the run on a disagreement). Never for a name another object linked from C defines (it would be defined twice).
8. **Matched-C definition (R8, Phase 4, step 56)**: a C file that matches its whole object (every retail function of the object is one of its functions: bytes with relocated fields masked, main.elf's relocations at every site, calls going where they must; its data placed by its own uses, every byte and pointer equal: tools/gen_dtk_splits.py) defines the name at that address (a `DEF` line of decomp/reports/splits_report.txt). Lines in the "Matched-C definitions" block of symbols.txt, tagged `// c-define obj:<C file> ref_size:` with a `why:`; checked by `tools/check_c_callsites.py` on every batch 9 (FAIL stops the run). For names other objects need when the file is linked from C.
Sanity for R4-R6: size within 0.5..2.0 of the reference, name unique in the reference, never C++ mangled, never a reference's own placeholder (`fn_`, `lbl_`, `kar_..._<addr>`).
**Cross-check (every run)**: `tools/crosscheck_names.py` (batch8) re-derives evidence for every name in symbols.txt, the candidates, descriptive.txt and cr_globals.txt, independent of the rule that produced it, and stops the run on any contradiction (FAIL). Report: `symbols/names_crosscheck.txt`, `docs/names_crosscheck.md`.
Before rejecting a name as "not in Melee", search all of Melee (config + src): it may be `static`/`inline` and folded into its caller (found for `FObjLoadWait` and `IDEntryAlloc`; the caller `HSD_IDInsertToTable` was verified and applied instead).
Everything else (descriptive guesses) lives in `symbols\unverified.txt` and is never applied. Earlier invented names (`File_ReadEntry`, `Viewer_LoadModel`, `Game_Panic`, …) were removed; `Game_Panic` turned out to be `OSPanic`.

Current state (Oct 29, 2026, step 52): 1,393 library function names applied from symbols.txt (dtk signatures, 46 Melee-assert, 862 layout-proof, 184 R3-R6 lib candidates merged by batch6) + 432 in descriptive.txt (300 of them generated by batch7 from the game's own tables, tagged game-real G1); 1,856 of 3,565 functions named; cross-check: 2,083 names, 0 FAIL, 1,632 OK, 435 WEAK (no contradiction, only the producing evidence), 16 UNCHECKABLE (prose `why:`). Historic count (Oct 25): 348 names.
Ghidra applies 347; `__save_fpr` / `__restore_fpr` / `__save_gpr` / `__restore_gpr` are runtime code fragments that Ghidra folds into neighbouring functions, and `OSDefaultExceptionHandler` did not get its own function header (not yet investigated).

## Roadmap

Each phase has a goal, the ideas for reaching it, and a finish line. Within a phase, work down the ladder and stop as soon as the finish line is met.

### Phase 0. Readable, named code: done (every item: [docs/history.md](docs/history.md#roadmap-finished-phases-and-items))

### Phase 1. Models (main priority)

**Goal.** Every model in the game can be opened, exported, replaced with a custom model, and loaded in game.

**Pipeline**
- [~] Fast test loop by booting the extracted folder in Dolphin: boots (step 20); automatic screenshots not working yet. Rebuild the ISO only for releases.

**HSD models (stages, title, effects)**

**Custom format models (robos and weapons)**
- [ ] Find the limits (file size, memory, vertex and bone counts).

**Other model containers**
- [ ] Not yet understood: `snr_map` (header has floats, likely bounds), `rpg_t_models` / `rpg_f_models` (u32 count + (id, offset) pairs), `snr_motion` (count+offset container, contents unknown), odd `bg_data` entries.

**Cross-checks**
- Custom Robo Model Swapper for anything it already knows.
- MeltyTool on the ISO.

**Done when**
- A custom model replaces a robo in game and works in battle.
- Every model archive has a working export and import path.

### Phase 2. Rest of the game

**Goal.** Every system and asset type understood well enough to mod.

**Ideas**
- Keep naming functions and structs as systems are understood, and keep exporting names.
- Tools for text (.bmg), textures (.tpl), audio (.hps/.ssm) and video (.thp).
- Stage data beyond the model, including the odd `bg_data` entries that look like index or collision data.
- Gameplay data such as part stats and battle rules.
- Story mode scripts and scenario data.
- A format document for each file type.

**Done when**
- Every file type on the disc has a documented format and a read and write tool.

### Phase 2b. New content: new robos, robo types, animations, sound (user request, Oct 28, 2026)

**Goal.** A full new robo, made from new assets, playable in battle: first by replacing an existing robo, then in a new slot of its own, and finally as a new robo type with its own behaviour.

**Two routes, used together**
- **Files** (`make_mod.py` -> `.crpatch`): models, skeletons, textures, text, sound banks, anything that is a disc file.
- **Code** (Gecko / Action Replay, and permanent DOL patches): table changes, new code, behaviour. The Custom Robo community says codes can add textures and other things but not models; research confirms it (`docs/community_codes.md`). The whole Dolphin code list gets about 6 KB (0x80001800-0x80003000); one robo entry is 186-233 KB and models live in the heap.

**Path, in dependency order**
1. **Models** (Phase 1): replacing works offline (484/484 byte-identical round trip; boots). [x] Confirmed in battle (Oct 30, 2026, Ray 01 legionnaire v1, docs/images/); a complete new body from Blender builds and boots (step 73, v2), its battle test pending. [~] Limits: body entry <= 0x3A000 bytes (the load buffer, checked by make_mod.py); memory, vertex / bone counts unknown (3,756 triangles, 58 joints, 44 DObjs boot).
2. **Skeletons**: a new bone tree works through the 19 role bones (`*.roles.json`, docs/MODDING.md). [ ] Confirm in battle. [ ] Robo size (lives in the vertices; root scale is overridden at run time, step 29).
3. **Animations**: 1,095 motion IDs and the 34 battle motion sets x 38 slots are named (step 46). [~] Map the motion data format (`snr_motion.bin`, `motion.DBN`): container and track layout verified Nov 2, 2026 (docs/formats/motion.md); the player code and the value meaning are "Resume here" item 2. [ ] Exporter + importer like crmodel.py. [ ] Blender retarget workflow for new skeletons. [ ] What "charge not linked" is (step 28).
4. **Sound effects and music**: `.ssm` banks and `.hps` streams, the same HAL formats as Melee (check Melee's tools first). HSD audio code is named (step 47). [ ] Read/write tools. [ ] Map SFX IDs -> bank, robo and action.
5. **Text, icons, stats**: `.bmg` names/descriptions, `.tpl` icons/portraits (590 of 915 disc files are `.tpl`), part and robo stat tables (Phase 2).
6. **New robo slot** (instead of replacing one): every table indexed by robo number has to grow. Known: 5 entries per robo in `chara.BIN` + main.dol's copy of the entry table, `cr_g_CharaMotionSets`, `cr_g_CharaYOffset`, `cr_g_BattleMotionSets`, stats, text, icons, unlock flags, robo-count limits. [x] Found (step 71): **[docs/robo_tables.md](docs/robo_tables.md)** lists the 16 body-indexed tables, files and constants with their users and the checklist for body 34 (cr_g_CharaMotionSets / cr_g_CharaYOffset turned out to be story-character tables). Step 72: [docs/community/crbr_parts_expansion.md](docs/community/crbr_parts_expansion.md): the community's parts expansion adds menu slots on existing body numbers (no table of the 16 grows) and gives the order lists' 210 direct users, the stats split, a separate model number and free space it uses. [ ] Move grown tables into new space (below).
7. **New robo type** (new behaviour: movement, air dash, charge, ...): needs the behaviour code understood (stage 2) and new code (below).

**The community already does parts and classes by code (community, analysed Oct 29, 2026: docs/community/codes_addresses.md).** Project Hive (the tournament patch, 3.2) loads a ~140 KB `GXCE01.GCT` through Riivolution (not limited to Dolphin's ~6 KB cheat area). It adds new parts, Oil Can, rebalanced stats, parts expansion data, and 4 new robo classes in test builds. 33 code hooks are mapped to our functions; the robo stats table is at 0x801C6C44 (0x134 per body, proven by the part-select code), and the 4 battle robos are at 0x8024E120 + n x 0x33520.
- [ ] Ask the community for the player-struct sheet. [x] PH's `.asm` sources are public (project-hive) and compared with our tables (step 72).
- [ ] Use the hooks' described behaviours as stage-2 naming leads, after our own proof.

**Codes (community request, Oct 28, 2026).** Public lists: 15 AR codes (cheathappens, decrypted: HP x4, 1-hit x4, parts owned x5, unlock flags) + Dolphin's 16:9 Gecko code. All 13 addresses map into our decomp (docs/community_codes.md, docs/community/codes_gxce01.tsv). Use them as naming evidence leads, not names.
- [ ] `tools/ardecrypt.py`: AR (encrypted or plain) -> Gecko lines, so community codes can be dropped into a mod (own implementation; Dolphin's ARDecrypt.cpp is GPL).
- [ ] `make_mod.py`: `code/*.gecko` -> Dolphin `GXCE01.ini` + `.gct`. Writes that land inside main.dol become static patches with `[was]` checks; bss/heap targets stay Gecko-only.
- [ ] `make_mod.py`: `code/*.s` -> C2 code (devkitPPC `powerpc-eabi-as`, pinned; see JLaferri's `gecko`, Melee Code Manager) and, permanently, a body in free space + a `b` hook.
- [ ] Free space for new code / data: a new DOL text section at 0x80001800-0x80003000 first (main.dol uses 2 of 7 text and 6 of 11 data sections); then dead code (MetroTRK, proven with `find.py callers`); check how `OSInit` sets ArenaLo before a section after bss.
- [ ] Name leads from the codes: player battle record (x4, stride 0x33520, HP float `DAT_8024e194`) used by `FUN_8004ede4`; parts-owned bitsets `803BFB9C..` (5 x 4 x 64 bits) set up by `FUN_800a3c0c`; bitset helpers `FUN_80023228` (set, checked) / `FUN_800231ec` (clear); unlock flags `DAT_80321d44`; camera aspect `FLOAT_8044c438` (1.26); 2D ortho setup `FUN_80091518`.
- [ ] Ask the community for its code lists (more evidence and ready-made table addresses; the open questions are in docs/community_codes.md).

**Textures.** A Dolphin hash pack exists (AI-upscaled HD pack, Dec 2024, unfinished). Our route is file replacement (model TObjs via make_mod.py, `.tpl` via `files/`).
- [ ] Map Dolphin texture hashes to our TObjs, so a texture pack can become a disc mod and back.
- [ ] `.tpl` read/write tool (Phase 2).

**Models: codes can't do it**; file replacement is the path. Cross-check Zoeyism's Model Swapper (CC0): it overwrites `rpg_t/f_models.BIN` with a strictly smaller model and leaves the entry table alone.

**Done when**
- A new robo, with its own model, skeleton, animations, sounds, name, icon and stats, is picked in its own slot and fights in battle, built from a mod folder by one command.

### Phase 3. Community release

**Goal.** Anyone with their own copy of the game can mod it.

**Ship**
- Tools, format docs, and the symbols file.
- Mods as patches applied to the user's own clean retail ISO, with a hash check against GXCE01 before applying.
- A public repo with a contributor guide.
- A `GXCE01.ini` / `.gct` export next to every `.crpatch`, so code-only mods work without rebuilding the ISO (Dolphin cheats, Nintendont).
- **"Install the tools" section in README.md** (user request, Oct 28, 2026), like other git repos have: per OS, the package-manager commands to copy and paste in order, then clone -> setup -> run. No prebuilt .exe or binaries shipped; everything comes from package managers or the pinned downloads in `tools/pins.txt`.
  - Windows: `winget install` lines (Git, Python 3, PowerShell 7, .NET 8 SDK, a JDK for Ghidra, 7-Zip).
  - Debian / Ubuntu: `sudo apt update` then `sudo apt install git python3 python3-venv openjdk-21-jdk dotnet-sdk-8.0 p7zip-full` (check exact package names per release).
  - Arch: `sudo pacman -S --needed git python jdk21-openjdk dotnet-sdk-8.0 p7zip`, then `yay -S` for AUR-only packages (e.g. ghidra, dolphin-emu if not in the repos).
  - Fedora (`dnf`) and macOS (`brew`) as best effort; a plain "you need these" list for anything else.
  - A check step (`tools/setup` prints each tool's version and stops with a clear message if one is missing).
  - The scripts themselves must then run on Linux too: `pwsh` (PowerShell 7 runs on Linux) or sh ports of `setup.ps1` / `run_all.ps1`; DolphinTool / Dolphin from the distro or flatpak; checksums equal on every OS (normalised line endings).
  - Done when: fresh Windows, Debian and Arch machines (or containers), following only the README commands, reach the same checksums and a patched ISO that boots in Dolphin.

**Ship (user decision, Oct 28, 2026)**
- All our progress: every script, batch, symbol file, type header, report, doc and finding, plus everything needed to replicate it from a clean ISO (pinned tools and references, `docs/REPRODUCE.md`, expected checksums).

**Never ship**
- Nintendo's / Noise's files: the ISO, extracted disc files, ripped models, textures or audio, and the decompiled C (`main_dol.c`, `decompiled/`). Everyone regenerates these from their own disc with the scripts.

**Done when**
- Someone outside the project can take a clean ISO, run the tools, and play a custom model.

### Phase 4. Rebuild the game from source (super goal, user, Oct 28, 2026)

**Goal.** Custom Robo builds from C source into a main.dol that matches the retail one byte for byte (SHA-1 `6a73cd11…fb50b`, in tools/dtk/proj/Config.yml). New robos, robo types and systems are then added in source and rebuilt, not patched.

**What we start with (Oct 28, 2026)**
- The game is plain C: no C++ mangled names, no extab / ctors-heavy code. The toolchain is Metrowerks CodeWarrior (the "Metrowerks CW runtime" and MetroTRK strings are in main.dol).
- 3,565 functions: about 1,800 library (1,393 named; HSD, Dolphin SDK Apr 17 2003, MSL / runtime, TRK) and about 1,760 game code.
- dtk already analyses the DOL (`tools/dtk/proj`: symbols.txt, sections, `out/` asm + ldscript), but `splits.txt` has sections only, no objects yet.
- Matching sources to reuse: HSD from the Melee decomp (a different revision: e.g. our HSD_PObj is 0x1C, Melee's 0x18), Kirby Air Ride's decomp (a second HAL), and the SDK / MSL from dolsdk2001 / dolsdk2004 and the other pinned GC decomps.
- The names, types (cr_types.h, ParseHeaders) and module notes (G3 `.c` files) all carry over.

**Milestone 1. A build that matches from asm (weeks)**
- [ ] Splits: object boundaries in `splits.txt`, from the library layouts (Melee / SDK object lists in their configure.py), the G3 `.c` module notes, alignment padding, and the rodata / data / bss order per object. dtk validates each split.
  - [ ] The rest of the library code (24 unproven code ranges, 812 functions outside the game range: mostly HSD, whose object edges sit on unnamed functions of another HSD revision).
  - [ ] The rest of the data (game data, HSD, .ctors / .dtors / .sbss2, pieces where the evidence disagrees): dtk's automatic objects, listed in splits_report.txt.
  - [ ] Game code (0x800055E0-0x80131C08, 1,751 functions): one unproven range. G3 notes prove no boundary yet (seq_movie.c, background.c, sprite.c: the functions next to them carry no file note), there is no alignment padding between any two game functions, and the per-file pools can extend a file but never prove where it ends without a known neighbouring file.

**Milestone 2. A shiftable build (the modding unlock, before anything matches)**
- [ ] Play a battle in the shifted build (by hand: docs/REPRODUCE.md section 2c).
- [ ] Padding anywhere inside game data: the game's data has no per-file split yet (Milestone 1); until then only at file boundaries (section starts, glue-span starts, library data-object starts).
- Once this works, a new robo slot (grown tables) and new code are ordinary source edits, with no Gecko or free-space hacks. Phase 2b steps 6-7 can then use this instead of patches.

**Milestone 3. Compiler and flags (days to weeks)**
- [ ] Narrow the game / Runtime / TRK compiler inside 1.3-2.7 (and MSL inside 2.5-2.7): the first matched function with a version-sensitive construct decides it.

**Milestone 4. Libraries to matching C (months)**
- [ ] Bring in the SDK, MSL and TRK sources from the pinned decomps; adapt them to our SDK revision; objdiff each object.
  - [ ] The 70 tried files (reasons in CR_NOT_MATCHING): names for the CARD / dsp / mtx calls, data pieces for the 12 code-perfect files, one-function revisions, then the rest.
- [ ] HSD from Melee / KAR, adapted to Custom Robo's revision (struct sizes from check_class_sizes.py, the differing functions from the R2/R4 size checks); step 54: util.c and object.c are 100% code from Melee unchanged, blocked by their .data in an automatic object / the unnamed hsdClass).
- [ ] HSD audio (axdriver.c, synth.c, devcom.c, dvdsche.c) and particle code.
- Progress = matched bytes per library, reported by objdiff and published in the README.

**Milestone 5. Game code to matching C (the long part: about 1,760 functions)**
- [ ] Order by system, the way naming reaches them (Phase 0 stage 2): file system / archives -> model loading -> script commands -> camera -> motion -> battle -> menus -> story.
- [ ] Per function: m2c from the asm for a first draft, then match on decomp.me / objdiff, using decomp-permuter for register allocation. Real names and our types make the drafts readable.
- [ ] Data: tables, strings and the per-file statics, defined in C in the right order.
- [ ] Batch scripts like the naming ones: progress report, regressions, one command from fresh.
- Scale: Melee took a large community years; with the libraries mostly already done elsewhere, about half of that work remains here. Plan for a long project, tracked by matched %.

**Milestone 6. Done**
- [ ] 100% matching: main.dol from source == retail SHA-1.
- [ ] Mods as source: a new robo (assets via the Phase 1 / 2 tools, tables and code in C) built by one command. Linux builds checked (minime + Arch/yay per the testing plan).

**Risks / open**
- Compiler version of the game code only narrowed to 1.3-2.7 (level-0 code barely differs between them; step 50).
- Hand-written asm or a non-matching library revision: keep those as asm objects.
- Splits mistakes show up late; validate early with the asm build.

## References

- HSDLib / HSDRawViewer: https://github.com/Ploaj/HSDLib
- Ghidra: https://github.com/NationalSecurityAgency/ghidra
- GameCube loader: https://github.com/Cuyler36/Ghidra-GameCube-Loader
- Custom Robo Model Swapper: https://www.romhacking.net/utilities/1735/
- MeltyTool: https://github.com/MeltyPlayer/MeltyTool
- decomp-toolkit: https://github.com/encounter/decomp-toolkit
- Ghidra PowerPC SDA issue #325: https://github.com/NationalSecurityAgency/ghidra/issues/325
- GameTDB GXCE01 (retail hashes): https://www.gametdb.com/Wii/GXCE01
