# Custom Robo (GameCube) — Decomp & Modding Toolkit

Tools, file-format docs, a named symbol map and a growing matching-C decompilation of **Custom Robo** (GameCube, US,
`GXCE01`), so anyone can read the game's code, pull out its 3D models, put modified models back in, and rebuild
`main.dol` from source.

> **No game files are in this repo.** You need your own copy of the game. Everything here is
> our own tools, notes and names; the setup script rebuilds the rest from *your* disc.

## What you can do with it

| I want to… | Go to |
|---|---|
| Look at / export the robots, weapons, stages (Blender, glTF) | [docs/workflows.md](docs/workflows.md) |
| Make a mod (models, files, code) and share it as a patch | [docs/MODDING.md](docs/MODDING.md) |
| Apply someone's mod to my ISO | [docs/MODDING.md](docs/MODDING.md#5-players-apply-a-patch) |
| Read the game's code as C, search it | [Reading the code](#reading-the-code) below |
| Help name functions | [CONTRIBUTING.md](CONTRIBUTING.md) |
| Rebuild `main.dol` from source (asm + matched C, retail SHA-1) | [docs/REPRODUCE.md](docs/REPRODUCE.md) section 2b, [docs/decomp.md](docs/decomp.md) |
| Help match a library file to C | [CONTRIBUTING.md](CONTRIBUTING.md#matching-a-library-file-phase-4), [docs/decomp.md](docs/decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026) |
| Help match game functions (m2c drafts, the work list) | [docs/drafts.md](docs/drafts.md) |
| Know which compiler and flags built each part | [docs/compiler.md](docs/compiler.md) |
| Understand a file format | [docs/formats/](docs/formats/) |
| Install the tools (Windows, Linux, macOS) | [Install the tools](#install-the-tools) |
| Rebuild everything myself and check nothing is hidden | [docs/REPRODUCE.md](docs/REPRODUCE.md) |

## Install the tools

Nothing prebuilt is shipped: everything comes from your package manager, or from the pinned, hash-checked downloads in
[tools/pins.txt](tools/pins.txt) that the setup script fetches. You need **git, Python 3.11+, PowerShell 7 (`pwsh`), a Java 21+ JDK**,
Dolphin (for its disc tool), on Windows the **.NET SDK 8+**, and your own *Custom Robo (USA)* disc image. Copy the block for your
system, in order (details, versions and notes: [docs/INSTALL.md](docs/INSTALL.md)):

**Windows 10/11 (winget)**

```powershell
winget install --id Git.Git -e
winget install --id Python.Python.3.12 -e
winget install --id Microsoft.PowerShell -e
winget install --id Microsoft.DotNet.SDK.8 -e
winget install --id EclipseAdoptium.Temurin.21.JDK -e
```

**Debian / Ubuntu (apt)**

```bash
sudo apt update
sudo apt install -y git curl unzip python3 python3-venv python-is-python3 openjdk-21-jdk dolphin-emu
# PowerShell comes from Microsoft's repo (Ubuntu: use ubuntu/24.04 instead of debian/12)
curl -fsSLO https://packages.microsoft.com/config/debian/12/packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb && rm packages-microsoft-prod.deb
sudo apt update && sudo apt install -y powershell
```

**Arch (pacman, then yay for AUR-only packages)**

```bash
sudo pacman -S --needed git unzip python jdk21-openjdk dolphin-emu
yay -S --needed powershell-bin
```

**Fedora (dnf)**

```bash
sudo dnf install -y git unzip python3 python-unversioned-command java-21-openjdk-devel dolphin-emu dolphin-emu-tool
sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
curl -fsSL https://packages.microsoft.com/config/rhel/9/prod.repo | sudo tee /etc/yum.repos.d/microsoft.repo
sudo dnf install -y powershell
```

**macOS (Homebrew, best effort)**

```bash
brew install git python@3.12 openjdk@21 unzip
brew install --cask powershell dolphin
```

**Anything else:** install the tools in the list above with your package manager.

> Windows is the tested system. On Linux / macOS everything runs except the glTF model export (HSDRawViewer is a Windows
> program); that part is skipped. The Linux / macOS packages and scripts are not tested end to end yet, see
> [docs/INSTALL.md](docs/INSTALL.md#limits-and-status).

## Quick start

Clone, set up, run. The first thing the setup does is print each tool's version and stop with a list if one is missing.

```powershell
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
pwsh tools/from_scratch.ps1 -Iso "D:\Games\Custom Robo (USA).iso"
```

(Linux / macOS: the same, with a path like `"$HOME/Games/Custom Robo (USA).iso"`.)

That one command checks your ISO is the clean retail dump (SHA-1 `fff6e745…0bcb16`), downloads the tools,
extracts the disc, decompiles and names everything, rebuilds `main.dol` from split objects and the matched C with the retail SHA-1, checks
your result is **byte-identical to ours**, and checks the archive/model tools round-trip. It ends with PASS or FAIL.
The first run takes longer than a rerun (downloads, and two Ghidra rebuilds on a fresh setup); the last test from an
empty folder (Oct 27, 2026: 10–15 minutes) was before the Phase 4 batches existed, so expect more than that now.

After that, one command reruns everything:

```powershell
pwsh tools\batches\run_all.ps1
```

It runs batches 1–12 in order and ends with `tools\checksums.ps1 -Compare` by itself (about 17 minutes), stopping
at the first failure. **During batch 11 Dolphin opens by itself, twice, for about a minute: don't type or click**
(a key mapped to Start moves the game past the title screen and the check fails). Batch 11 uses the pinned Dolphin
that `setup.ps1` downloads (`tools\dolphin\Dolphin-x64`), or the folder in `CR_DOLPHIN_DIR`; it is Windows-only and
skipped on Linux / macOS. `from_scratch.ps1` runs `run_all.ps1` too, so the same applies there.

Every step, what it runs and why: [docs/REPRODUCE.md](docs/REPRODUCE.md).

## Reading the code

After `run_all.ps1` you have, on your machine only:

- `decompiled\main_dol.c` — the whole game as C, Ghidra's output (3,565 functions)
- `decompiled\funcs\` — the same, one file per function
- `decompiled\index.db` — a search index used by `tools\find.py`

```powershell
python -I tools\find.py text chara_bin              # search all code
python -I tools\find.py func cr_Archive_OpenEntry --brief   # show one function, compact
python -I tools\find.py callers cr_CharaEntry_OnLoaded       # who uses it
python -I tools\find.py str snr_motion               # find a string and who uses it
python -I tools\find.py hint model                   # game functions that touch 3D models
```

Or open `ghidra_project\CustomRobo` in the Ghidra GUI to browse and click through.

### What the names mean

| Looks like | Where it comes from | Trust |
|---|---|---|
| `OSReport`, `GXSetTevOrder`, `HSD_JObjAlloc` | **Verified**: matched to the same function in the pinned reference decomps (Melee, Kirby Air Ride, Twilight Princess, Wind Waker, Mario Kart DD, Pikmin, Pikmin 2, TTYD, Sunshine, Mario Party 4, BfBB, Animal Crossing, XD, the Dolphin SDK decomps; [tools/pins.txt](tools/pins.txt)) by signature, assert text, position + size, or the matched C's own call sites. In `symbols/symbols.txt`, each with its evidence. | real names |
| `cr_Archive_OpenEntry` | **Descriptive**: our name for what the function does. Custom Robo's own code has no surviving real names. In `symbols/descriptive.txt`, each with a `why:` note. | our best reading |
| `HSD_JObj *jobj`, `jobj->child` | **Types**: the HAL library's structs and prototypes from the Melee decomp's headers, checked against the sizes Custom Robo registers for each class | verified |
| `cr_FileSlot`, `cr_g_FileSlots[i].in_use` | **Our types/globals**: `symbols/cr_types.h`, `symbols/cr_globals.txt`, every field with its offset and evidence | our best reading |
| `FUN_80012345` | not named yet; the number is its address | help wanted! |

## How it is built

```
your ISO ──DolphinTool──► Custom Robo extracted\ ──sfd.py──► unpacked\ ──extract_models.py──► models_dat\ ──export_models.py──► models_export\ (glTF)
                                   │
                                   ├─ sys\main.dol ──Ghidra + GameCube loader──► Ghidra project ──► decompiled C + index
                                   │                names: symbols\symbols.txt (verified) + symbols\descriptive.txt (cr_)
                                   │
                                   └─ sys\main.dol ──dtk split (decomp\config\GXCE01\splits.txt)──► objects: dtk's asm, or C from decomp\src
                                                    ──mwcceppc + mwldeppc (decomp\, ninja)──► decomp\build\GXCE01\main.dol == retail SHA-1
```

`tools\batches\run_all.ps1` is the one script that runs everything, in order, stopping at the first failure:

1. **Names ↔ Ghidra fixpoint**: batch 1 (library names by layout proof), batch 6 (the remaining library names),
   batch 7 (the game's own names from its tables and strings) and the lint, then a Ghidra rebuild with the names and
   types, then the name batches again, rebuilding until no name file changes.
2. **Batch 5** types check, **batch 2** cross-check against decomp-toolkit, **batch 3** game-code map
   ([docs/game_map.md](docs/game_map.md)), **batch 4** search index, **batch 8** independent cross-check of every name.
3. **Batch 9** the decomp build (`decomp\`, a decomp-toolkit project): object boundaries, the objects linked from C,
   `main.dol` must have the retail SHA-1; relocation check and static shift test.
4. **Batch 10** compiler probe ([docs/compiler.md](docs/compiler.md)), **batch 11** the shifted build booted in Dolphin.
5. **Batch 12** m2c drafts of every game function, compiled and compared ([docs/drafts.md](docs/drafts.md)); then
   `tools\promote_drafts.py` puts every draft that matches and can be linked into `decomp\src\game` (batch 9 again
   when that changed).
6. `tools\checksums.ps1 -Compare`: every output byte-identical to [docs/expected_checksums.txt](docs/expected_checksums.txt).

Details: [docs/REPRODUCE.md](docs/REPRODUCE.md), [docs/decomp.md](docs/decomp.md), tools: [docs/tools.md](docs/tools.md),
project log: [plan.md](plan.md).

## Status

- Models: all 1,006 models extract and export (glTF); robo/weapon/story entries rebuild byte-identical.
- Mods: model / file / code mods build into a playable ISO and a data-free `.crpatch`; robos can be swapped completely, including different skeletons (tested in battle with the community, Oct 27, 2026). Custom skeletons via joint roles.
- Code: 1,875 of the 3,565 functions in Ghidra's C are named (`decompiled\index.db` of the last full run): 1,429 of the 1,800 library functions and 432 of the 1,751 game functions (incl. the game's own SEQ_* mode names, script command names (regenerated every run from the game's tables by batch 7), CAMMODE_* camera modes and motion names; all SDK/HSD library names verified); HSD types and 672 prototypes applied and checked against the game (`symbols\types_report.txt`), first `cr_` structs/globals for the file system and robos.
- Archives: all 22 `.bin` (SFD) archives unpack and repack byte-identical.
- Rebuild (Phase 4): `main.dol` builds from dtk's split objects with the retail SHA-1, and 244 of its 495 units are compiled from matching C: 0x162E4 of 0x1B342C code bytes = **5.10%** of the code, 0.38% of the data, 572 of the 3,533 functions in dtk's analysis (a different count from Ghidra's 3,565). Per library: Dolphin SDK 47 of 75 objects (49.62% of its code), MSL 29 of 48 (31.87%), Runtime 4 of 5 (92.71%), MetroTRK 10 of 29 (10.45%), HSD 2 of 9 (1.17%); game code 152 functions, one object each (0.31% of the game code), promoted from m2c drafts that compile to the retail bytes (`decomp/src/game`). Drafts: 1,751 game functions, 891 compile, 153 match 100%. A shifted build (code and data moved) boots to the title screen. Source: `decomp/reports/progress_report.txt`, `draft_summary.txt`.
- **Super goal**: a full rebuild of the game from source, every object from C, so new robos and systems are added in source instead of patched ([plan.md, Phase 4](plan.md#phase-4-rebuild-the-game-from-source-super-goal-user-oct-28-2026)).
- Open: what the 17 motion slots are and where animations live; per-robo charge; 1,319 of the game's own 1,751 functions are still `FUN_`.

## What is in this repo

All of the project's own work: tools, batch scripts, symbol names with their evidence, type headers, reports, docs and
findings, the matched C in `decomp/src` (library files from the pinned reference decomps, game functions whose draft
compiles to the retail bytes), plus everything needed to reproduce the whole result from your own disc (pinned tools
and references, [docs/REPRODUCE.md](docs/REPRODUCE.md), expected checksums). Everything generated from the disc
(extracted files, models, Ghidra's C and project, the m2c drafts, the build) is gitignored and made locally by the
scripts; mods are shared as `.crpatch` files applied to a clean retail ISO ([CONTRIBUTING.md](CONTRIBUTING.md)).

## License

[MIT](LICENSE) for this repo's own tools, scripts and docs. Custom Robo and anything generated from it are not covered.

## Credits

Built on [Ghidra](https://github.com/NationalSecurityAgency/ghidra), the
[GameCube loader](https://github.com/Cuyler36/Ghidra-GameCube-Loader),
[decomp-toolkit](https://github.com/encounter/decomp-toolkit) and [dtk-template](https://github.com/encounter/dtk-template),
[objdiff](https://github.com/encounter/objdiff), [m2c](https://github.com/matt-kempster/m2c), [HSDLib](https://github.com/Ploaj/HSDLib),
[ninja](https://github.com/ninja-build/ninja), [gc-wii-binutils](https://github.com/encounter/gc-wii-binutils),
[sjiswrap](https://github.com/encounter/sjiswrap), [wibo](https://github.com/decompals/wibo), the CodeWarrior compiler
archive from [decomp.dev](https://decomp.dev), [Dolphin](https://dolphin-emu.org),
and the symbol and source work of the [Melee](https://github.com/doldecomp/melee), [Kirby Air Ride](https://github.com/doldecomp/kar)
(and [wowjinxy's map](https://github.com/wowjinxy/KAR)), [Twilight Princess](https://github.com/zeldaret/tp),
[Wind Waker](https://github.com/zeldaret/tww), [Mario Kart DD](https://github.com/doldecomp/mkdd),
[Pikmin](https://github.com/projectPiki/pikmin), [Pikmin 2](https://github.com/doldecomp/pikmin2), [TTYD](https://github.com/doldecomp/ttyd),
[Sunshine](https://github.com/doldecomp/sms), [Mario Party 4](https://github.com/mariopartyrd/marioparty4),
[BfBB](https://github.com/bfbbdecomp/bfbb), [Animal Crossing](https://github.com/ACreTeam/ac-decomp),
[XD](https://github.com/TeamOrre/xd-decomp), [dolsdk2001](https://github.com/doldecomp/dolsdk2001) and
[dolsdk2004](https://github.com/doldecomp/dolsdk2004) decomp projects (exact commits in [tools/pins.txt](tools/pins.txt)).
