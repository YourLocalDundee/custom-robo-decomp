/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80024578: .text 0x80024578-0x80024590 (24 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_8044F5C8;
extern u32 lbl_80321D40[18];

s32 fn_80024578(void) {
    lbl_8044F5C8 = (u16) M2C_FIELD(lbl_80321D40, u8 *, 0x44);
    return 9;
}
