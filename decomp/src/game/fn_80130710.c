/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80130710: .text 0x80130710-0x80130744 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041E938[24];

void fn_80130710(u8 arg0) {
    lbl_8041E938[1] = M2C_FIELD(lbl_8041E938, u8 *, 0);
    M2C_FIELD(lbl_8041E938, u8 *, 0) = arg0;
    lbl_8041E938[2] = 0;
}
