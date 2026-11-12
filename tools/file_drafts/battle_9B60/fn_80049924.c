
extern f32 __float_nan;

s32 fn_80049924(u8 arg4, u8 *arg5, f64 farg3, f64 farg4, f64 farg5) {
    struct {
        s32 sp8;
        u8 * spC;
        s32 sp10;
        s32 sp14;
        s32 sp18;
        s32 sp1C;
        f32 sp20;
        f32 sp24;
        f32 sp28;
        f32 sp2C;
        f32 sp30;
        f32 sp34;
        f32 sp38;
        f32 sp3C;
        f32 sp40;
        u8 pad_44[4];
        f64 sp48;
        f32 sp50;
        f32 sp54;
        f32 sp58;
        u8 pad_5C[20];
        s32 sp70;
        s32 sp74;
    } sp;
    f32 temp_f0_139;
    f32 temp_f1_189;
    f32 var_f18;
    f32 var_f19;
    f32 var_f20;
    f32 var_f21;
    f32 var_f22;
    f32 var_f23;
    f32 var_f24;
    f32 var_f25;
    f32 var_f27;
    f32 var_f28;
    f32 var_f30;
    f32 var_f31;
    f64 temp_f1_186;
    f64 var_f26;
    f64 var_f29;
    s16 temp_r0_1115;
    s16 temp_r3_903;
    s16 temp_r4_1501;
    s16 var_r28;
    s32 var_r18;
    s32 var_r19;
    s32 var_r20;
    s32 var_r21;
    s32 var_r22;
    s32 var_r25;
    u8 *var_r17;
    u8 *var_r23;
    u8 *var_r26;
    u8 *var_r27;
    u8 *var_r29;
    u8 *var_r30;
    u8 *var_r31;
    u8 temp_r4_1323;

    var_r31 = lbl_80329B60;
    cr_s_9B60_A60.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x170);
    cr_s_9B60_A60.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x174);
    cr_s_9B60_A60.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x178);
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2) {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) > lbl_80450638) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE4) > 0) {
                if ((u32) cr_g_SeqCurrent.unk_10 == 2) {
                    fn_80097CA8(M2C_FIELD((lbl_801F8F24 + (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1CC) * 8)), s32 *, 0), (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
                }
            } else if ((u32) cr_g_SeqCurrent.unk_10 == 2) {
                fn_80097CA8(M2C_FIELD((lbl_801F8F24 + (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1CC) * 8)), s32 *, 0), (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            }
            var_r28 = (s16) (((f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x3342A) * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C)) + (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE4));
            if (var_r28 > (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)) {
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = var_r28;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0);
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE4) = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
            temp_f0_139 = (f32) 0U;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) = temp_f0_139;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = temp_f0_139;
            fn_8001CCC4(cr_g_CurrentBattleRobo + 0x11C, lbl_80450638, lbl_80450638, lbl_80450638);
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
            cr_Vec3_Cross(&sp.sp50, cr_g_CurrentBattleRobo + 0xEC, cr_g_CurrentBattleRobo + 0x104);
            var_f31 = M2C_FIELD(M2C_ERROR(/* Read from unset register $r3 */), f32 *, 4);
            if (lbl_80450638 == var_f31) {
                fn_8001E5E4(cr_g_CurrentBattleRobo + 0xEC, cr_g_CurrentBattleRobo + 0x104);
            }
            sp.sp70 = 0x43300000;
            sp.sp74 = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33420) ^ 0x80000000;
            cr_Vec3_Cross(&sp.sp50, cr_g_CurrentBattleRobo + 0xEC, cr_g_CurrentBattleRobo + 0x104);
            var_f31 = sp.sp54;
            temp_f1_186 = fabs(var_f31);
            sp.sp48 = temp_f1_186;
            temp_f1_189 = (f32) temp_f1_186;
            sp.sp24 = temp_f1_189;
            if ((temp_f1_189 < lbl_804506A8) && (fn_8001E5E4(cr_g_CurrentBattleRobo + 0xEC, cr_g_CurrentBattleRobo + 0x104) < lbl_80450638)) {
                var_f31 = lbl_804506A8;
            }
            var_f30 = lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33420);
            if (var_f31 > ((lbl_804506C0 * var_f30) / lbl_804506C4)) {
                var_f31 = (lbl_804506C0 * var_f30) / lbl_804506C4;
            } else if (var_f31 < ((lbl_804506C0 * -var_f30) / lbl_804506C4)) {
                var_f31 = (lbl_804506C0 * -var_f30) / lbl_804506C4;
            }
            cr_Vec3_RotateY(cr_g_CurrentBattleRobo + 0xEC, var_f31);
            cr_Vec3_NormalizeTo(cr_g_CurrentBattleRobo + 0xEC, cr_g_CurrentBattleRobo + 0xEC);
            var_f31 = lbl_80450688 * ((f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x3341C) * fn_8001E424(cr_g_CurrentBattleRobo + 0x104));
            var_f30 = lbl_80450698 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x3341E);
            if (var_f31 > M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128)) {
                if ((var_f31 - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128)) > var_f30) {
                    var_f19 = var_f30;
                } else {
                    var_f19 = var_f31 - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128);
                }
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) += var_f19;
            } else {
                if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) - var_f31) > var_f30) {
                    var_f18 = var_f30;
                } else {
                    var_f18 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) - var_f31;
                }
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) -= var_f18;
            }
            if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) > lbl_80450638) {
                cr_s_9B60_A78.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xEC);
                cr_s_9B60_A78.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF0);
                cr_s_9B60_A78.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF4);
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1F8) & 1) {
                    cr_s_9B60_A60.x = (f32) cr_s_9B60_A54.x;
                    cr_s_9B60_A60.y = (f32) cr_s_9B60_A54.y;
                    cr_s_9B60_A60.z = (f32) cr_s_9B60_A54.z;
                    cr_s_9B60_A6C.x = (f32) cr_s_9B60_A60.x;
                    cr_s_9B60_A6C.y = (f32) cr_s_9B60_A60.y;
                    cr_s_9B60_A6C.z = (f32) cr_s_9B60_A60.z;
                    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x04000000;
                } else {
                    cr_s_9B60_A60.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xEC);
                    cr_s_9B60_A60.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF0);
                    cr_s_9B60_A60.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF4);
                    cr_s_9B60_A6C.x = (f32) cr_s_9B60_A60.x;
                    cr_s_9B60_A6C.y = (f32) cr_s_9B60_A60.y;
                    cr_s_9B60_A6C.z = (f32) cr_s_9B60_A60.z;
                }
                var_f31 = lbl_80450720 * (lbl_804507A4 * (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) / ((M2C_FIELD(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x85C), f32 *, 0x5C) + M2C_FIELD(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x85C), f32 *, 0x60)) / lbl_804507A8)));
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 2) {
                    if ((var_f31 / lbl_80450720) > lbl_80450688) {
                        var_r22 = 2;
                    } else {
                        var_r22 = 1;
                    }
                    cr_g_BattleMotionSlot = var_r22;
                } else if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1F8) & 1)) {
                    if ((var_f31 / lbl_80450720) > lbl_80450688) {
                        var_r21 = 2;
                    } else {
                        var_r21 = 1;
                    }
                    cr_g_BattleMotionSlot = var_r21;
                } else if ((var_f31 / lbl_80450720) <= (f32) lbl_804507B0) {
                    lbl_8044F4A8 = 1;
                    if (fn_8001E5E4((u8 *) &cr_s_9B60_A6C, (u8 *) &cr_s_9B60_A78) > lbl_80450638) {
                        var_r20 = 3;
                    } else {
                        var_r20 = 5;
                    }
                    cr_g_BattleMotionSlot = var_r20;
                } else if (M2C_FIELD(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238), u8 *, 0x528) != 3) {
                    lbl_8044F4A8 = 1;
                    if (fn_8001E5E4((u8 *) &cr_s_9B60_A6C, (u8 *) &cr_s_9B60_A78) > lbl_80450638) {
                        var_r19 = 4;
                    } else {
                        var_r19 = 6;
                    }
                    cr_g_BattleMotionSlot = var_r19;
                } else {
                    if (fn_8001E5E4((u8 *) &cr_s_9B60_A6C, (u8 *) &cr_s_9B60_A78) > lbl_80450638) {
                        var_r18 = 9;
                    } else {
                        var_r18 = 8;
                    }
                    cr_g_BattleMotionSlot = var_r18;
                }
                if ((var_f31 / lbl_80450720) > lbl_80450688) {
                    var_f31 *= lbl_804507B8;
                }
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += var_f31;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x54C) = var_f31;
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) = 0U;
            } else {
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1F8) & 1) {
                    cr_s_9B60_A6C.x = (f32) cr_s_9B60_A54.x;
                    cr_s_9B60_A6C.y = (f32) cr_s_9B60_A54.y;
                    cr_s_9B60_A6C.z = (f32) cr_s_9B60_A54.z;
                    lbl_8044F4A8 = 1;
                    cr_g_BattleMotionSlot = 0x14;
                } else {
                    cr_s_9B60_A60.x = (f32) cr_s_9B60_A6C.x;
                    cr_s_9B60_A60.y = (f32) cr_s_9B60_A6C.y;
                    cr_s_9B60_A60.z = (f32) cr_s_9B60_A6C.z;
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) = 0U;
                    cr_g_BattleMotionSlot = 0;
                }
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x138) != 0) {
                    if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x138) > 0) {
                        cr_g_BattleMotionSlot = 0x17;
                        cr_SinCos16(-0x444, &sp.sp3C, &sp.sp40);
                        var_f25 = cr_s_9B60_A60.x;
                        var_f24 = cr_s_9B60_A60.z;
                        cr_s_9B60_A60.z = (f32) ((sp.sp40 * var_f24) - (sp.sp3C * var_f25));
                        cr_s_9B60_A60.x = (f32) ((sp.sp3C * var_f24) + (sp.sp40 * var_f25));
                        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) -= 1;
                    } else {
                        cr_g_BattleMotionSlot = 0x18;
                        cr_SinCos16(0x444, &sp.sp34, &sp.sp38);
                        var_f23 = cr_s_9B60_A60.x;
                        var_f22 = cr_s_9B60_A60.z;
                        cr_s_9B60_A60.z = (f32) ((sp.sp38 * var_f22) - (sp.sp34 * var_f23));
                        cr_s_9B60_A60.x = (f32) ((sp.sp34 * var_f22) + (sp.sp38 * var_f23));
                        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) += 1;
                    }
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450654;
                } else if (fn_8001E5E4((u8 *) &cr_s_9B60_A6C, (u8 *) &cr_s_9B60_A60) < lbl_80450638) {
                    sp.sp50 = cr_s_9B60_A6C.x;
                    sp.sp54 = cr_s_9B60_A6C.y;
                    sp.sp58 = cr_s_9B60_A6C.z;
                    cr_SinCos16(-0x2000, &sp.sp2C, &sp.sp30);
                    var_f21 = sp.sp50;
                    var_f20 = sp.sp58;
                    sp.sp58 = (sp.sp30 * var_f20) - (sp.sp2C * var_f21);
                    sp.sp50 = (sp.sp2C * var_f20) + (sp.sp30 * var_f21);
                    cr_Vec3_Cross(&sp.sp50, &sp.sp50, (u8 *) &cr_s_9B60_A60);
                    if (sp.sp54 > lbl_80450638) {
                        sp.sp20 = lbl_80450654;
                    } else {
                        sp.sp20 = lbl_80450694;
                    }
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) = (u8) (lbl_80450720 * sp.sp20);
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450654;
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) += lbl_80450654;
                }
                cr_s_9B60_A78.x = (f32) cr_s_9B60_A60.x;
                cr_s_9B60_A78.y = (f32) cr_s_9B60_A60.y;
                cr_s_9B60_A78.z = (f32) cr_s_9B60_A60.z;
            }
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x2000) {
                fn_80097CA8(0x30079, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = (u8) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33424);
                switch ((s32) (u8) (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BEE))) {        /* switch 1; irregular */
                case 0:                             /* switch 1 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BF6);
                    break;
                case 1:                             /* switch 1 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BEF);
                    break;
                case 2:                             /* switch 1 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BF6);
                    break;
                }
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x11C) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x120) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x124) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x120) = (f32) lbl_80450638;
            }
        } else {
            var_r30 = cr_g_CurrentBattleRobo + 0x1E20;
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x20C4) != 0) {
                if (M2C_FIELD(var_r30, s16 *, 0x2A4) == 1) {
                    var_r17 = var_r30 + 0xE4;
                } else {
                    var_r17 = var_r30 + 0x1BC;
                }
                var_r27 = var_r17;
                if (M2C_FIELD(var_r30, s16 *, 0x2A6) <= (s32) M2C_FIELD(var_r17, u8 *, 1)) {
                    M2C_FIELD(var_r30, s16 *, 0x2AA) = (s16) M2C_FIELD(var_r27, u8 *, 5);
                    M2C_FIELD(var_r30, s16 *, 0x2A6) = (s16) (M2C_FIELD(var_r27, u8 *, 1) + 1);
                }
            }
            fn_8003F248(cr_g_CurrentBattleRobo + 0x28D40, (s32) (u8 *) &cr_s_9B60_A30, 4);
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) != 0) {
                sp.sp1C = 0xC;
            } else {
                sp.sp1C = 0xD;
            }
            cr_g_BattleMotionSlot = sp.sp1C;
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) > 0) {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) ((lbl_80450788 * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0));
            } else {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
            if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) <= 0) {
                switch ((s32) (u8) (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x56))) {        /* switch 2; irregular */
                case 0:                             /* switch 2 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5E);
                    break;
                case 1:                             /* switch 2 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x57);
                    break;
                case 2:                             /* switch 2 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5E);
                    break;
                }
            }
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) -= lbl_8044F4B0;
    } else {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1F8) & 1) {
            cr_s_9B60_A60.x = (f32) cr_s_9B60_A54.x;
            cr_s_9B60_A60.y = (f32) cr_s_9B60_A54.y;
            cr_s_9B60_A60.z = (f32) cr_s_9B60_A54.z;
            cr_s_9B60_A6C.x = (f32) cr_s_9B60_A60.x;
            cr_s_9B60_A6C.y = (f32) cr_s_9B60_A60.y;
            cr_s_9B60_A6C.z = (f32) cr_s_9B60_A60.z;
            lbl_8044F4A8 = 1;
            cr_s_9B60_A78.x = (f32) cr_s_9B60_A60.x;
            cr_s_9B60_A78.y = (f32) cr_s_9B60_A60.y;
            cr_s_9B60_A78.z = (f32) cr_s_9B60_A60.z;
        } else {
            cr_s_9B60_A60.x = (f32) cr_s_9B60_A6C.x;
            cr_s_9B60_A60.y = (f32) cr_s_9B60_A6C.y;
            cr_s_9B60_A60.z = (f32) cr_s_9B60_A6C.z;
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) != 0) {
                if (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x23E) & 0x2000) {
                    sp.sp70 = 0x43300000;
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) -= 1;
                    temp_r3_903 = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33424);
                    sp.sp74 = temp_r3_903 ^ 0x80000000;
                    var_f31 = (f32) temp_r3_903;
                    var_f30 = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33422);
                    var_f27 = (var_f31 * (var_f31 * (lbl_8044F4B0 * lbl_8044F4B0))) + (lbl_804507BC * lbl_8044F4B0 * var_f30);
                    if (var_f27 > lbl_80450638) {
                        var_f28 = var_f27;
                        var_f29 = __frsqrte(var_f28);
                        var_f29 = lbl_80450738 * var_f29 * -(((f64) var_f28 * (var_f29 * var_f29)) - lbl_80450740);
                        var_f29 = lbl_80450738 * var_f29 * -(((f64) var_f28 * (var_f29 * var_f29)) - lbl_80450740);
                        var_f29 = lbl_80450738 * var_f29 * -(((f64) var_f28 * (var_f29 * var_f29)) - lbl_80450740);
                        var_f26 = (f64) var_f28 * var_f29;
                        var_f26 = (f64) (f32) var_f26;
                    } else if (var_f27 < (f32) lbl_804506B8) {
                        var_f26 = (f64) __float_nan;
                    } else {
                        sp.sp28 = var_f27;
                        switch ((M2C_BITWISE(s32, sp.sp28) & 0x7F800000)) {      /* switch 3; irregular */
                        case 0x7F800000:            /* switch 3 */
                            if (M2C_BITWISE(s32, sp.sp28) & 0x7FFFFF) {
                                var_r25 = 1;
                            } else {
                                var_r25 = 2;
                            }
                            break;
                        case 0x0:                   /* switch 3 */
                            if (M2C_BITWISE(s32, sp.sp28) & 0x7FFFFF) {
                                var_r25 = 5;
                            } else {
                                var_r25 = 3;
                            }
                            break;
                        default:                    /* switch 3 */
                            var_r25 = 4;
                            break;
                        }
                        if (var_r25 == 1) {
                            var_f26 = (f64) __float_nan;
                        } else {
                            var_f26 = (f64) var_f27;
                        }
                    }
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) += ((lbl_80450648 * ((lbl_8044F4B0 * var_f31) + (f32) var_f26)) / var_f31);
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
                }
            }
            if ((M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) != 0) && (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x2000)) {
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
                switch ((s32) (u8) (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BEE))) {       /* switch 4; irregular */
                case 0:                             /* switch 4 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 7;
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = (u8) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33434);
                    break;
                case 1:                             /* switch 4 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 6;
                    break;
                case 2:                             /* switch 4 */
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 8;
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = (u8) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33434);
                    break;
                }
                temp_r0_1115 = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x3342E);
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = temp_r0_1115;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = temp_r0_1115;
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) -= 1;
                fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
                if (fn_8001E424(cr_g_CurrentBattleRobo + 0x104) > lbl_80450638) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x110) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104);
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x114) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108);
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x118) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C);
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x110) = (f32) cr_s_9B60_A60.x;
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x114) = (f32) cr_s_9B60_A60.y;
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x118) = (f32) cr_s_9B60_A60.z;
                }
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x114) = (f32) lbl_80450638;
                cr_Vec3_NormalizeTo(cr_g_CurrentBattleRobo + 0x110, cr_g_CurrentBattleRobo + 0x110);
            }
            sp.sp50 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
            sp.sp54 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
            sp.sp58 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
            fn_8001CA28(&sp.sp50, cr_g_CurrentBattleRobo + 0x104, lbl_804506D0 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33428));
            sp.sp54 = lbl_80450638;
            if (fn_8001E424(&sp.sp50) > (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33426))) {
                cr_Vec3_NormalizeTo(&sp.sp50, &sp.sp50);
                sp.sp50 *= lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33426);
                sp.sp54 *= lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33426);
                sp.sp58 *= lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33426);
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) = sp.sp50;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) = sp.sp58;
            cr_g_BattleMotionSlot = 7;
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1;
            cr_g_BattleMotionSlot = 0xA;
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) -= lbl_8044F4B0;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
        var_r28 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2;
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x20C4) == 0) && (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x10)) {
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 3;
            fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
            if (var_r28 != 0) {
                sp.sp18 = 0;
            } else {
                sp.sp18 = 1;
            }
            temp_r4_1323 = *(cr_g_CurrentBattleRobo + ((sp.sp18 * 0xD8) + 0x1F04));
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) temp_r4_1323;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) temp_r4_1323;
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0) {
                var_r29 = cr_g_CurrentBattleRobo + 0x1E20;
                if (var_r28 != 0) {
                    sp.sp14 = 1;
                } else {
                    sp.sp14 = 2;
                }
                if (sp.sp14 != 0) {
                    if (M2C_FIELD(var_r29, s16 *, 0x2A4) == 0) {
                        if (var_r28 != 0) {
                            sp.sp10 = 1;
                        } else {
                            sp.sp10 = 2;
                        }
                        M2C_FIELD(var_r29, s16 *, 0x2A4) = (s16) sp.sp10;
                        M2C_FIELD(var_r29, s16 *, 0x2AA) = 0;
                        M2C_FIELD(var_r29, s16 *, 0x2A6) = 0;
                        M2C_FIELD(var_r29, s16 *, 0x2A8) = 0x100;
                    }
                } else if (M2C_FIELD(var_r29, s16 *, 0x2A4) != 0) {
                    if (M2C_FIELD(var_r29, s16 *, 0x2A4) == 1) {
                        sp.spC = var_r29 + 0xE4;
                    } else {
                        sp.spC = var_r29 + 0x1BC;
                    }
                    var_r26 = sp.spC;
                    if (M2C_FIELD(var_r29, s16 *, 0x2A6) <= (s32) M2C_FIELD(var_r26, u8 *, 1)) {
                        M2C_FIELD(var_r29, s16 *, 0x2AA) = (s16) M2C_FIELD(var_r26, u8 *, 5);
                        M2C_FIELD(var_r29, s16 *, 0x2A6) = (s16) (M2C_FIELD(var_r26, u8 *, 1) + 1);
                    }
                }
                lbl_8044F4A8 = 1;
            }
        } else {
            var_r23 = cr_g_CurrentBattleRobo + 0x30000;
            if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x28ECE) <= 0) && (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x20)) {
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
                M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 4;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) cr_g_BattleParams.unk_08[4];
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) cr_g_BattleParams.unk_08[4];
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x138) = 0U;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
                fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
                if (var_r28 != 0) {
                    sp.sp8 = 0;
                } else {
                    sp.sp8 = 1;
                }
                fn_8003F248(cr_g_CurrentBattleRobo + 0x28D40, (s32) (cr_g_CurrentBattleRobo + 0x104), sp.sp8);
            } else if ((M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x1000) && (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x2E468) < (s32) M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x2E3C9))) {
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
                M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 5;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) cr_g_BattleParams.unk_08[6];
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) cr_g_BattleParams.unk_08[6];
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
            } else if (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x40) {
                if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0xC;
                    cr_g_BattleMotionSlot = 0x23;
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x148) = 0;
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x186C) = 0;
                    temp_r4_1501 = M2C_FIELD(cr_g_CurrentBodyStats, s16 *, 0xDA);
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = temp_r4_1501;
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = temp_r4_1501;
                    fn_8001C98C(cr_g_CurrentBattleRobo + 0x110, M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x238) + 0x20, cr_g_CurrentBattleRobo + 0x20);
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x114) = (f32) lbl_80450638;
                    cr_Vec3_Normalize(cr_g_CurrentBattleRobo + 0x110);
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x118) = (f32) -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x118);
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
                    M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
                    fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x1DCC), lbl_80450638);
                    M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1DBC) = 1;
                    fn_80097CA8(0x30001, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
                }
            } else if ((M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x80) && (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x146) == 0)) {
                fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
                M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0x11;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0xA;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = 0xA;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
                M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0xE6) = 3;
            }
        }
    }
    return 1;
}
