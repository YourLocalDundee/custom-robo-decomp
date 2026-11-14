/* fn_8011ABA4: Nov 13, 2026 hand draft: the name buffer is char[24] (frame 96), not one s8; the two flag bytes written 0x1A4 = 1 before 0x1A3 = 0 */

extern u8 *lbl_8044F850;
extern u8 *lbl_8044F854;

void fn_8011ABA4(void *arg0) {
    char sp8[24];
    s32 var_r31_28;
    u16 *temp_r29_62;
    void *temp_r28_213;
    u8 temp_r5_179;

    var_r31_28 = 0;
    *M2C_FIELD(lbl_8023F9F8, s32 **, 0x238) |= 1;
    M2C_FIELD(lbl_8023F9F8, s8 *, 0x1A4) = 1;
    M2C_FIELD(lbl_8023F9F8, s8 *, 0x1A3) = 0;
    M2C_FIELD(lbl_8023F9F8, s8 *, 0x1A2) = 0;
    lbl_8023F9F8[0xDC] = lbl_8023F9F8[0xB];
    lbl_8023F9F8[0xDA] = 0;
    lbl_8023F9F8[0xDB] = 0x13;
    lbl_8023F9F8[0xEA] = 0;
    lbl_8023F9F8[0xEB] = 1;
    M2C_FIELD(arg0, u8 *, 0xC) = 0U;
    M2C_FIELD(arg0, u8 *, 0xD) = 0U;
    while (var_r31_28 < 3) {
        if (var_r31_28 < 3) {
            temp_r29_62 = *M2C_FIELD((lbl_802408E8 + (var_r31_28 * 4)), u16 ***, 0);
            if (*temp_r29_62 & 0x8000) {
                strcpy((char *) (sp8), (const char *) (fn_80066948(0x11U, 8U)));
                M2C_FIELD(arg0, u8 *, 0xD) |= (1 << var_r31_28);
            } else if (*temp_r29_62 & 1) {
                strcpy((char *) (sp8), (const char *) (temp_r29_62 + 1));
            } else {
                strcpy((char *) (sp8), (const char *) (fn_80066948(0x11U, 4U)));
                M2C_FIELD(arg0, u8 *, 0xC) |= (1 << var_r31_28);
            }
        } else {
            sp8[0] = 0;
        }
        M2C_FIELD((lbl_8041A67C + (var_r31_28 * 2 * 0x34)), f32 *, 0x10) = (f32) lbl_804519B0;
        M2C_FIELD((lbl_8041A67C + (var_r31_28 * 2 * 0x34)), f32 *, 0x14) = (f32) ((lbl_804519BC * (f32) var_r31_28) + lbl_804519B8);
        fn_8006A00C(M2C_FIELD((lbl_8041A67C + (var_r31_28 * 2 * 0x34)), void **, 0x30), fn_80066948(0x11U, (u16) var_r31_28));
        M2C_FIELD((lbl_8041A67C + (((var_r31_28 * 2) + 1) * 0x34)), f32 *, 0x10) = (f32) lbl_804519C0;
        M2C_FIELD((lbl_8041A67C + (((var_r31_28 * 2) + 1) * 0x34)), f32 *, 0x14) = (f32) ((lbl_804519BC * (f32) var_r31_28) + lbl_804519B8);
        fn_8006A344(M2C_FIELD((lbl_8041A67C + (((var_r31_28 * 2) + 1) * 0x34)), void **, 0x30), (s32) sp8);
        var_r31_28 += 1;
    }
    M2C_FIELD(arg0, s32 *, 8) = 0;
    temp_r5_179 = M2C_FIELD(arg0, s32 *, 8);
    M2C_FIELD(lbl_8044F850, s32 *, 0) |= 1;
    M2C_FIELD(lbl_8044F87C, u8 *, 0) = temp_r5_179;
    if (lbl_8044F87C[1] != (u8) M2C_FIELD(lbl_8044F87C, u8 *, 0)) {
        fn_8008E108(lbl_8044F854, lbl_8044ED16);
        fn_8008E108(lbl_8044F850, lbl_8044ED10);
        lbl_8044F87C[1] = M2C_FIELD(lbl_8044F87C, u8 *, 0);
    }
    temp_r28_213 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F850, void **, 0x5C), void **, 0x1C), void **, 4);
    M2C_FIELD(temp_r28_213, f32 *, 0) = (f32) lbl_80451900;
    M2C_FIELD(temp_r28_213, f32 *, 4) = (f32) (u16) (((temp_r5_179 << 5) & 0x1FE0) + 0x77);
    M2C_FIELD(arg0, s32 *, 4) = 1;
    fn_8008B930(1U);
}
