/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026BF4: .text 0x80026BF4-0x80026C50 (92 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_80026BF4(u8 *arg0, s16 arg1) {
    u8 *var_r31_6;
    u8 *var_r30_9;
    u32 var_r29_11;

    var_r31_6 = arg0;
    var_r30_9 = &((u8 (*)[0x1A4]) (arg0))[arg1][0x350];
    var_r29_11 = 0U;
    while (var_r29_11 < 0x1A4U) {
        *var_r30_9 = *var_r31_6;
        var_r29_11 += 1;
        var_r31_6 += 1;
        var_r30_9 += 1;
    }
}
