/* fn_80092CB8: Nov 13, 2026 hand draft: vertex alpha pulse: 32-byte vertex record, u8 index, -= / += on the alpha byte */

typedef struct cr_Vtx20c {
    u8 pad[0x10];
    u16 kind;
    u8 pad2[9];
    u8 a;
    u8 pad3[4];
} cr_Vtx20c;

void fn_80092CB8(void *arg0) {
    cr_Vtx20c *v;
    u8 i;

    if (lbl_8044F670 != 0) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    if ((lbl_8044F66D >= 1U) && (lbl_8044F66F != 0)) {
        i = 0;
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = M2C_FIELD((lbl_8021A55C + ((s8) lbl_8044F672 * 8)), f32 *, 0);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = M2C_FIELD((lbl_8021A55C + ((s8) lbl_8044F672 * 8)), f32 *, 4);
        v = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), cr_Vtx20c **, 4);
        while (v[i].kind != 0xFFFF) {
            if (v[i].kind == 0xC) {
                if (lbl_8044F671 == 0) {
                    if ((v[i].a - 0xA) > 0) {
                        v[i].a -= 0xA;
                    } else {
                        v[i].a = 0;
                        lbl_8044F671 = 1;
                    }
                } else if (lbl_8044F671 == 1) {
                    if ((v[i].a + 5) < 0xC8) {
                        v[i].a += 5;
                    } else {
                        v[i].a = 0xC8;
                        lbl_8044F671 = 0;
                    }
                } else {
                    v[i].a = 0;
                    lbl_8044F671 = 0;
                }
            }
            i += 1;
        }
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
