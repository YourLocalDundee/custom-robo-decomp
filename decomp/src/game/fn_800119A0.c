/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800119A0: .text 0x800119A0-0x80011AC0 (288 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8031BF54[840];

/* fn_800119A0: Nov 3, 2026 hand draft (m2c 86%): the two goto loops written as for loops (the original tests at the bottom) */

u8 fn_800119A0(void) {
    u8 *p;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        if (i == 0) {
            p = lbl_8031BF54;
        } else if (i == 1) {
            p = &lbl_8031BF54[0x78];
        } else if (i == 2) {
            p = &lbl_8031BF54[0xF0];
        } else if (i == 3) {
            p = &lbl_8031BF54[0x168];
        } else if (i == 4) {
            p = &lbl_8031BF54[0x1E0];
        } else if (i == 5) {
            p = &lbl_8031BF54[0x258];
        } else if (i == 6) {
            p = &lbl_8031BF54[0x2D0];
        }
        for (j = 0; j < 10; j++) {
            if (p[4] == 2 || p[4] == 1 || p[4] == 3) {
                return 1;
            }
            p += 0xC;
        }
    }
    return 0;
}
