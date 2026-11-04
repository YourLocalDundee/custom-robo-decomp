/* fn_80005BC8: Nov 3, 2026 hand draft (m2c 64%): two expansions of one input reset, each with its own u16 zero local (the saved registers r31 / r30; +0x23E is stored from it, not from a literal), the second through a Vec on the stack */
void fn_80005BC8(void) {
    if (((u32) cr_g_SeqCurrent.unk_10 == 2) && (lbl_8041E938[3] == 0)) {
        if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x200)) {
            if (!(M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x1D8) & 0x80000000)) {
                fn_800DBAD0();
                fn_800057B8();
            } else {
                fn_800DBAD0();
            }
            fn_8000860C(M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242));
        } else {
            u16 buttons = 0;

            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x23E) = buttons;
            M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) = buttons & ~M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240);
            M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x244) = M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) & ~buttons;
            M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) = buttons;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104) = lbl_804501B8;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108) = lbl_804501B8;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C) = lbl_804501B8;
        }
    } else {
        u16 buttons = 0;
        Vec stick;

        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x23E) = buttons;
        stick.x = lbl_804501B8;
        stick.y = lbl_804501B8;
        stick.z = lbl_804501B8;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) = buttons & ~M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240);
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x244) = M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) & ~buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) = buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104) = stick.x;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108) = stick.y;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C) = stick.z;
    }
}
