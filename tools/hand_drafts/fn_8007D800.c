/* fn_8007D800: Nov 3, 2026 hand draft (m2c 89%): the fade test in double (`0.05 + x < 1.0`, lfd, no frsp), case 3 falls out with break and case 2 is a break (not returns), and the +5 written out (the original reloads the byte) */

void fn_8007D800(void *arg0) {
    void *var_r31;

    switch (M2C_FIELD(lbl_803864F8, s8 *, 0x1C)) { /* irregular */
    case 0:
        M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x2000;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) 0.0f;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 8) = (f32) 1.0f;
        return;
    case 1:
        var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8[6], void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(var_r31, s16 *, 0xC) = 0x40;
        M2C_FIELD(var_r31, s16 *, 0xE) = 0x40;
        M2C_FIELD(var_r31, s16 *, 8) = 0x240;
        M2C_FIELD(var_r31, s16 *, 0xA) = 0;
        if (0.05 + M2C_FIELD(var_r31, f32 *, 0x14) < 1.0) {
            M2C_FIELD(var_r31, f32 *, 0x14) += 0.05f;
            return;
        }
        M2C_FIELD(var_r31, f32 *, 0x14) = (f32) 1.0f;
        M2C_FIELD(lbl_803864F8, u8 *, 0x1C) = 2U;
        M2C_FIELD(lbl_803864F8, u8 *, 0x1D) = 0U;
        return;
    case 3:
        var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8[6], void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(lbl_803864F8, u8 *, 0x1D) += 1;
        if ((M2C_FIELD(lbl_803864F8, s8 *, 0x1D) > 0xA) && (M2C_FIELD(var_r31, f32 *, 0x14) > 0.0f)) {
            M2C_FIELD(var_r31, f32 *, 0x14) -= 0.1f;
        }
        break;
    case 2:
        break;
    case 4:
        var_r31 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8[6], void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(var_r31, f32 *, 0x14) = (f32) 1.0f;
        M2C_FIELD(var_r31, s16 *, 0xC) = 0x40;
        M2C_FIELD(var_r31, s16 *, 0xE) = 0x40;
        M2C_FIELD(var_r31, s16 *, 8) = 0;
        M2C_FIELD(var_r31, s16 *, 0xA) = 0;
        *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[6], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= 55.0f;
        if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 5) < 0xFF) {
            M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) + 5;
            return;
        }
        M2C_FIELD(lbl_803864F8, u8 *, 0x1C) = 2U;
        break;
    }
}
