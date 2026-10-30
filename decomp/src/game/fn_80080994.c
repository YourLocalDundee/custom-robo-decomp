/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80080994: .text 0x80080994-0x80080A94 (256 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_8044F63A;
extern u8 lbl_8044F63C;

void fn_80080994(void *arg0) {
    u32 var_r31_4;

    var_r31_4 = 0x14;
    if (!(lbl_8044F63A & 0x100)) {
        M2C_FIELD(arg0, u8 *, 0x54) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x55) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x56) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x57) = 0xFFU;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    if (lbl_8044F63A & 0x80) {
        var_r31_4 = 0x37;
    }
    if (lbl_8044F63C == 0) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) - 5) > 0xC8) {
            M2C_FIELD(arg0, u8 *, 0x57) -= 5;
            return;
        }
        M2C_FIELD(arg0, u8 *, 0x57) = 0xC8U;
        lbl_8044F63C += 1;
        return;
    }
    if ((u32) (M2C_FIELD(arg0, u8 *, 0x57) + (0x37U / var_r31_4)) < 0xFFU) {
        M2C_FIELD(arg0, u8 *, 0x57) += (0x37U / var_r31_4);
        return;
    }
    M2C_FIELD(arg0, u8 *, 0x57) = 0xFFU;
    lbl_8044F63C = 0;
    lbl_8044F63A &= 0xFFFFFEFF;
}
