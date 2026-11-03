/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CHAR_TALK: .text 0x800D4074-0x800D4244 (464 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s16 *cr_g_ScriptPC;
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
__declspec(section ".sdata") extern s32 cr_g_ScriptDepth;
typedef struct cr_ScenarioSlot cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
extern const u32 lbl_80451108;
struct cr_ScenarioSlot
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
};
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
s32 fn_80069D20(u8 arg0, s32 arg1);
extern u8 lbl_8032C2B0[2416];
void fn_800674E4(s16 arg0);
void fn_8006611C(u32 arg0, u32 arg1, s32 arg2);
void fn_80065E10(s32 arg0, s32 arg1);
extern const u32 lbl_80451164;
extern const u32 lbl_80451114;

/* cr_CHAR_TALK: step 110 hand draft (the m2c draft with the head rule, then: the slot index tested without a (u8) cast) */
extern u32 lbl_8044F320;

void cr_CHAR_TALK(void) {
    s16 *var_r31;
    u32 var_r30;
    s32 var_r29;
    s32 var_r28;
    u32 *cmd_cnt;

    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r31, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    cr_g_ScenarioSlotPtr->status |= lbl_80451108;
    cr_g_ScenarioSlotPtr->status &= 0xFFFFFEFF;
    if (cr_g_Scenario.slot_index != 0) {
        if ((u32) var_r31 < (u32) lbl_8044F320) {
            var_r30 = 0;
        } else {
            var_r30 = 1;
        }
    } else {
        var_r30 = 0;
    }
    var_r28 = M2C_FIELD(var_r31, s16 *, 4) & 0x7FFF;
    if (M2C_FIELD(var_r31, s16 *, 4) & 0x8000) {
        var_r30 ^= 1;
    }
    var_r29 = fn_80069D20(var_r30, var_r28);
    if (var_r29 != 0) {
        lbl_8032C2B0[0x4B0] = 0;
        cr_g_Scenario.unk_028 = 0;
        if (!(cr_g_ScenarioSlotPtr->status & 0x40)) {
            fn_800674E4(M2C_FIELD(var_r31, s16 *, 2));
            fn_8006611C(0x18U, 0x120U, var_r29);
        } else {
            fn_800674E4(M2C_FIELD(var_r31, s16 *, 2));
            fn_80065E10(0, var_r29);
        }
    } else {
        cr_g_Script.unk_464 = (u16) M2C_FIELD(var_r31, s16 *, 4);
        cr_g_Scenario.error_flags |= lbl_80451164;
    }
    cr_g_ScenarioSlotPtr->status |= lbl_80451114;
}
