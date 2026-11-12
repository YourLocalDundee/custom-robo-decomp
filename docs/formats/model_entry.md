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
+0x8F0..+0x908. Leg roots 0 and 2 are never part of the battle set (they are the Customize screen's form 0). **A second set**: `fn_800A5C60` (C at 100%) copies
gun root 0, **bomb root 0**, pod roots 0 / 1 and **leg roots 0..3** into `lbl_803BFC78` (+0x1AC..+0x1B8, its header[4]
joint at +0x1BC): the story-mode robos (see "Where each set is used" below). `fn_8010D2D0` releases the roots
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

**Two forms per part, and who switches them.** `fn_80007C74` (gun), `fn_80007C24` (bomb), `fn_80007BD4` (pod) and
`fn_80007AE4` (legs), all C at 100%, take 0 or 1 and point the battle robo's active roots at one form:

| Part | Form 0 | Form 1 | Battle start (`FUN_80101950`) | Switched by |
|---|---|---|---|---|
| gun | root 0 (+ joint picks 0, 1) | root 1 (+ picks 2, 3) | form 0 | `FUN_80042AB8` |
| bomb | root 0 (+ pick 4) | root 1 (+ pick 5) | form 1 | `FUN_8003FE98` |
| pod | roots 0 + 1 | roots 2 + 3 | form 0 | `FUN_8003DD38` |
| legs | roots 0 + 2 (+ picks 6..9) | roots 4 + 5 (+ picks 10..13) | form 1 | `FUN_8003D500` |

(leg roots 1 and 3 are active in both forms.) `FUN_801076D8` (called by `cr_SEQ_CUSTOMIZE_FrameA`) puts all four in form 0, and `FUN_80101950` (called by
`cr_SEQ_CUSTOMIZE_Leave`) sets the battle-start forms. Each switcher keeps a float per
part (gun state +0x2B4, bomb +0x1B0, pod +0x190, legs +0xB4), clamped to 0..1, and chooses **form 1 when it is exactly
1.0, form 0 otherwise**; for the legs that float follows a target 0.05 per frame (20 frames from one end to the
other), and a pod in some states (battle robo +0x1F4 & 0x30) is forced to form 0. **The legs and the bomb then call
their switch with 1 again, unconditionally**: in battle the legs always show roots 4 + 5 (with 1 + 3) and the bomb
root 1. Leg roots 0 / 2 and bomb root 0 (the big animated ones) are not shown in battle; they are in the second set
(lbl_803BFC78, `fn_800A5C60`), which belongs to the story-mode characters (see "Where each set is used" below).

**How the parts are placed on the body** (`fn_80039FAC`, every frame). The body is not moved by its own HSD animation
alone: the game's motion code (the 78 tracks, docs/formats/motion_playback.md) computes 20 bone matrices
(battle robo +0x1434, 0x30 each), and the loop writes matrix i+1 into **role joint i** (19 roles, the joints of
block C, kept at +0x864) with `PSMTXCopy` and sets the joint flags 0x3800000 (the matrix is used as given). Then each
active part root gets its matrix the same way, from the game's matrix stack: load a role's matrix, translate by the
body's offset for that part, scale by 0.1, scale by the body's vector (both from `cr_g_BodyData`, symbols/cr_types.h
`cr_BodyData`; the table per body is in [robo_tables.md, "Part mounts per body"](../robo_tables.md#part-mounts-per-body-cr_g_bodydata-nov-5-2026)):

| Part root | Role joint (block C) |
|---|---|
| gun | 8, arm_b_fore |
| bomb | 4, arm_a_fore |
| both pods | 0, upper_body (offset A / B) |
| legs +0x954 / +0x958 | 16 / 17, leg_b_shin / leg_b_ankle |
| legs +0x95C / +0x960 | 12 / 13, leg_a_shin / leg_a_ankle |

Only the shin roots take the leg offset. The 0.1 means the parts are modelled at ten times the scale of the role
matrices: a part exported from its file and placed on a robo needs that factor.

**How the parts animate.** A part root's joint animation (the one the loader attached) is not played by time: the game
sets its frame with `FUN_80095700(frame, root)` when a state float changes, **frame = 10 x the float** (gun: battle
robo +0x20D4; legs: +0x333B4), so each part animation is an 11-frame pose (frames 0..10) driven by a 0..1 value.
The body's own HSD animation (+0x860) is set the same way (frame 0, or +0x550 in some states).

**Joint picks are effect anchors.** `FUN_8003D500` copies the matrices of the leg roots' picked joints (+0x8FC..+0x908)
into the joints of four effect objects (battle robo +0x333E0..+0x333EC, each object's joint at +0x90) while the leg
value is above 0: the header[8] joint picks of a part file are anchor points for effects (which effect: open).

**Questions a model parser runs into, answered from the code** (Nov 5, 2026):

- *Which "draw groups" are drawn?* The root records the battle robo's active slots point at (the forms table above);
  the choice is per part type and state, not per robo body. What is per body is the mount (cr_g_BodyData).
- *Why does a part with one PObj come out right and one with many PObjs garbled?* Every part PObj is rigid; its
  vertices are in its joint's space. Apply each joint's world matrix (parents first, scale / rotation / translation of
  the joint, HSD's order) before placing the vertices.
- *Is anything mirrored?* No mirroring at run time: every scale in cr_g_BodyData is positive, and the left / right
  pods and legs are separate roots in the file, each with its own joints (pod roots 0 / 1 on the same role joint,
  told apart by the offsets A / B; leg roots on the two leg chains).
- *Why does the standard pod show extra geometry on one side?* Roots 0 and 1 are the two pods (form 0); drawn together
  without their mounts they overlap. The small roots 2 / 3 are the other form, not part of the same view.
- *Why are only some leg pieces ever drawn?* In battle the legs are forced to form 1 (roots 4 + 5, with 1 + 3); roots
  0 / 2 are the Customize display models
  (form 0) and are also in the story-mode set.

**Code status of this chain (priority list for the decomp; `draft_report.txt`, Nov 5)**. Matched and linked from C:
`fn_80007CE4`, the four switches `fn_80007AE4 / BD4 / C24 / C74`, `fn_8003D500` (legs), `fn_800A5C60` (second set),
`fn_8010D2D0` (release), `fn_80095700` (set frame), `fn_80095A30` (matrix update), `cr_BattleRobo_InitBody`.
Matched, not linked: `fn_80038CF8` (draw). **Hand-matched Nov 5 (tools/hand_drafts, 100%, not yet in a full run)**:
the matrix stack steps `fn_8001DF6C` (load) / `fn_8001DD68` (translate) / `fn_8001D9E8` (scale) / `fn_8001DA70`
(scale xyz), `fn_80101950` (the battle-start forms), `cr_BattleRobo_InitFromMenu` (the copy of the model slot) and
`cr_Chara_BuildModel` (the robo model, the 19 role joints, the material and the extra joint paths): these decisions
are now read from C that compiles to the original. **Not matched yet, still read from m2c / Ghidra output** (in order
of how much of this write-up rests on them; wip drafts in tools/sched/scratch/hand/wip):

| Function | Bytes | Draft | What it decides |
|---|---|---|---|
| `fn_80039FAC` | 3060 | 93% (wip, Nov 6; registers only) | role matrices into the joints, part placement, part animation frames |
| `cr_WeaponModel_OnLoaded` | 1112 | 97% (wip, Nov 6; three saved registers swapped) | part records, slot table, joint picks |
| `fn_80095E6C` | 1288 | 96% (wip; stack slots only) | drawing a joint tree with a matrix |
| `fn_80042AB8` | 5636 | 56% (wip, Nov 6; frame and stack slots match, 94.7% with registers normalized) | the gun's state float and form |
| `fn_8003FE98` | 4648 | 71% (wip, Nov 6; frame and stack slots match, 97.3% with registers normalized) | the bomb's state float and form |
| `fn_8003DD38` | 4808 | 33% (wip, Nov 6: 79.3% normalized; frame 16 bytes short) | the pods' state float and form |
| `fn_801076D8` | 2904 | 44% | all forms 0 (which mode: open); blocked: it reads its file's data through the pool base (r27 = lbl_80238008, the tables at +0x38C8 / +0x3C20 / +0x3C30 are other objects of the same file), so it can only match in a file object that defines that data |
| `cr_BattleRobo_InitFromLoadout` | 2228 | ~50% | the copy of cr_g_BodyData (mounts) |
| `fn_800A63C0` | 15404 | 3% (Nov 6: m2c drafts it since draft_batch.py rewrites its stack-spilled switch; compiles) | the screen that reads the second set |

**Where each set is used** (`tools/callmap.py`, from Ghidra's call references):

- **Battle**: `cr_SEQ_BATTLE_FrameA` -> `cr_Battle_UpdateRobos` (34%) -> the four part updaters (gun `FUN_80042AB8`,
  bomb `FUN_8003FE98`, pods `FUN_8003DD38`, legs `FUN_8003D500`); `cr_SEQ_BATTLE_FrameB` -> `cr_Battle_ApplyRoleMatrices`
  (C) -> `fn_80039FAC` (placement).
- **Customize**: `cr_SEQ_CUSTOMIZE_FrameA` -> `FUN_801076D8` (every part in form 0: the big animated roots gun 0, bomb 0,
  pods 0 + 1, legs 0 + 2, i.e. the full display models); `cr_SEQ_CUSTOMIZE_FrameB` -> `FUN_80101284` -> `fn_80039FAC`;
  `cr_SEQ_CUSTOMIZE_Leave` -> `FUN_80101950` (back to the battle-start forms).
- **Story mode**: `cr_SEQ_SCENARIO_FrameA` -> `cr_Chara_UpdateAll` (C) -> `FUN_800A63C0` (15 KB, m2c fails) ->
  `fn_800A5C60` (the second set: gun 0, bomb 0, pods 0 + 1, legs 0..3) and `fn_80039FAC`; the part files are loaded
  from there too (`cr_WeaponModel_OnLoaded` is its load callback).

**What moves the state floats.** Gun (`FUN_80042AB8`, gun state +0x2B4): + 1/60 every frame, clamped to 0..1, so form 1
(root 1) after 60 frames unless something sets it back (the reset is not in this function: open). Pods
(`FUN_8003DD38`, +0x190): + 1/60 per frame while the pods in flight (+0x188, counted down when one ends) are fewer
than the robo's pod count (+0xE9), - 1/60 otherwise; battle robo flags +0x1F4 & 0x30 force 0 and form 0. Legs: + or -
0.05 per frame toward a target (+0xA4). Bomb (+0x1B0): eased the same way, then overridden to form 1.

**Open**: what the gun's form 1 looks like in play (root 1, 6 joints, 1 PObj) and what resets its float; the bomb pick's and the gun picks' users; record[2] / record[3].

## Open questions

- The meaning of each block A float; what the block B material is used for; header word 5.
- Which game code uses each of the 19 role joints (beyond the limb code), and which side is left/right.
- Size / memory / vertex / bone limits for replacement models.
