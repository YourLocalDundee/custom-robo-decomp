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
