/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80108290: .text 0x80108290-0x801086A4 (1044 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80238008[344];
void fn_80023394(s8 *arg0, u32 arg1);
void fn_8008C5F0(void *arg0, void *arg1, s32 arg2, u32 arg3);
s32 fn_8008EDD8(s32 arg0);
extern const f32 lbl_804517A4;
void fn_800FB924(void *arg0);
extern u8 lbl_8040709C[436];
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
extern u8 lbl_80407250[16];
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
void fn_800FBAB4(void *arg0);
void fn_800671C4(s32 arg0, s32 arg1);
s32 fn_80066948(u8 arg0, u16 arg1);
void fn_80068988(s32 arg0, u32 arg1);
void fn_8006A344(void *arg0, s32 arg1);

/* fn_80108290: Nov 13, 2026 hand draft: the name buffer is char[84] (frame 128); one u8 counter for both loops (m2c had two for r30); fn_8006A344 returns a value that is dropped (r3 stays busy: the next statement loads through r4), the context says void */

void fn_80108290(u8 *arg0) {
    char sp8[84];
    u8 var_r30_69;
    u8 *var_r29_102;
    s32 var_r27_160;
    u8 *base_lbl_80238008;
    base_lbl_80238008 = lbl_80238008;

    (void) var_r27_160; 
    (void) base_lbl_80238008; 
    fn_80023394((signed char *) (arg0 + 0x22C), 0x40U);
    fn_80023394((signed char *) (arg0 + 0x21C), 0x10U);
    fn_80023394((signed char *) (arg0 + 0x1FC), 0x20U);
    fn_8008C5F0(arg0 + 0x1FC, arg0 + 0x21C, (s32) (arg0 + 0x22C), 2U);
    M2C_FIELD(arg0, s32 *, 0x26C) = fn_8008EDD8(8);
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), void **, 0x5C) = (void *) (arg0 + 0x1FC);
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0) |= 0x40;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0) |= 0x20;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), void **, 0x5C), f32 *, 8) = (f32) lbl_804517A4;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0x10) = (s32) M2C_FIELD(arg0, u8 *, 0x4854);
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), void (**)(void *), 0x50) = fn_800FB924;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s8 *, 0x58) = 0;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s8 *, 0x57) = 0;
    if (*lbl_8040709C != 0) {
        M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0) |= 0x10000;
    }
    var_r30_69 = 0;
    M2C_FIELD(arg0, s16 *, 0x23C) = (s16) (M2C_FIELD(arg0, u8 *, 0x4854) + 0x20);
    M2C_FIELD(arg0, s16 *, 0x238) = 0x12D;
    M2C_FIELD(arg0, s16 *, 0x23A) = 0xC7;
    while (var_r30_69 < 0xBU) {
        fn_80023394((signed char *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x278]), 0x600U);
        fn_80023394((signed char *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x878]), 0x10U);
        fn_80023394((signed char *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x888]), 0x20U);
        var_r30_69 += 1;
    }
    M2C_FIELD(arg0, s8 *, 0x274) = 1;
    var_r29_102 = NULL;
    if (cr_g_GameMode.flags & 0x800000) {
        if (M2C_FIELD(lbl_80407250, u16 *, 2) & 1) {
            if ((s32) cr_g_GameMode.battle_mode == 2) {
                var_r29_102 = base_lbl_80238008 + 0x398C;
            } else {
                var_r29_102 = base_lbl_80238008 + 0x3968;
            }
        }
    } else if (cr_g_GameMode.flags & 0x08000000) {
        if (M2C_FIELD(lbl_80407250, u16 *, 2) & 1) {
            if ((s32) cr_g_GameMode.battle_mode == 2) {
                var_r29_102 = base_lbl_80238008 + 0x39D0;
            } else {
                var_r29_102 = base_lbl_80238008 + 0x39AC;
            }
        }
    } else if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x1D8) & 0x80000000) {
        var_r29_102 = base_lbl_80238008 + 0x3A18;
    } else {
        var_r29_102 = base_lbl_80238008 + 0x39F0;
    }
    if (var_r29_102 != ((void *) 0)) {
        var_r27_160 = 0;
        var_r30_69 = 0;
        while ((u32) M2C_FIELD(var_r29_102, u32 *, (var_r30_69 * 4) & 0x3FC) != 0xBU) {
            fn_8008C5F0(&((u8 (*)[0x634]) (arg0))[var_r30_69][0x888], &((u8 (*)[0x634]) (arg0))[var_r30_69][0x878], (s32) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x278]), 0x18U);
            M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), s32 *, 0) = fn_8008EDD8(8);
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), void **, 0x5C) = (void *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x888]);
            M2C_FIELD(M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), void **, 0x5C), f32 *, 8) = (f32) lbl_804517A4;
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), s32 *, 0x10) = (s32) M2C_FIELD(arg0, u8 *, 0x4854);
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), s32 *, 0x14) = (s32) var_r30_69;
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), s32 *, 0x18) = var_r27_160;
            if ((u32) M2C_FIELD(var_r29_102, u32 *, (var_r30_69 * 4) & 0x3FC) == 6U) {
                var_r27_160 += 0xE;
            } else {
                var_r27_160 += 0x12;
            }
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), void (**)(void *), 0x50) = fn_800FBAB4;
            fn_800671C4((s32) sp8, fn_80066948(0x15U, (u16) M2C_FIELD(var_r29_102, s32 *, (var_r30_69 * 4) & 0x3FC)));
            fn_80068988((s32) sp8, 0x30U);
            ((s32 (*)(void *, s32)) fn_8006A344)((void *) (M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0)), (s32) sp8);
            if (*lbl_8040709C != 0) {
                *M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), s32 **, 0) |= 0x10000;
            }
            var_r30_69 += 1;
        }
        return;
    }
    M2C_FIELD(arg0, s8 *, 0x274) = 0;
}
