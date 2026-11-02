/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008E9FC: .text 0x8008E9FC-0x8008EA64 (104 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void *fn_8008E9FC(void *arg0) {
    u8 *var_r31_11;
    s16 var_r30_5;

    var_r30_5 = 0;
    var_r31_11 = (void *) (*(u32 *) &((u8 (*)[0x8]) (M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), s32 *, 0x1C)))[M2C_FIELD(arg0, u8 *, 0x5A)][0x4]);
    while (!(var_r30_5 >= 0x200)) {
        if (M2C_FIELD(var_r31_11, u16 *, 0x10) == 0xFFFF) {
            return var_r31_11;
        }
        var_r30_5 += 1;
        var_r31_11 += 0x20;
    }
    return NULL;
}
