/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001B1D4: .text 0x8001B1D4-0x8001B29C (200 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_804503D0;
extern const f64 lbl_804503D8;
f64 sqrt(f64 x);

void fn_8001B1D4(void *arg0, void *arg1) {
    f32 var_f31;

    var_f31 = M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg0, f32 *, 0);
    var_f31 += M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg0, f32 *, 4);
    var_f31 += M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg0, f32 *, 8);
    if (lbl_804503D0 != var_f31) {
        var_f31 = (f32) (lbl_804503D8 / sqrt((f64) var_f31));
        var_f31 = (f32) var_f31;
        M2C_FIELD(arg1, f32 *, 0) = (f32) (var_f31 * M2C_FIELD(arg0, f32 *, 0));
        M2C_FIELD(arg1, f32 *, 4) = (f32) (var_f31 * M2C_FIELD(arg0, f32 *, 4));
        M2C_FIELD(arg1, f32 *, 8) = (f32) (var_f31 * M2C_FIELD(arg0, f32 *, 8));
        return;
    }
    M2C_FIELD(arg1, f32 *, 0) = (f32) lbl_804503D0;
    M2C_FIELD(arg1, f32 *, 4) = (f32) lbl_804503D0;
    M2C_FIELD(arg1, f32 *, 8) = (f32) lbl_804503D0;
}
