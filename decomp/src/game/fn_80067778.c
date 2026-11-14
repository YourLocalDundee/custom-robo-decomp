/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80067778: .text 0x80067778-0x800677C4 (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F6A0;
extern u32 lbl_8044F69C;

/* fn_80067778: Nov 13, 2026 hand draft: the offset table read as ((s32 *) (file + 0x10))[id] (add base, index; lwz 0x10), the text base added first (text = base + off) */

s32 fn_80067778(s32 arg0) {
    u32 var_r31;
    s32 var_r30;

    (void) var_r31; 
    var_r31 = ((s32 *) (lbl_8044F6A0 + 0x10))[arg0] & 0xFFFF;
    if (var_r31 != 0) {
        var_r30 = var_r31 + 8;
        var_r30 = lbl_8044F69C + var_r30;
        return var_r30;
    }
    return 0;
}
