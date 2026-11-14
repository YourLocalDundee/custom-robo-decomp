/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SYS_OVERLAY_WAIT: .text 0x800D8B68-0x800D8D2C (452 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void fn_80021014(void *arg0);
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
extern u8 *lbl_8044F318;
void fn_8008F8D0(void);
void cr_MotionFile_Register(u32 *arg0);
void fn_800215F8(void *arg0);

/* cr_SYS_OVERLAY_WAIT: Nov 13, 2026 hand draft: the register order computed by rank_solve.py: two dead uses of the first local and one of the fourth, the locals declared in register order */

extern u8 *lbl_8044F320;
extern u32 *lbl_8044F338;
extern u8 *lbl_8044F40C;
extern u8 *lbl_8044F784;

void cr_SYS_OVERLAY_WAIT(void) {
    u8 *var_r31;
    u32 *cmd_cnt;
    s32 var_r30;
    u8 *var_r29;
    u8 *var_r28;
    u8 **var_r27;
    s16 *var_r26;

    (void) var_r31; (void) var_r31; 
    (void) var_r28; 
    var_r26 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) *var_r26;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    fn_80021014((void *) cr_g_Script.overlay_478);
    fn_80021014((void *) cr_g_Script.overlay_47c);
    fn_80021014((void *) cr_g_Script.overlay_480);
    var_r30 = 0;
    while (var_r30 < 0xA) {
        cr_g_Scenario.unk_258[var_r30] = 0;
        var_r30 += 1;
    }
    cr_g_Script.battle_arg = 0;
    var_r29 = lbl_8044F320 + 0x40;
    var_r31 = lbl_8044F320;
    var_r30 = 0;
    M2C_FIELD(var_r31, u8 **, 8) = (unsigned char *) ((u32) (M2C_FIELD(var_r31, u8 **, 8) + (s32) var_r29));
    M2C_FIELD(var_r31, u8 **, 0xC) = (unsigned char *) ((u32) (M2C_FIELD(var_r31, u8 **, 0xC) + (s32) var_r29));
    var_r28 = var_r29 + M2C_FIELD(var_r31, s32 *, 0x18);
    while (var_r30 < (s32) M2C_FIELD(var_r31, s32 *, 0x1C)) {
        var_r27 = (unsigned char **) (var_r29 + (*(u32 *)((s8 *)((var_r28 + (var_r30 * 4))) + (0))));
        var_r30 += 1;
        *var_r27 += (s32) var_r29;
    }
    lbl_8044F784 = M2C_FIELD(var_r31, void **, 8);
    M2C_FIELD(lbl_8044F40C, u8 **, 8) = (u8 *) lbl_8044F318;
    M2C_FIELD(lbl_8044F40C, s32 *, 0x10) = 1;
    fn_8008F8D0();
    cr_MotionFile_Register(lbl_8044F338);
    fn_800215F8((void *) cr_g_Script.overlay_478);
    fn_800215F8((void *) cr_g_Script.overlay_47c);
    fn_800215F8((void *) cr_g_Script.overlay_480);
}
