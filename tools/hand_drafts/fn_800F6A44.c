/* fn_800F6A44: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the mode tested with an if / else-if chain on an u32 parameter (cmplwi per case; m2c: an irregular switch on an s32), the u16 slot tests without their casts (cmplwi), and `{ (void) 0; }` in the empty then-block of the default case (beq / b, an empty debug macro) */
void fn_800F6A44(u32 arg0) {
    u8 var_r31;
    u8 var_r30;

    var_r31 = 0;
    var_r30 = 0;
    M2C_FIELD(lbl_80407250, u16 *, 2) = arg0;
    while (var_r31 < 4U) {
        M2C_FIELD(&lbl_80407260[(var_r31 * 0x4994)], s16 *, 0x4990) = 0;
        var_r31 += 1;
    }
    if (arg0 == 0x84) {
        M2C_FIELD(lbl_80407250, u16 *, 2) |= 0xC0;
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        return;
    } else if (arg0 == 0x4) {
        M2C_FIELD(lbl_80407250, u16 *, 2) |= 0x40;
        var_r31 = 0;
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) && (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) != 0)) {
                var_r30 += 1;
            }
            var_r31 += 1;
        }
        var_r31 = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) && (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) != 0)) {
                if (var_r30 > 1U) {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x908;
                } else {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x900;
                }
            }
            var_r31 += 1;
        }
        return;
    } else if (arg0 == 0x2) {
        M2C_FIELD(lbl_80407250, u16 *, 2) |= 0x20;
        var_r31 = 0;
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if ((M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) && (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) == 0)) {
                var_r30 += 1;
            }
            var_r31 += 1;
        }
        var_r31 = 0;
        while (var_r31 < (u32) lbl_8044F504) {
            if (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) == 0) {
                if (var_r30 > 1U) {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x908;
                } else {
                    M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0x900;
                }
            }
            var_r31 += 1;
        }
        return;
    } else if (arg0 == 0x1) {
        lbl_80407250[8] = 0;
        lbl_80407250[9] = 0;
        lbl_80407250[0xA] = 0;
        lbl_80407250[0xB] = 0;
        M2C_FIELD(lbl_80407260, u16 *, 0x4990) |= 0xF75;
        return;
    } else if (arg0 == 0x3) {
        M2C_FIELD(lbl_80407260, u16 *, 0x4990) |= 0xD7D;
        M2C_FIELD(lbl_80407260, u16 *, 0x9324) |= 0xD7D;
        lbl_80407250[8] = (u8) cr_g_BattleLoadouts->unk_e;
        lbl_80407250[9] = (u8) cr_g_BattleLoadouts[1].unk_e;
        return;
    } else {
        lbl_80407250[8] = (u8) cr_g_BattleLoadouts->unk_e;
        var_r31 = 0;
        lbl_80407250[9] = (u8) cr_g_BattleLoadouts[1].unk_e;
        lbl_80407250[0xA] = (u8) cr_g_BattleLoadouts[2].unk_e;
        lbl_80407250[0xB] = (u8) cr_g_BattleLoadouts[3].unk_e;
        while (var_r31 < 4U) {
            if (M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x80000000) {
                (void) 0;
            } else {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u16 *, 0x4990) |= 0xCDD;
            }
            var_r31 += 1;
        }
        return;
    }
}
