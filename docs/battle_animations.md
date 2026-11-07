# Battle animations: every motion slot, victory, lose, get-up and the charge attack (Nov 6, 2026)

Which animation the game plays for a robo in battle, which code decides it, and how its frames are timed. Read from
the m2c / hand drafts of the battle code (addresses below) and the tables in main.dol; the full per-body table is
[battle_motion_sets.tsv](battle_motion_sets.tsv) (34 bodies x 38 slots, written by `tools/battle_motions.py --tsv`).
Motion file format and the player: [formats/motion.md](formats/motion.md), [formats/motion_playback.md](formats/motion_playback.md).

## How a battle animation is chosen (verified)

1. **Body -> motion set.** `cr_g_BodyData` (0x801C5C10, 0x78 per body) word 2 points at the body's set in
   `cr_g_BattleMotionSets` (0x801C51B8, 38 u16 per set); it is copied to battle robo +0x554. Bodies 0..26 use set =
   body; **Chickenheart (33) uses set 27 and bodies 27..32 sets 28..33** (Chickenheart was added late). Always go
   through the pointer.
2. **Action -> handler.** The robo's action is the top byte of battle robo +0x528; `cr_BattleRobo_UpdateAction`
   (0x8004B0A0) calls `cr_g_BattleRoboActionFuncs[action]` (0x801F8EB8, 23 handlers) once per frame.
3. **Handler -> slot.** Each handler writes `cr_g_BattleMotionSlot` (0..37); the update code then plays
   `set[slot]` through `cr_Motion_Play` when it changes.
4. **Request -> motion.** A set entry is a motion ID (`& 0x1FFF`, 1,095 IDs, names in `cr_g_MotionNames` 0x80206DB0,
   three strings per ID, the name first) plus flags. **0x8000** (shown as T in the tables) also drives the body's own
   joint animation (the transforming bodies, [IMPORT_EXPORT_GUIDE.md 3c](IMPORT_EXPORT_GUIDE.md#3c-transforming-bodies-apply-their-own-animation-at-frame-0)):
   battle robo +0x85B is set from it and `fn_80039FAC` then poses the body animation at frame +0x550 instead of 0.
5. **Frame.** For the timed actions below, the motion frame is not playback time: the handler computes it each frame
   into battle robo **+0x548** from a countdown (+0xE2, counting down from the length +0xE0).

The motion data itself is the boot bundle `logo_read_us.dbn` entry 8 (all 1,095 motions in one file;
`cr_g_MotionInfo` 0x80205C94 has each motion's frame count).

## The 23 actions and 38 slots

| Action | Handler | What | Slots it plays |
|---|---|---|---|
| 0 | `fn_80049924` | standing / moving (the main handler: reads input, starts the other actions) | 0 stance (`*_ta_kamae`), 1..6 walk / run (normal, weapon, back; r180 = turned), 7, 8..9 guard runs, 0x14, 0x17, 0x18, 0x23 |
| 1, 2 | `fn_80049904`, `fn_800498FC` | (short handlers, not read) | |
| 3 | `fn_8004949C` | gun | 0x14 weapon stance, 0x15 weapon jump, 0x16 jump stiff, 0x19 weapon stiff (and 0x17 / 0x18 l90 / r90 from action 0) |
| 4 | `fn_800490C8` | bomb | 0x0E l90, 0x0F r90, 0x10 stance, 0x11 jump |
| 5 | `fn_80048E7C` | pod | 0x12 pod, 0x13 pod jump |
| 6 | `fn_80048B10` | double jump | 0x0B |
| 7 .. 11 | `fn_80048708`, `fn_80048050`, `fn_80047AC0`, `fn_8004752C`, `fn_80046F58` | jump / air dash states | 0x0A (`sf_jump_dash`; the transforming bodies: `*_change_skyer_*`, flag T) |
| **12** | **`fn_80046A68`** | **charge attack** (below) | **0x23** (`cr_atk_<body>_*`) |
| 13 | `fn_800468B8` | landing | 0x0C landing, (0x0D landing 2) |
| 14 | `fn_80045FEC` | knocked down / falling | 0x1A..0x1D falls (back, back into a wall, front, front into a wall) |
| 15 | `fn_80045CE8` | fallen in magma | 0x1E |
| **16** | **`fn_80045C08`** | **get-up** (`*_powerup`) | **0x1F** |
| 17, 18, 19 | `fn_80045B40`, `fn_80045A48`, `fn_800459A0` | "change" (entered on input bit 0x80 standing, 0x200 in the air; 10 frames) | 0x24 (`sf_change`) |
| **20** | **`fn_80045868`** | **victory** | **0x21 or 0x22** (`*_win_*_01` / `_02`) |
| **21** | **`fn_80045724`** | **lose** | **0x20** (`*_lose_*_01`) |
| 22 | `fn_800456B8` | (resets to slot 0) | 0 |

Slot 0x25 (37) repeats 0x24 in every set. Which of slots 1..9 action 0 picks (move direction, weapon state) was
not read in detail.

## Victory and lose (verified)

**Who enters them.** `fn_8004DC78` (battle end) calls `fn_8004E8D4(order, action, pose)` for each robo:

| Mode | Winner | Losers still standing (not out / flags 0x800200 clear) |
|---|---|---|
| free battle (`battle_mode` 0, one winner) | action 20, **pose = `cr_Rand_Int() & 1`** (win_01 or win_02 at random) | action 21 |
| free battle, team result | action 20, pose `(1 - i) & 1` by place i (first winner win_02, second win_01) | action 21 with the same rule |
| tag battle (`battle_mode` 2) | the active robo and its partner: action 20, pose 0 (win_01) | the losing team: action 21 |
| story mode (`fn_801314D4`, at a scenario timer) | robo 0: action 20, random pose | |

`fn_8004E8D4` also stops the robo (speed 0), turns it toward the camera side, snaps the limbs to the motion's first
frame and stores the pose byte at battle robo +0x1CA; `fn_80045868` (action 20) plays slot **0x21 + pose**.

**Timing.** Length +0xE0 = 140 frames; the countdown +0xE2 starts at 140 + (1 - order) x 90 + 30 (the first robo
waits longer). Each frame the countdown drops by 1 and: while it is still at or above 140 the frame is 0 (the motion
waits on its first frame: 120 frames for order 0, which is every single winner and loser, and 30 for order 1), then frame = **138.5 x
(140 - countdown) / 140**. Every win / lose motion is **139 frames** long, so the
whole motion plays once over the 140 frames and then holds. The body's own animation timer (+0x550) steps 1 per
frame to 10, then 9.9.

**Per body** (slot 0x21 / 0x22 / 0x20; T = the body animation runs too):

| Body | win_01 | win_02 | lose |
|---|---|---|---|
| 0..17, 21..27, 29..33 | `<fam>_win_<code>_01` | `<fam>_win_<code>_02` | `<fam>_lose_<code>_01` |
| 18 Defender | `ls_win_ls071_01` (T) | `ls_win_ls071_02` | `ls_lose_ls071_01` (T) |
| 19 Seeker | `ls_win_ls072_01` (T) | `ls_win_ls072_02` | `ls_lose_ls072_01` (T) |
| 20 Breaker | `ls_win_ls073_01` | `ls_win_ls073_02` (T) | `ls_lose_ls073_01` (T) |
| 28 Ruhiel | `ug_win_ug102_01` | `ug_win_ug102_02` (T) | `ug_lose_ug102_01` (T) |

(`<fam>` = sf, ab, mg, lr, sv, tf, fb, sp, ug, rr; `<code>` = the body code, e.g. sf011 for Ray 01. The three Rahu
bodies share rr111's motions.) The T poses are the ones where the transforming bodies change form during the win
or lose motion.

## The charge attack (action 12, slot 0x23) (verified)

**Start.** In action 0 (`fn_80049924`), on input bit 0x40 (+0x242) when the robo is not already in it (+0x139 != 2):
action 12, slot 0x23, phase (+0x186C) 0, countdown = the phase 0 duration; the aim vector toward the target; the
**attack model** at battle robo +0x1DCC is restarted at frame 0 and switched on (+0x1DBC = 1, drawn by
`fn_800389C8` with the body's matrix at +0x194); effect / sound **0x30001** at the robo's position.

**Three phases** (`fn_80046A68`, every frame). Each phase reads its own values from the body's stats record
(`lbl_801C6C44`, 0x134 per body; the community's "RoboBytes" +0xD2 block):

| Stats offset | Meaning (from the code) |
|---|---|
| +0xD8 (s16) | homing: the aim turns toward the target by at most value / 1000 radians per frame (phases 0 and 1 only) |
| +0xDA, +0xDC, +0xDE | duration of phase 0, 1, 2 (frames) |
| +0xE0, +0xE2, +0xE4 | speed of phase 0, 1, 2 (x 0.1) |
| +0xE6, +0xE8, +0xEA | climb angle of phase 0, 1, 2 (degrees; the velocity is aim x cos, y = sin) |
| +0xD0 (bits) | bit p set: a flag for phase p (battle robo +0x187C; with bit 3 also set, +0x1880 instead) - the community names +0xD0 "invulnerability" |
| +0xD1 (bits) | bit p clear: a state for phase p (the community: "invisibility"); exact effect not traced |

(+0xD2 / +0xD4 / +0xD6: damage and knockback per the community's table; not read here.) When a phase's countdown
ends the next phase starts; after phase 2 the robo returns to action 0 and the attack model is switched off.

**Frame.** frame = **20 x (phase + (duration - countdown) / duration)**: phase 0 plays motion frames 0..20, phase 1
20..40, phase 2 40..60, each stretched over that phase's duration. **Every `cr_atk_*` motion is 60 frames.** So a
charge attack animation is three 20-frame blocks, one per phase; a mod that changes a phase's duration slows or
speeds that block, not the others. The body animation timer (+0x550) runs too (Breaker's attack is flagged T: it
attacks in its other form).

**Per body** (stats values from main.dol; motion from the body's set; names in the stats record are the developers'):

| Body | Motion (slot 0x23) | Homing | Durations | Speeds | Angles |
|---|---|---|---|---|---|
| 0 Ray 01 | cr_atk_sf011_tackl_n | 10 | 10, 14, 35 | 80, 240, 80 | 0, 0, -90 |
| 1 Splendor | cr_atk_sf012_diving | 10 | 20, 20, 13 | 70, 188, 0 | 79, -17, -90 |
| 2 Glory | cr_atk_sf013_upper | 40 | 10, 19, 20 | 80, 160, 20 | 0, 60, -90 |
| 3 Milky Way | cr_atk_ab021_hiza | 15 | 10, 8, 20 | 0, 240, 100 | 0, 20, 0 |
| 4 Earth | cr_atk_ab022_suso | 20 | 1, 22, 30 | 0, 180, 44 | 0, 30, -90 |
| 5 Sol | cr_atk_ab023_spnkkt | 10 | 11, 12, 35 | 160, 160, 80 | 50, -50, -90 |
| 6 Metal Ape | cr_atk_mg031_spin | 100 | 1, 64, 15 | 0, 80, 66 | 0, 30, -90 |
| 7 Metal Bear | cr_atk_mg032_zutuki | 0 | 15, 12, 44 | 0, 350, 80 | 0, 0, -90 |
| 8 Metal Ox | cr_atk_mg033_headjump | 60 | 20, 80, 34 | 70, 72, 80 | 56, -10, -90 |
| 9 Swift | cr_atk_lr041_vrot | 10 | 10, 24, 30 | 223, 146, 80 | 63, -35, 0 |
| 10 Shrike | cr_atk_lr042_shoryu | 40 | 5, 30, 5 | 200, 120, 80 | 0, 40, -60 |
| 11 Peregrine | cr_atk_lr043_zujump | 0 | 5, 33, 30 | 120, 120, 30 | 0, 10, -30 |
| 12 Javelin | cr_atk_sv051_tetsuzan | 20 | 5, 8, 11 | 80, 240, 80 | 0, 0, -90 |
| 13 Glaive | cr_atk_sv052_utan | 10 | 10, 8, 30 | 120, 240, 80 | 0, 45, 160 |
| 14 Halberd | cr_atk_sv053_hiki | 10 | 10, 14, 30 | 80, 240, 0 | 0, 0, 0 |
| 15 Criminal | cr_atk_tf061_propkick | 40 | 15, 20, 20 | 0, 125, 75 | 0, 0, 30 |
| 16 Buggy | cr_atk_tf062_propeller | 156 | 8, 45, 34 | 450, 80, 80 | 20, -135, -90 |
| 17 Juggler | cr_atk_tf063_spin | 40 | 1, 28, 49 | 2000, 236, 80 | 68, -75, -90 |
| 18 Defender | cr_atk_ls071_skyerspn | 100 | 5, 120, 38 | 10, 40, 140 | 0, 0, -60 |
| 19 Seeker | cr_atk_ls072_skyeratk | 40 | 10, 60, 20 | 0, 80, 85 | 0, 15, -20 |
| 20 Breaker | cr_atk_ls073_skyeratk (T) | 100 | 5, 120, 30 | 10, 40, 40 | 0, 10, 0 |
| 21 Seal Head | cr_atk_fb081_zutsuki | 30 | 10, 50, 40 | 0, 100, 20 | 0, 0, -90 |
| 22 Dour Head | cr_atk_fb082_sideroll | 30 | 5, 30, 50 | 0, 120, 140 | 0, 30, -60 |
| 23 Tank Head | cr_atk_fb083_hippress | 60 | 10, 20, 30 | 225, 153, 20 | 58, -38, -90 |
| 24 Ray Legend | cr_atk_sp091_tackl | 15 | 4, 13, 30 | 80, 180, 180 | 0, 140, -120 |
| 25 Oil Can | cr_atk_sp092_dada | 10 | 10, 20, 35 | 80, 96, 80 | 0, 0, -90 |
| 26 Ray Warrior | cr_atk_sp093_tackl_n | 20 | 30, 8, 5 | 0, 350, 0 | 0, 0, -90 |
| 27 Rakansen | cr_atk_ug101_ude | 20 | 10, 20, 30 | 200, 350, 0 | 135, -20, -90 |
| 28 Ruhiel | cr_atk_ug102_drill | 20 | 1, 13, 38 | 0, 240, 240 | 0, 60, 0 |
| 29 Athena | cr_atk_ug103_hiji | 20 | 3, 12, 6 | 160, 160, 160 | 30, 90, 120 |
| 30 Rahu I | cr_atk_rr111_spinrari | 10 | 10, 14, 35 | 80, 240, 80 | 0, 0, -90 |
| 31 Rahu II | cr_atk_rr111_spinrari | 20 | 10, 20, 30 | 200, 350, 0 | 135, -20, -90 |
| 32 Rahu III | cr_atk_rr111_spinrari | 10 | 30, 60, 90 | 30, 50, 30 | 0, 0, 120 |
| 33 Chickenheart | cr_atk_sp094_kakku | 20 | 14, 10, 5 | 200, 320, 200 | 70, -50, 70 |

**The attack model.** Battle robo +0x1DCC is a joint tree loaded at battle start by `fn_80075258(.., 0)` from the
player's entry in `lbl_80377EB0` (next to the effect file slots `fn_80075950` fills; the attack_effect entry is
loaded by **body** number there, so this is most likely model 0 of the body's attack_effect entry: not verified).

## Get-up (action 16, slot 0x1F, `*_powerup`) (verified)

Not a charge: after a knockdown (action 14) or the magma fall (action 15), once the robo's down value (+0x74) reaches
1.0 (or a game-mode flag 0x100000 is set), the robo enters action 16 for **30 frames** with sound / effect 0x3007C;
frame = **20 x (30 - countdown) / 30**; every `*_powerup` motion is 20 frames. One powerup motion per family
(e.g. all three sf bodies use `sf_sf011_powerup`).

## Modding notes

- To change **which** animation a body uses: edit its set (the u16 at 0x801C51B8 + set x 0x4C + slot x 2; keep the
  0x8000 flag for the transforming bodies' forms) - a main.dol code change (docs/MODDING.md "Code changes").
- To change **the animation itself**: replace that motion ID's data in the boot bundle (`tools/crmotion.py`,
  [formats/motion.md](formats/motion.md#for-a-mod)); keep its length: win / lose 139 frames, charge attack 60 (three
  20-frame phases), get-up 20 - the game maps its own timers onto those frames, so a longer motion is cut and a
  shorter one holds its last frame.
- Charge attack **timing / movement** is in the body's stats record (table above), independent of the motion.

## Open

- The meaning of the +0xD0 / +0xD1 phase flags in play (the community calls them invulnerability / invisibility).
- Which of slots 1..9 action 0 picks when; actions 1, 2 and the "change" actions' purpose.
- The attack model's file (most likely attack_effect model 0).
