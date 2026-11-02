/* fn_800DF018: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): in the case 3 branch the computed x (`sp8 = 2.0f * a + b`) written before the constant `spC = 0.0f`: main.dol loads 2.0f and the operands first and schedules the 0.0f store above the computed one, which m2c read as the source order */
void fn_800DF018(u8 *arg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    f32 var_f31;
    s8 var_r30;
    u16 var_r29;
    s8 var_r28;
    u16 var_r27;
    s8 var_r26;
    u16 var_r25;

    M2C_FIELD(arg0, s32 *, 0xC) |= 0x1000;
    if (M2C_FIELD(arg0, u16 *, 0x377A) == 1) {
        fn_8001B1A0(arg0 + 0x24, arg0 + 0x3C, arg0 + 0x18);
        cr_Vec3_Normalize(arg0 + 0x18);
    } else if (M2C_FIELD(arg0, u16 *, 0x377A) == 3) {
        sp.sp8 = (2.0f * M2C_FIELD(arg0, f32 *, 0x14)) + M2C_FIELD(arg0, f32 *, 0x24);
        sp.spC = 0.0f;
        var_r29 = (u16) (fn_8001B3F0() >> 4U);
        M2C_FIELD(arg0, f32 *, 0x14) = (f32) ((f32) var_r29 / 65535.0f);
        var_r30 = (fn_8001B3F0() >> 4U) & 1;
        if (var_r30 != 0) {
            M2C_FIELD(arg0, f32 *, 0x14) = (f32) ((-1.0f) * M2C_FIELD(arg0, f32 *, 0x14));
        }
        sp.sp10 = (2.0f * M2C_FIELD(arg0, f32 *, 0x14)) + M2C_FIELD(arg0, f32 *, 0x2C);
        fn_8001B1A0(arg0 + 0x24, &sp.sp8, arg0 + 0x18);
        cr_Vec3_Normalize(arg0 + 0x18);
    } else if (M2C_FIELD(arg0, u16 *, 0x377A) == 2) {
        var_r27 = (u16) (fn_8001B3F0() >> 4U);
        M2C_FIELD(arg0, f32 *, 0x14) = (f32) ((f32) var_r27 / 65535.0f);
        var_r28 = (fn_8001B3F0() >> 4U) & 1;
        if (var_r28 != 0) {
            M2C_FIELD(arg0, f32 *, 0x14) = (f32) ((-1.0f) * M2C_FIELD(arg0, f32 *, 0x14));
        }
        if (M2C_FIELD(arg0, f32 *, 0x14) < 0.0f) {
            var_f31 = (-2.0f);
        } else {
            var_f31 = 2.0f;
        }
        sp.sp8 = var_f31 + ((5.0f * M2C_FIELD(arg0, f32 *, 0x14)) + M2C_FIELD(arg0, f32 *, 0x3C));
        sp.spC = M2C_FIELD(arg0, f32 *, 0x40);
        var_r25 = (u16) (fn_8001B3F0() >> 4U);
        M2C_FIELD(arg0, f32 *, 0x14) = (f32) ((f32) var_r25 / 65535.0f);
        var_r26 = (fn_8001B3F0() >> 4U) & 1;
        if (var_r26 != 0) {
            M2C_FIELD(arg0, f32 *, 0x14) = (f32) ((-1.0f) * M2C_FIELD(arg0, f32 *, 0x14));
        }
        if (M2C_FIELD(arg0, f32 *, 0x14) < 0.0f) {
            var_f31 = (-2.0f);
        } else {
            var_f31 = 2.0f;
        }
        sp.sp10 = var_f31 + ((5.0f * M2C_FIELD(arg0, f32 *, 0x14)) + M2C_FIELD(arg0, f32 *, 0x44));
        fn_8001B1A0(arg0 + 0x24, &sp.sp8, arg0 + 0x18);
        cr_Vec3_Normalize(arg0 + 0x18);
    }
    if (M2C_FIELD(arg0, u8 *, 0x71) != 0) {
        M2C_FIELD(arg0, f32 *, 0x18) *= (-1.0f);
        M2C_FIELD(arg0, f32 *, 0x20) *= (-1.0f);
    }
}
