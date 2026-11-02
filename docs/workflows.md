# Workflows

Step by step for the common jobs. Run every command from the repo root (the folder you cloned); each is given for
Windows (PowerShell 7) and, in sections 1-7, for Linux / macOS (bash), as explained in the [docs index](README.md#commands-in-these-docs).
The Linux / macOS commands are not maintained at the moment; sections 8-14 (the development loops) are Windows only.
First do the one-time run of the [README's quick start](../README.md#quick-start): it makes `Custom Robo extracted/`,
`unpacked/`, `models_dat/`, `models_export/` (Windows) and `decompiled/`, all local and never committed.

## 1. Look at a model

- Quickest: open `models_export/<category>/<name>/*.glb` in Blender (File → Import → glTF 2.0). Textures sit next to
  it as `.png`. A few models only have a `.dae` (Collada), because the glTF writer rejected a NaN value. The glTF
  export runs on Windows only (HSDRawViewer); on Linux / macOS use the `.dat` files below.
- To inspect the HSD structure (Windows): run
  `tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe`, File → Open → a file in
  `models_dat\<category>\`, select `root0_joint` (robo / weapon / story character) or `scene_data` (stages, effects,
  objects).

Robo numbers: `chara` entry = robo number × 5 + variant (34 robos × 5 entries; what the 5 variants are is **not
verified**). Details: [MODDING.md](MODDING.md#robo-numbers-vs-entry-numbers).

## 2. Extract the models again

The one-time run already did this. To redo a step (after changing a tool, say), the commands are in
[REPRODUCE.md](REPRODUCE.md), section 4, in order. The model round trip covers all 484 entries when given every entry
folder: `unpacked/chara`, `unpacked/weapon_gun`, `weapon_bom`, `weapon_pod`, `weapon_leg`,
`unpacked/scenario_us/snr_chara` and `unpacked/scenario_jp/snr_chara` (expect 484/484).

## 3. Edit a robo / weapon / story character and put it back

For a mod you want to play or share, use [MODDING.md](MODDING.md): `tools/make_mod.py` does all of this and checks
it. By hand:

1. Get the `.dat`: `models_dat/chara/chara_000.dat`, or convert the entry yourself (below).
2. Edit it (HSDRawViewer, or a script). Keep the root named `cr_header2`. Keep the same skeleton: the entry's joint
   path list (block C) is copied from the original and is only valid for the original joint tree (other skeletons:
   [MODDING.md](MODDING.md#a-robo-with-a-different-skeleton-more--fewer--other-bones)).
3. Rebuild the entry with the original as template, put it into a copy of the entry folder and repack the archive
   into a build folder (a copy of `Custom Robo extracted` with the changed files):

Windows:

```powershell
python -I tools\crmodel.py to-dat unpacked\chara\000.bin chara_000.dat                    # step 1, if needed
python -I tools\crmodel.py from-dat chara_000_edited.dat unpacked\chara\000.bin new_000.bin
Copy-Item -Recurse unpacked\chara work_chara; Copy-Item new_000.bin work_chara\000.bin
python -I tools\sfd.py pack work_chara test_build\files\chara.BIN
```

Linux / macOS:

```bash
python3 -I tools/crmodel.py to-dat unpacked/chara/000.bin chara_000.dat                    # step 1, if needed
python3 -I tools/crmodel.py from-dat chara_000_edited.dat unpacked/chara/000.bin new_000.bin
cp -r unpacked/chara work_chara && cp new_000.bin work_chara/000.bin
python3 -I tools/sfd.py pack work_chara test_build/files/chara.BIN
```

4. Boot the build folder (section 4).

`tools/make_test_build.py` does steps 1–3 for a test edit (red / blue texture swap + a 0x100 data shift):
`tools/make_test_build.py "Custom Robo extracted" unpacked test_build 0 1 2 3 4`. If an entry changes size, the copy
of the archive's entry table inside `main.dol` must change too ([formats/model_entry.md](formats/model_entry.md#the-game-keeps-its-own-copy-of-each-archives-entry-table));
`make_mod.py` does that, the steps above do not.

## 4. Test in Dolphin

A build folder (`sys/` + `files/`, like `Custom Robo extracted`) boots like a disc: open its `sys/main.dol` in
Dolphin (File → Open), or from the command line:

Windows:

```powershell
& "C:\path\to\Dolphin-x64\Dolphin.exe" -b -e "test_build\sys\main.dol"     # or the pinned tools\dolphin\Dolphin-x64
pwsh tools\boot_check.ps1 test_build\sys\main.dol 40 -Screenshot shot.png      # boots, reports warning dialogs
```

Linux / macOS:

```bash
dolphin-emu -b -e test_build/sys/main.dol      # Flatpak: flatpak run org.DolphinEmu.dolphin-emu -b -e ...
# macOS: /Applications/Dolphin.app/Contents/MacOS/Dolphin -b -e test_build/sys/main.dol
```

`tools/boot_check.ps1` (and the older `boot_test.ps1`) read Dolphin's windows through the Win32 API, so they are
Windows-only. A mod built by `make_mod.py` is a normal ISO: open it in Dolphin.

## 5. Read and search the code

`decompiled/main_dol.c` is the whole game as C (Ghidra's output), `decompiled/funcs/` the same one file per function,
and `tools/find.py` searches the index `decompiled/index.db` (built by batch 4).

Windows:

```powershell
python -I tools\find.py text chara_bin                       # full-text search over all C
python -I tools\find.py func cr_Archive_OpenEntry --brief    # one function (name or address), compact
python -I tools\find.py callers cr_CharaEntry_OnLoaded       # direct calls + places that take its address
python -I tools\find.py callees 800747b0                     # what it calls
python -I tools\find.py refs 801F1200                        # every instruction touching an address
python -I tools\find.py str snr_motion                       # strings + the functions using them
python -I tools\find.py hint model                           # game functions that reach 3D-model code
```

Linux / macOS:

```bash
python3 -I tools/find.py text chara_bin                      # full-text search over all C
python3 -I tools/find.py func cr_Archive_OpenEntry --brief   # one function (name or address), compact
python3 -I tools/find.py callers cr_CharaEntry_OnLoaded      # direct calls + places that take its address
python3 -I tools/find.py callees 800747b0                    # what it calls
python3 -I tools/find.py refs 801F1200                       # every instruction touching an address
python3 -I tools/find.py str snr_motion                      # strings + the functions using them
python3 -I tools/find.py hint model                          # game functions that reach 3D-model code
```

Hints (`model`, `anim`, `file-io`, ...) and the most-used unnamed functions: [game_map.md](game_map.md). What the
names mean (verified, descriptive `cr_`, `FUN_`): [README](../README.md#what-the-names-mean).

To browse and click through, open the Ghidra project: start Ghidra (`tools\ghidra_12.1.4_PUBLIC\ghidraRun.bat` on
Windows, `tools/ghidra_12.1.4_PUBLIC/ghidraRun` on Linux / macOS), File → Open Project →
`ghidra_project/CustomRobo.gpr`. Renaming there changes nothing in the repo: names come from the symbol files.

## 6. Rebuild the decompile

Normally the full run does this, together with the name batches (`pwsh tools\batches\run_all.ps1` /
`tools/batches/run_all.sh`). Only the Ghidra step, with the names as they are now:

Windows:

```powershell
pwsh tools\rebuild_decomp.ps1
```

Linux / macOS:

```bash
tools/rebuild_decomp.sh
```

Fresh import of `main.dol` into `ghidra_project/CustomRobo`, GameCube analyzer on, names from `symbols/symbols.txt`
and `symbols/descriptive.txt`, strings, types, then the C (`decompiled/main_dol.c`, log `decompiled/ghidra_log.txt`)
and the cross-references. Every script it runs: [decomp.md](decomp.md#pipeline-toolsrebuild_decompps1).

## 7. Add or change a function name

[CONTRIBUTING.md](../CONTRIBUTING.md) has the rules and the steps (real names first, then a descriptive `cr_` name
with its evidence, lint, rerun).

## 8. The decomp loop: match a library file (Phase 4)

One object of `decomp/config/GXCE01/splits.txt` goes from dtk's asm to C that links. Rules and the reference to take:
[CONTRIBUTING.md](../CONTRIBUTING.md#matching-a-library-file-phase-4), [decomp.md](decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026).

1. Pick the object: `CR_NOT_MATCHING` in `decomp/configure.py` (its reason says what blocks it), or a split object
   without a C file. Today only HSD objects are left (6 of 30: lobj, pobj, texp, displayfunc, jobj, texpdag;
   the reason per file is in `CR_NOT_MATCHING`).
2. Import the reference C with its provenance line (never Ghidra's C, never a draft):
   ```powershell
   python -I tools\import_ref_source.py <ref> <path in ref> src/<object path> --inc <ref include dir>=include/<ref>   # the HSD / SDK examples: decomp.md
   ```
3. Build and compare, as often as needed (seconds per round):
   ```powershell
   cd decomp; python configure.py; ..\tools\ninja\ninja.exe; cd ..
   decomp\build\tools\objdiff-cli.exe diff -p decomp -u main/sysdolphin/baselib/lobj -o -   # the unit is the object's path in decomp\objdiff.json, without .c (checked with the pinned 3.6.1: lobj.c is 'Unit not found'); -o - prints the JSON, without -o the interactive view opens; or the objdiff GUI on decomp\, or decomp\build\GXCE01\report.json
   python -I tools\check_c_objects.py --link-each                                        # links the candidate too, prints the SHA-1
   ```
   Adapt the C until every function is 100%: `#pragma` per function, Melee's / KAR's forms, the notes in decomp.md
   "HSD (sysdolphin) notes". A name the link needs that our symbols leave as `fn_` / `lbl_`: a `// c-callsite` line in
   `symbols/symbols.txt` from `decomp/reports/c_callsites_report.txt`; a global the file defines: a `// c-define` line
   from the `DEF` lines of `splits_report.txt`.
4. Move the object into `CR_MATCHING`, then `python -I tools\make_configure_patch.py` (the patch is what is committed),
   then `pwsh tools\batches\batch9_decomp_build.ps1`: the object must be 100% code and data, `main.dol` retail, the
   shift test PASS. Commit the C, the patch, `symbols/`, `decomp/config` and `decomp/reports`.
5. Merge through a worktree (section 14); the main checkout runs the checks and writes the checksums.

## 9. The draft loop: game functions (batch 12)

The game code has no reference: `tools/draft_batch.py` writes an m2c draft of every game function, compiles it and
compares; the 100% ones are promoted into `decomp/src/game` by `tools/promote_drafts.py`. The rule: **never edit a
draft**; every fix goes into the generator (a rule or a variant in `draft_batch.py`), a type in `symbols/cr_types.h` or
a global in `symbols/cr_globals.txt`, so the next full run reproduces it ([drafts.md](drafts.md)).

1. Pick targets: `decomp/drafts/draft_report.txt` (local; the 80-99% drafts and the compile failures, with the note of
   the best variant), or the file-run table in drafts.md "Step 83" (the blockers of the file objects).
2. Look at one: `dtk elf disasm` / the side-by-side of the draft's object against dtk's asm
   (`decomp\build\GXCE01\asm\...`), then write the fix as a generator rule (with its gate: only the shapes it is
   proven on).
3. Rerun the targets in seconds, then every draft the pattern occurs in:
   ```powershell
   python -I tools\draft_batch.py --only fn_8006EEF8 fn_80111E0C          # reuses decomp\drafts\ctx_protos.h / ctx_sdk.h
   python -I tools\draft_batch.py --sample 200 --out build\drafts_try      # an experiment apart from the last full run (copy the two ctx_*.h files there first)
   ```
   A rule must not lose a match elsewhere: compare the sample's `draft_report.txt` with the last full run's.
4. The full run, then promotion and batch 9: `pwsh tools\batches\batch12_drafts.ps1` (about 26 minutes). The counts
   go into `decomp/reports/draft_summary.txt` and `promote_report.txt`; `decomp/src/game` gets the new files.
5. Docs: a new generator round is a section in drafts.md (what changed, before -> after), the counts in README /
   decomp.md, a plan.md step. Commit the generator, the type files, `decomp/src/game`, the reports, the docs.

## 10. The naming loop (rounds 3 and 4: typed structs for the game's state)

Names and types for the game's own code and data, with evidence ([CONTRIBUTING.md](../CONTRIBUTING.md)):

1. Leads: `python -I tools\name_evidence.py` writes `docs/name_evidence.md` (name tables, identifier strings, the
   hand-kept "Leads" list) and `docs/name_tables.tsv`; `docs/game_map.md` lists the most-used unnamed functions.
2. Read the code: `python -I tools\find.py func FUN_80012345 --brief`, `callers`, `callees`, `refs <address>`; for a
   global, the base-register scan of the split asm (every `lis`/`addi` of the object and the loads / stores through
   that register: offset, width, sign, function) gives the struct's fields.
3. Write it down: a struct in `symbols/cr_types.h` (`/* +XX */` on every field, an `ASSERT_SIZE` comment, `unk_XX`
   for the rest), the global in `symbols/cr_globals.txt` (`cr_g_Name = .bss:0xADDR; // type:object datatype:Type // why: ...`),
   functions in `symbols/descriptive.txt` (`cr_Subsystem_Action = .text:0x...; // type:function scope:global // why: ...`).
   `python -I tools\lint_descriptive.py` checks the lines.
4. Check what the names do to the drafts before the full run: `python -I tools\draft_batch.py --only <the functions
   that use the global>`; a typed `lbl_` helps every draft that touches it, a wrong width costs matches.
5. Rebuild and check: in the main checkout `pwsh tools\batches\run_all.ps1` (the Ghidra export gets the names, batch 8
   cross-checks every one, batch 9 renames the promoted files through `tools/sync_game_names.py`, batch 12 rewrites
   them); or, on a branch, batch 9 + batch 12 + batch 8 and let the main checkout do the Ghidra rebuild after the merge.
6. The names' leads that were not proven stay in name_evidence.md "Leads" (the tool's `LEADS` list) with the reason.

## 11. The Blender model loop (robos)

Export a robo, edit or replace it with a Blender script, import it back, build the ISO and the `.crpatch`, boot it:
one command ([MODDING.md, "Blender loop"](MODDING.md#blender-loop) has every option, the import rules and the limits).

```powershell
pwsh tools\model_loop.ps1 -Mod rt_test -Script roundtrip -Entries 000                                     # no edit: proves the loop (the ISO keeps the clean SHA-1)
pwsh tools\model_loop.ps1 -Mod ray_legionnaire -Script legionnaire_v2 -Entries 000,001,002,003,004 -Boot   # a complete replacement, all 5 colour entries of robo 0
blender -b --factory-startup --python tools\blender\render.py -- build\model_loop\ray_legionnaire\chara_000.edited.glb preview.png   # look at it without the game
```

1. Write the script in `tools/blender/` from `legionnaire_v2.py` (complete model: new bones, roles by bone name,
   baked textures, smooth skinning) or `legionnaire.py` (parts added to the existing skeleton); `common.py` holds the
   import / export settings. A script may write `<out>.args` with the import options it needs (`--replace`,
   `--skeleton glb`, `--roles ...`).
2. Run the loop for one entry first (`-Entries 000`), look at the re-export (`render.py`) and the import's warnings;
   then all five entries of the body (robo N = entries 5N..5N+4). A body entry must stay under 0x3A000 bytes; the game
   draws a body with one material and one texture atlas (MODDING.md "How the game associates textures with meshes").
3. `-Boot` (40 s in Dolphin, no warning dialog) proves it loads; what it looks like needs a person at the Customize
   screen and in a battle. The ISO is `build/mods/<mod>/<mod>.iso`, the patch `mods/<mod>/<mod>.crpatch`; only
   `mods/<mod>/mod.json` and `notes.txt` are committed.

## 12. The stage loop (arenas)

The same loop over a `bg_data` entry: stage id s = entry 2s (the `scene_data` model file) and 2s+1 (the collision).
[MODDING.md, "Stages"](MODDING.md#stages), [stage_tables.md](stage_tables.md).

```powershell
pwsh tools\model_loop.ps1 -Mod rt_stage -Script roundtrip -Category bg_data -Entries 000,008,026        # no edit: every scene model through Blender, byte-identical back
pwsh tools\model_loop.ps1 -Mod basic_arena_pillar -Script stage_pillar -Category bg_data -Entries 000 -Boot   # the first edited arena
tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe --info models_dat\bg_data\bg_data_000.dat    # the scene: models, joints, materials
```

1. The export writes one glb per scene model (`scene_data_<n>.glb`); the script runs once per model (`-Models 0,3`
   limits it) and the imports are chained with `--model <n>`, written back in the template's own file layout.
2. New geometry on one of the stage's own materials shares it (texture, render mode, vertex colours); the model
   entry must stay under 0x100000 bytes, the collision under 0x8000; at most 20 models per scene, 21 DObjs on a root
   joint. The collision (entry 2s+1) is not edited by the loop yet: new geometry has no collision.
3. `make_mod.py` puts the `.dat` into `bg_data.BIN` as it is (no entry table in `main.dol` for this archive); `-Boot`
   as for robos.

## 13. The mod build loop

A mod is a folder with only what changed ([MODDING.md](MODDING.md)): `mods/<name>/mod.json`, `models/<category>/<same
name as in models_dat>.dat`, `files/<disc path>`, `code/*.txt`. Build, test, share:

```powershell
python -I tools\make_mod.py mods\my_mod                       # -> build\mods\my_mod\my_mod.iso + mods\my_mod\my_mod.crpatch (clean ISO: the repo root's, or a 2nd argument)
pwsh tools\boot_check.ps1 build\mods\my_mod\my_mod.iso 40 -Screenshot shot.png -Log boot.log   # boots it, reports a warning dialog, saves the picture and Dolphin's log
python -I tools\iso_patch.py info mods\my_mod\my_mod.crpatch                                   # what the patch holds
python -I tools\iso_patch.py apply "Custom Robo (USA).iso" mods\my_mod\my_mod.crpatch out.iso # what a player does (checks both SHA-1s)
python -I tools\robo_swap.py 0 7                                                              # a whole-robo swap as a mod folder + build, in one go
```

`make_mod.py` rebuilds a model entry through `crmodel.from_dat` with the original as template, syncs `main.dol`'s copy
of the archive's entry table when sizes changed, checks the body (0x3A000) and stage (0x100000 / 0x8000) load-buffer
limits, patches `main.dol` from the `code/*.txt` lines (each `[was ...]` checked against the clean DOL) and writes the
ISO over the clean one. Code changes that need room beyond a patch are the Phase 4 route (MODDING.md "Source-route
modding"), not a `.crpatch`.

## 14. Work in a worktree and merge it back

Every work stream (a library file, a generator round, a naming round, a model) runs on its own branch in a second
worktree with junctions into the main checkout for the big gitignored folders; the recipe and its rules:
[REPRODUCE.md, "A second checkout (git worktree)"](REPRODUCE.md#a-second-checkout-git-worktree). On the branch: the
work, batch 9 (and batch 12 for drafts, batch 8 for names), the docs of the step, plain commits
([CONTRIBUTING.md](../CONTRIBUTING.md#rules-for-everything)). Then, **in the main checkout**:

```powershell
git merge --no-ff <branch>                     # or a fast-forward; resolve plan.md / docs by keeping both sides' steps
pwsh tools\batches\batch9_decomp_build.ps1     # the merged sources and names: main.dol retail, every C object 100%, relocations, static shift PASS
pwsh tools\batches\batch8_crosscheck.ps1       # every name re-checked (the main checkout's index.db; fine when no name changed)
pwsh tools\checksums.ps1 -Compare              # expected: only the decomp\ reports and config differ, by the merge's own numbers
```

- When the branch **promoted new game files** and main **renamed symbols** since the branch started, batch 9's own
  rename pass misses them (it compares with the previous run, not with the branch), and the link fails on the old name
  (Nov 2, 2026: `undefined: 'lbl_8044F2A0'` from the branch's `main.c` after main named it `cr_g_FrameCount`). Rename
  against the branch's symbols first, then run batch 9:
  `git show <branch>:decomp/config/GXCE01/symbols.txt > decomp/build/symbols_branch.txt`, then
  `python -I tools/sync_game_names.py decomp/build/symbols_branch.txt`.
- When the merge **changed names or types** (`symbols/*.txt`, `cr_types.h`, `cr_globals.txt`): the Ghidra export must
  follow: `pwsh tools\rebuild_decomp.ps1`, then batches 4 and 8 (or the whole `run_all.ps1`, 36 minutes, which
  also reruns batch 12 and renames the promoted files). Until then `decompiled\*` and `symbols\types_report.txt`
  differ in the compare and `decompiled\main_dol.c` shows the old names.
- When the merge **changed `decomp/src/game` or the generator**: `pwsh tools\batches\batch12_drafts.ps1` once in the
  main checkout, so `draft_summary.txt`, `promote_report.txt` and the promoted files are the main build's (26 minutes).
- When all checks pass and the differences are the intended ones: `pwsh tools\checksums.ps1 -Write`, then one plain
  commit with the regenerated reports, `docs\expected_checksums.txt`, and the plan.md "Resume here" bullet of the step
  (what the numbers are now, what is left). Then remove the junctions, `git worktree remove ..\<worktree>` and delete
  the branch ([REPRODUCE.md](REPRODUCE.md#a-second-checkout-git-worktree), the removal lines).
- Two worktrees that both change `decomp/configure.py`: regenerate the patch after the merge
  (`python -I tools\make_configure_patch.py` on the merged `decomp/configure.py`; `setup_decomp.ps1` must then apply it
  without a reject).
