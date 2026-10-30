/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EB100: .text 0x800EB100-0x800EB158 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F7D0[2];
u32 ARAlloc(u32 length);
extern u8 lbl_803F63E8[56];

void fn_800EB100(void) {
    s32 var_r31_9;

    *lbl_8044F7D0 = ARAlloc(0x600000U);
    var_r31_9 = 0;
    while (var_r31_9 < 0x20) {
        *(u8 *) (&lbl_803F63E8[var_r31_9]) = 0;
        var_r31_9 += 1;
    }
}
