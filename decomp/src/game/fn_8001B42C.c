/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001B42C: .text 0x8001B42C-0x8001B544 (280 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_804503D0;
extern f32 lbl_804503FC;
extern f32 lbl_804503F8;
extern u8 lbl_801E90B0[32784];
extern f32 lbl_80450400;
extern f32 lbl_80450404;

f32 fn_8001B42C(f32 farg0) {
    f32 var_f30;
    s32 var_r30;
    s32 var_r31;

    var_r31 = 0;
    if (farg0 <= lbl_804503D0) {
        return lbl_804503D0;
    }
    while (farg0 > lbl_804503FC) {
        var_r31 += 1;
        farg0 /= lbl_804503F8;
    }
    while ((lbl_804503F8 * farg0) < lbl_804503FC) {
        var_r31 -= 1;
        farg0 *= lbl_804503F8;
    }
    var_f30 = M2C_FIELD((lbl_801E90B0 + ((u32) (lbl_80450400 + farg0) * 4)), f32 *, 0);
    if (var_r31 > 0) {
        var_r30 = 0;
        while (var_r30 < var_r31) {
            var_r30 += 1;
            var_f30 *= lbl_80450404;
        }
    } else if (var_r31 < 0) {
        var_r30 = 0;
        while (var_r30 > var_r31) {
            var_r30 -= 1;
            var_f30 /= lbl_80450404;
        }
    }
    return var_f30;
}
