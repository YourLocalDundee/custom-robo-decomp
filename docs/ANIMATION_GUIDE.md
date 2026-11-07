# Custom Robo animations: formats, playback and tools

Everything known about how Custom Robo (GameCube, US) stores and plays its animations, in one page for modders and
tool writers. Each fact is read from the game's code (function addresses given) or checked over every file on the
disc; anything not verified says so. Deeper pages: [formats/motion.md](formats/motion.md) (file layout),
[formats/motion_playback.md](formats/motion_playback.md) (what the game does with the tracks),
[battle_animations.md](battle_animations.md) (which animation plays when).

## 1. Four kinds of animation

| Kind | Where | Format | Played by |
|---|---|---|---|
| **Robo body motions** (walk, run, jump, gun, victory, charge attack, ...) | entry 8 of `logo_read_us.dbn`: one motion file, 1,095 motion IDs | the game's own key format (section 2) driving an IK rig (section 3), not joint rotations | the motion player, then written onto the model's 19 role joints |
| Story-mode field motions | `scenario_us/snr_motion.BIN`, 50 pieces | same motion file format, 1 group each | same player |
| **Transforming bodies' own animation** (Defender, Seeker, Breaker: 10 frames; Ruhiel: 9) | inside the body's model entry (`chara.BIN`) | standard HSD joint animation (AnimJoint / AObj / FObj, as Melee) | set to a frame by the game: 0 normally, a 0..10 timer during flagged moves (section 5) |
| **Part animations** (gun, bomb, pod, leg forms) | inside the part's model entry (`weapon_*.BIN`) | standard HSD joint animation, 11 frames (0..10) | a pose set by a 0..1 state value: frame = 10 x value |
| Stage, effect and menu animations | `bg_data.BIN`, `*_effect.BIN`, ... | standard HSD joint / material animations in the `scene_data` | HSD's own playback |

**`motion.DBN` on the disc is never read.** It is an older loose copy (differs in 2 motions). Edit entry 8 of
`logo_read_us.dbn`.

## 2. The motion file format (verified: byte-identical round trip of all 51 motion files)

All values big-endian.

**Boot bundle `logo_read_us.dbn`**: u32 count (9), count + 1 u32 offsets from the file start (the last = file
length), zeros to 0x40; entries 0x20-aligned. Entry 8 (the last) is the motion file; the bundle may grow up to
2,842,624 bytes (the load region at boot, `fn_80023CBC`).

**Motion file**

| Where | What |
|---|---|
| +0 | u32 group count G (3 in the bundle) |
| +4 | G u32 group offsets (from the file start), then the file length, zeros to 0x20 |
| group +0 | u32 motion count n |
| group +4 | n entries of 3 u32 offsets from the group start: **tracks**, **pool**, **counts** |
| group +4 + 12n | n u16 motion IDs; bit 15 set = 78 tracks (every motion on the disc), clear = 69 |

Per motion:

- **tracks**: 78 pairs of u32 offsets (time list, value list); both 0 for a track without keys.
- **counts**: one byte per track, its key count.
- **pool**: the same offset for every motion of a group: the group's shared float pool (de-duplicated; at most
  16,384 floats, the index is 14 bits).

A **time list** is `count` u16 frame numbers, starting at 0, non-decreasing (a repeated frame = an instant change);
tracks may share time lists. A **value list** is `count` u16 key words:

| Bits 15-14 (kind) | Floats at pool[bits 13-0] | Key |
|---|---|---|
| 0 | value, in-tangent, out-tangent | full |
| 1 (0x4000) | none | value 0, flat |
| 2 (0x8000) | value | flat tangents |
| 3 (0xC000) | value, tangent | one tangent both sides |

**The curve** (`cr_Motion_SampleTrack` 0x8001EB48): between keys at frames t0 and t1, s = (f - t0) / (t1 - t0),
dt = (t1 - t0) / 30,
`v = p0 (1 - s)^2 (1 + 2s) + p1 s^2 (3 - 2s) + dt (m0 s (1 - s)^2 - m1 s^2 (1 - s))` (cubic Hermite; tangents per
second at 30 frames a second). Before the first key and on a key: the key's value.

**Rule:** every non-empty track must end on its motion's last frame. Past a track's last key the game reads the
following words as more keys (a single-key track broke a battle in a test). Every file on the disc follows this.

**Layout details for writers** (needed for byte-identical output): the file length after the group offsets; 0xFF fill
from the counts to the 4-aligned value lists; time lists right after the value lists, shared when one is the start of
another; track tables 4-aligned; groups 0x20-aligned. A motion ID registered by an earlier file wins
(`cr_MotionFile_Register` 0x80074DCC).

## 3. What the 78 tracks mean: an IK rig, not joint rotations (verified from the player code)

| Tracks | Channel | Unit |
|---|---|---|
| 0-2 | point 0: root point (hips) | model units |
| 9-11, 15-17 | points 1, 2: upper body and head aim targets | model units |
| 24-26, 36-38 | points 3, 4: hand targets (swap when mirrored) | model units |
| 48-50, 60-62 | points 5, 6: foot targets (swap when mirrored) | model units |
| 3-8, 12-14, 18-23, 27-35, 39-47, 51-59, 63-68 | x, y, z rotation of 16 joints of the game's default skeleton: body, upper body, head, arm A shoulder / upper / hand, arm B the same, lower body, leg A thigh / ankle / foot, leg B the same | degrees |
| 69-71 | root translation | model units |
| 72-74 | root rotation x, y, z | degrees |
| 75-77 | a vector turned by the root rotation, added to the root translation | model units |

Every frame (`cr_Chara_PoseFromMotion` 0x8003D4C4): root -> blend -> the 7 points through the root matrix and the 16
joint angles -> rotation matrices Rz.Ry.Rx -> the body: upper body and head aimed at points 1 / 2, **two-bone IK** for
each arm and leg toward its point (solved on the game's default skeleton, then rebuilt with this robo's own limb
lengths from its model's block A) -> 20 body matrices -> matrix k + 1 written as the world matrix of the model joint
with role k (19 roles, the model's block C). So:

- **One motion fits every robo**: it is retargeted through IK with each robo's own limb lengths.
- You author rotations and IK targets, not bone positions: role joint positions always come from the robo's lengths.
- Elbows and knees bend one way; a bend off the hinge is not possible.
- Facing: a robo faces -Z in the points' frame, except in `*_r180` motions (root rotation y + 180).
- Root tracks and joint 0 are used by the game beyond the pose: leave them as the original has them (a +20 degree
  root tilt blacked the screen in a test).

## 4. Tables in main.dol (verified)

| Address | Name | What |
|---|---|---|
| 0x80205C94 | `cr_g_MotionInfo` | 1,095 pointers; +4 of each = header, **u16 & 0xFFF = the motion's frame count** (fixed here, not in the motion file) |
| 0x80206DB0 | `cr_g_MotionNames` | 3 strings per motion ID; the first is the game's own name (`sf_win_sf011_01`, ...) |
| 0x801C51B8 | `cr_g_BattleMotionSets` | 34 sets x 38 u16: the motion each body plays per slot; bits 0-12 ID, 0x8000 = also run the body's own HSD animation |
| 0x801C5C10 | `cr_g_BodyData` | 0x78 per body; word 2 = the body's set (bodies 27..33 are not set = body: Chickenheart (33) uses set 27) |
| 0x801C6C44 | body stats | 0x134 per body; charge attack values at +0xD0..+0xEB (section 6) |
| 0x801F8EB8 | `cr_g_BattleRoboActionFuncs` | 23 battle action handlers |

Name prefixes: family (sf, ab, mg, lr, sv, tf, ls, fb, sp, ug, rr = the body families), then the move, then the body
code (sf011 = Ray 01 ...). `cr_atk_*` = charge attacks, `*_win_*` / `*_lose_*` = end of battle, `*_powerup` = get-up,
`*_change_skyer_*` = a transforming body's air dash, `*_r180` = turned 180 degrees.

## 5. Which animation plays when (verified; full table in [battle_animations.md](battle_animations.md))

The 38 slots: 0 stance, 1..9 walk / run (normal, weapon, guard, back, turned), 10 air dash, 11 double jump, 12 / 13
landing, 14..17 bomb, 18 / 19 pod, 20..25 gun, 26..29 falls, 30 magma, **31 get-up, 32 lose, 33 / 34 victory, 35
charge attack**, 36 / 37 change. Per-body list: `python -I tools/anim.py slots --body N`.

The motion frame is not always playback time: for the timed actions the game computes it from its own counters.

| Animation | Motion length | How it is timed |
|---|---|---|
| Victory (`win_01` / `win_02`) | 139 frames | end of battle: free battle = random of the two, team = by place, tag = win_01; holds frame 0, then plays over 140 frames (frame = 138.5 x elapsed / 140) |
| Lose | 139 | the same, for every loser still standing |
| Charge attack (`cr_atk_*`) | 60 = three 20-frame blocks | three phases; each block is stretched over that phase's duration from the body's stats (frame = 20 x (phase + elapsed / duration)) |
| Get-up (`*_powerup`) | 20 | 30 game frames after a knockdown (frame = 20 x elapsed / 30) |
| The rest | per motion | played in time, 30 frames a second |

**Transforming bodies**: their model carries an HSD joint animation; the game poses it at frame 0 normally, and during
a move whose slot has flag 0x8000 at a timer that steps 1 per frame to 10 (then 9.9). Their bind pose is the other
form, so exporters must apply that animation at frame 0 to show the robo form.

## 6. Charge attack data (verified: `fn_80046A68`)

Per body, in the stats record (0x801C6C44 + body x 0x134):

| Offset | What |
|---|---|
| +0xB0 | the charge's name (Shift-JIS, developers' name) |
| +0xD0 | per-phase flag bits (bit p = phase p; bit 3 switches which flag) - called invulnerability by the community |
| +0xD1 | per-phase bits (community: invisibility) |
| +0xD2..+0xD6 | damage / knockback (community table) |
| +0xD8 | homing: the aim turns by at most value / 1000 radians per frame (phases 0, 1) |
| +0xDA, +0xDC, +0xDE | phase durations (frames) |
| +0xE0, +0xE2, +0xE4 | phase speeds (x 0.1) |
| +0xE6, +0xE8, +0xEA | phase climb angles (degrees) |

It also switches on an extra attack model (most likely from the body's `attack_effect` entry; not verified) and
spawns effect / sound 0x30001.

## 7. HSD animations (parts, transforming bodies, stages, effects)

Standard sysdolphin, as in Super Smash Bros. Melee (HSDLib reads and writes them): a model's JOBJDesc record holds
{root joint, joint animation list, material animation list, shape animation list}; a joint animation is an AnimJoint
tree (child, next, AObj) mirroring the joint tree; an AObj holds flags, end frame and a list of FObjs (one per
channel: rotation x / y / z, scale, translation; material colour, alpha, texture ...), each a compressed keyframe
stream. The game attaches the first animation of each list at load (`HSD_JObjAddAnimAll`) and either plays it
(stages, effects) or sets its frame (`fn_80095700`: bodies and parts). End frames: parts 10, transforming bodies 10
(Ruhiel 9).

## 8. Tools (this repository)

| Command | What |
|---|---|
| `python -I tools/anim.py slots --body 20` | a body's 38 slots: motion name, ID, length, group, flags, other bodies sharing it |
| `python -I tools/anim.py export --body 20 --slot victory2` | `build/anim/<name>.glb`: the body's model with that motion as a glTF animation (30 fps); open in Blender |
| `python -I tools/anim.py import --body 20 --slot victory2 IN.glb --mod MYMOD` | the glTF animation (on the model's `JOBJ_n` joints) solved back into that motion, stretched to the game's length, written to `mods/MYMOD/files/motion.DBN`; then `python -I tools/make_mod.py mods/MYMOD` builds an ISO and a shareable patch |
| `python -I tools/crmotion.py list / export / import / roundtrip / sample` | the raw format: every key as JSON and back (byte-identical) |
| `python -I tools/crmotion_rig.py pose / gltf / import-gltf` | the rig: positions of a pose, motion -> glTF, glTF -> motion |
| `python -I tools/battle_motions.py --tsv` | every body x slot as a table |

How the import works: for each frame it reads the role joints' world matrices from the glTF, solves the 16 joint
angles (Euler Z.Y.X, unwrapped) and the 7 IK targets backwards, keeps the replaced motion's root tracks and joint 0,
then places keys greedily until every frame is within the tolerance (0.1 degree by default), with least-squares
tangents. Checked: a game motion exported and imported back gives the role joints within 0.001 (Breaker's victory,
315 keys); earlier checks within 0.0034 on three robos and 0.0016 for an animation made outside the game.

**Limits**: motion length fixed per ID (stretch, or a main.dol change of `cr_g_MotionInfo`); a group's float pool of
16,384 floats (a 141-frame victory takes about 2,400; use a larger tolerance if a motion does not fit); a motion ID
shared by several bodies changes on all of them; the transforming bodies' HSD animation is not part of the motion and
not exported by `anim.py` yet.

## 9. Code map and decompilation status (Nov 6, 2026)

Matched to C (100%; the action handlers partly as hand drafts that join the build in the next full run): `cr_Motion_Play`, `cr_Motion_PlayFromFrame1`, `cr_Motion_LoadInfo`, `cr_Motion_UpdateBlend`,
`cr_Chara_PoseFromMotion`, `fn_80095700` (set an HSD animation frame), the action handlers for victory (`fn_80045868`),
get-up (`fn_80045C08`), change (`fn_800459A0`, `fn_80045B40`), double jump (`fn_80048B10`) and two short ones. In
progress (drafts): the file register (`cr_MotionFile_Register`), the key sampler (`cr_Motion_SampleTrack`), root /
points / rotations, the IK (`cr_Chara_SolveArmIK` / `SolveLegIK` / `PoseLimbs`), the role matrices
(`cr_Battle_ApplyRoleMatrices`, `fn_80039FAC`), the action dispatcher and the remaining handlers, the end-of-battle
code (`fn_8004DC78`, `fn_8004E8D4`).

## 10. Open questions

- An imported motion played in a battle in Dolphin (the march and air dash of an earlier test did play; the gun
  motions of that test broke the screen through root / joint 0 edits).
- What the +0xD0 / +0xD1 charge phase flags do in play; damage / knockback fields from the code.
- The extra charge attack model's file; which of slots 1..9 the game picks when.
- The second floats block of the default skeleton (0x801F9D94 +0x1C).
