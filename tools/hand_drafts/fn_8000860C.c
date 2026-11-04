/* fn_8000860C: Nov 3, 2026 hand draft (93% by hand, then decomp-permuter): the hit flag (arg0 & 0x3070) computed before the first counter is bumped */

void fn_8000860C(s32 arg0) {
    void *temp_r31_16;
    s8 temp_r0_29;

    temp_r31_16 = lbl_8024DB20 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x58);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x800) = (f32) lbl_804501B8;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x804) = (f32) lbl_804501B8;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x808) = (f32) lbl_804501B8;
    if (M2C_FIELD(temp_r31_16, s8 *, 0) != 0) {
        temp_r0_29 = (arg0 & 0x3070) != 0;
        M2C_FIELD(temp_r31_16, s32 *, 8) += 1;
        if (temp_r0_29 != 0) {
            M2C_FIELD(temp_r31_16, s32 *, 0xC) += 1;
        }
        M2C_FIELD(temp_r31_16, s8 *, 0x10) = 0;
        if ((M2C_FIELD(temp_r31_16, s32 *, 0xC) > 0) && (M2C_FIELD(temp_r31_16, s32 *, 8) >= 5)) {
            M2C_FIELD(temp_r31_16, s8 *, 0x10) = 1;
            M2C_FIELD(temp_r31_16, s32 *, 0xC) -= 1;
            M2C_FIELD(temp_r31_16, s32 *, 8) = 0;
        }
        M2C_FIELD(temp_r31_16, s32 *, 0x14) += 1;
        if (temp_r0_29 != 0) {
            M2C_FIELD(temp_r31_16, s32 *, 0x18) += 1;
        }
        M2C_FIELD(temp_r31_16, s8 *, 0x1C) = 0;
        if ((M2C_FIELD(temp_r31_16, s32 *, 0x18) > 0) && (M2C_FIELD(temp_r31_16, s32 *, 0x14) >= 0xA)) {
            M2C_FIELD(temp_r31_16, s8 *, 0x1C) = 1;
            M2C_FIELD(temp_r31_16, s32 *, 0x18) -= 1;
            M2C_FIELD(temp_r31_16, s32 *, 0x14) = 0;
        }
    }
    if ((s32) M2C_FIELD(temp_r31_16, u8 *, 0) == 1) {
        M2C_FIELD(temp_r31_16, u8 *, 0) = 0U;
        M2C_FIELD(temp_r31_16, s8 *, 0x10) = M2C_FIELD(temp_r31_16, s8 *, 0x1C) = 0;
        return;
    }
    if (((s32) M2C_FIELD(temp_r31_16, u8 *, 0) == 2) && (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x528) != 0xE)) {
        M2C_FIELD(temp_r31_16, u8 *, 0) = 0U;
        M2C_FIELD(temp_r31_16, s8 *, 0x10) = M2C_FIELD(temp_r31_16, s8 *, 0x1C) = 0;
    }
}
