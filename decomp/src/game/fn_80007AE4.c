/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80007AE4: .text 0x80007AE4-0x80007BD4 (240 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern u8 *lbl_8044F348;

void fn_80007AE4(s32 arg0) {
    if (arg0 == 0) {
        M2C_FIELD(lbl_8044F348, s32 *, 0x954) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x92C);
        M2C_FIELD(lbl_8044F348, s32 *, 0x95C) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x934);
        M2C_FIELD(lbl_8044F348, s32 *, 0x8FC) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8D0);
        M2C_FIELD(lbl_8044F348, s32 *, 0x900) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8D4);
        M2C_FIELD(lbl_8044F348, s32 *, 0x904) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8D8);
        M2C_FIELD(lbl_8044F348, s32 *, 0x908) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8DC);
    } else {
        M2C_FIELD(lbl_8044F348, s32 *, 0x954) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x93C);
        M2C_FIELD(lbl_8044F348, s32 *, 0x95C) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x940);
        M2C_FIELD(lbl_8044F348, s32 *, 0x8FC) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8E0);
        M2C_FIELD(lbl_8044F348, s32 *, 0x900) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8E4);
        M2C_FIELD(lbl_8044F348, s32 *, 0x904) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8E8);
        M2C_FIELD(lbl_8044F348, s32 *, 0x908) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x8EC);
    }
    M2C_FIELD(lbl_8044F348, s32 *, 0x958) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x930);
    M2C_FIELD(lbl_8044F348, s32 *, 0x960) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x938);
}
