/* fn_80048E7C: Nov 6, 2026 (m2c 42%): the pod action (action 5, slot 0x12, 0x13 in the air); the view vector from the robo direction (+0x104, normalised) or the pool target +0xA48, fn_8003D7E0 fires the pod on the first frame; frame = 10 x elapsed / length / lbl_80450720; Vec local and file statics */

s32 fn_80048E7C(void) {
    Vec v;

    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        if (cr_Vec3_LengthInline(cr_g_CurrentBattleRobo + 0x104) > lbl_80450638) {
            v = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x104);
            cr_Vec3_Normalize(&v);
            cr_s_9B60_A6C = v;
            cr_s_9B60_A78 = cr_s_9B60_A6C;
            cr_s_9B60_A60 = cr_s_9B60_A78;
        } else {
            cr_s_9B60_A6C = cr_s_9B60_A48;
            cr_s_9B60_A78 = cr_s_9B60_A6C;
            cr_s_9B60_A60 = cr_s_9B60_A78;
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = (f32) (lbl_80450688 * (f32) -cr_g_BattleParams.unk_08[2]);
            cr_g_BattleMotionSlot = 0x13;
        } else {
            cr_g_BattleMotionSlot = 0x12;
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) == (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)) {
            fn_8003D7E0(cr_g_CurrentBattleRobo + 0x2E2E0, (u8 *) &cr_s_9B60_A60, M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2);
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (((lbl_80450680 * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0)) / lbl_80450720);
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1;
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
    }
    return 1;
}
