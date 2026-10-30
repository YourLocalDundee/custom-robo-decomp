/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80007C24: .text 0x80007C24-0x80007C74 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern void *lbl_8044F348;

void fn_80007C24(s32 arg0) {
    if (arg0 == 0) {
        M2C_FIELD(lbl_8044F348, s32 *, 0x948) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x914);
        M2C_FIELD(lbl_8044F348, s32 *, 0x8F8) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8C8);
        return;
    }
    M2C_FIELD(lbl_8044F348, s32 *, 0x948) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x918);
    M2C_FIELD(lbl_8044F348, s32 *, 0x8F8) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8CC);
}
