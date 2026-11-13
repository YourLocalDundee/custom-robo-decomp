/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EA33C: .text 0x800EA33C-0x800EA468 (300 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F500;
int __cntlzw(unsigned int);
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
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

/* fn_800EA33C: Nov 6, 2026 hand draft (m2c 85%): the inner loop index is mentioned first, as a (void) statement at the top, which gives it r30 and the count r29 (as the original) with no code */

s8 fn_800EA33C(void *arg0, s32 arg1) {
    u32 temp_r31_12;
    u32 var_r30_35;
    u8 var_r29_6;
    u32 var_r28_8;

    (void) var_r30_35;
    var_r29_6 = 0;
    var_r28_8 = lbl_8044F500;
    while (var_r28_8 != 0) {
        temp_r31_12 = __cntlzw(var_r28_8);
        if ((M2C_FIELD(arg0, u8 *, 0x6A) != temp_r31_12) && (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8D) == (u8) cr_g_BattleRobos[temp_r31_12].unk_0008d) && !(M2C_FIELD(&cr_g_BattleRobos[temp_r31_12], s32 *, 0x1D8) & 0x200)) {
            var_r30_35 = 0U;
            for (; var_r30_35 < var_r29_6; var_r30_35 += 1) {
                if (M2C_FIELD((arg1 + var_r30_35), s8 *, 0) == (s8) cr_g_BattleRobos[temp_r31_12].unk_001d6[0x52]) {
                    break;
                }
            }
            if (var_r30_35 == var_r29_6) {
                M2C_FIELD((arg1 + (u8) (var_r29_6)), u8 *, 0) = cr_g_BattleRobos[temp_r31_12].unk_001d6[0x52];
                var_r29_6 += 1;
            }
        }
        var_r28_8 ^= 0x80000000U >> temp_r31_12;
    }
    return (s8) var_r29_6;
}
