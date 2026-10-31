/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008B95C: .text 0x8008B95C-0x8008BA44 (232 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8021A9A4[10];
extern f32 lbl_8021A9CC[13];
extern f32 lbl_80450FEC;

void fn_8008B95C(void *arg0) {

    M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) -= ((lbl_8021A9A4[9] - lbl_8021A9CC[9]) / lbl_80450FEC);
    M2C_FIELD(arg0, s32 *, 0x1C) += 1;
    switch ((M2C_FIELD(arg0, s32 *, 0x10))) {                           /* irregular */
    case 0:
        if (M2C_FIELD(arg0, s32 *, 0x1C) < 0xF0) {
            M2C_FIELD(arg0, u8 *, 0x57) += 1;
        } else {
            M2C_FIELD(arg0, s32 *, 0x10) += 1;
        }
        break;
    case 1:
        if (M2C_FIELD(arg0, s32 *, 0x1C) < 0x1E0) {
            M2C_FIELD(arg0, u8 *, 0x57) -= 1;
        } else {
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
        }
        break;
    }
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) < lbl_8021A9CC[9]) {
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8021A9A4[9];
    }
}
