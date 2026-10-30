/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8011413C: .text 0x8011413C-0x80114160 (36 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8041A234[4];

void fn_8011413C(void) {
    M2C_FIELD(lbl_8041A234, s8 *, 4) = 1;
    M2C_FIELD(lbl_8041A234, s8 *, 0xC) = 1;
}
