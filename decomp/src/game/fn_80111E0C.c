/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80111E0C: .text 0x80111E0C-0x80111E54 (72 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041A244[852];

void fn_80111E0C(s32 arg0) {
    u8 var_r31_11;

    M2C_FIELD(lbl_8041A244, s32 *, 0x10) &= ~arg0;
    if (arg0 & 0x2C) {
        var_r31_11 = 0;
    } else {
        var_r31_11 = M2C_FIELD(lbl_8041A244, u8 *, 0);
    }
    M2C_FIELD(lbl_8041A244, u8 *, 0) = var_r31_11;
}
