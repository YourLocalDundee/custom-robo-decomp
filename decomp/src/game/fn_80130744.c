/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80130744: .text 0x80130744-0x8013079C (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_802426C8[24];
extern u8 lbl_8041E938[24];

/* fn_80130744: Nov 13, 2026 hand draft: two-level function table [u8][u8] */

void fn_80130744(void) {
    ((void (*(*)[2])(void)) lbl_802426C8)[lbl_8041E938[0]][lbl_8041E938[2]]();
}
