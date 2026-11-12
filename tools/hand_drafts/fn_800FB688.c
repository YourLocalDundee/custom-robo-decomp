/* fn_800FB688: Nov 5, 2026 hand draft (m2c 92%): the four u16 values at lbl_804516F8 (36, 37, 38, 39) are a local array initializer, `u16 v[4] = {36, 37, 38, 39}` (the compiler pools the literals in .sdata2 and copies them with lhz / sth, no masking; four assignments from extern consts add clrlwi) */

static inline u8 fn_800FB688_search0(u8 var_r31) {
    u8 var_r30;

    for (var_r30 = 0; var_r30 < 4U; var_r30 += 1) {
        if ((0x400 << var_r30) & M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C)) {
            return var_r30;
        }
    }
    return 0xFF;
}

void fn_800FB688(void *arg0) {
    u16 v[4] = {36, 37, 38, 39};
    u8 var_r31;
    u8 var_r29;
    u8 var_r27;

    var_r29 = 0;
    var_r31 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    if ((M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x487C) & 0x40) && ((s32) M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u8 *, 0x4860) != 5)) {
        var_r29 = 1;
    } else {
        M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x4880) = 0;
    }
    if ((M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x487C) & 0x40) && (lbl_8044F7F1 == 1)) {
        var_r29 = 1;
    }
    if (var_r29 != 0) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0x10) = v[var_r31];
        if (lbl_8044F7F1 == 1) {
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) (lbl_80451798 + M2C_FIELD(lbl_8044EBF8, f32 *, 0));
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) (lbl_8045179C + (lbl_804517A0 + lbl_8044EBF8[1]));
        } else {
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) (lbl_80451798 + M2C_FIELD(lbl_8044EBF8, f32 *, 0));
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) (lbl_804517A0 + lbl_8044EBF8[1]);
        }
        var_r27 = fn_800FB688_search0(var_r31);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += M2C_FIELD(lbl_80239B48, f32 *, (var_r27 * 8) & 0x7F8);
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += M2C_FIELD(&lbl_80239B48[(var_r27 * 8) & 0x7F8], f32 *, 4);
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x2000;
        M2C_FIELD(arg0, s32 *, 0) |= 0x8000;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) lbl_804517A4;
        M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x4880) = 2;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 8) = (f32) lbl_804517A4;
        return;
    }
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 8) = (f32) lbl_804517A4;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) lbl_80451768;
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
