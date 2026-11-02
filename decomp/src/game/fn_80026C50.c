/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026C50: .text 0x80026C50-0x80026CA0 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_80026C50(u8 *arg0) {
    u8 *var_r31_4;
    u8 *var_r30_6;
    u32 var_r29_8;

    var_r31_4 = arg0;
    var_r30_6 = arg0 + 0x1AC;
    var_r29_8 = 0U;
    while (var_r29_8 < 0x1A4U) {
        *var_r30_6 = *var_r31_4;
        var_r29_8 += 1;
        var_r31_4 += 1;
        var_r30_6 += 1;
    }
}
