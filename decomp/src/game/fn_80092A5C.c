/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80092A5C: .text 0x80092A5C-0x80092CB8 (604 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F66D;
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
extern u32 lbl_8021A308[5];
void fn_80098290(s32 arg0);
extern u8 lbl_8044F672;
extern u8 lbl_8044F66F;
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
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
void fn_800440BC(void *arg0);
void fn_8003F000(void *arg0);
void fn_800410C0(void *arg0);
extern u16 lbl_8044F44C;
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
typedef struct cr_ScenarioSlot
{
  u32 status;
  u32 flags[32];
  u16 unk_84[6];
  u16 battle_count;
  u16 unk_92[2];
  u8 unk_96[2];
  u32 unk_98[5];
  u32 unk_ac[2];
  u32 unk_b4;
  u16 unk_b8;
  u16 unk_ba;
  u8 unk_bc[0x10];
} cr_ScenarioSlot;
typedef struct cr_ScenarioState
{
  u16 unk_000;
  char name[18];
  u32 error_flags;
  u32 unk_018;
  u32 unk_01c;
  u32 unk_020;
  s32 wait_timer;
  s32 unk_028;
  u8 unk_02c;
  u8 unk_02d;
  u8 unk_02e;
  u8 slot_index;
  cr_ScenarioSlot slot[2];
  u32 unk_1c8[4];
  u32 unk_1d8[32];
  u32 unk_258[10];
  u32 unk_280;
  u8 unk_284;
  u8 unk_285[3];
  u32 part_bits[5][8];
  u32 unk_328[2];
  u8 unk_330;
  u8 pickup_battle;
  u8 unk_332[2];
  u32 unk_334[4];
  u32 unk_344[4];
  u32 unk_354[4];
} cr_ScenarioState;
__declspec(section ".data") extern cr_ScenarioState cr_g_Scenario;

/* fn_80092A5C: Nov 5, 2026 hand draft (m2c 87%): a pad handler (states 1 and 4 of lbl_8044F66D, a 0 / 1 choice in lbl_8044F672); returns var_r30 (m2c: 1); the pad index is the parameter itself (no m2c alias); a folded local `id` (copied once into the first fn_80098290 call) holds the spare r29 of the original; and a bare compare of slot_index with no branch is `(c) ? (void) 0 : (void) 0` */

s32 fn_80092A5C(u8 arg0) {
    u8 var_r31;
    s32 var_r30;
    s32 id;

    var_r30 = 1;
    if (lbl_8044F66D == 1) {
        if (cr_g_Pads[arg0].unk_14 & 0x10) {
            id = (s32) lbl_8021A308[1];
            fn_80098290(id);
            lbl_8044F672 = 0;
            lbl_8044F66D = 2;
            lbl_8044F66F = 0;
        } else if (cr_g_Pads[arg0].unk_14 & 0x100) {
            switch ((s8) lbl_8044F672) {            /* switch 1; irregular */
            case 0:                                 /* switch 1 */
                fn_80098290((s32) lbl_8021A308[1]);
                break;
            case 1:                                 /* switch 1 */
                fn_80098290((s32) M2C_FIELD(lbl_8021A308, u32 *, 0));
                break;
            }
            lbl_8044F66D = 2;
            lbl_8044F66F = 0;
        }
    } else if (lbl_8044F66D == 4) {
        switch ((s8) lbl_8044F672) {                /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            lbl_8044F66D = 0;
            cr_g_SeqCurrent.unk_38 &= 0xFFFBFFFF;
            break;
        case 1:                                     /* switch 2 */
            lbl_8044F66D = 0;
            var_r31 = 0;
            cr_g_SeqCurrent.unk_10 = 9;
            while (var_r31 < 4U) {
                if (((void (*)()) cr_g_BattleRobos[var_r31].update) != NULL) {
                    cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[var_r31];
                    fn_800440BC(cr_g_CurrentBattleRobo + 0x1E20);
                    fn_8003F000(cr_g_CurrentBattleRobo + 0x2E2E0);
                    fn_800410C0(cr_g_CurrentBattleRobo + 0x28D40);
                }
                var_r31 += 1;
            }
            lbl_8044F44C |= 0x400;
            if (cr_g_GameMode.flags & 0x800000) {
                (cr_g_Scenario.slot_index == 0) ? (void) 0 : (void) 0; /* an assert without its message call */
                cr_g_Scenario.unk_02e |= 2;
                cr_g_Scenario.unk_02e |= 4;
            }
            break;
        }
    }
    return var_r30;
}
