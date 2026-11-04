/* fn_80023F0C: Nov 3, 2026 hand draft (m2c 86%): the search for the first live robo as a for loop over the bitmask (signed test), an int bit index, the update pointer tested against a cast NULL; battle_mode compared without the (u8) cast */

void fn_80023F0C(void *arg0) {
    u8 var_r31_0;
    u8 var_r30_74;
    s32 i;
    u32 var_r28_11;

    if (cr_g_GameMode.flags & 0x02000000) {
        for (var_r28_11 = lbl_8044F500; (s32) var_r28_11 != 0; var_r28_11 ^= 0x80000000U >> i) {
            i = __cntlzw(var_r28_11);
            if (!(M2C_FIELD(&cr_g_BattleRobos[i], s32 *, 0x1D8) & 0x80000000) && ((u8 *) cr_g_BattleRobos[i].update != (u8 *) NULL)) {
                var_r31_0 = i;
                break;
            }
        }
    } else if ((s32) cr_g_GameMode.battle_mode != 2) {
        var_r31_0 = 0;
    } else {
        var_r31_0 = cr_g_TagTeams->active;
    }
    if ((s8) cr_g_BattleRobos[var_r31_0].unk_001d6[0x52] != -1) {
        var_r30_74 = cr_g_BattleRobos[var_r31_0].unk_001d6[0x52];
    } else {
        var_r30_74 = 1;
    }
    M2C_FIELD(arg0, s32 *, 0x110) = 0;
    M2C_FIELD(arg0, f32 *, 0x98) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r31_0], f32 *, 0x10E0);
    M2C_FIELD(arg0, f32 *, 0x9C) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r31_0], f32 *, 0x10F0);
    M2C_FIELD(arg0, f32 *, 0xA0) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r31_0], f32 *, 0x1100);
    M2C_FIELD(arg0, f32 *, 0xC8) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r30_74], f32 *, 0x20);
    M2C_FIELD(arg0, f32 *, 0xCC) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r30_74], f32 *, 0x1480);
    M2C_FIELD(arg0, f32 *, 0xD0) = (f32) M2C_FIELD(&cr_g_BattleRobos[var_r30_74], f32 *, 0x28);
    M2C_FIELD(arg0, f32 *, 0xD4) = (f32) M2C_FIELD(arg0, f32 *, 0xC8);
    M2C_FIELD(arg0, f32 *, 0xD8) = (f32) M2C_FIELD(arg0, f32 *, 0xCC);
    M2C_FIELD(arg0, f32 *, 0xDC) = (f32) M2C_FIELD(arg0, f32 *, 0xD0);
    lbl_80321E90[var_r31_0] = 0;
}
