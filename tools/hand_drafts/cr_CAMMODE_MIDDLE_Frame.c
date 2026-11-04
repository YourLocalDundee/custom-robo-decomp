/* cr_CAMMODE_MIDDLE_Frame: Nov 3, 2026 hand draft (m2c 91%): `arg0 = arg0;` first (param_self_assign), the "allowed" flag initialised to 0 at the top (m2c dropped the dead store), unk_44 tested without the (u8) cast; the hang on a bad camera count as for (;;) */

void cr_CAMMODE_MIDDLE_Frame(void *arg0) {
    u32 spC;
    u32 sp8;
    u8 var_r29_155;
    u8 var_r30_20;
    arg0 = arg0;

    var_r30_20 = 0;
    sp8 = lbl_80452A40;
    spC = lbl_80452A44;
    if (!(cr_g_SeqCurrent.unk_38 & 0x4000)) {
        var_r30_20 = 1;
        if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) & 0x200) {
            cr_g_CamSystem.unk_30 = 0;
            var_r30_20 = 0;
            M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
        } else if (cr_g_GameMode.battle_mode & 2) {
            if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x336F8) & 0x200) {
                cr_g_CamSystem.unk_30 = 0;
                var_r30_20 = 0;
                M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
            } else {
                var_r30_20 = 1;
            }
        }
        if (var_r30_20 == 0) {
            cr_g_CamSystem.unk_30 = 0;
            M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
        }
        if (((lbl_8044F4F0 == 1) || (cr_g_GameMode.flags & 0x09800000)) && (cr_g_GameMode.unk_44 != 0) && (var_r30_20 != 0)) {
            if ((s8) (u8) cr_g_Pads->substick_y > 0x14) {
                cr_g_CamSystem.unk_30 = 2;
                M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) |= 1;
            } else if ((s8) (u8) cr_g_Pads->substick_y < -0x14) {
                cr_g_CamSystem.unk_30 = 0;
                M2C_FIELD(cr_g_CamSystem.unk_34, s32 *, 0) &= 0xFFFFFFFE;
            }
        }
        if ((u8) cr_g_CamSystem.unk_30 != 2) {
            if ((lbl_8044F461 == 0) || (lbl_8044F461 >= 5U)) {
for (;;) {
                }
            }
            fn_8002B3C4(arg0);
            fn_8002E04C(arg0);
            if (!(cr_g_CamSystem.unk_2c & 2)) {
                var_r29_155 = 0;
                for (; var_r29_155 < 4U; var_r29_155 += 1) {
                    if (!(!(M2C_FIELD(&cr_g_BattleRobos[var_r29_155], s32 *, 0x1D8) & 0x10000000))) {
                        break;
                    }
                }
                if (var_r29_155 == 4) {
                    cr_g_CamSystem.unk_2c |= 2;
                }
            }
            goto block_29;
        }
        fn_80023F0C(arg0);
block_29:
        if (cr_g_CamSystem.unk_2c & 2) {
            if (M2C_FIELD(arg0, u32 *, 0x114) > 9U) {
                M2C_FIELD(arg0, u32 *, 0x114) -= 9;
                return;
            }
            M2C_FIELD(arg0, u32 *, 0x114) += 0xFFF7;
        }
    }
}
