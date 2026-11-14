/* fn_800B44D0: Nov 13, 2026 hand draft: view_reset inlined in both branches; the rate is stored before the flag is set (m2c shows the scheduled order) */

static inline void view_reset(void) {
    f32 a[4] = { 0.0f, 640.0f, 0.0f, 480.0f };
    f32 b[4] = { 0.0f, 640.0f, 0.0f, 480.0f };

    fn_80029F70(cr_g_CurrentCam, a);
    fn_80029C20(cr_g_CurrentCam, b);
}

void fn_800B44D0(s32 arg0, u32 arg1) {

    if (arg0 & 0x80) {
        view_reset();
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg0;
        fn_80023394((s8 *) lbl_803BA114, 0xD4U);
        M2C_FIELD(lbl_803BA114, u32 *, 0) |= 1;
        if (arg1 != 0) {
            M2C_FIELD(lbl_803BA114, f32 *, 0xD0) = lbl_804512A8 / (f32) arg1;
            M2C_FIELD(lbl_803BA114, u32 *, 0) |= 4;
        }
    } else if (arg0 & 0x40) {
        view_reset();
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg0;
        fn_80023394((s8 *) lbl_803BA114, 0xD4U);
        M2C_FIELD(lbl_803BA114, u32 *, 0) |= 2;
        if (arg1 != 0) {
            M2C_FIELD(lbl_803BA114, f32 *, 0xD0) = lbl_804512A8 / (f32) arg1;
            M2C_FIELD(lbl_803BA114, u32 *, 0) |= 4;
        }
    }
}
