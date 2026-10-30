/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078438: .text 0x80078438-0x80078478 (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

M2C_UNK fn_800208F0(u32);                           /* extern */
M2C_UNK fn_800215F8(u32);                           /* extern */
extern u32 lbl_8044F600;

void fn_80078438(void) {
    if ((u32) lbl_8044F600 != 0U) {
        fn_800208F0(lbl_8044F600);
        fn_800215F8(lbl_8044F600);
        lbl_8044F600 = 0U;
    }
}
