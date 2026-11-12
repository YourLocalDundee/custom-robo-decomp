/* fn_80048050 wip 75% (Nov 6, with the pool: file statics cr_s_9B60_*; was before): m2c 62; the gun-chain sibling of fn_80048708: Vec struct copies (scratch veccopy.py), lbl_80329B60 through a local 'tb' declared first and used for every reference, the view-vector chain a6c = a78 = a60 = robo->x110, the stack Vec, the (f32) fabs result in a float local; left: register / scheduling details (the copies' robo load is not hoisted as in the original) */

s32 fn_80048050(void) {
    Vec sp;
    f32 var_f31_59;
    f32 temp_f30_111;
    u8 *temp_r31_161;
    s32 var_r29_19;
    u8 temp_r0_152;
    cr_g_BattleMotionSlot = 0xA;
    var_r29_19 = 0;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) <= 0) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BF7) != 0) {
                fn_80097CA8(0x3007A, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            } else {
                fn_80097CA8(0x30000, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            }
        }
        var_r29_19 = 1;
    } else if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) != 0) {
        cr_Vec3_Cross(&sp, cr_g_CurrentBattleRobo + 0x110, cr_g_CurrentBattleRobo + 0x104);
        var_f31_59 = sp.y;
        if (cr_Vec3_LengthInline(cr_g_CurrentBattleRobo + 0x104) > lbl_80450638) {
            sp = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x104);
            cr_Vec3_Normalize(&sp);
        } else {
            sp = cr_s_9B60_A6C;
        }
        if (((f32) fabs(var_f31_59) < lbl_804506A8) && (fn_8001E5E4(cr_g_CurrentBattleRobo + 0x110, cr_g_CurrentBattleRobo + 0x104) < lbl_80450638)) {
            var_f31_59 = lbl_804506A8;
        }
        temp_f30_111 = lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33438);
        if (var_f31_59 > ((lbl_804506C0 * temp_f30_111) / lbl_804506C4)) {
            var_f31_59 = (lbl_804506C0 * temp_f30_111) / lbl_804506C4;
        } else if (var_f31_59 < ((lbl_804506C0 * -temp_f30_111) / lbl_804506C4)) {
            var_f31_59 = (lbl_804506C0 * -temp_f30_111) / lbl_804506C4;
        }
        cr_Vec3_RotateY(cr_g_CurrentBattleRobo + 0x110, var_f31_59);
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x20C4) == 0) && (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x10)) {
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 9;
            temp_r0_152 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1FDC);
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) temp_r0_152;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) temp_r0_152;
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
                temp_r31_161 = cr_g_CurrentBattleRobo + 0x1E20;
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x20C4) == 0) {
                    M2C_FIELD(temp_r31_161, s16 *, 0x2A4) = 2;
                    M2C_FIELD(temp_r31_161, s16 *, 0x2AA) = 0;
                    M2C_FIELD(temp_r31_161, s16 *, 0x2A6) = 0;
                    M2C_FIELD(temp_r31_161, s16 *, 0x2A8) = 0x100;
                }
                lbl_8044F4A8 = 1;
            }
        } else if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x28ECE) <= 0) && (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x20)) {
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0xA;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) cr_g_BattleParams.unk_08[4];
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) cr_g_BattleParams.unk_08[4];
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x138) = 0;
            fn_8003F248(cr_g_CurrentBattleRobo + 0x28D40, (s32) (cr_g_CurrentBattleRobo + 0x104), 1);
        } else if ((M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x1000) && (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x2E468) < (s32) M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x2E3C9))) {
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0xB;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) cr_g_BattleParams.unk_08[6];
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) cr_g_BattleParams.unk_08[6];
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x10) = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x110);
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) *= (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33436));
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) *= (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33436));
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) *= (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33436));
        sp.x = lbl_80450638;
        sp.y = lbl_80450654;
        sp.z = lbl_80450638;
        cr_Vec3_Cross(&sp, &sp, cr_g_CurrentBattleRobo + 0x110);
        cr_Vec3_RotateAboutAxis(cr_g_CurrentBattleRobo + 0x10, &sp, (lbl_804506C0 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1BF8)) / lbl_804506C4);
        cr_s_9B60_A6C = cr_s_9B60_A78 = cr_s_9B60_A60 = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x110);
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) -= 1;
        if (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5F) == 0) {
            var_r29_19 = 1;
        }
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
        var_r29_19 = 1;
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE4) = (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33430);
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_8045078C;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) >= lbl_80450680) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450770;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x550) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548);
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
    return var_r29_19;
}
