/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Pad_StepIndex: .text 0x8001F558-0x8001F654 (252 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_Pad cr_Pad;
struct cr_Pad
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
};
__declspec(section ".data") extern cr_Pad cr_g_Pads[4];

s32 cr_Pad_StepIndex(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u32 arg6) {
    cr_Pad *temp_r31_19;
    s32 var_r30_5;
    u32 var_r29_0;
    s32 var_r28_8;

    var_r30_5 = 0;
    var_r28_8 = 0;
    while (var_r30_5 < 4) {
        if (arg5 & (1 << var_r30_5)) {
            temp_r31_19 = &cr_g_Pads[var_r30_5];
            if (arg6 == 0) {
                var_r29_0 = temp_r31_19->dir;
            } else if (arg6 == 1) {
                var_r29_0 = temp_r31_19->unk_14;
            } else if (arg6 == 2) {
                var_r29_0 = temp_r31_19->unk_28;
            } else if (arg6 == 3) {
                var_r29_0 = temp_r31_19->unk_2c;
            }
            if (var_r29_0 & arg4) {
                if ((s32) *arg0 < arg2) {
                    *arg0 += 1;
                } else {
                    *arg0 = arg1;
                }
                var_r28_8 = 1;
            }
            if (var_r29_0 & arg3) {
                if ((s32) *arg0 > arg1) {
                    *arg0 -= 1;
                } else {
                    *arg0 = arg2;
                }
                var_r28_8 = 1;
            }
        }
        var_r30_5 += 1;
    }
    return var_r28_8;
}
