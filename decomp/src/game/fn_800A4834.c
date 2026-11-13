/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A4834: .text 0x800A4834-0x800A4B98 (868 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_800A4834: Nov 6, 2026 hand draft (m2c 68%): the recursive AObj-flag test over a joint tree, auto-inlined by the file's settings (auto_inline, inline_max_size(1000), push / pop) */

#pragma push
#pragma auto_inline on
#pragma inline_max_size(1000)
u32 fn_800A4834(void *jobj) {
    while (jobj != NULL) {
        if ((M2C_FIELD(jobj, s32 **, 0x7C) != NULL) && (*M2C_FIELD(jobj, s32 **, 0x7C) & 0x40000000)) {
            return 1U;
        }
        if ((M2C_FIELD(jobj, void **, 0x10) != NULL) && (fn_800A4834(M2C_FIELD(jobj, void **, 0x10)) != 0)) {
            return 1U;
        }
        jobj = M2C_FIELD(jobj, void **, 8);
    }
    return 0U;
}
#pragma pop
