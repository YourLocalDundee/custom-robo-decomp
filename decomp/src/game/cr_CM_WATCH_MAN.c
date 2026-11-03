/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CM_WATCH_MAN: .text 0x800C875C-0x800C88DC (384 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void fn_80026CA0(void *arg0);
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
void fn_80026920(void *arg0);
typedef struct cr_ScenarioSlot cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
extern const u32 lbl_8045110C;
void cr_Cam_SetMode(u8 mode);
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
extern const u32 lbl_80451178;

void cr_CM_WATCH_MAN(void) {
    u8 *temp_r31_59;
    s16 *temp_r30_16;
    u32 *cmd_cnt;
    s16 var_r29_43;

    temp_r30_16 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_16, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 6;
    if (0) {
        (*cmd_cnt)++;
    }
    if (M2C_FIELD(temp_r30_16, s16 *, 4) < 8) {
        var_r29_43 = M2C_FIELD(temp_r30_16, s16 *, 2);
        while (var_r29_43 <= (s16) M2C_FIELD(temp_r30_16, s16 *, 4)) {
            fn_80026CA0(&cr_g_Script.char_pos[var_r29_43]);
            var_r29_43 += 1;
        }
        temp_r31_59 = cr_g_CurrentCam;
        fn_80026920(temp_r31_59);
        M2C_FIELD(temp_r31_59, f32 *, 0x98) = (f32) M2C_FIELD(temp_r31_59, f32 *, 0xC8);
        M2C_FIELD(temp_r31_59, f32 *, 0x9C) = (f32) M2C_FIELD(temp_r31_59, f32 *, 0xCC);
        M2C_FIELD(temp_r31_59, f32 *, 0xA0) = (f32) (M2C_FIELD(temp_r31_59, f32 *, 0xD0) + M2C_FIELD(temp_r30_16, f32 *, 8));
        M2C_FIELD(temp_r31_59, s32 *, 0xFC) = (s32) M2C_FIELD(temp_r30_16, s16 *, 6);
        M2C_FIELD(temp_r31_59, s8 *, 0x16A) = 5;
        M2C_FIELD(temp_r31_59, u8 *, 0x168) |= 4;
        cr_g_ScenarioSlotPtr->status |= lbl_8045110C;
        cr_Cam_SetMode(0xCU);
        return;
    }
    cr_g_Scenario.error_flags |= lbl_80451178;
}
