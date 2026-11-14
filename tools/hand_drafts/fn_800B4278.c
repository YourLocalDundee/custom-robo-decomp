/* fn_800B4278: Nov 13, 2026 hand draft: both branches call one inline (view_reset: two local f32[4] rectangles with initialisers, one pair of constants lbl_801C3768 / lbl_801C3778 for every inlining) */

static inline void view_reset(void) {
    f32 a[4] = { 0.0f, 640.0f, 0.0f, 480.0f };
    f32 b[4] = { 0.0f, 640.0f, 0.0f, 480.0f };

    fn_80029F70(cr_g_CurrentCam, a);
    fn_80029C20(cr_g_CurrentCam, b);
}

void fn_800B4278(s32 arg0, u32 arg1, f32 farg0, f32 farg1) {
    u8 *temp_r31_18;

    temp_r31_18 = cr_g_CurrentCam + 0x4C;
    if (arg0 & 0x200) {
        view_reset();
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg0;
        M2C_FIELD(temp_r31_18, s32 *, 0) = 0;
        M2C_FIELD(temp_r31_18, s32 *, 0) |= 8;
        if ((lbl_80451280 != farg0) || (lbl_80451280 != farg1)) {
            M2C_FIELD(temp_r31_18, f32 *, 0x14) = farg0;
            M2C_FIELD(temp_r31_18, f32 *, 0x18) = farg0;
            M2C_FIELD(temp_r31_18, f32 *, 0x1C) = farg1;
            M2C_FIELD(temp_r31_18, f32 *, 0x20) = farg1;
            M2C_FIELD(temp_r31_18, s32 *, 0) |= 1;
        }
        if (arg1 != 0) {
            M2C_FIELD(temp_r31_18, u32 *, 0x24) = arg1;
            M2C_FIELD(temp_r31_18, s32 *, 0) |= 2;
        }
    } else if (arg0 & 0x100) {
        view_reset();
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg0;
        M2C_FIELD(temp_r31_18, s32 *, 0) = 0;
        M2C_FIELD(temp_r31_18, s32 *, 0) |= 4;
        if ((lbl_80451280 != farg0) || (lbl_80451280 != farg1)) {
            M2C_FIELD(temp_r31_18, f32 *, 4) = farg0;
            M2C_FIELD(temp_r31_18, f32 *, 8) = farg0;
            M2C_FIELD(temp_r31_18, f32 *, 0xC) = farg1;
            M2C_FIELD(temp_r31_18, f32 *, 0x10) = farg1;
            M2C_FIELD(temp_r31_18, s32 *, 0) |= 1;
        }
        if (arg1 != 0) {
            M2C_FIELD(temp_r31_18, u32 *, 0x24) = arg1;
            M2C_FIELD(temp_r31_18, s32 *, 0) |= 2;
        }
    }
}
