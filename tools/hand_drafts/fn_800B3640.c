/* fn_800B3640: Nov 13, 2026 hand draft: the default rectangle a local f32[4] with an initialiser; in the 0x800 branch the camera view pointer (cam + 0x24) is a local too, assigned after the flag word is set */

void fn_800B3640(u8 *arg0, s32 arg1, u32 arg2, s32 arg3, f32 farg0, f32 farg1) {
    f32 t0[4] = { 0.0f, 640.0f, 0.0f, 480.0f };
    u8 *temp_r31_30;

    if (arg1 & 0x400) {
        temp_r31_30 = arg0 + 0x24;
        fn_80029C20(arg0, (void *) arg3);
        fn_80029F70(arg0, &t0[0]);
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg1;
        M2C_FIELD(temp_r31_30, s32 *, 0) = 0;
        M2C_FIELD(temp_r31_30, s32 *, 0) |= 4;
        if ((lbl_80451280 != farg0) || (lbl_80451280 != farg1)) {
            M2C_FIELD(temp_r31_30, f32 *, 4) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 8) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 0xC) = farg1;
            M2C_FIELD(temp_r31_30, f32 *, 0x10) = farg1;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 1;
        }
        if (arg2 != 0) {
            M2C_FIELD(temp_r31_30, u32 *, 0x24) = arg2;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 2;
        }
    } else if (arg1 & 0x800) {
        fn_80029C20(arg0, (void *) arg3);
        fn_80029F70(arg0, &t0[0]);
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg1;
        temp_r31_30 = cr_g_CurrentCam + 0x24;
        M2C_FIELD(temp_r31_30, s32 *, 0) = 0;
        M2C_FIELD(temp_r31_30, s32 *, 0) |= 8;
        if ((lbl_80451280 != farg0) || (lbl_80451280 != farg1)) {
            M2C_FIELD(temp_r31_30, f32 *, 0x14) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 0x18) = farg0;
            M2C_FIELD(temp_r31_30, f32 *, 0x1C) = farg1;
            M2C_FIELD(temp_r31_30, f32 *, 0x20) = farg1;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 1;
        }
        if (arg2 != 0) {
            M2C_FIELD(temp_r31_30, u32 *, 0x24) = arg2;
            M2C_FIELD(temp_r31_30, s32 *, 0) |= 2;
        }
    }
}
