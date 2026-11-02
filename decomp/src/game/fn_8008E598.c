/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008E598: .text 0x8008E598-0x8008E5D0 (56 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8008E598(u8 *arg0, u8 *arg1, u32 arg2) {
    u32 var_r31_4;

    var_r31_4 = 0U;
    while (var_r31_4 < arg2) {
        *arg1 = *arg0;
        arg0 += 1;
        var_r31_4 += 1;
        arg1 += 1;
    }
}
