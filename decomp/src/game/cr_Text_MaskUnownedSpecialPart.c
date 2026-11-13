/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Text_MaskUnownedSpecialPart: .text 0x800F8E4C-0x800F9014 (456 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 battle_mode;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;
typedef struct cr_ScenarioSlot
{
  u32 status;
  u32 flags[32];
  u16 unk_84[6];
  u16 battle_count;
  u16 unk_92[2];
  u8 unk_96[2];
  u32 unk_98[5];
  u32 unk_ac[2];
  u32 unk_b4;
  u16 unk_b8;
  u16 unk_ba;
  u8 unk_bc[0x10];
} cr_ScenarioSlot;
typedef struct cr_ScenarioState
{
  u16 unk_000;
  char name[18];
  u32 error_flags;
  u32 unk_018;
  u32 unk_01c;
  u32 unk_020;
  s32 wait_timer;
  s32 unk_028;
  u8 unk_02c;
  u8 unk_02d;
  u8 unk_02e;
  u8 slot_index;
  cr_ScenarioSlot slot[2];
  u32 unk_1c8[4];
  u32 unk_1d8[32];
  u32 unk_258[10];
  u32 unk_280;
  u8 unk_284;
  u8 unk_285[3];
  u32 part_bits[5][8];
  u32 unk_328[2];
  u8 unk_330;
  u8 pickup_battle;
  u8 unk_332[2];
  u32 unk_334[4];
  u32 unk_344[4];
  u32 unk_354[4];
} cr_ScenarioState;
__declspec(section ".data") extern cr_ScenarioState cr_g_Scenario;
typedef struct cr_BattleRecord
{
  u32 battles;
  u32 wins;
  u32 losses;
} cr_BattleRecord;
typedef struct cr_UnlockBits
{
  u32 parts[5][8];
  u32 stages[2];
} cr_UnlockBits;
typedef struct cr_SaveData
{
  u32 battles;
  u32 battles_by_mode[3];
  s32 battles_finished;
  u32 unk_014[3];
  cr_BattleRecord body_records[64];
  cr_BattleRecord unk_records[4][64];
  u32 stage_battles[40];
  cr_UnlockBits unlocks;
  cr_UnlockBits new_unlocks;
  u32 used_parts[5][2];
} cr_SaveData;
__declspec(section ".data") extern cr_SaveData cr_g_SaveData;
__declspec(section ".sdata") extern s32 cr_g_BodyCount;
__declspec(section ".sdata") extern s32 cr_g_GunCount;
__declspec(section ".sdata") extern s32 cr_g_BombCount;
__declspec(section ".sdata") extern s32 cr_g_PodCount;
__declspec(section ".sdata") extern s32 cr_g_LegCount;
u32 cr_Bits_Test(u32 *set, u32 bit);

/* cr_Text_MaskUnownedSpecialPart: Nov 6, 2026 hand draft (m2c 94%): the special part's text slot is (part - first special part) + the count of all parts, `(arg1 - 0x1E) + temp_r28_35` (the compiler adds first and subtracts last; m2c's `(arg1 + temp) - 0x1E` folds the constant into the count instead); the count is the plain sum body + gun + bomb + pod + leg (m2c's grouping loads the five counts in another order) */

void cr_Text_MaskUnownedSpecialPart(u16 arg0, u32 arg1, s16 *arg2) {
    u32 *var_r29_18;
    s32 temp_r28_35;

    if (cr_g_GameMode.flags & 0x800000) {
        var_r29_18 = cr_g_Scenario.part_bits[0];
    } else {
        var_r29_18 = cr_g_SaveData.unlocks.parts[0];
    }
    temp_r28_35 = cr_g_BodyCount + cr_g_GunCount + cr_g_BombCount + cr_g_PodCount + cr_g_LegCount;
    switch ((s32) arg0) {                            /* irregular */
    case 0:
        if (((arg1 == 0x1E) || (arg1 == 0x1F) || (arg1 == 0x20)) && (cr_Bits_Test(var_r29_18, arg1) == 0)) {
            *arg2 = (arg1 - 0x1E) + temp_r28_35;
        }
        return;
    case 1:
        if (((arg1 == 0x31) || (arg1 == 0x32) || (arg1 == 0x33)) && (cr_Bits_Test(var_r29_18 + 8, arg1) == 0)) {
            *arg2 = (arg1 - 0x2E) + temp_r28_35;
            return;
        }
        break;
    case 2:
        if ((arg1 == 0x28) && (cr_Bits_Test(var_r29_18 + 0x10, arg1) == 0)) {
            *arg2 = temp_r28_35 + 6;
            return;
        }
        break;
    case 3:
        if (((arg1 == 0x29) || (arg1 == 0x2A) || (arg1 == 0x2B)) && (cr_Bits_Test(var_r29_18 + 0x18, arg1) == 0)) {
            *arg2 = (arg1 - 0x22) + temp_r28_35;
            return;
        }
        break;
    case 4:
        if ((arg1 == 0xF) && (cr_Bits_Test(var_r29_18 + 0x20, arg1) == 0)) {
            *arg2 = temp_r28_35 + 0xA;
        }
        break;
    }
}
