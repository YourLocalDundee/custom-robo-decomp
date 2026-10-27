# Custom Robo (GameCube) — Decomp & Modding Toolkit

Tools, file-format docs and a named symbol map for **Custom Robo** (GameCube, US, `GXCE01`), so anyone can
read the game's code, pull out its 3D models, and put modified models back in.

> **No game files are in this repo.** You need your own copy of the game. Everything here is
> our own tools, notes and names; the setup script rebuilds the rest from *your* disc.

## What you can do with it

| I want to… | Go to |
|---|---|
| Look at / export the robots, weapons, stages (Blender, glTF) | [docs/workflows.md](docs/workflows.md) |
| Swap a model in the game | [docs/workflows.md](docs/workflows.md), [docs/formats/model_entry.md](docs/formats/model_entry.md) |
| Read the game's code as C, search it | [Reading the code](#reading-the-code) below |
| Help name functions | [CONTRIBUTING.md](CONTRIBUTING.md) |
| Understand a file format | [docs/formats/](docs/formats/) |
| Rebuild everything myself and check nothing is hidden | [docs/REPRODUCE.md](docs/REPRODUCE.md) |

## Quick start (Windows)

You need: **git**, **Python 3.11+**, **PowerShell 7** (`pwsh`), **Java 21+** and the **.NET SDK 8 or newer**,
and your own *Custom Robo (USA)* disc image. Everything else (Ghidra, Dolphin's disc tool, decomp-toolkit, …)
is downloaded at a pinned version and hash-checked.

```powershell
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
pwsh tools\from_scratch.ps1 -Iso "D:\Games\Custom Robo (USA).iso"
```

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
python -I tools\find.py callers cr_RoboModel_OnLoaded       # who uses it
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

- Models: all 1,006 models extract and export; robo/weapon entries rebuild byte-identical; custom model import works offline (in-game check pending).
- Code: ~1,050 of ~3,565 functions named (all SDK/HSD library names verified); the game's own ~1,750 functions are being named descriptively.
- Archives: all 22 `.bin` (SFD) archives unpack and repack byte-identical.

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
