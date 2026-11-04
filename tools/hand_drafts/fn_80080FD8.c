/* fn_80080FD8: Nov 3, 2026 hand draft (from the asm, 97%) + the permuter's change (a dead self-assignment of dead27 inside the first branch: the register order then puts b in r28): 100% */
void fn_80080FD8(void *arg0) {
    u8 *blk;
    s32 dead29;
    u8 b;
    s32 dead27;
    f32 k;
    f32 v;
    u8 *rec;

    k = lbl_80450E7C;
    if (!(lbl_8044F63A & 1)) {
        M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x5C), f32 *, 0) = lbl_80450E7C;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x5C), u8 **, 0x1C), u8 **, 4), f32 *, 0x14) = lbl_80450E5C;
        M2C_FIELD(arg0, u8 *, 0x57) = 0;
        M2C_FIELD(arg0, s32 *, 0x14) = 0;
        M2C_FIELD(arg0, s32 *, 0) &= ~1;
        return;
    }
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x5C), u8 **, 0x1C), u8 **, 4), u16 *, 0x12) |= 0x2000;
    if (M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x5C), u8 **, 0x1C), u8 **, 4), f32 *, 0x14) < lbl_80450E40) {
        dead27 = dead27;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x5C), u8 **, 0x1C), u8 **, 4), f32 *, 0x14) += lbl_80450EA0;
    }
    if (0) {
        dead29 = dead29;
    }
    v = M2C_FIELD(lbl_802191D8, f32 *, 0x40);
    b = 0;
    if (v > lbl_80450EA8) {
        b = 1;
    }
    if (0) {
        dead27 = dead27;
    }
    if (b) {
        blk = M2C_FIELD(arg0, u8 **, 0x5C);
        if (M2C_FIELD(blk, f32 *, 0) + (lbl_80450EAC + (lbl_80450E88 - k)) / lbl_80450E98 < lbl_80450E88) {
            M2C_FIELD(blk, f32 *, 0) = M2C_FIELD(blk, f32 *, 0) + (lbl_80450EAC + (lbl_80450E88 - k)) / lbl_80450E98;
        } else {
            M2C_FIELD(blk, f32 *, 0) = lbl_80450E88;
        }
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0) {
            fn_80098290(0x20000);
            M2C_FIELD(arg0, s32 *, 0x14) = 1;
        }
        if (lbl_80450EB0[0] + (f32) M2C_FIELD(arg0, u8 *, 0x57) < lbl_80450E64) {
            M2C_FIELD(arg0, u8 *, 0x57) = (f32) M2C_FIELD(arg0, u8 *, 0x57) + lbl_80450EB0[0];
        } else {
            lbl_8044F63A &= ~1;
        }
    }
    M2C_FIELD(arg0, s32 *, 0) |= 1;
}
