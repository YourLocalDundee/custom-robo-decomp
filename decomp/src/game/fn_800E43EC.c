/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800E43EC: .text 0x800E43EC-0x800E4618 (556 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
void fn_800E4618(void *arg0, void *arg1);
extern f32 lbl_8045151C;
void fn_800E4770(void *arg0);

void fn_800E43EC(u8 *arg0) {
    if (cr_g_BattleRobos[M2C_FIELD(arg0, s8 *, 0x6B)].unk_000e4[2] == 0) {
        if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, s8 *, 0x6B)], s16 *, 0x18A2) == 1) {
            fn_800E4618(arg0, arg0 + 0x378A);
        }
        if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, s8 *, 0x6B)], s16 *, 0x18A2) == 2) {
            fn_800E4618(arg0, arg0 + 0x378C);
        }
        if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, s8 *, 0x6B)], s16 *, 0x18A2) == 3) {
            fn_800E4618(arg0, arg0 + 0x378E);
        }
        if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, s8 *, 0x6B)], s16 *, 0x18A2) == 4) {
            fn_800E4618(arg0, arg0 + 0x3790);
        }
        if (M2C_FIELD(arg0, u8 *, 0x17B7) != 0) {
            M2C_FIELD(arg0, u8 *, 0x17B7) -= 1;
        } else {
            M2C_FIELD(arg0, f32 *, 0x74) = (f32) lbl_8045151C;
            M2C_FIELD(arg0, u8 *, 0x72) &= 0xFE;
        }
        if (M2C_FIELD(arg0, u8 *, 0x17B8) != 0) {
            M2C_FIELD(arg0, u8 *, 0x17B8) -= 1;
        } else {
            M2C_FIELD(arg0, f32 *, 0x78) = (f32) M2C_FIELD(arg0, f32 *, 0x3720);
            M2C_FIELD(arg0, u8 *, 0x72) &= 0xFFFFFFFD;
        }
        if (M2C_FIELD(arg0, u8 *, 0x17B9) != 0) {
            M2C_FIELD(arg0, u8 *, 0x17B9) -= 1;
        } else {
            M2C_FIELD(arg0, f32 *, 0x7C) = (f32) M2C_FIELD(arg0, f32 *, 0x3724);
            M2C_FIELD(arg0, u8 *, 0x72) &= 0xFFFFFFFB;
        }
        if (M2C_FIELD(arg0, u8 *, 0x17BA) != 0) {
            M2C_FIELD(arg0, u8 *, 0x17BA) -= 1;
        } else {
            M2C_FIELD(arg0, f32 *, 0x80) = (f32) M2C_FIELD(arg0, f32 *, 0x3728);
            M2C_FIELD(arg0, u8 *, 0x72) &= 0xFFFFFFF7;
        }
        if (M2C_FIELD(arg0, u8 *, 0x17BB) != 0) {
            M2C_FIELD(arg0, u8 *, 0x17BB) -= 1;
        } else {
            M2C_FIELD(arg0, f32 *, 0x84) = (f32) M2C_FIELD(arg0, f32 *, 0x3718);
            M2C_FIELD(arg0, u8 *, 0x72) &= 0xFFFFFFEF;
        }
        fn_800E4770(arg0);
    }
}
