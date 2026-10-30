/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001D1AC: .text 0x8001D1AC-0x8001D254 (168 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
f64 sqrt(f64 x);
extern f32 lbl_80450408;

f32 fn_8001D1AC(void *arg0) {
    f32 var_f30;
    f32 var_f31;

    var_f31 = (M2C_FIELD(arg0, f32 *, 0x20) * M2C_FIELD(arg0, f32 *, 0x20)) + ((M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg0, f32 *, 0)) + (M2C_FIELD(arg0, f32 *, 0x10) * M2C_FIELD(arg0, f32 *, 0x10)));
    var_f31 = (f32) sqrt((f64) var_f31);
    var_f30 = lbl_80450408 / var_f31;
    M2C_FIELD(arg0, f32 *, 0) *= var_f30;
    M2C_FIELD(arg0, f32 *, 0x10) *= var_f30;
    M2C_FIELD(arg0, f32 *, 0x20) *= var_f30;
    return var_f31;
}
