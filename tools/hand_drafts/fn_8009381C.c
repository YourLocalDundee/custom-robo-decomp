/* fn_8009381C: Nov 13, 2026 hand draft: the four goto loops are while loops with a break (b test ... blt body); the byte copy without m2c's (u8) cast; rank_solve.py order */

u8 fn_8009381C(u8 *arg0) {
    u32 sp14;
    s32 sp10;
    u32 spC;
    s32 sp8;
    void *var_r30;
    u32 var_r29;
    u8 *var_r28;
    s32 var_r27;
    u32 var_r26;
    u32 var_r25;
    u8 *var_r24;
    u8 *var_r23;
    u32 var_r22;
    u32 var_r21;
    s32 var_r20;
    u32 var_r19;
    u32 var_r18;
    s32 var_r17;

    (void) arg0; (void) arg0; (void) arg0; (void) arg0; (void) arg0; 
    (void) var_r30; (void) var_r30; (void) var_r30; (void) var_r30; (void) var_r30; (void) var_r30; 
    (void) var_r28; (void) var_r28; 
    (void) var_r25; 
    var_r26 = 0;
    var_r28 = NULL;
    if (M2C_FIELD(arg0, u32 *, 0) == 7U) {
        M2C_FIELD(arg0, u32 *, 0) += 1;
        M2C_FIELD(arg0, u32 *, 0) -= 1;
    }
    var_r25 = 0U;
    var_r21 = 0U;
    var_r27 = 0;
    spC = M2C_FIELD(arg0, u32 *, 0);
    sp10 = 0;
    while (var_r27 < 0x100) {
        if (((u8 *) (M2C_FIELD((lbl_8039649C + (var_r27 * 0x74)), u32 *, 0x68)) != (u8 *) NULL) && (M2C_FIELD((lbl_8039649C + (var_r27 * 0x74)), u32 *, 0x68) == (u32) (lbl_8039649C + (var_r27 * 0x74)))) {
            break;
        }
        var_r27 += 1;
    }
    if (var_r27 == 0x100) {
        var_r27 = -1;
    }
    sp8 = var_r27;
    var_r17 = sp8;
    if (var_r17 == -1) {
        var_r18 = 0U;
    } else {
        var_r20 = 0;
        var_r30 = lbl_8039649C + (var_r17 * 0x74);
        while (var_r20 < 0x100) {
            if ((u8 *) (var_r30) == (u8 *) NULL) {
                break;
            }
            {
                if (spC == (u32) M2C_FIELD(var_r30, u32 *, 0x60)) {
                    if (M2C_FIELD(var_r30, s32 *, 0) & 1) {
                        var_r25 = M2C_FIELD(var_r30, u32 *, 0x64);
                    }
                    if (var_r25 > var_r21) {
                        var_r21 = var_r25;
                    }
                }
                var_r30 = M2C_FIELD(var_r30, void **, 0x6C);
                var_r20 += 1;
            }
        }
        var_r18 = var_r21;
    }
    sp14 = var_r18;
    if (M2C_FIELD(arg0, u32 *, 0) == 7U) {
        M2C_FIELD(arg0, u32 *, 0) += 1;
        M2C_FIELD(arg0, u32 *, 0) -= 1;
    }
    if (sp14 != (u32) M2C_FIELD(arg0, u32 *, 4)) {
        return 0U;
    }
    var_r29 = 0;
    while (var_r29 < 0x40U) {
        fn_80023394((signed char *) (lbl_80391544 + (var_r29 << 5)), 0x20U);
        var_r29 += 1;
    }
    var_r29 = 0;
    while (var_r29 < 0x40U) {
        if ((M2C_FIELD(arg0, u32 *, 0) == (u32) M2C_FIELD((lbl_80391D44 + (var_r29 * 0x34)), u32 *, 0x2C)) && (M2C_FIELD((lbl_80391D44 + (var_r29 * 0x34)), u32 *, 0) == (u32) (lbl_80391D44 + (var_r29 * 0x34)))) {
            var_r22 = 0U;
            var_r28 = lbl_80391D44 + (var_r29 * 0x34);
            while (var_r22 < 0x40U) {
                if ((u8 *) (var_r28) == (u8 *) NULL) {
                    break;
                }
                {
                    if (M2C_FIELD(var_r28, u16 *, 0x28) == 0xFFFF) {
                        var_r24 = var_r28 + 8;
                        var_r23 = lbl_80391544 + (var_r26 << 5);
                        var_r19 = 0U;
                        while (var_r19 < 0x20U) {
                            *var_r23 = *var_r24;
                            var_r24 += 1;
                            var_r19 += 1;
                            var_r23 += 1;
                        }
                        var_r26 += 1;
                        M2C_FIELD(var_r28, u16 *, 0x28) = 0U;
                    }
                    var_r28 = M2C_FIELD(var_r28, void **, 4);
                    var_r22 += 1;
                }
            }
        }
        var_r29 += 1;
    }
    M2C_FIELD((lbl_80391544 + (var_r26 << 5)), u16 *, 0x10) = 0xFFFF;
    if (var_r26 == 0) {
        return 0U;
    }
    return 1U;
}
