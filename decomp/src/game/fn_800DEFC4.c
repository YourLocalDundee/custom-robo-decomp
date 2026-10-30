/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800DEFC4: .text 0x800DEFC4-0x800DEFF8 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80321D40[18];

void fn_800DEFC4(void *arg0) {
    if ((s32) lbl_80321D40[4] == 2) {
        M2C_FIELD(arg0, s32 *, 0xC) |= 0x80;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0xC) |= 0x100;
}
