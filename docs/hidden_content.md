# Hidden, unused and leftover content (audit started Nov 7, 2026)

What the US disc (GXCE01) holds that the game never shows, or shows only in a cut-down form. Each item says how it
was checked. Tools: `tools/bmg.py` (game text), `tools/ptcl.py` (particles), `tools/scr.py` (story scripts),
`tools/anim.py` (motion slots), `tools/scene.py` (effects and stages).

## In short

| What | Found | Is it real cut content? |
|---|---|---|
| Robo bodies | all 34 are selectable | no hidden bodies |
| Parts | 49 never-listed parts named "Dummy Gun / Bomb / Pod / Legs" | no: empty placeholder slots (free for mods) |
| Guns | sound banks `WIDEVULCAN.ssm` and `LIGHT_ARC.ssm` that the game never loads | yes: two cut guns, only their sounds survived |
| Stages | stage 6 (copy of Basic Arena) and stage 7 "RPG Test" (grey blockout of a story room) | yes, both hidden from the stage select |
| Battle animations | 52 never used: Rahu II / III's own victories, loses and arm-strike charges; "change back" motions of the 4 transforming robos; Seeker's own get-up; a jump; older walk / run sets | yes |
| Story animations | 59 of 646 never played (battle poses, a shout, posed walks, an office phone pair ...) | yes |
| Particles | 494 of 1,396 particle effects never started | yes (or leftovers) |
| Debug menu | "Start Debug Mode" text on the story file screen, never shown | the entry was cut; only its text remains |
| Developer files | HAL model viewer config, debugger config, test fonts, placeholder dialogue, the Japanese GBA link program ("ROBO GX") | leftovers; the US game never loads them |
| Debug camera | camera mode 0 `CAMMODE_DEBUG`: a free camera on controller 1 (d-pad orbits, X / Y raise and lower) | left in and **live**: it is the mode every camera starts in; the title runs it until its 3D model is loaded (checked in Dolphin), and mode select, the main menu and the encyclopedia never set another mode (from the code) |
| Story maps | map 33 (a grey placeholder square) is used by neither story; 4 test scripts and a blank chapter template are left in the chapter list; characters 50 / 54 and 27 object slots are placeholder copies | leftovers |
| Test text | `SNT_TEST.bmg`: early Japanese system text with a glossary from the N64 game's story and a cut "robo fusion / Dual Dive" machine | leftover: the logo sequence prepares a load request for it but never sends it, so it is never read or shown (checked in Dolphin) |

## Parts: 49 "Dummy" slots (checked)

The menus walk `cr_g_PartOrderBody` .. `cr_g_PartOrderLeg` (0x8023B73A..; each ends at 0xFFFF). Every body (0-33) is
listed; the archives hold more parts than the lists:

| Part | Listed | On the disc | Never listed | Their names (`bmg/info_usa.bin` file 9) | Their files |
|---|---|---|---|---|---|
| Gun | 52 (0-51) | 64 | 52-63 | Dummy Gun 1-12 ("Adjustment Gun n") | one placeholder model copied 12 times; gun_effect = copies of entry 0 |
| Bomb | 41 (0-40) | 52 | 41-51 | Dummy Bomb 1-11 | one placeholder copied; bomb_effect = copies of 0 |
| Pod | 44 (0-43) | 57 | 44-56 | Dummy Pod 1-13 | copies of pod 0; pod_effect = copies of 0 |
| Legs | 16 (0-15) | 29 | 16-28 | Dummy Legs 1-13 | one placeholder copied; leg_effect 16-33 = copies of 0 |

Message file 9 has 236 names = 34 bodies + 64 + 52 + 57 + 29, in that order (file 0 has their descriptions). These
are reserved slots (the game has "download new part data" text, file 19), not hidden finished parts: free slots for
mods (names, descriptions, models and effects already have a place).

## Cut guns: Wide Vulcan and Light Arc (checked)

The disc's `sound/` folder has 59 sound banks (`.ssm`, one per gun family plus system sounds). main.dol's list of
banks it loads (the `sound/*.ssm` names before `sound_data.sem`) has 57. The two it never names: `WIDEVULCAN.ssm`
and `LIGHT_ARC.ssm`. No gun name in file 9 matches them (the arc guns are Left / Right / Waxing / Waning Arc), no gun
model or effect is set aside for them (the 12 extra gun slots are all the one placeholder), so only the sounds of
two cut guns survived. (A community lead; checked here.)

**No other cut guns.** The gun stats table `cr_g_GunData` (0x801CA560, 64 x 0x28C) has a display name at +0x00 and
a developer label at +0x20. The labels show how the 64 slots were used:

| Slots | Developer label | What is there |
|---|---|---|
| 0-33 | the gun's own name | the original guns |
| 34-44 | "Dummy Gun 1-11" | guns added late into reserved slots: Left / Right Loop, Circle Sword, Thunder, Flare Cannon, Left / Right 5-Way, Angel Ring, Ripple Laser, X Laser, Meteor Flicker |
| 45-51 | "Illegal 1-7" | the illegal guns: Orochi, Garuda, Luna Orbit, Diana Orbit, Ragnarok 1st / 2nd / 3rd |
| 52-63 | "Dummy Gun 12-23" | unused: named only "Dummy Gun", one placeholder model, stat values that differ slot to slot (test values, none matching a real gun) |

Wide Vulcan and Light Arc have no record, name, model or effect: only the two sound banks.

**Dummy Guns 12-23 = slots 0x34-0x3F: community cross-check (Discord #modding, Nov 7, 2026; checked here Nov 8).**
The community counted the same 12 slots (0x34-0x3F = 52-63) and called all of them "copies of Basic Gun", with the
projectile archive (`gun_effect.BIN`) following gun order and holding 12 duplicates of Basic Gun's projectiles.
Checked against `models_dat/`:

| File | Entries 52-63 | Same bytes as Basic Gun (0)? |
|---|---|---|
| `gun_effect.BIN` (the shot and hit) | all 12 identical | **yes**: `gun_effect_052_0/1.dat` .. `_063_*` = `gun_effect_000_0/1.dat` (MD5 46d2bc6e... / e84b9826...) |
| `weapon_gun` (the gun on the arm) | all 12 identical | **no**: one shared placeholder (59,623 bytes, MD5 2aeb5dfb...) vs Basic Gun's 55,399 bytes; it may look like Basic Gun, but it is its own file |

So a mod that fills a dummy slot replaces three things: the arm model (the placeholder), the effect entry (Basic
Gun's shot until replaced) and the `cr_g_GunData` record (the test stats above). Gun numbers for the guns named in
the same thread (number = message file 9 index - 34): Basic 0, Vertical 3, V Laser 16, Claw 25, Twin Fang 30,
Gravity 31, Sword Storm 36 (their projectile notes: WHERE_IS_EVERYTHING.md section 4).

## Bodies (checked)

All 34 bodies are in the body list; `chara.BIN` has 170 entries = 5 per body. No hidden body model.

## Battle motions: 52 never in a slot (checked)

Every motion of `logo_read_us.dbn` entry 8 (363) against the 34 bodies' 38-slot tables (`battle_motions.sets`): 212
are in a slot; of the other 151, 99 are story-mode people (next section) and 52 are battle motions. No code passes a
motion number as a constant (the callers of `cr_Motion_Play` / `cr_Motion_PlayFromFrame1` take them from tables:
only 0, 0x2BF and 0x3E6 appear as constants), and no file names a motion (names are only in main.dol's
`cr_g_MotionNames`).

- **Cut boss motions.** Rahu II and Rahu III (bodies 31, 32) use all of Rahu I's special motions (get-up, lose, both
  victories, the spin charge `cr_atk_rr111_spinrari`). Their own are on the disc, never played:
  `rr_win_rr112_01/02`, `rr_win_rr113_01/02`, `rr_lose_rr112_01`, `rr_lose_rr113_01`, and two different charge
  attacks, `cr_atk_rr112_ude` and `cr_atk_rr113_ude` ("ude" = arm), 61 frames each.
- **Transform back.** Defender, Seeker, Breaker and Ruhiel play `*_change_skyer_*` (into flight form) as the air dash;
  their `*_change_kamae_*` (back to the stance; ls071, ls072, ls073, ug102) are never in a slot.
- **Seeker's own get-up** `ls_ls072_powerup`: Seeker uses Defender's (`ls_ls071_powerup`).
- **An unused jump** `sf_normal_jump` (31 frames).
- **Older locomotion.** 36 walk / run motions of the SF, MG and AB families without the `_r180` suffix
  (`sf_back_run_guard`, `sf_normal_walk_weapon`, ...): the slots use the `_r180` versions.
- `down_b_kamae`, `ta_dn_f_hi_c_grt` (group 2).
- The list: [data_unused_battle_motions.txt](data_unused_battle_motions.txt).

## Story-mode motions: 59 of 646 never played (checked)

Story people move with group 2 of `logo_read_us.dbn` entry 8 and the 50 pieces of `scenario_us/snr_motion.BIN`
(646 motions; `m_` = men, `w_` = women, `r_` = robos in story scenes). They are played three ways:

- the 9 motion sets `cr_g_CharaMotionSets` (0x8021B160: 56 characters -> 9 sets of 17 at 0x8021D364..: wait, walk,
  run, turn left / right, talk a-e / question / yes / no / oh, battle in / wait / out);
- story script commands: `CHAR_MOT_SET` (character, motion, mode: 0 = index into the character's set, else the
  motion number), `CHAR_SET_WMOT` (a character placed with a motion: argument 13 when argument 14 is 1),
  `C_ACT_MOT` (motion, mode);
- code: `m_pinpon` (0x2BF) and `m_sw_on` (0x3E6), the only motion numbers passed as constants.

Scripts were decoded with `tools/scr.py` (the 300 commands' sizes read from where each handler advances the
script pointer, `tools/scr_sizes.tsv`). The first count (148 never played) followed the code from each entry point
and stopped at the first end, so it missed code reached by jumps; the code of every real chapter script decodes
without a gap from its first entry point to its name table, and decoding all of it finds 89 of the 148 played
(the phone calls `m_keitai_01..04` / `m_call_keitai` in `snr_t_02`, `m_dead`, `m_knockout`, `w_sya_cry_01`,
`m_cry_02`, `r_stop_shinka`, `w_battle_out_lucy` ...). **Never played: 59**, among them `m_battle_in_pose` /
`m_battle_out_pose`, `m_battle_in_guts`, `w_wt_dead_iza` / `w_wt_dead_ira`, `m_tk_sakebi` (a shout), `m_tape_on`,
`m_nage_watch`, `r_weapon_stiff`, `r_battle_01_beat`, the office phone pair `m_is2_ude_tel_pickup` /
`m_is2_ude_tel_put`, and the 6 posed walk variants of the women's set (`w_walk_fa_2_st` ... `_kosiude_m_sp`). The
list: [data_unused_story_motions.txt](data_unused_story_motions.txt).

**Robo motions in story scenes** (`r_`): 10, in `snr_motion` pieces 9, 15 and 17 (scene pieces that also hold the
people's motions). 8 are played, all in A New Journey (`r_battle_01`, `r_jump` in `snr_t_16`; `r_last_battle_in` /
`_out`, `r_fuyu`, `r_weapon_stiff_2` in `snr_t_10`; `r_stop_shinka`, `r_down`, `r_battle_01`, `r_last_battle_in` /
`_out` in `snr_t_18`); `r_weapon_stiff` and `r_battle_01_beat` are never played, though `r_battle_01_beat` is in
all three pieces.

main.dol's motion name table also has `battle_dummy_046` .. `battle_dummy_102` (reserved names) and the script
name tables have `OBJ_DUMMY_*`, `CHAR_DUMMY_*`, `EV_DUMMY_*`, `CM_DUMMY_*`, `LGT_DUMMY_*`, `S_DUMMY_*`, `E_DUMMY_*`,
`F_DUMMY_*`, `SYS_DUMMY_016`, `MAP_DUMMY_016` (reserved ids).

## Stages (checked earlier, stage_tables.md)

40 stage records, 80 `bg_data` entries (2 per stage). Hidden in the select (record +0x80 = 0): stage 6 Basic Arena
(Test Use), a copy of Basic Arena, and stage 7 RPG Test, a grey blockout of the story room in `snr_map` entry 2.

The community (Discord #modding, Nov 8, 2026) exported the hidden and alternate ids as "unused arenas" 06, 07, 0A,
0B: 06 = duplicate of Basic Arena, 07 = a "real-world" room (our RPG Test / office blockout), 0A = "a greybox stage
with placeholders", 0B = "a placeholder plane". This matches stage_tables.md: 0x0A and 0x0B are not unused but the
**alternates** the select swaps in for Basic Arena (Practice Stage, 0x2E9A0-byte model, 10 models) and No-Man's
Land (one-model plane) while a pad bit is held (button not identified). Their export results per stage:
stage_tables.md, "Community stage exports".

## The story file-select: "Start Debug Mode" (checked)

Message file 17 is the story file-select. Messages 3 "Start Debug Mode" and 20 "Start Debug Mode. (Temporary.)" are
never shown: every reference to file 17 in the code is a constant (8, 0xB, 0xC, 0xF, 0x1E, 0x21, 0x22) or a menu
cursor plus a base (0x5.., 0x9.., 0xC.., 0x11.., 0x15.., 0x18.., 0x1A.., 0x1D..: files 1-3 = 17-19, the menu
= 21-23, 24-25 ...), and the bases skip 3 and 20. The debug entry was cut; its text stayed.

## Developer files left on the disc (checked)

| File | What it is | Used by the US game |
|---|---|---|
| `viewer.ini` | HAL Laboratory's model viewer config ("Copyright 1999-2001 HAL Laboratory"; default_model /scene.dat, default_ptcl_dir /particle, pad settings) | not named in main.dol. main.dol's `./viewer` is a folder name the disc file-list builder skips (`fn_800220C4`: `strcmp` with it, then `DVDOpenDir`): no viewer mode behind it |
| `IS USBServer.ini` | Intelligent Systems USB debugger settings (port 8686) | no |
| `gcflag_joy.bin` | a GBA program, header title "ROBO GX", game code ARJJ (the Japanese GBA Custom Robo GX): the link-cable part transfer | not named in the US main.dol: the GBA link was left out of the US release |
| `mess_test.bmg` | placeholder dialogue ("Player line 1..5", "Character 1 line 1..") | not named |
| `ascii_test.tpl`, `debug_ascii.tpl` | debug fonts | not named |
| `scenario/sentence/SNT_TEST.bmg` | 207 Japanese messages: part-data materialise prompts, menu words, the robo / gun / bomb stat explanations, and a glossary from another story (Takuma's school, Yurie's mascot dolls, the Great Robo Cup, a Marine Park tournament, "dark battles", a syndicate whose boss Shinonome was arrested - the N64 game's setting), ending with "a machine that rebuilds two or more robos into one powerful robo; robos made there show their full power through a Dual Dive" | **never read**. main.dol names it (file descriptor 28 in `cr_g_FileDescs`, list 0x801F4880) and the logo sequence `cr_SEQ_LOGO_FrameA` calls `fn_800AF2E0`, which fills a load request for it (0x802299D8: descriptor 28, entry 0, into the buffer at `lbl_8044F314`, callback `fn_800AF2A4`). Nothing hands that request to the loader: 0x802299D8 is used by no other code and no pointer to it exists in main.dol, the callback's "loaded" flag (`lbl_8044F720`) stays 0, the buffer holds the save-data defaults (it is the same address as `lbl_8044F310`), and Dolphin's file log of a boot to the title shows no read of the file. A request left in after the loading it belonged to was taken out |

## Particles: 494 of 1,396 generators never started (checked)

`logo_read_us.dbn` entry 0: 1,396 generators in 8 banks (`tools/ptcl.py`). The game starts particles only through
particle keys in animations (joint track type 40) and through parent generators (`EF` / `F0`): the spawn functions
`psGenerateParticle` / `psGenerateParticle0` / `psGenerateParticleID0` are called only from inside the particle
library, and game code calls only `psInitDataBankLoad`. Every model file on the disc was scanned (models_dat, the 4
common effects, the 94 story maps, the 579 models in `rpg_t_models` / `rpg_f_models`; only `scene_data` roots carry
animations): 1,658 particle keys, all naming real generators; with their children they reach 902 generators.

| Bank | Generators | Started | Never started |
|---|---|---|---|
| 0 gun shots | 150 | 126 | 24 |
| 1 bombs | 57 | 40 | 17 |
| 2 pods | 92 | 88 | 4 |
| 3 leg jets | 147 | 117 | 30 |
| 4 charge attacks | 344 | 207 | 137 |
| 5 common | 13 | 7 | 6 |
| 6 hits / explosions | 575 | 308 | 267 |
| 7 common effect 1 | 18 | 9 | 9 |

The numbers never started: [data_unused_particles.txt](data_unused_particles.txt). (The first particle-key reader,
like HSDLib's FOBJ_Player, read only a track's first key and misread tracks whose header byte continues; fixed in
`tools/scene_data.py` Nov 7.)

## Debug camera: CAMMODE_DEBUG (checked, also in Dolphin)

The camera code has 21 modes (`cr_g_CamModeFuncs` 0x801F7FF0: a start and a per-frame function each; names for
the first 18 in `cr_g_CamModeNames` 0x801F88A4). Mode 0 is `CAMMODE_DEBUG`, a free camera worked from controller 1.

| Mode | Name | Set by |
|---|---|---|
| 0 | CAMMODE_DEBUG | nothing: it is the value `cr_Cam_Init` leaves (it clears the four cameras) |
| 1 | NOMOVE | logo |
| 2, 3, 6, 9, 10 | MIDDLE, OPENING, RESULT, PEAK, DRAW | battle (4 BTL_WAIT, 5 DICE_WAIT and 8 ROT_WAIT are handed on by the modes themselves) |
| 7, 17, 18 | CUSTOMIZE, STAGE_INFO (2nd), - | customize |
| 11-14 | SNR_CHARA, SNR_AUTO_MOVE, SNR_AUTO_Y_ANG, SNR_WATCH_MEM | story (the `CM_*` script commands) |
| 15, 16 | STAGE_SELECT, STAGE_INFO | stage select |
| 19, 20 | - | battle (19), title once its 3D model is loaded (20) |

No code ever writes 0 to the mode byte (byte 0 of the camera, `cr_Cam_SetMode` 0x80024830 and 7 direct writes, all
checked). But `cr_Cam_Update` runs every frame (from `main`, unless sequence flag 0x4000 is set) and calls the
current mode's function, so **the debug camera is running wherever a sequence calls `cr_Cam_Init` and sets no
mode**: the title until its model is loaded (watched in Dolphin's RAM: mode 0 from the title's start, 20 once
`TITLE_MODEL.dat` is read; with no save on the card it waits there at the "no save data" message), and, from the
code, mode select, the main menu and the encyclopedia (they call `cr_Cam_Init` and never set a mode).

Controls (`cr_CAMMODE_DEBUG_Frame` 0x80035ED4 = `fn_8002E2A0`, `fn_8002E564`, `fn_8002EBB0`, `fn_8002E944`;
held in Dolphin on the title's no-save message, the camera block diffed before and after):

| Controller 1 | Effect |
|---|---|
| d-pad up / down | orbit up / down around the target (+0x11C, 0xB6 per frame, stops near 85 degrees) |
| d-pad left / right | orbit left / right (+0x120) |
| Y / X | move the camera and its target up / down together (+0x78, +0x84, +0x9C ...) |
| A or B held | the slow step (0x12 per frame instead of 0xB6), and the stick functions read the stick only then (code; not tried) |

On the screens where it runs nothing 3D is drawn with this camera, so it shows nothing: a developer tool left in,
not a hidden feature. A Gecko code that sets byte 0 of `cr_g_CurrentCam` (0x8044F480 points at it) to 0 during a
battle or the story would give a free camera there (not tried).

## Story mode: the two stories, their maps and chapters (checked)

**Two stories.** Each save file holds two story slots (`cr_g_Scenario` +0x2F, set by `fn_800F21C8(file, slot)`):
slot 0 is "A New Journey" (the main story) and slot 1 "The Grand Battle" (the after-story; the file-select's
message 33 offers it once the main story is done). They have their own chapter scripts and their own copy of
every map:

| | A New Journey | The Grand Battle |
|---|---|---|
| Chapter scripts (`scenario/`, text in `scenario_us/`) | `snr_t_01`..`t_19`, one after another (`SYS_SNR_NEXT`; `t_19` ends with `SYS_ENDING`) | `snr_f_01` is a hub that loads `f_02`..`f_24` on top of itself (`SYS_OVERLAY 21`..`43`; overlay n = script file n, `t_01`..`t_20` = 0..19, `f_01`..`f_30` = 20..49); each returns to it (`SYS_RET_GEKITOU`); `f_23` holds the ending |
| Maps (`scenario_us/snr_map.BIN`) | map m = entry m (0..46) | map m = entry m + 47 (47..93) |

38 of the 47 map pairs are byte for byte the same room; 9 differ: 8 in their model (2 OFC, 5 BAR, 6 BAR02, 10 PLH,
11 NXD, 17 LB03, 30 ZXL, 32 ZXR) and 36 AMP only in its collision block (0x120 bytes longer in The Grand Battle):
the same room as each story shows it (entry 49, the office in The Grand Battle, has no sofas and table). `MAP_SET`, `MAP_LINK` and the area-box code (`cr_AreaBoxes_Update`) pick the entry the same way.

**Map names.** The scripts end with a table of their entry points' developer names (an entry's id + 0x40 is its
name), and the map scripts are named after the map (`map_init_ZXP`, `Map_set_bar_01`, `snr_f_24_map_STF01_init_scr`
...). With the map-link test script below this gives every map's code:

| Map | Code | Map | Code | Map | Code | Map | Code |
|---|---|---|---|---|---|---|---|
| 0 | WORLD (world map) | 12 | RST | 24 | PCR | 36 | AMP |
| 1 | WRD02 | 13 | LBD (from the link order) | 25 | PCT | 37 | AMT |
| 2 | OFC (the office: stage 7 RPG Test is its blockout) | 14 | LBP | 26 | STN01 | 38 | GHT01 |
| 3 | LAB | 15 | LB01 | 27 | STN02 (from the link order) | 39 | GHT02 |
| 4 | PRR | 16 | LB02 | 28 | OBR | 40 | ZXL02 |
| 5 | BAR | 17 | LB03 | 29 | ZXP | 41 | ZXP02 |
| 6 | BAR02 | 18 | LBL (from the link order) | 30 | ZXL | 42 | ZXP03 |
| 7 | TRN (gym) | 19 | PTR | 31 | ZXT | 43 | ZXP04 |
| 8 | PRK (park) | 20 | MHH | 32 | ZXR | 44 | GHT03 |
| 9 | EXM | 21 | PXE | 33 | - (unused, below) | 45 | STF01 |
| 10 | PLH | 22 | PXM (from the link order) | 34 | ZBS | 46 | STF02 |
| 11 | NXD | 23 | PCP | 35 | AME | | |

**Which maps each story uses** (every map command in the code of every chapter script - the code decodes without a
gap from the first entry point to the name table - test scripts left out: `MAP_SET`, `MAP_LINK`, `MAP_NEXT_SET`, `MAP_INIT_SCRIPT`, `MAP_NO_ENTER_SCRIPT`, `MAP_BEF_SCRIPT`,
`MAP_DOOR_OUT` and the `MAP_CLEAR_*` commands):

- A New Journey: every map except 33, 45 and 46 (STF01, STF02: rooms of The Grand Battle only).
- The Grand Battle: every map except 18 (LBL), 25-28 (PCT, STN01, STN02, OBR) and 33.
- **Map 33 is used by neither**: entries 33 and 80 (identical) are 0x2220 bytes, with no collision block and no
  area table: one untextured grey square of 38 x 38 units, a placeholder.

The map entry layout (header +0x00 collision offset, +0x04 model, +0x08..+0x14 x / z bounds, +0x18 the last block or
0): the last block is a 0xE0-byte table (88 of the 94 entries; 50 of them follow it with a second small `scene_data`
model); it is not door data. Doors and areas are set up by the scripts (`EV_SET_SQARE`: box number, corners, the script to run; `MAP_LINK`
puts a destination map in the box).

**Characters and objects** (`scenario_us/snr_chara.BIN` 56 characters, `snr_obj.BIN` 89 objects; the characters
named by `CHAR_SET`, `CHAR_SET_WMOT`, `CHAR_SET_WMOT_Y` and `CHAR_SET_MODEL`, the objects by `OBJ_SET`,
`OBJ_SET_CHARA_POS`, `OBJ_SET_RANGE`, `OBJ_SCALE_SET` and `OBJ_SCALE_SET_CHARA_POS`, in the whole code of every
chapter script; the code also sets object 88 (`fn_8005C384`) and 73 (`fn_8009AEC4`) itself):

- Every map also loads its own model pack with it (`MAP_SET`: `scenario_us/rpg_t_models.BIN` entry m for A New
  Journey, `rpg_f_models.BIN` for The Grand Battle; 48 entries, entry 47 shared by all maps). A pack is a count,
  then (id, offset) pairs, then the models: id 0x0001nnnn = object nnnn, 0x0000nnnn = character nnnn.
- Characters: every one is placed by a script or carried by a map pack except 50 and 54, which are byte for byte
  the same file (a spare slot, unused). 13-16 and 58, 60, 63 appear only in A New Journey's scripts, 18 and 55 only
  in The Grand Battle's (58, 60 and 63, in `snr_t_05` and `snr_t_07`, are not `snr_chara` entries and no pack or loader fills those
  slots of `cr_g_CharaEntries`; `cr_Chara_BuildModel` draws a number whose slot is not loaded as character 0, so
  from the code they show as character 0 - not checked in the game).
- Objects: 27 entries (3, 9-31, 36, 51, 52) are copies of one 2,592-byte placeholder (one textured square), never
  used. Every real object is used: those no script places come with a map pack: 54-63 the world map's place signs
  ("Steel Hearts", "Hero's Home", "Lab", "Test Hall", "Training Gym", "Bogey's Cafe", "Daimon", "Hub Park",
  "Police Squad HQ", "Harry's Home") and 64-66 ("Dome", "Amusement Park", "Z's Hideout") with WRD02, the office
  furniture 81-83 (desks, the red sofa, a table) only in The Grand Battle's office pack (its office model has no
  furniture of its own), 79-80 with The Grand Battle's PRR, 47 (a framed handprint) with PCT, 48 (a long building)
  with STN01 / STN02, 85 (a tea set) with PTR, 41 with BAR02, and the shared pack 47 holds 2 (the parts machine)
  and 7 (the red guide arrow).

**Test scripts left in the chapter list** (each script file carries the date it was built, at +0x20):

| File | Built | What it is |
|---|---|---|
| `snr_f_25` .. `snr_f_28` | 2003/4/14 | four copies of a command test script (`cmd_test_spt_start`, `event_obj_test`, `main_script_2..9`, `chara_a_script` ...); their "US" text files are still the Japanese test messages |
| `snr_f_29` | 2003/6/3 | a map-link test: one entry per door of the map network of that time (`map_link_ofc_world`, `map_link_lbp_lb01`, `map_link_stn02_obr` ... `world_link_set`), and a name entry |
| `snr_f_30`, `snr_t_20` | 2003/8/20 | the same 216-byte blank chapter: map 0, one character, start, loop |

None of them is ever loaded: `t_20` would follow `t_19`, which ends the story, and the only overlays of The Grand
Battle are `f_02`..`f_24` (the hub's `SYS_OVERLAY 21`..`43`; `SYS_OVERLAY` takes its number straight from the
command). `f_29` itself overlays `t_01`, `t_03` and `f_01` (0, 2, 20): it was a tool for walking the maps.

## Still to trace

- Characters 58 / 60 / 63 in `snr_t_05` and `snr_t_07`: confirm in the game that they show as character 0.
