/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_MIDDLE_Frame: .text 0x80035BCC-0x80035ED4 (776 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80452A40;
extern u32 lbl_80452A44;
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
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
typedef struct cr_CamSystem
{
  u8 unk_00[8];
  u8 unk_08;
  u8 unk_09;
  u8 unk_0a[2];
  u8 *cams[4];
  u8 *unk_1c[4];
  u8 unk_2c;
  u8 unk_2d[3];
  u8 unk_30;
  u8 unk_31[3];
  u32 unk_34;
  u32 unk_38;
  u32 unk_3c;
  u32 unk_40;
  u32 unk_44;
  u32 unk_48[2];
} cr_CamSystem;
__declspec(section ".data") extern cr_CamSystem cr_g_CamSystem;
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
extern u32 lbl_8044F4F0;
typedef struct cr_Pad
{
  u32 pressed;
  u32 released;
  u32 held;
  u32 not_held;
  u32 prev_held;
  u32 unk_14;
  u32 unk_18;
  u32 dir;
  u32 unk_20;
  u32 prev_dir;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[0x20];
  u8 unk_50[0x20];
  u8 unk_70[0x22];
  u16 unk_92;
  u16 trigger_peak[2];
  s8 stick_x;
  s8 stick_y;
  s8 substick_x;
  s8 substick_y;
  u8 trigger_l;
  u8 trigger_r;
  u8 analog_a;
  u8 analog_b;
  u8 err;
  u8 trigger_clicked[2];
  u8 unk_a3;
} cr_Pad;
__declspec(section ".data") extern cr_Pad cr_g_Pads[4];
extern u8 lbl_8044F461;
void fn_8002B3C4(void *arg0);
void fn_8002E04C(void *arg0);
void fn_80023F0C(void *arg0);

/* cr_CAMMODE_MIDDLE_Frame: Nov 3, 2026 hand draft (m2c 91%): `arg0 = arg0;` first (param_self_assign), the "allowed" flag initialised to 0 at the top (m2c dropped the dead store), unk_44 tested without the (u8) cast; the hang on a bad camera count as for (;;) */

void cr_CAMMODE_MIDDLE_Frame(void *arg0) {
    u32 spC;
    u32 sp8;
    u8 var_r29_155;
    u8 var_r30_20;
    arg0 = arg0;

    var_r30_20 = 0;
    sp8 = lbl_80452A40;
    spC = lbl_80452A44;
    if (!(cr_g_SeqCurrent.unk_38 & 0x4000)) {
        var_r30_20 = 1;
        if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) & 0x200) {
            cr_g_CamSystem.unk_30 = 0;
            var_r30_20 = 0;
            M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
        } else if (cr_g_GameMode.battle_mode & 2) {
            if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x336F8) & 0x200) {
                cr_g_CamSystem.unk_30 = 0;
                var_r30_20 = 0;
                M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
            } else {
                var_r30_20 = 1;
            }
        }
        if (var_r30_20 == 0) {
            cr_g_CamSystem.unk_30 = 0;
            M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
        }
        if (((lbl_8044F4F0 == 1) || (cr_g_GameMode.flags & 0x09800000)) && (cr_g_GameMode.unk_44 != 0) && (var_r30_20 != 0)) {
            if ((s8) (u8) cr_g_Pads->substick_y > 0x14) {
                cr_g_CamSystem.unk_30 = 2;
                M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) |= 1;
            } else if ((s8) (u8) cr_g_Pads->substick_y < -0x14) {
                cr_g_CamSystem.unk_30 = 0;
                M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
            }
        }
        if ((u8) cr_g_CamSystem.unk_30 != 2) {
            if ((lbl_8044F461 == 0) || (lbl_8044F461 >= 5U)) {
for (;;) {
                }
            }
            fn_8002B3C4(arg0);
            fn_8002E04C(arg0);
            if (!(cr_g_CamSystem.unk_2c & 2)) {
                var_r29_155 = 0;
                for (; var_r29_155 < 4U; var_r29_155 += 1) {
                    if (!(!(M2C_FIELD(&cr_g_BattleRobos[var_r29_155], s32 *, 0x1D8) & 0x10000000))) {
                        break;
                    }
                }
                if (var_r29_155 == 4) {
                    cr_g_CamSystem.unk_2c |= 2;
                }
            }
            goto block_29;
        }
        fn_80023F0C(arg0);
block_29:
        if (cr_g_CamSystem.unk_2c & 2) {
            if (M2C_FIELD(arg0, u32 *, 0x114) > 9U) {
                M2C_FIELD(arg0, u32 *, 0x114) -= 9;
                return;
            }
            M2C_FIELD(arg0, u32 *, 0x114) += 0xFFF7;
        }
    }
}
