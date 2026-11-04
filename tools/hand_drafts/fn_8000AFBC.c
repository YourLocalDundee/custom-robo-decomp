/* fn_8000AFBC: Nov 4, 2026 hand draft (permuter queue 3, score 0): `temp_r31_14++; temp_r31_14--;` at the head of the lbl_8044F377 == 0 branch (keeps the register allocation of the original) */
void fn_8000AFBC(void *arg0) {
    u8 *temp_r31_14;
    u8 var_r30_7;

    var_r30_7 = 0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) lbl_804502F0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_804502F4;
    temp_r31_14 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
    while (var_r30_7 < 8U) {
        if (var_r30_7 == (u8) lbl_8044F386) {
            *(s16 *) &((u8 (*)[0x20]) (temp_r31_14))[var_r30_7][0xC] = 0x18;
            (*(f32 *)((s8 *)((temp_r31_14 + ((var_r30_7 << 5) & 0x1FE0))) + (0))) = (f32) (var_r30_7 * 0x1B);
        } else {
            *(s16 *) &((u8 (*)[0x20]) (temp_r31_14))[var_r30_7][0xC] = 0;
            M2C_FIELD((temp_r31_14 + ((var_r30_7 << 5) & 0x1FE0)), f32 *, 0) = lbl_804502F8;
        }
        var_r30_7 += 1;
    }
    if (lbl_8044F386 == 8) {
        M2C_FIELD(temp_r31_14, s16 *, 0xEC) = 0x18;
        M2C_FIELD(temp_r31_14, f32 *, 0xE0) = (f32) lbl_804502FC;
    }
    if ((s8) lbl_8044F377 == 0) {
        temp_r31_14++;
        temp_r31_14--;
        M2C_FIELD(arg0, s32 *, 0) |= 1;
    } else if ((s32) lbl_8044F377 == 0xF) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
    } else if ((s32) lbl_8044F377 == 0x2D) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        lbl_8044F377 = 0;
    }
    lbl_8044F377 += 1;
}
