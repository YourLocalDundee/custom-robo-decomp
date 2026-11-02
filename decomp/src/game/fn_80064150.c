/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80064150: .text 0x80064150-0x80064234 (228 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern f32 lbl_80450A70;

void fn_80064150(void *arg0) {
    u8 *var_r31_11;

    var_r31_11 = (void *) (*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 8), void **, 0x5C), s32 *, 0x1C)))[M2C_FIELD(M2C_FIELD(arg0, void **, 8), u8 *, 0x5A)][0x4]);
    while (M2C_FIELD(var_r31_11, u16 *, 0x10) != 0xFFFF) {
        if (M2C_FIELD(var_r31_11, u16 *, 0x12) & 0x8000) {
            if (cr_g_Pads->dir & 1) {
                M2C_FIELD(var_r31_11, f32 *, 4) -= lbl_80450A70;
            }
            if (cr_g_Pads->dir & 4) {
                M2C_FIELD(var_r31_11, f32 *, 4) += lbl_80450A70;
            }
            if (cr_g_Pads->dir & 2) {
                M2C_FIELD(var_r31_11, f32 *, 0) += lbl_80450A70;
            }
            if (cr_g_Pads->dir & 8) {
                M2C_FIELD(var_r31_11, f32 *, 0) -= lbl_80450A70;
            }
        }
        var_r31_11 += 0x20;
    }
}
