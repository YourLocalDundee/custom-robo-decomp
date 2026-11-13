/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F7914: .text 0x800F7914-0x800F7BF8 (740 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
u32 cr_Bits_Test(u32 *set, u32 bit);
typedef struct cr_UnlockBits
{
  u32 parts[5][8];
  u32 stages[2];
} cr_UnlockBits;
__declspec(section ".data") extern cr_UnlockBits cr_g_UnlockBits;
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
typedef struct cr_BodyStats
{
  char name_jp[0x20];
  char name[0x20];
  u16 unk_040;
  u8 unk_042[0xE];
  u8 leg_tri_050[3];
  u8 leg_tri_053[3];
  u8 air_type;
  u8 air_jumps;
  u16 leg_tri_058[3];
  u8 air_dashes;
  u8 unk_05f[2];
  u8 leg_tri_061[3];
  u16 leg_tri_064[3];
  u16 unk_06a;
  s16 unk_06c;
  u16 unk_06e;
  s16 leg_tri_070[8][3];
  u16 leg_tri_0a0[3];
  u8 unk_0a6[0x8E];
} cr_BodyStats;
__declspec(section ".data") extern cr_BodyStats cr_g_BodyStats[34];
typedef struct cr_GunData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_040[0x24C];
} cr_GunData;
__declspec(section ".data") extern cr_GunData cr_g_GunData[64];
typedef struct cr_BombData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_040[0x13C];
} cr_BombData;
__declspec(section ".data") extern cr_BombData cr_g_BombData[52];
typedef struct cr_PodData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_040[0x134];
} cr_PodData;
__declspec(section ".data") extern cr_PodData cr_g_PodData[57];
typedef struct cr_LegData
{
  char name_jp[0x20];
  char name[0x20];
  u8 unk_40[0x41];
  u8 leg_values[14];
} cr_LegData;
__declspec(section ".data") extern cr_LegData cr_g_LegData[29];

/* fn_800F7914: Nov 6, 2026 hand draft (m2c 98%): the parameters used as they are (m2c copied arg0 / arg1 into var_r28 / var_r29, which spilled them to the stack), plus 1 dead use of arg0 in an `if (0)` block so it keeps r28 */

u8 fn_800F7914(void *arg0, u16 arg1, u32 arg2) {
    u8 var_r30;

    if (0) {
        arg0 = arg0;
    }
    var_r30 = 0;
    if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x1D8) & 0x80000000) {
        return 1U;
    }
    if (arg1 == 0) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[0][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_BodyStats[arg2].unk_042[3] != 0) || (arg2 == 0x19))) {
            var_r30 = 0;
        }
    } else if (arg1 == 1) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[1][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_GunData[arg2].unk_040[0x60] != 0) || (arg2 == 0x21))) {
            var_r30 = 0;
        }
    } else if (arg1 == 2) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[2][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_BombData[arg2].unk_040[0x60] != 0) || (arg2 == 0x1F))) {
            var_r30 = 0;
        }
    } else if (arg1 == 3) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[3][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_PodData[arg2].unk_040[0xA0] != 0) || (arg2 == 0x23))) {
            var_r30 = 0;
        }
    } else if (arg1 == 4) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[4][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_LegData[arg2].unk_40[0x40] != 0) || (arg2 == 0xA))) {
            var_r30 = 0;
        }
    }
    return var_r30;
}
