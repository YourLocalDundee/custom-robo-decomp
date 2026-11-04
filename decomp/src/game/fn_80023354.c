/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80023354: .text 0x80023354-0x80023394 (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_80023354: Nov 3, 2026 hand draft (m2c 62-68%): the clear loop as `for (n = ..., p = arg; ...; n--) *p++ = 0;` (the counter takes r31, the pointer r30) */

void fn_80023354(s32 *arg0, u32 arg1) {
    u32 n;
    s32 *p;
    for (n = arg1 >> 2, p = arg0; n != 0; n--) {
        *p++ = 0;
    }
}
