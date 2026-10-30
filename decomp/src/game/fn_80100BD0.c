/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80100BD0: .text 0x80100BD0-0x80100C30 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];

s32 fn_80100BD0(void) {
    s32 var_r30_6;
    u8 var_r31_4;

    var_r31_4 = 0;
    var_r30_6 = 0;
    while (var_r31_4 < 4U) {
        if (M2C_FIELD((lbl_80407260 + (var_r31_4 * 0x4994)), s32 *, 0x498C) & 0x200) {
            var_r30_6 += 1;
        }
        var_r31_4 += 1;
    }
    return var_r30_6;
}
