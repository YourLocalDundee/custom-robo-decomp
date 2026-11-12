/* fn_8008CA14: Nov 6, 2026 hand draft (m2c 87%): the step added to / taken from the u8 at lbl_80388284 + 0x203B is the float quotient cast straight to u8, (u8) (C / (f32) d) (m2c's (s32) cast scheduled the int-to-float conversion differently; (u8) (s32) adds a clrlwi) */


void fn_8008CA14(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x10) != (s32) M2C_FIELD(arg0, s32 *, 0x14)) {
        M2C_FIELD(lbl_8044EAF0, u8 *, 0) = 0;
        lbl_8044EAF0[1] = 3;
        M2C_FIELD(arg0, u8 *, 0x57) = 0U;
        M2C_FIELD(lbl_80388284, u8 *, 0x203B) = 0U;
        fn_80098290(1);
    }
    if (M2C_FIELD(&lbl_8044EAF0[(s8) lbl_8044EAF0[1]], s8 *, 2) <= M2C_FIELD(lbl_8044EAF0, s8 *, 0)) {
        M2C_FIELD(lbl_8044EAF0, u8 *, 0) = 0;
        lbl_8044EAF0[1] += 1;
        if ((s32) lbl_8044EAF0[1] == 4) {
            lbl_8044EAF0[1] = 0;
        }
    } else {
        M2C_FIELD(lbl_8044EAF0, u8 *, 0) += 1;
        if ((s8) lbl_8044EAF0[1] == 0) {
            M2C_FIELD(lbl_80388284, u8 *, 0x203B) += (u8) (lbl_80450FF8 / (f32) M2C_FIELD(&lbl_8044EAF0[(s8) lbl_8044EAF0[1]], s8 *, 2));
        } else if ((s32) lbl_8044EAF0[1] == 2) {
            M2C_FIELD(lbl_80388284, u8 *, 0x203B) -= (u8) (lbl_80450FF8 / (f32) M2C_FIELD(&lbl_8044EAF0[(s8) lbl_8044EAF0[1]], s8 *, 2));
        }
    }
    M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(lbl_80388284, u8 *, 0x203B);
}
