/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80112B88: .text 0x80112B88-0x80112C24 (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041A244[852];

void fn_80112B88(void *arg0) {
    void *temp_r31_13;

    if (M2C_FIELD(lbl_8041A244, s32 *, 0x10) & 8) {
        temp_r31_13 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(arg0, s32 *, 0x10) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x10) > 2) {
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(temp_r31_13, u16 *, 0x10) += 1;
            if ((s32) (M2C_FIELD(temp_r31_13, u16 *, 0x10) - 0x2C) >= 3) {
                M2C_FIELD(temp_r31_13, u16 *, 0x10) = 0x2CU;
            }
        }
    } else {
        M2C_FIELD(arg0, s32 *, 0x10) = 0;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
}
