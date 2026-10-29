# Reproduce everything from a fresh ISO (and check nothing is hidden)

This page explains every step from a clean disc image to the named, decompiled, searchable game, **what each
step runs, what it reads and writes, and whether it touches the network**. The point is that you don't have
to trust us: every script is short, plain text and in this repo, every download is pinned to an exact version,
and the outputs are **byte-for-byte reproducible**. At the end you compare SHA-256 checksums with ours.

## 0. What you need

- Your own **Custom Robo (USA)** disc image, GameCube `GXCE01` revision 1.0
  (SHA-1 `fff6e74590366a5c0c5ba0d40c8ccf94770bcb16`, MD5 `546e5108173bceb4d5c8ca27b3fcfc96`, matching GameTDB).
- Windows 10/11, **PowerShell 7** (`pwsh`; every script has `#Requires -Version 7`, so the blue Windows PowerShell 5.1 refuses them. Under 5.1 the Ghidra call returned before Ghidra finished), **git**, **Python 3.11+**, **Java 21+** (Ghidra), **.NET SDK 8 or newer** (HSDRawViewer).
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

Everything below runs locally, with no network access. The name batches and the Ghidra rebuild run **to a fixpoint**
(about 4 minutes per rebuild + name pass; 1 rebuild when a previous export exists, 2 on a fresh setup):

1. **Start from the committed names**: every `// layout-proof` / `// lib-candidate` line is stripped from `symbols\symbols.txt`
   and `symbols\candidates_{sdk,rt,hsd}.txt` are emptied, so the result never depends on an earlier run.
2. **Names, pass 0**: batch 1, batch 6 and the lint, against the Ghidra export of the previous run if there is one
   (a fresh setup has none: only forced layout pairs, no lib candidates).
3. **Rebuild, then names again** against the fresh export; repeat while any name file changed (`symbols.txt`,
   `layout_candidates.txt`, `candidates_*.txt`, `verify_report.txt`, `lib_candidates_report.txt`). It stops when a rebuild
   leaves every name unchanged, so the decompiled C is exactly the C of the final names, and fails after 5 rebuilds
   (`-MaxPasses`). The summary prints how many rebuilds it took.
4. Batches 5, 2, 3, 4 and 8 on the final export.

The name matchers take **our names only from the symbol files** and the Ghidra export **only for structure**: function
starts, calls, string references and computed jumps, all by address (`tools\export_struct.py`; never a name or type Ghidra
applied, never `decompiled\index.db`, never the decompiled C). Assert calls (file / expression strings) come from
`main.dol`'s instruction bytes (`tools\dol_asserts.py`). `tools\rebuild_decomp.ps1` deletes the old export first and stops
unless Ghidra finished both exports completely, so a half-written export can never be read. That structure is the same whatever names were applied, so a fresh setup and a
rerun reach the same names, byte for byte. Batch 1 reads only the committed names (never batch 6's output or its own
earlier output), and batch 6 builds on batch 1, so one name pass on a given export is already final.

| Step | Script(s) | What it does | Reads → Writes |
|---|---|---|---|
| Batch 1 | `tools\layout_match.py`, `tools\verify_symbols.py` | Proposes real library names by **layout proof**: between two already-known functions, in the same order as in a reference decomp, size within 10%, both neighbours matching. Repeats until nothing new. Every name is re-checked; the batch **stops** if any fails | `tools\dtk\proj\symbols.txt`, reference `symbols.txt` files → `symbols\layout_candidates.txt`, `symbols\symbols.txt`, `symbols\verify_report.txt` |
| Batch 6 | `tools\match_lib_sdk.py`, `match_lib_rt.py`, `match_lib_hsd.py`, `tools\merge_lib_candidates.py`, `tools\verify_symbols.py` | Names the remaining SDK / runtime / HSD library functions by layout, asserts, call graph and unique strings and merges them tagged `lib-candidate`, repeating until nothing new; the batch **stops** if any merged name fails verification | Ghidra export (structure only), reference decomps → `symbols\candidates_*.txt`, `symbols\lib_candidates_report.txt`, `symbols\symbols.txt` |
| Lint | `tools\lint_descriptive.py` | Checks our own descriptive names: `cr_` prefix, a `why:` note, never on an address with a real name | `symbols\descriptive.txt` |
| Rebuild | `tools\rebuild_decomp.ps1` → Ghidra headless with the scripts in `tools\scripts\` (all plain Java, under 100 lines each): | | `main.dol` → `ghidra_project\`, `decompiled\` |
| | `EnableGCAnalyzer.java` | turns on the GameCube loader's own small-data (r2/r13) analyzer, which is off by default due to a name check bug in the loader | |
| | `CreateDtkFunctions.java` | creates functions at the starts dtk found and Ghidra missed | |
| | `ApplySymbols.java` (×2) | applies `symbols\symbols.txt`, then `symbols\descriptive.txt` | |
| | `DefineStrings.java` | marks ASCII strings in data sections so code shows `s_chara_bin_…` | |
| | `ParseHeaders.java` | parses the HAL library headers from the Melee decomp (a declarations-only copy made by `tools\clean_headers.py`) and our `symbols\cr_types.h`; checks every `ASSERT_SIZE` and `/* +XX */` offset; applies prototypes to verified library functions and our `cr_` functions; types the globals in `symbols\cr_globals.txt` | → `symbols\types_report.txt` |
| | `ExportXrefs.java` | writes functions / references / strings as TSV | → `decompiled\xrefs\` |
| | `ExportDecompiledC.java` | writes the C of every function | → `decompiled\main_dol.c` |
| Fixpoint | `tools\batches\run_all.ps1` | batch 1, batch 6 and the lint again on the fresh export; if any name file changed, back to Rebuild | export → name files |
| Batch 5 | `tools\batches\batch5_types.ps1`, `tools\check_class_sizes.py` | stops if any header size/offset, prototype or global failed, or if a parsed HSD struct size differs from the size Custom Robo itself registers for that class | `symbols\types_report.txt` |
| Batch 2 | `tools\crosscheck_dtk.py` | compares Ghidra with dtk (function starts, names, small-data) | → `symbols\crosscheck_report.txt` |
| Batch 3 | `tools\game_map.py` | callers / callees / strings / subsystem hints per function | → `symbols\game_map.tsv`, `docs\game_map.md` |
| Batch 4 | `tools\build_index.py` | SQLite search index + one file per function | → `decompiled\index.db`, `decompiled\funcs\` |
| Batch 8 | `tools\crosscheck_names.py` | independent cross-check of every name against all references; **stops** on any FAIL | `decompiled\index.db`, name files → `symbols\names_crosscheck.txt`, `docs\names_crosscheck.md` |

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
