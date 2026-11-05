/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80050388: .text 0x80050388-0x800506DC (852 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u32 lbl_8044F500;
int __cntlzw(unsigned int);
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
extern u32 lbl_8044F498;
extern u32 lbl_801F8F14[4];

/* fn_80050388: Nov 3, 2026 hand draft (m2c 95%): in the reset of the best-hp list the count and the first entry are cleared before the robo mask is read */

s32 fn_80050388(void) {
    u32 var_r31;
    u32 var_r30;
    s32 var_r29;
    u32 var_r28;
    s32 var_r27;
    u32 var_r26;

    var_r29 = 0;
    var_r26 = -1;
    var_r27 = 0;
    var_r28 = 0;
    if (!(cr_g_GameMode.flags & 0x01000000)) {
        var_r30 = lbl_8044F500;
        while ((s32) var_r30 != 0) {
            var_r31 = __cntlzw(var_r30);
            if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200)) {
                var_r26 = var_r31;
                var_r29 += 1;
                if ((u32) cr_g_BattleRobos[var_r31].hp > var_r28) {
                    var_r28 = (u32) cr_g_BattleRobos[var_r31].hp;
                    lbl_8044F498 = 0;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                } else if (var_r28 == (u32) cr_g_BattleRobos[var_r31].hp) {
                    lbl_8044F498 += 1;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                }
            }
            var_r30 ^= 0x80000000U >> var_r31;
        }
        if (var_r29 == 1) {
            var_r27 = 1;
            cr_g_GameMode.unk_16 = (s16) var_r26;
        } else if (var_r29 == 0) {
            var_r27 = 1;
            cr_g_GameMode.unk_16 = -1;
        }
    } else if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) & 0x200) {
        var_r27 = 1;
        cr_g_GameMode.unk_16 = 1;
    } else {
        var_r28 = (u32) cr_g_BattleRobos->hp;
        lbl_8044F498 = 0;
        lbl_801F8F14[lbl_8044F498] = 0;
        var_r30 = lbl_8044F500 ^ 0x80000000;
        while ((s32) var_r30 != 0) {
            var_r31 = __cntlzw(var_r30);
            if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200)) {
                var_r29 += 1;
                if ((u32) cr_g_BattleRobos[var_r31].hp > var_r28) {
                    var_r28 = (u32) cr_g_BattleRobos[var_r31].hp;
                    lbl_8044F498 = 0;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                } else if (var_r28 == (u32) cr_g_BattleRobos[var_r31].hp) {
                    lbl_8044F498 += 1;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                }
            }
            var_r30 ^= 0x80000000U >> var_r31;
        }
        if (var_r29 == 0) {
            var_r27 = 1;
            cr_g_GameMode.unk_16 = 0;
        }
    }
    if (var_r27 == 0) {
        if ((s32) lbl_8044F498 == 0) {
            cr_g_GameMode.unk_16 = (s16) *lbl_801F8F14;
        } else {
            cr_g_GameMode.unk_16 = -1;
        }
    }
    return var_r27;
}
