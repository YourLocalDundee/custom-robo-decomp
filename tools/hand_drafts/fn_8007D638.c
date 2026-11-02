/* fn_8007D638: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the switch with a break per case (m2c's returns and its fall-through from case 1 into the empty case 2 add a blr), the byte steps written `x = x - 0xC` / `x = x + 5` (the value back in r4, where `-=` uses r0), the 0.1 step compared in double against 0.0 (lfd, no frsp) */
void fn_8007D638(void *arg0) {
    switch (M2C_FIELD(lbl_803864F8, s8 *, 4)) {
    case 0:
        M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x2000;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 8) = 1.0f;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = 1.0f;
        break;
    case 1:
        M2C_FIELD(lbl_803864F8, u8 *, 5) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 5) > 0xA) {
            if ((M2C_FIELD(arg0, u8 *, 0x57) - 0xC) > 0) {
                M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) - 0xC;
            } else {
                M2C_FIELD(arg0, u8 *, 0x57) = 0U;
                M2C_FIELD(lbl_803864F8, u8 *, 4) = 2U;
            }
        }
        break;
    case 2:
        break;
    case 3:
        M2C_FIELD(lbl_803864F8, u8 *, 5) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 5) > 0xA) {
            if (M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) - 0.1 > 0.0) {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) -= 0.1;
            } else {
                M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = 0.0f;
            }
        }
        break;
    case 4:
        *M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8, u32 *, 0), void **, 0x5C), void **, 0x1C)), f32 **, 4) -= 55.0f;
        if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 5) < 0xFF) {
            M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) + 5;
        } else {
            M2C_FIELD(lbl_803864F8, u8 *, 4) = 2U;
        }
        break;
    }
}
