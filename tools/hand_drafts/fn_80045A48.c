/* fn_80045A48: Nov 6, 2026 hand draft (m2c 78%): the change action (action 18) counting down to action 19; the two halfword stores as one chained assignment `E0 = E2 = x` (m2c read the table twice: one more lis / addi) */
s32 fn_80045A48(void) {
    if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) > 0) {
        cr_g_BattleMotionSlot = 0x24;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) ((lbl_80450774 * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0));
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x146) = cr_g_BattleParams.tag_cooldown;
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0x13;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = cr_g_BattleParams.unk_1c[4][0];
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0xE6) = cr_g_BattleParams.unk_1c[4][1];
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1884) = lbl_80450654;
    return 1;
}
