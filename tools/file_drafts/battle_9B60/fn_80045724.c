/* fn_80045724: Nov 6, 2026 (m2c 62%): the lose action (slot 0x20; frame = 138.5 x (140 - countdown) / 140, the body's own animation timer +0x550 to 10); the closing view-vector copies are two file statics: a78 = a6c; a60 = a78 */
s32 fn_80045724(void) {
    cr_g_BattleMotionSlot = 0x20;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) > (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) ((lbl_8045076C * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0));
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) += lbl_80450654;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) >= lbl_80450680) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) = (f32) lbl_80450770;
    }
    cr_s_9B60_A78 = cr_s_9B60_A6C;
    cr_s_9B60_A60 = cr_s_9B60_A78;
    return 1;
}
