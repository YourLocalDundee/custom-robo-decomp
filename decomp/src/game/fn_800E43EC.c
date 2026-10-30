/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800E43EC: .text 0x800E43EC-0x800E4618 (556 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8024E120[840832];
void fn_800E4618(void *arg0, void *arg1);
extern f32 lbl_8045151C;
void fn_800E4770(void *arg0);

void fn_800E43EC(u8 *arg0) {
    if (M2C_FIELD((lbl_8024E120 + (M2C_FIELD(arg0, s8 *, 0x6B) * 0x33520)), u8 *, 0xE6) == 0) {
        if (M2C_FIELD((lbl_8024E120 + (M2C_FIELD(arg0, s8 *, 0x6B) * 0x33520)), s16 *, 0x18A2) == 1) {
            fn_800E4618(arg0, arg0 + 0x378A);
        }
        if (M2C_FIELD((lbl_8024E120 + (M2C_FIELD(arg0, s8 *, 0x6B) * 0x33520)), s16 *, 0x18A2) == 2) {
            fn_800E4618(arg0, arg0 + 0x378C);
        }
        if (M2C_FIELD((lbl_8024E120 + (M2C_FIELD(arg0, s8 *, 0x6B) * 0x33520)), s16 *, 0x18A2) == 3) {
            fn_800E4618(arg0, arg0 + 0x378E);
        }
        if (M2C_FIELD((lbl_8024E120 + (M2C_FIELD(arg0, s8 *, 0x6B) * 0x33520)), s16 *, 0x18A2) == 4) {
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
