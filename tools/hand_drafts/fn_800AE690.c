/* fn_800AE690: Nov 13, 2026 hand draft: the five positions a local f32 table[5][2] with an initialiser (lbl_801C37F8); the row index with (u8) i; the unsigned count to float; the cursor flags a member array (index sum, then lbzx); the goto loop a while / break */

extern u8 *lbl_8044F728;

typedef struct {
    u8 pad[0x5D];
    u8 flag[1];
} WinAE690;

void fn_800AE690(void *arg0) {
    f32 t0[5][2] = { { 32.0f, 16.0f }, { 32.0f, 68.0f }, { 32.0f, 94.0f }, { 32.0f, 120.0f }, { 32.0f, 146.0f } };
    f32 spC;
    f32 sp8;
    u8 var_r31_8;
    u8 var_r29_12;

    var_r31_8 = 0;
    var_r29_12 = 0;
    sp8 = *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
    spC = M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 4);
    while ((s32) var_r31_8 < (s32) (lbl_8044F6F4 + 2)) {
        if (var_r31_8 != 0) {
            var_r29_12 = (5 - (lbl_8044F6F4 + 2)) + (u8) var_r31_8;
        }
        fn_80068358((s32) arg0, M2C_FIELD(&lbl_80229D18[(var_r31_8 * 0x10) & 0xFF0], s32 *, 8), (s32) t0[var_r29_12], (s32) &sp8, 0U);
        var_r31_8 += 1;
    }
    M2C_FIELD(lbl_8044F728, u16 *, 0) |= 0x10;
    M2C_FIELD(lbl_8044F728, f32 *, 0x54) = (f32) lbl_80451280;
    M2C_FIELD(lbl_8044F728, f32 *, 0x58) = (f32) (lbl_8045141C * (f32) (u32) (5 - (lbl_8044F6F4 + 1)));
    M2C_FIELD(lbl_8044F728, s16 *, 0x2C) = (s16) (lbl_8044F6F4 + 2);
    if ((void *) lbl_8044F728 != ((void *) 0)) {
        M2C_FIELD(lbl_8044F728, u8 *, 0x5D) |= 2;
    }
    while (((WinAE690 *) lbl_8044F728)->flag[M2C_FIELD(lbl_8044F728, s16 *, 0x38)] & 3) {
        if (!((M2C_FIELD(lbl_8044F728, s16 *, 0x38) += 1) < (s16) M2C_FIELD(lbl_8044F728, s16 *, 0x2C))) {
            break;
        }
    }
    if ((void *) lbl_8044F728 != ((void *) 0)) {
        M2C_FIELD(lbl_8044F728, u8 *, 0x5D) |= 4;
    }
    M2C_FIELD(arg0, s8 *, 0x58) = 1;
}
