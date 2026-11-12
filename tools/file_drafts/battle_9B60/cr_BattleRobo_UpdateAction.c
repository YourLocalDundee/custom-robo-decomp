
extern f32 __float_nan;
extern u8 *lbl_8044F4A4;

s8 cr_BattleRobo_UpdateAction(void) {
    f32 sp6C;
    f32 sp70;
    f32 spF0;                                       /* compiler-managed */
    s32 spEC;
    s32 spE8;
    s32 spDC;
    M2C_UNK32 spD8;
    s32 spCC;
    s32 spC8;
    f32 spC4;
    f32 spC0;
    f32 spBC;
    f32 spB8;
    f32 spB4;
    f32 spB0;
    M2C_UNK spA4;
    M2C_UNK sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    u8 *sp48;
    u32 sp44;
    s32 sp40;
    f32 sp3C;
    s32 sp38;
    s32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    u8 *sp20;
    s32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    s32 spC;
    u8 sp8;
    f32 temp_f31_1111;
    f32 temp_f31_1247;
    f32 temp_f31_1656;
    f32 temp_f31_511;
    f32 temp_f31_607;
    f32 temp_f31_697;
    f32 temp_f31_891;
    f64 temp_f30_1812;
    f64 temp_f30_1819;
    f64 temp_f30_1823;
    f32 var_f29_1581;
    f32 temp_f27_1805;
    f32 var_f26_1651;
    f32 var_f25_1682;
    f32 var_f24_1829;
    f32 temp_f23_1619;
    f32 temp_f21_1420;
    f32 temp_f20_1419;
    f32 temp_f5_1262;
    f32 temp_f4_1268;
    f32 temp_f2_897;
    f32 temp_f1_1432;
    u8 *temp_r30_1019;
    s32 (**temp_r7_212)(s32, s32, s32, s32, M2C_UNK, u8 *, u8);
    u8 *temp_r29_1409;
    u8 *temp_r28_947;
    u8 *temp_r27_2322;
    s32 var_r26_946;
    s32 var_r25_2296;
    void *temp_r24_55;
    u8 *temp_r23_2257;
    u8 *temp_r21_1414;
    u8 *temp_r20_1405;
    s32 var_r19_1853;
    s8 var_r18_331;
    s8 var_r17_1642;
    u8 temp_r9_205;
    s16 temp_r7_96;
    s32 temp_r7_732;
    s32 temp_r6_207;
    u32 temp_r6_830;
    s32 temp_r5_208;
    u32 temp_r5_1188;
    s32 temp_r4_211;
    s32 temp_r3_197;
    u32 temp_r3_2204;

    cr_g_CurrentBodyStats = cr_g_CurrentBattleRobo + 0x1B98;
    lbl_8044F4A4 = cr_g_CurrentBattleRobo + 0x18A0;
    lbl_8044F4A0 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x238) + 0x18A0;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) &= 0xDFFFFFFF;
    if ((void *) M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238) != NULL) {
        temp_r24_55 = M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238);
        if (((M2C_FIELD(temp_r24_55, u8 *, 0x528) == 0x11) || (M2C_FIELD(temp_r24_55, u8 *, 0x528) == 0x12)) && (M2C_FIELD(temp_r24_55, s16 *, 0xE2) < (s16) M2C_FIELD(temp_r24_55, s16 *, 0xE0))) {
            sp50 = 1;
        } else {
            sp50 = 0;
        }
        if (sp50 != 0) {
            return 1;
        }
        goto block_8;
    }
block_8:
    spC8 = 0x43300000;
    lbl_8044F4B4 = lbl_804506B0 * cr_g_BattleParams.unk_04;
    temp_r7_96 = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x3342C);
    spCC = temp_r7_96 ^ 0x80000000;
    lbl_8044F4B0 = lbl_804506A8 * (lbl_8044F4B4 * (f32) temp_r7_96);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x333A4) = (f32) lbl_80450638;
    cr_g_BattleMotionSlot = 0;
    cr_s_9B60_A6C.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x158);
    cr_s_9B60_A6C.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x15C);
    cr_s_9B60_A6C.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x160);
    cr_s_9B60_A60.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x170);
    cr_s_9B60_A60.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x174);
    cr_s_9B60_A60.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x178);
    cr_s_9B60_A78.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17C);
    cr_s_9B60_A78.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x180);
    cr_s_9B60_A78.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x184);
    if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 2)) {
        fn_8001CEF4(M2C_FIELD(cr_g_CurrentBattleRobo, u8 **, 0x238) + 0x10A4, cr_g_CurrentBattleRobo + 0x14C);
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14C) = (f32) cr_s_9B60_A6C.x;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x150) = (f32) cr_s_9B60_A6C.y;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x154) = (f32) cr_s_9B60_A6C.z;
    }
    fn_8001CEF4(cr_g_CurrentBattleRobo + 0x10A4, &spA4);
    fn_8001C98C((u8 *) &cr_s_9B60_A54, cr_g_CurrentBattleRobo + 0x14C, &spA4);
    cr_s_9B60_A54.z = (f32) -cr_s_9B60_A54.z;
    cr_Vec3_Normalize((u8 *) &cr_s_9B60_A54);
    fn_8001CEF4(cr_g_CurrentBattleRobo + 0x1284, &sp98);
    fn_8001C98C((u8 *) &cr_s_9B60_A48, cr_g_CurrentBattleRobo + 0x14C, &sp98);
    cr_s_9B60_A48.z = (f32) -cr_s_9B60_A48.z;
    cr_Vec3_Normalize((u8 *) &cr_s_9B60_A48);
    lbl_8044F4A8 = 0;
    cr_s_9B60_A3C.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14C);
    cr_s_9B60_A3C.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x150);
    cr_s_9B60_A3C.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x154);
    fn_8001CCC4((u8 *) &cr_s_9B60_A30, lbl_80450638, lbl_80450638, lbl_80450638);
    temp_r3_197 = M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) = (s32) (temp_r3_197 & 0xFBFFFFFF);
    lbl_8044F4C4 = M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528);
    temp_r9_205 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x52C);
    lbl_8044F4C0 = (u32) temp_r9_205;
    temp_r6_207 = M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8);
    temp_r5_208 = temp_r6_207 & 0xFFFBFFFF;
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) = temp_r5_208;
    temp_r4_211 = ((u32) lbl_8044F4C4 >> 0x16U) & 0x3FC;
    temp_r7_212 = (long (**)(long, long, long, long, long, unsigned char *, unsigned char)) ((cr_RoboActionFunc *) ((u8 *) cr_g_BattleRoboActionFuncs + temp_r4_211));
    sp58 = (*(M2C_UNK (**)()) temp_r7_212)(temp_r3_197, temp_r4_211, temp_r5_208, temp_r6_207, temp_r7_212, cr_g_CurrentBattleRobo, temp_r9_205);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) &= 0xF4FFFFFF;
    if ((s32) lbl_8044F4A8 == 1) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x02000000;
    }
    if ((s32) lbl_8044F4A8 == 2) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x01000000;
    }
    if (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 9) || ((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 0xA)) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x08000000;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) > 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) -= 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE6) != 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE6) -= 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE7) != 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE7) -= 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE8) != 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE8) -= 1;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x146) > 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x146) -= 1;
    }
    sp38 = 1;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE6) == 0) {
        sp34 = 0;
        if (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 0xC) && ((1 << M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x186C)) & M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0xD0))) {
            sp34 = 1;
        }
        if (sp34 == 0) {
            sp38 = 0;
        }
    }
    var_r18_331 = (s8) sp38;
    if (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 7) && (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5F) != 0) && (M2C_FIELD(lbl_8044F4A4, s16 *, 2) == 1) && (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) == 0)) {
        var_r18_331 = 1;
    }
    if (((u32) cr_g_SeqCurrent.unk_10 != 2) && ((u32) cr_g_SeqCurrent.unk_10 != 3)) {
        spF0 = 0.0f;
        var_r18_331 = 0;
        M2C_FIELD(lbl_8044F4A4, s16 *, 2) = 0;
    }
    if ((M2C_FIELD(lbl_8044F4A4, s16 *, 2) != 0) && (var_r18_331 == 0)) {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x10000000) {
            fn_8008688C(&cr_g_BattleRoboStates[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C)]);
            return (s8) sp58;
        }
        if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x80000000)) {
            fn_8001F6BC((s32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x8E), 0xFU);
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x149) = 1;
        if (M2C_FIELD(lbl_8044F4A4, s8 *, 0x26) != 0) {
            fn_8001C98C(&spB0, cr_g_CurrentBattleRobo + 0x20, (void *) (M2C_FIELD(lbl_8044F4A4, s32 *, 4) + 0x20));
            spB8 = -spB8;
        } else {
            spB0 = M2C_FIELD(lbl_8044F4A4, f32 *, 8);
            spB4 = M2C_FIELD(lbl_8044F4A4, f32 *, 0xC);
            spB8 = M2C_FIELD(lbl_8044F4A4, f32 *, 0x10);
        }
        spB4 = lbl_80450638;
        if (cr_Vec3_LengthInline(&spB0) > lbl_80450638) {
            cr_Vec3_Normalize(&spB0);
        } else {
            fn_8001CCC4(&spB0, lbl_80450638, lbl_80450638, lbl_80450654);
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x13A) = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x140) = (f32) lbl_80450638;
        if ((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 0xC) {
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1DBC) = 0;
        }
        if (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) != 0xE) && ((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) != 0xF)) {
            if (((void *) M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238) != NULL) && ((M2C_FIELD(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238), f32 *, 0x74) >= lbl_80450654) || (cr_g_GameMode.flags & 0x100000))) {
                temp_f31_511 = (((f32) M2C_FIELD(lbl_8044F4A4, u16 *, 0x14) * (f32) M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x42)) / (f32) cr_g_BattleParams.unk_00) / lbl_80450658;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) = (f32) -((lbl_80450700 * temp_f31_511) - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74));
                fn_8007AB14(M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C), lbl_80450700 * temp_f31_511);
            }
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1DB8) != 0) {
                M2C_FIELD(lbl_8044F4A4, s16 *, 0x16) = 0;
                if (M2C_FIELD(lbl_8044F4A4, s16 *, 2) == 4) {
                    M2C_FIELD(lbl_8044F4A4, s16 *, 0x18) = 0x78;
                    M2C_FIELD(lbl_8044F4A4, s16 *, 0x1A) = 0;
                    M2C_FIELD(lbl_8044F4A4, s16 *, 0x1C) = 0x14;
                    M2C_FIELD(lbl_8044F4A4, s16 *, 0x1E) = 0x64;
                }
            }
            if ((M2C_FIELD(lbl_8044F4A4, s16 *, 0x16) >= 0) && ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) >= lbl_80450654) || (cr_g_GameMode.flags & 0x100000))) {
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) += M2C_FIELD(lbl_8044F4A4, s16 *, 0x16);
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) < (s32) M2C_FIELD(cr_g_CurrentBodyStats, u16 *, 0x40)) {
                    if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1DB8) != 0) {
                        if (M2C_FIELD(lbl_8044F4A4, s16 *, 2) == 4) {
                            M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) = 0xD;
                            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) M2C_FIELD(lbl_8044F4A4, s16 *, 0x1C);
                            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2);
                            spBC = spB0;
                            spC0 = spB4;
                            spC4 = spB8;
                            temp_f31_607 = (lbl_804506C0 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x1A)) / lbl_804506C4;
                            spC0 = (f32) sin((f64) temp_f31_607);
                            spBC *= (f32) cos((f64) temp_f31_607);
                            sp30 = (f32) cos((f64) temp_f31_607);
                            spC4 *= sp30;
                            fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
                            fn_8001CA28(cr_g_CurrentBattleRobo + 0x10, &spBC, lbl_80450688 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x18));
                            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
                        }
                    } else {
                        M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) = 0xD;
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) ((M2C_FIELD(lbl_8044F4A4, s16 *, 0x1C) * M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x44)) / 100);
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2);
                        spBC = spB0;
                        spC0 = spB4;
                        spC4 = spB8;
                        temp_f31_697 = (lbl_80450708 * ((lbl_804506C0 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x1A)) / lbl_804506C4)) / lbl_8045070C;
                        spC0 = cr_Sin16((u32) (s32) temp_f31_697);
                        spBC *= cr_Cos16((u32) (s32) temp_f31_697);
                        spC4 *= cr_Cos16((u32) (s32) temp_f31_697);
                        fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
                        temp_r7_732 = M2C_FIELD(lbl_8044F4A4, s16 *, 0x18) * M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x43);
                        spE8 = 0x43300000;
                        spEC = temp_r7_732 ^ 0x80000000;
                        fn_8001CA28(cr_g_CurrentBattleRobo + 0x10, &spBC, (f32) temp_r7_732 / lbl_80450700);
                        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
                    }
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) = -1;
                }
            } else {
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) = -1;
            }
            *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 8;
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) == -1) {
                if (M2C_FIELD(lbl_8044F4A4, s16 *, 2) != 5) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) = 0xE;
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x46);
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) = 0xF;
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) cr_g_BattleParams.unk_08[8];
                }
                if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) <= lbl_80450654) && !(cr_g_GameMode.flags & 0x100000)) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0xFF;
                }
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) = spB0;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = spB4;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) = spB8;
                fn_8001E5E4((u8 *) &cr_s_9B60_A6C, cr_g_CurrentBattleRobo + 0x10);
                temp_r6_830 = (u32) M2C_ERROR(/* unknown instruction: mfcr $r6 */) >> 0x1FU;
                M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) = (s8) temp_r6_830;
                if ((s8) temp_r6_830 != 0) {
                    cr_s_9B60_A6C.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
                    cr_s_9B60_A6C.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
                    cr_s_9B60_A6C.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
                    fn_8001CA5C((u8 *) &cr_s_9B60_A6C, lbl_80450694);
                } else {
                    cr_s_9B60_A6C.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
                    cr_s_9B60_A6C.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
                    cr_s_9B60_A6C.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
                }
                cr_s_9B60_A78.y = (f32) cr_s_9B60_A6C.y;
                cr_s_9B60_A78.x = (f32) cr_s_9B60_A6C.x;
                cr_s_9B60_A78.z = (f32) cr_s_9B60_A6C.z;
                spF0 = cr_s_9B60_A78.y;
                cr_s_9B60_A60.x = (f32) cr_s_9B60_A78.x;
                cr_s_9B60_A60.y = (f32) cr_s_9B60_A78.y;
                cr_s_9B60_A60.z = (f32) cr_s_9B60_A78.z;
                M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14B) = 0;
                temp_f31_891 = (lbl_804506C0 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x22)) / lbl_804506C4;
                temp_f2_897 = (f32) sin((f64) temp_f31_891);
                sp2C = temp_f2_897;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = temp_f2_897;
                sp28 = (f32) cos((f64) temp_f31_891);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) *= sp28;
                sp24 = (f32) cos((f64) temp_f31_891);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) *= sp24;
                fn_8001CA5C(cr_g_CurrentBattleRobo + 0x10, (f32) (M2C_FIELD(lbl_8044F4A4, s16 *, 0x20) * M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x43)) / lbl_80450700);
                M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) = 0;
                if (M2C_FIELD(lbl_8044F4A4, s16 *, 2) == 1) {
                    sp4C = 0;
                    var_r26_946 = 0;
                    temp_r28_947 = cr_g_CurrentBattleRobo + 0x1E20;
                    while (var_r26_946 < 0x20) {
                        if ((s8) *(temp_r28_947 + ((var_r26_946 * 0x1360) + 0x305)) != 0) {
                            *(temp_r28_947 + ((var_r26_946 * 0x1360) + 0x305)) = 0;
                            fn_800750F8(M2C_FIELD((temp_r28_947 + ((var_r26_946 * 0x1360) + 0x610)), s32 *, 0));
                            fn_800798CC(temp_r28_947 + ((var_r26_946 * 0x1360) + 0x310), M2C_FIELD((temp_r28_947 + ((var_r26_946 * 0x1360) + 0x658)), HSD_JObj **, 0));
                            fn_80196C50(M2C_FIELD((temp_r28_947 + ((var_r26_946 * 0x1360) + 0x658)), HSD_JObj **, 0), lbl_80450638);
                            fn_80097CA8(0x3007D, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
                            fn_8009760C(M2C_FIELD((lbl_801F9074 + (cr_g_BattleLoadouts[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C)].parts[1] * 0x14)), s32 *, 0));
                            fn_8009760C(M2C_FIELD((lbl_801F9074 + (cr_g_BattleLoadouts[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C)].parts[1] * 0x14)), s32 *, 4));
                            sp4C = 1;
                        }
                        var_r26_946 += 1;
                    }
                }
            }
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1DB8) == 0) {
                temp_r30_1019 = cr_g_CurrentBattleRobo + 0x1E20;
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x20C4) != 0) {
                    if (M2C_FIELD(temp_r30_1019, s16 *, 0x2A4) == 1) {
                        sp20 = temp_r30_1019 + 0xE4;
                    } else {
                        sp20 = temp_r30_1019 + 0x1BC;
                    }
                    if (M2C_FIELD(temp_r30_1019, s16 *, 0x2A6) <= (s32) M2C_FIELD(sp20, u8 *, 1)) {
                        M2C_FIELD(temp_r30_1019, s16 *, 0x2AA) = (s16) M2C_FIELD(sp20, u8 *, 5);
                        M2C_FIELD(temp_r30_1019, s16 *, 0x2A6) = (s16) (M2C_FIELD(sp20, u8 *, 1) + 1);
                    }
                }
                fn_8003F248(cr_g_CurrentBattleRobo + 0x28D40, (s32) (u8 *) &cr_s_9B60_A30, 4);
            }
        } else {
            if (((void *) M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238) != NULL) && ((M2C_FIELD(M2C_FIELD(cr_g_CurrentBattleRobo, void **, 0x238), f32 *, 0x74) >= lbl_80450654) || (cr_g_GameMode.flags & 0x100000))) {
                temp_f31_1111 = ((lbl_804506A8 * (f32) (M2C_FIELD(lbl_8044F4A4, u16 *, 0x14) * M2C_FIELD(&cr_g_CurrentBodyStats[M2C_FIELD(lbl_8044F4A4, s16 *, 2)], u8 *, 0x47)) * (f32) M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x42)) / (f32) cr_g_BattleParams.unk_00) / lbl_80450658;
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) = (f32) -((lbl_80450700 * temp_f31_1111) - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74));
                fn_8007AB14(M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C), lbl_80450700 * temp_f31_1111);
                if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) <= lbl_80450654) && !(cr_g_GameMode.flags & 0x100000)) {
                    if (M2C_FIELD(lbl_8044F4A4, s16 *, 2) != 5) {
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x46);
                    } else {
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = (s16) cr_g_BattleParams.unk_08[8];
                    }
                    if (!(cr_g_GameMode.flags & 0x100000)) {
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = 0xFF;
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0xFF;
                    }
                }
            }
            if (M2C_FIELD(lbl_8044F4A4, s16 *, 2) != 5) {
                sp1C = 0xE;
            } else {
                sp1C = 0xF;
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) = (s8) sp1C;
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) = spB0;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = spB4;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) = spB8;
            fn_8001E5E4((u8 *) &cr_s_9B60_A6C, cr_g_CurrentBattleRobo + 0x10);
            temp_r5_1188 = (u32) M2C_ERROR(/* unknown instruction: mfcr $r6 */) >> 0x1FU;
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) = (s8) temp_r5_1188;
            if ((s8) temp_r5_1188 != 0) {
                cr_s_9B60_A6C.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
                cr_s_9B60_A6C.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
                cr_s_9B60_A6C.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
                fn_8001CA5C((u8 *) &cr_s_9B60_A6C, lbl_80450694);
            } else {
                cr_s_9B60_A6C.x = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10);
                cr_s_9B60_A6C.y = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
                cr_s_9B60_A6C.z = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18);
            }
            cr_s_9B60_A78.x = (f32) cr_s_9B60_A6C.x;
            cr_s_9B60_A78.y = (f32) cr_s_9B60_A6C.y;
            cr_s_9B60_A78.z = (f32) cr_s_9B60_A6C.z;
            cr_s_9B60_A60.x = (f32) cr_s_9B60_A78.x;
            cr_s_9B60_A60.y = (f32) cr_s_9B60_A78.y;
            cr_s_9B60_A60.z = (f32) cr_s_9B60_A78.z;
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14B) = 0;
            temp_f31_1247 = (lbl_804506C0 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x22)) / lbl_804506C4;
            sp18 = (f32) sin((f64) temp_f31_1247);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = sp18;
            temp_f5_1262 = (f32) cos((f64) temp_f31_1247);
            sp14 = temp_f5_1262;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) *= temp_f5_1262;
            temp_f4_1268 = (f32) cos((f64) temp_f31_1247);
            sp10 = temp_f4_1268;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) *= temp_f4_1268;
            fn_8001CA5C(cr_g_CurrentBattleRobo + 0x10, (f32) (M2C_FIELD(lbl_8044F4A4, s16 *, 0x20) * M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x43)) / lbl_80450700);
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x144) = 0;
            *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 8;
        }
        goto block_116;
    }
block_116:
    spBC = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xEC);
    spC0 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF0);
    spC4 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF4);
    sp8 = fn_8005D068(cr_g_CurrentBattleRobo + 0x33488);
    spF0 = 176.0f;
    spD8 = 0x43300000;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x12C) = (f32) (lbl_804507C0 * (f32) M2C_FIELD(((u8 *) &cr_g_BattleParams + (sp8 * 6)), s16 *, 0x1C));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x130) = (f32) (lbl_804507C4 * (f32) M2C_FIELD(((u8 *) &cr_g_BattleParams + (sp8 * 6)), s16 *, 0x1E));
    spDC = M2C_FIELD(((u8 *) &cr_g_BattleParams + (sp8 * 6)), s16 *, 0x20) ^ 0x80000000;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x134) = (f32) (lbl_804506A8 * ((f32) M2C_BITWISE(f64, spD8) - (f32) lbl_80450660));
    fn_8001CA5C(&spBC, M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x134));
    fn_8005CF0C();
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xF8) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0xFC) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x100) = (f32) M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8);
    sp8C = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1E8);
    temp_r20_1405 = cr_g_CurrentBattleRobo + 0x139;
    temp_r29_1409 = cr_g_CurrentBattleRobo + 0x10;
    sp90 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1EC);
    temp_r21_1414 = cr_g_CurrentBattleRobo + 0x20;
    sp48 = cr_g_CurrentBattleRobo + 0xF8;
    sp94 = -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1F0);
    temp_f20_1419 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x130);
    temp_f21_1420 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x12C);
    fn_8001CCC4((u8 *) &cr_s_9B60_A24, lbl_80450638, lbl_80450638, lbl_80450638);
    if ((u8) *temp_r20_1405 != 2) {
        sp5C = spBC;
        sp60 = spC0;
        sp64 = spC4;
        temp_f1_1432 = fn_8001E424(&sp5C);
        if (temp_f1_1432 > lbl_80450638) {
            fn_8001CA28(&sp5C, &sp8C, -fn_8001E5E4(&sp5C, &sp8C));
            if (fn_8001E424(&sp5C) > lbl_80450638) {
                cr_Vec3_Normalize(&sp5C);
                fn_8001CA5C(&sp5C, temp_f1_1432);
            }
        }
        if (fn_8001E424(temp_r29_1409) > lbl_80450638) {
            fn_8001CA28(temp_r29_1409, &sp8C, -fn_8001E5E4(temp_r29_1409, &sp8C));
        }
        fn_8001CCC4(&sp68, lbl_80450638, lbl_80450638, lbl_80450638);
        fn_8001C9F4(&sp68, &sp5C);
        fn_8001C9F4(&sp68, sp48);
        fn_8001C98C((u8 *) &cr_s_9B60_A24, &sp68, temp_r29_1409);
        if (fn_8001E424((u8 *) &cr_s_9B60_A24) > temp_f21_1420) {
            spC = 1;
        } else {
            spC = 0;
        }
        *temp_r20_1405 = (u8) spC;
        if ((u8) *temp_r20_1405 == 0) {
            M2C_FIELD(temp_r29_1409, f32 *, 0) = sp68;
            M2C_FIELD(temp_r29_1409, f32 *, 4) = sp6C;
            M2C_FIELD(temp_r29_1409, f32 *, 8) = sp70;
        } else {
            fn_8001CA28(temp_r29_1409, (u8 *) &cr_s_9B60_A24, temp_f20_1419);
        }
    }
    M2C_FIELD(temp_r21_1414, f32 *, 0) = (f32) ((lbl_804506A8 * M2C_FIELD(temp_r29_1409, f32 *, 0)) + M2C_FIELD(temp_r21_1414, f32 *, 0));
    M2C_FIELD(temp_r21_1414, f32 *, 4) = (f32) ((lbl_804506A8 * M2C_FIELD(temp_r29_1409, f32 *, 4)) + M2C_FIELD(temp_r21_1414, f32 *, 4));
    M2C_FIELD(temp_r21_1414, f32 *, 8) = (f32) -((lbl_804506A8 * M2C_FIELD(temp_r29_1409, f32 *, 8)) - M2C_FIELD(temp_r21_1414, f32 *, 8));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4) = (f32) lbl_80450638;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8) = (f32) lbl_80450638;
    sp80 = lbl_80450680 * (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334B0) - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334BC));
    sp84 = lbl_80450680 * (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334B4) - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334C0));
    sp88 = lbl_80450680 * (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334B8) - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334C4));
    cr_Vec3_NormalizeTo(cr_g_CurrentBattleRobo + 0x334E0, cr_g_CurrentBattleRobo + 0x334E0);
    var_f29_1581 = fn_8001E5E4(&sp80, cr_g_CurrentBattleRobo + 0x334E0);
    if (var_f29_1581 < lbl_80450638) {
        if (var_f29_1581 < lbl_80450694) {
            var_f29_1581 = lbl_80450694;
        }
        sp74 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334E0);
        sp78 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334E4);
        sp7C = -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334E8);
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334E4) > lbl_80450638) {
                temp_f23_1619 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
                fn_8001CA28(cr_g_CurrentBattleRobo + 0x10, &sp74, lbl_804506A0 * -var_f29_1581);
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) = temp_f23_1619;
            } else {
                fn_8001CA28(cr_g_CurrentBattleRobo + 0x10, &sp74, lbl_804506A0 * -var_f29_1581);
            }
        } else {
            fn_8001CA28(cr_g_CurrentBattleRobo + 0x10, &sp74, -var_f29_1581);
        }
    }
    var_r17_1642 = 0;
    var_f26_1651 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10F0) - M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24);
    sp54 = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139);
    temp_f31_1656 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14);
    if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17F4) * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1C4)) > var_f26_1651) {
        var_f26_1651 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17F4) * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1C4);
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) < lbl_80450654) {
        var_f25_1682 = fn_8006207C(M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3343C), lbl_80450688 + M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1090), M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33444), lbl_80450654);
    } else {
        var_f25_1682 = fn_8006207C(M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3343C), M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1090), M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33444), lbl_80450654);
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33488) = (f32) M2C_FIELD(lbl_8032C288, f32 *, 0);
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3348C) = (f32) lbl_8032C288[1];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33490) = (f32) lbl_8032C288[2];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33494) = (f32) lbl_8032C288[3];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33498) = (f32) lbl_8032C288[4];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3349C) = (f32) lbl_8032C288[5];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0) = (f32) lbl_8032C288[6];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4) = (f32) lbl_8032C288[7];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8) = (f32) lbl_8032C288[8];
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334AC) = (f32) lbl_8032C288[9];
    if (((M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) && ((((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17F4) * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1C4)) + M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24)) - var_f25_1682) < (var_f26_1651 - lbl_80450688))) || ((M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2) && ((((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17F4) * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1C4)) + M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24)) - var_f25_1682) < (lbl_80450648 + var_f26_1651)))) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24) = var_f25_1682;
        spBC = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1E8);
        spC0 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1EC);
        spC4 = -M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1F0);
        fn_8001CA28(cr_g_CurrentBattleRobo + 0x10, &spBC, -fn_8001E5E4(cr_g_CurrentBattleRobo + 0x10, &spBC));
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 1U;
        }
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) = 2U;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) != 2) {
        if (sp54 == 2) {
            temp_f27_1805 = lbl_804506A8 * (lbl_8045064C * lbl_8044F4B0 * M2C_FIELD(&cr_g_BattleParams, f32 *, 8));
            if (temp_f27_1805 > lbl_80450638) {
                temp_f30_1812 = __frsqrte(temp_f27_1805);
                temp_f30_1819 = lbl_80450738 * temp_f30_1812 * -(((f64) temp_f27_1805 * (temp_f30_1812 * temp_f30_1812)) - lbl_80450740);
                temp_f30_1823 = lbl_80450738 * temp_f30_1819 * -(((f64) temp_f27_1805 * (temp_f30_1819 * temp_f30_1819)) - lbl_80450740);
                var_f24_1829 = (f32) ((f64) temp_f27_1805 * (lbl_80450738 * temp_f30_1823 * -(((f64) temp_f27_1805 * (temp_f30_1823 * temp_f30_1823)) - lbl_80450740)));
            } else if (temp_f27_1805 < (f32) lbl_804506B8) {
                var_f24_1829 = __float_nan;
            } else {
                sp3C = temp_f27_1805;
                switch ((M2C_BITWISE(s32, sp3C) & 0x7F800000)) {             /* switch 1; irregular */
                case 0x7F800000:                    /* switch 1 */
                    if (M2C_BITWISE(s32, sp3C) & 0x7FFFFF) {
                        var_r19_1853 = 1;
                    } else {
                        var_r19_1853 = 2;
                    }
                    break;
                case 0x0:                           /* switch 1 */
                    if (M2C_BITWISE(s32, sp3C) & 0x7FFFFF) {
                        var_r19_1853 = 5;
                    } else {
                        var_r19_1853 = 3;
                    }
                    break;
                default:                            /* switch 1 */
                    var_r19_1853 = 4;
                    break;
                }
                if (var_r19_1853 == 1) {
                    var_f24_1829 = __float_nan;
                } else {
                    var_f24_1829 = temp_f27_1805;
                }
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) = (f32) ((lbl_804506A8 * (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) - temp_f31_1656)) / var_f24_1829);
            if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) < lbl_80450638) {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) = (f32) lbl_80450638;
            }
            if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) > lbl_80450654) {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) = (f32) lbl_80450654;
            }
            if (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) != 0xD) && !(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x10000000)) {
                fn_800798CC(cr_g_CurrentBattleRobo + 0x20, M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333D4));
                fn_80196C50(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x333D4), lbl_80450638);
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE8) = 0x3CU;
            }
        }
        if ((u32) (M2C_BITWISE(u32, lbl_8032C288[4]) >> 3U) == 1) {
            var_r17_1642 = 1;
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33488) = (f32) M2C_FIELD(lbl_8032C288, f32 *, 0);
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3348C) = (f32) lbl_8032C288[1];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33490) = (f32) lbl_8032C288[2];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33494) = (f32) lbl_8032C288[3];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33498) = (f32) lbl_8032C288[4];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3349C) = (f32) lbl_8032C288[5];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A0) = (f32) lbl_8032C288[6];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A4) = (f32) lbl_8032C288[7];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334A8) = (f32) lbl_8032C288[8];
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x334AC) = (f32) lbl_8032C288[9];
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1E8) = (f32) ((lbl_804507C8 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1E8)) + (lbl_804507CC * lbl_8032C288[1]));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1EC) = (f32) ((lbl_804507C8 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1EC)) + (lbl_804507CC * lbl_8032C288[2]));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1F0) = (f32) ((lbl_804507C8 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1F0)) + (lbl_804507CC * lbl_8032C288[3]));
    if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x24) < lbl_804507D0) {
        fn_8001CCC4(cr_g_CurrentBattleRobo + 0x20, lbl_80450638, lbl_80450654, lbl_80450638);
    }
    cr_Vec3_Normalize((u8 *) &cr_s_9B60_A6C);
    cr_s_9B60_A60.y = (f32) lbl_80450638;
    if (cr_Vec3_LengthInline((u8 *) &cr_s_9B60_A60) > lbl_80450638) {
        cr_Vec3_Normalize((u8 *) &cr_s_9B60_A60);
    } else {
        cr_s_9B60_A60.x = (f32) cr_s_9B60_A6C.x;
        cr_s_9B60_A60.y = (f32) cr_s_9B60_A6C.y;
        cr_s_9B60_A60.z = (f32) cr_s_9B60_A6C.z;
    }
    cr_s_9B60_A78.y = (f32) lbl_80450638;
    if (cr_Vec3_LengthInline((u8 *) &cr_s_9B60_A78) > lbl_80450638) {
        cr_Vec3_Normalize((u8 *) &cr_s_9B60_A78);
    } else {
        cr_s_9B60_A78.x = (f32) cr_s_9B60_A6C.x;
        cr_s_9B60_A78.y = (f32) cr_s_9B60_A6C.y;
        cr_s_9B60_A78.z = (f32) cr_s_9B60_A6C.z;
    }
    if (fn_8001E5E4((u8 *) &cr_s_9B60_A60, (u8 *) &cr_s_9B60_A78) < lbl_80450638) {
        fn_8001CA5C((u8 *) &cr_s_9B60_A78, lbl_80450694);
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x158) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x158)) + (lbl_80450648 * cr_s_9B60_A6C.x));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x15C) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x15C)) + (lbl_80450648 * cr_s_9B60_A6C.y));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x160) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x160)) + (lbl_80450648 * cr_s_9B60_A6C.z));
    spF0 = (float) (s32) cr_g_CurrentBattleRobo;
    (*(f32 *)((s8 *)(s32) (spF0) + (0x170))) = (f32) ((lbl_80450648 * (*(f32 *)((s8 *)(cr_g_CurrentBattleRobo) + (0x170)))) + (lbl_80450648 * (*(f32 *)((s8 *)(lbl_80329B60) + (0xA60)))));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x174) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x174)) + (lbl_80450648 * cr_s_9B60_A60.y));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x178) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x178)) + (lbl_80450648 * cr_s_9B60_A60.z));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17C) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x17C)) + (lbl_80450648 * cr_s_9B60_A78.x));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x180) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x180)) + (lbl_80450648 * cr_s_9B60_A78.y));
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x184) = (f32) ((lbl_80450648 * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x184)) + (lbl_80450648 * cr_s_9B60_A78.z));
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x64) = (s32) -fn_8001F060(cr_s_9B60_A78.x, cr_s_9B60_A78.z);
    if (((s32) (*(u16 *)((s8 *)(cr_g_CurrentBattleRobo) + (0xDA))) != (s32) ((*(u16 *)((s8 *)(((*(u8 **)((s8 *)(cr_g_CurrentBattleRobo) + (0x554))) + (cr_g_BattleMotionSlot * 2))) + (0))) & 0xFFFF7FFF)) && (!((*(f32 *)((s8 *)(cr_g_CurrentBattleRobo) + (0x74))) < lbl_80450654) || (((s32) (*(u16 *)((s8 *)(cr_g_CurrentBattleRobo) + (0xDA))) != (s32) ((*(u16 *)((s8 *)((*(u8 **)((s8 *)(cr_g_CurrentBattleRobo) + (0x554)))) + (0x34))) & 0xFFFF7FFF)) && ((s32) (*(u16 *)((s8 *)(cr_g_CurrentBattleRobo) + (0xDA))) != (s32) ((*(u16 *)((s8 *)((*(u8 **)((s8 *)(cr_g_CurrentBattleRobo) + (0x554)))) + (0x38))) & 0xFFFF7FFF)))) && (((*(f32 *)((s8 *)(cr_g_CurrentBattleRobo) + (0x550))) = (f32) lbl_80450638, cr_Battle_SetCurrentPointers(), (*(s8 *)((s8 *)(cr_g_CurrentBattleRobo) + (0x85B))) = (s8) (((u16) (*(u16 *)((s8 *)(((*(u8 **)((s8 *)(cr_g_CurrentBattleRobo) + (0x554))) + (cr_g_BattleMotionSlot * 2))) + (0))) >> 0xFU) & 1), temp_r3_2204 = (*(u16 *)((s8 *)(((*(u8 **)((s8 *)(cr_g_CurrentBattleRobo) + (0x554))) + (cr_g_BattleMotionSlot * 2))) + (0))) & 0xFFFF7FFF, lbl_8044F4EC = 0, sp44 = temp_r3_2204, cr_Motion_Play(temp_r3_2204, 1U), ((cr_g_BattleMotionSlot == 0) != 0)) || (cr_g_BattleMotionSlot == 0xC) || (cr_g_BattleMotionSlot == 0xD) || (cr_g_BattleMotionSlot == 7) || (cr_g_BattleMotionSlot == 0xA) || (cr_g_BattleMotionSlot == 0xB) || (cr_g_BattleMotionSlot == 0x11) || (cr_g_BattleMotionSlot == 0x13) || (cr_g_BattleMotionSlot == 0x15) || (cr_g_BattleMotionSlot == 0x16) || (cr_g_BattleMotionSlot == 0x21) || (cr_g_BattleMotionSlot == 0x22) || (cr_g_BattleMotionSlot == 0x20))) {
        M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 2) = 0x10;
    }
    temp_r23_2257 = cr_g_CurrentBattleRobo + 0x18A0;
    M2C_FIELD(temp_r23_2257, s16 *, 2) = 0;
    M2C_FIELD(temp_r23_2257, s16 *, 0x16) = 0;
    M2C_FIELD(temp_r23_2257, s16 *, 0x14) = 0;
    M2C_FIELD(temp_r23_2257, s8 *, 1) = 0;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x187C) *= lbl_804507D4;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1880) *= lbl_804507D4;
    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1884) *= lbl_804507D4;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) < lbl_80450654) {
        if (cr_g_GameMode.flags & 0x100000) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) += lbl_80450700;
        } else {
            var_r25_2296 = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x200;
            sp40 = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x3346C) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33470) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x33474) = (f32) lbl_80450638;
            temp_r27_2322 = cr_g_CurrentBattleRobo + 0x1E20;
            while (var_r25_2296 < 0x20) {
                if ((s8) *(temp_r27_2322 + ((var_r25_2296 * 0x1360) + 0x305)) != 0) {
                    *(temp_r27_2322 + ((var_r25_2296 * 0x1360) + 0x305)) = 0;
                    fn_800750F8(M2C_FIELD((temp_r27_2322 + ((var_r25_2296 * 0x1360) + 0x610)), s32 *, 0));
                    fn_800798CC(temp_r27_2322 + ((var_r25_2296 * 0x1360) + 0x310), M2C_FIELD((temp_r27_2322 + ((var_r25_2296 * 0x1360) + 0x658)), HSD_JObj **, 0));
                    fn_80196C50(M2C_FIELD((temp_r27_2322 + ((var_r25_2296 * 0x1360) + 0x658)), HSD_JObj **, 0), lbl_80450638);
                    fn_80097CA8(0x3007D, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
                    fn_8009760C(M2C_FIELD((lbl_801F9074 + (cr_g_BattleLoadouts[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C)].parts[1] * 0x14)), s32 *, 0));
                    fn_8009760C(M2C_FIELD((lbl_801F9074 + (cr_g_BattleLoadouts[M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C)].parts[1] * 0x14)), s32 *, 4));
                    sp40 = 1;
                }
                var_r25_2296 += 1;
            }
            if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x100000)) {
                *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 0x20;
            }
        }
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x10000000) {
        var_r17_1642 = 0;
    }
    if (var_r17_1642 != 0) {
        if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x200)) {
            fn_80078F00(M2C_FIELD(cr_g_CurrentBattleRobo, s8 **, 0x1E04), M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x1E08));
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 **, 0x1E04) = (signed char *) (fn_800798CC(cr_g_CurrentBattleRobo + 0x20, (HSD_JObj *) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x1E08)));
            fn_80196C50((HSD_JObj *) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x1E08), lbl_80450638);
            M2C_FIELD(lbl_8044F4A4, s16 *, 2) = 5;
            M2C_FIELD(lbl_8044F4A4, u16 *, 0x14) += cr_g_BattleParams.unk_08[9];
            M2C_FIELD(lbl_8044F4A4, s16 *, 0x16) = -1;
            M2C_FIELD(lbl_8044F4A4, s16 *, 0x20) = (s16) cr_g_BattleParams.unk_08[7];
            M2C_FIELD(lbl_8044F4A4, s16 *, 0x22) = 0x5A;
            M2C_FIELD(lbl_8044F4A4, s16 *, 0x24) = (s16) cr_g_BattleParams.unk_08[8];
            M2C_FIELD(lbl_8044F4A4, f32 *, 8) = (f32) cr_s_9B60_A60.x;
            M2C_FIELD(lbl_8044F4A4, f32 *, 0xC) = (f32) cr_s_9B60_A60.y;
            M2C_FIELD(lbl_8044F4A4, f32 *, 0x10) = (f32) cr_s_9B60_A60.z;
            M2C_FIELD(lbl_8044F4A4, u8 *, 1) += 1;
            switch ((s32) (u8) (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x56))) {           /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5E);
                break;
            case 1:                                 /* switch 2 */
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x57);
                break;
            case 2:                                 /* switch 2 */
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5E);
                break;
            }
            if ((u32) cr_g_SeqCurrent.unk_10 == 2) {
                fn_80097CA8(0x3007F, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            }
        } else {
            M2C_FIELD(lbl_8044F4A4, s16 *, 2) = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) = 0xE;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 2;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450720;
        }
    } else if (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 0xF) && (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x200)) {
        M2C_FIELD(lbl_8044F4A4, s16 *, 2) = 0;
        spF0 = (float) (s32) cr_g_CurrentBattleRobo;
        (*(s8 *)((s8 *)(s32) (spF0) + (0x528))) = 0xE;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 2;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450720;
    }
    if ((var_r18_331 != 0) || (((u8) M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x528) == 7) && (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5F) != 0))) {
        M2C_FIELD(lbl_8044F4A4, u8 *, 0) = 1;
    } else {
        M2C_FIELD(lbl_8044F4A4, u8 *, 0) = 0;
    }
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x188) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x104);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x18C) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x108);
    M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x190) = (s32) M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x10C);
    return (s8) sp58;
}
