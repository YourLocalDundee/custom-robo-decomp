/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80098F4C: .text 0x80098F4C-0x80098F8C (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_801735EC(void);

s32 fn_80098F4C(void) {
    s32 var_r31_9;

    if (fn_801735EC() != 0) {
        var_r31_9 = 1;
    } else {
        var_r31_9 = 0;
    }
    return var_r31_9;
}
