/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80079F28: .text 0x80079F28-0x8007A130 (520 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
extern u8 lbl_80386378[208];
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  Vec gun_scale;
  Vec gun_offset;
  Vec bomb_scale;
  Vec bomb_offset;
  Vec pod_scale;
  Vec pod_offset_a;
  Vec pod_offset_b;
  Vec leg_scale;
  Vec shin_offset;
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
s32 fn_80029A9C(void *arg0, f32 (*arg1)[4], void *arg2, f32 *arg3, f32 *arg4);
extern f32 lbl_8032344C[237];
extern const f32 lbl_80450D08;

/* fn_80079F28: Nov 13, 2026 hand draft: the unused r28 is an s16 intermediate of the slot index (g = kind - 6; slot = g;), declared last; the screen position passed as p and p + 1; fn_80029A9C returns u8; five dead uses of the object */

void fn_80079F28(void *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
    } sp;
    u8 var_r30;
    u8 *var_r29;
    s16 g;

    (void) arg0; (void) arg0; (void) arg0; (void) arg0; (void) arg0; 
    var_r29 = cr_g_CurrentCam;
    g = M2C_FIELD(arg0, s32 *, 0x64) - 6;
    var_r30 = g;
    sp.sp8 = M2C_FIELD(var_r29, f32 *, 0x18);
    if (M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u16 *, 0) & 0x20) {
        M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u8 *, 2) |= 0x20;
        *((u16 *) (lbl_80386378 + (var_r30 * 0x34))) &= 0xFFFFFFDF;
    }
    if (M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u16 *, 0) & 0x40) {
        M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u8 *, 2) &= 0xFFFFFFDF;
        *((u16 *) (lbl_80386378 + (var_r30 * 0x34))) &= 0xFFFFFFBF;
    }
    if (M2C_FIELD((lbl_80386378 + (var_r30 * 0x34)), u8 *, 2) & 0x20) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        sp.spC = M2C_FIELD(&cr_g_BattleRobos[var_r30], f32 *, 0x30);
        sp.sp10 = M2C_FIELD(&cr_g_BattleRobos[var_r30], f32 *, 0x34);
        sp.sp14 = M2C_FIELD(&cr_g_BattleRobos[var_r30], f32 *, 0x38);
        if ((u8) fn_80029A9C(cr_g_CurrentCam, (f32 (*)[4]) lbl_8032344C, &sp.spC, M2C_FIELD(arg0, f32 **, 0x5C), M2C_FIELD(arg0, f32 **, 0x5C) + 1) == 0) {
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        }
        *M2C_FIELD(arg0, f32 **, 0x5C) -= lbl_80450D08;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
