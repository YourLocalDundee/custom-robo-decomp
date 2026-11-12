/* cr_Records_BuildBodyRows: Nov 6, 2026 hand draft (m2c 94%): the body order index stepped inside the read (`cr_g_PartOrderBody[var_r28++]`) and the totals loop steps the row pointer before the counter */

void cr_Records_BuildBodyRows(u8 *arg0, void *arg1) {
    f32 var_f31;
    s32 var_r29;
    s32 var_r28;
    void *var_r27;

    var_r27 = arg0;
    var_r28 = 0;
    var_r29 = 0;
    while (var_r29 < 0x22) {
        M2C_FIELD(arg0, s32 *, 0) = (s32) cr_g_PartOrderBody[var_r28++];
        M2C_FIELD(arg0, u32 *, 4) = (u32) cr_g_SaveData.body_records[M2C_FIELD(arg0, s32 *, 0)].wins;
        M2C_FIELD(arg0, u32 *, 0xC) = (u32) cr_g_SaveData.body_records[M2C_FIELD(arg0, s32 *, 0)].losses;
        M2C_FIELD(arg0, u32 *, 0x10) = (u32) cr_g_SaveData.body_records[M2C_FIELD(arg0, s32 *, 0)].battles;
        M2C_FIELD(arg0, s32 *, 8) = (s32) ((M2C_FIELD(arg0, u32 *, 0x10) - M2C_FIELD(arg0, u32 *, 4)) - M2C_FIELD(arg0, u32 *, 0xC));
        if (M2C_FIELD(arg0, u32 *, 0x10) != 0) {
            var_f31 = (f32) M2C_FIELD(arg0, u32 *, 4) / (f32) M2C_FIELD(arg0, u32 *, 0x10);
        } else {
            var_f31 = lbl_80451920;
        }
        M2C_FIELD(arg0, u32 *, 0x14) = (u32) ((lbl_80451978 * var_f31) + lbl_80451994);
        arg0 += 0x18;
        var_r29 += 1;
    }
    M2C_FIELD(arg1, u32 *, 4) = 0U;
    arg0 = var_r27;
    M2C_FIELD(arg1, s32 *, 0xC) = 0;
    var_r29 = 0;
    M2C_FIELD(arg1, s32 *, 8) = 0;
    M2C_FIELD(arg1, u32 *, 0x10) = 0U;
    while (var_r29 < 0x22) {
        M2C_FIELD(arg1, u32 *, 4) += M2C_FIELD(arg0, s32 *, 4);
        M2C_FIELD(arg1, s32 *, 0xC) += M2C_FIELD(arg0, s32 *, 0xC);
        M2C_FIELD(arg1, s32 *, 8) += M2C_FIELD(arg0, s32 *, 8);
        M2C_FIELD(arg1, u32 *, 0x10) += M2C_FIELD(arg0, s32 *, 0x10);
        if (M2C_FIELD(arg1, u32 *, 0x10) != 0) {
            var_f31 = (f32) M2C_FIELD(arg1, u32 *, 4) / (f32) M2C_FIELD(arg1, u32 *, 0x10);
        } else {
            var_f31 = lbl_80451920;
        }
        M2C_FIELD(arg1, u32 *, 0x14) = (u32) ((lbl_80451978 * var_f31) + lbl_80451994);
        arg0 += 0x18;
        var_r29 += 1;
    }
}
