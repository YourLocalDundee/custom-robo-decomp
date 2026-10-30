/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80068B50: .text 0x80068B50-0x80068B84 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void *fn_80068B50(void *arg0, void *arg1, u8 *arg2) {
    M2C_FIELD(arg2, u16 *, 0x10) = 0xFFF0;
    M2C_FIELD(arg2, u16 *, 0xC) = 0xFFFF;
    M2C_FIELD(arg2, s32 *, 0x1C) = (s32) M2C_FIELD(arg0, u8 *, 5);
    M2C_FIELD(arg1, u8 *, 0xC) += 1;
    return arg2 + 0x20;
}
