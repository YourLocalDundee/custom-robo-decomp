/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011E9E0: .text 0x8011E9E0-0x8011EA3C (92 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80240828[144];
extern u8 lbl_8041CBC0[16];

/* fn_8011E9E0: Nov 13, 2026 hand draft: m2c 60%: a call through a two-dimensional table of functions, `((void (*(*)[2])(void)) table)[(s8) state[0]][state[1]]()`; m2c passed leftover registers as arguments */

void fn_8011E9E0(void) {
    ((void (*(*)[2])(void)) lbl_80240828)[(s8) M2C_FIELD(lbl_8041CBC0, s8 *, 0)][lbl_8041CBC0[1]]();
}
