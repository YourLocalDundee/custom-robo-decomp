/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_BattleRobo_InitBody: .text 0x800A59D8-0x800A5C60 (648 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  f32 unk_0c[27];
} cr_BodyData;
typedef struct cr_RoboLegStats
{
  s16 blended[14];
  s16 unk_1c;
} cr_RoboLegStats;
typedef struct cr_BattleRobo
{
  void (*update)(void);
  u8 unk_00004[0x6C];
  f32 unk_00070;
  f32 hp;
  u8 unk_00078[0x14];
  u8 index;
  u8 unk_0008d;
  u16 unk_0008e;
  u8 unk_00090[0x50];
  s16 timer_len;
  s16 timer_left;
  u8 unk_000e4[0x57];
  u8 air_actions_left;
  u8 unk_0013c[0xA];
  s16 tag_cooldown;
  u8 unk_00148[0x84];
  u16 parts[5];
  u8 unk_001d6[0x352];
  u8 action;
  u8 unk_00529[0x12CB];
  cr_BodyData body_data;
  u8 unk_0186c[0x32C];
  u8 body_stats[0x134];
  u8 unk_01ccc[0x15C];
  u8 gun_data[0x28C];
  u8 unk_020b4[0x26C94];
  u8 bomb_data[0x17C];
  u8 unk_28ec4[0x5424];
  u8 pod_data[0x174];
  u8 unk_2e45c[0x4EAC];
  u8 leg_data[0x8F];
  u8 unk_33397[0x85];
  cr_RoboLegStats leg_stats;
  u8 unk_3343a[0xE6];
} cr_BattleRobo;
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
__declspec(section ".data") extern cr_BodyData cr_g_BodyData[34];
void cr_Battle_SetCurrentPointers(void);
extern const f32 lbl_80451284;
void cr_Motion_PlayFromFrame1(u32 id);
extern u8 lbl_8024DFA0[352];
void cr_Motion_UpdateRoot(void);
void fn_80037FF0(void);
void cr_Chara_PoseLimbs(void);

typedef struct cr_Copy78_4 {
    u32 w[0x1E];
} cr_Copy78_4;

void cr_BattleRobo_InitBody(u8 arg0, u8 arg1) {
    u8 var_r31;

    cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[arg1];
    *(cr_Copy78_4 *) ((s32) (cr_g_CurrentBattleRobo + 0x17F0) + 4) = *(cr_Copy78_4 *) (&cr_g_BodyData[arg0]);
    M2C_FIELD(cr_g_CurrentBattleRobo, u16 **, 0x554) = (u16 *) M2C_FIELD(cr_g_CurrentBattleRobo, u16 **, 0x17FC);
    cr_Battle_SetCurrentPointers();
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1C4) = (f32) lbl_80451284;
    cr_Motion_PlayFromFrame1((u32) *M2C_FIELD(cr_g_CurrentBattleRobo, u16 **, 0x554));
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x85C) = (s32) M2C_FIELD((lbl_8024DFA0 + (arg1 * 0x58)), u32 *, 0);
    var_r31 = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x860) = (s32) M2C_FIELD((lbl_8024DFA0 + (arg1 * 0x58)), s32 *, 4);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x8B0) = (s32) M2C_FIELD((lbl_8024DFA0 + (arg1 * 0x58)), s32 *, 0x54);
    while (var_r31 < 0x13U) {
        M2C_FIELD(&cr_g_CurrentBattleRobo[(var_r31 * 4)], s32 *, 0x864) = (s32) M2C_FIELD((lbl_8024DFA0 + (arg1 * 0x58) + (var_r31 * 4)), s32 *, 8);
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 0xEU) {
        M2C_FIELD(&cr_g_CurrentBattleRobo[(var_r31 * 4)], s32 *, 0x8B8) = 0;
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 7U) {
        M2C_FIELD(&cr_g_CurrentBattleRobo[(var_r31 * 4)], s32 *, 0x8F0) = 0;
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 0xEU) {
        M2C_FIELD(&cr_g_CurrentBattleRobo[(var_r31 * 4)], s32 *, 0x90C) = 0;
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 8U) {
        M2C_FIELD(&cr_g_CurrentBattleRobo[(var_r31 * 4)], s32 *, 0x944) = 0;
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < 4U) {
        M2C_FIELD(&cr_g_CurrentBattleRobo[(var_r31 * 4)], s32 *, 0x964) = 0;
        var_r31 += 1;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x84A) = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x851) = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x64) = 0x8000;
    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x139) = 0;
    cr_Battle_SetCurrentPointers();
    cr_Motion_UpdateRoot();
    fn_80037FF0();
    cr_Chara_PoseLimbs();
}
