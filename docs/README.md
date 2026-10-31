# Documentation index

Every document in this repo, what it is for, and which ones are written by tools. Start with the
[top-level README](../README.md) (what the project is, quick start, status); the project log and roadmap is
[plan.md](../plan.md). Each topic has one home; other docs link to it instead of repeating it.

## Commands in these docs

Every command is shown for the system it runs on:

| | Windows 10 / 11 | Linux and macOS |
|---|---|---|
| Shell | PowerShell 7 (`pwsh`, not the blue Windows PowerShell 5.1) | bash |
| Project scripts | `pwsh tools\setup.ps1 ...`, `pwsh tools\batches\run_all.ps1` | `tools/setup.sh ...`, `tools/batches/run_all.sh` |
| Script options | `-Compare`, `-SkipRebuild`, `-Jobs 8` | `--compare`, `--skip-rebuild`, `--jobs 8` |
| Python tools | `python -I tools\find.py ...` | `python3 -I tools/find.py ...` |

Code blocks come in pairs, Windows first, then Linux / macOS. In tables and running text a script is named as
`pwsh tools\x.ps1` / `tools/x.sh`, and a Python tool by its script and arguments only (`tools/find.py text chara_bin`):
run it with the Python of your system as in the last row. The `.sh` scripts do the same steps as the `.ps1` ones, with
the same arguments and the same output files; PowerShell is never needed on Linux or macOS. Batch 11 (booting the
shifted build in Dolphin) and the glTF model export are Windows-only ([INSTALL.md](INSTALL.md#limits-and-status)).
Every command runs from the repo root. `tools/check_docs.py` checks this convention, the links and this index
([tools.md](tools.md#checking-the-docs)).

## Start here

| Doc | What it is for |
|---|---|
| [../README.md](../README.md) | Front door: what the project is, install and quick start per OS, status, where to go next |
| [INSTALL.md](INSTALL.md) | The tools to install per OS (Windows, Debian / Ubuntu, Arch, Fedora, macOS), versions, limits |
| [arch_container.md](arch_container.md) | The Arch Linux test image (`tools/arch_container/Dockerfile`): how it is built and run, what it verified |
| [REPRODUCE.md](REPRODUCE.md) | Fresh ISO to the named decompile and the rebuilt `main.dol`: every step, what it runs, reads, writes and downloads, checksums to compare |
| [../CONTRIBUTING.md](../CONTRIBUTING.md) | How to help: naming rules for contributors, types, matching a library file, repo rules |
| [../plan.md](../plan.md) | Project log (every step, numbered and dated) and roadmap; the full naming policy |

## Using the project

| Doc | What it is for |
|---|---|
| [workflows.md](workflows.md) | Step by step: look at and export models, edit and re-import one, test in Dolphin, read and search the code |
| [MODDING.md](MODDING.md) | Make a mod (models, files, code), build a playable ISO and a data-free `.crpatch`, apply someone's patch |
| [tools.md](tools.md) | Every tool in `tools/`: purpose, usage, inputs and outputs; the batches; the third-party tools |

## Code and the rebuild (Phase 4)

| Doc | What it is for |
|---|---|
| [decomp.md](decomp.md) | Ghidra's C (pipeline, output, first findings) and building library objects from C |
| [compiler.md](compiler.md) | Which CodeWarrior version and flags built each part of `main.dol` (batch 10), as applied in `decomp/configure.py` |
| [drafts.md](drafts.md) | Batch 12: an m2c C draft of every game function, compiled and compared; promotion into `decomp/src/game` |

## File formats and community research

| Doc | What it is for |
|---|---|
| [formats/sfd.md](formats/sfd.md) | The `.BIN` SFD archive format (verified on all 22 archives) |
| [formats/model_entry.md](formats/model_entry.md) | Robo / weapon / story-character model entries (header decoded from the game's loaders), joint roles |
| [formats/containers.md](formats/containers.md) | Every archive and file type on the disc: what is inside, what is decoded, what is not |
| [community_codes.md](community_codes.md) | Public AR / Gecko codes (decrypted, mapped to our addresses), texture mods, Gecko tooling (research notes, Oct 28, 2026) |
| [robo_tables.md](robo_tables.md) | Every table, file and constant indexed by the body (robo) number, and what must grow for a new body (step 71, Oct 30, 2026) |
| [images/ray_legionnaire_v1_customize.jpg](images/ray_legionnaire_v1_customize.jpg) | Screenshot: the first edited robo model (Ray 01 legionnaire v1) in the Customize screen, Oct 30, 2026 |
| [images/ray_legionnaire_v1_battle1.jpg](images/ray_legionnaire_v1_battle1.jpg), [images/ray_legionnaire_v1_battle2.jpg](images/ray_legionnaire_v1_battle2.jpg) | Screenshots: the same model in a battle, the added parts follow their bones, Oct 30, 2026 |
| [community/discord_codes.md](community/discord_codes.md) | Codes and addresses from the community Discord, mapped to our functions (Oct 29, 2026) |
| [community/crbr_parts_expansion.md](community/crbr_parts_expansion.md) | DesiacX's CRBR-Parts-Expansion and Project Hive's sources mapped to our names: how they add part slots, record layouts, free space, textures, similar repos (step 72) |
| [community/codes_gxce01.tsv](community/codes_gxce01.tsv) | The codes of community_codes.md as a table, one row per code. Research data of Oct 28, 2026: no tool in the repo writes it; keep it in step with community_codes.md |

## Written by tools (never edit by hand)

Rerun the tool instead. The batches rewrite theirs on every run of `pwsh tools\batches\run_all.ps1` /
`tools/batches/run_all.sh`.

| File | Written by | What it holds |
|---|---|---|
| [game_map.md](game_map.md) | `tools/game_map.py` (batch 3) | Map of the game's own code: subsystem hints, source files, hub functions, file-name users |
| [names_crosscheck.md](names_crosscheck.md) | `tools/crosscheck_names.py` (batch 8) | Independent cross-check of every name: OK / WEAK / UNCHECKABLE / FAIL per source |
| [expected_checksums.txt](expected_checksums.txt) | `pwsh tools\checksums.ps1 -Write` / `tools/checksums.sh --write` (maintainers, after an intended change) | SHA-256 of every checked output; `-Compare` / `--compare` checks yours against it ([REPRODUCE.md](REPRODUCE.md#if-a-checksum-differs)) |
| [name_evidence.md](name_evidence.md), [name_tables.tsv](name_tables.tsv) | `tools/name_evidence.py` (run by hand, not by a batch) | The game's own names (name tables, identifier / format / source-file strings) and the code that uses them |
| [struct_cr_Chara.md](struct_cr_Chara.md) | `tools/struct_fields.py cr_Chara report` (run by hand) | Accesses inside the unknown stretches of `cr_Chara`. Snapshot of Oct 27, 2026: its `FUN_` names predate later naming |

## How claims are marked

- **Verified**: checked by code reading, a tool run, or an independent source; the check is named.
- **Inferred**: follows from the data, but no code was found that confirms it.
- **Unknown** / **not verified**: not worked out yet.

Dates are absolute (Oct 29, 2026). A count names the report it comes from (`decomp/reports/*.txt`, `symbols/*.txt`);
when a doc and a report disagree, the report of the last full run is right.
