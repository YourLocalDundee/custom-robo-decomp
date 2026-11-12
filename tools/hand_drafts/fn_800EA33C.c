/* fn_800EA33C: Nov 6, 2026 hand draft (m2c 85%): the inner loop index is mentioned first, as a (void) statement at the top, which gives it r30 and the count r29 (as the original) with no code */

s8 fn_800EA33C(void *arg0, s32 arg1) {
    u32 temp_r31_12;
    u32 var_r30_35;
    u8 var_r29_6;
    u32 var_r28_8;

    (void) var_r30_35;
    var_r29_6 = 0;
    var_r28_8 = lbl_8044F500;
    while (var_r28_8 != 0) {
        temp_r31_12 = __cntlzw(var_r28_8);
        if ((M2C_FIELD(arg0, u8 *, 0x6A) != temp_r31_12) && (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8D) == (u8) cr_g_BattleRobos[temp_r31_12].unk_0008d) && !(M2C_FIELD(&cr_g_BattleRobos[temp_r31_12], s32 *, 0x1D8) & 0x200)) {
            var_r30_35 = 0U;
            for (; var_r30_35 < var_r29_6; var_r30_35 += 1) {
                if (M2C_FIELD((arg1 + var_r30_35), s8 *, 0) == (s8) cr_g_BattleRobos[temp_r31_12].unk_001d6[0x52]) {
                    break;
                }
            }
            if (var_r30_35 == var_r29_6) {
                M2C_FIELD((arg1 + (u8) (var_r29_6)), u8 *, 0) = cr_g_BattleRobos[temp_r31_12].unk_001d6[0x52];
                var_r29_6 += 1;
            }
        }
        var_r28_8 ^= 0x80000000U >> temp_r31_12;
    }
    return (s8) var_r29_6;
}
