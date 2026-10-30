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
| Read the game's code as C, search it | [Reading the code](#reading-the-code) below, [docs/workflows.md](docs/workflows.md#5-read-and-search-the-code) |
| Help name functions | [CONTRIBUTING.md](CONTRIBUTING.md) |
| Rebuild `main.dol` from source (asm + matched C, retail SHA-1) | [docs/REPRODUCE.md](docs/REPRODUCE.md) section 2b, [docs/decomp.md](docs/decomp.md) |
| Help match a library file to C | [CONTRIBUTING.md](CONTRIBUTING.md#matching-a-library-file-phase-4), [docs/decomp.md](docs/decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026) |
| Help match game functions (m2c drafts, the work list) | [docs/drafts.md](docs/drafts.md) |
| Know which compiler and flags built each part | [docs/compiler.md](docs/compiler.md) |
| Understand a file format | [docs/formats/](docs/formats/) |
| Install the tools (Windows, Linux, macOS) | [Install the tools](#install-the-tools) |
| Rebuild everything myself and check nothing is hidden | [docs/REPRODUCE.md](docs/REPRODUCE.md) |
| Find any doc (and how commands are shown per OS) | [docs/README.md](docs/README.md), the index |

## Install the tools

Nothing prebuilt is shipped: everything comes from your package manager, or from the pinned, hash-checked downloads in
[tools/pins.txt](tools/pins.txt) that the setup script fetches. You need **git, Python 3.11+ and a Java 21+ JDK**; on Windows also
**PowerShell 7 (`pwsh`)** and the **.NET SDK 8+** (Dolphin's disc tool is downloaded for you); on Linux / macOS `bash`, `curl` and
`unzip` (the pinned decomp-toolkit extracts the disc, no PowerShell and no Dolphin); and your own *Custom Robo (USA)* disc image.
Copy the block for your system, in order (details, versions and notes: [docs/INSTALL.md](docs/INSTALL.md)):

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
sudo apt install -y git curl unzip python3 python3-venv openjdk-21-jdk
```

(No root? A JDK from the Temurin tarball under `~/.local` works too: [docs/INSTALL.md](docs/INSTALL.md#debian--ubuntu-apt).)

**Arch (KDE: in Konsole, with yay; bash, zsh or fish all work)**

```bash
yay -S --needed git curl unzip python jdk21-openjdk
```

(Without yay: `sudo pacman -S --needed git curl unzip python jdk21-openjdk`. Repository packages only, nothing from the
AUR; the JDK note and the shell note: [docs/INSTALL.md](docs/INSTALL.md#arch-kde-konsole-yay--pacman).)

**Fedora (dnf)**

```bash
sudo dnf install -y git curl unzip python3 java-21-openjdk-devel
```

**macOS (Homebrew, best effort)**

```bash
brew install git python@3.12 openjdk@21 bash
```

**Anything else:** install the tools in the list above with your package manager.

> Tested end to end on Windows and on Ubuntu 26.04 (Oct 30, 2026). On Linux / macOS everything runs except the glTF
> model export and batch 11 (Dolphin boot check), which are skipped. Arch: the install block is verified in an Arch
> container (the yay line from bash, zsh and fish, the pacman line from bash), the pipeline is not yet run there;
> Fedora and macOS are not yet run ([docs/INSTALL.md](docs/INSTALL.md#limits-and-status)).

## Quick start

Clone, set up, run. The first thing the setup does is print each tool's version and stop with a list if one is missing.

Windows:

```powershell
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
pwsh tools\from_scratch.ps1 -Iso "D:\Games\Custom Robo (USA).iso"
```

Linux / macOS (from any shell: the scripts run in bash; the `.sh` scripts are the `.ps1` ones step for step):

```bash
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
bash tools/from_scratch.sh "$HOME/Games/Custom Robo (USA).iso"
```

That one command checks your ISO is the clean retail dump (SHA-1 `fff6e745…0bcb16`), downloads the tools,
extracts the disc, decompiles and names everything, rebuilds `main.dol` from split objects and the matched C with the retail SHA-1, checks
your result is **byte-identical to ours**, and checks the archive/model tools round-trip. It ends with PASS or FAIL.
The first run takes longer than a rerun (downloads, and two Ghidra rebuilds on a fresh setup). The last timed run from
an empty folder (Windows, Oct 27, 2026: 10–15 minutes) was before the Phase 4 batches existed, so expect more now; the
Ubuntu run of Oct 30, 2026 (PASS) took 90 minutes on a 4-core machine with the tools already downloaded.

After that, one command reruns everything. Windows:

```powershell
pwsh tools\batches\run_all.ps1
```

```bash
tools/batches/run_all.sh                 # Linux / macOS
```

It runs batches 1–12 in order, stopping at the first failure, and ends by comparing every output with
[docs/expected_checksums.txt](docs/expected_checksums.txt). The last timed full run took 831 s (plan.md step 58);
step 59 made batch 12 about 4.5 minutes longer. **On Windows, during batch 11 Dolphin opens by itself, twice, for
about a minute: don't type or click** (a key mapped to Start moves the game past the title screen and the check
fails); `from_scratch.ps1` runs it too. Batch 11 is skipped on Linux / macOS.

Every step, what it runs and why: [docs/REPRODUCE.md](docs/REPRODUCE.md).

## Reading the code

After the first run you have, on your machine only: `decompiled/main_dol.c` (the whole game as C, Ghidra's output,
3,565 functions), the same one file per function in `decompiled/funcs/`, the search index `decompiled/index.db` for
`tools/find.py`, and the Ghidra project `ghidra_project/CustomRobo`. How to search and browse them:
[docs/workflows.md](docs/workflows.md#5-read-and-search-the-code).

### What the names mean

| Looks like | Where it comes from | Trust |
|---|---|---|
| `OSReport`, `GXSetTevOrder`, `HSD_JObjAlloc` | **Verified**: matched to the same function in the pinned reference decomps (Melee, Kirby Air Ride, Twilight Princess, Wind Waker, Mario Kart DD, Pikmin, Pikmin 2, TTYD, Sunshine, Mario Party 4, BfBB, Animal Crossing, XD, the Dolphin SDK decomps; [tools/pins.txt](tools/pins.txt)) by signature, assert text, position + size, or the matched C's own call sites. In `symbols/symbols.txt`, each with its evidence. | real names |
| `cr_SEQ_BATTLE_Enter`, `cr_SYS_SET_SCR` | **The game's own names**: Custom Robo's code has no symbols, but its tables and strings still name modes, script commands, camera modes and motions; batch 7 applies them where the tables prove it (`// game-real` in `symbols/descriptive.txt`, with the address of the name). | the game's own word |
| `cr_Archive_OpenEntry` | **Descriptive**: our name for what the function does, where the game gives none. In `symbols/descriptive.txt`, each with a `why:` note. | our best reading |
| `HSD_JObj *jobj`, `jobj->child` | **Types**: the HAL library's structs and prototypes from the Melee decomp's headers, checked against the sizes Custom Robo registers for each class | verified |
| `cr_FileSlot`, `cr_g_FileSlots[i].in_use` | **Our types/globals**: `symbols/cr_types.h`, `symbols/cr_globals.txt`, every field with its offset and evidence | our best reading |
| `FUN_80012345` | not named yet; the number is its address | help wanted! |

## How it is built

```
your ISO ──extract──► Custom Robo extracted\ ──sfd.py──► unpacked\ ──extract_models.py──► models_dat\ ──export_models.py──► models_export\ (glTF)
                                   │
                                   ├─ sys\main.dol ──Ghidra + GameCube loader──► Ghidra project ──► decompiled C + index
                                   │                names: symbols\symbols.txt (verified) + symbols\descriptive.txt (cr_)
                                   │
                                   └─ sys\main.dol ──dtk split (decomp\config\GXCE01\splits.txt)──► objects: dtk's asm, or C from decomp\src
                                                    ──mwcceppc + mwldeppc (decomp\, ninja)──► decomp\build\GXCE01\main.dol == retail SHA-1
```

"extract" is Dolphin's DolphinTool on Windows and the pinned decomp-toolkit on Linux / macOS (byte-identical output).
One script, `pwsh tools\batches\run_all.ps1` / `tools/batches/run_all.sh`, runs everything in order, stopping at the
first failure: the name batches (1, 6, 7) and the Ghidra rebuild to a fixpoint; types (5), the decomp-toolkit
cross-check (2), the game-code map (3, [docs/game_map.md](docs/game_map.md)), the search index (4) and the independent
name cross-check (8); the Phase 4 build of `main.dol` (9), the compiler probe (10, [docs/compiler.md](docs/compiler.md)),
the shifted build booted in Dolphin (11, Windows); the m2c drafts of the game code and their promotion into
`decomp/src/game` (12, [docs/drafts.md](docs/drafts.md)); then the checksum compare. Every step, what it reads and
writes: [docs/REPRODUCE.md](docs/REPRODUCE.md); every tool and batch: [docs/tools.md](docs/tools.md); the project
log: [plan.md](plan.md).

## Status

- Models: all 1,006 models extract and export (glTF; the export is Windows-only); all 484 robo / weapon / story-character entries rebuild byte-identical.
- Mods: model / file / code mods build into a playable ISO and a data-free `.crpatch`; robos can be swapped completely, including different skeletons (tested in battle with the community, Oct 27, 2026). Custom skeletons via joint roles.
- Code: 1,875 of the 3,565 functions in Ghidra's C are named (`decompiled/index.db` of the last full run): 446 of its 1,761 game functions (`docs/game_map.md`) and 1,429 of the other 1,804, the libraries. `symbols/descriptive.txt` holds 432 game names, 300 of them the game's own (incl. the game's own SEQ_* mode names, script command names (regenerated every run from the game's tables by batch 7), CAMMODE_* camera modes and motion names; all SDK/HSD library names verified); HSD types and 672 prototypes applied and checked against the game (`symbols/types_report.txt`), first `cr_` structs/globals for the file system and robos.
- Archives: all 22 `.bin` (SFD) archives unpack and repack byte-identical.
- Rebuild (Phase 4): `main.dol` builds from dtk's split objects with the retail SHA-1, and 389 of its 733 units are compiled from matching C: 0x1D230 of 0x1B342C code bytes = **6.69%** of the code, 0.46% of the data, 794 of the 3,533 functions in dtk's analysis (a different count from Ghidra's 3,565). Per library: Dolphin SDK 47 of 75 objects (49.62% of its code), MSL 29 of 48 (31.87%), Runtime 4 of 5 (92.71%), MetroTRK 10 of 29 (10.45%), HSD 9 of 29 (10.44%; 29 HSD objects split after step 61, 9 before it); game code 290 functions, one object each (1.33% of the game code), promoted from m2c drafts that compile to the retail bytes (`decomp/src/game`). Drafts: 1,751 game functions, 1,697 compile, 293 match 100%, 399,064 of 1,230,376 bytes equal after alignment (plan.md steps 59-63: sized data, the context's missing library prototypes, a repair loop, the globals' sections and m2c's statement forms in the generator). A matched draft that owns a constant or string links once its whole source file is matched (file objects, step 63; none complete yet). A shifted build (code and data moved) boots to the title screen. Source: `decomp/reports/progress_report.txt`, `draft_summary.txt`.
- **Super goal**: a full rebuild of the game from source, every object from C, so new robos and systems are added in source instead of patched ([plan.md, Phase 4](plan.md#phase-4-rebuild-the-game-from-source-super-goal-user-oct-28-2026)).
- Open: what the 17 motion slots are and where animations live (`snr_motion`); per-robo charge; story maps (`snr_map`), `rpg_t/f_models`, the odd `bg_data` entries, model trailer blocks A/B/D ([docs/formats/containers.md](docs/formats/containers.md)); 1,315 of the 1,761 game functions in Ghidra's C are still `FUN_` (`docs/game_map.md`).
- Waiting on a person: a battle played by hand in the shifted build ([docs/REPRODUCE.md](docs/REPRODUCE.md), section 2c).

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
