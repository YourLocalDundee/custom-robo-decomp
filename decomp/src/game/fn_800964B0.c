/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800964B0: .text 0x800964B0-0x80096528 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803B7678[28];

u8 fn_800964B0(s32 arg0) {
    u8 var_r31_4;

    var_r31_4 = 0;
    while (!(var_r31_4 >= 2U)) {
        if ((arg0 == (s32) M2C_FIELD((((u32 *) ((u8 *) lbl_803B7678 + (var_r31_4 * 0x38)))), s32 *, 8)) && (*(((u32 *) ((u8 *) lbl_803B7678 + (var_r31_4 * 0x38)))) & 0x80000000)) {
            return 1U;
        }
        var_r31_4 += 1;
    }
    return 0U;
}
