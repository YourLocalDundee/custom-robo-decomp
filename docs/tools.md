# Tools

All in `D:\emulators\gameTesting\tools`. Python tools run with the system Python 3.13 in isolated mode (`python -I`); `render_preview.py` needs `tools\pyenv` (trimesh, matplotlib). Run commands from `D:\emulators\gameTesting`.

## Our tools

| Tool | Purpose | Usage |
|---|---|---|
| `sfd.py` | SFD archives: unpack / pack / byte-identical verify ([format](formats/sfd.md)) | `python -I tools\sfd.py unpack "Custom Robo extracted\files" unpacked`<br>`python -I tools\sfd.py pack <entry_dir> <out.BIN>`<br>`python -I tools\sfd.py verify "Custom Robo extracted\files" unpacked` |
| `crmodel.py` | Model entry ↔ standard .dat ([format](formats/model_entry.md)) | `python -I tools\crmodel.py to-dat <entry.bin> <out.dat>`<br>`python -I tools\crmodel.py from-dat <in.dat> <template_entry.bin> <out_entry.bin>`<br>`python -I tools\crmodel.py roundtrip unpacked\chara unpacked\weapon_gun …` |
| `extract_models.py` | Every known model → `models_dat\<category>\*.dat` | `python -I tools\extract_models.py unpacked "Custom Robo extracted\files" models_dat` |
| `export_models.py` | `models_dat\` → `models_export\<category>\<name>\` via HSDRawViewer (`.glb`, `.dae` fallback); writes `export_log.txt` | `python -I tools\export_models.py models_dat models_export` |
| `make_test_build.py` | Copies the extracted disc to a build folder and rebuilds chosen `chara` entries through the full import path (red/blue texture swap + 0x100 data shift) | `python -I tools\make_test_build.py "Custom Robo extracted" unpacked test_build 0 1 2 3 4` |
| `boot_test.ps1` | Boots a build folder in the Project Hive Dolphin, captures the window, closes Dolphin. Capture currently comes out black. | `pwsh tools\boot_test.ps1 -Build test_build -Out shot.png -Seconds 40` |
| `render_preview.py` | Front + side flat-shaded PNG of a `.glb` (sanity check) | `tools\pyenv\Scripts\python tools\render_preview.py <model.glb> <out.png>` |
| `cr_model_explore.py` | Prints a model entry's header, reloc stats and pointer graph (research) | `python -I tools\cr_model_explore.py unpacked\chara\000.bin 5` |
| `rebuild_decomp.ps1` | Fresh Ghidra import + GameCube analyzer + names + strings + C export ([decomp](decomp.md)) | `pwsh tools\rebuild_decomp.ps1` |
| `match_hsd_asserts.py` | Matches `__assert` (file, expression) calls against the Melee decomp to propose HSD function names | `python -I tools\match_hsd_asserts.py decompiled\main_dol.c "Custom Robo extracted\sys\main.dol" tools\melee\src\sysdolphin symbols\melee_matches.txt` |
| `verify_symbols.py` | Checks every name in `symbols\symbols.txt` against Melee's verified symbols (existence + size) | `python -I tools\verify_symbols.py symbols\symbols.txt tools\dtk\proj\symbols.txt tools\melee\config\GALE01\symbols.txt > symbols\verify_report.txt` |

## Ghidra scripts (`tools\scripts`)

| Script | Role |
|---|---|
| `EnableGCAnalyzer.java` | pre-script: turns on the GameCube loader's "(GameCube/Wii) Program Analyzer" (sets r13/r2), which stays off by default because its format-name check no longer matches the loader |
| `ApplySymbols.java` | applies names from a dtk-style symbols file (skips generic `fn_`/`lbl_` names) |
| `DefineStrings.java` | defines null-terminated ASCII strings (≥ 4 chars) in non-executable memory |
| `ExportDecompiledC.java` | decompiles every function into one C file |

## Third-party tools (installed under `tools\`)

| Tool | Version | Notes |
|---|---|---|
| Ghidra | 12.1.4 (`ghidra_12.1.4_PUBLIC`) | needs Java 21 (installed system-wide) |
| Ghidra GameCube Loader | 1.3.1 (built for 12.1) | `extension.properties` version patched 12.1 → 12.1.4. Headless needs `-loader-autoloadMaps false` or it hangs on a hidden dialog. |
| HSDLib / HSDRawViewer | built from source (`tools\HSDLib`, base commit in `tools\patches\hsdlib_base_commit.txt`) | targets .NET 8; runtimeconfig set to `rollForward: Major` to run on the installed .NET 10. Our patch (`tools\patches\hsdlib_headless_export_resave.patch`) adds `--export <out_dir> <glb|dae|smd> <files…>` and `--resave <in.dat> <out.dat>`. After every rebuild the runtimeconfig roll-forward line must be re-added. |
| decomp-toolkit (dtk) | 1.8.4 (`tools\dtk\dtk.exe`) | analysis project in `tools\dtk\proj` (`symbols.txt`, `splits.txt`) |
| Melee decomp | sparse clone (`tools\melee`) | only `src\sysdolphin`, `src\dolphin`, `config` checked out |
| Python venv | `tools\pyenv` | trimesh, matplotlib, numpy, pygltflib, pillow |

## Rebuilding HSDRawViewer

```
cd tools\HSDLib
dotnet build HSDRawViewer\HSDRawViewer.csproj -c Release
# then add  "rollForward": "Major",  under "runtimeOptions" in
# HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.runtimeconfig.json
```
On a fresh clone: `git apply ..\patches\hsdlib_headless_export_resave.patch` first.
