/* fn_80045CE8 wip 94% (m2c 74%): the magma action; file statics for the view vectors, `E0 = E2 = 0x1E`. Left: the registers of the +0x8C flag-table index (the original takes r5 / r6 / r7 and loads the robo twice) and the countdown store before its extsh */
s32 fn_80045CE8(void) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;

    fn_8001CEF4(cr_g_CurrentBattleRobo + 0x1074, &sp.sp8);
    if (((void *) M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x1E04) != NULL) && (M2C_FIELD(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x1E04), u32 *, 0x90) == (u32) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x1E08))) {
        fn_800784B0(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x1E04), &sp.sp8, NULL, NULL, NULL);
    }
    *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 4;
    if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) > 0) {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            sp.sp8 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
            sp.spC = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
            sp.sp10 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
            fn_8001CA28(&sp.sp8, cr_g_CurrentBattleRobo + 0x104, lbl_804506D0 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33428));
            sp.spC = lbl_80450638;
            if (cr_Vec3_LengthInline(&sp.sp8) > (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33426))) {
                cr_Vec3_Normalize(&sp.sp8);
                fn_8001CA5C(&sp.sp8, lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33426));
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) = sp.sp8;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) = sp.sp10;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) -= lbl_8044F4B4;
            fn_8001CCC4(&sp.sp8, lbl_80450638, lbl_80450638, lbl_80450638);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450778;
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
        }
        cr_g_BattleMotionSlot = 0x1E;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
    } else if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) >= lbl_80450654) || (cr_g_GameMode.flags & 0x100000)) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0x10;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0x1E;
        fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0xE6) = (s8) cr_g_BattleParams.unk_08[3];
        fn_80098290(0x3007C);
    } else {
        cr_g_BattleMotionSlot = 0x1E;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 2;
    }
    cr_s_9B60_A60 = cr_s_9B60_A6C;
    cr_s_9B60_A78 = cr_s_9B60_A60;
    return 1;
}
