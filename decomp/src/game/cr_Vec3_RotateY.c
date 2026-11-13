/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Vec3_RotateY: .text 0x8001BD54-0x8001BF04 (432 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_8045040C;
extern const f32 lbl_80450410;
extern u8 lbl_801E6580[4100];

/* cr_Vec3_RotateY: Nov 6, 2026 hand draft (m2c 87%): m2c's draft with the sine-table pointer set one statement later, in both halves (adjacent-swap sweep) */

void cr_Vec3_RotateY(void *arg0, f32 farg0) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 *var_r31;
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;

    var_r29 = (s32) ((lbl_8045040C * farg0) / lbl_80450410) & 0x3FFF;
    if ((s32) ((lbl_8045040C * farg0) / lbl_80450410) & 0x4000) {
        var_r28 = ((s32) (0x3FFF - var_r29) >> 4) + 1;
        var_r30 = (s32) ((var_r29 * 2) - 0x3FFF) >> 4;
        var_r31 = (float *) (lbl_801E6580 + (var_r28 * 4));
        if ((s32) ((lbl_8045040C * farg0) / lbl_80450410) & 0x8000) {
            var_f30 = *(var_r31 + var_r30);
            var_f31 = -*var_r31;
        } else {
            var_f31 = *var_r31;
            var_f30 = -*(var_r31 + var_r30);
        }
    } else {
        var_r28 = (var_r29 >> 4) + 1;
        var_r30 = (s32) (0x3FFF - (var_r29 * 2)) >> 4;
        var_r31 = (float *) (lbl_801E6580 + (var_r28 * 4));
        if ((s32) ((lbl_8045040C * farg0) / lbl_80450410) & 0x8000) {
            var_f31 = -*var_r31;
            var_f30 = -*(var_r31 + var_r30);
        } else {
            var_f31 = *var_r31;
            var_f30 = *(var_r31 + var_r30);
        }
    }
    var_f29 = M2C_FIELD(arg0, f32 *, 0);
    var_f28 = M2C_FIELD(arg0, f32 *, 8);
    M2C_FIELD(arg0, f32 *, 8) = (f32) ((var_f30 * var_f28) - (var_f31 * var_f29));
    M2C_FIELD(arg0, f32 *, 0) = (f32) ((var_f31 * var_f28) + (var_f30 * var_f29));
}
