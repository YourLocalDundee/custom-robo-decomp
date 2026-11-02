/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80089F40: .text 0x80089F40-0x80089FB8 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80392A44[4];

/* fn_80089F40: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the list head compared with ((void *) 0), the entries read by index */
s32 fn_80089F40(void) {
    u8 var_r31_15;
    s32 var_r30_14;

    if ((void *) lbl_80392A44[0] == ((void *) 0)) {
        return 0;
    }
    var_r30_14 = 0;
    var_r31_15 = 0;
    while (var_r31_15 < 4U) {
        var_r30_14 |= M2C_FIELD(lbl_80392A44[var_r31_15], s32 *, 0x18);
        var_r31_15 += 1;
    }
    return var_r30_14;
}
