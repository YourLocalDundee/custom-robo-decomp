/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B1B1C: .text 0x800B1B1C-0x800B1B2C (16 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern void *lbl_8044F744;

void fn_800B1B1C(void) {
    M2C_FIELD(lbl_8044F744, s32 *, 4) = 1;
}
