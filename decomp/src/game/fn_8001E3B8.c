/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001E3B8: .text 0x8001E3B8-0x8001E424 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8001E3B8(void *arg0, void *arg1, void *arg2, f32 farg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;

    sp.sp8 = M2C_FIELD(arg1, f32 *, 0) - M2C_FIELD(arg2, f32 *, 0);
    sp.spC = M2C_FIELD(arg1, f32 *, 4) - M2C_FIELD(arg2, f32 *, 4);
    sp.sp10 = M2C_FIELD(arg1, f32 *, 8) - M2C_FIELD(arg2, f32 *, 8);
    M2C_FIELD(arg0, f32 *, 0) = (f32) ((sp.sp8 * farg0) + M2C_FIELD(arg2, f32 *, 0));
    M2C_FIELD(arg0, f32 *, 4) = (f32) ((sp.spC * farg0) + M2C_FIELD(arg2, f32 *, 4));
    M2C_FIELD(arg0, f32 *, 8) = (f32) ((sp.sp10 * farg0) + M2C_FIELD(arg2, f32 *, 8));
}
