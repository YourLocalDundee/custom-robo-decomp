/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A0204: .text 0x800A0204-0x800A0228 (36 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

M2C_UNK cr_MotionFile_Register(s32);                /* extern */
extern s32 lbl_8044F33C;

void fn_800A0204(void) {
    cr_MotionFile_Register(lbl_8044F33C);
}
