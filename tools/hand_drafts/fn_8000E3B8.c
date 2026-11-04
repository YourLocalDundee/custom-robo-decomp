/* fn_8000E3B8: Nov 3, 2026 hand draft (m2c 94%, then decomp-permuter): the 0x400000 mode flag set after the team flags, next to the 0x01000000 one */

void fn_8000E3B8(void) {
    s32 var_r31;
    u32 var_r30;
    u32 var_r29;

    var_r31 = 0x80000000;
    var_r31 = var_r31 >> 1;
    lbl_8044F504 = 2;
    cr_g_GameMode.battle_mode = 0;
    var_r29 = lbl_8044F504;
    lbl_8044F500 = (u32) var_r31;
    cr_g_BattleLoadouts->unk_0 = 0;
    cr_g_BattleLoadouts[1].unk_0 = 1;
    cr_g_BattleLoadouts->unk_e = 0;
    cr_g_BattleLoadouts[1].unk_e = 1;
    if (lbl_8044F366 != 0) {
        cr_g_GameMode.unk_14 = lbl_8044F366;
        if (lbl_8044F366 == -1U) {
            cr_g_GameMode.flags |= 0x80000000;
        } else {
            cr_g_GameMode.flags &= 0x7FFFFFFF;
        }
    } else {
        cr_g_GameMode.flags &= 0x7FFFFFFF;
        if (var_r29 == 2) {
            cr_g_GameMode.unk_14 = 0x1C20;
        } else if ((var_r29 == 3) || (var_r29 == 4)) {
            cr_g_GameMode.unk_14 = 0x2A30;
        } else {
            cr_g_GameMode.unk_14 = 0x1518;
        }
    }
    fn_800F6A44(7U);
    cr_g_BattleLoadouts->unk_e = 0;
    var_r30 = lbl_8044F504;
    cr_g_BattleLoadouts[1].unk_e = 0;
    M2C_FIELD(cr_g_BattleRobos, s32 *, 0x336F8) |= 0x80000000;
    cr_g_GameMode.flags |= 0x400000;
    cr_g_GameMode.flags |= 0x01000000;
    if (lbl_8044F366 != 0) {
        cr_g_GameMode.unk_14 = lbl_8044F366;
        if (lbl_8044F366 == -1U) {
            cr_g_GameMode.flags |= 0x80000000;
        } else {
            cr_g_GameMode.flags &= 0x7FFFFFFF;
        }
    } else {
        cr_g_GameMode.flags &= 0x7FFFFFFF;
        if (var_r30 == 2) {
            cr_g_GameMode.unk_14 = 0x1C20;
        } else if ((var_r30 == 3) || (var_r30 == 4)) {
            cr_g_GameMode.unk_14 = 0x2A30;
        } else {
            cr_g_GameMode.unk_14 = 0x1518;
        }
    }
    fn_800F6A44(7U);
}
