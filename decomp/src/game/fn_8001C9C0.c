/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001C9C0: .text 0x8001C9C0-0x8001C9F4 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8001C9C0(void *arg0, void *arg1) {
    M2C_FIELD(arg0, f32 *, 0) -= M2C_FIELD(arg1, f32 *, 0);
    M2C_FIELD(arg0, f32 *, 4) -= M2C_FIELD(arg1, f32 *, 4);
    M2C_FIELD(arg0, f32 *, 8) -= M2C_FIELD(arg1, f32 *, 8);
}
