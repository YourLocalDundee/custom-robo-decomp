/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80066EEC: .text 0x80066EEC-0x80066F58 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_80066EEC: Nov 13, 2026 hand draft: the offset table read as ((s32 *) (file + 0x10))[id]; the text base added first */

s32 fn_80066EEC(u8 *arg0, u16 arg1) {
    u16 var_r31;
    s32 var_r30;

    (void) var_r31; 
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0), u16 *, 8) <= arg1) {
        return 0;
    }
    var_r31 = (u16) ((s32 *) (M2C_FIELD(arg0, u8 **, 0) + 0x10))[arg1];
    if (var_r31 != 0) {
        var_r30 = var_r31 + 8;
        var_r30 = M2C_FIELD(arg0, s32 *, 4) + var_r30;
        return var_r30;
    }
    return 0;
}
