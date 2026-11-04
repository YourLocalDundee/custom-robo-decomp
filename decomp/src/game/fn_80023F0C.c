/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80023F0C: .text 0x80023F0C-0x80024148 (572 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
typedef struct cr_TagTeam
{
  u8 active;
  u8 unk_01[3];
  u32 unk_04;
  u8 unk_08[0x10];
} cr_TagTeam;
__declspec(section ".data") extern cr_TagTeam cr_g_TagTeams[2];
extern u8 lbl_80321E90[5088];

/* fn_80023F0C: Nov 3, 2026 hand draft (m2c 86%): the search for the first live robo as a for loop over the bitmask (signed test), an int bit index, the update pointer tested against a cast NULL; battle_mode compared without the (u8) cast */

void fn_80023F0C(void *arg0) {
    u8 var_r31_0;
    u8 var_r30_74;
    s32 i;
    u32 var_r28_11;

    if (cr_g_GameMode.flags & 0x02000000) {
        for (var_r28_11 = lbl_8044F500; (s32) var_r28_11 != 0; var_r28_11 ^= 0x80000000U >> i) {
            i = __cntlzw(var_r28_11);
            if (!(M2C_FIELD(&cr_g_BattleRobos[i], s32 *, 0x1D8) & 0x80000000) && ((u8 *) cr_g_BattleRobos[i].update != (u8 *) NULL)) {
                var_r31_0 = i;
                break;
            }
        }
    } else if ((s32) cr_g_GameMode.battle_mode != 2) {
        var_r31_0 = 0;
    } else {
        var_r31_0 = cr_g_TagTeams->active;
    }
    if ((s8) cr_g_BattleRobos[var_r31_0].unk_001d6[0x52] != -1) {
        var_r30_74 = cr_g_BattleRobos[var_r31_0].unk_001d6[0x52];
    } else {
        var_r30_74 = 1;
    }
    M2C_FIELD(arg0, s32 *, 0x110) = 0;
    M2C_FIELD(arg0, f32 *, 0x98) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r31_0], f32 *, 0x10E0);
    M2C_FIELD(arg0, f32 *, 0x9C) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r31_0], f32 *, 0x10F0);
    M2C_FIELD(arg0, f32 *, 0xA0) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r31_0], f32 *, 0x1100);
    M2C_FIELD(arg0, f32 *, 0xC8) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r30_74], f32 *, 0x20);
    M2C_FIELD(arg0, f32 *, 0xCC) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r30_74], f32 *, 0x1480);
    M2C_FIELD(arg0, f32 *, 0xD0) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r30_74], f32 *, 0x28);
    M2C_FIELD(arg0, f32 *, 0xD4) = (f32) M2C_FIELD(arg0, f32 *, 0xC8);
    M2C_FIELD(arg0, f32 *, 0xD8) = (f32) M2C_FIELD(arg0, f32 *, 0xCC);
    M2C_FIELD(arg0, f32 *, 0xDC) = (f32) M2C_FIELD(arg0, f32 *, 0xD0);
    lbl_80321E90[var_r31_0] = 0;
}
