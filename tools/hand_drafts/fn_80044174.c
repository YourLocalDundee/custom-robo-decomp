/* fn_80044174: Nov 13, 2026 hand draft: part reset: compiled-out debug test that names a constant local ((void) var), base-first record address, chained float and byte stores */

void fn_80044174(u8 *arg0, u16 arg1) {
    u8 *var_r31;
    s32 var_r28;
    s32 var_r27;

    var_r27 = 0x20;
    if (M2C_FIELD((lbl_801F9074 + (arg1 * 0x14)), u8 *, 0xC) == 1) {
        (void) var_r27;
    }
    var_r28 = 0;
    while (var_r28 < (s32) M2C_FIELD((lbl_801F9074 + (arg1 * 0x14)), u8 *, 0xD)) {
        var_r31 = (void *) ((var_r28 * 0x1360) + 0x300);
        var_r31 = arg0 + (s32) var_r31;
        M2C_FIELD(var_r31, s8 *, 5) = 0;
        M2C_FIELD(var_r31, void **, 0x2F8) = (void *) (var_r31 + 0x10);
        M2C_FIELD(var_r31, void **, 0x2FC) = (void *) (var_r31 + 0x1C);
        M2C_FIELD(var_r31, f32 *, 0x300) = M2C_FIELD(var_r31, f32 *, 0x304) = lbl_80450654;
        M2C_FIELD(var_r31, f32 *, 0x308) = (f32) lbl_80450638;
        fn_800755E8((struct HSD_JObj **) (var_r31 + 0x310), 0);
        fn_800755E8((struct HSD_JObj **) (var_r31 + 0x314), 1);
        fn_80077888((struct HSD_JObj **) (var_r31 + 0x350), 0);
        fn_80077888((struct HSD_JObj **) (var_r31 + 0x354), 1);
        fn_800774A4((struct HSD_JObj **) (var_r31 + 0x358));
        var_r28 += 1;
    }
    fn_800755E8((struct HSD_JObj **) (arg0 + 0x2DC), 2);
    M2C_FIELD(arg0, s16 *, 0x2A4) = 0;
    M2C_FIELD(arg0, s16 *, 0x2A6) = 0;
    M2C_FIELD(arg0, s8 *, 0x296) = M2C_FIELD(arg0, s8 *, 0x297) = 0;
    fn_8001CCC4(arg0 + 0x2B8, lbl_80450638, lbl_80450638, lbl_80450638);
    if ((arg1 == 0x1A) || (arg1 == 0x18)) {
        M2C_FIELD(arg0, s8 *, 0x26F00) = 1;
    }
    if (arg1 == 0x18) {
        M2C_FIELD(arg0, s8 *, 0x26F01) = 1;
    }
    if (arg1 == 0x1E) {
        M2C_FIELD(arg0, s8 *, 0x26F02) = 1;
    }
}
