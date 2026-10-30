/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A5844: .text 0x800A5844-0x800A5890 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F748;
extern f32 lbl_80451280;
extern f32 lbl_80451284;

void fn_800A5844(void) {
    if (M2C_FIELD(lbl_8044F748, f32 *, 0x170) > lbl_80451280) {
        M2C_FIELD(lbl_8044F748, f32 *, 0x170) -= lbl_80451284;
        return;
    }
    M2C_FIELD(lbl_8044F748, s32 *, 4) += 1;
    M2C_FIELD(lbl_8044F748, s16 *, 0x16C) = 0;
}
