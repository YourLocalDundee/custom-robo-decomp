/* fn_8005CF0C: Nov 13, 2026 hand draft: a local f32 table[5][3] with an initialiser (the compiler copies the 15 words from its own constant, which stands for lbl_801C0A28): the offset of the gun kind (kind - 0x18) picks the row copied into the robo */

void fn_8005CF0C(void) {
    f32 tbl[5][3] = { { 0.0f, 0.0f, -2.2f }, { 0.0f, 0.0f, -2.2f }, { 0.0f, 0.0f, 2.2f }, { 0.0f, 0.0f, 6.0f }, { 0.0f, 0.0f, -6.0f } };
    s32 i;

    i = (u8) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x33498) - 0x18;
    if ((i >= 0) && (i < 5) && (M2C_FIELD(lbl_8037569C, u8 *, 0x25BC) != 0)) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0) = tbl[i][0];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4) = tbl[i][1];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8) = tbl[i][2];
    }
}
