/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80067250: .text 0x80067250-0x800672AC (92 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008EE68(void *arg0, void *arg1);
extern u8 lbl_80396258[544];

/* fn_80067250: Nov 3, 2026 hand draft (m2c 73%): the out-parameter is a record of at least 0x18 bytes (fn_8006975C passes one with an f32 at 0x14), which gives the 0x30 frame */

void fn_80067250(void) {
    u8 sp8[0x18];
    u8 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 4U) {
        fn_8008EE68(M2C_FIELD((lbl_80396258 + (var_r31_6 * 0x88)), void **, 4), &sp8);
        var_r31_6 += 1;
    }
}
