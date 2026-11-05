/* fn_800B7D8C: Nov 5, 2026 hand draft (m2c 95%, permuter queue 5 score 0, run in cr-linux): the target point as the 12-byte row element &rows[n - 1][4] like the two stores after it, and the float divisor tested without `!= 0.0f` */
s32 fn_800B7D8C(u8 *arg0) {
    f32 var_f31_38;
    f32 temp_f30_181;
    s32 var_r30_12;

    var_r30_12 = 0;
    if (*(u16 *) &((u8 (*)[0x2]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0x11C] != 0) {
        var_f31_38 = (f32) M2C_FIELD(arg0, u16 *, 0x13C) / (f32) *(u16 *) &((u8 (*)[0x2]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0x11C];
    } else {
        var_f31_38 = 0.0f;
    }
    M2C_FIELD(arg0, f32 *, 0xC4) = (f32) ((3.0f * M2C_FIELD(arg0, f32 *, 0xC4) * (1.0f - var_f31_38)) + (var_f31_38 * (*(f32 *) &((u8 (*)[0xC]) (arg0))[M2C_FIELD(arg0, u8 *, 0x13E)][0x4] - M2C_FIELD(arg0, f32 *, 0xD0))));
    M2C_FIELD(arg0, f32 *, 0xC8) = (f32) ((3.0f * M2C_FIELD(arg0, f32 *, 0xC8) * (1.0f - var_f31_38)) + (var_f31_38 * (*(f32 *) &((u8 (*)[0xC]) (arg0))[M2C_FIELD(arg0, u8 *, 0x13E)][0x8] - M2C_FIELD(arg0, f32 *, 0xD4))));
    M2C_FIELD(arg0, f32 *, 0xCC) = (f32) ((3.0f * M2C_FIELD(arg0, f32 *, 0xCC) * (1.0f - var_f31_38)) + (var_f31_38 * (*(f32 *) &((u8 (*)[0xC]) (arg0))[M2C_FIELD(arg0, u8 *, 0x13E)][0xC] - M2C_FIELD(arg0, f32 *, 0xD8))));
    cr_Vec3_Normalize(arg0 + 0xC4);
    M2C_FIELD(arg0, f32 *, 0xD0) = (f32) ((M2C_FIELD(arg0, f32 *, 0xC4) * *(f32 *) &((u8 (*)[0x4]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0xDC]) + M2C_FIELD(arg0, f32 *, 0xD0));
    M2C_FIELD(arg0, f32 *, 0xD4) = (f32) ((M2C_FIELD(arg0, f32 *, 0xC8) * *(f32 *) &((u8 (*)[0x4]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0xDC]) + M2C_FIELD(arg0, f32 *, 0xD4));
    M2C_FIELD(arg0, f32 *, 0xD8) = (f32) ((M2C_FIELD(arg0, f32 *, 0xCC) * *(f32 *) &((u8 (*)[0x4]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0xDC]) + M2C_FIELD(arg0, f32 *, 0xD8));
    cr_g_CurrentChara->move_target.x = M2C_FIELD(arg0, f32 *, 0xD0);
    cr_g_CurrentChara->move_target.z = M2C_FIELD(arg0, f32 *, 0xD8);
    cr_g_CurrentChara->angle = fn_8001F060(-M2C_FIELD(arg0, f32 *, 0xC4), -M2C_FIELD(arg0, f32 *, 0xCC));
    cr_g_CurrentChara->countdown_198c = 1;
    cr_g_CurrentChara->unk_0010 = cr_g_CurrentChara->move_target.x - cr_g_CurrentChara->pos.x;
    cr_g_CurrentChara->unk_0018 = cr_g_CurrentChara->move_target.z - cr_g_CurrentChara->pos.z;
    cr_g_CurrentChara->flags2 |= 2;
    M2C_FIELD(arg0, u16 *, 0x13C) += 1;
    if (M2C_FIELD(arg0, u16 *, 0x13C) >= (u16) *(u16 *) &((u8 (*)[0x2]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0x11C]) {
        if ((s32) M2C_FIELD(arg0, u8 *, 0x13E) < (s32) (M2C_FIELD(arg0, u8 *, 0) - 1)) {
            M2C_FIELD(arg0, u8 *, 0x13E) += 1;
            M2C_FIELD(arg0, u16 *, 0x13C) = 0U;
        } else {
            var_r30_12 = 1;
            temp_f30_181 = cr_Vec3_Distance(&cr_g_CurrentChara->pos, &((u8 (*)[0xC]) (arg0))[(M2C_FIELD(arg0, u8 *, 0) - 1)][0x4]);
            cr_g_CurrentChara->move_target.x = *(f32 *) &((u8 (*)[0xC]) (arg0))[(M2C_FIELD(arg0, u8 *, 0) - 1)][0x4];
            cr_g_CurrentChara->move_target.z = *(f32 *) &((u8 (*)[0xC]) (arg0))[(M2C_FIELD(arg0, u8 *, 0) - 1)][0xC];
            if (*(f32 *) &((u8 (*)[0x4]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0xDC]) {
                cr_g_CurrentChara->countdown_198c = (u16) (temp_f30_181 / *(f32 *) &((u8 (*)[0x4]) (arg0))[(M2C_FIELD(arg0, u8 *, 0x13E) - 1)][0xDC]);
            } else {
                cr_g_CurrentChara->countdown_198c = 1;
            }
            cr_g_CurrentChara->unk_0010 = (cr_g_CurrentChara->move_target.x - cr_g_CurrentChara->pos.x) / (f32) cr_g_CurrentChara->countdown_198c;
            cr_g_CurrentChara->unk_0018 = (cr_g_CurrentChara->move_target.z - cr_g_CurrentChara->pos.z) / (f32) cr_g_CurrentChara->countdown_198c;
        }
    }
    return var_r30_12;
}
