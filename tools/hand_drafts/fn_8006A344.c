/* fn_8006A344: Nov 13, 2026 hand draft: sibling of fn_8006A00C (void, flags 0, 0) */

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

void fn_8006A344(void *arg0, s32 arg1) {
    void *msg;
    f32 *pos;

    msg = ((void **) (M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u8 **, 0x1C) + M2C_FIELD(arg0, u8 *, 0x5A) * 8))[1];
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFBFFFF;
    M2C_FIELD(arg0, s32 *, 0) |= 1;
    pos = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
    cr_Window_SetStyle(pos[0] + M2C_FIELD(lbl_8020DD70, f32 *, 0), pos[1] + lbl_8020DD70[1], lbl_80450AA8, lbl_80450AAC,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    fn_8006A7D8((s32 *) arg0, arg1, msg, 0U, 0U, *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4));
}
