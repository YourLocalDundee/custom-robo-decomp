/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800668DC: .text 0x800668DC-0x80066904 (40 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_803BF914[434];
extern f32 lbl_8044EA40;
extern u8 lbl_8020B0C0[2876];

void fn_800668DC(u8 arg0) {
    M2C_FIELD(lbl_803BF914, u8 *, 0x330) = arg0;
    lbl_8044EA40 = M2C_FIELD(lbl_8020B0C0, f32 *, (arg0 * 4) & 0x3FC);
}
