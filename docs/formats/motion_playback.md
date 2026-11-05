# Motion playback: what the tracks mean and how the game poses a robo (Nov 2, 2026)

The file layout of the motion files is in [motion.md](motion.md); the motions the game plays are entry 8 of
`logo_read_us.dbn`, not the loose `motion.DBN` ([the full path](motion.md#the-full-path-disc-to-screen-and-mod-to-iso)). This page is what the game does with
the tracks, read from its code, and the tools that convert motions both ways. Everything below is **verified**
by the code paths named and by the round trips at the end, unless marked otherwise.

## The chain, every frame

| Step | Function | What it does |
|---|---|---|
| 1 | `cr_Motion_UpdateRoot` 0x80056104 | root tracks 69-77 -> the root matrix; tracks 0-2 -> the root point |
| 2 | `cr_Motion_UpdateBlend` 0x800527D4 | blend factor of a motion change (off for a single motion) |
| 3 | `cr_Motion_UpdatePoints` 0x80054238 | the 7 IK points through the root matrix; the 16 joint angles |
| 4 | `cr_Chara_BuildJointRotations` 0x80053DA8 | one rotation matrix Rz . Ry . Rx per rotation joint |
| 5 | `cr_Chara_PoseLimbs` 0x8005AEE8 | the body: upper body, head, two arms (`cr_Chara_SolveArmIK` 0x8005AAA8 + `cr_Chara_PoseLimbA`), lower body, two legs (`cr_Chara_SolveLegIK` 0x8005A668 + `cr_Chara_PoseLimbB`) -> 20 body matrices at chara +0x154C |
| 6 | `cr_Battle_ApplyRoleMatrices` 0x800392B0 | body matrix k + 1 becomes the **world matrix** of the model joint with role k (19 roles, [model_entry.md](model_entry.md#joint-roles-block-c)) |

`cr_Chara_PoseFromMotion` 0x8003D4C4 runs steps 2-6 (step 1 is called just before it). The globals the steps share:
`cr_g_MotionTracks`, `cr_g_MotionCounts`, `cr_g_MotionFloats`, `cr_g_MotionFrame`, `cr_g_MotionFlags`
(symbols/cr_globals.txt).

## The game's tables (main.dol .data; to be typed, plan.md item 4)

| dtk object | Size | What (read from the bytes and the code above) |
|---|---|---|
| lbl_801F9B00 | 0x294 | +0x000: the default skeleton, 26 entries of 0x14 (u8 flags: bit 2 = a rotation joint; 3 bytes; 3 f32 lengths / offsets; a pointer). +0x208: 27 f32, the shape of a robo's block A (the limb code reads its indices 0..0x1A): most likely the default skeleton's limb parameters (0.358, 0.02, -0.02, -0.05, -0.16, 0.277, ...). +0x274: u8[16], the first track of each rotation joint in order (0x801F9D74). +0x284: u8[16], the same mirrored (0x801F9D84). |
| lbl_801F9D94 | 0x34 | +0x00: u8[26], the first rotation track of each skeleton slot (0 = none); +0x1A: 2 bytes padding; +0x1C: 6 f32 (0, 1, 0, 0, 0, 1: not read yet) |

## A track: keys and the curve (`cr_Motion_SampleTrack` 0x8001EB48)

A track is `count` u16 key frames and `count` u16 key words. A key word is a 2-bit kind (bits 14-15) and a 14-bit
index into the group's float pool (the "shared" table of motion.md):

| Kind | Floats at the index | Meaning |
|---|---|---|
| 0 | value, in-tangent, out-tangent | full key |
| 1 (0x4000) | none | value 0, flat |
| 2 (0x8000) | value | flat tangents |
| 3 (0xC000) | value, tangent | one tangent for both sides |

Between two keys the value is a cubic Hermite curve: s = (frame - t0) / (t1 - t0),
v = p0 (1 - s)^2 (1 + 2 s) + p1 s^2 (3 - 2 s) + dt (m0 s (1 - s)^2 - m1 s^2 (1 - s)), with dt = (t1 - t0) / 30: the
tangents are per second at 30 frames a second (constant 1/30 at 0x80450428). On a key, and before the first key, the
value is the key's. Past the last key the code reads the words after the list as more keys, so **every non-empty track
must end on its motion's last frame**: all 52 motion files on the disc do, and `crmotion.build` refuses a file that
does not. Nov 2, 2026: tracks with one key at frame 0 (held values) in the Atlas dash and gun motions made a battle
glitch, a robo vanish and the screen go black; `crmotion_rig.reduce_keys` now keys a held value on both ends.

The pool is shared by all motions of a group and de-duplicated, so a 14-bit index limits a group to 16,384 floats
(group 0 of motion.DBN uses 13,867).

## The 78 tracks

| Tracks | Role | Unit |
|---|---|---|
| 0-2 | point 0: the root point (hips) | model units, through the root matrix |
| 9-11, 15-17 | points 1 and 2: upper body and head targets | model units |
| 24-26, 36-38 | points 3 and 4: the two hand targets (they swap when mirrored) | model units |
| 48-50, 60-62 | points 5 and 6: the two foot targets (they swap when mirrored) | model units |
| 3-5, 6-8, 12-14, 18-23, 27-35, 39-47, 51-59, 63-68 | x, y, z rotation of the 16 rotation joints | degrees (x 65536 / 360 -> 16-bit angle) |
| 69-71 | root translation | model units |
| 72-74 | root rotation x, y, z (added to joint 0's angles, and the feet's base rotation) | degrees |
| 75-77 | a vector turned by the root rotation and added to the root translation | model units |

The rotation joints are the slots of the game's default skeleton (26 entries of 0x14 at 0x801F9B00) whose flag byte
has bit 2: slots 0, 1, 3, 5, 6, 9, 10, 11, 14, 15, 16, 19, 20, 21, 24, 25; their first tracks are in the byte table
0x801F9D94 (by slot) and 0x801F9D74 (in order; 0x801F9D84 is the mirrored order). In order they drive: 0 the body
(root), 1 the upper body, 3 the head, 5 / 6 / 9 arm A shoulder / upper arm / hand, 10 / 11 / 14 arm B, 15 the lower
body, 16 / 19 / 20 leg A thigh / ankle / foot, 21 / 24 / 25 leg B. A motion with ID bit 15 clear has 69 tracks (no root
tracks): the root matrix is the identity and point 0 is taken as it is. Every motion on the disc has 78.

## The body (cr_Chara_PoseLimbs)

The body matrices are built on a matrix stack (`cr_Mtx_Push` / `cr_Mtx_Pop`); `cr_Mtx_Mul` multiplies the 3x3 part
only, so translations come only from `cr_Mtx_TranslateX` / `TranslateZX` and their relatives:

- Matrix 0: at point 0, the body rotation (joint 0). Matrix 1 (upper body): joint 1, then its X axis turned to
  point 1. Matrix 2 (head): moved up the upper body by block A [0], [1], joint 3, X axis turned to point 2.
- Arms: `cr_Chara_SolveArmIK` puts the hand target in the shoulder frame of the **default** skeleton (lengths from
  0x801F9B00) and solves the elbow by the law of cosines; `cr_Chara_PoseLimbA` then rebuilds the arm with the robo's
  own lengths (model block A [2..7] and [8..13]) and those angles: shoulder, upper arm aimed at the target and bent,
  forearm, hand. A robo's arms keep the motion's angles, not its hand positions.
- Matrix 11 (lower body): joint 0, joint 15, a quarter turn about Z. Legs: `cr_Chara_SolveLegIK` +
  `cr_Chara_PoseLimbB` the same way (block A [0xE] down to the hips, [0xF..0x14] and [0x15..0x1A]); the ankle takes
  the root rotation as its base, so the feet do not follow the leg's bend.

Off for a robo at the origin facing +Z and not modelled by the tools: the facing angle and position, mirroring
(`cr_g_MotionFlags` & 2), blending (& 4), the relative root modes (& 1, & 8), the y-offset scale, the per-character
point offset at chara +0x9A0.

## Part animations are standard HSD joint animations

The robo body's motions are the game's own key format above (motion.DBN / logo_read_us.dbn entry 8, played by the
procedural chain). The parts are different: each weapon_gun / weapon_bom / weapon_pod / weapon_leg entry has several
root joints, and the big ones carry a standard sysdolphin joint animation (`HSD_JObjAddAnimAll`, started at frame 0
by the loader). Which roots the battle robo uses (gun 0, bomb 1, pod 0 + 1, legs 4 / 1 and 5 / 3), and why a part
looks garbled without its joint matrices: [model_entry.md, "Part files"](model_entry.md#part-files-root-joints-draw-groups-which-ones-the-game-uses-and-why-a-part-can-come-out-garbled-nov-5-2026).
Open: how the parts' joints attach to the robo's role joints (block C) and whether the body motions drive them.

## Tools

| Command | What |
|---|---|
| `python -I tools/crmotion.py list FILE` | every motion: ID, game name (cr_g_MotionNames), frame count, tracks used |
| `python -I tools/crmotion.py export FILE OUT.json` | every key decoded (frame, kind, value, tangents; the raw word kept) |
| `python -I tools/crmotion.py import IN.json OUT` | rebuild the file; a changed key gets new floats in the pool |
| `python -I tools/crmotion.py roundtrip FILE_OR_FOLDER ...` | file -> JSON -> file, byte compare |
| `python -I tools/crmotion.py sample FILE ID FRAME` | every channel at a frame, by role |
| `python -I tools/crmotion_rig.py pose FILE ID FRAME ENTRY.bin` | the 7 points and the 19 role joints' world positions |
| `python -I tools/crmotion_rig.py gltf FILE ID ENTRY.bin MODEL.glb OUT.glb` | the robo model (HSDRawViewer export) with the motion as a glTF animation |
| `python -I tools/crmotion_rig.py import-gltf IN.glb ANIM ENTRY.bin FILE ID OUT` | a glTF animation of that robo's role joints -> motion ID in FILE |
| `tools/pyenv/Scripts/python -I tools/render_motion.py IN.glb OUT.png` | frames of a glb animation side by side (skinned on the CPU) |

`import-gltf` solves the body backwards. For each frame it reads the role joints' world matrices. The rotation joints
come from each matrix relative to its parent frame (Euler Z.Y.X, the solution nearest the previous frame, so gimbal
lock does not make the angles jump). An elbow's or knee's bend gives the target distance on the default skeleton, and
each IK target is placed along the solved bone. The root tracks and joint 0 are kept from the motion being replaced
(`--no-template`: zero). Keys are placed greedily at the worst frame, with tangents fitted by least squares, until
every frame is within `--tolerance` degrees (default 0.1). A limb's IK target is held tighter near a straight limb,
where a small change of distance is a large change of angle; other points stay within tolerance / 100 units.

What the rig cannot do is projected onto what it can:
- An elbow or knee bent the other way.
- A bend off the hinge axis.
- Role joint positions. The game places every role joint from the block A lengths, so only rotations and the IK
  targets can be authored.

Robo 0's bind pose, for example, has its hands and foot plates parked away from the limbs; the game never shows that.

**Authoring notes** (found making mods/atlas_ninja_run, Nov 2, 2026; each checked with `crmotion_rig.pose` numbers):
- Facing: in the points' frame a robo faces -Z, except in the `*_r180` motions, which add 180 to root rotation y and
  to joint 0's y (the body faces the same world way, the points' frame is turned): there forward is +Z and the sides
  swap. Read it per frame from root rotation y.
- Limb reach: `cr_Chara_SolveLegIK` / `SolveArmIK` measure the target on the **default** skeleton (0x801F9B00), then
  the robo's own lengths are posed with those angles. On robo 0 a foot target 0.6 from the hip joint (0.08 below
  point 0) gives a 96-degree knee, 0.8 gives 49 degrees, about 0.9 is straight (past it the solve leaves the cosine
  range). The knee bends one way only, so a thigh swings only as far as the target direction allows.
- The gun: the barrel is the gun hand's (role 9) X axis, along the forearm while joint 14 is 0. `joint14.rotz` pitches
  it (-60: 33 degrees up from a forearm 24 degrees down), `joint14.roty` turns it sideways, `joint14.rotx` rolls it.
  In a battle the game aims this arm itself, so what it keeps of these is for the in-game test to show.
- The joint angle tracks of the lower body / thighs (15, 16, 21) barely move the knee once an IK target is set: pose
  legs by their foot targets.
- Root tracks (69-77) and joint 0: the game uses them beyond the pose. Battle results (Nov 2, 2026, not yet proven
  one by one): root rotation x +20 on the air dash blacked the 3D screen on the dash; joint 0 rotation x +20 (dash)
  with joint 0 rotation y +180 (gun motions) blacked it as the battle started; root rotation y +180 on the gun
  motions ran. Lean a pose by its 7 targets; the port's equal pose is no proof here.

**Capacity.** A group's float pool is limited to 16,384 floats by the 14-bit index. motion.DBN group 0 uses 13,867
(about 2,300 free after the unused floats of a replaced motion are dropped), group 1 9,864, group 2 4,198. An imported
51-frame run takes about 1,000 floats at 0.1 degree, a 141-frame victory pose about 2,400. When a motion does not fit,
the tool says how many floats it needs and how many are free. Then use a larger `--tolerance`, a shorter animation, or
a motion ID in a group with room. The pieces of snr_motion.BIN are files of their own.

## Checked (Nov 2, 2026)

- `crmotion.py roundtrip` on motion.DBN and the 50 pieces of snr_motion.BIN (piece 24 is an empty group): **51 of 51
  byte-identical**. The layout rules this needed: the file length after the group offsets; 0xFF fill from the
  counts to the 4-aligned value lists; time lists right after the value lists, shared when one is the start of
  another; track tables 4-aligned; groups 0x20-aligned.
- An edited key and an added key (export, change, import): the sampled values change, every other motion stays equal.
- `sf_back_run` (0x8005), `sf_normal_jump`, `ab_win_ab021_01` exported on robos 0, 5 and 30 and rendered: a running
  stride, the jump pose (a held pose: all keys equal), the victory pose.
- `sf_back_run` on robo 0 -> glTF -> `import-gltf` -> re-posed: the 19 role matrices equal the original's within
  0.0034 (worst matrix element over 51 frames, tolerance 0.1 degree; 0.0027 at 0.05) on robo 0 **and** on robos 5
  and 30, 536 keys (the original has 211).
- An animation made outside the game: robo 0's bind pose with both upper arms swung 50 degrees about their Z axis
  (61 frames), imported with `--no-template`: the role rotations are reproduced within 0.0016 (218 keys).
- `ab_win_ab021_01` (141 frames) exported, then imported over `m_wait` (0x81A3, group 2): within 0.021; the same
  import into group 0 after two others stops with the capacity message.

Not checked yet: an imported motion played in the game (Dolphin), and a motion authored in Blender rather than one that
came from the game.
