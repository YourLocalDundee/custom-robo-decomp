/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80066904: .text 0x80066904-0x80066948 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

s32 fn_80066904(u8 *arg0, void *arg1, u8 *arg2) {
    u8 *var_r31;

    var_r31 = arg0 + 5;
    M2C_FIELD(arg2, u16 *, 0x10) = 0xFFF0;
    M2C_FIELD(arg2, u16 *, 0xC) = M2C_FIELD(var_r31, u16 *, 0);
    M2C_FIELD(arg2, u16 *, 0xE) = M2C_FIELD(var_r31, u16 *, 2);
    M2C_FIELD(arg1, u8 *, 0xC) += 1;
    return (s32) (arg2 + 0x20);
}
