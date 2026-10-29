# Custom Robo (GameCube) — Decomp & Modding Toolkit

Tools, file-format docs and a named symbol map for **Custom Robo** (GameCube, US, `GXCE01`), so anyone can
read the game's code, pull out its 3D models, and put modified models back in.

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
extracts the disc, decompiles and names everything, checks your result is **byte-identical to ours**, and
checks the archive/model tools round-trip. It ends with PASS or FAIL (10–15 minutes the first time).

After that, rebuilding takes about 2 minutes:

```powershell
pwsh tools\batches\run_all.ps1
pwsh tools\checksums.ps1 -Compare
```

Every step, what it runs and why: [docs/REPRODUCE.md](docs/REPRODUCE.md).

## Reading the code

After `run_all.ps1` you have, on your machine only:

- `decompiled\main_dol.c` — the whole game as C (≈3,500 functions)
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
| `OSReport`, `GXSetTevOrder`, `HSD_JObjAlloc` | **Verified**: matched to the same function in other GameCube decomps (Melee, Twilight Princess, Mario Kart DD, Pikmin 2, TTYD, Wind Waker) by signature, assert text, or position + size. In `symbols/symbols.txt`. | real names |
| `cr_Archive_OpenEntry` | **Descriptive**: our name for what the function does. Custom Robo's own code has no surviving real names. In `symbols/descriptive.txt`, each with a `why:` note. | our best reading |
| `HSD_JObj *jobj`, `jobj->child` | **Types**: the HAL library's structs and prototypes from the Melee decomp's headers, checked against the sizes Custom Robo registers for each class | verified |
| `cr_FileSlot`, `cr_g_FileSlots[i].in_use` | **Our types/globals**: `symbols/cr_types.h`, `symbols/cr_globals.txt`, every field with its offset and evidence | our best reading |
| `FUN_80012345` | not named yet; the number is its address | help wanted! |

## How it is built

```
your ISO ──DolphinTool──► Custom Robo extracted\ ──sfd.py──► unpacked\ ──extract_models.py──► models_dat\ ──export_models.py──► models_export\ (glTF)
                                   │
                                   └─ sys\main.dol ──Ghidra + GameCube loader──► Ghidra project ──► decompiled C + index
                                                    names: symbols\symbols.txt (verified) + symbols\descriptive.txt (cr_)
```

`tools\batches\run_all.ps1` runs: **(1)** verified library naming against the reference decomps → **(2)** Ghidra
rebuild → cross-check vs decomp-toolkit → **(3)** game-code map ([docs/game_map.md](docs/game_map.md)) → **(4)** search index.
Details: [docs/decomp.md](docs/decomp.md), tools: [docs/tools.md](docs/tools.md), project log: [plan.md](plan.md).

## Status

- Models: all 1,006 models extract and export (glTF); robo/weapon/story entries rebuild byte-identical.
- Mods: model / file / code mods build into a playable ISO and a data-free `.crpatch`; robos can be swapped completely, including different skeletons (tested in battle with the community, Oct 27, 2026). Custom skeletons via joint roles.
- Code: ~1,860 of ~3,565 functions named (incl. the game's own SEQ_* mode names, script command names, CAMMODE_* camera modes and motion names) (all SDK/HSD library names verified), HSD types and 441 prototypes applied and checked against the game, first `cr_` structs/globals for the file system and robos.
- Archives: all 22 `.bin` (SFD) archives unpack and repack byte-identical.
- Open: what the 17 motion slots are and where animations live; per-robo charge; naming the game's own ~1,740 functions.

## What is in this repo

All of the project's own work: tools, batch scripts, symbol names with their evidence, type headers, reports, docs and
findings, plus everything needed to reproduce the whole result from your own disc (pinned tools and references,
[docs/REPRODUCE.md](docs/REPRODUCE.md), expected checksums).

## What is not in this repo (and never will be)

The ISO, extracted files, ripped models / textures / audio, the decompiled C, and the Ghidra project. They are
Nintendo's / Noise's work; you generate them locally from your own disc. Mods are shared as patches applied to
a clean retail ISO.

## License

[MIT](LICENSE) for this repo's own tools, scripts and docs. Custom Robo and anything generated from it are not covered.

## Credits

Built on [Ghidra](https://github.com/NationalSecurityAgency/ghidra), the
[GameCube loader](https://github.com/Cuyler36/Ghidra-GameCube-Loader),
[decomp-toolkit](https://github.com/encounter/decomp-toolkit), [HSDLib](https://github.com/Ploaj/HSDLib),
and the symbol work of the [Melee](https://github.com/doldecomp/melee), [Twilight Princess](https://github.com/zeldaret/tp),
[Wind Waker](https://github.com/zeldaret/tww), [Mario Kart DD](https://github.com/doldecomp/mkdd),
[Pikmin 2](https://github.com/doldecomp/pikmin2) and [TTYD](https://github.com/doldecomp/ttyd) decomp projects.
