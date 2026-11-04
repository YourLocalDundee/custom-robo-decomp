/* cr_Vec3_LengthInline: Nov 3, 2026 hand draft (m2c 67-71%), by the msl_sqrtf_blocks rule (draft_batch.py step 111): MSL's inline sqrtf in its source form (f32 result, Newton steps, NaN through __float_nan, the copy declared before the value) */

extern u32 __float_nan[];
#define CR_NAN (*(f32 *) __float_nan)

f32 cr_Vec3_LengthInline(void *arg0) {
    struct {
        f32 sp8;
    } sp;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f64 var_f31;
    s32 var_r31;

    var_f29 = (M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg0, f32 *, 0)) + ((M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg0, f32 *, 4)) + (M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg0, f32 *, 8)));
    if (var_f29 > lbl_804503D0) {
        var_f30 = var_f29;
        var_f31 = __frsqrte(var_f30);
        var_f31 = lbl_804503E0 * var_f31 * (lbl_804503E8 - var_f31 * var_f31 * var_f30);
        var_f31 = lbl_804503E0 * var_f31 * (lbl_804503E8 - var_f31 * var_f31 * var_f30);
        var_f31 = lbl_804503E0 * var_f31 * (lbl_804503E8 - var_f31 * var_f31 * var_f30);
        var_f28 = (f32) (var_f30 * var_f31);
    } else if (var_f29 < lbl_804503F0) {
        var_f28 = CR_NAN;
    } else {
        sp.sp8 = var_f29;
        switch (*(s32 *) &sp.sp8 & 0x7F800000) {                       /* irregular */
        case 0x7F800000:
            if (*(s32 *) &sp.sp8 & 0x7FFFFF) {
                var_r31 = 1;
            } else {
                var_r31 = 2;
            }
            break;
        case 0x0:
            if (*(s32 *) &sp.sp8 & 0x7FFFFF) {
                var_r31 = 5;
            } else {
                var_r31 = 3;
            }
            break;
        default:
            var_r31 = 4;
            break;
        }
        if (var_r31 == 1) {
            var_f28 = CR_NAN;
        } else {
            var_f28 = var_f29;
        }
    }
    return (f32) var_f28;
}
