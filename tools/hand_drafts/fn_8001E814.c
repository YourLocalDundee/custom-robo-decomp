/* fn_8001E814: Nov 13, 2026 hand draft: the last key time read as M2C_FIELD(keys + n * 0x1C, f32 *, -0xC) (lfsx with the index n * 28 - 12); the copy of the key pointer is a dead stack local (no register); the end time is the stack local itself, read back, not a temporary */

f32 fn_8001E814(u8 *arg0, u8 arg1, f32 farg0) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    void *sp8;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f25;
    f32 var_f24;
    f32 var_f23;
    f32 var_f22;
    f32 var_f21;
    f32 var_f20;
    f32 var_f19;
    u8 var_r31;

    var_f23 = M2C_FIELD(arg0, f32 *, 4);
    var_f31 = lbl_8045041C * farg0;
    sp8 = arg0;
    var_f24 = M2C_FIELD(arg0 + (arg1 * 0x1C), f32 *, -0xC);
    if (var_f31 < var_f23) {
        var_f31 = (lbl_8045041C * farg0) + (var_f24 - var_f23);
    } else if (var_f31 > var_f24) {
        var_f31 = (lbl_8045041C * farg0) - (var_f24 - var_f23);
    }
    var_r31 = 0;
    for (; var_r31 < arg1; var_r31 += 1) {
        if (!(!(M2C_FIELD(arg0, f32 *, 4) <= var_f31) || !(M2C_FIELD(arg0, f32 *, 0x10) >= var_f31))) {
            break;
        }
        arg0 += 0x1C;
    }
    sp18 = M2C_FIELD(arg0, f32 *, 0x10);
    var_f27 = M2C_FIELD(arg0, f32 *, 4);
    var_f30 = var_f31 - var_f27;
    var_f26 = sp18 - var_f27;
    sp20 = M2C_FIELD(arg0, f32 *, 0xC);
    var_f28 = M2C_FIELD(arg0, f32 *, 8);
    var_f30 /= var_f26;
    sp1C = M2C_FIELD(arg0, f32 *, 0x18);
    var_f22 = M2C_FIELD(arg0, f32 *, 0x14);
    if (M2C_FIELD(arg0, u8 *, 0) == 0) {
        var_f25 = lbl_80450408 - var_f30;
        var_f21 = var_f30 * var_f30;
        var_f19 = lbl_80450420 * var_f30;
        var_f20 = var_f25 * var_f25;
        sp10 = -var_f21 * var_f25;
        var_f29 = var_f20 * (lbl_80450408 + var_f19);
        sp14 = var_f30 * var_f20;
        spC = var_f21 * (lbl_80450424 - var_f19);
        return ((var_f28 * var_f29) + ((var_f22 * spC) + (var_f26 * ((sp20 * sp14) + (sp1C * sp10)))));
    }
    var_f29 = var_f22 - var_f28;
    var_f29 /= var_f26;
    return ((var_f29 * (var_f31 - var_f27)) + var_f28);
}
