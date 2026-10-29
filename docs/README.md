# Custom Robo (GC) research — documentation

Reference docs for `D:\emulators\gameTesting`. The running project log and roadmap is [`../plan.md`](../plan.md).

| Doc | What it covers |
|---|---|
| [workflows.md](workflows.md) | Step-by-step: view models, export, edit + re-import, build a test copy, rebuild the decompile |
| [tools.md](tools.md) | Every tool in `tools\`: purpose, usage, inputs/outputs |
| [formats/sfd.md](formats/sfd.md) | `.BIN` SFD archive format (verified on all 22 archives) |
| [formats/model_entry.md](formats/model_entry.md) | Robo / weapon / story-character model entries (header decoded from the game's loaders) |
| [formats/containers.md](formats/containers.md) | Every archive on the disc: what is inside, what is decoded, what is not |
| [decomp.md](decomp.md) | Decompile pipeline, naming policy and verification, code findings with addresses |
| [drafts.md](drafts.md) | Batch 12: m2c C draft of every game function, compiled with the game compiler and compared with the original |
| [REPRODUCE.md](REPRODUCE.md) | Fresh ISO to named, searchable decompile: every step, what it runs, checksums to compare |
| [game_map.md](game_map.md) | Generated map of the game's own code: subsystems, hub functions, file-name users |

## Game

- Custom Robo, GameCube, US, game ID **GXCE01**, v1.0.
- Working ISO `D:\emulators\gameTesting\Custom Robo (USA).iso` is a copy of
  `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Games\Custom Robo (USA).iso` (original untouched).
- MD5 `546e5108173bceb4d5c8ca27b3fcfc96`, SHA-1 `fff6e74590366a5c0c5ba0d40c8ccf94770bcb16` — match GameTDB's GXCE01 entry (clean retail dump).

## How claims are marked in these docs

- **Verified** — checked by code reading, a tool run, or an independent source; the check is named.
- **Inferred** — follows from the data but no code was found that confirms it.
- **Unknown** — not worked out yet.

## Status at a glance (Oct 25, 2026)

| Area | State |
|---|---|
| Disc extract, SFD unpack/repack | Done; all 22 archives repack byte-identical |
| Model export | 1,006 models in 14 categories exported (2,130 `.glb`, 59 `.dae`, 1,913 texture `.png`) |
| Model import (robo / weapon / story char) | `crmodel.py from-dat` works; 484/484 untouched entries round-trip byte-identical; an edited + relocated robo boots in Dolphin; in-game look **not yet confirmed** |
| Decompile | 3,416 functions as C; 347 verified names applied; `unaff_r13` = 0 |
| Not decoded yet | Animations (`snr_motion`), story maps (`snr_map`), `rpg_t/f_models`, odd `bg_data` entries, model trailer blocks A/B/D |
