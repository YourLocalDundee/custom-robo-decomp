/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005D024: .text 0x8005D024-0x8005D068 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

f32 fn_8005D024(void *arg0) {
    f32 var_f31;

    var_f31 = M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg0, f32 *, 0);
    var_f31 += M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg0, f32 *, 4);
    var_f31 += M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg0, f32 *, 8);
    return var_f31;
}
