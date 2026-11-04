/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80065DB0: .text 0x80065DB0-0x80065E10 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8032CC20[8368];

/* fn_80065DB0: Nov 3, 2026 hand draft (m2c 70%): the field tested as a pointer against a cast NULL (`li r0, 0; cmplw`) */

void fn_80065DB0(void) {
    u32 var_r31_4;

    var_r31_4 = 0;
    while (var_r31_4 < 2U) {
        if ((u8 *) M2C_FIELD((lbl_8032CC20 + (var_r31_4 * 0x28)), void **, 8) != (u8 *) NULL) {
            *(lbl_8032CC20 + (var_r31_4 * 0x28)) = 3;
        }
        var_r31_4 += 1;
    }
}
