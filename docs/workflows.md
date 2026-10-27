# Workflows

Run everything from `D:\emulators\gameTesting`.

## 1. Look at a model

- Quickest: open `models_export\<category>\<name>\*.glb` in Blender (File → Import → glTF 2.0). Textures sit next to it as `.png`. A few models only have `.dae` (Collada) because the glTF writer rejected a NaN value.
- To inspect the HSD structure: run `tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe`, File → Open → a file in `models_dat\<category>\`, select `root0_joint` (robo / weapon / story character) or `scene_data` (stages, effects, objects).

Robo IDs: `chara` entry = robo_id × 5 + variant (34 robos × 5 entries; the 5 entries of a robo have the same size — what the variants are is **not verified**).

## 2. Regenerate everything from the disc

```
DolphinTool.exe extract -i "Custom Robo (USA).iso" -o "Custom Robo extracted"     # (already done)
python -I tools\sfd.py unpack "Custom Robo extracted\files" unpacked
python -I tools\sfd.py verify "Custom Robo extracted\files" unpacked              # expect 22 x OK
python -I tools\extract_models.py unpacked "Custom Robo extracted\files" models_dat
python -I tools\export_models.py models_dat models_export
python -I tools\crmodel.py roundtrip unpacked\chara unpacked\weapon_gun unpacked\weapon_bom unpacked\weapon_pod unpacked\weapon_leg unpacked\scenario_us\snr_chara unpacked\scenario_jp\snr_chara   # expect 484/484
```

## 3. Edit a robo / weapon / story character and put it back

1. Get the .dat: `models_dat\chara\chara_000.dat` (or `python -I tools\crmodel.py to-dat unpacked\chara\000.bin chara_000.dat`).
2. Edit it (HSDRawViewer, or a script). Keep the root named `cr_header2`. Keep the same skeleton: the entry's joint path list (block C) is copied from the original and is only valid for the original joint tree.
3. Rebuild the entry with the original as template:
   `python -I tools\crmodel.py from-dat chara_000_edited.dat unpacked\chara\000.bin new_000.bin`
4. Put it into a copy of the entry folder and repack:
   copy `unpacked\chara` to a work folder, replace `000.bin`, then `python -I tools\sfd.py pack <work folder> <build>\files\chara.BIN`.
5. Boot the build folder (see 4).

`tools\make_test_build.py` does steps 1–4 automatically for a test edit (red/blue texture swap + 0x100 data shift).

## 4. Test in Dolphin

- Build folder = a full copy of `Custom Robo extracted` with changed files (e.g. `test_build\`).
- Boot: open `<build>\sys\main.dol` in Dolphin (Dolphin treats a `sys\` + `files\` folder as a disc), or
  `"D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Dolphin.exe" -b -e "D:\emulators\gameTesting\test_build\sys\main.dol"`.
- Current `test_build\`: robo 0 (all 5 entries) has red and blue swapped in its textures. It boots; the in-game look has **not been checked yet** — play to a screen that shows robo 0 (its texture sheet is the one with the pilot's face).

## 5. Rebuild the decompile

```
pwsh tools\rebuild_decomp.ps1
```
Fresh import of `main.dol` into `ghidra_project\CustomRobo`, GameCube analyzer on, names from `symbols\symbols.txt`, strings defined, C written to `decompiled\main_dol.c`, log in `decompiled\ghidra_log.txt`. About 2 minutes.

## 6. Add or change a function name

1. Find evidence first (see the naming policy in [decomp.md](decomp.md)). No evidence → write it in `symbols\unverified.txt` instead.
2. Add a line to `symbols\symbols.txt`: `Name = .text:0x8XXXXXXX; // type:function scope:global // <evidence>`.
3. Run `verify_symbols.py` (see [tools.md](tools.md)); a non-dtk name must not show MISMATCH or UNSOURCED.
4. `pwsh tools\rebuild_decomp.ps1`.
