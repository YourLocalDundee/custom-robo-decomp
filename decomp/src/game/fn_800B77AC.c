/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B77AC: .text 0x800B77AC-0x800B7810 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803BADA8[384];

void fn_800B77AC(void) {
    s32 var_r31_4;
    s32 var_r30_8;

    var_r31_4 = 0;
    while (var_r31_4 < 2) {
        var_r30_8 = 0;
        while (var_r30_8 < 2) {
            M2C_FIELD((lbl_803BADA8 + (var_r31_4 * 0xC0) + (var_r30_8 * 0x60)), u32 *, 0) = 0;
            var_r30_8 += 1;
        }
        var_r31_4 += 1;
    }
}
