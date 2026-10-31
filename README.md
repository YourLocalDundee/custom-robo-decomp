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
| Check that my checkout is good (the short and the full check, with durations) | [docs/REPRODUCE.md, "Verify this checkout"](docs/REPRODUCE.md#verify-this-checkout) |
| Rebuild `main.dol` from source (asm + matched C, retail SHA-1) | [docs/REPRODUCE.md](docs/REPRODUCE.md) section 2b, [docs/decomp.md](docs/decomp.md) |
| Work on a branch in a second worktree, merge it back | [docs/workflows.md](docs/workflows.md#14-work-in-a-worktree-and-merge-it-back), [docs/REPRODUCE.md](docs/REPRODUCE.md#a-second-checkout-git-worktree) |
| Help match a library file to C | [CONTRIBUTING.md](CONTRIBUTING.md#matching-a-library-file-phase-4), [docs/decomp.md](docs/decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026) |
| Help match game functions (m2c drafts, the work list) | [docs/drafts.md](docs/drafts.md) |
| Know which compiler and flags built each part | [docs/compiler.md](docs/compiler.md) |
| Understand a file format | [docs/formats/](docs/formats/) |
| Install the tools (Windows; the Linux / macOS notes are not maintained at the moment) | [Install the tools](#install-the-tools) |
| Rebuild everything myself and check nothing is hidden | [docs/REPRODUCE.md](docs/REPRODUCE.md) |
| Find any doc (and how commands are shown per OS) | [docs/README.md](docs/README.md), the index |

## Install the tools

Nothing prebuilt is shipped: everything comes from your package manager, or from the pinned, hash-checked downloads in
[tools/pins.txt](tools/pins.txt) that the setup script fetches. You need **git, Python 3.11+ and a Java 21+ JDK**; on Windows also
**PowerShell 7 (`pwsh`)** and the **.NET SDK 8+** (Dolphin's disc tool is downloaded for you); on Linux / macOS `bash`, `curl` and
`unzip` (the pinned decomp-toolkit extracts the disc, no PowerShell and no Dolphin); and your own *Custom Robo (USA)* disc image.
Copy the block for your system, in order (details, the versions in use on the development machine, where each tool
ends up: [docs/INSTALL.md](docs/INSTALL.md)). Blender (any 4.x / 5.x; 5.2 in use) is only needed for the model loop,
Dolphin only for booting builds (the pinned one is downloaded).

**Windows 10/11 (winget)**

```powershell
winget install --id Git.Git -e
winget install --id Python.Python.3.12 -e
winget install --id Microsoft.PowerShell -e
winget install --id Microsoft.DotNet.SDK.8 -e
winget install --id EclipseAdoptium.Temurin.21.JDK -e
```

The Linux and macOS blocks below are kept as they were last tested (Oct 30, 2026) and are **not maintained at the
moment**: the project is developed and verified on Windows only.

**Debian / Ubuntu (apt)**

```bash
sudo apt update
sudo apt install -y git curl unzip python3 python3-venv openjdk-21-jdk
```

(No root? A JDK from the Temurin tarball under `~/.local` works too: [docs/INSTALL.md](docs/INSTALL.md#debian--ubuntu-apt).)

**Arch (Hyprland or KDE, any terminal, with yay; bash, zsh or fish all work)**

```bash
yay -S --needed git curl unzip python jdk21-openjdk
```

(Without yay: `sudo pacman -S --needed git curl unzip python jdk21-openjdk`. Repository packages only, nothing from the
AUR; the JDK note and the shell note: [docs/INSTALL.md](docs/INSTALL.md#arch-hyprland-or-kde-any-terminal-yay--pacman).)

**Fedora (dnf)**

```bash
sudo dnf install -y git curl unzip python3 java-21-openjdk-devel
```

**macOS (Homebrew, best effort)**

```bash
brew install git python@3.12 openjdk@21 bash
```

**Anything else:** install the tools in the list above with your package manager.

> Verified end to end on Windows (every step up to plan.md step 83, Oct 31, 2026). Ubuntu 26.04 passed once on
> Oct 30, 2026 (glTF export and batch 11 skipped there); Arch's install block was checked in a container; Fedora and
> macOS were never run. None of the non-Windows paths has been run since ([docs/INSTALL.md](docs/INSTALL.md#limits-and-status)).

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
The first run takes longer than a rerun (downloads, and two Ghidra rebuilds on a fresh setup). Expect about an hour
on a desktop: the game-code drafts (batch 12) alone take about 26 minutes, the Ghidra rebuild about 4 minutes per
pass, the compiler probe 5 minutes. Timed on Oct 31, 2026 (Windows, 8 cores): `run_all` 2,138 s (36 minutes, one
rebuild) in a set-up checkout; `from_scratch.ps1` 2,489 s (41 minutes, PASS) in a fresh clone with the tools already in the download cache (Ubuntu, Oct 30, 2026: 90 minutes on 4 cores)
([docs/REPRODUCE.md, "Verify this checkout"](docs/REPRODUCE.md#verify-this-checkout)).

After that, one command reruns everything. Windows:

```powershell
pwsh tools\batches\run_all.ps1
```

```bash
tools/batches/run_all.sh                 # Linux / macOS
```

It runs batches 1–12 in order, stopping at the first failure, and ends by comparing every output with
[docs/expected_checksums.txt](docs/expected_checksums.txt); 36 minutes when a previous Ghidra export exists
(batch 12 is 26 of them). **On Windows, during batch 11 Dolphin opens by itself, twice, for
about a minute (32 s each when its memory card has no Custom Robo save): don't type or click** (a key mapped to Start
moves the game past the title screen and the check fails); `from_scratch.ps1` runs it too. Batch 11 is skipped on
Linux / macOS. To check a checkout without the full run (batch 9, batch 8, checksums; about a minute):
[docs/REPRODUCE.md, "Verify this checkout"](docs/REPRODUCE.md#verify-this-checkout).

Every step, what it runs and why: [docs/REPRODUCE.md](docs/REPRODUCE.md).

## Reading the code

After the first run you have, on your machine only: `decompiled/main_dol.c` (the whole game as C, Ghidra's output,
3,565 functions, 2,078 of them named), the same one file per function in `decompiled/funcs/`, the search index `decompiled/index.db` for
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
- Code: 2,078 of the 3,565 functions in Ghidra's C are named (`decompiled/main_dol.c` of the last full run, Oct 31, 2026): 481 of its 1,761 game functions (`docs/game_map.md`) and 1,597 of the other 1,804, the libraries. `symbols/descriptive.txt` holds 477 game names, 300 of them the game's own (incl. the game's own SEQ_* mode names, script command names (regenerated every run from the game's tables by batch 7), CAMMODE_* camera modes and motion names; all SDK/HSD library names verified); HSD types and 748 prototypes applied and checked against the game (`symbols/types_report.txt`: 37 struct sizes and 841 field offsets checked), 82 typed `cr_g_` globals (`symbols/cr_globals.txt`) and the `cr_` structs of `symbols/cr_types.h`: the file system, robos, the script engine, scenario, pads, game mode, save data, stage selector and customize screen (naming rounds 1-4). Every name is cross-checked on every run: 2,381 names, 0 FAIL (`docs/names_crosscheck.md`).
- Archives: all 22 `.bin` (SFD) archives unpack and repack byte-identical.
- Rebuild (Phase 4, plan.md step 83, Oct 31, 2026): `main.dol` builds from dtk's split objects with the retail SHA-1, and **646 of its 922 units are compiled from matching C: 0x608F8 of 0x1B3430 code bytes = 22.18% of the code, 5.90% of the data**, 1,803 of the 3,533 functions in dtk's analysis (a different count from Ghidra's 3,565). Every non-HSD library is complete: Dolphin SDK 98 of 98 objects, MSL 49 of 49, Runtime 5 of 5, MetroTRK 29 of 29 (100% of their code and data); HSD (sysdolphin) 22 of 30 split objects (42.57% of its code, 79.44% of its data). Game code: 443 objects from C (0xCC44 bytes, 4.25% of the game code): 438 one-function objects, 4 file objects (runs of functions that share a constant pool, with their own data) and 1 kept, all promoted by script from m2c drafts that compile to the retail bytes (`decomp/src/game`). Drafts: 1,751 game functions, 1,706 compile, **480 match 100%**, 520,092 of 1,230,376 bytes equal after alignment (ten generator rounds, plan.md steps 57-83). A shifted build (code and data moved, 0x100 bytes at 30 points) boots to the title screen (batch 11, last PASS Oct 31, 2026). Source: `decomp/reports/progress_report.txt`, `draft_summary.txt`, `promote_report.txt`.
- **Super goal**: a full rebuild of the game from source, every object from C, so new robos and systems are added in source instead of patched ([plan.md, Phase 4](plan.md#phase-4-rebuild-the-game-from-source-super-goal-user-oct-28-2026)).
- Models and stages (Phase 1): the Blender loop replaces a robo completely (custom skeleton, baked textures, smooth skinning; the legionnaire v2 was seen in Customize and in a battle on Oct 31, 2026, and one atlas per body turned out to be the game's rule) and edits a stage (`bg_data` scene models round-trip byte for byte for all 40 stages; the first edited arena, a pillar on Basic Arena, builds and boots). [docs/MODDING.md](docs/MODDING.md), [docs/stage_tables.md](docs/stage_tables.md), [docs/robo_tables.md](docs/robo_tables.md).
- Open: what the 17 motion slots are and where animations live (`snr_motion`); per-robo charge; story maps (`snr_map`), `rpg_t/f_models`, the odd `bg_data` entries, model trailer blocks A/B/D ([docs/formats/containers.md](docs/formats/containers.md)); stage collision editing; 1,280 of the 1,761 game functions in Ghidra's C are still `FUN_` (`docs/game_map.md`); the 8 HSD objects, the game's data and the ~1,270 game functions not yet matched (plan.md "What is left for the super goal").
- Waiting on a person: a battle played by hand in the shifted build ([docs/REPRODUCE.md](docs/REPRODUCE.md), section 2c); the legionnaire v2 atlas build and the Basic Arena pillar seen in the game (plan.md "Waiting on the user").

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
