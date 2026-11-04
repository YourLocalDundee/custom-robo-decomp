/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800671C4: .text 0x800671C4-0x80067250 (140 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_800671C4: Nov 3, 2026 hand draft (m2c 77%): the copy loop tests the terminator at the top (`for (;;)` with the end first); a 0x1A control code copies itself and its p[1] argument bytes */

void fn_800671C4(s32 arg0, s32 arg1) {
    s32 i;
    u32 j;
    u8 *p;

    i = 0;
    for (;;) {
        if (*(s8 *) (arg1 + i) == 0) {
            *(s8 *) (arg0 + i) = 0;
            break;
        }
        if (*(s8 *) (arg1 + i) == 0x1A) {
            p = (u8 *) (arg1 + i);
            for (j = 0; j <= p[1]; j++) {
                *(u8 *) (arg0 + i) = *(u8 *) (arg1 + i);
                i += 1;
            }
        } else {
            *(u8 *) (arg0 + i) = *(u8 *) (arg1 + i);
            i += 1;
        }
    }
}
