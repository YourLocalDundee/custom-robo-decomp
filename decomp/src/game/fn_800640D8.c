/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800640D8: .text 0x800640D8-0x80064150 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8032CC20[8368];

s16 fn_800640D8(void) {
    s16 var_r31;
    s16 var_r30;
    s16 var_r29;
    s16 var_r28;
    s16 var_r27;

    var_r30 = 0;
    var_r31 = 0;
    while (var_r31 < 2) {
        var_r28 = 0;
        if ((u8) *(lbl_8032CC20 + (var_r31 * 0x28)) == 3) {
            var_r28 = 1;
        }
        var_r27 = var_r28;
        var_r29 = var_r27;
        if (var_r30 < var_r29) {
            var_r30 = var_r29;
        }
        var_r31 += 1;
    }
    return var_r30;
}
