/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B2300: .text 0x800B2300-0x800B2338 (56 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044EB2C[8];
extern u8 *lbl_8044F744;

/* fn_800B2300: Nov 13, 2026 hand draft: one-call function table: ((void (**)(void)) tbl)[state]() */

void fn_800B2300(void) {
    ((void (**)(void)) lbl_8044EB2C)[M2C_FIELD(lbl_8044F744, s32 *, 4)]();
}
