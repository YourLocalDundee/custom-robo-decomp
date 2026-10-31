/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80111EEC_fn_80111EEC: .text 0x80111EEC-0x8011214C (608 bytes, 1 functions: fn_80111EEC .. fn_80111EEC), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041A244[852];
extern u8 lbl_8023C6AC[416];
extern f32 lbl_8044ECDC[2];
extern f32 lbl_8044ECE8[2];
extern f32 lbl_80451858;
extern u16 lbl_8044ECE4;

/* the file's functions (each is defined below, in address order) */
void fn_80111EEC(void);

void fn_80111EEC(void) {
    s8 sp8;
    u8 var_r31;
    u8 var_r30;
    u8 var_r29;
    u8 var_r28;
    u8 var_r27;

    var_r30 = 0;
    var_r27 = lbl_8041A244[4];
    while (var_r30 < 2U) {
        var_r29 = 0;
        while (var_r29 < 6U) {
            sp8 = 0;
            var_r31 = var_r29 + (var_r30 * 6);
            var_r28 = var_r31 + (var_r27 * 6);
            if ((s8) lbl_8041A244[5] <= (s32) var_r28) {
                M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) = 0xFFFF;
            } else if (M2C_FIELD(&lbl_8041A244[var_r28], u8 *, 0x92) != 0) {
                M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) = M2C_FIELD(&lbl_8041A244[(var_r28 * 2) & 0x1FE], u16 *, 0x3E);
            } else {
                M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], s16 *, 0x10) = 0xB;
            }
            if (lbl_8041A244[0xD] != 0) {
                if (M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) == 1) {
                    M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], s16 *, 0x10) = 0x11;
                } else if (M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) == 0x1F) {
                    M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], s16 *, 0x10) = 0x12;
                }
            }
            M2C_FIELD(lbl_8023C6AC, f32 *, (var_r31 << 5) & 0x1FE0) = (f32) (M2C_FIELD(lbl_8044ECDC, f32 *, 0) + ((M2C_FIELD(lbl_8044ECE8, f32 *, 0) * (f32) var_r29) + lbl_80451858));
            M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], f32 *, 4) = (f32) (lbl_8044ECDC[1] + ((lbl_8044ECE8[1] * (f32) var_r30) + lbl_80451858));
            M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0xC) = M2C_FIELD(&lbl_8044ECE4, u16 *, 0);
            var_r29 += 1;
            M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0xE) = M2C_FIELD(&lbl_8044ECE4, u16 *, 2);
        }
        var_r30 += 1;
    }
    M2C_FIELD(lbl_8023C6AC, u16 *, 0x190) = 0xFFFF;
}
