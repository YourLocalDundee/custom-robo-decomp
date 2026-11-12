/* fn_800456B8: Nov 6, 2026 (m2c 9%: the pool was the whole difference): action 0 (stance, slot 0); motion frame + 1 per game frame; the view vectors a78 = a6c, a60 = a78 (file statics) */

s32 fn_800456B8(void) {
    cr_g_BattleMotionSlot = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450654;
    cr_s_9B60_A78 = cr_s_9B60_A6C;
    cr_s_9B60_A60 = cr_s_9B60_A78;
    return 1;
}
