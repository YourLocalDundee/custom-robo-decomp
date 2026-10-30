/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A0804: .text 0x800A0804-0x800A0854 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

M2C_UNK fn_800F21C8(s32, s32);                      /* extern */
M2C_UNK fn_800F5448(s32);                           /* extern */
extern s32 *lbl_8044F794;

void fn_800A0804(s32 arg0) {
    fn_800F5448(arg0 >> 1);
    fn_800F21C8(arg0 >> 1, arg0 & 1);
    *lbl_8044F794 = *lbl_8044F794 & 0xFFFFFBFF;
}
