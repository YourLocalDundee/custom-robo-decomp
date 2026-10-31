/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800D9A0C: .text 0x800D9A0C-0x800D9C88 (636 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef struct cr_ScriptState
{
  u32 unk_000;
  u32 history_init;
  u32 history_index;
  u32 cmd_count[16];
  s32 cmd_history[64];
  u32 map_bef_script[50];
  u32 map_init_script[50];
  u32 map_no_enter_script[50];
  void *script_name[16];
  u32 unk_3e4;
  u8 unk_3e8;
  u8 unk_3e9[3];
  Vec3 char_pos[8];
  s32 unk_44c;
  s32 unk_450;
  s32 unk_454;
  u32 call_count;
  s32 battle_arg_slot0;
  s32 battle_arg;
  u16 unk_464;
  u8 unk_466;
  u8 unk_467;
  u8 unk_468;
  u8 unk_469;
  u8 unk_46a;
  u8 unk_46b;
  u32 unk_46c;
  u8 unk_470;
  u8 unk_471[5];
  u8 unk_476;
  u8 unk_477;
  u32 overlay_478;
  u32 overlay_47c;
  u32 overlay_480;
} cr_ScriptState;
__declspec(section ".data") extern cr_ScriptState cr_g_Script;
extern u32 lbl_80224384[20];
extern u32 lbl_80229960[30];
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 unk_10;
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
void cr_Bits_Clear(u32 *set, u32 bit);
extern u16 lbl_801C5BD0[32];
u32 cr_Bits_Test(u32 *set, u32 bit);
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
void cr_Bits_Set(u32 *set, u32 bit);

void fn_800D9A0C(void) {
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;

    var_r30 = 0;
    if (cr_g_Scenario.slot_index == 0) {
        var_r31 = 0;
        while (var_r31 < cr_g_Script.unk_44c) {
            var_r30 += lbl_80224384[var_r31];
            if (var_r31 == 1) {
                var_r30 -= 1;
            }
            var_r31 += 1;
        }
        var_r30 += cr_g_Script.battle_arg_slot0;
        if ((var_r31 == 1) && (cr_g_Script.battle_arg_slot0 == 6)) {
            var_r30 -= 1;
        }
    } else {
        if (cr_g_Script.unk_450 != 0) {
            var_r31 = 0;
            while (var_r31 < cr_g_Script.unk_450) {
                var_r30 += *(((u32 *) ((u8 *) lbl_80229960 + (var_r31 * 4))));
                var_r31 += 1;
            }
        } else {
            var_r31 = 0;
            while (var_r31 < cr_g_Script.unk_454) {
                var_r30 += *(((u32 *) ((u8 *) lbl_80229960 + (var_r31 * 4))));
                var_r31 += 1;
            }
        }
        var_r30 += cr_g_Script.battle_arg;
    }
    if ((s32) cr_g_GameMode.unk_10 != 2) {
        var_r29 = 1;
    } else {
        var_r29 = 2;
    }
    var_r31 = 0;
    while (var_r31 < var_r29) {
        cr_Bits_Clear(&cr_g_Scenario.part_bits[0][2], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 2));
        cr_Bits_Clear(&cr_g_Scenario.part_bits[1][2], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 4));
        cr_Bits_Clear(&cr_g_Scenario.part_bits[2][2], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 6));
        cr_Bits_Clear(&cr_g_Scenario.part_bits[3][2], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 8));
        cr_Bits_Clear(&cr_g_Scenario.part_bits[4][2], (u32) M2C_FIELD((((u16 *) ((u8 *) lbl_801C5BD0 + (var_r31 * 0x10)))), u16 *, 0xA));
        var_r31 += 1;
    }
    if (cr_Bits_Test(cr_g_ScenarioSlotPtr->unk_98, var_r30) == 0) {
        cr_g_ScenarioSlotPtr->unk_92[1] += 1;
        cr_Bits_Set(cr_g_ScenarioSlotPtr->unk_98, var_r30);
    }
}
