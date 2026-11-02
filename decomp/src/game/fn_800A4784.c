/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A4784: .text 0x800A4784-0x800A4834 (176 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
u32 fn_800A4834(void *arg0);

s32 fn_800A4784(void *arg0) {

    while (arg0 != NULL) {
        if (((void *) M2C_FIELD(arg0, void **, 0x18) != NULL) && ((void *) M2C_FIELD(M2C_FIELD(arg0, void **, 0x18), void **, 8) != NULL) && (M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x18), void **, 8), u32 *, 0x14) != 0U) && (*M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x18), void **, 8), s32 **, 0x14) & 0x40000000)) {
            return 1;
        }
        if (((void *) M2C_FIELD(arg0, void **, 0x10) != NULL) && (fn_800A4834(M2C_FIELD(arg0, void **, 0x10)) != 0)) {
            return 1;
        }
        arg0 = M2C_FIELD(arg0, void **, 8);
    }
    return 0;
}
