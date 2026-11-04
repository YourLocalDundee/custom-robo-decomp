/* fn_80050388: Nov 3, 2026 hand draft (m2c 95%): in the reset of the best-hp list the count and the first entry are cleared before the robo mask is read */

s32 fn_80050388(void) {
    u32 var_r31;
    u32 var_r30;
    s32 var_r29;
    u32 var_r28;
    s32 var_r27;
    u32 var_r26;

    var_r29 = 0;
    var_r26 = -1;
    var_r27 = 0;
    var_r28 = 0;
    if (!(cr_g_GameMode.flags & 0x01000000)) {
        var_r30 = lbl_8044F500;
        while ((s32) var_r30 != 0) {
            var_r31 = __cntlzw(var_r30);
            if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200)) {
                var_r26 = var_r31;
                var_r29 += 1;
                if ((u32) cr_g_BattleRobos[var_r31].hp > var_r28) {
                    var_r28 = (u32) cr_g_BattleRobos[var_r31].hp;
                    lbl_8044F498 = 0;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                } else if (var_r28 == (u32) cr_g_BattleRobos[var_r31].hp) {
                    lbl_8044F498 += 1;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                }
            }
            var_r30 ^= 0x80000000U >> var_r31;
        }
        if (var_r29 == 1) {
            var_r27 = 1;
            cr_g_GameMode.unk_16 = (s16) var_r26;
        } else if (var_r29 == 0) {
            var_r27 = 1;
            cr_g_GameMode.unk_16 = -1;
        }
    } else if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) & 0x200) {
        var_r27 = 1;
        cr_g_GameMode.unk_16 = 1;
    } else {
        var_r28 = (u32) cr_g_BattleRobos->hp;
        lbl_8044F498 = 0;
        lbl_801F8F14[lbl_8044F498] = 0;
        var_r30 = lbl_8044F500 ^ 0x80000000;
        while ((s32) var_r30 != 0) {
            var_r31 = __cntlzw(var_r30);
            if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200)) {
                var_r29 += 1;
                if ((u32) cr_g_BattleRobos[var_r31].hp > var_r28) {
                    var_r28 = (u32) cr_g_BattleRobos[var_r31].hp;
                    lbl_8044F498 = 0;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                } else if (var_r28 == (u32) cr_g_BattleRobos[var_r31].hp) {
                    lbl_8044F498 += 1;
                    lbl_801F8F14[lbl_8044F498] = var_r31;
                }
            }
            var_r30 ^= 0x80000000U >> var_r31;
        }
        if (var_r29 == 0) {
            var_r27 = 1;
            cr_g_GameMode.unk_16 = 0;
        }
    }
    if (var_r27 == 0) {
        if ((s32) lbl_8044F498 == 0) {
            cr_g_GameMode.unk_16 = (s16) *lbl_801F8F14;
        } else {
            cr_g_GameMode.unk_16 = -1;
        }
    }
    return var_r27;
}
