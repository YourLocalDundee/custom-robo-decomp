/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010E3A4: .text 0x8010E3A4-0x8010E478 (212 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_8010DF48(void *arg0);
extern u8 lbl_80407260[75344];
extern u8 lbl_8023BA70[20];

/* fn_8010E3A4: Nov 13, 2026 hand draft: per-record handler table, count homed on the stack (volatile), handler takes (record, u8 slot) */

void fn_8010E3A4(volatile u32 n) {
    u8 i;
    u8 j;

    for (i = 0; i < n; i++) {
        fn_8010DF48(lbl_80407260 + i * 0x4994);
        for (j = 0; j < 5; j++) {
            ((void (**)(u8 *, u8)) lbl_8023BA70)[(lbl_80407260 + i * 0x4994 + j * 0xC)[0x47F0]](lbl_80407260 + i * 0x4994, j);
        }
    }
}
