/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80020204: .text 0x80020204-0x800202EC (232 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u32 lbl_8044F3F0[2];

/* fn_80020204: hand-written from main.dol's asm on its m2c draft (step 109b): the third counter array as p->unk_70[i] (m2c's byte address in a temporary took a saved register) */
s32 fn_80020204(void) {
    cr_Pad *var_r31_6;
    s32 var_r30_29;
    s32 var_r29_9;

    var_r31_6 = cr_g_Pads;
    var_r29_9 = 0;
    *lbl_8044F3F0 = 0;
    while (var_r29_9 < 4) {
        var_r31_6->prev_held = 0;
        var_r31_6->held = 0;
        var_r31_6->pressed = 0;
        var_r31_6->released = 0;
        var_r30_29 = 0;
        var_r31_6->not_held = -1;
        var_r31_6->prev_dir = 0;
        var_r31_6->dir = 0;
        var_r31_6->unk_14 = 0;
        var_r31_6->unk_18 = 0;
        var_r31_6->unk_20 = -1;
        var_r31_6->unk_28 = 0;
        var_r31_6->unk_2c = 0;
        while (var_r30_29 < 0x20) {
            var_r31_6->unk_30[var_r30_29] = 0x14;
            var_r31_6->unk_50[var_r30_29] = 0x14;
            var_r31_6->unk_70[var_r30_29] = 0;
            var_r30_29 += 1;
        }
        var_r29_9 += 1;
        var_r31_6 = (cr_Pad *) ((u8 *) var_r31_6 + 0xA4);
    }
    return 0;
}
