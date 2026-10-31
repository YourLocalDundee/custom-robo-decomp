# CRBR-Parts-Expansion (DesiacX) and Project Hive's sources: what they change, mapped to our decomp (plan.md step 72)

Sources studied (Oct 30, 2026), cloned with `git clone --depth 1` into the gitignored `tools/refs_new/` (never committed):

- **https://github.com/DesiacX/CRBR-Parts-Expansion** (`tools/refs_new/CRBR-Parts-Expansion`, commit e05d633, "Robobytes Control Bugfix", Sep 17, 2024). Created Aug 18, 2022; 30 commits, one branch (`main`), 5 stars, **no licence file** (all rights reserved by default: we study it and cite it, we do not copy its code into the repo).
- **https://github.com/0seiken0/project-hive** (`tools/refs_new/project-hive`, commit c18b5de, Mar 30, 2026): **Project Hive's own sources are public.** It is a GitHub fork of CRBR-Parts-Expansion, 19 commits ahead and 0 behind (branches `main`, `vanilla`, `ph3`, `ph3dot1`, `ph3dot2`), also without a licence. docs/community/discord_codes.md and plan.md wanted to "ask the community for PH's `.asm` sources": they are here.

All addresses below were checked against the retail `main.dol` (`decomp/orig/GXCE01/sys/main.dol`: the original instruction at each hook, disassembled with our pinned gc-wii-binutils objdump) and mapped to our names (`decomp/config/GXCE01/symbols.txt`, symbols/descriptive.txt, symbols/cr_globals.txt). The scratch script (`scratchpad/scripts/addrs.py`, not committed) found 344 address uses in the core files: 47 codes, 63 distinct functions and data objects.

## What it is

| | |
|---|---|
| Format | **GCTRealMate (GCTRM) assembly**, the Brawl / Project+ code language (`HOOK @ $addr { asm }` = a C2 insert, `op ... @ $addr` = a 04 write, `CODE @ $addr` = code copied to a fixed address, `PULSE` = run once per frame, `.BA<-label` / `.BA->$addr` = store a label's address at addr, `half[] / byte[] / word[]` = data). `GXCE/Codes/GXCE01.asm` includes `Source/**.asm`; `GCTRealMate.exe` (bundled) builds `GXCE01.GCT` (152,464 bytes in the repo) and a plain Gecko listing `codeset.txt` (410 KB). |
| Size | 213 part files (34 bodies, 52 guns, 41 bombs, 44 pods, 16 legs = every retail menu part, plus 5 custom: Swift DX, Dragon Fist, Plus One Legs C / S / U, Standard Pod F), `Source/Parts/PartExpansion.asm` (2,804 lines, the engine), 13 utility codes in `GXCE01.asm`, 6 extras, stage disablers |
| Tools | `GXCE/Converters/*Convert.py` (5 Python scripts, pyperclip + numpy): paste a part's raw "RoboBytes" hex, get the commented `.asm` part file |
| Applied by | **Riivolution on Dolphin** (`GXCE.xml`: `<memory valuefile="GXCE01.GCT" offset="0x81800000">` and 0x800000F0 = 0x02000000) with Dolphin's "Emulated Memory Size Override" = **32 MB**; a one-line Gecko code in Dolphin's ini ("External Codeset", `Source/Extras/Dolphin Codes/ExternalCodeset.asm`) makes Dolphin's code handler run the list at 0x81800008. On hardware: GeckoLoader v7.1.0-sup39 patches the GCT into `main.dol` (codehandler Full, hook GC, "optimize codes" off). Also two Riivolution folder patches (`/sound` replacement, extra disc files). |
| What it changes | **New parts without new part IDs.** README.md lines 10-29: the menu's "physical slot" used to select a part ID, and name, description, model and RoboBytes all followed the part ID; the mod gives each *slot* its own base ID, model ID, name, description and RoboBytes, so a type can have up to 255 slots. No new bodies (robos) by ID: every body slot reuses one of the 34 retail body numbers as its "PartBase" (README.md lines 66-68: the base gives the hard-coded behaviour, e.g. the body's charge attack, the gun's bullet model). |
| Limits (theirs) | README.md lines 31-32: "no way to add unique models / GFX / SFX"; every part looks like an existing one. |
| Depends on | Stands alone (Project Hive depends on it, not the other way). |

## Headline: how the parts list grows

The game's part menus walk **cr_g_PartOrderBody .. cr_g_PartOrderLeg** (0x8023B73A ..), our "menu order lists", through the pointer table **cr_g_PartOrderLists** (0x8023B8BC). The mod proves what these lists are: the **slot -> part ID map** of the part select (README.md "How it Works"). Growing it:

1. **New slot tables in the code list** (in RAM at 0x81800000+, the expanded 32 MB; on hardware wherever GeckoLoader puts the codes). PartExpansion.asm lines 96-776: per type a 253-entry u16 base-ID list (0xFFFF-ended, like ours), a 252-entry name-pointer list, a description-pointer block, a 252-entry RoboBytes-pointer list and a 253-byte model-ID list.
2. **Repoint the game**: `.BA->` writes the new lists' addresses into **cr_g_PartOrderLists[0..4]** (the five words at 0x8023B8BC; lines 98-107), and 6 new pointer words each at 0x8023BD50 (names, + descriptions at 0x8023BD64), 0x8023BD68 (RoboBytes), 0x8023BD7C (models).
3. **Hook every direct user**: "Part Expansion V2" (lines 1680-2752) replaces **210 instructions** in 10 menu functions that reach the five lists not through the pointer table but as `addi rX, r31, 0x3732 / 0x3778 / 0x37E2 / 0x3836 / 0x3890` (address of the list) or `lhz rX, same(r31)` (first entry), with r31 = 0x80238008 (lbl_80238008, the module's data base). 31 `addi` + 11 `lhz` per list. Functions: fn_80109314 (45), fn_80101E00, fn_80103594, fn_80104028 (30 each: CPU difficulty / type changes, left and right), fn_800F8820, fn_8010A5CC (20 each), fn_80106EB8 (15: exit with B, arcade), fn_8010DF48 (10), fn_80105784, fn_801086A4 (5 each).
4. **Fill the tables at boot**: each part file is a PULSE that stores its name / description / RoboBytes addresses at 0x8023BCF0..0x8023BCF8, saves r0-r31 at 0x8023BC70 (`stmw`), and calls the "Custom Parts Master Code" at **0x800048E0**, which appends one slot (counters per type at 0x8023BD98, bytes). "Part Expansion Closer" sets 0x8023BD9E = 0xFF so the setup runs once.
5. **Make the rest follow the slot, not the part ID**: names in the menus (fn_80105784 +0x23C, fn_800F96A4 +0x3A4), descriptions (cr_Text_GetPartDescription +0x68), the preview models (fn_8010DF48 +0x17C, +0x378), the "illegal" marks (fn_800666B8, 10 hooks), the stat bars and body-type label (fn_800FCE50, fn_800FBFD4), and **in battle the RoboBytes** (cr_BattleRobo_InitFromLoadout, 5 hooks: see below).

What still follows the **base ID** (the body number 0..33 the loadout carries into battle): everything in docs/robo_tables.md except the stats tail, the menu text and the preview model, i.e. cr_g_BodyData, the battle motion set, the body stats' names (+0x00..+0x3F), attack_effect.BIN, the ending and Parts Index pictures, the records, the bitsets. That is why no table of our 16 had to grow.

## Address map

"Agrees" = what it shows about our named thing. Offsets are from the function or object start (our names). Retail instruction = main.dol at that address.

### The parts engine (Source/Parts/PartExpansion.asm)

| Their address(es) | Our name + offset | Retail instruction / content | What the code does | Agreement with us |
|---|---|---|---|---|
| 0x8023B8BC .. 0x8023B8CC (`.BA->`, lines 98-107) | cr_g_PartOrderLists[0..4] | pointers to cr_g_PartOrderBody .. Leg | repointed to 253-entry lists of base IDs in the code list | **confirms** our table #6 and the pointer table; proves the lists are the slot -> part ID map; shows the repoint works |
| 210 hooks 0x800F8894 .. 0x8010E2B4 (lines 1708-2752) | fn_80109314, fn_80101E00, fn_80103594, fn_80104028, fn_800F8820, fn_8010A5CC, fn_80106EB8, fn_8010DF48, fn_80105784, fn_801086A4 | `addi rX, r31, 0x3732..0x3890` / `lhz rX, 0x3732..(r31)` (r31 = 0x80238008) | load the new list's address instead | **extends** our users list of #6 with **fn_80106EB8** and the exact instructions. Not hooked (our own scan of main.dol for the same offsets): **30 more in fn_80109314 at 0x801093A8 .. 0x80109A0C** (two more blocks of the same pattern; a path the mod leaves on the retail lists, not identified) and **cr_Records_BuildBodyRows 0x8011706C** (`addi r0, r3, -0x48C6` = 0x8023B73A; the scoreboard stays by body number). The pointer-table users fn_801019D8 / fn_80105288 / fn_80108F7C (0x80101C88, 0x801054B4, 0x80108FA4) follow the repointed table. |
| CODE 0x800048E0 (line 26) | .init, inside gTRKInterruptVectorTable (0x800034AC .. 0x800053E0) | zeros 0x800048E0 .. 0x80004AAC (0x1CC bytes) | the master function (append one slot) | **free-space lead**: unused MetroTRK vector-table space in .init used for code |
| 0x8023BC70 .. 0x8023BCEF (register save), 0x8023BCF0 .. 0x8023BCFB, 0x8023BD50 .. 0x8023BD8F, 0x8023BD98 .. 0x8023BD9E | lbl_8023BA84 +0x1EC .. +0x31A (dtk object 0x8023BA84, size 0x344) | all zero in main.dol; no code reference found (refs.tsv: the object's last referenced words are 0x8023BC28 .. 0x8023BC60, a 13-pointer menu handler table read by fn_801076D8) | pointer slots and counters of the mod | **corrects** our sizes: the part load requests (20 x 0x14 + the 0xFF end record at 0x8023BC14) end at 0x8023BC28; dtk's object also holds that handler table and ~0x150 zero bytes. **Free-space lead** (0x8023BC70 .. 0x8023BDB3; runtime use by unseen code not excluded) |
| Name / description hooks 0x801059C0, 0x800F9A48 (+ nop 0x800F9A4C), 0x80066E34; 0x800F9700, 0x800F972C (lines 1153-1291) | fn_80105784 +0x23C, fn_800F96A4 +0x3A4 / +0x3A8, cr_Text_GetPartDescription +0x68, fn_800F96A4 +0x5C / +0x88 | `lwz r3, 0x10(r1)`, `li r3, 1`, `lwz r17, 0x178(r31)`, `add r29, r0, r29`, `lbzx r0, r31, r4`, `stbx r3, r31, r0` | menu name and description from the slot's pointer (fallback 0x817FC817 when 0); force the description to redraw | **confirms** cr_Text_GetPartDescription (description by running index); fn_800F96A4 is the part-select name / description drawer |
| Model hooks 0x8010E0C4, 0x8010E2C0 (lines 1294-1364) | fn_8010DF48 +0x17C, +0x378 | `lbz r3, 0x4866(r31)`; `li r29, 0` after the five `lhzx` from the order lists | zoom preview and the 5 part models from the slot's model ID | **extends**: fn_8010DF48 builds 5 model requests at menu state +0x47F0 + n x 12 (u8 reload flag, u16 part at +2) from the order lists; the model number is a separate value the mod can set apart from the base ID |
| Illegal Byte Control V3: 0x800667B0 / D8 / 800 / 828 / 850 + `b` at 0x800667A4 / CC / F4 / 81C / 844 (lines 1367-1542) | fn_800666B8 +0xF8 .. +0x198 | `lbz r0, 0x45 / 0xA0 / 0xA0 / 0xE0 / 0x80(r7)` after `mulli` 308 / 652 / 380 / 372 / 143 | read the "illegal" byte from the slot's RoboBytes | **confirms and names** body stats +0x45 ("special-part byte" in our #5) = the **illegal-part flag** (README / Ray 01.asm line 38 "Illegal Toggle"); gives the other four part-stat tables (below) |
| 0x801C0A88, 10 halfwords 0x00FF (line 1552) | lbl_801C0A88 (.rodata, 0x10) | retail {0x19, 0x21, 0x1F, 0x23, 0x0A} | disables fn_800666B8's "this part ID gets the special colour (95, 190, 255)" compare | **new**: one part ID per category with its own menu colour; body 0x19 = 25 = Oil Can, so likely the "Can" parts (gun 33, bomb 31, pod 35, leg 10 not checked by name) |
| Robobytes Control V2: 0x80005F40, 0x80005F80, 0x80005FC8, 0x80006010, 0x80006058 (lines 1544-1678) | cr_BattleRobo_InitFromLoadout +0x19C / +0x1DC / +0x224 / +0x26C / +0x2B4 | the five record-copy loops: `lhz r4, 2(r5)` (body, 0x4D pairs of halfwords), `lwz r4, 4(r5)` (gun 81 / bomb 47 / pod 46 pairs of words), `lbz r3, 1(r4)` (leg 71 pairs of bytes) | when the loop counter reaches 0x3D / 0x3D / 0x1B / 0x12 / 7, switch the source to the slot's RoboBytes (slot = menu state of this robo +0x4857 / 59 / 5B / 5D / 5F) | **confirms** our cr_BattleRobo_InitFromLoadout (the record copies to robo +0x1B98 etc.) and fixes the **split of every part record**: the first 0x40 (body), 0xA0 (gun, bomb), 0xE0 (pod), 0x80 (leg) bytes always come from the base ID, the rest (the "RoboBytes") from the slot |
| 0x80005F54 "Plus One Legs" (line 2754) | cr_BattleRobo_InitFromLoadout +0x1B0 | `lwz r6, -0x5078(r13)` | if the loadout's leg is 0x10, add 1 to the robo's stats copy +0x57 and +0x5E | **confirms** robo +0x1B98 + 0x57 (continuous jumps) / + 0x5E (air dashes) from docs/community/discord_codes.md; loadout +0xA = leg (our battle-loadout layout) |
| 0x800F8D24, 0x800F9A60 (line 2776) | fn_800F8C68 +0xBC, fn_800F96A4 +0x3BC | `lbzx r0, r29, r31`, `lbzx r0, r17, r27` | `^` / `` ` `` in names drawn as the illegal symbols 0x81 / 0x8F | name drawers, as above |
| 0x8023BD9E (Closer) | lbl_8023BA84 +0x31A | 0 | set-up done flag | - |

### Utility codes (GXCE/Codes/GXCE01.asm)

| Code (line) | Their address(es) | Our name + offset | Retail | What it does / agreement |
|---|---|---|---|---|
| Universal Settings Modifier v1 (8) | 0x801C6C00 .. 0x801C6C42, 0x800466A4 | lbl_801C6C00 (0x44 bytes, directly before the body stats), fn_80045FEC +0x6B8 | `li r6, 3` | **new**: lbl_801C6C00 = global battle constants: u16 damage multiplier (+0), floats gravity / jump (+4, +8), 27 u16 (gravity while shooting, invulnerability after down, bomb lock time, bomb marker speed (+0x12 = 0x801C6C12, Zach777's "reticle speed" agrees), pod-shot lock, lava bounce / downtime / damage, friction, acceleration, ice, tag rebirth, HP regen cap 150 (+0x3E), **tag cooldown 420** (+0x40)); also the tag-while-downed invulnerability frames = 3. Agrees with discord_codes.md's 0x80045BE4, where retail loads the tag cooldown with `lha r0, 0x40(r3)`: that is this field |
| Stagelist Toggle (46) | 0x8023CF00, 0x8023F0E8, 0x80AA28CA / 0x80AA29DC | lbl_8023CF00 (0x28), lbl_8023F0C0 +0x28 | 37 stage numbers | the stage select's order (37 entries + end); the toggle tests the halfword 0x8023F0E8 == 0x156 (an option value) and relabels that option "Stage Hazards"; the two strings are in the heap (loaded message text) |
| Unlock Everything (81) | 0x80321D44, 0x803BF79C .. 0x803BF83C | lbl_80321D40 +4, lbl_803BE7DC +0xFC0 .. +0x1060 | - | known (docs/community_codes.md): unlock word + the part bitsets of #14 |
| Illegal Parts enabled on startup (92) | 0x800F1098 | cr_Save_ResetData +0x2AC | `stb r20, 0x43(r21)` | the save's "illegal parts" option byte (+0x43 of the options block) starts as 1 |
| Boot to Main Menu v1.1 (98) | 0x80023C88, 0x800089B4 | cr_Game_InitSystem +0x284, cr_Game_Update +0x228 | `stw r8, 8(r7)`, `lwz r4, 0x38(r5)` | starts at sequence 5 (main menu) / 0x120: consistent with both step-71 names |
| Synchronized Spawns V2 (117) | 0x800898E0, nop 0x80089900, 0x80085448 | fn_80089550 +0x390 / +0x3B0, fn_8008538C +0xBC | `sth r29, 0x184(r30)`, `sth r11, 0x186(r30)`, `lwz r3, 0x150(r31)` | +0x184 of the object set to 0x1A0 for every robo (same spawn timing, per the code's title) |
| HP 70 for Tag in VS (134) | 0x8010B204 (reads 0x8041C0AF, 0x8041CBFC, 0x8044F884) | fn_8010A5CC +0xC38 | `clrlwi r7, r9, 16` | default HP; 0x8041C0AF = matches played, 0x8041CBFC = main-menu choice (2 = VS), 0x8044F884 = VS choice (2 = tag); agrees with discord_codes.md |
| Temporarily Disable Illegal Parts by holding R (158) | 0x800F79A0, 0x800F7A1C, 0x800F7A98, 0x800F7B14, 0x800F7B90 | fn_800F7914 +0x8C .. +0x27C | `lbz r0, 0x43(r3)` | the illegal-part check per category reads the option byte +0x43 (above); menu state +0x46B8 = pad buttons |
| Remember Parts Selection (198) | nops at 0x80109B18, 0x80109BE8, 0x80109CD4, 0x80109DC0, 0x80109E68 | fn_80109314 +0x804 .. +0xB54 | `sth r0, 0x4856 .. 0x485E(r30)` | **new struct fields**: menu state +0x4856 .. +0x485E = the 5 selected slots (u16 each); fn_80109314 resets them on entry |
| Default Color is Original (207, off) | 0x80109E78 | fn_80109314 +0xB64 | `lbzx r3, r4, r6` | colour variant (cf. +0x47E5) |
| Extra Bytes - Body Type (212) | 0x800FC170 (reads 0x8023BD68) | fn_800FBFD4 +0x19C | `cmpwi r0, 7` | label Normal / Armor / Speed / Other per body slot from the slot's extra byte: a **display label, not behaviour** (this is what discord_codes.md called "robo classes") |
| Build Title V2 (292) | 0x800F8C88, nop 0x800F8C94 (compares with 0x8023B498) | fn_800F8C68 +0x20 / +0x2C | `li r25, 0`, `lwz r4, 8(r1)` | build-title string per slot; 0x8023B498 = lbl_8023B350 +0x148 (a text pointer) |
| Stat Lines (341) | 0x800FD0B4, nop 0x800FD1CC | fn_800FCE50 +0x264 / +0x37C | `lbzx r5, r20, r3`, `bne 0x800FD2BC` | stat bars from the slot's extra bytes (bodies 1-10, others made 1-10 too) |
| External Codeset / Closer V2 (416) | 0x800000F0 | OS globals (memory size) | 0x01800000 | 32 MB check, then back to 24 MB so the OS arena stays below the codes |

### Extras and stages (GXCE/Codes/Source/Extras, Source/Stages)

| Code | Our name + offset (retail) | Note |
|---|---|---|
| Camera Freeze | fn_8002F320 +0x488 / +0x48C, cr_SEQ_BATTLE_FrameC +0x4C / +0x50 | battle camera |
| Cannon Manipulation Fix | fn_8008538C +0xBC (`lwz r3, 0x150(r31)`) | same hook as Synchronized Spawns |
| Fastfall | fn_8004B0A0 +0x138 (`lfs f1, 4(r5)`) | fn_8004B0A0 = the battle motion player of robo_tables #4 |
| Cannot Tag while (downed and) in Mid Air | fn_80049924 +0x168C, fn_80045FEC +0x680 (`lha r0, 0x146(r3)`) | robo +0x146 (s16) is the value tested |
| Stage Striking v1.1, Z-Alt Stages | fn_80111040 +0x878 / +0x9F4 / +0xA54 / +0xA98, fn_80111EEC +0x104 | stage select |
| Stage disablers | fn_80070170 (+0x7A0 .. +0x1724, 16 hooks on `bl 0x80197FD8` and `lwzx` of stage objects), fn_80072A58 +0xAC, fn_8004B0A0 +0x19A0 / +0x1D10 / +0x23FC, lbl_8037569C +6 / +9 | stage objects; PH adds fn_80071974 +0x6B8 (Castle Keep wall) |

### Project Hive only (0seiken0/project-hive, GXCE/Codes)

Same engine (one fix: PartExpansion.asm line 1251 `add r30, r3, r30` instead of `add r3, r3, r30` in the description hook), 43 body / 52 gun / 54 bomb / 46 pod / 20 leg slots in `PartData/CRBR/PartList.asm`; every body slot's PartBase and ModelID are still 0x00..0x21 (no new body ID); body types 0-3 used (15 / 13 / 14 / 10). New codes: Disable Memory Card Check (0x800EB47C, fn_800EB158 +0x324, `bl 0x800ECA28`: already in discord_codes.md), Quick VS Menu [Glass_Knuckle] (AR, 0x8044F874 .., 0x8041CBEE / 0x8041CBFC), Loading Dock Platform Delete and Castle Keep wall removal [seiken] (0x803756A2, 0x8041A238, 0x8034DD68, 0x8007202C), Damage Multiplier 900 -> 1000, Synchronized Spawns V2.1 (0x1A5).

## Part records: the layout the mod documents

The "RoboBytes" of a part are the tail of its record in the part tables; the mod's part files spell out every field. **Body, checked byte for byte**: `PartData/CRBR/Body/Ray 01.asm` lines 31-119 equal main.dol's Ray 01 record (lbl_801C6C44) from +0x40 to +0x133.

| Body record offset | Field (their comment) |
|---|---|
| +0x00 / +0x20 | Shift-JIS / romaji names (ours, from the base ID, not in RoboBytes) |
| +0x40 u16 | "Down" (1000 Ray 01 .. 30000 Rahu III: our "u16 at +0x40") |
| +0x42 .. +0x44 | defense, knockback, hitstun (percent) |
| +0x45 | **illegal flag** (1 = banned when illegal parts are off) |
| +0x46, +0x47 | time down without / with mashing |
| +0x48 .. +0x4B | damage taken while down from gun / bomb / pod / charge |
| +0x4C .. +0x4F | gun / bomb / pod damage, gun end lag |
| +0x50 .. +0x55 | air-dash start-up, dash landing lag (reduced / normal / boosted) |
| +0x56 | air-dash type (0 normal, 1 continuous jump, 2 LS) |
| +0x57 | number of continuous jumps |
| +0x58 u16 x 3 | continuous-jump height |
| +0x5E .. +0x63 | number of air dashes, SV air-dash toggle, air-dash angle, air-dash length x 3 |
| +0x64 u16 x 38 | air-dash speed, LS turning, run speed, ground acceleration, ground turning, jump height, jump speed, lateral air speed, air acceleration, landing lag, gravity (x 3 each), collision box x 2, ?, hurtbox size, ? |
| +0xB0 char[32] | charge name (Shift-JIS) |
| +0xD0, +0xD1 | charge invulnerability, invisibility |
| +0xD2 s16 x 16 | charge: damage, knockback velocity / angle, homing, phase durations / speeds / angles, hitbox |
| +0xF2 .. +0x133 | a second (unused) charge: name, 2 bytes, 16 values |

The other part tables (bases from fn_800666B8's `lis` / `addi`, record sizes from its `mulli` and from cr_BattleRobo_InitFromLoadout's loops; the split from Robobytes Control; the field lists in the mod's Gun / Bomb / Pod / Leg files and converters):

| Table | Base | Record x count | Base-ID part | RoboBytes part | Illegal flag | Copied to battle robo |
|---|---|---|---|---|---|---|
| body | 0x801C6C44 | 0x134 x 34 | +0x00 .. +0x3F | +0x40 .. +0x133 (0xF4) | +0x45 | +0x1B98 |
| leg | 0x801C952C | 0x8F x 29 | +0x00 .. +0x7F | +0x80 .. +0x8E (0xF) | +0x80 | +0x33308 |
| gun | 0x801CA560 | 0x28C x 64 | +0x00 .. +0x9F | +0xA0 .. +0x28B (0x1EC) | +0xA0 | +0x1E28 |
| bomb | 0x801D4860 | 0x17C x 52 | +0x00 .. +0x9F | +0xA0 .. +0x17B (0xDC) | +0xA0 | +0x28D48 |
| pod | 0x801D9590 | 0x174 x 57 | +0x00 .. +0xDF | +0xE0 .. +0x173 (0x94) | +0xE0 | +0x2E2E8 |

The tables follow each other (0x801C6C44 + 34 x 0x134 = 0x801C952C, + 29 x 0x8F = 0x801CA553 (padded to 0x801CA560), + 64 x 0x28C = 0x801D4860, + 52 x 0x17C = 0x801D9590). README.md line 11: guns, bombs, pods and legs each have about 12 hidden development parts and bodies none, which matches our counts (64 / 52 / 57 / 29 IDs against 52 / 41 / 44 / 16 menu entries; 34 / 34 bodies).

**Menu state (part select), lbl_80407260, 4 x 0x4994 (size 0x12650)**, indexed by battle-robo number in Robobytes Control: +0x46B8 pad buttons, +0x47E5 colour variant (ours), +0x47F0 + n x 12 model requests, +0x4854 (byte, re-read by the CPU-change code), +0x4855 menu state (0 = top menu), +0x4856 .. +0x485E five u16 selected slots (low bytes +0x4857 .. +0x485F), +0x4860 cursor, +0x4861 sub-menu part type, +0x4862 (s16), +0x4863 page (3 bodies or 4 other parts per page), +0x4866, +0x4868 page part type.

## What it teaches us for Phase 2b item 6 (a new robo slot)

1. **Two kinds of "new robo".** (a) Theirs: a new *menu slot* that reuses a body number. It needs no table of our 16 to grow, only the order list and the slot data, and it gives new stats, name, description, stat bars, type label and a model choice, but the base body's motions, charge code, effects and pictures. (b) Ours: a new *body number 34*, for its own motions, effects, pictures and records. (a) is a working, cheap first step (and it is the route the community uses: Project Hive has 43 body slots on 34 body numbers); (b) still needs docs/robo_tables.md's checklist.
2. **The order lists can grow and be moved**: the menus page by 3 bodies / 4 parts (the mod's hooks compute slot = page x 3 or x 4 + cursor) and the lists stay 0xFFFF-ended. 43 body slots in Project Hive (and up to 253 in the mod) work in the menus. That answers part of our open "menus' own limits" question. Moving cr_g_PartOrderBody means: repoint cr_g_PartOrderLists[0] **and** the 31 `addi` / 11 `lhz` r31-relative users per list (the mod's list of 210 is a ready-made map), the 30 in fn_80109314 the mod leaves alone, and cr_Records_BuildBodyRows (0x8011706C).
3. **The model number is separate from the base ID** in the menu's model requests (fn_8010DF48, menu state +0x47F0). With chara.BIN grown (5 entries, the entry table moved: our checklist item 5), a slot with base ID n and model 34 may load a new model while keeping body n's behaviour. Lead, not tested: whether the battle loads the model from these requests or from the loadout's body number (fn_8010DE04 / lbl_8023BA84) is open.
4. **Stats per slot, in battle**: the hook points in cr_BattleRobo_InitFromLoadout (the five copy loops, and the counter values that mark the base / RoboBytes split) are where a new body's record would be picked up; with a moved stats table we would instead change the `lis` / `addi` / `mulli` of its six users (robo_tables item 7).
5. **Free space they use**: (a) code at 0x800048E0 inside .init's gTRKInterruptVectorTable (0x1CC zero bytes to 0x80004AAC); (b) data words in the zero tail of lbl_8023BA84 (0x8023BC70 .. 0x8023BDB3); (c) for the big tables, the expanded RAM above 24 MB (Dolphin only) or the GeckoLoader-patched DOL. For a DOL patch, (a) and (b) are candidates next to plan.md's "new text section" plan; check that MetroTRK never copies its vector table in retail (`__TRK_reset`, `EnableMetroTRKInterrupts`) before using (a).
6. **Ready-made field names** for cr_types.h: the body record layout above (+0x40 .. +0x133), the illegal flag in all five tables, lbl_801C6C00's battle constants, the menu-state fields. They are leads under our naming policy: each needs our own proof (most are values a code changes, not code that reads them).

## Open questions

- The 30 order-list users in fn_80109314 0x801093A8 .. 0x80109A0C that the mod does not hook: which menu path (a mode the mod does not support, or one that only reads entry 0)?
- Does the battle model come from the menu's model requests (+0x47F0) or from the loadout body? (Test: a slot with base 0, model 1, in battle.)
- The description block: the master code puts type t's descriptions at byte t x 0x100 (64 words per type), so a type with more than 64 slots would overwrite the next type's descriptions (PartExpansion.asm lines 66-72); the declared block is 1,275 words. Their limit is 64 described parts per type, not 255.
- Their part files keep the "extra bytes" 4 bytes after the RoboBytes (1 for legs: Stat Lines reads +0xF8 / +0x1F0 / +0xE0 / +0x98 / +0x10); harmless, not explained.
- lbl_801C0A88's gun / bomb / pod / leg IDs (33, 31, 35, 10): check that they are the Can parts by name (message file 9).
- The part bitsets (#14) and the records (#13) stay by base ID: two slots with the same base share "owned" and the scoreboard row.

## Textures

Short answer for the textured-robo work: **neither repo adds, swaps or recolours a texture by code.** Searched: every `.asm`, the converters, README.md, GXCE.xml and the shipped files of both repos (`texture`, `.tpl`, `tlut`, `CMP`, `palette`, `colour`).

- **No texture data, texture slot tables or texture-to-model tables** anywhere in the code. A part's look is chosen only by its **ModelID** byte (the body / part model number, PartExpansion.asm "Part Model Control V2", 0x8010E0C4 / 0x8010E2C0 in fn_8010DF48), i.e. a whole model entry with the textures inside it. README.md lines 31-32: no unique models / GFX / SFX; every custom part is "visually derivative" of an existing one (Swift DX = Swift's body with ModelID 0x09, Plus One Legs = ModelID 0x0A).
- **Colour variant**: "!Default Color is Original" (GXCE01.asm line 207, off by default) changes `lbzx r3, r4, r6` at 0x80109E78 (fn_80109314 +0xB64) to `li r3, 0`: the menu's colour pick, which (our robo_tables #1) selects one of the 5 `chara.BIN` entries of a body via menu state +0x47E5. So the per-body colours are separate model entries, not palette swaps; a code can only pick which of the 5 loads.
- **The only texture code** is stage-side: "Stage Animation Disabler" / "Stage Specific Disablers" (Source/Stages/StageDisablers.asm lines 1-345) skip or keep calls of **HSD_JObjAnimAll** (0x80197FD8) from fn_80070170 (+0x1104 .. +0x1724) to freeze texture / joint animations of stage objects (Battle Gear Station, Merry-Go-Round shadow, Dead Line and Checkmate Foundry conveyors, Panic Walls). That is HSD's own animation of the loaded .dat (texture-animation AObjs inside the stage model), not texture swapping.
- **File route instead of codes**: both repos' Riivolution XML has a "Disc File Replacement / Addition" patch (`<folder external="./GXCE" disc="/">`): any file in `GXCE/` replaces the disc file of the same path ("Disc File Replacement.txt"; the example is `sound/CUSTOM.SSM` + `sound_data.sem`). That is how a modded `chara.BIN` / `.tpl` would ship with their setup; it is the same idea as our `make_mod.py` `files/`.
- **Project Hive's texture pack** is a separate Dolphin hash-based pack on Google Drive (docs/community/discord_codes.md "Tools"), not in the repo; no texture tool or script is in either repo.

For our model worker this means: textures must live inside the robo's model entries (HSD TObjs in `chara.BIN`, our existing route: edit the TObj / re-encode CMP, docs/MODDING.md "Blender loop"), one copy per colour entry (the 5 entries per body), or as a Dolphin hash pack for Dolphin-only play. Nothing in these repos maps textures to bodies beyond "model entry = body x 5 + colour".

## Similar repos and forks (GitHub scan, Oct 30, 2026)

Searched: the fork network of DesiacX/CRBR-Parts-Expansion (`gh api .../forks`, `compare`), DesiacX's repos, `gh search repos` for "custom robo", "crbr", "GXCE01", "custom robo battle revolution", "project hive custom robo", "custom robo decomp", "custom robo model", "MeltyTool", and `gh search code GXCE01`. Ranked by use to us:

| # | Repo | What it is | Activity | Ahead / different | Verdict |
|---|---|---|---|---|---|
| 1 | https://github.com/0seiken0/project-hive | Project Hive (the tournament patch) sources: fork of CRBR-Parts-Expansion | Jan 9 - Mar 30, 2026; 19 commits ahead, 0 behind; branches vanilla / ph3 / ph3dot1 / ph3dot2; no licence | ahead: PH 3.0-3.2 part files (43 bodies), a description-hook fix, stage / menu / memory-card codes | **the most useful**: the PH `.asm` sources we wanted, same engine, newer part data |
| 2 | https://github.com/DesiacX/CRBR-Parts-Expansion | the original (this page) | Aug 2022 - Sep 2024; 30 commits; no licence | - | the engine and the record layouts; studied above |
| 3 | https://github.com/Sylk/project-hive | fork of project-hive | Jan 14, 2026; 1 ahead, 6 behind | "Re-arranging folders for mono-repo structure" only | nothing new |
| 4 | https://github.com/NastyKactus/project-hive | fork of project-hive | Jul 6, 2026; identical to 0seiken0 main | none | nothing new |
| 5 | https://github.com/alexander-clawthorne/custom-robo-recomp | static recompilation of Custom Robo (GXCE01) to C for the New 3DS (GPL-2.0, C) | created Oct 21, 2026, pushed Oct 29, 2026; 100+ commits | a different goal (recomp, not matching decomp) | worth a later look for its `docs/sdk_functions.md` / `recomp_unknowns.md` and its runtime's view of the hardware; no table addresses seen in the README |
| 6 | https://github.com/0seiken0/crbr-overlay-manager | OBS overlay tool reading parts from memory (vanilla and PH part lists) | Mar 30, 2026 | - | already in discord_codes.md; a part-ID list cross-check at most |
| 7 | https://github.com/Zoeyism/custom_robo_model_swapper | Python model swapper (rpg models, same-size replacement) | Jun 2024 | - | already in docs/community_codes.md; superseded by our make_mod.py |
| 8 | https://github.com/MeltyPlayer/MeltyTool | multi-game asset viewer / extractor (N64 / GCN / 3DS) | active (Oct 31, 2026) | Custom Robo appears only in its title list | no Custom Robo specific support found |
| - | DesiacX/Codes, DesiacX/PowerPC-Assembly-Functions | Brawl codes (2021), a fork of a PPC asm helper library | 2021 | - | not Custom Robo |
| - | pizdex/robogx, Ganenis/Custom-Robo-GX-English | Custom Robo **GX** (GBA) decomp / translation | 2026 | another game | not for GXCE01 |
| - | OdeRelic/OdeRelic, SilverCrow2323/dolphin-rt-core (`GXCE01.txt` / `.ini`) | emulator cheat databases | - | the same 15 AR codes + 16:9 code we already have | nothing new |

No other Custom Robo (GameCube) decomp or table map was found on GitHub. The original's `forks_count` is 3, but only one direct fork exists; the other two are the forks of that fork (rows 3-4).
