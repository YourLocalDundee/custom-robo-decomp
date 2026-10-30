# Workflows

Step by step for the common jobs. Run every command from the repo root (the folder you cloned); each is given for
Windows (PowerShell 7) and for Linux / macOS (bash), as explained in the [docs index](README.md#commands-in-these-docs).
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
