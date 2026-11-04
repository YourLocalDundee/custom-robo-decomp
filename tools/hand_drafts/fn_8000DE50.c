/* fn_8000DE50: Nov 3, 2026 hand draft (m2c 86%): the robo count read into a local before the time-limit branch (r29: the original saves five registers) and the time limit chosen by an if chain on it (unsigned compares; m2c read a switch) */

void fn_8000DE50(void) {
    u8 var_r31_11;
    s32 var_r30_10;
    u32 n;

    var_r30_10 = 0x80000000;
    var_r31_11 = 0;
    lbl_8044F504 = 4;
    cr_g_GameMode.battle_mode = 2;
    while (var_r31_11 < 3U) {
        var_r30_10 >>= 1;
        var_r31_11 += 1;
    }
    lbl_8044F500 = (u32) var_r30_10;
    cr_g_BattleLoadouts->unk_0 = 0;
    cr_g_BattleLoadouts[1].unk_0 = 0;
    cr_g_BattleLoadouts[2].unk_0 = 1;
    cr_g_BattleLoadouts[3].unk_0 = 1;
    M2C_FIELD(cr_g_TagTeams, f32 *, 0x10) = (f32) lbl_8045031C;
    M2C_FIELD(cr_g_TagTeams, f32 *, 0x14) = (f32) lbl_8045031C;
    M2C_FIELD(cr_g_TagTeams, f32 *, 0x28) = (f32) lbl_8045031C;
    M2C_FIELD(cr_g_TagTeams, f32 *, 0x2C) = (f32) lbl_8045031C;
    switch (M2C_FIELD(&lbl_8044C3F4, s8 *, 2)) { /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        cr_g_BattleLoadouts->unk_e = 0;
        cr_g_BattleLoadouts[1].unk_e = 0;
        cr_g_BattleLoadouts[2].unk_e = 0;
        cr_g_BattleLoadouts[3].unk_e = 0;
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x66C18) |= 0x80000000;
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x9A138) |= 0x80000000;
        break;
    case 1:                                         /* switch 1 */
        cr_g_BattleLoadouts->unk_e = 0;
        cr_g_BattleLoadouts[1].unk_e = 0;
        cr_g_BattleLoadouts[2].unk_e = 1;
        cr_g_BattleLoadouts[3].unk_e = 1;
        break;
    case 2:                                         /* switch 1 */
        cr_g_BattleLoadouts->unk_e = 0;
        cr_g_BattleLoadouts[1].unk_e = 1;
        cr_g_BattleLoadouts[2].unk_e = 0;
        cr_g_BattleLoadouts[3].unk_e = 0;
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x66C18) |= 0x80000000;
        M2C_FIELD(cr_g_BattleRobos, s32 *, 0x9A138) |= 0x80000000;
        break;
    case 3:                                         /* switch 1 */
        cr_g_BattleLoadouts->unk_e = 0;
        cr_g_BattleLoadouts[1].unk_e = 1;
        cr_g_BattleLoadouts[2].unk_e = 2;
        cr_g_BattleLoadouts[3].unk_e = 2;
        break;
    case 4:                                         /* switch 1 */
        cr_g_BattleLoadouts->unk_e = 0;
        cr_g_BattleLoadouts[1].unk_e = 1;
        cr_g_BattleLoadouts[2].unk_e = 2;
        cr_g_BattleLoadouts[3].unk_e = 3;
        break;
    }
    n = lbl_8044F504;
    if (lbl_8044F366 != 0) {
        cr_g_GameMode.unk_14 = lbl_8044F366;
        if (lbl_8044F366 == -1U) {
            cr_g_GameMode.flags |= 0x80000000;
        } else {
            cr_g_GameMode.flags &= 0x7FFFFFFF;
        }
    } else {
        cr_g_GameMode.flags &= 0x7FFFFFFF;
        if (n == 2) {
            cr_g_GameMode.unk_14 = 0x1C20;
        } else if (n == 3 || n == 4) {
            cr_g_GameMode.unk_14 = 0x2A30;
        } else {
            cr_g_GameMode.unk_14 = 0x1518;
        }
    }
    fn_800F6A44(7U);
}
