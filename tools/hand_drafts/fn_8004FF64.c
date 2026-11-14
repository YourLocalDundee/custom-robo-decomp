/* fn_8004FF64: Nov 13, 2026 hand draft: returns the decided flag (r3 = r28); the team totals a local u32[4] with a zero initialiser, added to with one compound assignment (the base and the scaled index are the compiler temporaries r25 / r24, the u32 += f32 conversions); the two team counts a u32[2] set by assignment; one dead use of the bit mask keeps it above the flag */

u32 fn_8004FF64(void) {
    u32 t0[4] = { 0, 0, 0, 0 };
    u32 cnt[2];
    u32 var_r31;
    u32 var_r30;
    u32 var_r29;
    u32 var_r28;
    u32 var_r27;
    u32 var_r26;

    (void) var_r29;
    var_r27 = -1;
    var_r30 = 0U;
    var_r28 = 0;
    var_r26 = 0;
    if (cr_g_GameMode.flags & 0x800000) {
        if (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) & 0x200) {
            var_r28 = 1;
            cr_g_GameMode.unk_16 = 1;
        } else {
            var_r31 = 0;
            while (var_r31 < 4U) {
                if (lbl_8032A614[var_r31] != 0) {
                    var_r27 = var_r31;
                    var_r30 += 1;
                }
                var_r31 += 1;
            }
            if (var_r30 == 1U) {
                var_r28 = 1;
                cr_g_GameMode.unk_16 = (s16) var_r27;
            } else if (var_r30 == 0U) {
                var_r28 = 1;
                cr_g_GameMode.unk_16 = -1;
            }
        }
    } else {
        if ((cr_g_GameMode.flags & 0x01000000) && (M2C_FIELD(cr_g_BattleRobos, s32 *, 0x1D8) & 0x200)) {
            var_r28 = 1;
            cr_g_GameMode.unk_16 = 1;
        }
        if (var_r28 == 0) {
            if (cr_g_GameMode.flags & 0x200000) {
                cnt[0] = 0U;
                var_r29 = lbl_8044F4F8;
                cnt[1] = 0U;
                while (var_r29 != 0) {
                    var_r31 = __cntlzw(var_r29);
                    if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200)) {
                        cnt[cr_g_BattleRobos[var_r31].unk_0008d] += 1;
                    }
                    var_r29 ^= 0x80000000U >> var_r31;
                }
                if (cnt[0] == (u32) (M2C_FIELD(lbl_8032A5E4, u32 *, 0) - 1)) {
                    var_r27 = 1;
                    var_r30 = 1U;
                } else if (cnt[1] == (u32) (lbl_8032A5E4[1] - 1)) {
                    var_r27 = 0;
                    var_r30 = 1U;
                }
                if (var_r30 == 1U) {
                    var_r28 = 1;
                    cr_g_GameMode.unk_16 = (s16) var_r27;
                }
            } else {
                var_r31 = 0;
                while (var_r31 < 4U) {
                    if (lbl_8032A614[var_r31] != 0) {
                        var_r27 = var_r31;
                        var_r30 += 1;
                    }
                    var_r31 += 1;
                }
                if (var_r30 == 1U) {
                    var_r28 = 1;
                    cr_g_GameMode.unk_16 = (s16) var_r27;
                } else if (var_r30 == 0U) {
                    var_r28 = 1;
                    cr_g_GameMode.unk_16 = -1;
                }
            }
        }
    }
    if (var_r28 == 0) {
        var_r29 = lbl_8044F4F8;
        while (var_r29 != 0) {
            var_r31 = __cntlzw(var_r29);
            if (!(M2C_FIELD(&cr_g_BattleRobos[var_r31], s32 *, 0x1D8) & 0x200)) {
                t0[cr_g_BattleRobos[var_r31].unk_0008d] += cr_g_BattleRobos[var_r31].hp;
            }
            var_r29 ^= 0x80000000U >> var_r31;
        }
        var_r31 = 0;
        while (var_r31 < 4U) {
            if ((u32) t0[var_r31] > var_r26) {
                lbl_8044F498 = 0;
                var_r26 = t0[var_r31];
                lbl_801F8F14[lbl_8044F498] = var_r31;
            } else if (var_r26 == (u32) t0[var_r31]) {
                lbl_8044F498 += 1;
                lbl_801F8F14[lbl_8044F498] = var_r31;
            }
            var_r31 += 1;
        }
        if ((s32) lbl_8044F498 == 0) {
            cr_g_GameMode.unk_16 = (s16) *lbl_801F8F14;
        }
    }
    return var_r28;
}
