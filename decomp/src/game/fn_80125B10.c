/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80125B10: .text 0x80125B10-0x80125B58 (72 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041CDE4[6996];

void fn_80125B10(void) {
    lbl_8041CDE4[4] = 8;
    lbl_8041CDE4[1] = M2C_FIELD(lbl_8041CDE4, u8 *, 0);
    M2C_FIELD(lbl_8041CDE4, u8 *, 0) = 4;
    lbl_8041CDE4[2] = 0;
}
