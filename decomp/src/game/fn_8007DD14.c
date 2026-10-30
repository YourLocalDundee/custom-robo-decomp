/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007DD14: .text 0x8007DD14-0x8007DD60 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern s16 lbl_8044EAD4;
extern s16 lbl_8044EAD6;
extern s16 lbl_8044EAD8;
extern s16 lbl_8044EADA;
extern s16 lbl_8044EADC;
extern s32 lbl_8044F620;

void fn_8007DD14(s32 arg0, s16 arg1) {
    if (arg0 & 0x40000000) {
        lbl_8044EADC = arg1;
    }
    if (arg0 & 4) {
        lbl_8044EAD8 = arg1;
    }
    if (arg0 & 8) {
        lbl_8044EADA = arg1;
    }
    if (arg0 & 2) {
        lbl_8044EAD6 = arg1;
    }
    if (arg0 & 1) {
        lbl_8044EAD4 = arg1;
    }
    lbl_8044F620 |= arg0;
}
