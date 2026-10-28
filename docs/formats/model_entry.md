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
| `cr_RoboModel_OnLoaded` (0x800747B0) | robo ("abs" variant) | base = entry + header[0]; for i < header[7]: `*(base + table[i]) += base` with table at **entry + header[6]**; header[2] += base; header[3], [4], [8] += entry. Then only stores the entry in `cr_g_RoboEntries[robo]` (loaded = 1). **Correction (Oct 27, 2026):** it does not build the skeleton; `cr_Robo_BuildModel` does. |
| `cr_Robo_BuildModel` (0x80074110) | robo, when one is set up | loads the root joint `**header[2]` (`FUN_801988fc`), attaches its first joint animation (`FUN_80196e0c`), resolves blocks B, C (and D) to joints of the new tree and keeps block A; see "Trailer blocks". |
| `cr_CharaModel_OnLoaded` (0x800748A8) | story character ("abs") | same relocation, then loads the root joint and animation itself and walks its own joint paths. |
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
| A | header[3] | **Limb measurements**: 0x80 bytes = 0x20 floats. Kept at robo+0xB3C, copied to `cr_g_CurrentLimbParams` and read only by the procedural limb code (`FUN_8005aee8`, `FUN_80058ad0`, `FUN_8005299c`, `FUN_8003c4f0`): [0], [1], [0xE] are scaled by the robo scale and used as lengths; [2..7] and [8..13] feed the two symmetric calls of one limb pair, [0xF..0x14] and [0x15..0x1A] the other pair; [0x13] and [0x19] are compared with heights. Exact meaning of each float: open. |
| B | header[4] | **One joint path ending in `FE`** (same 02/01 bytes as C). `cr_Robo_BuildModel` keeps that joint's material (`jobj->dobj->mobj`) at robo+0xBC0. Usually `02 fe` = joint 1. What the material is used for: open. |
| C | header[8] | **19 joint roles** - `02` = child, `01` = next sibling, `ff` = end of one path (`ff` alone = no joint). `cr_Robo_BuildModel` stores the joints at robo+0xB44 + 4*role. Roles from robo 0's bind pose (table below). `tools/robo_joints.py` decodes and re-encodes them; re-encoding is byte-identical for all 170 robo entries. |
| D | header[9] | 0 in every robo. When non-zero `cr_Robo_BuildModel` reads more joint paths from entry + header[9] + 0x60: 6 for robos 0 and 0xD, 2 x 6 for robo 2 (robo+0xB90...). In some `snr_chara` (read by their own loader). |

When re-importing, `crmodel.py from-dat` copies A/B/C/D unchanged and shifts their header offsets - right as long as the skeleton is unchanged. For a **different skeleton** `tools/make_mod.py` rebuilds B and C from a roles file (`tools/robo_joints.py write_paths`) and can take block A from another robo; see docs/MODDING.md.

### Joint roles (block C)

| Role | Name in roles.json | Robo 0 joint | Seen as |
|---|---|---|---|
| 0 | upper_body | 28 | upper body root |
| 1 | head | 32 | above the torso |
| 2-5 | arm_a_shoulder, arm_a_upper, arm_a_fore, arm_a_hand | 35, 37, 39, 41 | arm chain on the -X side |
| 6-9 | arm_b_shoulder, arm_b_upper, arm_b_fore, arm_b_hand | 45, 47, 49, 51 | arm chain on the +X side |
| 10 | lower_body | 4 | hips / lower body root |
| 11-14 | leg_a_thigh, leg_a_shin, leg_a_ankle, leg_a_foot | 8, 10, 13, 15 | leg chain on the -X side (down, then forward to the toe) |
| 15-18 | leg_b_thigh, leg_b_shin, leg_b_ankle, leg_b_foot | 18, 20, 23, 25 | leg chain on the +X side |

Source: joint positions of robo 0's bind pose (glTF export; joint numbers verified equal to HSDRawViewer's JOBJ_n by comparing all 55 parents). Every robo has the same pattern with its own joint numbers (51-69 joints). Which side is the robo's left/right is **not verified** (hence a/b).

## Conversion to a standard .dat (`tools\crmodel.py`)

- `to-dat`: standard HSD .dat = 0x20 header {file size, data size, reloc count, root count, 0} + the data block + reloc table + root table + names. Roots written:
  - `cr_header2` → the header[2] object (needed by `from-dat` to rebuild header word 2)
  - `root<i>_joint` → each root joint, reached exactly as the loaders reach it (`_joint` makes HSDRawViewer treat it as a JObj)
  - These names are **ours**; game entries carry no names.
- `from-dat`: rebuilds an entry from a .dat using the original entry as a template (header layout and trailer blocks). Trailer alignment: 0x20 if the original had padding before the trailer, otherwise 4. Entry end padded to 0x20 if the original was padded.
- `roundtrip`: entry → .dat → entry for every entry; currently 484/484 byte-identical.

## Open questions

- The meaning of each block A float; what the block B material is used for; header word 5.
- Which game code uses each of the 19 role joints (beyond the limb code), and which side is left/right.
- Size / memory / vertex / bone limits for replacement models.
