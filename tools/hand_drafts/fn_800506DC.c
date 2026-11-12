/* fn_800506DC: Nov 6, 2026 hand draft (m2c 86%): two statements moved (adjacent-swap sweep), and the last two bit loops are `for (mask = lbl_8044F4F8; mask != 0; mask ^= 0x80000000 >> i) { i = __cntlzw(mask); ... }`: the clear comes after the body (m2c's while put it first, which changed the register order) */

void fn_800506DC(void) {
    u32 var_r31;
    s32 var_r30;
    s32 var_r29;
    s32 var_r28;

    var_r31 = lbl_8044F4F8;
    while ((s32) var_r31 != 0) {
        var_r30 = __cntlzw(var_r31);
        M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) &= 0xFFFFFFEF;
        var_r31 ^= 0x80000000U >> var_r30;
    }
    if (cr_g_GameMode.flags & 0x10000) {
        if ((u32) ((cr_g_GameMode.flags & 0x60000) + 0xFFFA0000) != 0U) {
            var_r29 = 0;
            if ((u32) ((cr_g_GameMode.flags & 0x60000) + 0xFFFC0000) == 0U) {
                var_r30 = 0;
                while (var_r30 < 4) {
                    var_r29 = 0;
                    var_r31 = lbl_8032A614[var_r30];
                    while ((s32) var_r31 != 0) {
                        var_r28 = __cntlzw(var_r31);
                        var_r29 += 1;
                        var_r31 ^= 0x80000000U >> var_r28;
                    }
                    if (var_r29 >= 2) {
                        var_r31 = lbl_8044F4F8;
                        while ((s32) var_r31 != 0) {
                            var_r28 = __cntlzw(var_r31);
                            if (var_r30 == (s32) cr_g_BattleRobos[var_r28].unk_0008d) {
                                M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r28])), s32 *, 0x1F4) |= 0x10;
                            }
                            var_r31 ^= 0x80000000U >> var_r28;
                        }
                    }
                    var_r30 += 1;
                }
                return;
            }
            var_r31 = lbl_8044F4F8;
            while ((s32) var_r31 != 0) {
                var_r30 = __cntlzw(var_r31);
                if (!(M2C_FIELD(&cr_g_BattleRobos[var_r30], s32 *, 0x1D8) & 0x200)) {
                    var_r29 += 1;
                }
                var_r31 ^= 0x80000000U >> var_r30;
            }
            if (!(cr_g_GameMode.flags & 0x60000)) {
                if (var_r29 > 2) {
                    var_r31 = lbl_8044F4F8;
                    while ((s32) var_r31 != 0) {
                        var_r30 = __cntlzw(var_r31);
                        M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) |= 0x10;
                        var_r31 ^= 0x80000000U >> var_r30;
                    }
                }
            } else if (var_r29 > 3) {
                for (var_r31 = lbl_8044F4F8; (s32) var_r31 != 0; var_r31 ^= 0x80000000U >> var_r30) {
                    var_r30 = __cntlzw(var_r31);
                    M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) |= 0x10;
                }
            }
        } else {
            for (var_r31 = lbl_8044F4F8; (s32) var_r31 != 0; var_r31 ^= 0x80000000U >> var_r30) {
                var_r30 = __cntlzw(var_r31);
                M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[var_r30])), s32 *, 0x1F4) |= 0x10;
            }
        }
    }
}
