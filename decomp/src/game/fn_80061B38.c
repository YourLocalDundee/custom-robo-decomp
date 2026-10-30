/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80061B38: .text 0x80061B38-0x80061B90 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023394(s8 *arg0, u32 arg1);
extern f32 lbl_8032C234[21];
s8 fn_80061620(f32 *arg0, void *arg1, void *arg2, u8 arg3, f32 farg0);

void fn_80061B38(s32 arg0, s32 arg1, s32 arg2, f32 farg0) {

    fn_80023394((s8 *) lbl_8032C234, 0x54U);
    fn_80061620((f32 *) arg0, (void *) arg1, (void *) arg2, 1U, farg0);
}
