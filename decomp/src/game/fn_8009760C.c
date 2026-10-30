/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8009760C: .text 0x8009760C-0x80097670 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803B76E8[1344];

void fn_8009760C(s32 arg0) {
    u32 *var_r31_5;
    u16 var_r30_7;

    var_r31_5 = lbl_803B76E8;
    var_r30_7 = 0;
    while (var_r30_7 < 0x60U) {
        if (((s32) var_r31_5[2] == arg0) && (M2C_FIELD(var_r31_5, u32 *, 0) & 0x80000000)) {
            M2C_FIELD(var_r31_5, u32 *, 0) |= 0x80;
        }
        var_r30_7 += 1;
        var_r31_5 += 0xE;
    }
}
