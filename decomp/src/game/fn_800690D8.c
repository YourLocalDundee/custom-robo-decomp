/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800690D8: .text 0x800690D8-0x800690F4 (28 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

s32 fn_800690D8(void *arg0, void *arg1, s32 arg2) {
    M2C_FIELD(arg1, u8 *, 0) |= 4;
    M2C_FIELD(arg1, s32 *, 0x20) = (s32) M2C_FIELD(arg0, u8 *, 5);
    return arg2;
}
