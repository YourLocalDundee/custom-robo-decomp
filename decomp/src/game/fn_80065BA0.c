/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80065BA0: .text 0x80065BA0-0x80065C68 (200 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8032CC20[8368];
extern u8 lbl_8020AC50[28];

/* fn_80065BA0: Nov 13, 2026 hand draft: two-slot handler loop; record pointer tested against (void *) 0 */

void fn_80065BA0(void) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (*(void **) (lbl_8032CC20 + i * 0x28 + 8) != ((void *) 0) && (lbl_8032CC20 + i * 0x28)[0] != 0) {
            ((void (**)(u8 *)) lbl_8020AC50)[(lbl_8032CC20 + i * 0x28)[0]](lbl_8032CC20 + i * 0x28);
        }
    }
}
