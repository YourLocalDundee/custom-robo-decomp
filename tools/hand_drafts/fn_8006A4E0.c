/* fn_8006A4E0: Nov 13, 2026 hand draft: window text with a style per branch: the style-setter inline, one position local per branch */

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

s32 fn_8006A4E0(void *arg0, u16 arg1, s32 arg2, u8 arg3, u8 arg4) {
    void *msg;
    f32 *pos;
    f32 *pos2;

    msg = fn_8008E9FC(arg0);
    if (arg4 != 0) {
        M2C_FIELD(arg0, s32 *, 0) |= 0x40000;
        pos = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
        cr_Window_SetStyle(pos[0] + ((f32 (*)[2]) lbl_8020DD70)[arg1][0], pos[1] + ((f32 (*)[2]) lbl_8020DD70)[arg1][1], lbl_80450AA8, lbl_80450AAC,
                           0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0);
    } else {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFBFFFF;
        pos2 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
        cr_Window_SetStyle(pos2[0] + ((f32 (*)[2]) lbl_8020DD70)[arg1][0], pos2[1] + ((f32 (*)[2]) lbl_8020DD70)[arg1][1], lbl_80450AA8, lbl_80450AAC,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    return fn_8006A7D8((s32 *) arg0, arg2, msg, arg3, 0U, *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) + ((f32 (*)[2]) lbl_8020DD70)[arg1][0]);
}
