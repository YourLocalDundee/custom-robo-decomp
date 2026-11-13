/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008F3CC: .text 0x8008F3CC-0x8008F4F4 (296 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_8008F3CC: Nov 13, 2026 hand draft: animation step: an s32 * flags alias of the object, 8-byte steps, if-chain on 0xFFFF / 0xFFFE / 0xFFFD */

typedef struct cr_AnimStep {
    u16 frames;
    u8 pad[6];
} cr_AnimStep;

void fn_8008F3CC(void *arg0) {
    s32 *var_r31;
    cr_AnimStep *var_r30;

    var_r31 = (s32 *) arg0;
    if ((M2C_FIELD(arg0, s32 *, 0) & 1) && !(*var_r31 & 0x40)) {
        if (M2C_FIELD(arg0, u32 *, 8) == 0U) {
            if (!(*var_r31 & 0x1000)) {
                M2C_FIELD(arg0, u8 *, 0x5A) += 1;
            } else {
                M2C_FIELD(arg0, u8 *, 0x5A) -= 1;
                if (M2C_FIELD(arg0, u8 *, 0x5A) == 0) {
                    *var_r31 &= 0xFFFFEFFF;
                }
            }
            var_r30 = &M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), cr_AnimStep **, 0x1C)[M2C_FIELD(arg0, u8 *, 0x5A)];
            if (var_r30->frames == 0xFFFF) {
                M2C_FIELD(arg0, u8 *, 0x5A) = 0U;
                M2C_FIELD(arg0, u32 *, 8) = M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), cr_AnimStep **, 0x1C)->frames;
            } else if (var_r30->frames == 0xFFFE) {
                *var_r31 |= 0x40;
                M2C_FIELD(arg0, u8 *, 0x5A) -= 1;
            } else if (var_r30->frames == 0xFFFD) {
                *var_r31 |= 0x1000;
                M2C_FIELD(arg0, u8 *, 0x5A) -= 1;
            } else {
                M2C_FIELD(arg0, u32 *, 8) = var_r30->frames;
            }
        } else {
            M2C_FIELD(arg0, u32 *, 8) -= 1;
        }
    }
}
