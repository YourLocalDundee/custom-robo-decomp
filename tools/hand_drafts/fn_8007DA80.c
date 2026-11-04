/* fn_8007DA80: Nov 3, 2026 hand draft (m2c 67%): the frame count 180 is a local (r31) converted at run time in the fade step (m2c folded it into a constant), the alpha stores unsigned (255, 200) */

void fn_8007DA80(void *arg0) {
    u32 var_r31;

    var_r31 = 0xB4;
    if (!(lbl_8044F620 & 0x4000)) {
        M2C_FIELD(arg0, u8 *, 0x54) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x55) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x56) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x57) = 0xFF;
        *lbl_8044F630 = lbl_80450DC8;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    if (lbl_8044F62C == 0) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        if ((*lbl_8044F630 - lbl_80450DCC) > lbl_80450DD0) {
            *lbl_8044F630 -= lbl_80450DCC;
            M2C_FIELD(arg0, s8 *, 0x57) = (s8) *lbl_8044F630;
            return;
        }
        M2C_FIELD(arg0, u8 *, 0x57) = 200;
        *lbl_8044F630 = lbl_80450DD0;
        lbl_8044F62C += 1;
        return;
    }
    if ((*lbl_8044F630 + (lbl_80450D9C / (f32) var_r31)) < lbl_80450DC8) {
        *lbl_8044F630 += lbl_80450D9C / (f32) var_r31;
        M2C_FIELD(arg0, s8 *, 0x57) = (s8) *lbl_8044F630;
        return;
    }
    M2C_FIELD(arg0, u8 *, 0x57) = 0xFF;
    lbl_8044F62C = 0;
    lbl_8044F620 &= 0xFFFFBFFF;
    *lbl_8044F630 = lbl_80450DC8;
}
