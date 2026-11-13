/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Text_GetPartDescription: .text 0x80066DCC-0x80066E5C (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern u32 cr_g_MessageFiles[87];

/* cr_Text_GetPartDescription: Nov 13, 2026 hand draft: message lookup, file 0: ((u32 *) (file + 0x10))[id], text + off + 8 */

s32 cr_Text_GetPartDescription(u16 arg0) {
    s32 var_r31;
    u16 var_r30;
    s32 var_r29;

    if (M2C_FIELD(M2C_FIELD(cr_g_MessageFiles, u32 *, 0), u16 *, 8) <= arg0) {
        var_r31 = 0;
    } else {
        var_r30 = (u16) ((u32 *) (cr_g_MessageFiles[0] + 0x10))[arg0];
        if (var_r30 != 0) {
            var_r29 = cr_g_MessageFiles[1] + var_r30 + 8;
            var_r31 = var_r29;
        } else {
            var_r31 = 0;
        }
    }
    return var_r31;
}
