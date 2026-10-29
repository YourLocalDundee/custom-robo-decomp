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
| 3 | Sparse git clones of public decomps at the commits in `tools\pins.txt`, **only** their `config/*/symbols.txt` + `splits.txt` (Melee also `src/sysdolphin`, `src/MSL`, `src/Runtime`, `src/MetroTRK` and the SDK headers: assert matching, and the compiler probe of batch 10). These are lists of names, addresses and sizes | GitHub | `tools\melee\`, `tools\refs\` |
| 4 | HSDLib cloned at the pinned commit, our patch `tools\patches\hsdlib_headless_export_resave.patch` applied (read it: it adds a command-line `--export` / `--resave`), built with `dotnet build` | GitHub, NuGet | `tools\HSDLib\` |
| 5 | decomp-toolkit `dtk.exe` downloaded and **its SHA-256 checked** against `tools\pins.txt`; `dtk dol split config.yml out` analyses your `main.dol` (function boundaries, SDK signature names) | GitHub | `tools\dtk\` |
| 6 | Python venv with the exact versions in `tools\requirements-preview.txt` (only used to render preview PNGs) | PyPI | `tools\pyenv\` |
| 7 | Ghidra 12.1.4 (SHA-256 = the one Ghidra publishes on its release page) and the GameCube loader 1.3.1 (hash-checked), unzipped into `Ghidra\Extensions`; the loader's `version=` line is set to 12.1.4 because the zip is built for 12.1 | GitHub | `tools\ghidra_12.1.4_PUBLIC\` |
| 8 | `tools\setup_decomp.ps1` (Phase 4 build, see section 2b): `encounter/dtk-template` cloned at the pinned commit into `tools\refs\dtk-template\` (no line-ending conversion); its `configure.py` and `tools\*.py` copied into `decomp\` and our patch `tools\patches\dtk_template_configure.patch` applied (read it: version GXCE01, pinned tool paths, object list from `splits.txt`); ninja 1.13.2, objdiff-cli 3.6.1, gc-wii-binutils 2.42-2, sjiswrap 1.2.2 (wibo 1.0.3 on Linux / macOS) and the CodeWarrior compilers zip `compilers_20251118` downloaded and SHA-256 checked; your `main.dol` (SHA-1 checked) copied to `decomp\orig\GXCE01\sys\` | GitHub, files.decomp.dev | `tools\ninja\`, `decomp\build\{tools,binutils,compilers}\`, `decomp\orig\`, `decomp\configure.py`, `decomp\tools\` |

Nothing in setup sends anything anywhere; downloads are the only network use.

## 2. Decompile and name — `tools\batches\run_all.ps1`

```powershell
pwsh tools\batches\run_all.ps1
```

Everything below runs locally, with no network access. The name batches and the Ghidra rebuild run **to a fixpoint**
(about 4 minutes per rebuild + name pass; 1 rebuild when a previous export exists, 2 on a fresh setup):

1. **Start from the committed names**: every `// layout-proof` / `// lib-candidate` line is stripped from `symbols\symbols.txt`
   and `symbols\candidates_{sdk,rt,hsd}.txt` are emptied, so the result never depends on an earlier run (batch 7 replaces
   its own `// game-real` lines in `symbols\descriptive.txt` itself).
2. **Names, pass 0**: batch 1, batch 6, batch 7 and the lint, against the Ghidra export of the previous run if there is one
   (a fresh setup has none: only forced layout pairs, no lib candidates).
3. **Rebuild, then names again** against the fresh export; repeat while any name file changed (`symbols.txt`,
   `layout_candidates.txt`, `candidates_*.txt`, `verify_report.txt`, `lib_candidates_report.txt`, `descriptive.txt`,
   `game_real.txt`, `game_unnamed_real.txt`). It stops when a rebuild
   leaves every name unchanged, so the decompiled C is exactly the C of the final names, and fails after 5 rebuilds
   (`-MaxPasses`). The summary prints how many rebuilds it took.
4. Batches 5, 2, 3, 4 and 8 on the final export, then batch 9 (the Phase 4 build, section 2b; it reads only the final name files),
   batch 10 (compiler probe, about 5 minutes), batch 11 (the shifted build booted in Dolphin, section 2c, about 1 minute; don't
   type while Dolphin is open), and finally `tools\checksums.ps1 -Compare` (section 3). **This one script runs every batch in
   order** and stops at the first failure. `tools\from_scratch.ps1` runs setup, then this script, then the archive / model / mod steps.

The name matchers take **our names only from the symbol files** and the Ghidra export **only for structure**: function
starts, calls, string references and computed jumps, all by address (`tools\export_struct.py`; never a name or type Ghidra
applied, never `decompiled\index.db`, never the decompiled C). Assert calls (file / expression strings) come from
`main.dol`'s instruction bytes (`tools\dol_asserts.py`). `tools\rebuild_decomp.ps1` deletes the old export first and stops
unless Ghidra finished both exports completely, so a half-written export can never be read. That structure is the same whatever names were applied, so a fresh setup and a
rerun reach the same names, byte for byte. Batch 1 reads only the committed names (never batch 6's output or its own
earlier output), and batch 6 builds on batch 1, so one name pass on a given export is already final. Batch 7 reads only
`main.dol` (code references by constant propagation over the instruction bytes), the dtk analysis and the committed hand
names, never the export, so its output is the same from the very first pass.

| Step | Script(s) | What it does | Reads → Writes |
|---|---|---|---|
| Batch 1 | `tools\layout_match.py`, `tools\verify_symbols.py` | Proposes real library names by **layout proof**: between two already-known functions, in the same order as in a reference decomp, size within 10%, both neighbours matching. Repeats until nothing new. Every name is re-checked; the batch **stops** if any fails | `tools\dtk\proj\symbols.txt`, reference `symbols.txt` files → `symbols\layout_candidates.txt`, `symbols\symbols.txt`, `symbols\verify_report.txt` |
| Batch 6 | `tools\match_lib_sdk.py`, `match_lib_rt.py`, `match_lib_hsd.py`, `tools\merge_lib_candidates.py`, `tools\verify_symbols.py` | Names the remaining SDK / runtime / HSD library functions by layout, asserts, call graph and unique strings and merges them tagged `lib-candidate`, repeating until nothing new; the batch **stops** if any merged name fails verification | Ghidra export (structure only), reference decomps → `symbols\candidates_*.txt`, `symbols\lib_candidates_report.txt`, `symbols\symbols.txt` |
| Batch 7 | `tools\match_game_names.py` | Game code, stage 1: the game's **own** names, nothing invented. G1: a name table (string pointers) directly next to a function table of the same length, indexed together (one function references both, or the dispatcher bounds the index by that length) -> `cr_` + the string as written; G2: messages of the form `Name(): ...` inside the function (checked against already-named functions first; emits nothing while that check disagrees); G3: `.c` source-file names from asserts, as module notes only. Merged into `symbols\descriptive.txt` tagged `game-real`; **stops** if a name would replace a different existing one | `main.dol`, dtk analysis, committed names → `symbols\game_real.txt`, `symbols\game_unnamed_real.txt` (leftovers), `symbols\descriptive.txt` |
| Lint | `tools\lint_descriptive.py` | Checks our own descriptive names: `cr_` prefix, a `why:` note, never on an address with a real name | `symbols\descriptive.txt` |
| Rebuild | `tools\rebuild_decomp.ps1` → Ghidra headless with the scripts in `tools\scripts\` (all plain Java, under 100 lines each): | | `main.dol` → `ghidra_project\`, `decompiled\` |
| | `EnableGCAnalyzer.java` | turns on the GameCube loader's own small-data (r2/r13) analyzer, which is off by default due to a name check bug in the loader | |
| | `CreateDtkFunctions.java` | creates functions at the starts dtk found and Ghidra missed | |
| | `ApplySymbols.java` (×2) | applies `symbols\symbols.txt`, then `symbols\descriptive.txt` | |
| | `DefineStrings.java` | marks ASCII strings in data sections so code shows `s_chara_bin_…` | |
| | `ParseHeaders.java` | parses the HAL library headers from the Melee decomp (a declarations-only copy made by `tools\clean_headers.py`) and our `symbols\cr_types.h`; checks every `ASSERT_SIZE` and `/* +XX */` offset; applies prototypes to verified library functions and our `cr_` functions; types the globals in `symbols\cr_globals.txt` | → `symbols\types_report.txt` |
| | `ExportXrefs.java` | writes functions / references / strings as TSV | → `decompiled\xrefs\` |
| | `ExportDecompiledC.java` | writes the C of every function | → `decompiled\main_dol.c` |
| Fixpoint | `tools\batches\run_all.ps1` | batch 1, batch 6, batch 7 and the lint again on the fresh export; if any name file changed, back to Rebuild | export → name files |
| Batch 5 | `tools\batches\batch5_types.ps1`, `tools\check_class_sizes.py` | stops if any header size/offset, prototype or global failed, or if a parsed HSD struct size differs from the size Custom Robo itself registers for that class | `symbols\types_report.txt` |
| Batch 2 | `tools\crosscheck_dtk.py` | compares Ghidra with dtk (function starts, names, small-data) | → `symbols\crosscheck_report.txt` |
| Batch 3 | `tools\game_map.py` | callers / callees / strings / subsystem hints per function | → `symbols\game_map.tsv`, `docs\game_map.md` |
| Batch 4 | `tools\build_index.py` | SQLite search index + one file per function | → `decompiled\index.db`, `decompiled\funcs\` |
| Batch 8 | `tools\crosscheck_names.py` | independent cross-check of every name against all references (game-real names: the string still at the cited address, the table indexes still matching, the tables still adjacent and of the same length); **stops** on any FAIL | `decompiled\index.db`, name files → `symbols\names_crosscheck.txt`, `docs\names_crosscheck.md` |
| Batch 9 | `tools\batches\batch9_decomp_build.ps1` | the Phase 4 build, section 2b; **stops** unless the built `main.dol` is the retail one | name files, dtk analysis, reference layouts → `decomp\config\GXCE01\{symbols,splits}.txt`, `decomp\reports\` |
| Batch 10 | `tools\batches\batch10_compiler_probe.ps1` → `tools\compiler_probe\probe.py` | Phase 4 Milestone 3: compiles reference C (dolsdk2004, Melee sysdolphin / MSL / Runtime / MetroTRK, Mario Party 4 MSL / Runtime / TRK) and our hand-written C for small game functions (`tools\compiler_probe\game\`) with all 19 pinned GC compilers × 40+ flag sets per library and compares each function with `main.dol` (relocated fields masked); about 5 minutes. Conclusions in `docs\compiler.md`; `probe.py --show FUNCTION --version V --flags "..."` diffs one function | `main.dol`, `decomp\config\GXCE01\symbols.txt`, compilers, references → `tools\compiler_probe\out\{summary.txt,results.tsv}` (gitignored) |

## 2b. Build main.dol from asm — `tools\batches\batch9_decomp_build.ps1` (Phase 4, Milestones 1 + 2)

```powershell
pwsh tools\batches\batch9_decomp_build.ps1          # run_all.ps1 runs it last; it needs setup step 8
# or by hand, after the batch has generated the config once:
cd decomp; python configure.py; ..\tools\ninja\ninja.exe
```

`decomp\` is a [decomp-toolkit](https://github.com/encounter/decomp-toolkit) project made from `encounter/dtk-template`.
Only `decomp\config\GXCE01\` (config.yml, symbols.txt, splits.txt, build.sha1) and `decomp\reports\` are in git; the
template files, your DOL, the split objects and the linked DOL are local and gitignored. No network use (setup step 8 did
the downloads; `configure.py` stops instead of downloading anything unpinned).

| Step | What it does | Reads → Writes |
|---|---|---|
| `tools\gen_dtk_symbols.py` | dtk's analysis with our cross-checked names applied (`symbols\symbols.txt`, `descriptive.txt`, `cr_globals.txt`). Only the name of an existing dtk symbol changes (same address and kind); never added, removed, moved or resized. A name that is invalid for the linker, given twice, held by another dtk symbol, or has no dtk symbol of its kind at its address keeps dtk's default name and is listed. At one address labels are written after the function / object (mwld hangs otherwise) | `tools\dtk\proj\symbols.txt`, name files → `decomp\config\GXCE01\symbols.txt`, `decomp\reports\symbols_report.txt` |
| `tools\gen_dtk_splits.py` | object (`.c` file) boundaries, only where proven (method in its docstring). **Code**: library objects from the 13 pinned reference decomps' `splits.txt` / `symbols.txt` (both ends anchored by our verified names; an end on unnamed functions is also proven by a G3 assert note, a size run equal to the reference's, or the per-file constant / string pools); when references disagree, the pinned library C sources decide (which file defines the functions in question), else the reference whose function sizes equal ours; the wowjinxy KAR map counts only with a second source (another reference, or doldecomp/kar's asm + src for HSD). **Data** (.rodata .data .bss .sdata .sbss .sdata2 .sbss2 .ctors .dtors): a sections-only scratch link (`decomp\build\splitdata\`, SHA-1 retail) gives every data symbol its users (which code unit accesses it, by relocation or constant offset); an object's piece of a section = the symbols only it uses, in link order between its neighbours, never cutting a constant-offset span, and of the size a reference that proved the object's code gives that piece. Game code only where G3 notes prove both ends (none so far). Everything else stays in dtk's automatic objects (listed in the report) | `decomp\config\GXCE01\symbols.txt`, `config.yml`, reference configs and sources, `symbols\game_real.txt` → `decomp\config\GXCE01\splits.txt`, `decomp\reports\splits_report.txt` |
| `tools\gen_dtk_relocs.py` | Milestone 2: which pointers dtk missed or invented. Splits and links once more with config.yml's generated block removed (`decomp\build\relocs\`, SHA-1 retail), then `tools\reloc_audit.py` checks dtk's relocations against main.dol's bytes: constant propagation over every function (lis + addi / ori pairs, loads / stores, r2 / r13 small-data accesses) and every data word in the moving range 0x80003100..__ArenaLo; jump tables verified (cases in one function with a bctr that references the table); numbers used as bit masks / signed compares, and data words pointing into a function that does not switch on them, are false positives. Writes the fixes as a marked block at the end of `config.yml` (`add_relocations` / `block_relocations`; regenerated every run, never edited by hand) | `main.dol`, config files → `decomp\config\GXCE01\config.yml`, `decomp\reports\relocs_report.txt` |
| `configure.py` + `ninja` | `dtk dol split` writes one object per split straight from your `main.dol` (dtk validates every split here) and normalises both config files once (its re-analysis: `align:` / `data:` attributes, a few sizes, link order; names and ranges unchanged); every object is NonMatching, so `mwldeppc` (GC/1.3.2) links dtk's objects into `build\GXCE01\main.elf` → `main.dol`, and the template checks `build.sha1` | → `decomp\build\GXCE01\` |
| fixpoint + SHA-1 | a second, forced split must leave the three config files byte-identical; the built `main.dol` must have the retail SHA-1 `6a73cd111ef349b20be294511b8c51a8addfb50b` | → `decomp\reports\build_report.txt` |

| `gen_dtk_relocs.py --check` | the built `main.elf`: no missed pointer, no false one (except the blocked numbers), every relocated value equals its target | → `decomp\reports\relocs_report.txt` (last section) |
| `tools\shift_test.py` | the static shift test, on a scratch copy (`decomp\build\shift\`): 0x100 bytes of padding at 30 points chosen by rule (each section start, inside the game code, between library objects, the first glue-span start after the middle of .rodata / .data / .bss, and inside every data section at the first, middle and last data-object boundary that gen_dtk_splits.py proved; never inside a glue span; `--all-data` pads every data-object boundary at once, by hand), those objects reassembled from dtk's asm (a control link without padding must be retail), relinked, then every symbol, relocation, data word, branch and lis pair checked against the unshifted build, and no unrelocated word may still hold a moved address | `decomp\build\GXCE01\` → `decomp\reports\shift_report.txt`, `decomp\build\shift\main.dol` |

## 2c. Boot the shifted build — `tools\batches\batch11_shift_test.ps1` (Phase 4, Milestone 2; run by run_all.ps1, or alone)

```powershell
pwsh tools\batches\batch11_shift_test.ps1           # after batch 9; starts Dolphin twice, about 1 minute
```

Reruns `tools\shift_test.py`, makes `decomp\build\shift\disc\` (hard links to `Custom Robo extracted`, which is never written, with the shifted `sys\main.dol` as a real file), then boots the unmodified folder (control) and the scratch copy for 22 s each with `tools\boot_check.ps1 -Log -Screenshot` (Dolphin's log for that run only, switched on by command-line config). PASS when the shifted build shows no warning dialog, OSInit reports the arena at the shifted `_stack_addr` (the control at the retail one), the log has no error line, and it reads the same disc files in the same order as the control up to the title screen (`sprite/us_title.tpl`, `TITLE_MODEL.dat`). Results and screenshots in `decomp\build\shift\boot\` (timing-dependent, not checksummed). **Don't type or click while it runs:** the Dolphin window takes focus, and a key mapped to Start moves the game past the title screen (Oct 29, 2026: one run reached the main menu and read `sprite/mode_select.tpl`, so the file check failed; two reruns without input passed). That run also showed the shifted build reacting to Start and loading the main menu correctly.

**A battle in the shifted build (by hand)** - the script cannot play:
1. Run batch 9, then batch 11 (it must say PASS). Open Dolphin and boot `decomp\build\shift\disc\sys\main.dol` (File > Open), not your ISO.
2. At the title screen press Start. If there is no save, start a new game when asked (the shifted game uses the same memory card as the real one: make a copy of your memory card / GCI folder first if you care about it).
3. From the main menu start a battle: the versus / battle mode against the computer if your save has it, else Story mode up to its first battle. Keep the default robo and parts, and fight one full battle to the end (win or lose), including the results screen.
4. Back in the menu, start a second battle with a different robo, gun, bomb and pod (different code and motion tables are used), and a different stage.
5. Pass: no Dolphin warning dialog ("Invalid read", "Unknown instruction"), no freeze, the models, animations, sounds and the camera behave as in the real game. Note anything odd and where (menu, battle start, a weapon, the results), and send `decomp\build\shift\boot\boot_result.txt` with it.

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
- only `decomp\*` differs → the name files differ (then symbols.txt does too), or a different dtk / template commit
  than `tools\pins.txt`. `build_report.txt` says whether your built `main.dol` matched the retail one.

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
