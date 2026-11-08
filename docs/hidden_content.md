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
| Story animations | up to 148 of 646 never played (phone calls, crying, knock-outs, posed walks ...) | mostly yes (upper bound) |
| Particles | 494 of 1,396 particle effects never started | yes (or leftovers) |
| Debug menu | "Start Debug Mode" text on the story file screen, never shown | the entry was cut; only its text remains |
| Developer files | HAL model viewer config, debugger config, test fonts, placeholder dialogue, the Japanese GBA link program ("ROBO GX") | leftovers; the US game never loads them |
| Test text | `SNT_TEST.bmg`: early Japanese system text with a glossary from the N64 game's story and a cut "robo fusion / Dual Dive" machine | loaded at every boot; whether shown is open |

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

## Story-mode motions: 148 of 646 never found played (checked, upper bound)

Story people move with group 2 of `logo_read_us.dbn` entry 8 and the 50 pieces of `scenario_us/snr_motion.BIN`
(646 motions; `m_` = men, `w_` = women, `r_` = robos in story scenes). They are played three ways:

- the 9 motion sets `cr_g_CharaMotionSets` (0x8021B160: 56 characters -> 9 sets of 17 at 0x8021D364..: wait, walk,
  run, turn left / right, talk a-e / question / yes / no / oh, battle in / wait / out);
- story script commands: `CHAR_MOT_SET` (character, motion, mode: 0 = index into the character's set, else the
  motion number), `CHAR_SET_WMOT` (a character placed with a motion: argument 13 when argument 14 is 1),
  `C_ACT_MOT`;
- code: `m_pinpon` (0x2BF) and `m_sw_on` (0x3E6), the only motion numbers passed as constants.

Scripts were decoded with `tools/scr.py` (new: the 300 commands' sizes read from where each handler advances the
script pointer, `tools/scr_sizes.tsv`); 8,296 motion commands found. 1,757 of the 7,112 entry points of the
script files do not decode to the end (1,342 are not code: data tables), so a few motions counted as never played
may be played from code the walk did not reach. Never found played: 148, among them whole scenes' worth of
acting (`m_keitai_01..04` / `m_call_keitai` phone calls, `m_dead`, `m_knockout`, `w_sya_cry_01`, `m_cry_02`,
`w_wt_dead_iza`, `r_stop_shinka`, `r_battle_01_beat`, `m_battle_in_pose` / `m_battle_out_pose`, `w_battle_out_lucy`)
and 7 posed walk variants of the women's set (`w_walk_fa_2_st` ... `_kosiude_m_sp`). The list:
[data_unused_story_motions.txt](data_unused_story_motions.txt).

main.dol's motion name table also has `battle_dummy_046` .. `battle_dummy_102` (reserved names) and the script
name tables have `OBJ_DUMMY_*`, `CHAR_DUMMY_*`, `EV_DUMMY_*`, `CM_DUMMY_*`, `LGT_DUMMY_*`, `S_DUMMY_*`, `E_DUMMY_*`,
`F_DUMMY_*`, `SYS_DUMMY_016`, `MAP_DUMMY_016` (reserved ids).

## Stages (checked earlier, stage_tables.md)

40 stage records, 80 `bg_data` entries (2 per stage). Hidden in the select (record +0x80 = 0): stage 6 Basic Arena
(Test Use), a copy of Basic Arena, and stage 7 RPG Test, a grey blockout of the story room in `snr_map` entry 2.

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
| `scenario/sentence/SNT_TEST.bmg` | 207 Japanese messages: part-data materialise prompts, menu words, the robo / gun / bomb stat explanations, and a glossary from another story (Takuma's school, Yurie's mascot dolls, the Great Robo Cup, a Marine Park tournament, "dark battles", a syndicate whose boss Shinonome was arrested - the N64 game's setting), ending with "a machine that rebuilds two or more robos into one powerful robo; robos made there show their full power through a Dual Dive" | **loaded on every boot**: file descriptor 28 (`cr_g_FileDescs`, list 0x801F4880); the logo sequence `cr_SEQ_LOGO_FrameA` calls `fn_800AF2E0`, which queues the request at 0x802299D8 (descriptor 28, entry 0, callback `fn_800AF2A4`). Whether any of its text is shown in the US game is not traced |

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

## Still to trace

- `CAMMODE_DEBUG`: camera mode 0 of `cr_g_CamModeNames` (0x801F88A4, 18 modes) has its own update function
  (0x80035ED4 in `cr_g_CamModeFuncs`); whether anything sets mode 0 is not traced.
- Whether any `SNT_TEST.bmg` message is shown (it is loaded at boot; its first messages are the part-data download prompts).
- Story maps, characters and objects no script loads. From the decoded scripts (`tools/scr.py grep MAP_SET MAP_LINK
  CHAR_SET OBJ_SET ...`): maps 1, 11, 24-28, 30, 31, 33, 36-38, 41, 43, 46 and character models 16, 38, 46, 50, 54
  never appear in a command - not conclusive: maps are also entered through door / area data inside the maps (the
  last block of a `snr_map` entry, not decoded) and part of the script code is not reached by the walk.
- The 8 `snr_map` pairs whose two versions differ (2/49, 5/52, 6/53, 10/57, 11/58, 17/64, 30/77, 32/79): entry 49
  is entry 2 without the sofas and table, so they look like the same room at two points of the story.
- The 13 robo-motion `snr_motion` pieces and the `r_*` story robo motions.
