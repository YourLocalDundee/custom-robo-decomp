/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80093180: .text 0x80093180-0x800932D0 (336 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F670;
extern u8 lbl_8044F66F;
extern u8 lbl_8044F66D;
extern const f32 lbl_8045102C;
extern const f32 lbl_8045108C;
extern u8 lbl_8044F66C;
extern const f32 lbl_80451008;

void fn_80093180(void *arg0) {
    void *var_r31;
    u8 var_r30;

    var_r30 = 0;
    if (lbl_8044F670 != 0) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    if (lbl_8044F66F != 0) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        if (lbl_8044F66D == 0) {
            var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
            M2C_FIELD(var_r31, u16 *, 0x12) |= 0x2000;
            if (M2C_FIELD(var_r31, f32 *, 0x14) < lbl_8045102C) {
                M2C_FIELD(var_r31, f32 *, 0x14) += lbl_8045108C;
            } else {
                var_r30 = 1;
                M2C_FIELD(var_r31, f32 *, 0x14) = (f32) lbl_8045102C;
            }
            if (var_r30 != 0) {
                lbl_8044F66C = 1;
            }
        }
    } else if ((lbl_8044F66C == 2) && (lbl_8044F66D == 3)) {
        var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(var_r31, u16 *, 0x12) |= 0x2000;
        if ((M2C_FIELD(var_r31, f32 *, 0x14) - lbl_8045108C) > lbl_80451008) {
            M2C_FIELD(var_r31, f32 *, 0x14) -= lbl_8045108C;
        } else {
            var_r30 = 1;
            M2C_FIELD(var_r31, f32 *, 0x14) = (f32) lbl_80451008;
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        }
        if (var_r30 != 0) {
            lbl_8044F66D = 4;
            lbl_8044F66C = 0;
        }
    }
    M2C_FIELD(arg0, u8 *, 0x57) = 0x80;
}
