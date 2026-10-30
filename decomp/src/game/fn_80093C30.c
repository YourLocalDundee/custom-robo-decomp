/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80093C30: .text 0x80093C30-0x80093C5C (44 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

s32 fn_80093C30(void *arg0) {
    s32 var_r31_6;

    if (arg0 != NULL) {
        var_r31_6 = M2C_FIELD(arg0, s32 *, 4);
    } else {
        var_r31_6 = 0;
    }
    return var_r31_6;
}
