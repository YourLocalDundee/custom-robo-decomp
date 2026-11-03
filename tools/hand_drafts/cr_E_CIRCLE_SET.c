/* cr_E_CIRCLE_SET: step 110 hand draft (the first float test is `>=`: the original compiles an ordered compare, m2c wrote ==) */
void cr_E_CIRCLE_SET(void) {
    f32 var_f31_0;
    f32 var_f30_0;
    s16 *temp_r31_14;
    u32 *cmd_cnt;
    s32 var_r30_73;

    temp_r31_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 8;
    if (0) {
        (*cmd_cnt)++;
    }
    M2C_FIELD(lbl_803BA330, f32 *, 8) = (f32) (M2C_FIELD(temp_r31_14, f32 *, 4) - lbl_804512AC);
    M2C_FIELD(lbl_803BA330, f32 *, 0xC) = (f32) (M2C_FIELD(temp_r31_14, f32 *, 8) - lbl_804512B0);
    M2C_FIELD(lbl_803BA330, f32 *, 0x18) = (f32) M2C_FIELD(temp_r31_14, f32 *, 0xC);
    M2C_FIELD(lbl_803BA330, u16 *, 0) |= 0x4000;
    M2C_ERROR(/* unknown instruction: cror 0x2, 0x1, 0x2 */);
    if (M2C_FIELD(lbl_803BA330, f32 *, 8) >= lbl_80451280) {
        var_r30_73 = 0;
    } else {
        var_r30_73 = 2;
    }
    if (M2C_FIELD(lbl_803BA330, f32 *, 0xC) >= lbl_80451280) {
        var_r30_73 += 1;
    }
    switch (var_r30_73) {                           /* irregular */
    case 0:
        var_f31_0 = lbl_80451480 + M2C_FIELD(lbl_803BA330, f32 *, 8);
        var_f30_0 = lbl_804512B0 + M2C_FIELD(lbl_803BA330, f32 *, 0xC);
        break;
    case 1:
        var_f31_0 = lbl_80451480 + M2C_FIELD(lbl_803BA330, f32 *, 8);
        var_f30_0 = M2C_FIELD(lbl_803BA330, f32 *, 0xC) - lbl_804512B0;
        break;
    case 2:
        var_f31_0 = M2C_FIELD(lbl_803BA330, f32 *, 8) - lbl_80451480;
        var_f30_0 = M2C_FIELD(lbl_803BA330, f32 *, 0xC) - lbl_804512B0;
        break;
    case 3:
        var_f31_0 = M2C_FIELD(lbl_803BA330, f32 *, 8) - lbl_80451480;
        var_f30_0 = lbl_804512B0 + M2C_FIELD(lbl_803BA330, f32 *, 0xC);
        break;
    }
    M2C_FIELD(lbl_803BA330, f32 *, 0x1C) = (f32) sqrt((f64) ((var_f31_0 * var_f31_0) + (var_f30_0 * var_f30_0)));
    M2C_FIELD(lbl_803BA330, f32 *, 0x18) = (f32) M2C_FIELD(temp_r31_14, f32 *, 0xC);
}
