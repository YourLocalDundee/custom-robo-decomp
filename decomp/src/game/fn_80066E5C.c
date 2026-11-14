/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80066E5C: .text 0x80066E5C-0x80066EEC (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern u32 cr_g_MessageFiles[87];

/* fn_80066E5C: Nov 13, 2026 hand draft: the offset table read as ((s32 *) (file + 0x10))[id]; the text address one flat sum, base + off + 8, left to right; rank_solve.py order */

s32 fn_80066E5C(u16 arg0) {
    s32 var_r31;
    u16 var_r30;
    s32 var_r29;

    (void) var_r30; 
    if (M2C_FIELD(cr_g_MessageFiles[3], u16 *, 8) <= arg0) {
        var_r31 = 0;
    } else {
        var_r30 = (u16) ((s32 *) (cr_g_MessageFiles[3] + 0x10))[arg0];
        if (var_r30 != 0) {
            var_r29 = (s32) cr_g_MessageFiles[4] + var_r30 + 8;
            var_r31 = var_r29;
        } else {
            var_r31 = 0;
        }
    }
    return var_r31;
}
