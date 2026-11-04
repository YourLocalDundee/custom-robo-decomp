# Motion files: `logo_read_us.dbn` entry 8, `motion.DBN` and `snr_motion.BIN` (research notes, Nov 2, 2026)

The game's animation data. **Status (Nov 2, 2026): the container, the track layout and what every track means are
verified**, by the code that loads and plays them and by a check of every track. Tools convert motions both ways:
motion <-> JSON, byte-identical on all 51 files; motion -> glTF animation on any robo; glTF -> motion. **Still needed: a
human test in the game** (an imported motion played in Dolphin, in a battle); everything so far is checked by the
game's code ported to Python, not by the running game. How the game plays a motion and the tools:
[motion_playback.md](motion_playback.md).

**Nov 2, 2026, evening: the game never loads `motion.DBN`.** The battle and field motions it plays are the copy
inside `logo_read_us.dbn` (entry 8). The first in-game test edited `motion.DBN` only, so the stock poses played. The
tools and `make_mod.py` now write the bundle: [the full path](#the-full-path-disc-to-screen-and-mod-to-iso) below.

## Files

| File | What it is |
|---|---|
| `logo_read_us.dbn` (2,085,216 bytes) | **the one the game plays.** A boot bundle of 9 entries (below); entry 8 (+0x163120, 630,848 bytes) is a motion file, 3 groups: 168 + 94 + 101 = 363 motions. `logo_read_jp.dbn` has the same entry 8 byte for byte |
| `motion.DBN` (629,952 bytes) | a loose copy of the same 363 motions, **never read by the game** (no string names it in main.dol; Dolphin's file log of a boot to the title shows `logo_read_us.dbn` and `snr_motion.BIN`, not it). It differs from entry 8 in 2 motions (`sf_win_sf013_01`, `_02`): an older build's file |
| `snr_motion.BIN` (3,688,448 bytes) | an SFD archive ([sfd.md](sfd.md)) of 50 pieces; each piece is one motion file of 1 group (the scenario's own field motions: piece 0 has 119, piece 49 has 1) |

### The boot bundle `logo_read_us.dbn`

u32 count (9), then count + 1 u32 offsets from the file start (the last is the file length), zeros to 0x40; entries
0x20-aligned. Entries 0-7 are what `fn_80019E84` loads besides the motions (HSD archives through `fn_80074FBC`: the
joints at +8 and +0xC, more at +0x14 / +0x18, two blocks for 0x80386628; +4 and +0x10 are other data). Entry 8, the
last one, goes to `cr_MotionFile_Register`. Because it is last, a larger motion file only moves the end.

## Layout (verified)

From `cr_MotionFile_Register` (0x80074DCC), which walks a loaded motion file and registers it:

| Where | What |
|---|---|
| file +0 | u32 group count G |
| file +4 | u32 offset of each group (from the file start), G of them |
| group +0 | u32 motion count n |
| group +4 | n entries of 3 words (offsets from the group start; the loader relocates them): **tracks**, **shared**, **counts** |
| group +4 + 12n | n u16 motion IDs; bit 15 set = 78 tracks, clear = 69 (the loader relocates 0x4E or 0x45 pointer pairs) |

The loader stores per motion ID (bits 0-14) a pointer to the entry in `cr_g_MotionData` and bit 15 in
`cr_g_MotionDataBit15`; an ID already registered is skipped and its u16 is overwritten with 0xFFFF.
`cr_Motion_LoadInfo` (0x80037A80) later copies the entry's three words to the playing object (+0x6DC / +0x6E0 / +0x6E4,
reached through `cr_g_CurrentChara_06dc`). Every motion in both files has bit 15 set (78 tracks).

Per entry:

- **tracks**: 78 (or 69) pairs of offsets (time list, value list); both 0 for a track without keys.
- **counts**: one byte per track: its key count (0 for an empty track).
- **shared**: the same offset for every motion of a group; it points to floats (group 0: 0, 95.24, 0, 0.14, -1.22, ...).

A time list is `count` u16 frame numbers starting at 0, non-decreasing (a repeated frame is an instant change: 61
tracks); several tracks share one time list. A value list is `count` u16 values, one per key. **Checked**: in
`motion.DBN` all 28,314 tracks of the 363 motions agree with this (empty tracks have count 0 and null offsets; every
other time list starts at 0 and never goes down); the longest motion's last key is frame 330.

## What the values mean (verified from the player code; details in [motion_playback.md](motion_playback.md))

- **Not** 26 joints x 3 rotation axes. The 78 tracks are: 7 IK points (tracks 0, 9, 15, 24, 36, 48, 60; x, y, z
  each: the root, the upper body and head targets, two hands, two feet), 16 rotation joints of the game's default
  skeleton (x, y, z in degrees), and 9 root tracks (69-71 translation, 72-74 rotation in degrees, 75-77 a vector
  turned by the root rotation). The 69-track form has no root tracks.
- A value word is **not** an angle. It is a 2-bit key kind and a 14-bit index into **shared**: the group's float
  pool. The kinds are value + in-tangent + out-tangent; zero; value with flat tangents; value with one tangent.
- Between keys the curve is a cubic Hermite (`cr_Motion_SampleTrack` 0x8001EB48). Tangents are per second, at 30
  frames a second.
- **shared** is that float pool: one per group, de-duplicated, at most 16,384 floats (14-bit index). Group 0 of
  motion.DBN uses 13,867.
- The game builds the body from the tracks by IK, two-bone arms and legs (`cr_Chara_PoseLimbs`). It writes 19 world
  matrices onto the model's role joints (`cr_Battle_ApplyRoleMatrices`). A motion therefore plays on any robo,
  rebuilt with that robo's own limb lengths.
- The frame count is in `cr_g_MotionInfo[id]` (+4: u16 & 0xFFF; step 46) and the names in `cr_g_MotionNames`.
- The boot motion file (entry 8; same group split as `motion.DBN`) group 0 holds the battle motion IDs 0x001..0x0A8
  (sf_ / mg_ / ab_), groups 1 and 2 more battle and field IDs; `snr_motion.BIN` holds field motions (IDs from 0x1C1)
  per scenario.

## The full path: disc to screen, and mod to ISO

Found Nov 2, 2026 (evening) after the first in-game test showed stock poses. Each step names the code or the check
that shows it.

### In the game

| # | Step | Where |
|---|---|---|
| 1 | At boot, the heap block for loaded files is cut into regions; `lbl_8044F334` = base + 0x40000, the next region at base + 0x2F6000: **the bundle can be up to 0x2B6000 = 2,842,624 bytes** | `fn_80023CBC` |
| 2 | `cr_Archive_OpenEntry(0, 0)`: file descriptor slot 0 (`cr_g_FileDescs` 0x801F1200 -> 0x801F21E0, a list of loose files), entry 0 = `logo_read_us.dbn`; its size comes from the disc at run time (the descriptor's size word is 0 in main.dol, unlike chara.BIN's entry table) | `fn_80019E84`, [decomp.md](../decomp.md) |
| 3 | `cr_FileSlot_QueueRead` reads the whole file to `lbl_8044F334` | `fn_80019E84` |
| 4 | When the read is done: entries 0-7 set up (HSD archives, joints), then `cr_MotionFile_Register(bundle + offset[8])` | `fn_80019E84` |
| 5 | Per motion ID (bits 0-14): `cr_g_MotionData[id]` = pointer to its entry (tracks, shared, counts), bit 15 in `cr_g_MotionDataBit15`. **The first file to register an ID wins**; a later file's copy of it is skipped. Later calls register other files (`fn_800A0204`: `lbl_8044F33C`, the story's `snr_motion.BIN` pieces) | `cr_MotionFile_Register` 0x80074DCC |
| 6 | In a battle, a robo's body number picks its motion set: `cr_g_BattleMotionSets` (0x801C51B8, 34 sets x 38 slots of motion IDs); `fn_8004B0A0` plays `set[cr_g_BattleMotionSlot]` (run, gun, jump, ...) | [robo_tables.md](../robo_tables.md) table 4, plan.md step 46 |
| 7 | `cr_Motion_Play` -> `cr_Motion_LoadInfo` 0x80037A80: the entry's 3 words to the playing object (+0x6DC / +0x6E0 / +0x6E4); frame count from `cr_g_MotionInfo[id]` | this page |
| 8 | Every frame: root, blend, the 7 IK points and 16 joint angles, joint rotations, IK limbs with the robo's own lengths, 19 world matrices on the model's role joints | [motion_playback.md](motion_playback.md) |

So the motions on screen are entry 8 of `logo_read_us.dbn` (steps 2-5), never `motion.DBN`. Checked: main.dol has no
string `motion.DBN`; Dolphin's FileMonitor log of a boot (tools/boot_check.ps1 -Log) reads `logo_read_us.dbn` twice
and `snr_motion.BIN`, no `motion.DBN`.

### For a mod

| # | Step | Command / code |
|---|---|---|
| 1 | Read the motions the game plays | `crmotion.read_motion("…/files/logo_read_us.dbn")` (any `crmotion.py` / `crmotion_rig.py` command takes the bundle as its motion file); `crmotion.py boot-extract logo_read_us.dbn out.bin` writes entry 8 alone |
| 2 | Edit: JSON, a script (`crmotion.sample` + `crmotion_rig.reduce_keys`), or Blender glTF (`crmotion_rig.py gltf` / `import-gltf`) | [MODDING.md](../MODDING.md#motions-animations) |
| 3 | Write the edited motion file as `mods/<mod>/files/motion.DBN` (the name stays as the mod's way to say "the motion file") | e.g. `mods/atlas_ninja_run/make_motion.py` |
| 4 | `make_mod.py` puts it into a copy of the clean `logo_read_us.dbn` as entry 8 (`crmotion.boot_replace`: padded to 0x20, the end offset moved; refuses a bundle over 2,842,624 bytes). The loose `motion.DBN` on the disc stays as it is. Or give `files/logo_read_us.dbn` yourself (`crmotion.py boot-pack`); not both | `tools/make_mod.py` step 2 |
| 5 | `iso_patch.py build` writes the bundle in place or moves it to free disc space and updates the FST; no main.dol table to sync (step 2 above) | `tools/iso_patch.py` |
| 6 | Check the ISO: read `logo_read_us.dbn` back through the FST, entries 0-7 byte-identical to the clean file, entry 8 = the mod's file; boot it (`pwsh tools/boot_check.ps1 <iso> 35 -Log x.log -PressA 16`: no warning, the log reads the new size) | done for atlas_ninja_run, Nov 2 |
| 7 | A battle in Dolphin: the edited poses show | **partly**: the march and dash show (Nov 2); the gun motions still break the battle |

## Tools (tools/crmotion.py, tools/crmotion_rig.py)

| Direction | Command | Checked |
|---|---|---|
| motion -> JSON -> motion | `crmotion.py export` / `import` / `roundtrip` | 51 of 51 files byte-identical; edited keys import; entry 8 of logo_read_us.dbn byte-identical too |
| motion -> glTF animation on a robo | `crmotion_rig.py gltf` | rendered on robos 0, 5, 30 (`tools/render_motion.py`) |
| glTF animation -> motion | `crmotion_rig.py import-gltf` | re-posed within 0.0034 of the original on 3 robos; a hand-made animation within 0.0016 |
| motion file <-> boot bundle | `crmotion.py boot-extract` / `boot-pack`; `make_mod.py` packs `files/motion.DBN` | the rebuilt Atlas ISO: entries 0-7 unchanged, entry 8 = the mod's file, boots to the title |
| in the game | (a mod with edited motions, then a battle in Dolphin) | **partly (Nov 2, 2026, owner's battles)**: edited motions in logo_read_us.dbn play in game; the Atlas march and air dash play; the gun motions' butt shot blacks the 3D screen (being narrowed down, mods/atlas_ninja_run/notes.txt). Rules learned: every track ends on its motion's last frame; leave root and joint 0 tracks alone ([motion_playback.md](motion_playback.md) "Authoring notes") |
