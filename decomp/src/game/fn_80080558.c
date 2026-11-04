/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80080558: .text 0x80080558-0x8008057C (36 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F640[2];
void fn_8008052C(void *arg0);

/* fn_80080558: Nov 3, 2026 hand draft (m2c 66%): the test as a pointer compare against a cast NULL, `(u8 *) x != (u8 *) NULL`, which keeps the zero in a register (`li r0, 0; cmplw r3, r0`) */

void fn_80080558(void) {

    if ((u8 *) *lbl_8044F640 != (u8 *) NULL) {
        M2C_FIELD(*lbl_8044F640, void (**)(void *), 0x50) = fn_8008052C;
    }
}
