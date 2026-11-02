# Motion files: `motion.DBN` and `snr_motion.BIN` (research notes, Nov 2, 2026)

The game's animation data. **Status: the container and track layout are verified** (by the code that loads them and
by a check of every track in `motion.DBN`); **what the values mean is inferred** and marked so. The way to finish it:
plan.md "Resume here", item 2.

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

## Inferred, to check against the code

- 78 tracks = 26 joints x 3 rotation axes (69 = 23 x 3): the track index k as joint k / 3, axis k % 3.
- The u16 values as angles (0x10000 = one turn); the interpolation between keys (linear, since no tangents are stored).
- **shared** as the skeleton's per-joint base values (offsets or scales) for the group's body type.
- The frame count is in `cr_g_MotionInfo[id]` (+4: u16 & 0xFFF; step 46) and the names in `cr_g_MotionNames`.
- `motion.DBN` group 0 holds the battle motion IDs 0x001..0x0A8 (sf_ / mg_ / ab_), groups 1 and 2 more battle and
  field IDs; `snr_motion.BIN` holds field motions (IDs from 0x1C1) per scenario.

The code that reads the tracks while a motion plays is not found yet: no function but the loader loops 0x45 / 0x4E
times, so the player probably walks the model's own joints; the users of `cr_g_CurrentChara_06dc` (cr_Motion_Play,
fn_80037FF0, fn_8003C4F0, fn_8004E8D4, fn_80053430 ...) are where to start.
