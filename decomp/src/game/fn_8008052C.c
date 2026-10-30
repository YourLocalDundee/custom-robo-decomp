/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008052C: .text 0x8008052C-0x80080558 (44 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8008052C(void *arg0) {
    if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 0x20) < 0xFF) {
        M2C_FIELD(arg0, u8 *, 0x57) += 0x20;
        return;
    }
    M2C_FIELD(arg0, u8 *, 0x57) = 0xFFU;
}
