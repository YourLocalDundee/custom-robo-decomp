/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80095E00: .text 0x80095E00-0x80095E6C (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void __assert(const char *, u32, const char *);
extern const char lbl_80451090[7];
extern const char lbl_80451097[5];

s32 fn_80095E00(void *arg0) {
    s32 var_r30_17;
    arg0 = arg0;

    (arg0 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
    var_r30_17 = 0;
    if (!(M2C_FIELD(arg0, s32 *, 0x14) & 0x800000) && (M2C_FIELD(arg0, s32 *, 0x14) & 0x40)) {
        var_r30_17 = 1;
    }
    return var_r30_17;
}
