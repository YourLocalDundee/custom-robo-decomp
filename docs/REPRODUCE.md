# Reproduce everything from a fresh ISO (and check nothing is hidden)

This page explains every step from a clean disc image to the named, decompiled, searchable game, **what each
step runs, what it reads and writes, and whether it touches the network**. The point is that you don't have
to trust us: every script is short, plain text and in this repo, every download is pinned to an exact version,
and the outputs are **byte-for-byte reproducible**. At the end you compare SHA-256 checksums with ours.

## 0. What you need

- Your own **Custom Robo (USA)** disc image, GameCube `GXCE01` revision 1.0
  (SHA-1 `fff6e74590366a5c0c5ba0d40c8ccf94770bcb16`, MD5 `546e5108173bceb4d5c8ca27b3fcfc96`, matching GameTDB).
- Windows 10/11, **PowerShell 7** (`pwsh`), **git**, **Python 3.11+**, **Java 21+** (Ghidra), **.NET SDK 8 or newer** (HSDRawViewer).
  Everything else is downloaded and hash-checked by the scripts. Dolphin is optional (the official build is fetched for its `DolphinTool`).

## The one command

```powershell
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
pwsh tools\from_scratch.ps1 -Iso "D:\Games\Custom Robo (USA).iso"
```

`tools\from_scratch.ps1` runs sections 1–4 below in order, stops at the first failure, and ends with a
PASS / FAIL table (about 10–15 minutes the first time, mostly downloads). Tested from an empty folder on
Oct 27, 2026: **PASS**. The sections explain what each part does, so you can also run them one by one.

## 1. Setup — `tools\setup.ps1`

```powershell
pwsh tools\setup.ps1 -Iso "D:\Games\Custom Robo (USA).iso"            # -Dolphin "C:\Dolphin" to use your own
```

Downloads are kept in `tools\downloads\` and **every one is SHA-256 checked against `tools\pins.txt`** on every run.

| Step | What happens | Network | Writes |
|---|---|---|---|
| 1 | SHA-1 of your ISO checked against the retail dump; stops if different | no | — |
| 2 | `DolphinTool.exe extract` (Dolphin's own tool) unpacks the disc. Without `-Dolphin`, the official Dolphin 2609a build is downloaded (hash-checked) and unpacked with Windows' `tar` | dl.dolphin-emu.org | `tools\dolphin\`, `Custom Robo extracted\` |
| 3 | Sparse git clones of public decomps at the commits in `tools\pins.txt`, **only** their `config/*/symbols.txt` + `splits.txt` (Melee also `src/dolphin`, `src/sysdolphin` for assert matching). These are lists of names, addresses and sizes | GitHub | `tools\melee\`, `tools\refs\` |
| 4 | HSDLib cloned at the pinned commit, our patch `tools\patches\hsdlib_headless_export_resave.patch` applied (read it: it adds a command-line `--export` / `--resave`), built with `dotnet build` | GitHub, NuGet | `tools\HSDLib\` |
| 5 | decomp-toolkit `dtk.exe` downloaded and **its SHA-256 checked** against `tools\pins.txt`; `dtk dol split config.yml out` analyses your `main.dol` (function boundaries, SDK signature names) | GitHub | `tools\dtk\` |
| 6 | Python venv with the exact versions in `tools\requirements-preview.txt` (only used to render preview PNGs) | PyPI | `tools\pyenv\` |
| 7 | Ghidra 12.1.4 (SHA-256 = the one Ghidra publishes on its release page) and the GameCube loader 1.3.1 (hash-checked), unzipped into `Ghidra\Extensions`; the loader's `version=` line is set to 12.1.4 because the zip is built for 12.1 | GitHub | `tools\ghidra_12.1.4_PUBLIC\` |

Nothing in setup sends anything anywhere; downloads are the only network use.

## 2. Decompile and name — `tools\batches\run_all.ps1`

```powershell
pwsh tools\batches\run_all.ps1
```

Everything below runs locally, with no network access, in this order (~2 minutes):

| Step | Script(s) | What it does | Reads → Writes |
|---|---|---|---|
| Batch 1 | `tools\layout_match.py`, `tools\verify_symbols.py` | Proposes real library names by **layout proof**: between two already-known functions, in the same order as in a reference decomp, size within 10%, both neighbours matching. Repeats until nothing new. Every name is re-checked; the batch **stops** if any fails | `tools\dtk\proj\symbols.txt`, reference `symbols.txt` files → `symbols\layout_candidates.txt`, `symbols\symbols.txt`, `symbols\verify_report.txt` |
| Lint | `tools\lint_descriptive.py` | Checks our own descriptive names: `cr_` prefix, a `why:` note, never on an address with a real name | `symbols\descriptive.txt` |
| Rebuild | `tools\rebuild_decomp.ps1` → Ghidra headless with the scripts in `tools\scripts\` (all plain Java, under 100 lines each): | | `main.dol` → `ghidra_project\`, `decompiled\` |
| | `EnableGCAnalyzer.java` | turns on the GameCube loader's own small-data (r2/r13) analyzer, which is off by default due to a name check bug in the loader | |
| | `CreateDtkFunctions.java` | creates functions at the starts dtk found and Ghidra missed | |
| | `ApplySymbols.java` (×2) | applies `symbols\symbols.txt`, then `symbols\descriptive.txt` | |
| | `DefineStrings.java` | marks ASCII strings in data sections so code shows `s_chara_bin_…` | |
| | `ExportXrefs.java` | writes functions / references / strings as TSV | → `decompiled\xrefs\` |
| | `ExportDecompiledC.java` | writes the C of every function | → `decompiled\main_dol.c` |
| Batch 2 | `tools\crosscheck_dtk.py` | compares Ghidra with dtk (function starts, names, small-data) | → `symbols\crosscheck_report.txt` |
| Batch 3 | `tools\game_map.py` | callers / callees / strings / subsystem hints per function | → `symbols\game_map.tsv`, `docs\game_map.md` |
| Batch 4 | `tools\build_index.py` | SQLite search index + one file per function | → `decompiled\index.db`, `decompiled\funcs\` |

## 3. Check you got the same result — `tools\checksums.ps1`

```powershell
pwsh tools\checksums.ps1 -Compare
```

Compares SHA-256 of your `main.dol`, the dtk analysis, every symbols/report file and the decompiled C with
[`docs/expected_checksums.txt`](expected_checksums.txt). We rebuilt twice from scratch and got identical hashes
for all of them. **All OK = you have exactly what we have**, produced only by the scripts above.

### If a checksum differs

- `main.dol` differs → not the same disc; nothing after it can match.
- dtk files differ → a different dtk build (check its SHA-256).
- `symbols\*` differ but dtk matches → the reference commits differ from `tools\pins.txt`, or files were edited.
- only `decompiled\*` differs → a different Ghidra or GameCube-loader version, or a different Java.

## 4. Models — same tools we used, in order (`from_scratch.ps1` runs the unpack, verify and round-trip lines)

```powershell
python -I tools\sfd.py unpack "Custom Robo extracted\files" unpacked     # all 22 SFD .bin archives -> entries
python -I tools\sfd.py verify "Custom Robo extracted\files" unpacked     # repack in memory: must be byte-identical
python -I tools\extract_models.py unpacked "Custom Robo extracted\files" models_dat   # 1,006 models as HSD .dat
python -I tools\crmodel.py roundtrip unpacked\chara unpacked\weapon_gun  # entry -> .dat -> entry, byte-identical
python -I tools\export_models.py models_dat models_export                # glTF/.dae via patched HSDRawViewer
```

Formats: [formats/sfd.md](formats/sfd.md), [formats/model_entry.md](formats/model_entry.md), [formats/containers.md](formats/containers.md).

## 5. Auditing the code yourself

- **Everything that executes** is in `tools\` (`*.ps1`, `*.py`), `tools\scripts\*.java` and `tools\patches\*.patch`.
  There is no compiled code of ours and nothing is obfuscated.
- Python scripts use only the standard library (except `render_preview.py`: trimesh/matplotlib) and are run with
  `python -I` (isolated mode: ignores environment variables and the user's site-packages).
- The only network access is in `tools\setup.ps1`, to the URLs listed in `tools\pins.txt`.
- Quick self-check for anything unexpected:
  ```powershell
  Select-String -Path tools\*.py,tools\*.ps1,tools\batches\*.ps1,tools\scripts\*.java `
      -Pattern 'http|Invoke-WebRequest|urllib|socket|requests|subprocess|Start-Process|eval|exec\('
  ```
  Expected hits, and only these: `setup.ps1` (the dtk download), `export_models.py` (runs the locally built
  HSDRawViewer) and `boot_test.ps1` (starts your local Dolphin).
- Mods are published as patches against the clean ISO, never as modified game files, so you can always diff them.
