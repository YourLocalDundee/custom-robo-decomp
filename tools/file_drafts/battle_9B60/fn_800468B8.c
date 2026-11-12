/* fn_800468B8: Nov 6, 2026 (m2c 60%): the landing action (slot 0x0C, or 0x1A / 0x1C when +0x139 is 2; frame = 20 x elapsed / length); `E0 = E2 = 0` chained, the fall step as `-=`, the closing view-vector copies as file statics */
extern u8 *lbl_8044F4A4;

s32 fn_800468B8(void) {
    s32 var_r30_21;

    if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) > 0) {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) != 0) {
                var_r30_21 = 0x1C;
            } else {
                var_r30_21 = 0x1A;
            }
            cr_g_BattleMotionSlot = var_r30_21;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
        } else {
            cr_g_BattleMotionSlot = 0xC;
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) > 0) {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) ((lbl_80450788 * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0));
            } else {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
            }
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) -= lbl_804506A8 * (lbl_8044F4B4 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x1E));
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0;
    }
    cr_s_9B60_A60 = cr_s_9B60_A6C;
    cr_s_9B60_A78 = cr_s_9B60_A60;
    return 1;
}
