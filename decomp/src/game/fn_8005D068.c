/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005D068: .text 0x8005D068-0x8005D08C (36 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_8005D068: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the byte taken as `(x & 0xFF0000) >> 16` (rlwinm then srwi) */
u8 fn_8005D068(void *arg0) {
    u8 var_r31;

    var_r31 = (M2C_FIELD(arg0, u32 *, 0x10) & 0xFF0000) >> 16;
    return var_r31;
}
