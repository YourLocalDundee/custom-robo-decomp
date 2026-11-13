/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Text_GetBombName: .text 0x80066B64-0x80066C0C (168 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s32 cr_g_BodyCount;
__declspec(section ".sdata") extern s32 cr_g_GunCount;
__declspec(section ".data") extern u32 cr_g_MessageFiles[87];

/* cr_Text_GetBombName: Nov 13, 2026 hand draft: message lookup; the part index arg0 + the earlier part counts, left to right */

s32 cr_Text_GetBombName(s32 arg0) {
    s32 var_r31;
    u16 var_r30;
    u16 var_r29;
    s32 var_r28;

    var_r30 = arg0 + cr_g_BodyCount + cr_g_GunCount;
    if (M2C_FIELD(cr_g_MessageFiles[0x1B], u16 *, 8) <= var_r30) {
        var_r31 = 0;
    } else {
        var_r29 = (u16) ((u32 *) (cr_g_MessageFiles[0x1B] + 0x10))[var_r30];
        if (var_r29 != 0) {
            var_r28 = cr_g_MessageFiles[0x1C] + var_r29 + 8;
            var_r31 = var_r28;
        } else {
            var_r31 = 0;
        }
    }
    return var_r31;
}
