/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_C_ACT_JP: .text 0x800BDB24-0x800BDB54 (48 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern u8 *lbl_8044F778;

void cr_C_ACT_JP(void) {
    void *temp_r31_5;

    temp_r31_5 = lbl_8044F778;
    lbl_8044F778 += 8;
    lbl_8044F778 = *M2C_FIELD(temp_r31_5, void ***, 4);
}
