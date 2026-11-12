/* fn_800DF320: Nov 6, 2026 hand draft (m2c 93%): the table pointer tested against `(void *) 0` (li r0,0; cmplw), and in the first camera case x is negated before y is copied */

extern u32 lbl_8044F5DC;
extern u8 *lbl_8044F5EC;

void fn_800DF320(u8 *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
        f32 sp14;
        f32 sp18;
        f32 sp1C;
    } sp;

    if ((void *) lbl_8044F5DC != (void *) 0) {
        sp.sp14 = M2C_FIELD(arg0, f32 *, 0x24);
        sp.sp18 = M2C_FIELD(arg0, f32 *, 0x28);
        sp.sp1C = M2C_FIELD(arg0, f32 *, 0x2C);
        if ((M2C_FIELD(arg0, s8 *, 0x1709) == (s32) (M2C_FIELD(arg0, s8 *, 0x1504) - 1)) || (M2C_FIELD(arg0, u8 *, 0x17BF) != 0)) {
            sp.sp8 = M2C_FIELD(arg0, f32 *, 0x30);
            sp.spC = M2C_FIELD(arg0, f32 *, 0x34);
            sp.sp10 = M2C_FIELD(arg0, f32 *, 0x38);
        } else {
            sp.sp8 = M2C_FIELD((lbl_8044F5DC + (M2C_FIELD(arg0, s8 *, 0x1709) * 0x284)), f32 *, 0);
            sp.spC = *(f32 *) &((u8 (*)[0x284]) (lbl_8044F5DC))[M2C_FIELD(arg0, s8 *, 0x1709)][0x4];
            sp.sp10 = *(f32 *) &((u8 (*)[0x284]) (lbl_8044F5DC))[M2C_FIELD(arg0, s8 *, 0x1709)][0x8];
        }
        if (M2C_FIELD(arg0, u8 *, 0x73) != 0) {
            if (M2C_FIELD(lbl_8044F5EC, u8 *, 0x338) == 1) {
                sp.sp8 = -sp.sp14;
                sp.spC = sp.sp18;
                sp.sp10 = sp.sp1C;
            } else if (M2C_FIELD(lbl_8044F5EC, u8 *, 0x338) == 2) {
                sp.sp10 = lbl_8045151C;
            } else if (M2C_FIELD(lbl_8044F5EC, u8 *, 0x338) == 3) {
                sp.sp10 = lbl_8045151C;
            } else if (M2C_FIELD(lbl_8044F5EC, u8 *, 0x338) == 4) {
                sp.sp8 = lbl_80451598;
            }
            M2C_FIELD(arg0, u16 *, 4) &= 0xFFFFFFC1;
            M2C_FIELD(arg0, u16 *, 4) |= 0x10;
            M2C_FIELD(arg0, s8 *, 0x17A7) = 0x1E;
        }
        fn_8001B1A0(&sp.sp14, &sp.sp8, arg0 + 0x18);
        cr_Vec3_Normalize(arg0 + 0x18);
    }
}
