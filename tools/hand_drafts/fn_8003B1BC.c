/* fn_8003B1BC: Nov 5, 2026 hand draft: the robo pointer before the xor; 1 dead use of var_r26 in an `if (0)` block settles the r25 / r26 order of the original */

typedef struct { u8 pad[0x214]; f32 dist[4]; } cr_RoboDist;
void fn_8003B1BC(void) {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f25;
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;
    u8 *var_r28;
    cr_BattleRobo *var_r27;
    cr_BattleRobo *var_r26;
    u32 var_r25;
    if (0) {
        var_r26 = var_r26;
    }

    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r29 = __cntlzw(var_r30);
        var_r31 = 0;
        var_r27 = &cr_g_BattleRobos[var_r29];
        var_r28 = &var_r27->unk_001d6[0x3E];
        while (var_r31 < 4) {
            var_r31 += 1;
            M2C_FIELD(var_r28, f32 *, 0) = (f32) lbl_80450690;
            var_r28 += 4;
        }
        var_r30 ^= 0x80000000U >> var_r29;
    }
    var_r30 = lbl_8044F4F4;
    while (var_r30 != 0) {
        var_r29 = __cntlzw(var_r30);
        var_r27 = &cr_g_BattleRobos[var_r29];
        var_r30 ^= 0x80000000U >> var_r29;
        var_r25 = var_r30;
        var_f28 = M2C_FIELD(var_r27, f32 *, 0x1080);
        var_r28 = &var_r27->unk_001d6[0x3E];
        var_f27 = M2C_FIELD(var_r27, f32 *, 0x10A0);
        while (var_r25 != 0) {
            var_r31 = __cntlzw(var_r25);
            var_r26 = &cr_g_BattleRobos[var_r31];
            if (var_r29 > var_r31) {
                M2C_FIELD((var_r28 + (var_r31 * 4)), f32 *, 0) = ((cr_RoboDist *) var_r26)->dist[var_r29];
            } else {
                var_f26 = M2C_FIELD(var_r26, f32 *, 0x1080);
                var_f25 = M2C_FIELD(var_r26, f32 *, 0x10A0);
                var_f31 = var_f26 - var_f28;
                var_f30 = var_f25 - var_f27;
                var_f29 = (var_f30 * var_f30) + (var_f31 * var_f31);
                M2C_FIELD((var_r28 + (var_r31 * 4)), f32 *, 0) = var_f29;
                ((cr_RoboDist *) &cr_g_BattleRobos[var_r31])->dist[var_r29] = var_f29;
            }
            var_r25 ^= 0x80000000U >> var_r31;
        }
    }
}
