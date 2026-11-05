# Model entry format (robo, weapon, story character)

Used by entries of `chara.BIN`, `weapon_*.BIN` and `scenario_*/snr_chara.BIN`.

**Sources**: the game's own loaders in `decompiled/main_dol.c` (addresses below), cross-checked by HSDLib parsing and exporting every entry, and by `tools/crmodel.py roundtrip` (484/484 entries byte-identical).

## Summary

An entry is HAL Laboratory **sysdolphin (HSD)** model data — the same engine library and structures as Super Smash Bros. Melee — behind a game-specific 0x40-byte header instead of the standard HSD .dat header.

```
0x00  header (16 × u32, big-endian)
0x40  data block: HSD structures; every pointer is an offset from 0x40
....  reloc table: u32 offsets (from 0x40) of every pointer slot in the data block
....  trailer blocks A, B, C (and D in some story characters)
```

## Loaders (verified by reading the decompiled code)

Names are today's (`cr_` from `symbols/descriptive.txt`, `HSD_` from `symbols/symbols.txt`, each with its evidence); `FUN_` = not named yet. Offsets written `cr_Chara+0x…` are into the character object that builds the model (`cr_Chara`, a story-mode field character, plan.md steps 39-41; older notes called it "robo").

| Function | Loads | What it does |
|---|---|---|
| `cr_CharaEntry_OnLoaded` (0x800747B0) | robo ("abs" variant) | base = entry + header[0]; for i < header[7]: `*(base + table[i]) += base` with table at **entry + header[6]**; header[2] += base; header[3], [4], [8] += entry. Then only stores the entry in `cr_g_CharaEntries[robo]` (loaded = 1). **Correction (Oct 27, 2026):** it does not build the skeleton; `cr_Chara_BuildModel` does. |
| `cr_Chara_BuildModel` (0x80074110) | robo, when one is set up | loads the root joint `**header[2]` (`HSD_JObjLoadJoint`), attaches its first joint animation (`HSD_JObjAddAnimAll`), resolves blocks B, C (and D) to joints of the new tree and keeps block A; see "Trailer blocks". |
| `cr_ModelEntry_LoadAndBuild` (0x800748A8) | story character ("abs") | same relocation, then loads the root joint and animation itself and walks its own joint paths. |
| `cr_WeaponModel_OnLoaded` (0x80073CB8) | weapon ("rel" variant) | same relocation but table at **base + header[6]**; header[2] and header[4] += base; header[8] += base unless −1. Then for i < header[3]: loads the root joint of the i-th 0x10-byte JOBJDesc record at header[2], attaches its animation if present, and walks joint paths from header[8]. |

## The game keeps its own copy of each archive's entry table

Every packed archive (chara, weapon_*, *_effect, snr_chara, snr_obj, rpg_*_models: 13 in all) has a copy of its entry table (count, 0, then offset/size per entry) inside main.dol, pointed to by its file descriptor (`cr_g_FileDescs[i]->packed`). The game reads that copy, not the archive header. Verified: all 13 copies equal the disc headers. **A mod that changes any entry's size must update the copy** - `tools/make_mod.py` does it (`sync_entry_table`). Found Oct 27, 2026: a size-changing mod without it gave "The Game Disc could not be read" and invalid-read crashes. Loose files store size 0 there (the game asks the disc), so they are not affected.

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

Standard HSD structures, offsets per HSDLib (`HSDRaw/Common/*.cs`), confirmed by HSDLib decoding every entry:
- **JObj** (0x40): flags +0x04, child +0x08, next +0x0C, DObj +0x10 (unless SPLINE/PTCL flag), rotation / scale / translation floats +0x14…+0x34, inverse bind matrix +0x38.
- **DObj**: next +0x04, MObj +0x08, PObj +0x0C.
- **MObj**: render mode +0x04, TObj +0x08, material colours +0x0C.
- **TObj**: next +0x04, image +0x4C. **Image**: data +0x00, width +0x04 (u16), height +0x06 (u16), format +0x08 (CMPR = 14).
- **PObj**: vertex attribute list, display lists, envelope (skin weight) tables.

Example (robo 0, `chara/000.bin`): 55 joints, 18 DObjs, 52 PObjs, 2 CMPR textures.

## Trailer blocks

| Block | Where | What is known |
|---|---|---|
| A | header[3] | **Limb measurements**: 0x80 bytes = 0x20 floats. Kept at cr_Chara+0xB3C, copied to `cr_g_CurrentCharaLimbParams` and read only by the procedural limb code (`cr_Chara_PoseLimbs`, `FUN_80058ad0`, `FUN_8005299c`, `FUN_8003c4f0`): [0], [1], [0xE] are scaled by the robo scale and used as lengths; [2..7] and [8..13] feed the two symmetric calls of one limb pair, [0xF..0x14] and [0x15..0x1A] the other pair; [0x13] and [0x19] are compared with heights. Exact meaning of each float: open. |
| B | header[4] | **One joint path ending in `FE`** (same 02/01 bytes as C). `cr_Chara_BuildModel` keeps that joint's material (`jobj->dobj->mobj`) at cr_Chara+0xBC0. Usually `02 fe` = joint 1. What the material is used for: open. |
| C | header[8] | **19 joint roles** - `02` = child, `01` = next sibling, `ff` = end of one path (`ff` alone = no joint). `cr_Chara_BuildModel` stores the joints at cr_Chara+0xB44 + 4*role. Roles from robo 0's bind pose (table below). `tools/robo_joints.py` decodes and re-encodes them; re-encoding is byte-identical for all 170 robo entries. |
| D | header[9] | 0 in every robo. When non-zero `cr_Chara_BuildModel` reads more joint paths from entry + header[9] + 0x60: 6 for robos 0 and 0xD, 2 x 6 for robo 2 (cr_Chara+0xB90...). In some `snr_chara` (read by their own loader). |

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

## Conversion to a standard .dat (`tools/crmodel.py`)

- `to-dat`: standard HSD .dat = 0x20 header {file size, data size, reloc count, root count, 0} + the data block + reloc table + root table + names. Roots written:
  - `cr_header2` → the header[2] object (needed by `from-dat` to rebuild header word 2)
  - `root<i>_joint` → each root joint, reached exactly as the loaders reach it (`_joint` makes HSDRawViewer treat it as a JObj)
  - These names are **ours**; game entries carry no names.
- `from-dat`: rebuilds an entry from a .dat using the original entry as a template (header layout and trailer blocks). Trailer alignment: 0x20 if the original had padding before the trailer, otherwise 4. Entry end padded to 0x20 if the original was padded.
- `roundtrip`: entry → .dat → entry for every entry; currently 484/484 byte-identical.

## Robo size

The game draws every robo at the same scale: the factor the limb code multiplies block A by (cr_Chara+0x3C0) is set to 1.0 for every robo (FUN_800ba488), and every robo model's root joint has scale (1, 1, 1). A robo's size is in its vertices. Setting the root joint scale (HSD_Joint +0x20) scales the drawn model (`tools/robo_swap.py --size`). **Tested in game: no visible effect** - the root scale is overridden at run time, most likely by the joint animations.

## Part files: root joints ("draw groups"), which ones the game uses, and why a part can come out garbled (Nov 5, 2026)

`weapon_gun` / `weapon_bom` / `weapon_pod` / `weapon_leg` entries (the "rel" variant) hold **several root joints**:
header[2] is an array of header[3] records of 0x10 bytes, and `cr_WeaponModel_OnLoaded` (0x80073CB8) does, for each
record i: `jobj = HSD_JObjLoadJoint(record[0])`; when record[1] is not 0, `HSD_JObjAddAnimAll(jobj, *record[1], NULL,
NULL)` (record[1] points to a list of joint animations, the first is used) and `FUN_80196c50(0.0, jobj)` (laid out
right after `HSD_JObjReqAnimAllByFlags`, where sysdolphin has `HSD_JObjReqAnimAll`: start the animation at frame 0).
record[2] / record[3] are not read here (0 in the files checked). Community parsers call these root joints "draw
groups"; the numbers below are the record index.

**Where they go.** The loader files each part's roots into a per-player table, `lbl_80377C60` (.bss, 0x80 bytes per
player, 4 players), by part type (the low half of the request word; the high half is the player):

| Type | Part | Roots (slots) | Table offset | header[4] joint at | header[8] joint picks at |
|---|---|---|---|---|---|
| 0 | gun | 2 | +0x00 | +0x38 | +0x48 (4) |
| 1 | bomb | 2 | +0x08 | +0x3C | +0x58 (2) |
| 2 | pod | 4 | +0x10 | +0x40 | none |
| 3 | legs | 6 | +0x20 | +0x44 | +0x60 (8) |

**The battle set.** `fn_80007CE4` (C at 100%, tools/hand_drafts) copies the 14 roots of the current player to the
battle robo (cr_g_CurrentBattleRobo+0x90C, in the order of the table) and then names the ones the robo uses:
**gun root 0** (+0x944), **bomb root 1** (+0x948), **pod roots 0 and 1** (+0x950 / +0x94C), **leg roots 4 / 1**
(+0x954 / +0x958) and **5 / 3** (+0x95C / +0x960); the four header[4] joints to +0x964..+0x970 and some joint picks to
+0x8F0..+0x908. Leg roots 0 and 2 are never part of the battle set. **A second set**: `fn_800A5C60` (C at 100%) copies
gun root 0, **bomb root 0**, pod roots 0 / 1 and **leg roots 0..3** into `lbl_803BFC78` (+0x1AC..+0x1B8, its header[4]
joint at +0x1BC): another view of the parts (which screen: open). `fn_8010D2D0` releases the roots
(`HSD_JObjUnref`) and clears the battle robo's copies.

**What is in them** (`tools/sched/scratch/hand/hsdparts.py` on `models_dat/`, the first entry of each file):

| Part (entry 0) | Root: joints / PObjs / joint animation |
|---|---|
| gun | 0: 21 / 12 / yes; 1: 6 / 1 / no |
| bomb | 0: 18 / 12 / yes; 1: 5 / 1 / no |
| pod | 0: 13 / 9 / yes; 1: 13 / 9 / yes; 2: 4 / 1 / no; 3: 4 / 1 / no |
| legs | 0: 15 / 6 / yes; 1: 5 / 1 / no; 2: 15 / 6 / yes; 3: 5 / 1 / no; 4: 3 / 1 / no; 5: 3 / 1 / no |

**Every PObj in these parts is rigid** (PObj flags & 0x3000 = 0: no envelope / skin weights, no shape animation):
its vertices are in the local space of the joint that owns its DObj. Most of the joints of the big roots are moved
(gun root 0: 18 of 21 joints have a rotation, scale or translation that is not identity). **A parser that places the
vertices without each joint's world matrix** (the parent chain of scale, rotation, translation) gets the single-PObj
roots about right (one joint, close to identity) and the many-PObj roots garbled: the pieces pile up near the origin
and turn. Applying the joint matrices is the standard HSD model transform, which HSDRawViewer's glTF export does (the part
exports in models_export/ come from `tools/crmodel.py to-dat` + HSDRawViewer; not yet compared with in-game views here). The big roots also carry a joint animation started at frame 0: their rest pose is the
joints' own transforms, and the animation moves them from there.

**Open**: what the second set (lbl_803BFC78) shows; why the bomb uses root 1 in battle and root 0 there; what
record[2] / record[3] are when they are not 0; the meaning of each joint pick (the gun's 4, the legs' 8).

## Open questions

- The meaning of each block A float; what the block B material is used for; header word 5.
- Which game code uses each of the 19 role joints (beyond the limb code), and which side is left/right.
- Size / memory / vertex / bone limits for replacement models.
