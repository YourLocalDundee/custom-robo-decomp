/* fn_80069DA4: Nov 13, 2026 hand draft: window text at a position: the style-setter inline, size scaled; the object a void * parameter cast at each use */

static inline void cr_Window_SetStyle(f32 x, f32 y, f32 sz, f32 sp, u8 c0, u8 c1, u8 c2, u8 c3, u8 b0c, u8 b0d,
                                      u8 b1c, u8 b00, u8 g0, u8 g1) {
    cr_g_WindowStyle.unk_04 = x;
    cr_g_WindowStyle.unk_08 = y;
    cr_g_WindowStyle.unk_14 = sz;
    cr_g_WindowStyle.unk_24 = sp;
    cr_g_WindowStyle.unk_18[0] = c0;
    cr_g_WindowStyle.unk_18[1] = c1;
    cr_g_WindowStyle.unk_18[2] = c2;
    cr_g_WindowStyle.unk_18[3] = c3;
    cr_g_WindowStyle.unk_0c = b0c;
    cr_g_WindowStyle.unk_0d = b0d;
    cr_g_WindowStyle.unk_1c = b1c;
    cr_g_WindowStyle.unk_00 = b00;
    lbl_8044F5D0 = g0;
    lbl_8044F5CC = g1;
}

void fn_80069DA4(void *arg0, s32 arg1, void *arg2, u8 arg3, f32 farg0) {
    void *msg;

    msg = fn_8008E9FC(arg0);
    M2C_FIELD(msg, u16 *, 0x12) |= 0x8000;
    if (*(s32 *) arg0 & 0x40000) {
        cr_Window_SetStyle(M2C_FIELD(arg2, f32 *, 0), M2C_FIELD(arg2, f32 *, 4), lbl_80450AA8 * farg0, lbl_80450AAC,
                           0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0);
    } else {
        cr_Window_SetStyle(M2C_FIELD(arg2, f32 *, 0), M2C_FIELD(arg2, f32 *, 4), lbl_80450AA8 * farg0, lbl_80450AAC,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    fn_8006A7D8((s32 *) arg0, arg1, msg, arg3, 0U, M2C_FIELD(arg2, f32 *, 0));
}
