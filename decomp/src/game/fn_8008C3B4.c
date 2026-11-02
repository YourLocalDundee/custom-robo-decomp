/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008C3B4: .text 0x8008C3B4-0x8008C430 (124 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8008C3B4(void *arg0, s32 arg1) {
    u8 var_r31_4;

    var_r31_4 = 0;
    while (var_r31_4 < 4U) {
        *(u8 *) &((u8 (*)[0x20]) (arg0))[var_r31_4][0x1B] = 0xFF;
        var_r31_4 += 1;
    }
    if (arg1 & 1) {
        M2C_FIELD(arg0, s8 *, 0x1B) = 0;
    }
    if (arg1 & 2) {
        M2C_FIELD(arg0, s8 *, 0x3B) = 0;
    }
    if (arg1 & 8) {
        M2C_FIELD(arg0, s8 *, 0x5B) = 0;
    }
    if (arg1 & 4) {
        M2C_FIELD(arg0, s8 *, 0x7B) = 0;
    }
}
