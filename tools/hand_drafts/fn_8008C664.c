/* fn_8008C664: Nov 6, 2026 hand draft (m2c 78%): makes one entry (fn_8008EE04 on the next 32-byte slot of lbl_80392A54 +0x2800, counter at +0x3800) with two optional u16 / f32 pairs; a compiled-out assert on the counter, `(count < 0x80) ? (void) 0 : (void) 0` (a compare with no branch), `(void *) 0` compares, and an empty then-branch holding `(void) 0;` (a compiled-out macro: the original keeps the jump over it, a truly empty branch loses it) */

void *fn_8008C664(s32 arg0, u16 arg1, void *arg2, void *arg3) {
    s32 temp_r31_27;

    (M2C_FIELD(lbl_80392A54, u16 *, 0x3800) < 0x80) ? (void) 0 : (void) 0;
    temp_r31_27 = fn_8008EE04(arg0, lbl_80392A54 + (M2C_FIELD(lbl_80392A54, u16 *, 0x3800) << 5) + 0x2800);
    if (arg3 == (void *) 0) {
        M2C_FIELD(temp_r31_27, s32 *, 0) |= 0x4000;
    } else {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(temp_r31_27, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0xC) = M2C_FIELD(arg3, u16 *, 0);
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(temp_r31_27, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0xE) = M2C_FIELD(arg3, u16 *, 2);
    }
    if (arg2 == (void *) 0) {
        (void) 0;
    } else {
        M2C_FIELD(M2C_FIELD(temp_r31_27, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(arg2, f32 *, 0);
        M2C_FIELD(M2C_FIELD(temp_r31_27, void **, 0x5C), f32 *, 4) = (f32) M2C_FIELD(arg2, f32 *, 4);
    }
    M2C_FIELD(temp_r31_27, void (**)(), 0x50) = fn_8008C660;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(temp_r31_27, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0x10) = arg1;
    M2C_FIELD(lbl_80392A54, u16 *, 0x3800) += 1;
    return (void *) temp_r31_27;
}
