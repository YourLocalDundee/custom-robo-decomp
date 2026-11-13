/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800929E0: .text 0x800929E0-0x80092A5C (124 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F66E;
extern u8 lbl_8021A584[12];

/* fn_800929E0: Nov 13, 2026 hand draft: u8-returning function table indexed by state - 1 */

u8 fn_800929E0(void) {
    u8 var_r31_6;

    var_r31_6 = 4;
    if ((lbl_8044F66E != 0) && (lbl_8044F66E < 4U)) {
        lbl_8044F66E = ((u8 (**)(void)) lbl_8021A584)[lbl_8044F66E - 1]();
        if (lbl_8044F66E != 4) {
            var_r31_6 = 0;
        }
    }
    return var_r31_6;
}
