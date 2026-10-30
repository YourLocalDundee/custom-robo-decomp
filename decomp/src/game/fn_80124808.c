/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80124808: .text 0x80124808-0x8012481C (20 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern M2C_UNK lbl_8044F885;

void fn_80124808(s8 arg0) {
    M2C_FIELD(&lbl_8044F885, s8 *, 0) = arg0;
    M2C_FIELD(&lbl_8044F885, s8 *, 1) = 0;
}
