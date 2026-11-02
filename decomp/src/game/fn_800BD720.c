/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BD720: .text 0x800BD720-0x800BD7F0 (208 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_800BD720: hand-written from main.dol's asm on its m2c draft (step 109b): one alpha variable for both fades, loaded and then stepped in place (lbz r31; addi r31, r31, 0x20) */
extern u8 *lbl_8044F710;

void fn_800BD720(void) {
    s32 v;

    if (M2C_FIELD(lbl_8044F710, s32 *, 0) & 1) {
        switch ((M2C_FIELD(lbl_8044F710, s32 *, 0x10))) {
        case 0:
            v = M2C_FIELD(lbl_8044F710, u8 *, 0x57);
            v += 0x20;
            if (v >= 0x100) {
                v = 0xFF;
                M2C_FIELD(lbl_8044F710, s32 *, 0) &= 0xFFFFFFFE;
            }
            M2C_FIELD(lbl_8044F710, u8 *, 0x57) = (u8) v;
            return;
        case 1:
            M2C_FIELD(lbl_8044F710, s32 *, 0x10) = 2;
            if (M2C_FIELD(lbl_8044F710, u8 *, 0x57) > 0x80U) {
                M2C_FIELD(lbl_8044F710, u8 *, 0x57) = 0xFFU;
                return;
            }
            break;
        case 2:
            v = M2C_FIELD(lbl_8044F710, u8 *, 0x57);
            v -= 0x20;
            if (v <= 0) {
                v = 0;
            }
            M2C_FIELD(lbl_8044F710, u8 *, 0x57) = (u8) v;
            break;
        }
    }
}
