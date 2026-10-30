/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_MOVIE_FrameB: .text 0x80017100-0x80017128 (40 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F3A4;
extern u8 lbl_8031F020[392];

void cr_SEQ_MOVIE_FrameB(void) {
    if (lbl_8044F3A4 != 0) {
        lbl_8031F020[0xA5] = 1;
        lbl_8044F3A4 = 0;
    }
}
