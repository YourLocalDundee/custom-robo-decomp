# Custom Robo Discord: codes, addresses and tools (GXCE01)

Source: the Custom Robo community Discord's codes / resources channels, Oct 2024 - Oct 2026, pasted into the project by the user on Oct 29, 2026. Credits are the authors' handles as given in each code's title or post. The full code listings stay with their authors (Discord and the linked sheets); this file keeps only what each code touches, what it does, and what that tells us about the game. Every hook address was mapped to our decomp (`decompiled/index.db`), and the original instructions were checked against `main.dol` (scratch scripts `map_hooks.py`, `chk_orig.py`).

**Status: naming leads, not names.** A community description says what a code *changes*. Before it becomes a `cr_` name it still needs our own proof (plan.md, naming policy).

## Key findings

- **Codes are not limited to ~6 KB.** Project Hive (PH), the tournament-standard patch (3.0 -> 3.2, Mar 28, 2026), ships a ~140 KB `GXCE01.GCT` loaded through Riivolution (`<Dolphin user>/Load/Riivolution/GXCE/Codes/GXCE01.GCT`, built with GCTRealMate from `.asm` sources). It adds new parts, Oil Can, rebalanced stats, "parts expansion data" with per-part flags, and (in the test builds) 4 new robo classes. That corrects `docs/community_codes.md`, which assumed Dolphin's cheat area is the limit. **New parts and new robo types are already possible through codes; models still go through files.**
- **Robo stats table: `0x801C6C44`, one 0x134 (308)-byte record per robo body, indexed by "part base", Ray 01 first.** Proven from the game's own code: `0x800F79BC mulli r4, r31, 308` / `lis r3, 0x801C` / `addi r0, r3, 0x6C44` (all three instructions checked against main.dol). Stat codes (Misty Mirage, Phantom Sniper and Fleeting Figment tests by Xithyl_kykorl) write fields inside it (0x801C6C84 .. 0x801C6DE0, 0x801C824A ..). The bomb aim / reticle speed is at 0x801C6C12 (Zach777).
- **The four robos in battle: `0x8024E120 + n x 0x33520`** (robo 1 0x8024E120, 2 0x80281640, 3 0x802B4B60, 4 0x802E8080; players own robos 1+2 and 3+4 in tag). This agrees with the AR research (the HP float at 0x8024E194 = robo 1 + 0x74). The robo data block is at robo + 0x1B98: air action type +0x56 (0 normal, 1 jump, 2 sky), dash counts +0x57 / +0x5E, and current dash count at robo + 0x13B (seiken).
- **Player struct map:** Xithyl_kykorl's spreadsheet "custom robo stuff" (https://docs.google.com/spreadsheets/d/1wftGjEh58oSUvOF4vcxFUW98QAlnAkCRMur5VlR4Cpo). This is the best lead for `cr_` struct fields; read it before naming robo-instance fields.
- **Stat locations for Dolphin Memory Engine:** Glass_Knuckle's `Custom Robo Stats.dmw` (taken from PH3's .asm files); the stats sheets for PH 3.1 / 3.2 (seiken, Glass_Knuckle) list every part's raw stats.

## Code hooks mapped to our functions

| Address | Our function (+offset) | What the code changes (author) | Original instruction (main.dol) |
|---|---|---|---|
| 0x800058C8 | FUN_800057b8 +0x110 | pad input: Z / X modifiers, Z-Charge (DesiacX, kactus) | |
| 0x80005ACC | FUN_800057b8 +0x314 | charge start, Z-Charge (kactus) | |
| 0x800081BC / 0x800081D0 | FUN_8000805c +0x160 / +0x174 | down time reduced by mashing / by one input (seiken) | 0x800081D0 `sub r3, r0, r29` OK |
| 0x8001F988 | FUN_8001f758 +0x230 | part select: quick swap builds (seiken) | |
| 0x80035EA0 | **cr_CAMMODE_MIDDLE_Frame** +0x2D4 | nop freezes the battle camera's rotation (seiken, TideHunter) | (confirms our step 45 camera-mode name by behaviour) |
| 0x8003D0A8 | FUN_8003c4f0 +0xBB8 | gun vertical tracking value, robo + 0x118 (seiken) | `stfs f0, 0x118(r3)` OK |
| 0x80045BE4 | FUN_80045b40 +0xA4 | neutral tag cooldown, 420 frames (seiken) | retail is `lha r0, 0x40(r3)`, not `li r0, 420`: the cooldown is loaded from data here; the code was likely written against PH. **Unverified** |
| 0x80046070 | FUN_80045fec +0x84 | down time countdown (seiken) | `subi r0, r3, 1` OK |
| 0x80048560 | FUN_80048050 +0x510 | gun vulnerability / shot cancel per class (Xithyl_kykorl) | |
| 0x800499CC / 0x8004A538 | FUN_80049924 +0xA8 / +0xC14 | dash from ground / match start, dash refill (Xithyl_kykorl, seiken) | 0x8004A538 `lwz r3, -0x5078(r13)` OK |
| 0x8004B7C0 | FUN_8004b0a0 +0x720 | damage applied while not downed, damage variance (DesiacX) | |
| 0x8004FBF4 | FUN_8004f6d4 +0x520 | HP regen after tagging out below 150 (seiken) | |
| 0x800636E4 / 0x80063744 | FUN_80063654 +0x90 / +0xF0 | X / Y axis movement per class (Xithyl_kykorl) | |
| 0x800651A8 / 0x800656D0 | FUN_80064fe4 +0x1C4 / FUN_80065688 +0x48 | text box: hold A/B advances / skips to the end (DesiacX) | |
| 0x8007202C | FUN_80071974 +0x6B8 | stage entity texture loading loop (seiken) | |
| 0x80079CE0 .. 0x80079ED0 | FUN_80079cb8 +0x28 .. +0x218 | combo / damage counter: always on, colours, positions, values (DesiacX) | |
| 0x8007C028 | FUN_8007bdc4 +0x264 | counter shadow box (DesiacX) | |
| 0x80096610 | FUN_800965f4 +0x1C | settings: BGM volume change (seiken) | `stfs f1, -0x4CF0(r13)` OK |
| 0x80099320 | FUN_80099124 +0x1FC | boot: BGM volume init (seiken) | |
| 0x800EB47C | FUN_800eb158 +0x324 | the call to the memory-card routine (nop = no save prompt) (DesiacX) | `bl 0x800ECA28` (a branch, as described) |
| 0x800F79BC | FUN_800f7914 +0xA8 | part select: illegal-part check, part base x 308 (seiken) | `mulli r4, r31, 308` OK |
| 0x80103DF0 | FUN_80103594 +0x85C | HP setting step up (DesiacX) | retail `addi r0, r4, 20`: steps of 20 |
| 0x80104880 / 0x8010488C | FUN_80104028 +0x858 / +0x864 | HP setting step down / minimum (DesiacX) | |
| 0x8010B204 | FUN_8010a5cc +0xC38 | default HP for all modes (DesiacX) | |

## Data addresses

| Address | Meaning (author) | Notes |
|---|---|---|
| 0x801C6C44, stride 0x134 | robo stats per body / part base | proven by the code above |
| 0x801C6C12 | bomb aim / reticle speed (Zach777) | inside the part data before the robo table |
| 0x8022B3B8 / 0x8022B3C4 | arcade practice opponent: robo ID / behaviour (seiken) | |
| 0x8023B656 + n x 0x14 | default HP of the 4 robos (700) (seiken) | |
| 0x8023BD68 | pointer to PH's parts expansion data (seiken) | PH, not retail |
| 0x8024E120 + n x 0x33520 | the 4 robos in battle | see above |
| 0x8024FCF8 | battle running flag (seiken) | used as a condition |
| 0x8031FEA8, 0x8031FEB4 | P1 / P2 buttons, 16-bit (0x0410 = Z + X) | stride 0xC = PADStatus size: likely the PADStatus array |
| 0x8031FEEB | D-pad left check in the stage-ban code (Glass_Knuckle) | |
| 0x80321D53 | battle mode (2 = tag) (seiken) | |
| 0x80321DB4 | remaining round time (DesiacX) | |
| 0x8032A888 / 0x8032A8A0 | tag counts per player (HP regen rate) (seiken) | not reset between battles in retail |
| 0x8034DD68, 0x803756A5 | stage collision data / stage entity count (seiken) | |
| 0x803872B3 | player 1 still in the cube (pre-launch) (seiken) | |
| 0x8041A238 | stage select state: 0x8041A23A stage ID, 0x8041A245 cursor, 0x8041A2D4 .. 0x8041A2FB one enable byte per stage in menu order (Glass_Knuckle, seiken) | the AR post lists the 40 stage names in that order |
| 0x8041CBEE, 0x8041CBFC, 0x8044F873 .. 0x8044F886 | menu cursors / state; 0x8044F885 == 0x0D at stage select (Glass_Knuckle) | |
| 0x8044F2A0 | game runtime counter (DesiacX) | |
| 0x8044F6D0 | BGM volume float 0..2 (= r13 - 0x4CF0) (seiken) | checked: the stfs above writes it |
| 0x80462970 | HP regen divisor (seiken) | |

## Tools and resources from the channel

- **Project Hive 3.2** (kactus and team): patch notes, a full Dolphin build or the Riivolution folder only, and a texture pack (Google Drive). The PH sources are `.asm` files turned into `GXCE01.GCT` by GCTRealMate.
- **CR Randomizer v1.2.2** (PoggieB): part IDs are 16-bit, e.g. Diving pod = 0x0312 (786), the standard pod 0x0300 (768).
- **CRBR Overlay Manager** (seiken): https://github.com/0seiken0/crbr-overlay-manager (reads the parts from memory for OBS; part lists for vanilla and PH).
- **Dolphin Memory Engine**: https://github.com/aldelaro5/dolphin-memory-engine, plus Glass_Knuckle's `.dmw`.
- **Stats / build-code sheets** for PH 3.1 and 3.2 (seiken, Glass_Knuckle): every part's stats plus host codes.
- **Stage select textures** with starter / counterpick outlines (seiken): a texture-pack example.
- Guides for online play: CRV2 (N64) NetPlay + English patch, Arena (DS) on melonDS, Battle Revolution on Dolphin. The CRV2 "unlock everything" code is for the N64 game, not this one.

## What this changes in the plan

- Phase 2b: new parts and new robo classes are already done in practice through Riivolution-loaded GCT code (PH). Our route can learn from PH's `.asm` sources (ask for them) and later turn them into source changes (Phase 4).
- Naming: 23 functions above have a described behaviour. The robo stats table (0x801C6C44, record 0x134), the 4-robo array (stride 0x33520), the PADStatus array and the stage-select state are the strongest leads for `cr_g_` globals and `cr_types.h` structs, after our own proof.
