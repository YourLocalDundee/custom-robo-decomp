/* fn_80048708 wip 96% (Nov 6, with the pool: file statics cr_s_9B60_*; was before): m2c 68; the robo-position copies as Vec struct copies (84), lbl_80329B60 held in a local declared first (87), the three view vectors as one chained assignment a6c = a78 = a60 = robo->x110 (92); left: the scheduling of that chain and of the rotate call's arguments */

s32 fn_80048708(void) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    s32 var_r31_8;

    var_r31_8 = 0;
    cr_g_BattleMotionSlot = 0xA;
    if (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) > 0) {
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) (((lbl_80450790 * (f32) (M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0) - M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2))) / (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE0)) / lbl_80450720);
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE2) -= 1) <= 0) {
            if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x1BF7) != 0) {
                fn_80097CA8(0x3007A, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            } else {
                fn_80097CA8(0x30000, (Point3d *) (cr_g_CurrentBattleRobo + 0x20));
            }
        }
        var_r31_8 = 1;
    } else {
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) != 0) {
            M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x10) = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x110);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10) *= (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33436));
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x14) *= (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33436));
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x18) *= (lbl_80450688 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x33436));
            sp.sp8 = lbl_80450638;
            sp.spC = lbl_80450654;
            sp.sp10 = lbl_80450638;
            cr_Vec3_Cross(&sp.sp8, &sp.sp8, cr_g_CurrentBattleRobo + 0x110);
            cr_Vec3_RotateAboutAxis(cr_g_CurrentBattleRobo + 0x10, &sp.sp8, (lbl_804506C0 * (f32) M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1BF8)) / lbl_804506C4);
            cr_s_9B60_A6C = cr_s_9B60_A78 = cr_s_9B60_A60 = M2C_FIELD(cr_g_CurrentBattleRobo, Vec *, 0x110);
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) -= 1;
            if (M2C_FIELD(cr_g_CurrentBodyStats, u8 *, 0x5F) == 0) {
                var_r31_8 = 1;
            }
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x13A) = 0U;
            var_r31_8 = 1;
            M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x528) = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0xE4) = (s16) M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x33430);
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x128) = (f32) lbl_80450638;
        }
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x548) = (f32) lbl_80450794;
    }
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
    return var_r31_8;
}
