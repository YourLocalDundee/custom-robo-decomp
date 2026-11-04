/* fn_8007D3C8: Nov 3, 2026 hand draft (m2c 82%): the alpha steps written out, `x = x - 5` (the original reloads each pointer), case 3 / 2 / 0 end in break, and case 1 writes its float step out too (the pointer chain walked again) */

void fn_8007D3C8(void *arg0) {

    switch (M2C_FIELD(lbl_803864F8, s8 *, 0xC)) { /* irregular */
    case 1:
        M2C_FIELD(lbl_803864F8, u8 *, 0xD) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 0xD) < 0x14) {
            M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) - 5;
            M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) = M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) - 5;
            *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) = *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) - lbl_80450D94;
            return;
        }
        *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) = lbl_80450D98;
        M2C_FIELD(lbl_803864F8, u8 *, 0xC) = 2U;
        *M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8[4], void **, 0x5C), void **, 0x1C), f32 **, 4) = lbl_80450D98;
        return;
    case 3:
        M2C_FIELD(lbl_803864F8, u8 *, 0xD) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 0xD) > 0xA) {
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[2], void **, 0x5C), void **, 0x1C)), f32 **, 4) += lbl_80450D9C;
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[4], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= lbl_80450D9C;
            return;
        }
        break;
    case 2:
    case 0:
        break;
    case 4:
        M2C_FIELD(lbl_803864F8, u8 *, 0xD) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 0xD) > 0xA) {
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[2], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= lbl_80450D9C;
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[4], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= lbl_80450D9C;
            if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 5) < 0xFF) {
                M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) + 5;
                M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) = M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) + 5;
                return;
            }
            M2C_FIELD(lbl_803864F8, u8 *, 0xC) = 2U;
            return;
        }
        break;
    }
}
