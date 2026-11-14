/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003B37C: .text 0x8003B37C-0x8003B528 (428 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BattleRobo cr_BattleRobo;
extern u32 lbl_8044F4F4;
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
struct cr_BattleRobo
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
};
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
s32 fn_8001F060(f32 farg0, f32 farg1);

/* fn_8003B37C: Nov 13, 2026 hand draft: the angle read as ((s16 *) ((u8 *) robo + i * 2))[0x106]: the constant index on the computed pointer keeps i * 2 + 0x20C as the index (lhax); dead uses from the rank sweep */

void fn_8003B37C(void) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    s32 var_r31;
    u32 var_r30;
    u8 *var_r29;
    s32 var_r28;
    cr_BattleRobo *var_r27;
    cr_BattleRobo *var_r26;
    u32 var_r25;

    (void) var_r29; 
    (void) var_r26; 
    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r28 = __cntlzw(var_r30);
        var_r31 = 0;
        var_r27 = &cr_g_BattleRobos[var_r28];
        var_r29 = &var_r27->unk_001d6[0x36];
        while (var_r31 < 4) {
            var_r31 += 1;
            M2C_FIELD(var_r29, s16 *, 0) = 0;
            var_r29 += 2;
        }
        var_r30 ^= 0x80000000U >> var_r28;
    }
    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r28 = __cntlzw(var_r30);
        var_r27 = &cr_g_BattleRobos[var_r28];
        var_r30 ^= 0x80000000U >> var_r28;
        var_r25 = var_r30;
        var_f31 = M2C_FIELD(var_r27, f32 *, 0x1080);
        var_r29 = &var_r27->unk_001d6[0x36];
        var_f30 = M2C_FIELD(var_r27, f32 *, 0x10A0);
        while (var_r25 != 0) {
            var_r31 = __cntlzw(var_r25);
            var_r26 = &cr_g_BattleRobos[var_r31];
            if (var_r28 > var_r31) {
                M2C_FIELD((var_r29 + (var_r31 * 2)), s16 *, 0) = ((s16 *) ((u8 *) var_r26 + (var_r28 * 2)))[0x106] - 0x8000;
            } else {
                var_f29 = M2C_FIELD(var_r26, f32 *, 0x1080);
                var_f28 = M2C_FIELD(var_r26, f32 *, 0x10A0);
                M2C_FIELD((var_r29 + (var_r31 * 2)), s16 *, 0) = fn_8001F060(var_f31 - var_f29, var_f30 - var_f28);
                M2C_FIELD(((u8 *) &cr_g_BattleRobos[var_r31] + (var_r28 * 2)), s16 *, 0x20C) = (s16) (M2C_FIELD((var_r29 + (var_r31 * 2)), s16 *, 0) - 0x8000);
            }
            var_r25 ^= 0x80000000U >> var_r31;
        }
    }
}
