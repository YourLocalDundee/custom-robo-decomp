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

25. **Robo skeletons decoded; custom skeletons supported** (Oct 27, 2026). Found by following the robo entry pointer: `cr_CharaEntry_OnLoaded` only relocates and stores the entry (`cr_g_CharaEntries`, 100 x 8 bytes, ends exactly at the story-character table); `cr_Chara_BuildModel` (0x80074110) builds the robo: root joint, then **block C = 19 joint role paths** -> robo+0xB44, **block B = one path ending FE** -> that joint's MObj at +0xBC0, block D = extra paths (0 in all robos), **block A = 0x20 limb floats** -> +0xB3C -> `cr_g_CurrentCharaLimbParams`, read only by the procedural arm/leg code. Roles named from robo 0's bind pose (head, 2 x 4-bone arms, 2 x 4-bone legs, upper/lower body); every robo has the pattern with its own joint numbers (51-69 joints). `tools/robo_joints.py` decodes / re-encodes paths: **170/170 robo entries re-encode byte-identical** (`selftest`, now in from_scratch). `make_mod.py` takes `<model>.roles.json` (+ optional `A_from`) for a new skeleton and refuses a changed skeleton without one. Tested: robo 1 model (entry 005, 59 joints) in entry 000 (robo 0, 55 joints) builds, 32 KB patch (177 new bytes), boots. `iso_patch.py diff` now merges back-to-back copies (3 MB -> 32 KB). Open: in-battle confirmation, each block A float, block B material use, left/right.

26. **main.dol keeps the archive entry tables** (Oct 27, 2026). First in-game test of the skeleton-swap mod (by the user): "Game Disc could not be read" + invalid reads. Cause: every packed archive's entry table is duplicated in main.dol (`cr_g_FileDescs[i]->packed`, all 13 equal the disc headers) and the game reads the copy; robo 1's entry 005 is smaller, so chara.BIN shrank by 0x800 and the old table pointed past the end. `make_mod.py sync_entry_table` now rewrites the copy (checked against the clean archive first). `tools/boot_check.ps1` boots an ISO and reports Dolphin warning dialogs. Note: a warning during the intro video was also seen once on the **clean** ISO in this Dolphin build (another 120 s run: none), so intro-video warnings are not by themselves a mod problem. In-battle check of the fixed skel_swap still pending.

27. **Naming rule: robo N = chara entries 5N..5N+4** (Oct 27, 2026; user correction). Earlier notes said "robo 5" for entry 005 - that is robo 1 variant 0; fixed in docs and tool output (`robo_joints.py table` shows robo/variant). Community test build: robo 0 <-> robo 4, all 5 variants each (entries 000-004 <-> 020-024, 55 <-> 53 joints).

28. **Per-robo animations and height** (Oct 27, 2026; community test). Robo 0 <-> robo 7 body swap: robo 7's body ran with robo 0's animations (and looked robo 0's size - see step 29: the size table guess was wrong), but its own charge. Cause: cr_Chara_BuildModel reads two tables by robo number - `cr_g_CharaYOffset` (0x8021B080, f32[56]; first misnamed RoboScale) and `cr_g_CharaMotionSets` (0x8021B160, 56 pointers to 9 shared 17-ID `cr_MotionSet`s) - while the charge is the model's own joint animation. Only these two tables are indexed by robo+0x3F4 in the code. `tools/robo_swap.py` now swaps both (as checked main.dol patches); `--model-only` keeps the old behaviour. Open: which of the 17 IDs is which action; where motion IDs are loaded from (motion.DBN / snr_motion.bin).

29. **Community test log + corrections** (Oct 27, 2026, live test with community members). Tested in Dolphin 2609a (Project Hive):
   | Build | Result |
   |---|---|
   | skel_swap v1 (entry 000 <- entry 005, no main.dol table sync) | "Game Disc could not be read" + invalid reads -> step 26 fix |
   | swap robo 0 <-> robo 7, bodies only | battle runs; robo 7's body uses robo 0's animations; its own charge |
   | swap robo 0 <-> robo 6 (55 <-> 54 bones) + motion sets | battle runs, robos animate; user: "charge animation was not linked" (details pending) |
   | same + `cr_g_CharaYOffset` x3 | robos **not** bigger -> the table is not size |
   | same + root joint scale 3 (`--size 3`), A: motion swapped / B: motion kept | built; superseded by the next test |
   | Ray 01 own body, root joint scale 3 + motion set pointers 0 <-> 6 (`mods/ray_moves_as_robo6_size3`) | **regular moves and regular size**: neither change has a visible effect in battle |
   Corrections: (1) entry 005 is robo 1, not "robo 5" (step 27). (2) 0x8021B080 is not a size: robo+0xE14 is **added to the Y position** in FUN_8002fefc and used in FUN_80054238 -> renamed `cr_g_CharaYOffset` (meaning still open). (3) The size factor the limb code uses (robo+0x3C0, read as DAT_8044f558+0x154) is set to 1.0 (FLOAT_80451284) for **every** robo in FUN_800ba488, and every robo model's root joint scale is 1.0: robo size lives in the vertices; `robo_swap.py --size X` sets the root joint scale (experimental).
   Known issue (not caused by mods): a warning "Invalid read from 0x04efe5cb, PC = 0x801b5f38" in the THP decoder during the intro video, seen on the clean ISO too; click "Ignore for this session".
   Conclusions from the last test: (a) the root joint scale is overridden at run time (most likely the joint animations set it every frame), so `--size` does not work; size needs scaled vertices or another hook. (b) Battle movement is **not** chosen through `cr_g_CharaMotionSets` (it is read by the robo builder and by the script command FUN_800cf5ec / FUN_800d0268 - likely scripted scenes); the earlier body-only swap result (robo 7's body with robo 0's moves) means battle moves stay with the robo number through something else, still to find.
   Open: what "charge not linked" is exactly; which of the 17 motion slots is which action; where motion data lives (motion.DBN / snr_motion.bin); robo+0xE14 meaning.

30. **Robo object struct `cr_Chara`** (Oct 27, 2026). Robos in play live in a static array `cr_g_Charas` at 0x803BFE88: 24 slots x 0x19A8 bytes = 0x267C0, exactly the object size decomp-toolkit found there. `cr_Chara_UpdateAll` (0x80099EAC) calls each active slot's `update` every frame and adds 1.0 to `frame_count`. Fields named with evidence (symbols/cr_types.h): update, pos, motion, limb_scale, flags, number, frame_count, motion_set, limb_params, root_joint, role_joints[19], extra_joints[12], material, height, ...; unknown stretches stay unk_XXXX. Globals: cr_g_Charas, cr_g_CurrentChara (now cr_Chara*), cr_g_FirstChara, cr_g_CurrentCharaIndex. Result: 1,563 field accesses in 50 functions now read as `robo->field`; all type checks pass (22 sizes, 154 offsets). Next: name the most-used unknown stretches (unk_194c 122 uses, unk_0bc8 113, unk_06f8 62), and find what picks battle movement.

31. **Typed fields in `cr_Chara`** (Oct 27, 2026). New `tools/struct_fields.py <struct> report|apply`: scans every access into a struct's unknown stretches (`*(float *)(robo->unk_194c + 0x10)`, `robo->unk_0bc8[3]`), and splits the stretches into typed fields where all accesses agree on one size and nothing overlaps; names stay `unk_XXXX` with "auto: N uses in M functions". cr_Chara: 105 + 1 typed fields; 11 offsets with conflicting access sizes stay unknown on purpose. Field offsets verified: 312 (was 154), size still 0x19A8. Report: docs/struct_cr_Chara.md. Next: real names for the most-used fields, from the functions that use them.

32. **First real field names in cr_Chara; sin/cos** (Oct 27, 2026). `cr_Sin16` (0x8001F4C0) / `cr_Cos16` (0x8001F428): lookup-table sine/cosine on 16-bit binary angles (0x10000 = full turn, quarter-wave table at 0x801E6584); 65 uses now named. `cr_Chara.angle` (+0x58): the angle passed to them (facing/turn angle; set by scripts, advanced by +0x1958's value). `cr_Chara.flags_1942`: bit flags (meaning open). Ranked by use, the next fields to name: unk_0bc8 (an array of 0x48-byte records holding 0xC-byte vectors: needs its own struct), unk_193c (0x10-byte block copied with FUN_80023264), unk_193e, unk_1988, unk_1944.

33. **cr_Chara.vecs_0bc8** (Oct 27, 2026): the 0x240-byte stretch at +0xBC8 (101 uses, all 4-byte float accesses) is 48 `Vec3`, read as rows of 6 (stride 0x48 + 0xC) - typed `Vec3 vecs_0bc8[48]`; the C now reads `vecs_0bc8[i * 6 + 0x25].x` instead of raw casts. Rows of 6 match the 2 x 6 extra joints (block D) - meaning still open. Remaining next fields by use: unk_193c (0x10-byte block), unk_193e, unk_1988, unk_1944, unk_00bd, unk_1958 (added to angle each update - an angular speed?), unk_1984, unk_197c.

34. **Movement fields in cr_Chara** (Oct 27, 2026), each from its uses: `build_number` (+0x193E, passed to cr_Chara_BuildModel), `target_angle` (+0x1950, angle eases to it), `turn_speed` (+0x1958, added to angle each update; set as degrees*0x10000/360), `move_start` (+0x1960 Vec3, saved pos), `move_target` (+0x197C Vec3, pos steps toward it and lands on it), `countdown_198c`, `flags_1944`. Byte splits from the auto pass merged into the real 32-bit / Vec3 fields (305 offsets checked). FUN_8009aec4 and FUN_800b5d4c set move_target / turn_speed (scripted moves) - next to name. Still open: +0x196C..0x1978 (4 floats set together), unk_1940, unk_1954, unk_1990..1996.

35. **Area triggers** (Oct 27, 2026): `cr_AreaBoxes_Update` (0x800B5D4C) loops `cr_g_AreaBoxes` (0x803BAF28, 28 x `cr_AreaBox` of 0x40 bytes = the 0x700-byte object decomp-toolkit found there): flags (bit 1 in use, 0x20 test), min/max Vec3 box, robo index at +0x2A; tests whether that robo's x/z is inside the box (story-mode areas, most likely). FUN_8009aec4 (called after the robo loop) is a big game-mode state machine on DAT_803bf941 - not named yet.

36. **More cr_Chara fields** (Oct 27, 2026): `id` (+0x193C) / `target_id` (+0x1996) - one robo's target is compared with the others' ID; `built_number` (+0x1940, rebuild when build_number differs); `move_start_angle` (+0x1954); `countdown_1990`; `action` (+0x1994: low byte drives a switch, high byte tested; from script data). Open: the 4 floats +0x196C..0x1978 (written together, negated values), the conflicting byte/word offsets in docs/struct_cr_Chara.md, the big front of the struct (+0x5C..+0x2D6, +0x3F6..+0x6E8, +0x6F8..+0xB3C, +0xE18..).

37. **cr_Chara through its shortcut globals** (Oct 27, 2026): cr_Chara_SetCurrentPointers keeps 7 globals = current robo + fixed offset (+0x26C, +0x678, +0x6DC, +0x9E8, +0xE4C, +0x11CC, +0x154C; 1,000+ accesses, 500 through +0x6DC alone - the limb code). tools/struct_fields.py now counts accesses through these aliases as robo offsets: +184 typed fields; **548 field offsets checked** (was 311); 7 conflicting offsets left unknown in the whole struct. The globals are named cr_g_CurrentChara_XXXX (u8*). Next: meaning of the big blocks (+0x6DC and +0x154C are the limb / pose workspace used with block A).

38. **Matrix stack + limb posing** (Oct 27, 2026). The robo's arms/legs are built on a matrix stack: current matrix `cr_g_CurrentMtx` (0x8031FE68, 3x4), `cr_Mtx_TranslateX`, `cr_Mtx_TranslateZX`, `cr_Mtx_Mul`, `cr_Mtx_Store`, `cr_Mtx_Push` / `cr_Mtx_Pop` (stack top / depth / max depth globals), `cr_Sqrt` (51 callers). `cr_Chara_PoseLimbs` (0x8005AEE8) poses all four limbs with `cr_Chara_PoseLimbA` (block A floats 2.. and 8..) and `cr_Chara_PoseLimbB` (0xF.. and 0x15..): translate by block A lengths x limb_scale, rotate to a direction vector and cos/sin joint angles, store bone matrices (robo +0x11CC block), input matrices from +0x154C, end positions into the +0x6DC block. This is the code that makes swapped bodies move with their own proportions. Open: which pair is arms vs legs; per-field names inside +0x6DC / +0x11CC / +0x154C.

39. **Is cr_Chara the battle robo? - answered in step 40: no, field characters** (Oct 27, 2026). For "story-mode scene actor": FUN_800b81c4 / FUN_800ba488 rebuild the model when build_number changes and build robo 0xD instead of 0 when story flag 0x24C is set; script commands (FUN_800cf5ec) and area triggers drive these objects; motion sets / height / root scale changes had no visible effect in battle. For "battle robo" (or both): ground snapping (FUN_8006207c), a 64-entry x/z position trail (+0x5C, index +0x264; dash afterimages?), robos targeting each other (target_id vs id). No field is set to 1000 (battle HP), so HP likely comes from part data. FUN_800735c8 is not a second builder: it bulk-loads a container of robo entries into the shared cr_g_CharaEntries. Next: find where battle reads HP / part stats, and which object those land in.

40. **Real names first: game-wide harvest** (Oct 27, 2026; user rule: don't invent names while the game still holds real ones). New `tools/name_evidence.py` -> `docs/name_evidence.md`, `docs/name_tables.tsv`: 10 pointer tables to strings (enum names in index order) and 682 UPPER_NAME / 1,131 lower_snake / 81 source-file / 114 format strings with the code that uses them. Found: `SEQ_LOGO` ... `SEQ_ENDING` (15 game modes), `CAMMODE_*` (18 camera modes), script command names (`SYS_SET_SCR` ..., 300), object file IDs, and the **motion name table** (0x80206DB0, 3 strings per motion ID). The 17 motion-set slots are `wait, walk_fa, run, turn_l, turn_r, talk_a..talk_e, talk_question, talk_yes, talk_no, talk_oh, battle_in, battle_wait, battle_out` with `m_` / `w_` families - so the `cr_Chara` objects are **story-mode field characters** (they walk and talk), which settles step 39 and explains the community tests. Battle motions are separate (`sf_normal_walk_weapon`, `mg_back_run_guard`, ...). The struct name `cr_Chara` is now known to be imprecise; renaming is pending a decision. Next: apply the SEQ / CAMMODE / script-command names to the variables and switch cases that use them, then the battle motions.

41. **Accuracy pass: every name checked against its evidence; inaccurate ones renamed** (Oct 27, 2026; user: "I don't want any incorrect names, not one"). Renamed (old names replaced everywhere, this log included): character objects `cr_Robo` -> **`cr_Chara`** (they walk and talk; "chara" is the game's own word: chara.bin, snr_chara, CHAR_SCRIPT), `cr_g_Robos`/`CurrentRobo*`/`FirstRobo` -> `cr_g_Charas`/`CurrentChara*`/`FirstChara`, `cr_Robo_*` functions -> `cr_Chara_*`; `cr_g_RoboEntries`/`RoboEntryHeader`/`RoboEntrySlot`/`RoboModel_OnLoaded` -> `cr_g_CharaEntries`/`CharaEntryHeader`/`CharaEntrySlot`/`CharaEntry_OnLoaded` (a second loader fills the same table); `CharaModel_OnLoaded` -> `cr_ModelEntry_LoadAndBuild` ("story character" was never verified); `RoboHeight`/`height` -> `cr_g_CharaYOffset`/`y_offset` (only shown to be added to Y); `RoboMotionSets` -> `cr_g_CharaMotionSets`; `scene_id`/`target_scene_id` -> `id`/`target_id`; `action` -> `unk_1994` (only shown to drive a switch); `cr_AreaTrigger(s)` -> `cr_AreaBox`/`cr_g_AreaBoxes`/`cr_AreaBoxes_Update` ("trigger" assumed behaviour), field `robo` -> `chara`. Kept, with evidence: file system / archive names, matrix stack, cr_Sin16/Cos16, cr_Sqrt, limb posing, joint roles (chara.bin robo models), motion-slot names (the game's own). Note: plan entries above use the new names; where they once said "robo" for these objects, read "character".

42. **The game's own sequence names applied** (Oct 27, 2026). The SEQ_* name table (0x801DEAB4, 15 names) sits among five 15-entry function tables that FUN_8000878c / FUN_800055e8 (called by main) index with the same variable: on a change the Leave handler (0x801DEBA4) of the current sequence runs, previous = current, current = next, then the Enter handler (0x801DEB68); every frame three more run (FrameA 0x801DEA78, FrameB 0x801DEAF0, FrameC 0x801DEB2C). Independent check: SEQ_MOVIE's FrameA/FrameC are the two functions that assert seq_movie.c. Applied: enum `cr_Seq` with the game's value names (SEQ_LOGO .. SEQ_ENDING), `cr_g_SeqCurrent/Next/Previous` typed with it, `cr_g_SeqNames`, `cr_g_SeqEnter/Leave/FrameA/FrameB/FrameC`, and all 75 handlers named `cr_<SEQ name>_<role>` (roles only as the dispatcher shows them; FrameA/B/C not guessed as update/draw). The C now reads `cr_g_SeqNext = SEQ_MODE_SELECTOR`, `cr_g_SeqPrevious == SEQ_BATTLE`. Named functions 1,066 -> 1,151. Next by the same method: CAMMODE_* (18 camera modes), the script command names, the battle motions.

43. **CAMMODE_* not applied yet - no proof of which variable holds the camera mode** (Oct 27, 2026). The 18 names (table 0x801F88A4: CAMMODE_DEBUG, NOMOVE, MIDDLE, OPENING, BTL_WAIT, DICE_WAIT, RESULT, CUSTOMIZE, ROT_WAIT, PEAK, DRAW, SNR_CHARA, SNR_AUTO_MOVE, SNR_AUTO_Y_ANG, SNR_WATCH_MEM, STAGE_SELECT, STAGE_INFO x2) have no direct code reference; the code near them (FUN_80027170, FUN_80034bf0, FUN_8003491c) indexes the neighbouring table 0x801F8894, and the words pointing into that area lead to other strings. Per the real-names rule they are applied only once a variable is shown to index this table (or a handler table of the same length) - next lead: a 17/18-entry function table near 0x801F88A4, or a debug print that indexes it via r13/r2.

44. **The game's own script command names applied** (Oct 28, 2026). The 300 command names (table 0x8021BF44, `SYS_SET_SCR` ...) are directly followed by a 300-entry handler table (0x8021C3F4). Checked against main.dol: 300 distinct names, 300 distinct handlers, all 300 entries in descriptive.txt match the bytes. The script dispatcher (decompiled/main_dol.c, ~line 66839) does `if (*cmd < 300) ... (*(code*)(&PTR_FUN_8021c3f4)[*cmd])()`, with an extra `< 0x121` branch when DAT_8044f774 == 0 (meaning not known; not interpreted). Applied: 300 handlers named `cr_<COMMAND NAME>` (`cr_SYS_SET_SCR`, `cr_MAP_*`, `cr_CHAR_*`, ...), globals `cr_g_ScrCmdNames` (char*[300]) and `cr_g_ScrCmdFuncs`. The C now reads `(*cr_g_ScrCmdFuncs[*DAT_8044f778])()`. Full rebuild OK (104 s, 0 lint problems, types 0 failed); named functions 1,151 -> 1,451 (of 3,565). expected_checksums.txt refreshed (changes only in names: types_report, crosscheck_report, game_map.tsv, main_dol.c, functions/refs.tsv; main.dol, dtk, symbols.txt unchanged). Next: argument/parameter meaning of the commands from their handlers, CAMMODE_* once a variable is shown to index it, then the battle motions.

45. **CAMMODE_* applied: the camera mode is byte 0 of the camera object** (Oct 28, 2026; resolves step 43). `cr_Cam_SetMode` (0x80024830, 44 calls in the C) does `cam[0] = mode; cam[0x13CC] = 0` on `cr_g_CurrentCam` (0x8044F480, -> one of 4 x 0x13D0 cameras in `cr_g_Cams` 0x80324BD0). Every camera driver calls `cr_g_CamModeFuncs[cam[0]*2 + cam[0x13CC]]` (0x801F7FF0: 42 code pointers back to back = 21 [Start, Frame] pairs; checked against main.dol). The game's name table (0x801F88A4, 18 names, no code reference: debug data) lines up with the values for 0..16: SEQ_LOGO sets 1 and mode 1's Frame is empty (NOMOVE); SEQ_BATTLE sets 3/2/6/10 (OPENING/MIDDLE/RESULT/DRAW); the battle chain OPENING -> BTL_WAIT -> DICE_WAIT -> ROT_WAIT -> MIDDLE and PEAK -> RESULT or DRAW; the script commands CM_PLAYER / CM_MOVE... / CM_ROTATE / CM_WATCH_MAN_KEEP set 11..14 (SNR_*); SEQ_STAGE_SELECTOR sets 15/16. Applied: enum `cr_CamMode` (0..16), `cr_g_CamModeNames/Funcs`, `cr_g_CurrentCam`, `cr_g_Cams`, `cr_g_CamPtrs`, `cr_Cam_SetMode`, `cr_Cam_Init`, `cr_CamMode_DefaultStart` and 22 handlers `cr_CAMMODE_<name>_Start/Frame`. Not named on purpose: modes 17..20 (17's name entry repeats CAMMODE_STAGE_INFO, 18..20 are past the end of the name table) and 0x80024848 (Frame of both STAGE_SELECT and STAGE_INFO). 0x801F8894 turned out unrelated (4 per-player position buffers). Next: what modes 17..20 are (callers FUN_801076d8, FUN_8010a5cc, FUN_801314d4, FUN_80016600).

46. **One motion ID space for field and battle; battle motion sets named** (Oct 28, 2026). 0x80206DB0 is the game's only motion name table (step 40 called it the field table): 1095 IDs x 3 strings (name, "unknown", "2001/**/**"); 0x001..0x106 battle (sf_/mg_/ab_ families, per-body win/lose/cr_atk/powerup), 0x107..0x1A1 battle_dummy_046..200, 0x1A2..0x446 field (m_/w_/r_; the "MOTION = %s" viewer FUN_800a0e64 steps exactly this range). Parallel: 0x80205C94 holds 1095 record pointers in the same order and ends exactly at the name table ([0] = 0, all 155 dummies share one record; checked against main.dol). Motion files register per ID into `cr_g_MotionData` / `cr_g_MotionDataBit15` (`cr_MotionFile_Register`); `cr_Motion_Play` (0x80037D14) stores ID & 0x1FFF in cr_Chara.motion or battle object +0xDA (the shortcut globals point into the battle object via `cr_Battle_SetCurrentPointers`, 0x80050F08) and falls back to 0x1E7 m_win_guts if not loaded. Battle motion sets: 34 x 38 u16 at 0x801C51B8 (`cr_BattleMotionSet`, slot names from the game's strings: normal_ta_kamae, normal_walk, ..., powerup, lose, win_01/02, atk, change); the slot played is `cr_g_BattleMotionSlot` (0x8044F4AC), set by the 23 handlers of table 0x801F8EB8 (state meanings not known, not named). Applied: enum `cr_MotionId` (1095 game names, prefix MOTION_ from "MOTION = %s"; documentation only - fields stay u16), structs `cr_MotionName` / `cr_BattleMotionSet` (ASSERT_SIZE ok), 6 globals, 5 functions. Named functions 1,451 -> 1,481. Next: battle state handlers (0x801F8EB8), the 0x801C5C10 per-body records (+0x1CC; body names table 0x801C6C00 is a lead), cr_Chara +0x690 (likely motion length) / +0x692 (next-motion code).

47. **Library naming round: 767 -> 391 unnamed library functions, 0 wrong names** (Oct 28, 2026). Named functions 1,481 -> 1,856 of 3,565. Ten more pinned references (tools/pins.txt, fetched by setup.ps1): Kirby Air Ride (doldecomp/kar incl. asm, wowjinxy/KAR map), Pokemon XD, dolsdk2001/2004, Animal Crossing, Super Mario Sunshine, Mario Party 4, Pikmin 1, Battle for Bikini Bottom. New matchers, all rerun by `tools/batches/batch6_lib_candidates.ps1` in rounds until nothing new (names found by one become neighbours for the others): `tools/match_lib_hsd.py` (HSD + hsd audio; R4/R5/R6 against Melee C and Kirby Air Ride asm/C), `tools/match_lib_sdk.py` (Dolphin SDK; R3/R5), `tools/match_lib_rt.py` (MSL / runtime); merged and conflict-checked by `tools/merge_lib_candidates.py` (report `symbols/lib_candidates_report.txt`). Leftovers with the sources checked: `symbols/lib_unnamed_{hsd,sdk,rt}.txt`. **Wrong names found and fixed** (in symbols.txt since Oct 25-27, all layout-proof): the 0x20 math wrappers were shifted by one (asin/atan2/exp/fmod/log10 at 0x80169088.. are really acos/asin/atan2/exp/fmod - each calls the matching __ieee754_*), the 0x24 float wrappers likewise (now powf/tanf/sinf/cosf at 0x80169394..), PackArgs -> GetFontCode, OSSetFontEncode -> ReadROM, VDevice_Shutdown -> stateCheckID2a, GXSetTevKColor -> GXSetTevKAlphaSel (writes the TEV ksel alpha fields), __AXGetAuxBOutput -> GXInitLightAttn (stores the 6 attenuation floats). Cause: layout alignment cannot tell equal-size neighbours apart; `tools/audit_layout.py` audited all 669 old layout names (323 confirmed, 282 unique position, 8 wrong, 14 unproven, 42 doubtful - mostly TRK; `symbols/layout_audit.txt`) and layout_match.py now needs a unique alignment or call-graph/string confirmation. New every-run cross-check `tools/crosscheck_names.py` (batch8, after the Ghidra export) found the float-wrapper shift on its first run and stops the pipeline on any FAIL. Dropped on purpose (correct-looking but not proven by these rules): 7 names whose evidence only came from each other (UnsetRun, SetEffectivePriority, __CARDWritePage, BlockWriteCallback, __CARDWrite, DVDOpenDir, expLoadDesc), and 0x801A3D98 (was Melee's HSD_SListAllocAndAppend; ours inlines HSD_SListAlloc so the call graph no longer matches; the wowjinxy map calls it HSD_SListAppend). **Same result every run, fresh or not** (Oct 28): the first version read names from the previous run's Ghidra export, so reruns drifted (181 -> 182 candidates) and a fixpoint attempt cycled. Now the matchers take our names only from the committed symbol files and the export only for address-level structure (`tools/export_struct.py`), assert strings come from main.dol's instruction bytes (`tools/dol_asserts.py`: 443 asserts, the 411 the C showed + 32 its regex missed), and `run_all.ps1` starts from the committed names and repeats names + rebuild until nothing changes (1 rebuild with an export present, 2 fresh; fails after `-MaxPasses` 5). `rebuild_decomp.ps1` refuses to start on a Ghidra lock, deletes the old export first, and throws unless both exports are complete; all scripts now carry `#Requires -Version 7` (under Windows PowerShell 5.1 the Ghidra call returned early and the name batches read a half-written export, which looked like a 2-cycle: 184 <-> 179 candidates with the 17 R4 names lost). Proof: a run from no export converged in 2 rebuilds and matched the previous run in all 18 checksums (`tools/checksums.ps1`, now also covering the candidate files, lib_candidates_report and names_crosscheck). Final: 184 lib candidates (R3 95, R4 17, R5 64, R6 8), 391 library functions unnamed (hsd 295, sdk 92, rt 4). Next: the 391 library leftovers (most have no asserts/strings and unnamed callers - lead: build dolsdk2001/2004 objects and match code hashes with objdiff), then the game's own code (step 48).

48. **Game-code naming, stage 1: the game's own names, harvested by script** (Oct 28, 2026; user rule: no incorrect names, invent nothing, reproducible). New `tools/match_game_names.py`, run by new `tools/batches/batch7_game_names.ps1` (in `run_all.ps1`'s name pass after batch 6, so inside the names <-> rebuild fixpoint). It reads only `main.dol`, the dtk analysis and our committed hand names - never the Ghidra export, index.db or the C: code references come from constant propagation over the instruction bytes (dol_asserts' PowerPC model; switch-case blocks start from the entry state) - so the output is the same from the first pass, fresh or not. Evidence kinds: **G1** parallel tables - a name table (maximal run of string pointers, all distinct) directly adjacent to a function table of exactly the same length, proven indexed together (one function references both bases, or the name table has no code reference and a function referencing the function table compares the index with that length); name = `cr_` + the string as written (step 44's convention), only for handlers that occur once in the table; {string, function} struct arrays were also scanned (none exist). **G2** self-naming messages `Name(): ...`: checked first against already-named functions - 10 of 14 agree, 4 do not (`DVDReadAsync():` inside DVDReadAsyncPrio, `DVDRead():` inside DVDReadPrio, `DVDChangeDisk():` inside stateReadingFST and cbForStateCheckID2: the message names the public API, not the function printing it), so G2 emits nothing while any disagreement exists; the weak form `Name: ...` is only reported (it gives shortened names: `DoReadRegisters :` in TRKDoReadRegisters). **G3** `.c` file names passed to __assert / HSD_Panic / OSPanic: 219 module notes (8 in game code: seq_movie.c 2 = the SEQ_MOVIE handlers, background.c 2, sprite.c 4), never used as names; 104 header (`.h`) strings are library inlines, counted only. Result: **G1 300 names** - exactly the script commands of step 44 (0x8021BF44 names + 0x8021C3F4 handlers, 300 each, adjacent; 118 dispatchers compare the command with 300, e.g. `cmpwi r0, 300` at 0x8009D740 next to `lis r3, 0x8022; addi r0, r3, -0x3C0C`), now regenerated by the script: the 300 hand lines were taken over (same names, same addresses, tagged `// game-real G1 names:0xT[i]@0xS funcs:0xF[i] "STR"`, block after a marker line in `symbols/descriptive.txt`); G2 0, conflicts 0, no new function names (1,856 of 3,565). No other name table in main.dol has an adjacent function table of the same length. Why descriptive.txt and not a new file: these are cr_ game names, ApplySymbols / lint / ParseHeaders / the cross-check already read it, and the tagged regenerated lines follow batch 6's `lib-candidate` pattern; cr_globals.txt untouched (the game names no tables). The batch stops, writing nothing, if a name would replace a different existing name or reuse a name at another address (tested). `tools/crosscheck_names.py` (batch 8) now checks every game-real tag against main.dol (string still at the cited address, both table words at the index, name = cr_ + string, tables still adjacent runs of the same length; a corrupted index was tested to FAIL). Leftovers with reasons: `symbols/game_unnamed_real.txt` (46: SEQ_* 15 names next to 18 / 60 code pointers, CAMMODE_* no adjacent table, MAIN_SCRIPT.. 4 names after the 300 handlers, the file-ID / sound / motion tables have no function tables, the G2 disagreements, weak-form `HSD_ArchiveParse:` at unnamed 0x801A5708 - a library lead). The SEQ / CAMMODE handler names (steps 42, 45) stay hand lines: their role suffixes are ours. Outputs added to `tools/checksums.ps1` (descriptive.txt, game_real.txt, game_unnamed_real.txt). Proof: two consecutive full runs identical in all 21 checksums; cross-check 2,083 names, FAIL 0. Next: stage 2 (descriptive `cr_` names for game code, only where the game gives none), starting from the G3 modules and the script-command handlers' callees.

49. **Phase 4 Milestone 1: main.dol builds from dtk's split objects and matches retail** (Oct 28, 2026). New `decomp/` (a decomp-toolkit project from `encounter/dtk-template` at commit d3668764, pinned in `tools/pins.txt`). Only `decomp/config/GXCE01/` (config.yml, symbols.txt, splits.txt, build.sha1) and `decomp/reports/` are in git; the template files, the DOL, the split objects and the build are gitignored (`decomp/build/`, `decomp/orig/`, `decomp/tools/`, `decomp/configure.py`, `tools/ninja/`). New `tools/setup_decomp.ps1` (also setup.ps1 step 8): clones the template without line-ending conversion, copies its `configure.py` + `tools/*.py` into `decomp/`, applies `tools/patches/dtk_template_configure.patch` (version GXCE01; every tool from the pinned downloads, `configure.py` stops instead of downloading anything unpinned; the object list is read from splits.txt, all NonMatching), downloads ninja 1.13.2, objdiff-cli 3.6.1, gc-wii-binutils 2.42-2, sjiswrap 1.2.2, wibo 1.0.3 (Linux / macOS) and the compilers zip `compilers_20251118` (SHA-256 each), and copies your `main.dol` (SHA-1 checked) to `decomp/orig/GXCE01/sys/`. dtk is our existing v1.8.4 pin (the template asks for 1.8.3). Linker: GC/1.3.2 (the template default; the compiler itself is Milestone 3). **Names**: new `tools/gen_dtk_symbols.py` takes dtk's own analysis (`tools/dtk/proj/symbols.txt`) and only renames existing dtk symbols to our cross-checked names (symbols.txt, descriptive.txt, cr_globals.txt) at the same address and kind: 1,567 renamed, 2,068 non-default names in all; 15 not applied and kept as dtk's default (`decomp/reports/symbols_report.txt`): 1 conflict (0x801B6600: `__destroy_global_chain_reference` vs `_dtors`, both ours, one dtk object) and 14 cr_g_ globals that sit inside a larger dtk object (cr_g_SeqNames/SeqEnter/SeqLeave, CamModeNames, ScrCmdNames, ArchiveDvdEntry, ReadQueueHead/Tail, ReadChunkSectors, CurrentRead, ReadQueue, SeqPrevious/Next, CamPtrs). No invalid names (CodeWarrior's own `name$123` local-static form is accepted) and no collisions. **Finding**: mwld (GC/1.3.2 and 2.6) hangs, not fails, linking an object whose first ELF symbol is a label sitting in front of the function at the same address (0x80137F98 `__OSSystemCallVectorStart` / `SystemCallVector` as soon as OSSync.c starts there); the generator writes labels after the sized symbol at one address (dtk keeps that order). **Splits**: new `tools/gen_dtk_splits.py`, method in its docstring: for every object of the 13 pinned references with a dtk config (Melee, TWW, Pikmin 2, MKDD, SMS, TP, KAR map, XD, AC, MP4, Pikmin, BfBB, TTYD), our range is from the first to the last of our functions that the reference puts in that file (names used only if unique in ours and in exactly one reference file); every named function in between must belong to it, and both ends must be proven (the neighbour is a named function the reference puts in another file, or unnamed while our end function is the reference's own first / last, or a section edge); function counts within 25%; every reference that proves an object must give the same range, else it is dropped; overlaps dropped. Paths: Melee's where Melee has the file, else the reference's file name in Melee's folder layout. Result: **143 library objects, 859 functions** (dolphin 57: os 18, gx 12, card 8, dvd 6, ar/mtx 2, ai/ax/base/db/dsp/exi/si/vi/OdemuExi2 1; MSL 47; MetroTRK 26; Runtime 5; HSD 8: tobj, state, mobj, util, fog, object, bytecode, texp), code sections only. Rejected with reasons in `decomp/reports/splits_report.txt` (1,285 candidate lines), notably 9 files where the references disagree because of SDK revisions (MSL printf.c, __start.c, dsp_task.c, mtx44.c, odenotstub.c, OSError.c, OSTime.c, TRK main.c, mslsupp.c) and exi.c vs EXIBios.c. **Unproven, left in dtk's automatic objects** (34 code ranges, 2,673 functions): the game code 0x800055E0-0x80131C08 (1,751 functions, one object: the 8 G3 notes in game code - seq_movie.c, background.c, sprite.c - never sit next to another file's note, and there is no alignment padding between any two game functions), most of HSD (0x8016D324-0x80186B44 189, 0x8018CC40-0x801A039C 237, 0x801A0568-0x801A33E4 44, ... : object edges sit on unnamed functions) and SDK gaps (e.g. 0x8013B13C-0x8013E464 62 dvd functions, 0x80144760-0x801464C0 43, 0x80147A48-0x8014A67C 35, 0x8014A6CC-0x8014D494 36), .init 0x80003100-0x80003400 (__start.c, references disagree); all data sections. New `tools/batches/batch9_decomp_build.ps1` (last in `run_all.ps1`, outside the name fixpoint): setup_decomp -> both generators -> `python configure.py` + ninja (dtk validates every split, and normalises both files once: `align:` / `data:` attributes, a few sizes, link order; names and ranges unchanged) -> a forced second split must change nothing -> SHA-1. **Proof**: `decomp/build/GXCE01/main.dol` SHA-1 `6a73cd111ef349b20be294511b8c51a8addfb50b` = retail, with the sections-only splits and with all 143 objects; two consecutive batch 9 runs gave identical config files and reports, now in `tools/checksums.ps1` (26 files, all OK). Next: the data sections per object (dtk's relocation targets per object), HSD edges (KAR's real map order + sizes), then Milestone 2.

50. **Phase 4 Milestone 3: compiler versions and flags per library** (Oct 28, 2026). New `tools/compiler_probe/probe.py` (+ hand-written C for 13 small game functions in `tools/compiler_probe/game/`), run by new `tools/batches/batch10_compiler_probe.ps1` (about 5 minutes, ~50,000 compiles; not in run_all). It compiles reference C (dolsdk2004 29 files, Melee sysdolphin 40, MSL 54 from MP4 + Melee, Runtime 9, MetroTRK 42 from MP4 + Melee) with all 19 pinned GC compilers and 40+ flag sets per library, and compares every function also named in our symbols.txt with main.dol's bytes (relocated fields masked; exact = same size, all words equal). Stage 2 gives every version near the best the same flag sets, so versions are compared fairly. **Result** (`docs/compiler.md`, with evidence and a ready-to-use flag table for configure.py): Dolphin SDK **GC/1.2.5n** `-O4,p -inline auto -fp_contract off -char unsigned`, EXIBios.c `-O3,p` (336 of 406 exact, 103 only with 1.2.5n); HSD **GC/1.3.2** `-O4,p -inline auto -fp_contract on -str reuse` (174 of 370; 1.3 fails 5 of them, drawShapeAnim rules out 2.x; HSD audio assumed the same, 1 of 16 exact); MSL **GC/2.5-2.7** `-O4,p -inline auto,deferred -use_lmw_stmw on -str pool,readonly -common off` (70 of 107, 5 only 2.5+); Runtime and MetroTRK 1.3.2-2.7 (TRK `-sdata 0 -sdata2 0 -inline auto,deferred`, `-enum int`); **game code: optimization level 0 with peephole + scheduling, speed, `-fp_contract on`**, 13 of 13 hand-written functions exact with every compiler 1.3-2.7 (each local in its own saved register even in leaves, no constant folding, but scheduled). GC/2.6 is the working choice for MSL / Runtime / TRK / game (the CodeWarrior release MSL proves); the never-exact functions are source-revision differences, left to Milestone 4. Melee's `src/MetroTRK/` added to its sparse checkout (setup.ps1; MSL's math.h includes it). configure.py unchanged (nothing compiles C yet). Next: Milestone 2 / 4, and a version-sensitive game function to narrow 1.3-2.7.

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
3. **Layout proof (R3)**: between two verified anchors that appear in the same order in a reference, the unnamed functions align to the reference by size (within 10%). Since Oct 28, 2026 (step 47) a name is only emitted when its alignment is unique, or call graph / strings confirm it against every shifted alternative (`tools/layout_evidence.py`); equal-size runs (e.g. 0x20 math wrappers) are never named by layout alone. Names from the wowjinxy Kirby Air Ride map count only when another reference has the same name (it invents some).
4. **Assert sequence (R4)**: same assert source file and expression(s) as exactly one reference function, in an order consistent with the reference file; line numbers may differ.
5. **Call graph (R5)**: a unique match of the function's verified callees and callers (at least 2 distinct verified neighbours, common helpers like `__assert`/`OSReport`/`memcpy` not counted) to one reference function in the same reference unit; re-checked against the final name set.
6. **Unique string (R6)**: references a string literal that one reference function uses alone, or a game string that names the function.
Sanity for R4-R6: size within 0.5..2.0 of the reference, name unique in the reference, never C++ mangled, never a reference's own placeholder (`fn_`, `lbl_`, `kar_..._<addr>`).
**Cross-check (every run)**: `tools/crosscheck_names.py` (batch8) re-derives evidence for every name in symbols.txt, the candidates, descriptive.txt and cr_globals.txt, independent of the rule that produced it, and stops the run on any contradiction (FAIL). Report: `symbols/names_crosscheck.txt`, `docs/names_crosscheck.md`.
Before rejecting a name as "not in Melee", search all of Melee (config + src): it may be `static`/`inline` and folded into its caller (found for `FObjLoadWait` and `IDEntryAlloc`; the caller `HSD_IDInsertToTable` was verified and applied instead).
Everything else (descriptive guesses) lives in `symbols\unverified.txt` and is never applied. Earlier invented names (`File_ReadEntry`, `Viewer_LoadModel`, `Game_Panic`, …) were removed; `Game_Panic` turned out to be `OSPanic`.

Current state (Oct 28, 2026, step 48): 1,393 library function names applied from symbols.txt (dtk signatures, 46 Melee-assert, 862 layout-proof, 184 R3-R6 lib candidates merged by batch6) + 432 in descriptive.txt (300 of them generated by batch7 from the game's own tables, tagged game-real G1); 1,856 of 3,565 functions named; cross-check: 2,083 names, 0 FAIL, 1,631 OK, 436 WEAK (no contradiction, only the producing evidence), 16 UNCHECKABLE (prose `why:`). Historic count (Oct 25): 348 names.
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
- [x] Symbols file under version control (public repo since Oct 27, 2026).

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

### Phase 2b. New content: new robos, robo types, animations, sound (user request, Oct 28, 2026)

**Goal.** A full new robo, made from new assets, playable in battle: first by replacing an existing robo, then in a new slot of its own, and finally as a new robo type with its own behaviour.

**Two routes, used together**
- **Files** (`make_mod.py` -> `.crpatch`): models, skeletons, textures, text, sound banks, anything that is a disc file.
- **Code** (Gecko / Action Replay, and permanent DOL patches): table changes, new code, behaviour. The Custom Robo Discord says codes can add textures and other things but not models; research confirms it (`docs/community_codes.md`). The whole Dolphin code list gets about 6 KB (0x80001800-0x80003000); one robo entry is 186-233 KB and models live in the heap.

**Path, in dependency order**
1. **Models** (Phase 1): replacing works offline (484/484 byte-identical round trip; boots). [ ] Confirm in battle. [ ] Limits: file size, memory, vertex / bone counts.
2. **Skeletons**: a new bone tree works through the 19 role bones (`*.roles.json`, docs/MODDING.md). [ ] Confirm in battle. [ ] Robo size (lives in the vertices; root scale is overridden at run time, step 29).
3. **Animations**: 1,095 motion IDs and the 34 battle motion sets x 38 slots are named (step 46). [ ] Map the motion data format (`snr_motion.bin`, `motion.DBN`). [ ] Exporter + importer like crmodel.py. [ ] Blender retarget workflow for new skeletons. [ ] What "charge not linked" is (step 28).
4. **Sound effects and music**: `.ssm` banks and `.hps` streams, the same HAL formats as Melee (check Melee's tools first). HSD audio code is named (step 47). [ ] Read/write tools. [ ] Map SFX IDs -> bank, robo and action.
5. **Text, icons, stats**: `.bmg` names/descriptions, `.tpl` icons/portraits (590 of 915 disc files are `.tpl`), part and robo stat tables (Phase 2).
6. **New robo slot** (instead of replacing one): every table indexed by robo number has to grow. Known: 5 entries per robo in `chara.BIN` + main.dol's copy of the entry table, `cr_g_CharaMotionSets`, `cr_g_CharaYOffset`, `cr_g_BattleMotionSets`, stats, text, icons, unlock flags, robo-count limits. [ ] Find them all (game-code naming, stage 2). [ ] Move grown tables into new space (below).
7. **New robo type** (new behaviour: movement, air dash, charge, ...): needs the behaviour code understood (stage 2) and new code (below).

**Codes (community request, Oct 28, 2026).** Public lists: 15 AR codes (cheathappens, decrypted: HP x4, 1-hit x4, parts owned x5, unlock flags) + Dolphin's 16:9 Gecko code. All 13 addresses map into our decomp (docs/community_codes.md, docs/community/codes_gxce01.tsv). Use them as naming evidence leads, not names.
- [ ] `tools/ardecrypt.py`: AR (encrypted or plain) -> Gecko lines, so community codes can be dropped into a mod (own implementation; Dolphin's ARDecrypt.cpp is GPL).
- [ ] `make_mod.py`: `code/*.gecko` -> Dolphin `GXCE01.ini` + `.gct`. Writes that land inside main.dol become static patches with `[was]` checks; bss/heap targets stay Gecko-only.
- [ ] `make_mod.py`: `code/*.s` -> C2 code (devkitPPC `powerpc-eabi-as`, pinned; see JLaferri's `gecko`, Melee Code Manager) and, permanently, a body in free space + a `b` hook.
- [ ] Free space for new code / data: a new DOL text section at 0x80001800-0x80003000 first (main.dol uses 2 of 7 text and 6 of 11 data sections); then dead code (MetroTRK, proven with `find.py callers`); check how `OSInit` sets ArenaLo before a section after bss.
- [ ] Name leads from the codes: player battle record (x4, stride 0x33520, HP float `DAT_8024e194`) used by `FUN_8004ede4`; parts-owned bitsets `803BFB9C..` (5 x 4 x 64 bits) set up by `FUN_800a3c0c`; bitset helpers `FUN_80023228` (set, checked) / `FUN_800231ec` (clear); unlock flags `DAT_80321d44`; camera aspect `FLOAT_8044c438` (1.26); 2D ortho setup `FUN_80091518`.
- [ ] Ask the Discord for its code lists (more evidence and ready-made table addresses; the open questions are in docs/community_codes.md).

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
- [x] New `decomp/` project from `encounter/dtk-template` (configure.py + ninja + objdiff), pinned like everything else. The build needs the user's own ISO (dtk extracts main.dol); no game data in git. (Step 49: `tools/setup_decomp.ps1`, template commit + every tool in `tools/pins.txt`.)
- [x] Every object "NonMatching": the build links dtk's split objects -> retail SHA-1 `6a73cd11…fb50b` (step 49, `decomp/reports/build_report.txt`). This proves the toolchain, linker script and section layout.
- [ ] Splits: object boundaries in `splits.txt`, from the library layouts (Melee / SDK object lists in their configure.py), the G3 `.c` module notes, alignment padding, and the rodata / data / bss order per object. dtk validates each split.
  - [x] First set (step 49): 143 library objects, code sections only (dolphin 57, MSL 47, MetroTRK 26, Runtime 5, HSD 8), every one proven by the reference layouts; SHA-1 still matches.
  - [ ] The rest of the library code (34 unproven code ranges, 922 functions outside the game range: mostly HSD, whose object edges sit on unnamed functions, and 9 SDK / MSL / TRK files where the references disagree).
  - [ ] Data sections per object (.data / .rodata / .bss / .sdata / .sdata2 / .sbss, .ctors / .dtors): all still in dtk's automatic objects.
  - [ ] Game code (0x800055E0-0x80131C08, 1,751 functions): one unproven range. G3 notes prove no boundary yet and there is no alignment padding between any two game functions.

**Milestone 2. A shiftable build (the modding unlock, before anything matches)**
- [ ] Every pointer in code and data becomes a symbol (dtk relocation analysis; check jump tables, pointer tables and the script / motion / camera tables), so code and data can move.
- [ ] Test: insert a dummy function and a padded table, relink, boot, play a battle.
- Once this works, a new robo slot (grown tables) and new code are ordinary source edits, with no Gecko or free-space hacks. Phase 2b steps 6-7 can then use this instead of patches.

**Milestone 3. Compiler and flags (days to weeks)**
- [x] Find the CodeWarrior version and flags (step 50, `docs/compiler.md`, batch 10): SDK GC/1.2.5n `-O4,p -inline auto -fp_contract off` (EXIBios `-O3,p`); HSD GC/1.3.2 `-O4,p -inline auto -fp_contract on`; MSL GC/2.5-2.7 `-O4,p -inline auto,deferred -use_lmw_stmw on`; Runtime / TRK 1.3.2-2.7 (TRK `-sdata 0 -sdata2 0`); game code `-opt level=0,peephole,schedule,speed -fp_contract on`, 1.3-2.7 (GC/2.6 chosen, the MSL release).
- [x] Pin the compilers in tools/pins.txt and add them to setup.ps1 (step 49: `compilers_20251118.zip`, setup step 8).
- [ ] Narrow the game / Runtime / TRK compiler inside 1.3-2.7 (and MSL inside 2.5-2.7): the first matched function with a version-sensitive construct decides it.

**Milestone 4. Libraries to matching C (months)**
- [ ] Bring in the SDK, MSL and TRK sources from the pinned decomps; adapt them to our SDK revision; objdiff each object.
- [ ] HSD from Melee / KAR, adapted to Custom Robo's revision (struct sizes from check_class_sizes.py, the differing functions from the R2/R4 size checks).
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
