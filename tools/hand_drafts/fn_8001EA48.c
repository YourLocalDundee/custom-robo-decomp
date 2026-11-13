/* fn_8001EA48: Nov 12, 2026 hand draft (m2c 89%): the Hermite key interpolation; only the scratch float registers of three constants differed, set by the statement order. A sweep of the 63 legal orders of the seven statements after t * t: 1 - t first, then 2t, (1 - t)^2, then the basis values in the order h00, h11, h10, h01 (the original's stores) */

f32 fn_8001EA48(f32 farg0, f32 farg1, f32 farg2, f32 farg3, f32 farg4, f32 farg5, f32 farg6) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f25;

        var_f31 = lbl_80450428 * (farg6 - farg2);
    var_f29 = lbl_80450428 * (farg5 - farg2);
    var_f31 /= var_f29;
    var_f28 = var_f31 * var_f31;
    var_f30 = lbl_80450408 - var_f31;
    var_f26 = lbl_80450420 * var_f31;
    var_f27 = var_f30 * var_f30;
    var_f25 = var_f27 * (lbl_80450408 + var_f26);
    sp.sp10 = var_f31 * var_f27;
    sp.spC = -var_f28 * var_f30;
    sp.sp8 = var_f28 * (lbl_80450424 - var_f26);
    return ((farg0 * var_f25) + ((farg3 * sp.sp8) + (var_f29 * ((farg1 * sp.sp10) + (farg4 * sp.spC)))));
}
