/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80007BD4: .text 0x80007BD4-0x80007C24 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

extern u8 *lbl_8044F348;

void fn_80007BD4(s32 arg0) {
    if (arg0 == 0) {
        M2C_FIELD(lbl_8044F348, s32 *, 0x950) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x91C);
        M2C_FIELD(lbl_8044F348, s32 *, 0x94C) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x920);
        return;
    }
    M2C_FIELD(lbl_8044F348, s32 *, 0x950) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x924);
    M2C_FIELD(lbl_8044F348, s32 *, 0x94C) = (s32) M2C_FIELD(lbl_8044F348, s32 *, 0x928);
}
