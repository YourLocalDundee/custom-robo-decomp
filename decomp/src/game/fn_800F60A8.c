/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F60A8: .text 0x800F60A8-0x800F60EC (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

u8 fn_80099538(M2C_UNK);                            /* extern */

s32 fn_800F60A8(void) {
    s32 var_r31_10;

    if (fn_80099538(0x10000) != 0) {
        var_r31_10 = 0x10000;
    } else {
        var_r31_10 = -1;
    }
    return var_r31_10;
}
