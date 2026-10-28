# Custom Robo (GC) — Model Extraction & Decompile

Goal: get viewable/exportable 3D models out of Custom Robo (USA) for GameCube, and a decompiled copy of the game code.

This file is the project log and roadmap. Reference documentation (formats, tools, workflows, decompile) is in [`docs\`](docs/README.md).

## File sources

| What | Path |
|---|---|
| **Original ISO (untouched)** | `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Games\Custom Robo (USA).iso` |
| Working copy of ISO | `D:\emulators\gameTesting\Custom Robo (USA).iso` |
| Dolphin (Project Hive build) | `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Dolphin.exe` |

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
   ├─ HSDLib\                   HSDRawViewer built from source + our patch: headless --export (BatchExport.cs), --resave (Program.cs)
   ├─ patches\                  our tool fixes as patches: hsdlib_headless_export_resave.patch (+ base commit)
   ├─ dtk\                      decomp-toolkit 1.8.4 + its analysis project (proj\symbols.txt)
   ├─ melee\                    sparse clone of doldecomp/melee (sysdolphin source + verified symbols)
   └─ pyenv\                    Python venv (trimesh, matplotlib) for previews
```

## What's done

1. **Copied ISO** from the Dolphin Games folder into `gameTesting`.
2. **Extracted disc** with Dolphin's own tool:
   `DolphinTool.exe extract -i "Custom Robo (USA).iso" -o "Custom Robo extracted"`
3. **Identified formats** (by header bytes):
   - `*.BIN` = **SFD archive**: `"SFD\0"`, u32 count, then count × (u32 offset, u32 size), big-endian, entries 0x800-aligned.
   - `TITLE_MODEL.dat` and even `bg_data` entries = **HAL sysdolphin (HSD) .dat** — same format as Smash Melee / Kirby Air Ride. Root node `scene_data`.
   - `chara` / `weapon_*` entries = **also HSD data, behind a custom 0x40-byte header** (see "Model format" below). First thought to be a custom format; corrected in step 12.
   - `.tpl` = GameCube textures, `.bmg` = text, `.thp` = video, `.hps`/`.ssm` = audio.
4. **Unpacked all SFD archives**: `python -I tools\sfd.py unpack "Custom Robo extracted\files" unpacked` (originally a top-level-only unpacker; replaced in step 15).
5. **Collected HSD models** (originally into `models_hsd\`; now `models_dat\`, step 16).
6. **Built HSDRawViewer** from source (github.com/Ploaj/HSDLib), set to roll forward to .NET 10.
   Exe: `tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe`
7. **Installed Ghidra 12.1.4 + GameCubeLoader 1.3.1** (loader's `extension.properties` version patched 12.1 → 12.1.4).
8. **Ran headless decompile** of `main.dol` → `decompiled\main_dol.c` *(3,387 functions at first; 3,416 now that names create a few extra functions; 0 failed)*. Command at the time (run from `gameTesting`):
   ```
   analyzeHeadless.bat ghidra_project CustomRobo -import "Custom Robo extracted\sys\main.dol" -overwrite ^
     -loader GameCubeLoader -loader-autoloadMaps false -scriptPath tools\scripts ^
     -preScript EnableGCAnalyzer.java -postScript ExportDecompiledC.java decompiled\main_dol.c
   ```
   Gotcha: without `-loader-autoloadMaps false` the GameCube loader opens a hidden "Load symbol map?" dialog and headless hangs forever (first run stalled ~12 min on this). With it, the whole run takes ~2 min.
   **Now superseded by `tools\rebuild_decomp.ps1`**, which also applies names and strings.
9. **decomp-toolkit analysis** (`tools\dtk\proj`): 298 SDK / runtime functions named by signature (OS, DVD, GX, TRK, C library).
10. **sysdolphin discovered**: the game's asserts name HAL library files (`jobj.c`, `pobj.c`, `dobj.c`, `mobj.c`, `tobj.c`, …) — the same HSD library as Melee. `tools\match_hsd_asserts.py` matched 55 functions against the Melee decomp; **46 survived verification** (step 11).
11. **Name verification** (see "Naming policy"). Every applied name checked by `tools\verify_symbols.py`; unverifiable names removed.
12. **Model format solved**: robo/weapon entries are standard HSD structures (JObjDesc / DObjDesc / MObjDesc / TObjDesc / PObjDesc with envelopes) behind a different file header. First converter (heuristic root finding) replaced in step 17.
13. **Batch export**: patched HSDRawViewer with a headless `--export` mode (`BatchExport.cs`, handles `*_joint` and `scene_data` roots).
14. **Visual check**: robo texture atlas decodes correctly; robos render in T-pose bind pose, weapons, story characters, story objects and effects render correctly (`previews\`).
15. **SFD repack**: `tools\sfd.py` — layout rules verified on all 22 archives (0x8000 header, 0x800-aligned tightly packed entries, file padded to 0x800). `verify` rebuilds **all 22 archives byte-identical**. Found 10 more archives under `scenario_us\` / `scenario_jp\`.
16. **All model containers surveyed and extracted** (`tools\extract_models.py`): 1,006 models in 14 categories. `*_effect` entries are a small container (u32 count + offsets) of standard .dat files; `snr_chara` uses the robo format; `snr_obj` is standard .dat.
17. **Header decoded from the game's loaders** (see "Model format") and `tools\crmodel.py` written: entry → .dat → entry is **byte-identical for all 484 robo / weapon / story-character entries**. Root joints are now taken exactly as the loaders take them; joint/DObj/PObj counts match the earlier heuristic for all 1,006 files.
18. **Full export** (`tools\export_models.py`): every model exported, 0 failures; 22 files use the `.dae` fallback because the glTF writer rejects a NaN (cause not yet investigated).
19. **Import path test build** (`tools\make_test_build.py "Custom Robo extracted" unpacked test_build 0 1 2 3 4`): chara entries 000–004 (all 5 entries of robo 0) go entry → .dat → red/blue swap in both CMPR textures → whole data block shifted by 0x100 → `crmodel from-dat` → `chara.BIN` repacked. Offline check: the rebuilt entry re-reads with the same joint/DObj/PObj counts (55/18/52), the texture shows the swap cleanly, the other 165 entries are byte-identical.
   Note: HSDLib's `--resave` of an unmodified .dat is byte-identical, so the 0x100 shift is what exercises relocation.
20. **Boot test**: Dolphin (Project Hive build) boots `test_build\sys\main.dol`; window title "Custom Robo (GXCE01)", ran until closed (40 s and 60 s runs). **Not yet confirmed in game**: screenshots via screen copy / PrintWindow came out covered or black, and `-C Dolphin.Movie.DumpFrames=True -C GFX.Settings.DumpFramesAsImages=True` produced no frames (reason not found). Needs a person to play to a screen showing robo 0.

21. **Batch pipeline** (Oct 27, 2026): `pwsh tools\batches\run_all.ps1` runs everything in one go (~95 s):
   - batch 1 `tools\layout_match.py`: **222 library names** by layout proof (rule 3), repeated until nothing new is found; all VERIFIED/CLOSE in `verify_report.txt`. 569 function names applied now (was 347).
   - rebuild: new `CreateDtkFunctions.java` creates the functions dtk found and Ghidra missed: **+178 functions** (3,565 in the C, 1 failed).
   - batch 2 `tools\crosscheck_dtk.py` (Phase 0 step 4) → `symbols\crosscheck_report.txt`: only 1 dtk function missing in Ghidra, 34 Ghidra-only, 5 names not applied (the known 4 fragments + `OSDefaultExceptionHandler`), 74 `unaff_r2` functions.
   - batch 4 `tools\build_index.py` (+ Ghidra `ExportXrefs.java`): **everything searchable** in `decompiled\index.db` (3,565 functions, 137,738 references, 3,541 strings, full-text C) and one file per function in `decompiled\funcs\<addr>\`. Query with `python -I tools\find.py text|func|callers|callees|refs|str|hint <arg>`. First find: the robo loader `FUN_800747b0` is never called directly; `FUN_800a0e64` stores its address (callback / function table).
   - batch 3 `tools\game_map.py` → `docs\game_map.md`, `symbols\game_map.tsv`: 1,761 game functions with callers/callees/strings/subsystem hints and the hub list (most-called unnamed functions). Behaviour notes only.

22. **More references, descriptive names, public repo prep** (Oct 27, 2026):
   - Reference decomps added (pinned in `tools/pins.txt`): MKDD, Pikmin 2, TTYD, Wind Waker, Twilight Princess. `layout_match.py` uses all of them: **669 layout-proof names** (all VERIFIED/CLOSE), guards: no C++ mangled names, no names used twice in one reference, long gaps only inside the library range. Totals: 1,016 verified function names, 1,060 named functions in the C.
   - `symbols/descriptive.txt`: our own `cr_` names for game code with a `why:` note each, checked by `tools/lint_descriptive.py`, applied after verified names. First 11: file system / archives / model load callbacks. Correction found: `FUN_80020a70` opens an entry into a file slot (returns the slot), it does not return a disc offset.
   - `find.py func --brief`; README, CONTRIBUTING, `.gitignore`, `tools/setup.ps1` (pinned clones, dtk hash check, dtk analysis regenerated), `docs/REPRODUCE.md`, `tools/checksums.ps1` + `docs/expected_checksums.txt`. **Verified**: two full rebuilds give identical SHA-256 for all outputs incl. `main_dol.c`; `dtk dol split` regenerates `tools/dtk/proj/symbols.txt` byte-identical.

23. **Types, fields, parameters, globals** (Oct 27, 2026). The C now reads `cr_g_FileSlots[i].in_use`, `HSD_JObj *`, named parameters:
   - `tools/clean_headers.py` makes a declarations-only copy of Melee's HSD + SDK headers (inline bodies dropped; Ghidra misparses `T (*p)[N]`, rewritten). `tools/scripts/ParseHeaders.java` parses them + our `symbols/cr_types.h`, checks **17/17 ASSERT_SIZE and 95/95 `/* +XX */` field offsets**, applies **439 prototypes** (432 verified library functions, 7 `cr_`), types **11 globals** from `symbols/cr_globals.txt`.
   - **Checked against the game itself** (`tools/check_class_sizes.py`): every HSD class registers its sizes at startup (`hsd_jobj` 0x54/0x88, …). 10 object structs match. Found: CR's `HSD_PObj` is 0x1C (Melee 0x18); its load method (0x80193B14) fills +4..+14 like Melee, so the extra word is at the end (override `cr_unk_18`). Every `HSD_*Info` method table is +4 (one extra method; `HSD_ObjInfo` itself matches) - these types are not applied.
   - First `cr_` types: `cr_FileDesc` (0x10), `cr_PackedEntry` (8), `cr_PackedArchive` (0x44), `cr_FileSlot` (0x78, ends in a `DVDFileInfo`). The globals fit back to back exactly (slots end where the dir tree starts; 43 packed archives end at the ready flag).
   - `tools/batches/batch5_types.ps1` stops the run on any type mismatch. Rebuilds still byte-identical (13 checksummed outputs).

24. **Mods back into the game** (Oct 27, 2026): `tools/make_mod.py <mod dir>` turns a mod folder (edited `models/<cat>/<name>.dat`, replaced `files/`, `code/*.txt` main.dol patches with a checked `[was ...]`) into a playable ISO and a `.crpatch`. `tools/iso_patch.py`: `build` writes changed files over the clean ISO (in place or appended, FST/boot.bin updated), `diff` stores clean-ISO data as references (also when moved; LZ-style search around the same place) and only new bytes, `apply` checks base and result SHA-1. Tested: robo 0 recolour = 43 KB of new bytes; applied ISO identical (SHA-1); boots in Dolphin ("Custom Robo (GXCE01)"). `from_scratch.ps1` now also does a patch round trip. Guide: `docs/MODDING.md`. Not yet: different skeletons (blocks A/B/D), `*_effect` containers, new files on the disc, a main.dol that grows.

25. **Robo skeletons decoded; custom skeletons supported** (Oct 27, 2026). Found by following the robo entry pointer: `cr_RoboModel_OnLoaded` only relocates and stores the entry (`cr_g_RoboEntries`, 100 x 8 bytes, ends exactly at the story-character table); `cr_Robo_BuildModel` (0x80074110) builds the robo: root joint, then **block C = 19 joint role paths** -> robo+0xB44, **block B = one path ending FE** -> that joint's MObj at +0xBC0, block D = extra paths (0 in all robos), **block A = 0x20 limb floats** -> +0xB3C -> `cr_g_CurrentLimbParams`, read only by the procedural arm/leg code. Roles named from robo 0's bind pose (head, 2 x 4-bone arms, 2 x 4-bone legs, upper/lower body); every robo has the pattern with its own joint numbers (51-69 joints). `tools/robo_joints.py` decodes / re-encodes paths: **170/170 robo entries re-encode byte-identical** (`selftest`, now in from_scratch). `make_mod.py` takes `<model>.roles.json` (+ optional `A_from`) for a new skeleton and refuses a changed skeleton without one. Tested: robo 1 model (entry 005, 59 joints) in entry 000 (robo 0, 55 joints) builds, 32 KB patch (177 new bytes), boots. `iso_patch.py diff` now merges back-to-back copies (3 MB -> 32 KB). Open: in-battle confirmation, each block A float, block B material use, left/right.

26. **main.dol keeps the archive entry tables** (Oct 27, 2026). First in-game test of the skeleton-swap mod (by the user): "Game Disc could not be read" + invalid reads. Cause: every packed archive's entry table is duplicated in main.dol (`cr_g_FileDescs[i]->packed`, all 13 equal the disc headers) and the game reads the copy; robo 1's entry 005 is smaller, so chara.BIN shrank by 0x800 and the old table pointed past the end. `make_mod.py sync_entry_table` now rewrites the copy (checked against the clean archive first). `tools/boot_check.ps1` boots an ISO and reports Dolphin warning dialogs. Note: a warning during the intro video was also seen once on the **clean** ISO in this Dolphin build (another 120 s run: none), so intro-video warnings are not by themselves a mod problem. In-battle check of the fixed skel_swap still pending.

27. **Naming rule: robo N = chara entries 5N..5N+4** (Oct 27, 2026; user correction). Earlier notes said "robo 5" for entry 005 - that is robo 1 variant 0; fixed in docs and tool output (`robo_joints.py table` shows robo/variant). Community test build: robo 0 <-> robo 4, all 5 variants each (entries 000-004 <-> 020-024, 55 <-> 53 joints).

28. **Per-robo animations and height** (Oct 27, 2026; community test). Robo 0 <-> robo 7 body swap: robo 7's body ran with robo 0's animations (and looked robo 0's size - see step 29: the size table guess was wrong), but its own charge. Cause: cr_Robo_BuildModel reads two tables by robo number - `cr_g_RoboHeight` (0x8021B080, f32[56]; first misnamed RoboScale) and `cr_g_RoboMotionSets` (0x8021B160, 56 pointers to 9 shared 17-ID `cr_MotionSet`s) - while the charge is the model's own joint animation. Only these two tables are indexed by robo+0x3F4 in the code. `tools/robo_swap.py` now swaps both (as checked main.dol patches); `--model-only` keeps the old behaviour. Open: which of the 17 IDs is which action; where motion IDs are loaded from (motion.DBN / snr_motion.bin).

29. **Community test log + corrections** (Oct 27, 2026, live test with community members). Tested in Dolphin 2609a (Project Hive):
   | Build | Result |
   |---|---|
   | skel_swap v1 (entry 000 <- entry 005, no main.dol table sync) | "Game Disc could not be read" + invalid reads -> step 26 fix |
   | swap robo 0 <-> robo 7, bodies only | battle runs; robo 7's body uses robo 0's animations; its own charge |
   | swap robo 0 <-> robo 6 (55 <-> 54 bones) + motion sets | battle runs, robos animate; user: "charge animation was not linked" (details pending) |
   | same + `cr_g_RoboHeight` x3 | robos **not** bigger -> the table is not size |
   | same + root joint scale 3 (`--size 3`), A: motion swapped / B: motion kept | built; superseded by the next test |
   | Ray 01 own body, root joint scale 3 + motion set pointers 0 <-> 6 (`mods/ray_moves_as_robo6_size3`) | **regular moves and regular size**: neither change has a visible effect in battle |
   Corrections: (1) entry 005 is robo 1, not "robo 5" (step 27). (2) 0x8021B080 is not a size: robo+0xE14 is **added to the Y position** in FUN_8002fefc and used in FUN_80054238 -> renamed `cr_g_RoboHeight` (meaning still open). (3) The size factor the limb code uses (robo+0x3C0, read as DAT_8044f558+0x154) is set to 1.0 (FLOAT_80451284) for **every** robo in FUN_800ba488, and every robo model's root joint scale is 1.0: robo size lives in the vertices; `robo_swap.py --size X` sets the root joint scale (experimental).
   Known issue (not caused by mods): a warning "Invalid read from 0x04efe5cb, PC = 0x801b5f38" in the THP decoder during the intro video, seen on the clean ISO too; click "Ignore for this session".
   Conclusions from the last test: (a) the root joint scale is overridden at run time (most likely the joint animations set it every frame), so `--size` does not work; size needs scaled vertices or another hook. (b) Battle movement is **not** chosen through `cr_g_RoboMotionSets` (it is read by the robo builder and by the script command FUN_800cf5ec / FUN_800d0268 - likely scripted scenes); the earlier body-only swap result (robo 7's body with robo 0's moves) means battle moves stay with the robo number through something else, still to find.
   Open: what "charge not linked" is exactly; which of the 17 motion slots is which action; where motion data lives (motion.DBN / snr_motion.bin); robo+0xE14 meaning.

30. **Robo object struct `cr_Robo`** (Oct 27, 2026). Robos in play live in a static array `cr_g_Robos` at 0x803BFE88: 24 slots x 0x19A8 bytes = 0x267C0, exactly the object size decomp-toolkit found there. `cr_Robo_UpdateAll` (0x80099EAC) calls each active slot's `update` every frame and adds 1.0 to `frame_count`. Fields named with evidence (symbols/cr_types.h): update, pos, motion, limb_scale, flags, number, frame_count, motion_set, limb_params, root_joint, role_joints[19], extra_joints[12], material, height, ...; unknown stretches stay unk_XXXX. Globals: cr_g_Robos, cr_g_CurrentRobo (now cr_Robo*), cr_g_FirstRobo, cr_g_CurrentRoboIndex. Result: 1,563 field accesses in 50 functions now read as `robo->field`; all type checks pass (22 sizes, 154 offsets). Next: name the most-used unknown stretches (unk_194c 122 uses, unk_0bc8 113, unk_06f8 62), and find what picks battle movement.

31. **Typed fields in `cr_Robo`** (Oct 27, 2026). New `tools/struct_fields.py <struct> report|apply`: scans every access into a struct's unknown stretches (`*(float *)(robo->unk_194c + 0x10)`, `robo->unk_0bc8[3]`), and splits the stretches into typed fields where all accesses agree on one size and nothing overlaps; names stay `unk_XXXX` with "auto: N uses in M functions". cr_Robo: 105 + 1 typed fields; 11 offsets with conflicting access sizes stay unknown on purpose. Field offsets verified: 312 (was 154), size still 0x19A8. Report: docs/struct_cr_Robo.md. Next: real names for the most-used fields, from the functions that use them.

32. **First real field names in cr_Robo; sin/cos** (Oct 27, 2026). `cr_Sin16` (0x8001F4C0) / `cr_Cos16` (0x8001F428): lookup-table sine/cosine on 16-bit binary angles (0x10000 = full turn, quarter-wave table at 0x801E6584); 65 uses now named. `cr_Robo.angle` (+0x58): the angle passed to them (facing/turn angle; set by scripts, advanced by +0x1958's value). `cr_Robo.flags_1942`: bit flags (meaning open). Ranked by use, the next fields to name: unk_0bc8 (an array of 0x48-byte records holding 0xC-byte vectors: needs its own struct), unk_193c (0x10-byte block copied with FUN_80023264), unk_193e, unk_1988, unk_1944.

## Findings from the decompiled code

- **File table**: pointer list at `0x801f1200` → 16-byte entries `{char* name, ?, ?, extra}` at `0x801f21e0+`.
  Index 0 = `logo_read_us.dbn`, **1 = `chara.bin`**, 2 = `scenario/snr_t_01.bin`, 3 = `scenario_us/snr_chara.bin`,
  4 = `snr_map.bin`, 5 = `rpg_t_models.bin`, 6 = `rpg_f_models.bin`, 7 = `snr_motion.bin`.
- **Archive accessors** (behaviour observed; functions keep their `FUN_` names — no verified real name exists): `FUN_80020a70(archive, entry)` → entry disc offset; `FUN_800202ec` → entry size; `FUN_80020e44` reads it into memory.
- **Robo loading**: `FUN_800a63c0` loads `chara` entry `robo_id * 5` → **34 robos × 5 entries** (the 5 entries per robo are the same size — guess: colour variants, not verified). Line numbers in `main_dol.c` change on every rebuild; search for the function address.
  The same function also loads archives 8/9/10/11 by a category byte (`DAT_803bfc95`) — looks like the in-game model viewer (`viewer.ini`).
- **Model loaders** (behaviour read from the code; functions keep `FUN_` names): `FUN_800747b0` and `FUN_800748a8` load robo / story-character entries; `FUN_80073cb8` loads weapon entries. Both relocate pointers from the entry's reloc table, then load root joints with `FUN_801988fc` and attach the first joint animation (if any) with `FUN_80196e0c`.

## Model format (robo / weapon / story-character entries)

Source: the loaders above, cross-checked by HSDLib parsing all files, exports, and a byte-identical round trip of all 484 entries.

| word | robo / story char (`FUN_800747b0`, `FUN_800748a8`) | weapon (`FUN_80073cb8`) |
|---|---|---|
| 0 | data base = 0x40; all pointers are offsets from here | same |
| 1 | file offset of the end of trailer blocks A/B/C (not read by these loaders) | same |
| 2 | → pointer to one **JOBJDesc** {RootJoint, JointAnims, …} | → array of word-3 **JOBJDesc** records (0x10 each) |
| 3 | file offset of block A (0x80 bytes, floats; purpose unknown) | number of JOBJDesc records |
| 4 | file offset of block B (bytes, e.g. `02 fe`; purpose unknown) | block B (offset from 0x40) |
| 5 | small value (1 in every robo seen; not read by these loaders) | -1 / small value |
| 6, 7 | reloc table file offset, entry count | reloc table offset from 0x40, entry count |
| 8 | file offset of block C = **joint path list** (02 = child, 01 = next sibling, ff = end); robo loader walks 0x13 paths to pick joints | block C (offset from 0x40) or -1; walked per record |
| 9 | 0, or file offset of an extra block D running to end of file (some `snr_chara` only; inferred from data, no loader seen reading it) | 0 / -1 |

- Data block = file[0x40 : reloc table]: standard HSD JObjDesc (0x40), DObjDesc, MObjDesc, TObjDesc (CMPR textures), PObjDesc (vertex attributes, display lists, envelopes).
- **Unknown, carried over unchanged when repacking**: blocks A, B, D, word 5. The joint path list (C) depends on the skeleton — a custom model with a different joint tree will need new paths.
- `.dat` root names written by `crmodel.py` (`cr_header2`, `root<i>_joint`) are **ours**; the game files carry no names. `cr_header2` must be kept: `from-dat` rebuilds header word 2 from it.

## Naming policy (applies to symbols/symbols.txt)

A name is applied only with an independent source:
1. **decomp-toolkit signature match** (SDK / runtime). Sizes can differ from Melee because Custom Robo uses a newer SDK.
2. **Melee decomp match**: assert (file, expression) match **and** function size within 10% of Melee's verified size.
3. **Layout proof**: same neighbours + same/near size as Melee's verified symbol (used for `__assert`, `HSD_Panic`, `OSPanic`).
Before rejecting a name as "not in Melee", search all of Melee (config + src): it may be `static`/`inline` and folded into its caller (found for `FObjLoadWait` and `IDEntryAlloc`; the caller `HSD_IDInsertToTable` was verified and applied instead).
Everything else (descriptive guesses) lives in `symbols\unverified.txt` and is never applied. Earlier invented names (`File_ReadEntry`, `Viewer_LoadModel`, `Game_Panic`, …) were removed; `Game_Panic` turned out to be `OSPanic`.

Current state: 348 function names listed — 298 dtk, 46 Melee-assert, 4 layout-proven (`__assert`, `HSD_Panic`, `OSPanic`, `HSD_IDInsertToTable`). 0 non-dtk names fail verification.
Ghidra applies 347; `__save_fpr` / `__restore_fpr` / `__save_gpr` / `__restore_gpr` are runtime code fragments that Ghidra folds into neighbouring functions, and `OSDefaultExceptionHandler` did not get its own function header (not yet investigated).

## How to view models now

- Ready-made exports: `models_export\<category>\<name>\*.glb` (open in Blender: File → Import → glTF) with textures beside them.
- To inspect or edit: run `HSDRawViewer.exe`, File → Open → any file in `models_dat\<category>\`, select a `root0_joint` (robo/weapon) or `scene_data` (stages, effects, objects) node.
- Regenerate everything from the disc: `sfd.py unpack` → `extract_models.py` → `export_models.py`.

## First decompile results, v1 (Oct 25, 2026) — superseded, files deleted

Kept for history. v1 was run without the GameCube analyzer; its output and project were replaced by the current ones (see next section).

- 3,387 functions exported, 0 failed. Analysis, import and save all succeeded.
- v1 `main_dol.c` was about 9 MB, 270,586 lines.
- About 3,366 functions are still generic `FUN_8xxxxxxx` names. No debug symbols came through.
- Warnings are mostly routine. 2,829 are "removing unreachable block". A handful are infinite loop, stack pointer, and type propagation warnings, worth a look only if those functions matter.
- 22,062 `unaff_r13` and 12,882 `unaff_r2` references. Ghidra does not know the small data base registers, so globals and strings show up as raw offsets.
- Only 313 strings got labels. File names like `chara.bin`, `snr_chara.bin`, `rpg_t_models.bin`, `rpg_f_models.bin`, `snr_motion.bin` and the scenario files are in main.dol but are not linked in the C output.
- SDA base values, read from the startup code at 0x8000332C
  - r13 = 0x804543C0 (.sdata start + 0x8000)
  - r2 = 0x804581A0 (.sdata2 start + 0x8000)
- A few functions around 0x8015C0A8 load r2 as a temporary and may still decompile oddly after the registers are fixed.
- Function ID matched nothing. No GameCube signature databases are installed in Ghidra.
- No existing public Custom Robo decomp or model format documentation was found. The Model Swapper on romhacking.net is the only prior tool found.

### Phase 0 step 1 result — loader analyzer enabled (confirmed)

Re-ran with `-preScript EnableGCAnalyzer.java` (sets `(GameCube/Wii) Program Analyzer` = true). This is now the only version: `ghidra_project\CustomRobo` and `decompiled\main_dol.c` (v1 deleted during cleanup).

| | v1 (deleted) | After analyzer (step 1) | Now (names + strings applied) |
|---|---|---|---|
| Lines | 270,586 | 222,133 | 222,965 |
| `unaff_r13` | 22,062 | **0** | 0 |
| `unaff_r2` | 13,682 | **792** (71 functions) | 852 (74 functions; why it rose from 792 was not investigated) |
| Labeled strings (`s_`) | 378 | 388 | 513 |
| Functions / generic `FUN_` | 3,387 / 3,382 | 3,387 / 3,382 | 3,416 / 3,038 |

Steps 2 and 3 of the Phase 0 ladder were done afterwards (see Roadmap).

### Root cause of the missing names (confirmed)

The GameCube loader ships its own "(GameCube/Wii) Program Analyzer" that finds and sets r13 and r2. It never ran in our first headless run. In the loader source it only turns itself on by default when the program's format name equals "Nintendo GameCube/Wii Binary", but the loader now names itself "Nintendo GameCube/Wii Binary (Executable)", so the check fails and the analyzer stays off. **Confirmed**: turning it on (`EnableGCAnalyzer.java`) took `unaff_r13` from 22,062 to 0.

## Roadmap

Each phase has a goal, the ideas for reaching it, and a finish line. Within a phase, work down the ladder and stop as soon as the finish line is met.

### Phase 0. Readable, named code (prerequisite for everything else)

**Goal.** Decompiled output where globals, strings and SDK functions have real names, and every name we add survives outside Ghidra.

**Ladder, cheapest first**
1. ~~Use what already exists. Turn on the loader's own GameCube/Wii analyzer and reanalyze. Compare the counts above before and after.~~ **Done**: r13 fully resolved, r2 down 94%. See results above.
2. ~~Do it ourselves.~~ **Done**: `DefineStrings.java` defines 392 strings in data sections on every rebuild (r13/r2 already handled by step 1).
3. ~~Bring in outside analysis.~~ **Done**: dtk signatures (298 names) + Melee assert matching (46 verified names), all applied by `ApplySymbols.java`.
4. ~~Verify against a second source.~~ **Done (first pass)**: `tools\crosscheck_dtk.py`, see step 21. Still open: the 34 Ghidra-only functions and the 74 `unaff_r2` functions.
5. Last resort, fix the tools. If Ghidra or the loader still can't resolve r13 and r2, patch the loader (the enablement check and any gaps in its SDA handling), teach Ghidra's Gekko compiler spec to treat r13 and r2 as fixed known values, or write our own analyzer. Keep fixes as patches we can rebuild from and offer upstream.

**Keeping names safe**
- The Ghidra project is the working copy. An exported symbols file (name, address, type) is the source of truth.
- A script can wipe and reapply every name from that file, so a fresh import never loses work.
- The symbols file goes under version control. Names and addresses are not Nintendo code, so it can be shared later.

**Done when** (status Oct 25, 2026)
- [x] `unaff_r13` = 0; `unaff_r2` = 852 in 74 functions (down from 13,682).
- [~] File-name strings are defined (`s_chara_bin_801b6a66` etc.), but they are reached through the archive table at `0x801f1200`, so the C shows the table, not the string. Who uses the table is known (see Findings).
- [~] SDK: OS, DVD, GX (partial), TRK, C runtime named. Audio (AX) and most GX setters are not named by dtk's signatures (reason not checked).
- [x] C export regenerated with the verified names.
- [x] `tools\rebuild_decomp.ps1` does a from-scratch import and reapplies everything from `symbols\symbols.txt`.
- [ ] Symbols file under version control (git not set up yet).

### Phase 1. Models (main priority)

**Goal.** Every model in the game can be opened, exported, replaced with a custom model, and loaded in game.

**Pipeline**
- [x] Repack mode in the SFD tool (`sfd.py pack`), correct offsets, sizes and 0x800 alignment.
- [~] Fast test loop by booting the extracted folder in Dolphin: boots (step 20); automatic screenshots not working yet. Rebuild the ISO only for releases.
- [x] Automatic check that unpack then repack gives byte-identical archives (`sfd.py verify`: 22/22).

**HSD models (stages, title, effects)**
- [x] Confirm they open and export (all exported by `export_models.py`).
- [ ] Import a custom model into a .dat, repack, and see it in game.

**Custom format models (robos and weapons)**
- [x] Trace the loaders (robo `FUN_800747b0`/`FUN_800748a8`, weapon `FUN_80073cb8`).
- [x] Map the format: it is standard HSD (vertices, normals, UVs, skinning, materials, textures handled by HSDLib). Header documented above. Animation (`snr_motion`) not yet mapped.
- [x] Document the format (see "Model format").
- [x] Exporter (`export_models.py`). [~] Importer: `crmodel.py from-dat` rebuilds a game entry from a .dat; tested with an edited + relocated model offline and boots (steps 19–20); in-game look not yet confirmed; not yet tested with a new skeleton.
- [x] Byte-identical round trip on untouched models: 484/484.
- [ ] Find the limits (file size, memory, vertex and bone counts).
- [x] Work out blocks A/B/D and the joint path list (C) well enough to generate them for a new skeleton (step 25; block A reused from a similar robo).

**Other model containers**
- [x] Survey: `snr_chara` = robo format, `snr_obj` = standard .dat, `*_effect` = count+offset container of standard .dat (all extracted).
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

### Phase 3. Community release

**Goal.** Anyone with their own copy of the game can mod it.

**Ship**
- Tools, format docs, and the symbols file.
- Mods as patches applied to the user's own clean retail ISO, with a hash check against GXCE01 before applying.
- A public repo with a contributor guide.

**Never ship**
- The ISO, extracted files, ripped models, textures or audio, `main_dol.c`, or any other decompiled output.

**Done when**
- Someone outside the project can take a clean ISO, run the tools, and play a custom model.

## References

- HSDLib / HSDRawViewer: https://github.com/Ploaj/HSDLib
- Ghidra: https://github.com/NationalSecurityAgency/ghidra
- GameCube loader: https://github.com/Cuyler36/Ghidra-GameCube-Loader
- Custom Robo Model Swapper: https://www.romhacking.net/utilities/1735/
- MeltyTool: https://github.com/MeltyPlayer/MeltyTool
- decomp-toolkit: https://github.com/encounter/decomp-toolkit
- Ghidra PowerPC SDA issue #325: https://github.com/NationalSecurityAgency/ghidra/issues/325
- GameTDB GXCE01 (retail hashes): https://www.gametdb.com/Wii/GXCE01
