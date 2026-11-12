/* fn_8010E47C: Nov 6, 2026 hand draft (m2c 92%): m2c's draft with the window pointer reads (cr_g_StageSelector.windows[0]) each one statement later (adjacent-swap sweep) */

void fn_8010E47C(void) {
    u8 *var_r31;
    u8 var_r30;
    u8 var_r29;
    s32 var_r28;
    u8 *var_r27;

    fn_8008C5F0(&cr_g_StageSelector.unk_0e0[0x3C], &cr_g_StageSelector.unk_0e0[0x2C], (s32) &cr_g_StageSelector.unk_0cc, 2U);
    cr_g_StageSelector.unk_0dc = 0x3E;
    cr_g_StageSelector.unk_0d8 = 0x258;
    cr_g_StageSelector.unk_0da = 0xF8;
    cr_g_StageSelector.unk_0cc = lbl_80451840;
    cr_g_StageSelector.unk_0d0 = lbl_80451844;
    cr_g_StageSelector.unk_13c = (void *) (fn_8008EE04(9, (void *) (&cr_g_StageSelector.unk_0e0[0x3C])));
    M2C_FIELD(cr_g_StageSelector.unk_13c, void (**)(), 0x50) = fn_8010E478;
    var_r31 = cr_g_StageSelector.windows[0];
    fn_8008C5F0(var_r31 + 0x10, var_r31, (s32) &cr_g_TextRecords[0x620], 0x40U);
    fn_8008C5F0(var_r31 + 0x44, var_r31 + 0x34, (s32) &cr_g_TextRecords[0x660], 0x40U);
    fn_8008C5F0(var_r31 + 0x78, var_r31 + 0x68, (s32) &cr_g_TextRecords[0x6A0], 0x40U);
    fn_8008C5F0(var_r31 + 0xAC, var_r31 + 0x9C, (s32) &cr_g_TextRecords[0x6E0], 0x40U);
    fn_8008C5F0(var_r31 + 0xE0, var_r31 + 0xD0, (s32) &cr_g_TextRecords[0x720], 0x40U);
    fn_8008C5F0(var_r31 + 0x114, var_r31 + 0x104, (s32) &cr_g_TextRecords[0x760], 0x40U);
    fn_8008C5F0(var_r31 + 0x148, var_r31 + 0x138, (s32) &cr_g_TextRecords[0x7A0], 0x80U);
    fn_8008C5F0(var_r31 + 0x17C, var_r31 + 0x16C, (s32) &cr_g_TextRecords[0x820], 0x40U);
    var_r30 = 0;
    while (var_r30 < 8U) {
        *(s32 *) &((u8 (*)[0x34]) (var_r31))[var_r30][0x30] = fn_8008EE04(9, &((u8 (*)[0x34]) (var_r31))[var_r30][0x10]);
        var_r28 = fn_80066D3C((u16) var_r30);
        if (var_r30 == 6) {
            fn_80068988(var_r28, 0x80U);
        } else {
            fn_80068988(var_r28, 0x40U);
        }
        fn_8006A4E0((void *) (*(u32 *) &((u8 (*)[0x34]) (var_r31))[var_r30][0x30]), 0, var_r28, 0U, 0U);
        M2C_FIELD(*(u32 *) &((u8 (*)[0x34]) (var_r31))[var_r30][0x30], void (**)(), 0x50) = fn_8010E478;
        *M2C_FIELD(*(u32 *) &((u8 (*)[0x34]) (var_r31))[var_r30][0x30], f32 **, 0x5C) = lbl_80451848;
        M2C_FIELD(M2C_FIELD(*(u32 *) &((u8 (*)[0x34]) (var_r31))[var_r30][0x30], void **, 0x5C), f32 *, 4) = (f32) ((var_r30 * 0x18) + 0x50);
        if (var_r30 == 7) {
            M2C_FIELD(M2C_FIELD(*(u32 *) &((u8 (*)[0x34]) (var_r31))[var_r30][0x30], void **, 0x5C), f32 *, 4) = (f32) ((var_r30 * 0x18) + 0x5C);
        }
        var_r30 += 1;
    }
    var_r29 = 0;
    M2C_FIELD(cr_g_StageSelector.unk_13c, s32 *, 0) &= 0xFFFFFFFE;
    var_r27 = cr_g_StageSelector.windows[0];
    while (var_r29 < 8U) {
        **(s32 **) &((u8 (*)[0x34]) (var_r27))[(u8) (var_r29)][0x30] &= 0xFFFFFFFE;
        var_r29 += 1;
    }
}
