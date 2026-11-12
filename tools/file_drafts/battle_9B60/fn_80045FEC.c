/* fn_80045FEC wip 80% (m2c 70%): the falls action (action 14; slots 0x1A-0x1D; knocked-out count via the inline cr_KnockOutCount (limits 240 on the ground, 60 in the air)); chained counters, file statics. Left: one more float local in the original (it saves f26-f31), the flag-table index registers as in fn_80045CE8 */

extern u8 *lbl_8044F4A4;

static inline void cr_KnockOutCount_5FEC(u16 limit) {
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x800000) {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1D6) < limit) {
            if ((u32) cr_g_SeqCurrent.unk_10 == 2) {
                M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1D6) += 1;
            } else {
                *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 0x40;
            }
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x100000;
            fn_8006E690(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x860));
            *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 0x40;
        }
    } else {
        M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) |= 0x800000;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x1D6) = 0U;
    }
}

s32 fn_80045FEC(void) {
    f32 var_f31_27;
    f32 temp_f30_28;
    u8 *temp_r31_103;
    u8 var_r29_122;
    s32 var_r28_72;
    s32 var_r27_87;
    s32 var_r26_203;
    s32 var_r25_263;

    var_f31_27 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548);
    temp_f30_28 = M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548);
    *((u16 *) (lbl_80386378 + (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C) * 0x34))) |= 4;
    if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) > 0) {
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) < lbl_80450654) && !(cr_g_GameMode.flags & 0x100000) && (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) < 0x1E)) {
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0x1E;
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x139) == 2) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14B) != 0) {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_8045077C;
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) != 0) {
                    var_r28_72 = 0x1D;
                } else {
                    var_r28_72 = 0x1B;
                }
                cr_g_BattleMotionSlot = var_r28_72;
            } else {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) != 0) {
                    var_r27_87 = 0x1C;
                } else {
                    var_r27_87 = 0x1A;
                }
                cr_g_BattleMotionSlot = var_r27_87;
                if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) < lbl_80450654) && !(cr_g_GameMode.flags & 0x100000)) {
                    temp_r31_103 = cr_g_CurrentBattleRobo + 0x18A0;
                    if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x18B8) == 0) && (M2C_FIELD(temp_r31_103, s16 *, 0x1A) == 0) && (M2C_FIELD(temp_r31_103, s16 *, 0x1E) == 0) && (M2C_FIELD(temp_r31_103, s16 *, 0x20) == 0) && (M2C_FIELD(temp_r31_103, s16 *, 0x22) == 0) && (M2C_FIELD(temp_r31_103, s16 *, 0x24) == 0)) {
                        var_r29_122 = 1;
                    } else {
                        var_r29_122 = 0;
                    }
                    if (var_r29_122 != 0) {
                        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) += 1;
                        cr_KnockOutCount_5FEC(0xF0);
                    }
                }
            }
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2);
        } else {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14B) != 0) {
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) != 0) {
                    var_r26_203 = 0x1D;
                } else {
                    var_r26_203 = 0x1B;
                }
                cr_g_BattleMotionSlot = var_r26_203;
                if ((s32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)) < 0x14) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (lbl_80450774 + (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)));
                } else if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) < 0xF) {
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (lbl_80450780 + (f32) (0xF - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)));
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450780;
                }
            } else {
                if (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x14A) != 0) {
                    var_r25_263 = 0x1C;
                } else {
                    var_r25_263 = 0x1A;
                }
                cr_g_BattleMotionSlot = var_r25_263;
                if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) >= lbl_80450654) || (cr_g_GameMode.flags & 0x100000)) {
                    if ((s32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)) < 0x1E) {
                        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2));
                    } else if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) <= 0x1E) {
                        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (lbl_80450720 + (lbl_80450720 - (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2)));
                    } else {
                        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450784;
                    }
                } else {
                    M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) += 1;
                    M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450784;
                    cr_KnockOutCount_5FEC(0x3C);
                }
            }
            if (lbl_80450638 != M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C)) {
                M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x13C) = (f32) lbl_80450638;
                fn_80097CA8(0x30080, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            }
            switch ((s32) (u8) (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x56))) {            /* irregular */
            case 0:
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5E);
                break;
            case 1:
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x57);
                break;
            case 2:
                M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13B) = M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5E);
                break;
            }
            fn_8000805C(cr_g_CurrentBattleRobo + 0x104, (s32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242));
        }
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) >= lbl_80450654) && (M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) & 0x200) && (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x146) == 0)) {
            fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x528) = 0x12U;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0xA;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450638;
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0xE6) = 3;
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) -= lbl_804506A8 * (lbl_8044F4B4 * (f32) M2C_FIELD(lbl_8044F4A4, s16 *, 0x24));
    } else if ((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x74) >= lbl_80450654) || (cr_g_GameMode.flags & 0x100000)) {
        M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x528) = 0x10U;
        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) = M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) = 0x1E;
        fn_8001CCC4(cr_g_CurrentBattleRobo + 0x10, lbl_80450638, lbl_80450638, lbl_80450638);
        M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0xE6) = (s8) cr_g_BattleParams.unk_08[3];
        fn_80098290(0x3007C);
    }
    cr_s_9B60_A60 = cr_s_9B60_A6C;
    cr_s_9B60_A78 = cr_s_9B60_A60;
    if ((M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x528) == 0xE) && ((f32) fabs(M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) - var_f31_27) > lbl_80450684)) {
        cr_Battle_SetCurrentPointers();
        fn_800528B8();
        if (var_f31_27 > lbl_80450680) {
            var_f31_27 = lbl_80450680;
        }
        M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 2) = (s16) var_f31_27;
    }
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x200) {
        if (temp_f30_28 < lbl_80450784) {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (temp_f30_28 + lbl_80450654);
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450784;
        }
    }
    return 1;
}
