/* fn_8003B37C: Nov 13, 2026 hand draft: the angle read as ((s16 *) ((u8 *) robo + i * 2))[0x106]: the constant index on the computed pointer keeps i * 2 + 0x20C as the index (lhax); dead uses from the rank sweep */

void fn_8003B37C(void) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    s32 var_r31;
    u32 var_r30;
    u8 *var_r29;
    s32 var_r28;
    cr_BattleRobo *var_r27;
    cr_BattleRobo *var_r26;
    u32 var_r25;

    (void) var_r29; 
    (void) var_r26; 
    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r28 = __cntlzw(var_r30);
        var_r31 = 0;
        var_r27 = &cr_g_BattleRobos[var_r28];
        var_r29 = &var_r27->unk_001d6[0x36];
        while (var_r31 < 4) {
            var_r31 += 1;
            M2C_FIELD(var_r29, s16 *, 0) = 0;
            var_r29 += 2;
        }
        var_r30 ^= 0x80000000U >> var_r28;
    }
    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r28 = __cntlzw(var_r30);
        var_r27 = &cr_g_BattleRobos[var_r28];
        var_r30 ^= 0x80000000U >> var_r28;
        var_r25 = var_r30;
        var_f31 = M2C_FIELD(var_r27, f32 *, 0x1080);
        var_r29 = &var_r27->unk_001d6[0x36];
        var_f30 = M2C_FIELD(var_r27, f32 *, 0x10A0);
        while (var_r25 != 0) {
            var_r31 = __cntlzw(var_r25);
            var_r26 = &cr_g_BattleRobos[var_r31];
            if (var_r28 > var_r31) {
                M2C_FIELD((var_r29 + (var_r31 * 2)), s16 *, 0) = ((s16 *) ((u8 *) var_r26 + (var_r28 * 2)))[0x106] - 0x8000;
            } else {
                var_f29 = M2C_FIELD(var_r26, f32 *, 0x1080);
                var_f28 = M2C_FIELD(var_r26, f32 *, 0x10A0);
                M2C_FIELD((var_r29 + (var_r31 * 2)), s16 *, 0) = fn_8001F060(var_f31 - var_f29, var_f30 - var_f28);
                M2C_FIELD(((u8 *) &cr_g_BattleRobos[var_r31] + (var_r28 * 2)), s16 *, 0x20C) = (s16) (M2C_FIELD((var_r29 + (var_r31 * 2)), s16 *, 0) - 0x8000);
            }
            var_r25 ^= 0x80000000U >> var_r31;
        }
    }
}
