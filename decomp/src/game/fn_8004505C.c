/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8004505C: .text 0x8004505C-0x800450DC (128 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8024E100[8];

s32 fn_8004505C(f32 farg0, f32 farg1) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;

    var_f31 = farg0 - M2C_FIELD(lbl_8024E100, f32 *, 0);
    var_f30 = farg1 - lbl_8024E100[1];
    var_f29 = (var_f31 * var_f31) + (var_f30 * var_f30);
    if (var_f29 > lbl_8024E100[2]) {
        return 1;
    }
    return 0;
}
