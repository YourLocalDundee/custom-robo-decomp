/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80096544: .text 0x80096544-0x80096594 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80173850(s32 arg0);
extern u8 lbl_8044F6BC;

void fn_80096544(u32 arg0) {

    if (arg0 == 1) {
        fn_80173850(1);
        lbl_8044F6BC = 1;
        return;
    }
    fn_80173850(0);
    lbl_8044F6BC = 0;
}
