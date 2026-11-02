/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008C5F0: .text 0x8008C5F0-0x8008C640 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void fn_8008C5F0(void *arg0, void *arg1, s32 arg2, u32 arg3) {
    u32 var_r31_4;

    var_r31_4 = 0;
    while (var_r31_4 < arg3) {
        *(u16 *) &((u8 (*)[0x20]) (arg2))[var_r31_4][0x10] = 0xFFFF;
        var_r31_4 += 1;
    }
    M2C_FIELD(arg1, s32 *, 4) = arg2;
    M2C_FIELD(arg1, u16 *, 8) = 0xFFFE;
    M2C_FIELD(arg0, void **, 0x1C) = arg1;
}
