/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001D254: .text 0x8001D254-0x8001D350 (252 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8001D254(void *arg0, void *arg1, void *arg2) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;

    var_f31 = M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg1, f32 *, 0);
    var_f30 = (M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg1, f32 *, 4)) + var_f31;
    var_f29 = M2C_FIELD(arg0, f32 *, 0x10) * M2C_FIELD(arg1, f32 *, 0);
    var_f27 = M2C_FIELD(arg0, f32 *, 0x20) * M2C_FIELD(arg1, f32 *, 0);
    var_f28 = (M2C_FIELD(arg0, f32 *, 0x14) * M2C_FIELD(arg1, f32 *, 4)) + var_f29;
    var_f26 = (M2C_FIELD(arg0, f32 *, 0x24) * M2C_FIELD(arg1, f32 *, 4)) + var_f27;
    M2C_FIELD(arg2, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0xC) + ((M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg1, f32 *, 8)) + var_f30));
    M2C_FIELD(arg2, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 0x1C) + ((M2C_FIELD(arg0, f32 *, 0x18) * M2C_FIELD(arg1, f32 *, 8)) + var_f28));
    M2C_FIELD(arg2, f32 *, 8) = (f32) (M2C_FIELD(arg0, f32 *, 0x2C) + ((M2C_FIELD(arg0, f32 *, 0x28) * M2C_FIELD(arg1, f32 *, 8)) + var_f26));
}
