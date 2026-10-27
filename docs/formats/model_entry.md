# Model entry format (robo, weapon, story character)

Used by entries of `chara.BIN`, `weapon_*.BIN` and `scenario_*\snr_chara.BIN`.

**Sources**: the game's own loaders in `decompiled\main_dol.c` (addresses below), cross-checked by HSDLib parsing and exporting every entry, and by `tools\crmodel.py roundtrip` (484/484 entries byte-identical).

## Summary

An entry is HAL Laboratory **sysdolphin (HSD)** model data — the same engine library and structures as Super Smash Bros. Melee — behind a game-specific 0x40-byte header instead of the standard HSD .dat header.

```
0x00  header (16 × u32, big-endian)
0x40  data block: HSD structures; every pointer is an offset from 0x40
....  reloc table: u32 offsets (from 0x40) of every pointer slot in the data block
....  trailer blocks A, B, C (and D in some story characters)
```

## Loaders (verified by reading the decompiled code)

Function names stay `FUN_…` — no verified real names exist for game code.

| Function | Loads | What it does |
|---|---|---|
| `FUN_800747b0`, `FUN_800748a8` | robo, story character ("abs" variant) | base = entry + header[0]; for i < header[7]: `*(base + table[i]) += base` with table at **entry + header[6]**; header[2] += base; header[3], [4], [8] += entry. Then loads the root joint `**header[2]` with `FUN_801988fc` and, if the JOBJDesc has joint animations, attaches the first with `FUN_80196e0c`. `FUN_800748a8` then walks 0x13 joint paths from header[8]. |
| `FUN_80073cb8` | weapon ("rel" variant) | same relocation but table at **base + header[6]**; header[2] and header[4] += base; header[8] += base unless −1. Then for i < header[3]: loads the root joint of the i-th 0x10-byte JOBJDesc record at header[2], attaches its animation if present, and walks joint paths from header[8]. |

## Header

| Word | Robo / story character ("abs") | Weapon ("rel") | Source |
|---|---|---|---|
| 0 | data base, always 0x40 | same | verified (loaders) |
| 1 | file offset where trailer blocks A/B/C end | same | inferred (matches data; not read by these loaders) |
| 2 | offset from 0x40 → pointer to one **JOBJDesc** | offset from 0x40 → array of header[3] **JOBJDesc** records, 0x10 bytes each | verified (loaders) |
| 3 | file offset of block A | number of JOBJDesc records | verified (loaders) |
| 4 | file offset of block B | offset from 0x40 of block B | verified (loaders relocate it) |
| 5 | small value (1 in every robo seen) | −1 or small value | unknown (not read by these loaders) |
| 6 | file offset of reloc table | offset from 0x40 of reloc table | verified (loaders) |
| 7 | reloc entry count | same | verified (loaders) |
| 8 | file offset of block C (joint paths) | offset from 0x40 of block C, or −1 | verified (loaders) |
| 9 | 0, or file offset of block D | 0 / −1 | inferred (block D seen only in some `snr_chara`; no loader seen reading it) |
| 10–15 | 0 | 0 | seen as 0 in every entry checked |

JOBJDesc (HSD, 0x10 bytes): `{ RootJoint*, JointAnimations*, MaterialAnimations*, ShapeAnimations* }` — layout from HSDLib (`HSD_JOBJDesc`).

## Data block

Standard HSD structures, offsets per HSDLib (`HSDRaw\Common\*.cs`), confirmed by HSDLib decoding every entry:
- **JObj** (0x40): flags +0x04, child +0x08, next +0x0C, DObj +0x10 (unless SPLINE/PTCL flag), rotation / scale / translation floats +0x14…+0x34, inverse bind matrix +0x38.
- **DObj**: next +0x04, MObj +0x08, PObj +0x0C.
- **MObj**: render mode +0x04, TObj +0x08, material colours +0x0C.
- **TObj**: next +0x04, image +0x4C. **Image**: data +0x00, width +0x04 (u16), height +0x06 (u16), format +0x08 (CMPR = 14).
- **PObj**: vertex attribute list, display lists, envelope (skin weight) tables.

Example (robo 0, `chara\000.bin`): 55 joints, 18 DObjs, 52 PObjs, 2 CMPR textures.

## Trailer blocks

| Block | Where | What is known |
|---|---|---|
| A | abs: header[3] | 0x80 bytes of floats in robos. Purpose **unknown**. |
| B | header[4] | short byte sequences such as `02 fe`. Purpose **unknown**. |
| C | header[8] | **Joint paths** — verified from `FUN_800748a8`: bytes are read one at a time; `02` = go to child, `01` = go to next sibling, `ff` = end of one path. The robo loader resolves 0x13 (19) paths to joints and stores them. What the picked joints are used for is **not verified**. |
| D | header[9] (some `snr_chara`) | Increasing u32 values, runs to end of file. **Inferred** from data only. |

When re-importing, `crmodel.py from-dat` copies A/B/C/D unchanged and shifts their header offsets. Block C depends on the joint tree, so a model with a **different skeleton needs new joint paths** (not implemented).

## Conversion to a standard .dat (`tools\crmodel.py`)

- `to-dat`: standard HSD .dat = 0x20 header {file size, data size, reloc count, root count, 0} + the data block + reloc table + root table + names. Roots written:
  - `cr_header2` → the header[2] object (needed by `from-dat` to rebuild header word 2)
  - `root<i>_joint` → each root joint, reached exactly as the loaders reach it (`_joint` makes HSDRawViewer treat it as a JObj)
  - These names are **ours**; game entries carry no names.
- `from-dat`: rebuilds an entry from a .dat using the original entry as a template (header layout and trailer blocks). Trailer alignment: 0x20 if the original had padding before the trailer, otherwise 4. Entry end padded to 0x20 if the original was padded.
- `roundtrip`: entry → .dat → entry for every entry; currently 484/484 byte-identical.

## Open questions

- Blocks A, B, D and header word 5.
- What the 19 joint paths are used for (guess: attachment points for parts/weapons — **not verified**).
- Size / memory / vertex / bone limits for replacement models.
