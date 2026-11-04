/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800232B4: .text 0x800232B4-0x80023304 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_800232B4: Nov 3, 2026 hand draft (m2c 70%): the copy loop as `for (n = ..., d = dst, s = src; n > 0; n--) *d++ = *s++;` (counter r31, dst r30, src r29) */

void fn_800232B4(u16 *arg0, u16 *arg1, u32 arg2) {
    s32 n;
    u16 *d;
    u16 *s;
    for (n = arg2 >> 1, d = arg0, s = arg1; n > 0; n--) {
        *d++ = *s++;
    }
}
