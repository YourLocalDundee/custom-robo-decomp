/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80007C74: .text 0x80007C74-0x80007CE4 (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern void *lbl_8044F348;

void fn_80007C74(s32 arg0) {
    if (arg0 == 0) {
        M2C_FIELD(lbl_8044F348, s32 *, 0x944) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x90C);
        M2C_FIELD(lbl_8044F348, s32 *, 0x8F0) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8B8);
        M2C_FIELD(lbl_8044F348, s32 *, 0x8F4) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8BC);
        return;
    }
    M2C_FIELD(lbl_8044F348, s32 *, 0x944) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x910);
    M2C_FIELD(lbl_8044F348, s32 *, 0x8F0) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8C0);
    M2C_FIELD(lbl_8044F348, s32 *, 0x8F4) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8C4);
}
