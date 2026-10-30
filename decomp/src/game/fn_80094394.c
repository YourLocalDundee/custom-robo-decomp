/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80094394: .text 0x80094394-0x800943D8 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void __assert(const char *, u32, const char *);
extern char lbl_804510A0[7];
extern char lbl_804510A7[5];

f32 fn_80094394(void *arg0) {
    (arg0 != NULL) ? (void) 0 : __assert(lbl_804510A0, 0x95U, lbl_804510A7);
    return M2C_FIELD(arg0, f32 *, 4);
}
