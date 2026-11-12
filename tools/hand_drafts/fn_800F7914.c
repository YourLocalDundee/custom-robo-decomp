/* fn_800F7914: Nov 6, 2026 hand draft (m2c 98%): the parameters used as they are (m2c copied arg0 / arg1 into var_r28 / var_r29, which spilled them to the stack), plus 1 dead use of arg0 in an `if (0)` block so it keeps r28 */

u8 fn_800F7914(void *arg0, u16 arg1, u32 arg2) {
    u8 var_r30;

    if (0) {
        arg0 = arg0;
    }
    var_r30 = 0;
    if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x1D8) & 0x80000000) {
        return 1U;
    }
    if (arg1 == 0) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[0][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_BodyStats[arg2].unk_042[3] != 0) || (arg2 == 0x19))) {
            var_r30 = 0;
        }
    } else if (arg1 == 1) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[1][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_GunData[arg2].unk_040[0x60] != 0) || (arg2 == 0x21))) {
            var_r30 = 0;
        }
    } else if (arg1 == 2) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[2][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_BombData[arg2].unk_040[0x60] != 0) || (arg2 == 0x1F))) {
            var_r30 = 0;
        }
    } else if (arg1 == 3) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[3][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_PodData[arg2].unk_040[0xA0] != 0) || (arg2 == 0x23))) {
            var_r30 = 0;
        }
    } else if (arg1 == 4) {
        if (cr_Bits_Test(&cr_g_UnlockBits.parts[4][4], arg2) == 0) {
            var_r30 = 1;
        }
        if ((cr_g_GameMode.unk_43 == 0) && !(cr_g_GameMode.flags & 0x800000) && ((cr_g_LegData[arg2].unk_40[0x40] != 0) || (arg2 == 0xA))) {
            var_r30 = 0;
        }
    }
    return var_r30;
}
