/* fn_80092A5C: Nov 5, 2026 hand draft (m2c 87%): a pad handler (states 1 and 4 of lbl_8044F66D, a 0 / 1 choice in lbl_8044F672); returns var_r30 (m2c: 1); the pad index is the parameter itself (no m2c alias); a folded local `id` (copied once into the first fn_80098290 call) holds the spare r29 of the original; and a bare compare of slot_index with no branch is `(c) ? (void) 0 : (void) 0` */

s32 fn_80092A5C(u8 arg0) {
    u8 var_r31;
    s32 var_r30;
    s32 id;

    var_r30 = 1;
    if (lbl_8044F66D == 1) {
        if (cr_g_Pads[arg0].unk_14 & 0x10) {
            id = (s32) lbl_8021A308[1];
            fn_80098290(id);
            lbl_8044F672 = 0;
            lbl_8044F66D = 2;
            lbl_8044F66F = 0;
        } else if (cr_g_Pads[arg0].unk_14 & 0x100) {
            switch ((s8) lbl_8044F672) {            /* switch 1; irregular */
            case 0:                                 /* switch 1 */
                fn_80098290((s32) lbl_8021A308[1]);
                break;
            case 1:                                 /* switch 1 */
                fn_80098290((s32) M2C_FIELD(lbl_8021A308, u32 *, 0));
                break;
            }
            lbl_8044F66D = 2;
            lbl_8044F66F = 0;
        }
    } else if (lbl_8044F66D == 4) {
        switch ((s8) lbl_8044F672) {                /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            lbl_8044F66D = 0;
            cr_g_SeqCurrent.unk_38 &= 0xFFFBFFFF;
            break;
        case 1:                                     /* switch 2 */
            lbl_8044F66D = 0;
            var_r31 = 0;
            cr_g_SeqCurrent.unk_10 = 9;
            while (var_r31 < 4U) {
                if (((void (*)()) cr_g_BattleRobos[var_r31].update) != NULL) {
                    cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[var_r31];
                    fn_800440BC(cr_g_CurrentBattleRobo + 0x1E20);
                    fn_8003F000(cr_g_CurrentBattleRobo + 0x2E2E0);
                    fn_800410C0(cr_g_CurrentBattleRobo + 0x28D40);
                }
                var_r31 += 1;
            }
            lbl_8044F44C |= 0x400;
            if (cr_g_GameMode.flags & 0x800000) {
                (cr_g_Scenario.slot_index == 0) ? (void) 0 : (void) 0; /* an assert without its message call */
                cr_g_Scenario.unk_02e |= 2;
                cr_g_Scenario.unk_02e |= 4;
            }
            break;
        }
    }
    return var_r30;
}
