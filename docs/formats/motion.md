# Motion files: `motion.DBN` and `snr_motion.BIN` (research notes, Nov 2, 2026)

The game's animation data. **Status (Nov 2, 2026): the container, the track layout and what every track means are
verified**, by the code that loads and plays them and by a check of every track. Tools convert motions both ways:
motion <-> JSON, byte-identical on all 51 files; motion -> glTF animation on any robo; glTF -> motion. **Still needed: a
human test in the game** (an imported motion played in Dolphin, in a battle); everything so far is checked by the
game's code ported to Python, not by the running game. How the game plays a motion and the tools:
[motion_playback.md](motion_playback.md).

## Files

| File | What it is |
|---|---|
| `motion.DBN` (629,952 bytes) | one motion file, 3 groups: 168 + 94 + 101 = 363 motions |
| `snr_motion.BIN` (3,688,448 bytes) | an SFD archive ([sfd.md](sfd.md)) of 50 pieces; each piece is one motion file of 1 group (the scenario's own field motions: piece 0 has 119, piece 49 has 1) |

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
- `motion.DBN` group 0 holds the battle motion IDs 0x001..0x0A8 (sf_ / mg_ / ab_), groups 1 and 2 more battle and
  field IDs; `snr_motion.BIN` holds field motions (IDs from 0x1C1) per scenario.

## Tools (tools/crmotion.py, tools/crmotion_rig.py)

| Direction | Command | Checked |
|---|---|---|
| motion -> JSON -> motion | `crmotion.py export` / `import` / `roundtrip` | 51 of 51 files byte-identical; edited keys import |
| motion -> glTF animation on a robo | `crmotion_rig.py gltf` | rendered on robos 0, 5, 30 (`tools/render_motion.py`) |
| glTF animation -> motion | `crmotion_rig.py import-gltf` | re-posed within 0.0034 of the original on 3 robos; a hand-made animation within 0.0016 |
| in the game | (a mod with an imported motion, then a battle in Dolphin) | **not yet: needs a human test** |
