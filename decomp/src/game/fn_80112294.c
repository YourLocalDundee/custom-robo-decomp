/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80112294: .text 0x80112294-0x8011266C (984 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_StageSelectorState
{
  u8 state;
  u8 col;
  u8 row;
  u8 unk_003;
  u8 top_row;
  u8 stage_count;
  u8 stage;
  u8 cursor;
  u32 timer;
  u8 ready;
  u8 unk_00d;
  u8 unk_00e[2];
  u32 flags;
  s8 stage_ids[42];
  u16 stage_msgs[42];
  u8 stage_open[42];
  u8 unk_0bc;
  u8 unk_0bd;
  u8 unk_0be;
  u8 unk_0bf;
  u8 unk_0c0[2];
  u8 unk_0c2;
  u8 unk_0c3[7];
  u8 unk_0ca[2];
  f32 unk_0cc;
  f32 unk_0d0;
  u32 unk_0d4;
  u16 unk_0d8;
  u16 unk_0da;
  u16 unk_0dc;
  u16 unk_0de;
  u8 unk_0e0[0x5C];
  void *unk_13c;
  u8 windows[8][0x34];
  u8 unk_2e0[0x68];
  u32 unk_348;
  u8 unk_34c;
  u8 unk_34d[3];
  s8 unk_350;
  u8 unk_351[3];
} cr_StageSelectorState;
__declspec(section ".data") extern cr_StageSelectorState cr_g_StageSelector;
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
extern u32 lbl_8023CE54[43];
void fn_80098290(s32 arg0);

/* fn_80112294: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the remainder written out as `x - (s8) ((s8) x / 6) * 6` (main.dol loads the byte twice: extsb, mulhw, the s8 quotient, mulli, subf from the second load), where `x % 6` loads it once */
u8 fn_80112294(u8 arg0) {
    u8 var_r31_12;
    u32 var_r30_14;
    s8 var_r29_27;

    var_r31_12 = 0;
    var_r30_14 = -1;
    var_r29_27 = cr_g_StageSelector.stage_count - (s8) ((s8) cr_g_StageSelector.stage_count / 6) * 6;
    if (var_r29_27 == 0) {
        var_r29_27 = 6;
    }
    if (cr_g_Pads[arg0].unk_28 & 0x01000000) {
        var_r30_14 = lbl_8023CE54[2];
        var_r31_12 = 1;
        if (cr_g_Pads[arg0].dir & 0x100000) {
            cr_g_StageSelector.top_row -= 2;
        } else {
            cr_g_StageSelector.row -= 1;
            if ((s8) cr_g_StageSelector.row < 0) {
                cr_g_StageSelector.row = 0;
                cr_g_StageSelector.top_row -= 1;
            }
        }
        if ((s8) cr_g_StageSelector.top_row < 0) {
            var_r30_14 = -1;
            cr_g_StageSelector.top_row = 0;
            var_r31_12 = 0;
        }
    } else if (cr_g_Pads[arg0].unk_28 & 0x04000000) {
        var_r30_14 = lbl_8023CE54[3];
        var_r31_12 = 1;
        if (cr_g_Pads[arg0].dir & 0x100000) {
            cr_g_StageSelector.top_row += 2;
        } else {
            cr_g_StageSelector.row += 1;
            if ((s8) cr_g_StageSelector.row > 1) {
                cr_g_StageSelector.row = 1;
                cr_g_StageSelector.top_row += 1;
            }
        }
        if ((s8) cr_g_StageSelector.top_row >= 5) {
            var_r30_14 = -1;
            cr_g_StageSelector.top_row = 4;
            var_r31_12 = 0;
        }
    } else if (cr_g_Pads[arg0].unk_28 & 0x08000000) {
        var_r30_14 = lbl_8023CE54[5];
        var_r31_12 = 1;
        cr_g_StageSelector.col -= 1;
        if ((s8) cr_g_StageSelector.col < 0) {
            if (((s32) (u8) cr_g_StageSelector.row == 1) && ((s32) (u8) cr_g_StageSelector.top_row == 4)) {
                cr_g_StageSelector.col = var_r29_27 - 1;
            } else {
                cr_g_StageSelector.col = 5;
            }
        }
    } else if (cr_g_Pads[arg0].unk_28 & 0x02000000) {
        var_r30_14 = lbl_8023CE54[4];
        var_r31_12 = 1;
        cr_g_StageSelector.col += 1;
        if (((s32) (u8) cr_g_StageSelector.row != 1) || ((s32) (u8) cr_g_StageSelector.top_row != 4)) {
            var_r29_27 = 6;
        }
        if ((s8) cr_g_StageSelector.col > (s32) (var_r29_27 - 1)) {
            var_r31_12 = 1;
            cr_g_StageSelector.col = 0;
        }
    }
    if ((s32) var_r30_14 >= 0) {
        fn_80098290((s32) var_r30_14);
    }
    if (var_r31_12 != 0) {
        cr_g_StageSelector.unk_0bf = 0;
    }
    return var_r31_12;
}
