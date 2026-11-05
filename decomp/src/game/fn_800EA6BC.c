/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EA6BC: .text 0x800EA6BC-0x800EA830 (372 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80451518;
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

s8 fn_800EA6BC(void *arg0, u8 *arg1) {
    s8 sp8;
    f32 var_f31;
    u8 var_r31;
    u8 var_r30;
    u32 var_r29;
    u32 var_r28;
    u8 var_r27;
    s32 var_r26;
    u32 var_r25;

    var_f31 = lbl_80451518;
    sp8 = -1;
    var_r26 = 0;
    var_r27 = 0;
    var_r28 = 0;
    var_r30 = 0;
    while (var_r30 < 4U) {
        if ((s8) arg1[var_r30] != -1) {
            var_r27 += 1;
            var_r28 |= 0x80000000U >> (s8) arg1[var_r30];
        }
        var_r30 += 1;
    }
    if (var_r27 == 0) {
        var_r28 = lbl_8044F500;
    }
    var_r25 = var_r28;
    var_r29 = var_r25;
    while (var_r29 != 0) {
        var_r31 = __cntlzw(var_r29);
        if ((M2C_FIELD(arg0, u8 *, 0x6A) != var_r31) && (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8D) != cr_g_BattleRobos[var_r31].unk_0008d) && !(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200) && (var_f31 < cr_g_BattleRobos[var_r31].hp)) {
            var_r26 = 1;
            var_f31 = cr_g_BattleRobos[var_r31].hp;
            *arg1 = var_r31;
        }
        var_r29 ^= 0x80000000U >> var_r31;
    }
    return var_r26;
}
