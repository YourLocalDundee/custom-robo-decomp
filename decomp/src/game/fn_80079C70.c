/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80079C70: .text 0x80079C70-0x80079CB8 (72 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F608[2];
extern const f32 lbl_80450CB0;

void fn_80079C70(void) {
    if (M2C_FIELD(*lbl_8044F608, u32 *, 4) == 0U) {
        M2C_FIELD(*lbl_8044F608, f32 *, 0x28) = (f32) lbl_80450CB0;
        M2C_FIELD(*lbl_8044F608, f32 *, 0x2C) = (f32) lbl_80450CB0;
        M2C_FIELD(*lbl_8044F608, f32 *, 0x30) = (f32) lbl_80450CB0;
        M2C_FIELD(*lbl_8044F608, u32 *, 4) += 1;
    }
}
