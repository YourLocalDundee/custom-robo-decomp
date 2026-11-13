/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Text_CountChars: .text 0x80068A74-0x80068B50 (220 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* cr_Text_CountChars: Nov 13, 2026 hand draft: counts display characters (two-byte >= 0x81, 0x1A control records); top-tested loop with break, returns the count */

s32 cr_Text_CountChars(u8 *s) {
    s32 i;
    u8 *p;
    s32 n;
    u32 j;
    u8 *q;

    i = 0;
    n = 0;
    j = 0;
    for (;;) {
        if (s[i] == 0) {
            break;
        }
        if ((s[i] >> 5) == 0) {
            if (s[i] == 0xA) {
                i++;
            } else if (s[i] == 0x1A) {
                p = s + i;
                if (p[2] == 0xFF && p[4] == 2) {
                    for (j = 1; j < p[1] - 5;) {
                        q = p + j;
                        if (q[5] >= 0x81) {
                            n++;
                            j += 2;
                        } else {
                            n++;
                            j++;
                        }
                    }
                }
                i += p[1];
            }
        } else {
            if (s[i] >= 0x81) {
                i++;
            }
            n++;
            i++;
        }
    }
    return n;
}
