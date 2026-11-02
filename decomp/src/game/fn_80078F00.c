/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078F00: .text 0x80078F00-0x80078F94 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8017EE20(void *arg0);

void fn_80078F00(s8 *arg0, u32 arg1) {
    u32 var_r30_24;
    s8 *var_r29_23;

    if (((u32) arg0 != NULL) && (M2C_FIELD(arg0, u32 *, 0) != 0U) && ((u32) M2C_FIELD(arg0, u8 **, 0x90) == arg1) && (M2C_FIELD(arg0, u32 *, 0) != 0U)) {
        fn_8017EE20(M2C_FIELD(arg0, void **, 0x90));
        for (var_r29_23 = arg0, var_r30_24 = 0U; var_r30_24 < 0xBCU; var_r30_24 += 1) {
            *var_r29_23++ = 0;
        }
    }
}
