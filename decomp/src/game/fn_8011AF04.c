/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011AF04: .text 0x8011AF04-0x8011AF64 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_802408B8[48];
extern u32 lbl_8041CBAC[5];

/* fn_8011AF04: Nov 13, 2026 hand draft: two-level function table [u32][u32] called with the state block */

void fn_8011AF04(void) {
    ((void (*(*)[2])(void *)) lbl_802408B8)[M2C_FIELD(lbl_8041CBAC, u32 *, 0)][M2C_FIELD(lbl_8041CBAC, u32 *, 4)](lbl_8041CBAC);
}
