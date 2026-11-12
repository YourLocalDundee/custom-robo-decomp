/* fn_800490C8 wip 99% (m2c 51%): the bomb action (action 4; slots 0x0E / 0x0F turning, 0x10 ground, 0x11 air); fires through fn_8003F248 with the direction in the pool (+0xA30) and 2 / 3 (ground / air), the on-ground test before the flag check; Vec copies and file statics. Left: the register of the first address in `A6C.z = -A6C.z` (r3 against r4) */

s32 fn_800490C8(void) {
    Vec v;
    f32 var_f31_144;
    s32 var_r30_37;
    u32 ground;

    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
        cr_s_9B60_A30 = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x104);
        ground = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2;
        if (!(M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x23E) & 0x20)) {
            if (ground != 0) {
                var_r30_37 = 2;
            } else {
                var_r30_37 = 3;
            }
            fn_8003F248(cr_g_CurrentBattleRobo + 0x28D40, (s32) (u8 *) &cr_s_9B60_A30, var_r30_37);
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
        }
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1;
    }
    fn_8001C98C((u8 *) &cr_s_9B60_A6C, cr_g_CurrentBattleRobo + 0x28ED4, cr_g_CurrentBattleRobo + 0x20);
    cr_s_9B60_A6C.z = -cr_s_9B60_A6C.z;
    M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x164) = cr_s_9B60_A6C;
    lbl_8044F4A8 = 2;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2) {
        cr_g_BattleMotionSlot = 0x10;
        if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x138) != 0) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x138) > 0) {
                cr_g_BattleMotionSlot = 0xE;
                cr_Vec3_RotateY((u8 *) &cr_s_9B60_A60, lbl_80450798);
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) -= 1;
            } else {
                cr_g_BattleMotionSlot = 0xF;
                cr_Vec3_RotateY((u8 *) &cr_s_9B60_A60, lbl_8045079C);
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) += 1;
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450654;
        } else if (fn_8001E5E4((u8 *) &cr_s_9B60_A6C, (u8 *) &cr_s_9B60_A60) < lbl_80450638) {
            v = cr_s_9B60_A6C;
            cr_Vec3_RotateY(&v, lbl_804507A0);
            cr_Vec3_Cross(&v, &v, (u8 *) &cr_s_9B60_A60);
            if (v.y > lbl_80450638) {
                var_f31_144 = lbl_80450654;
            } else {
                var_f31_144 = lbl_80450694;
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) = (u8) (lbl_80450720 * var_f31_144);
        }
        cr_s_9B60_A78 = cr_s_9B60_A60;
    } else {
        cr_s_9B60_A78 = cr_s_9B60_A6C;
        cr_s_9B60_A60 = cr_s_9B60_A78;
        cr_g_BattleMotionSlot = 0x11;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = (f32) (lbl_80450688 * (f32) -cr_g_BattleParams.unk_08[2]);
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A4) = (f32) lbl_80450654;
        if (lbl_80450638 == M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A8)) {
            fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x954));
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8FC) != 0U) {
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x333E0) = fn_80079214((float (*)[4]) (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8FC) + 0x44), M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4));
            }
            fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x95C));
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x904) != 0U) {
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x333E8) = fn_80079214((float (*)[4]) (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x904) + 0x44), M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC));
            }
            fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333C4), lbl_80450638);
            fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333CC), lbl_80450638);
        }
    }
    return 1;
}
