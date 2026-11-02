/* fn_80086F54: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the three words copied from lbl_801C0C8C as one 12-byte struct assignment (the table address formed once, lwz x3 / stw x3; m2c: three word copies, each addressing it again) */
typedef struct cr_Copy0C {
    u32 w[3];
} cr_Copy0C;

void fn_80086F54(void *arg0) {
    cr_Copy0C sp8;
    s8 var_r30;
    s8 var_r29;
    s8 var_r28;
    u32 var_r27;
    s8 var_r26;
    s8 var_r25;
    s8 var_r24;

    var_r26 = 1;
    var_r25 = 1;
    var_r24 = 1;
    sp8 = *(cr_Copy0C *) lbl_801C0C8C;
    var_r27 = fn_8001B3F0() >> M2C_FIELD(arg0, u8 *, 0x2B7);
    if (var_r27 & 1) {
        var_r26 = -1;
    }
    var_r27 = fn_8001B3F0() >> M2C_FIELD(arg0, u8 *, 0x2B7);
    if (var_r27 & 1) {
        var_r25 = -1;
    }
    var_r27 = fn_8001B3F0() >> (M2C_FIELD(arg0, u8 *, 0x2B7) + 1);
    if (var_r27 & 1) {
        var_r24 = -1;
    }
    var_r30 = (s8) (fn_8001B3F0() >> M2C_FIELD(arg0, u8 *, 0x2B7));
    var_r29 = (s8) (fn_8001B3F0() >> M2C_FIELD(arg0, u8 *, 0x2B7));
    var_r28 = (s8) (fn_8001B3F0() >> M2C_FIELD(arg0, u8 *, 0x2B7));
    if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x80000000)) {
        var_r30 &= 7;
        var_r29 &= 7;
        var_r28 &= 7;
    } else {
        var_r30 &= 0;
        var_r29 &= 0;
        var_r28 &= 0;
    }
    M2C_FIELD(arg0, f32 *, 0x278) = (f32) ((0.5f * (f32) (var_r30 * var_r26)) + M2C_FIELD(arg0, f32 *, 0x278));
    M2C_FIELD(arg0, f32 *, 0x27C) = (f32) ((0.5f * (f32) (var_r29 * var_r25)) + M2C_FIELD(arg0, f32 *, 0x27C));
    M2C_FIELD(arg0, f32 *, 0x280) = (f32) ((0.5f * (f32) (var_r28 * var_r24)) + M2C_FIELD(arg0, f32 *, 0x280));
    M2C_FIELD(arg0, f32 *, 0x2D8) = (f32) (var_r30 * var_r26);
    M2C_FIELD(arg0, f32 *, 0x2DC) = (f32) (var_r29 * var_r25);
    M2C_FIELD(arg0, f32 *, 0x2E0) = (f32) (var_r28 * var_r24);
}
