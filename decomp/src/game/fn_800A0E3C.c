/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A0E3C: .text 0x800A0E3C-0x800A0E64 (40 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

M2C_UNK fn_80073A74(s32);                           /* extern */
extern void *lbl_8044F40C;

void fn_800A0E3C(void) {
    fn_80073A74(M2C_FIELD(lbl_8044F40C, s32 *, 8));
}
