/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80044374: .text 0x80044374-0x800443A0 (44 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450654;

void fn_80044374(void *arg0, f32 farg0, f32 farg1) {
    f32 var_f31;

    var_f31 = lbl_80450654;
    M2C_FIELD(arg0, f32 *, 8) = farg0;
    M2C_FIELD(arg0, f32 *, 0xC) = (f32) (farg1 * var_f31);
}
