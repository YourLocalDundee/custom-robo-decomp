# Robo-indexed tables: what a new robo slot needs (plan.md step 71)

A "robo" here is a **body** (the robo part that carries the model, the stats, the motions; plan.md Phase 2b item 6).
Bodies are numbered 0..33. This page lists every table, file and constant found that is indexed by the body
number or sized by the body count, with the code that proves it, and ends with a checklist for adding body 34.

How it was searched (Oct 30, 2026): from the loaders (cr_CharaEntry_OnLoaded / cr_ModelEntry_LoadAndBuild,
fn_800A63C0's `chara.bin` entry `part x 5`), every `mulli` by a body-record size (0x78, 0x134) and every compare /
loop bound with 0x21 / 0x22 in the game code (dtk asm, `decomp/build/GXCE01/asm`), every data object whose size is a
multiple of 34 (dtk symbols), every archive descriptor of `cr_g_FileDescs` with 34 or 170 entries, the body number's
own fields (battle robo +0x1CC, loadout +2, save +0xBD) in the Ghidra export, and the community leads
(docs/community_codes.md, docs/community/codes_addresses.md). Names below are the ones applied in step 71
(symbols/descriptive.txt, symbols/cr_globals.txt); `lbl_` / `fn_` names are kept where a C file of decomp/src uses
the address by name (renaming it there would break another stream's objects) or where the meaning is not proven.

## The bodies

The codes are the game's own (file names of the body pictures, `sprite/parts_illustrated_book/body_tpl/CR_S1_<code>.tpl`);
the US names are message file 9 of `bmg/info_usa.bin` (entry = body number). The stats table's own names (Shift-JIS and
romaji development names) are listed in [name_evidence.md](name_evidence.md#robo-bodies-the-games-own-body-codes-and-stats-names-by-body-number).

| body | code | US name | | body | code | US name |
|---|---|---|---|---|---|---|
| 0 | SF011 | Ray 01 | | 17 | TF063 | Juggler |
| 1 | SF012 | Splendor | | 18 | LS071 | Defender |
| 2 | SF013 | Glory | | 19 | LS072 | Seeker |
| 3 | AB021 | Milky Way | | 20 | LS073 | Breaker |
| 4 | AB022 | Earth | | 21 | FB081 | Seal Head |
| 5 | AB023 | Sol | | 22 | FB082 | Dour Head |
| 6 | MG031 | Metal Ape | | 23 | FB083 | Tank Head |
| 7 | MG032 | Metal Bear | | 24 | SP091 | Ray Legend |
| 8 | MG033 | Metal Ox | | 25 | SP092 | Oil Can |
| 9 | LR041 | Swift | | 26 | SP093 | Ray Warrior |
| 10 | LR042 | Shrike | | 27 | UG101 | Rakansen |
| 11 | LR043 | Peregrine | | 28 | UG102 | Ruhiel |
| 12 | SV051 | Javelin | | 29 | UG103 | Athena |
| 13 | SV052 | Glaive | | 30 | RR111 | Rahu I |
| 14 | SV053 | Halberd | | 31 | RR112 | Rahu II |
| 15 | TF061 | Criminal | | 32 | RR113 | Rahu III |
| 16 | TF062 | Buggy | | 33 | SP094 | Chickenheart |

Eleven families of three (01 SF .. 11 RR) plus SP094, which looks added late: its battle motion set is set 27 (the
sets of bodies 27-33 are shifted by one), and its menu place is fixed by cr_g_PartOrderBody, not by its number.

## Tables and files indexed by body

"Grows?" = must get one more entry for body 34. Sizes are in bytes.

| # | where | element x count | read / written by | meaning and proof | grows? |
|---|---|---|---|---|---|
| 1 | `chara.BIN` (archive 1) | entry, 5 per body (170) | fn_8010DE04 (entry = variant + body x 5, via the part load requests lbl_8023BA84), fn_800A63C0 (part x 5), loader cr_ModelEntry_LoadAndBuild | the body models; the 5 entries per body are picked by a variant byte (menu state +0x47E5); entry sizes differ inside a body (colour variants likely, not proven) | yes, 5 entries |
| 2 | main.dol's entry table of `chara.BIN` at 0x801F1C88 | u32 count, u32 0, then 170 x {offset, size} | cr_Archive_OpenEntry / GetEntrySize through cr_g_FileDescs[1] (+0xC points here) | the game reads offsets and sizes from this copy, not from the disc header (plan.md Phase 1); it ends at 0x801F21E0, where the file descriptors start | yes: 5 records; it cannot grow in place, move it and repoint desc[1] +0xC |
| 3 | cr_g_BodyData 0x801C5C10 | 0x78 x 34 (0xFF0) | cr_BattleRobo_InitFromLoadout, cr_BattleRobo_InitBody, cr_BattleRobo_InitFromMenu (copy to robo +0x17F0), cr_CAMMODE_CUSTOMIZE_Start / Frame, fn_800A63C0 | per-body battle data: +0 / +4 floats frame the robo in the customize and viewer cameras, word 2 = pointer to the body's battle motion set (copied to robo +0x554), then floats | yes |
| 4 | cr_g_BattleMotionSets 0x801C51B8 | 0x4C x 34 | through #3 word 2; fn_8004B0A0 plays set[cr_g_BattleMotionSlot] | 38 motion IDs per body (plan.md step 46) | yes (or point the new body at an existing set) |
| 5 | body stats, lbl_801C6C44 | 0x134 x 34 (0x28E8, ends exactly at the leg table 0x801C952C) | cr_BattleRobo_InitFromLoadout (copy to robo +0x1B98), fn_800F7914 (part check, `mulli 308`), fn_800666B8 / fn_800AA17C / fn_800AA49C (+0x45) | +0x00 Shift-JIS name, +0x20 romaji name (0x20 each), +0x40 u16 (1000 Ray 01 .. 30000 Rahu III), +0x45 special-part byte (1 for SP091, SP093, UG10x, RR11x); the community's stat codes write inside it. **from CRBR-Parts-Expansion**: +0x45 is the **illegal-part flag** (PartExpansion.asm line 1367 "Illegal Byte Control" replaces fn_800666B8's `lbz r0, 0x45(r7)`; Body/Ray 01.asm line 38 "Illegal Toggle"); +0x40 .. +0x133 (0xF4 bytes) are the battle stats ("RoboBytes"), field by field in Body/Ray 01.asm lines 31-119, which equal main.dol's Ray 01 record byte for byte (+0x40 u16 "Down", +0x56 air-dash type, +0x57 continuous jumps, +0x5E air dashes, +0xB0 charge name, +0xD2 charge values; table in docs/community/crbr_parts_expansion.md); cr_BattleRobo_InitFromLoadout's copy loop (77 halfword pairs) is where the mod switches the source after +0x40 (PartExpansion.asm line 1564) | yes; no room after it (the leg table follows), so move it and fix the 6 users |
| 6 | cr_g_PartOrderBody 0x8023B73A | u16 x 35 (34 + 0xFFFF) | cr_Records_BuildBodyRows, the part menus fn_800F8820 / fn_80101E00 / fn_80103594 / fn_80104028 / fn_80109314 / fn_8010A5CC, fn_80105784, fn_801086A4, fn_8010DF48 | menu / scoreboard order of the bodies; entry 0 of cr_g_PartOrderLists (0x8023B8BC); the gun list follows directly. **from CRBR-Parts-Expansion** (README.md lines 10-29, PartExpansion.asm lines 96-107 and 1680-2752): the list is the part select's **slot -> part ID map**; the mod repoints cr_g_PartOrderLists[0..4] to 253-entry lists and hooks 210 direct users, `addi rX, r31, 0x3732` / `lhz rX, 0x3732(r31)` (r31 = 0x80238008; 0x3778 / 0x37E2 / 0x3836 / 0x3890 for the other lists) in the functions here plus **fn_80106EB8**; our scan of main.dol finds 30 more such users in fn_80109314 (0x801093A8 .. 0x80109A0C) that the mod leaves alone, and cr_Records_BuildBodyRows uses `addi r0, r3, -0x48C6` (0x8011706C) | yes: one u16 before the 0xFFFF; move it (the gun list follows) and repoint cr_g_PartOrderLists[0] and the direct users |
| 7 | `attack_effect.BIN` (archive 18) + its main.dol entry table 0x801F3778 | entry x 34 | fn_80075950 (opens entry [loadout body] for each of the 4 robos) | the body's attack effects; read into a 0x14000-byte buffer per robo (largest retail entry 0x13B80) | yes, 1 entry (+ table record; the table ends at 0x801F3890, the next descriptor) |
| 8 | `sprite/ending/us/robo_tpl/` (archive 31) | file x 34 (0x10-byte descs at 0x801F3960, 0-terminated) | cr_Ending_LoadPartPictures (entry = loadout[0] body) | ending picture of player 1's robo; JP set under ending/jp | yes, a new .tpl, a descriptor and the terminator moved (0x801F3B80 is followed by the gun list 0x801F3B90) |
| 9 | `sprite/parts_illustrated_book/body_tpl/` (archive 36) | file x 34 (descs at 0x801F5090) | loaded through the sprite request lists (fn_8008FDEC / fn_80090190, not traced to the index) | the Parts Index pictures, one per body in body order (file names = codes above) | yes (index by body assumed from the order, not traced) |
| 10 | message file 9 of `bmg/info_usa.bin` (and `info.bin`) | message x 34 at the start of 236 | cr_Text_GetBodyName (message [body]); the gun / bomb / pod / leg names add cr_g_BodyCount | part names: 34 bodies, 64 guns, 52 bombs, 57 pods, 29 legs | yes: insert at 34, everything after moves and the counts keep it right |
| 11 | message file 0 of the same files | message x 34 at the start of 248 | cr_Text_GetPartDescription (index = the same running index), fn_800F9014 | part descriptions, then "Unknown" texts after the 236 parts | yes, insert at 34 |
| 12 | cr_g_BodyCount 0x8044C3C0 (.sdata) | s32 = 34 | cr_Text_GetGunName / BombName / PodName / LegName, cr_Text_MaskUnownedSpecialPart, fn_800095F4, fn_80011E08, fn_800F9014, fn_8010A5CC | the body count; the other four counts follow (0x8044C3C4 legs 29, C8 guns 64, CC bombs 52, D0 pods 57) | yes: 35 |
| 13 | cr_g_BodyBattleRecords 0x803BE7FC (.bss, saved) | {battles, wins, losses} u32 x 64 | cr_Battle_UpdateBodyRecords (+1, cap 999), cr_Records_BuildBodyRows (34), cr_Save_ResetData (clears 64) | the Vs. scoreboard per body; part of the 0x1138-byte save block 0x803BE7DC | no: 64 slots reserved |
| 14 | part bitsets 0x803BF79C / 0x803BFB9C (.bss, saved), category 0 | 4 x 64 bits per category | fn_800A3C0C (new game), cr_Save_ResetData, fn_800A63C0, cr_Text_MaskUnownedSpecialPart, the scoreboard fn_801166E4 (0x803BF79C bit [body]: name or ???) | owned / seen bits of each body (AR code "All Body Types" fills 0x803BFB9C) | no: 64 bits per set |
| 15 | cr_Records_BuildBodyRows 0x80117034 | loop bound 0x22 (twice) | - | the scoreboard walks 34 bodies (`cmpwi r29, 0x22`); its row buffer 0x8041C450 has 0x3C0 bytes (40 rows of 0x18) before the total row 0x8041C810 | yes: the constant (the buffer has room) |
| 16 | cr_Text_MaskUnownedSpecialPart 0x800F8E4C | body numbers 0x1E, 0x1F, 0x20 | - | Rahu I-III are hard-coded as the special bodies that show Unknown until owned | no (only if the new body is special) |

### Tables indexed by the battle robo (0..3), which hold a body number

| where | element x count | meaning |
|---|---|---|
| battle loadouts lbl_801C5BD0 | 0x10 x 4 | +2 body, +4 gun, +6 bomb, +8 pod, +0xA leg, +0xC HP, +0xE controller (cr_BattleRobo_InitFromLoadout, fn_80075950, cr_Ending_LoadPartPictures) |
| battle robos lbl_8024E120 | 0x33520 x 4 | body at +0x1CC (u16), gun / bomb / pod / leg +0x1CE.. +0x1D4, HP +0x74 (community AR code), body data copy +0x17F0, body stats copy +0x1B98 |
| part load requests lbl_8023BA84 | 0x14 x 20 + end | body requests use archive 1 and a 0x3A000-byte buffer per robo (largest retail body entry 0x39000, body 20). The 20 records and the 0xFF end record end at 0x8023BC28; dtk's object (0x344 bytes) also holds a 13-pointer menu handler table (0x8023BC28, read by fn_801076D8) and zeros 0x8023BC70 .. 0x8023BDB3, which **CRBR-Parts-Expansion** uses for its pointers and counters (PartExpansion.asm lines 4-10) |
| model slots lbl_8024DFA0 | 0x58 x 4 | filled by cr_ModelEntry_LoadAndBuild per robo |
| 0x801DE868 / 0x801DE9F0 | same layout | a second set of weapon / body request lists; no code or data reference found |

### Not robo tables (corrections to plan.md Phase 2b item 6)

- **cr_g_CharaMotionSets (0x8021B160) and cr_g_CharaYOffset (0x8021B080), 56 entries**: indexed by the *story
  character* model number, not by body. 56 = the entries of `scenario_us/snr_chara.bin` (archive 3) = the 56
  `SNRM_001_BIN ..` names of the game's own table 0x8021AFA0; cr_Chara_BuildModel takes that model number. A new robo
  does not touch them (a new story character would).
- **Unlock flags 0x80321D44** (AR "Unlock Bonuses"): a mode-unlock word (cr_Save_ResetData sets it to 0xF), not per body.
- **leg_effect.BIN** (archive 22) has 34 entries but fn_80075950 indexes it by the **leg** number (loadout +0xA; 29 legs).

## What must grow for a new slot (body 34)

Files (a mod folder, `make_mod.py`):
1. `chara.BIN`: 5 new entries (170 -> 175), each at most 0x3A000 bytes (the per-robo buffer of the request table).
2. `attack_effect.BIN`: entry 34 (at most 0x14000 bytes).
3. `bmg/info_usa.bin` (and `info.bin`): message 34 inserted in file 9 (name) and file 0 (description).
4. `sprite/ending/us/robo_tpl/` (+ jp) and `sprite/parts_illustrated_book/body_tpl/`: one picture each.

main.dol data (DOL patch; most tables are packed between other data, so they move to new space and their users are repointed):
5. `chara.BIN`'s entry table (0x801F1C88): +5 records; repoint cr_g_FileDescs[1] +0xC. The same for `attack_effect.BIN`
   (0x801F3778, desc 18) and the two picture lists (descs 31 and 36: a new 0x10-byte descriptor each).
6. cr_g_BodyData: record 34 (0x78), with word 2 pointing at a battle motion set (a new 0x4C set, or an existing one).
7. Body stats (lbl_801C6C44): record 34 (0x134), names at +0x00 / +0x20; moving the table means fixing its users
   (fn_80005DA4, fn_800666B8, fn_800A0E64, fn_800AA17C, fn_800AA49C, fn_800F7914).
8. cr_g_PartOrderBody: 34 inserted before the 0xFFFF (a 36-halfword copy, repoint cr_g_PartOrderLists[0] and the direct users: 42 r31-relative ones in the menu functions (the list the mod hooks, see #6), 6 more in fn_80109314, and cr_Records_BuildBodyRows 0x8011706C).
9. cr_g_BodyCount = 35.
10. cr_Records_BuildBodyRows: both loop bounds 0x22 -> 0x23 (its row buffer has room for 40).

Already big enough: the save data (64 records, 64 bits per category), message lookups (they follow cr_g_BodyCount).

## Notes from CRBR-Parts-Expansion (step 72)

docs/community/crbr_parts_expansion.md has the full map. Facts the repo (CRBR-Parts-Expansion e05d633) proves for this page:

- **It adds menu slots, not bodies.** Every body slot carries a "PartBase" 0..0x21 (README.md lines 66-68; all 34 + 1 custom body files, and Project Hive's 43). The base keeps everything in the table above except the stats tail (+0x40 ..), the menu name / description and the menu preview model; none of the 16 grows. A slot is a cheap "new robo" (new stats, text, model choice) on an existing body's motions, effects, pictures and records.
- **The model number is separate from the body number** in the part select's model requests (menu state lbl_80407260 + n x 0x4994, +0x47F0 + k x 12; the mod's "Part Model Control V2" at fn_8010DF48 +0x17C / +0x378 writes a model ID there instead of the part ID). Whether battle takes the model from there is not shown.
- **The other part tables** have the same "base part / stats part" split (the mod's Robobytes Control, PartExpansion.asm lines 1564-1678, switches each copy loop of cr_BattleRobo_InitFromLoadout at the offsets below; bases from fn_800666B8): leg 0x801C952C (0x8F x 29, stats from +0x80), gun 0x801CA560 (0x28C x 64, from +0xA0), bomb 0x801D4860 (0x17C x 52, from +0xA0), pod 0x801D9590 (0x174 x 57, from +0xE0); the first stats byte is each table's illegal flag.
- **Free space they use**: code at 0x800048E0 (.init, inside gTRKInterruptVectorTable, zero to 0x80004AAC) and data words at 0x8023BC70 .. 0x8023BD9E; the grown tables live in the code list above 24 MB (Dolphin's 32 MB override).

### The stat triples and the leg values (Nov 2, 2026)

**Verified from the code**: fn_80006658 (called once, by cr_BattleRobo_InitFromLoadout, with the robo's body record copy robo + 0x1B98, its leg record copy robo + 0x33308 and an output block robo + 0x341C) turns 14 low / mid / high triples of the body record into the robo's stats. Each stat has one leg byte v (leg record +0x81 .. +0x8E): v <= 100 gives low + v x (mid - low) / 100, else mid + (v - 100) x (high - mid) / 100, so v = 0 / 100 / 200 picks low / mid / high and the values between blend linearly; the result is stored as s16.

| Body record triple | Type | Leg byte | Output (robo + 0x341C +) | Ray 01 low / mid / high |
|---|---|---|---|---|
| +0x70 | s16 x 3 | +0x81 | +0x00 | 48 / 80 / 96 |
| +0x76 | s16 x 3 | +0x82 | +0x02 | 90 / 150 / 240 |
| +0x7C | s16 x 3 | +0x83 | +0x04 | 100 / 200 / 400 |
| +0x82 | s16 x 3 | +0x84 | +0x06 | 120 / 250 / 375 |
| +0x88 | s16 x 3 | +0x85 | +0x08 | 4 / 8 / 16 |
| +0x8E | s16 x 3 | +0x86 | +0x0A | 100 / 140 / 180 |
| +0x94 | s16 x 3 | +0x87 | +0x0C | 50 / 100 / 325 |
| +0x9A | s16 x 3 | +0x88 | +0x0E | 10 / 20 / 40 |
| +0xA0 | u16 x 3 | +0x89 | +0x10 | 50 / 100 / 150 |
| +0x50 | u8 x 3 | +0x8A | +0x12 | 3 / 6 / 10 |
| +0x53 | u8 x 3 | +0x8B | +0x14 | 5 / 15 / 30 |
| +0x58 | u16 x 3 | +0x8C | +0x16 | 100 / 160 / 264 |
| +0x61 | u8 x 3 | +0x8D | +0x18 | 5 / 10 / 15 |
| +0x64 | u16 x 3 | +0x8E | +0x1A | 150 / 300 / 396 |
| +0x6C | s16 | - | +0x1C (copied) | - |

Checked on main.dol: every one of these 14 triples is non-decreasing in all 34 retail records. The mod's field list (docs/community/crbr_parts_expansion.md, +0x64 row) names stats in this area (air-dash speed, turning, run speed, ground acceleration, jump height, jump speed, air speed, air acceleration, landing lag, gravity), but it groups them from +0x64 in steps of 6, which the code does not do (+0x6A / +0x6E are not read here), so its names are not mapped to rows yet. A community note's Ray 01 jump triple 125 / 250 / 375 matches no row exactly; the +0x82 row is 120 / 250 / 375. Readers of the output block (displacement scan of every function): fn_80049924 (the movement function) reads +0x00 .. +0x0E, +0x12 and +0x18; fn_80045CE8 +0x0A / +0x0C; fn_8004B0A0 +0x10; fn_80048B10 +0x14 / +0x16; fn_80046F58, fn_8004752C, fn_80047AC0, fn_80048050, fn_80048708 +0x14 / +0x1A / +0x1C. Leads for names (not proven): +0x08 (triple +0x88) and +0x18 (triple +0x61) are each stored as a byte to robo +0x13A, a count next to the dash count robo +0x13B of docs/community/codes_addresses.md; +0x12 (triple +0x50) is stored to robo +0xE0 and +0xE2; +0x00 / +0x02 and +0x04 are converted to float in the movement maths. Names applied (Nov 2, 2026): fn_80006658 = cr_BattleRobo_ApplyLegValues; the output block is cr_BattleRobo.leg_stats (cr_RoboLegStats, 0x1E bytes at robo + 0x3341C); the triples are cr_BodyStats.leg_tri_* and the leg bytes cr_LegData.leg_values[14] (symbols/cr_types.h). The fields read directly elsewhere: +0x0A6 / +0x0A8 (fn_8003B528, fn_8004E8D4, fn_80062FF4), +0x0AC (7 battle functions), +0x0AE (fn_8003B528). The lead's address 0x801C6C64 is +0x20 of record 0, the romaji name (cr_g_BodyStats = 0x801C6C44).

## Still unknown (next steps)

- **The menus' own limits.** The part-select and Parts Index code (fn_800F8820, fn_80101E00, fn_80103594, fn_80104028,
  fn_80109314, fn_8010A5CC, fn_80105784) walk cr_g_PartOrderBody to its 0xFFFF; whether a screen also has a fixed
  number of rows / icons, or a page size, is not checked.
  **Partly answered from CRBR-Parts-Expansion**: the menus page by 3 bodies / 4 other parts (their hooks compute slot = page x 3 or x 4 + cursor) and still end at the list's 0xFFFF; the mod allows 253 slots per type (counters are bytes) and Project Hive's sources (project-hive, `PartData/CRBR/PartList.asm`) ship 43 body slots, so the menus have no fixed 34-row limit. Not covered: the Parts Index (archive 36) and the scoreboard.
- **Which body picture index the Parts Index uses** (archive 36): loaded through the sprite request lists, not traced.
- **Body-specific code**: compares of the body number with constants other than 0x1E-0x20 (bosses, Oil Can,
  Chickenheart) were not searched exhaustively; story / arcade opponent tables (e.g. 0x8022B3B8 + n x 0x8C, community
  lead) and scenario scripts hold body numbers but do not need to grow.
- **What the 5 `chara.BIN` entries per body are** (variant byte +0x47E5 of the menu state, set from a value mod 3 in one
  path): colours are likely, not proven.
- **The model / motion side** of a new body: the 19 role bones and the 38 battle motion slots are known (docs/MODDING.md,
  plan.md step 46); the body's limb data (block A) lives in the model entry, not in main.dol.
- **Free space** for the moved tables: plan.md Phase 2b "Free space for new code / data".
