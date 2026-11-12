/* fn_80100C30: Nov 6, 2026 hand draft (m2c 89%): the first player's bit is `0x80000000 >> var_r31` with var_r31 = 0 (m2c folded it to >> 0, a plain oris), the loadouts compared as records (`cr_g_BattleLoadouts[0].unk_0 == cr_g_BattleLoadouts[i].unk_0`, `[i].unk_0 != 0`), and the team byte stored through an s8 field (the original loads the source byte first) */

void fn_80100C30(void) {
    u8 var_r31;
    u8 var_r30;

    var_r31 = 0;
    lbl_8044F4F8 = 0;
    if (M2C_FIELD(lbl_80407250, u16 *, 2) & 1) {
        M2C_FIELD(lbl_80407260, s32 *, 0x498C) |= 0x200;
        lbl_8044F4F8 |= 0x80000000U >> var_r31;
        if (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x10) {
            M2C_FIELD(lbl_80407260, s32 *, 0x498C) |= 0x4000;
        }
    }
    if (M2C_FIELD(lbl_80407250, u16 *, 2) & 2) {
        var_r31 = 1;
        while (var_r31 < (u32) lbl_8044F504) {
            if (cr_g_BattleLoadouts[0].unk_0 == cr_g_BattleLoadouts[var_r31].unk_0) {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) |= 0x200;
                lbl_8044F4F8 |= 0x80000000U >> var_r31;
                if (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x20) {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) |= 0x4000;
                }
            }
            var_r31 += 1;
        }
    }
    if (M2C_FIELD(lbl_80407250, u16 *, 2) & 4) {
        var_r31 = 1;
        while (var_r31 < (u32) lbl_8044F504) {
            if (cr_g_BattleLoadouts[var_r31].unk_0 != 0) {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) |= 0x200;
                lbl_8044F4F8 |= 0x80000000U >> var_r31;
                if (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x40) {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) |= 0x4000;
                }
            }
            var_r31 += 1;
        }
    }
    var_r31 = 0;
    while (var_r31 < (u32) lbl_8044F504) {
        if (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) & 0x200) {
            var_r30 = 0;
            M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) |= (4 << M2C_FIELD(&lbl_80407250[var_r31], u8 *, 8));
            for (; var_r30 < (u32) lbl_8044F504; var_r30 += 1) {
                if ((s32) M2C_FIELD(&lbl_80407250[var_r31], u8 *, 8) == M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s8 *, 0x46C2)) {
                    break;
                }
            }
            if (var_r30 == (u32) lbl_8044F504) {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x46C2) = M2C_FIELD(&lbl_80407250[var_r31], u8 *, 8);
            } else {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x46C2) = -1;
            }
        }
        var_r31 += 1;
    }
}
