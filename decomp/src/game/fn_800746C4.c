/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800746C4: .text 0x800746C4-0x800747B0 (236 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void *fn_800746C4(void *arg0, u8 **arg1) {
    void *var_r31_16;
    void *var_r30_28;
    void *var_r29_46;

    if ((u8) **arg1 != 0xFF) {
loop_1:
        if ((u8) **arg1 == 0xFF) {
            var_r31_16 = arg0;
            *arg1 += 1;
        } else {
            if ((u8) **arg1 == 2) {
                if (arg0 == ((void *) 0)) {
                    var_r30_28 = NULL;
                } else {
                    var_r30_28 = M2C_FIELD(arg0, void **, 0x10);
                }
                arg0 = var_r30_28;
                *arg1 += 1;
                goto loop_1;
            }
            if ((u8) **arg1 == 1) {
                if (arg0 == ((void *) 0)) {
                    var_r29_46 = NULL;
                } else {
                    var_r29_46 = M2C_FIELD(arg0, void **, 8);
                }
                arg0 = var_r29_46;
                *arg1 += 1;
                goto loop_1;
            }
            var_r31_16 = NULL;
        }
    } else {
        var_r31_16 = NULL;
        *arg1 += 1;
    }
    return var_r31_16;
}
