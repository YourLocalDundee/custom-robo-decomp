/* fn_8008F6E8: Nov 6, 2026 hand draft (m2c 76%): search of the lbl_8039649C list (0x74-byte nodes) for the entry whose +0x60 field equals arg0; both search loops are for loops that break (m2c's goto loops and phi copies took extra saved registers), and arg0 is compared directly (m2c's stack copy sp8 = arg0 added a 'lwz 8(r1)' reload) */


void fn_8008F6E8(u32 arg0) {
    s8 *var_r31;
    s32 var_r30;
    s8 *var_r29;
    s8 *var_r28;
    s32 var_r27;
    u8 var_r26;
    s32 var_r25;
    s8 *var_r24;
    s32 var_r23;

    var_r26 = 0;
    var_r31 = NULL;
    var_r30 = 0;
    for (; var_r30 < 0x100; var_r30 += 1) {
        if (!(((u8 *) (M2C_FIELD((lbl_8039649C + (var_r30 * 0x74)), u32 *, 0x68)) == (u8 *) NULL) || (M2C_FIELD((lbl_8039649C + (var_r30 * 0x74)), u32 *, 0x68) != (u32) (lbl_8039649C + (var_r30 * 0x74))))) {
            break;
        }
    }
    if (var_r30 == 0x100) {
        var_r30 = -1;
    }
    var_r23 = var_r30;
    var_r25 = var_r23;
    if (var_r25 != -1) {
        var_r27 = 0;
        var_r31 = (signed char *) (lbl_8039649C + (var_r25 * 0x74));
        for (; var_r27 < 0x100; var_r27 += 1) {
            var_r26 = 0;
            if ((u8 *) (var_r31) == (u8 *) NULL) {
                break;
            }
            if (M2C_FIELD(var_r31, u32 *, 0x60) == arg0) {
                var_r28 = M2C_FIELD(var_r31, s8 **, 0x68);
                var_r24 = var_r31;
                var_r29 = M2C_FIELD(var_r31, s8 **, 0x6C);
                var_r26 = 1;
                if (var_r28 == var_r31) {
                    if ((u8 *) (var_r29) != (u8 *) NULL) {
                        M2C_FIELD(var_r29, s8 **, 0x68) = var_r29;
                    }
                } else if ((u8 *) (var_r29) != (u8 *) NULL) {
                    M2C_FIELD(var_r29, s8 **, 0x68) = var_r28;
                }
                M2C_FIELD(var_r28, s8 **, 0x6C) = var_r29;
            }
            var_r31 = M2C_FIELD(var_r31, s8 **, 0x6C);
            if (var_r26 == 1) {
                fn_80023394(var_r24, 0x74U);
                var_r24 = NULL;
            }
        }
    }
}
