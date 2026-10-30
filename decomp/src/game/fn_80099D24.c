/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80099D24: .text 0x80099D24-0x80099D50 (44 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023394(s8 *arg0, u32 arg1);
extern f32 lbl_803BE308[10];

void fn_80099D24(void) {
    fn_80023394((s8 *) lbl_803BE308, 0x28U);
}
