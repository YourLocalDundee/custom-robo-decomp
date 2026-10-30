/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80049904: .text 0x80049904-0x80049924 (32 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F4C8;
extern u32 lbl_8044F4D8;

extern u8 *lbl_8044F348;

s32 fn_80049904(void) {
    lbl_8044F4C8 = M2C_FIELD(lbl_8044F348, u32 *, 0x1D8);
    lbl_8044F4D8 = M2C_FIELD(lbl_8044F348, u32 *, 0x4CC);
    return 1;
}
