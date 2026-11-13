/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003B1BC: .text 0x8003B1BC-0x8003B37C (448 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern const f32 lbl_80450690;

/* fn_8003B1BC: Nov 5, 2026 hand draft: the robo pointer before the xor; 1 dead use of var_r26 in an `if (0)` block settles the r25 / r26 order of the original */

typedef struct { u8 pad[0x214]; f32 dist[4]; } cr_RoboDist;
void fn_8003B1BC(void) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f25;
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;
    u8 *var_r28;
    cr_BattleRobo *var_r27;
    cr_BattleRobo *var_r26;
    u32 var_r25;
    if (0) {
        var_r26 = var_r26;
    }

    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r29 = __cntlzw(var_r30);
        var_r31 = 0;
        var_r27 = &cr_g_BattleRobos[var_r29];
        var_r28 = &var_r27->unk_001d6[0x3E];
        while (var_r31 < 4) {
            var_r31 += 1;
            M2C_FIELD(var_r28, f32 *, 0) = (f32) lbl_80450690;
            var_r28 += 4;
        }
        var_r30 ^= 0x80000000U >> var_r29;
    }
    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r29 = __cntlzw(var_r30);
        var_r27 = &cr_g_BattleRobos[var_r29];
        var_r30 ^= 0x80000000U >> var_r29;
        var_r25 = var_r30;
        var_f28 = M2C_FIELD(var_r27, f32 *, 0x1080);
        var_r28 = &var_r27->unk_001d6[0x3E];
        var_f27 = M2C_FIELD(var_r27, f32 *, 0x10A0);
        while (var_r25 != 0) {
            var_r31 = __cntlzw(var_r25);
            var_r26 = &cr_g_BattleRobos[var_r31];
            if (var_r29 > var_r31) {
                M2C_FIELD((var_r28 + (var_r31 * 4)), f32 *, 0) = ((cr_RoboDist *) var_r26)->dist[var_r29];
            } else {
                var_f26 = M2C_FIELD(var_r26, f32 *, 0x1080);
                var_f25 = M2C_FIELD(var_r26, f32 *, 0x10A0);
                var_f31 = var_f26 - var_f28;
                var_f30 = var_f25 - var_f27;
                var_f29 = (var_f30 * var_f30) + (var_f31 * var_f31);
                M2C_FIELD((var_r28 + (var_r31 * 4)), f32 *, 0) = var_f29;
                ((cr_RoboDist *) &cr_g_BattleRobos[var_r31])->dist[var_r29] = var_f29;
            }
            var_r25 ^= 0x80000000U >> var_r31;
        }
    }
}
