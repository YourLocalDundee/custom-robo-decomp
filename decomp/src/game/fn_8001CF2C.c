/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001CF2C: .text 0x8001CF2C-0x8001CFA4 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* used from the HSD / SDK headers and symbols/cr_types.h (generated from the drafts' context) */
f64 cr_Sqrt(f64 x);

extern f32 lbl_804503D0;

f32 fn_8001CF2C(void *arg0) {
    f32 temp_f31_17;

    temp_f31_17 = (M2C_FIELD(arg0, f32 *, 0x28) * M2C_FIELD(arg0, f32 *, 0x28)) + ((M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg0, f32 *, 8)) + (M2C_FIELD(arg0, f32 *, 0x18) * M2C_FIELD(arg0, f32 *, 0x18)));
    if (temp_f31_17 != lbl_804503D0) {
        return (f32) cr_Sqrt((f64) temp_f31_17);
    }
    return lbl_804503D0;
}
