/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800690F4: .text 0x800690F4-0x80069108 (20 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_800690F4: Nov 13, 2026 hand draft (m2c 40%): a callback with three parameters; m2c saw only two (the first is unused) and lost the return of the third (`mr r3, r5`, M2C_ERROR "Read from unset register $r5") */

s32 fn_800690F4(void *arg0, u8 *arg1, s32 arg2) {
    *arg1 |= 2;
    return arg2;
}
