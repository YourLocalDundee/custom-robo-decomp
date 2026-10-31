/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80015E04: .text 0x80015E04-0x80015E18 (20 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern s16 lbl_8044F366;
extern u32 lbl_80321D40[18];

void fn_80015E04(void) {
    lbl_8044F366 = M2C_FIELD(lbl_80321D40, s16 *, 0x18);
}
