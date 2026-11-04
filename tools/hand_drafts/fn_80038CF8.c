/* fn_80038CF8: Nov 3, 2026 hand draft (m2c 96%, then decomp-permuter): the 0x964 / 0x968 tests as cast-NULL compares (`li r0, 0; cmplw`), and the flash colours stored without the (s8) casts m2c put on the float-to-byte conversions */

void fn_80038CF8(u32 *arg0) {
    u32 var_r30_8;

    var_r30_8 = 0U;
    if ((HSD_MObj *) M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0) != NULL) {
        M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.r = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.g = 0;
        M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.b = 0;
        ((HSD_TObj *) (M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj))->flags &= 0xFFF8FFFF;
        M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj->blending = lbl_80450654;
        if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x187C) > lbl_80450668) {
            var_r30_8 = 1U;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.r = lbl_8045066C * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x187C);
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.g = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.b = 0;
            ((HSD_TObj *) (M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj))->flags |= 0x70000;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj->blending = -((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x187C) * lbl_80450648) - lbl_80450654);
        } else if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1884) > lbl_80450668) {
            var_r30_8 = 1U;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.r = lbl_8045066C * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1884);
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.g = lbl_8045066C * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1884);
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.b = 0;
            ((HSD_TObj *) (M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj))->flags |= 0x70000;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj->blending = -((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1884) * lbl_80450648) - lbl_80450654);
        } else if (M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1880) > lbl_80450668) {
            var_r30_8 = 1U;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.r = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.g = 0;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->diffuse.b = lbl_8045066C * M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1880);
            ((HSD_TObj *) (M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj))->flags |= 0x70000;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj->blending = -((M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x1880) * lbl_80450648) - lbl_80450654);
        }
        if ((M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0xE6) != 0) || (M2C_FIELD(cr_g_CurrentBattleRobo, s8 *, 0x1C8) == 0)) {
            var_r30_8 = 1U;
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->alpha = lbl_80450648;
            ((HSD_MObj *) (M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)))->rendermode |= 0x40000000;
        } else {
            M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->mat->alpha = lbl_80450654;
            ((HSD_MObj *) (M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)))->rendermode &= 0xBFFFFFFF;
        }
        if (var_r30_8 != 0U) {
            *arg0 = 0;
        }
        if ((u32) *arg0 == 1) {
            fn_800943D8(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0)->tobj);
        } else {
            HSD_MObjSetup(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x8B0), 7U);
            *arg0 = 1;
        }
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x860), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
    }
    if ((u8 *) M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x964) != (u8 *) NULL) {
        if ((u32) *arg0 == 1) {
            fn_800943D8(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x964)->tobj);
        } else {
            HSD_MObjSetup(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x964), 7U);
            *arg0 = 1;
        }
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x944), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
    }
    if ((u8 *) M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x968) != (u8 *) NULL) {
        if ((u32) *arg0 == 1) {
            fn_800943D8(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x968)->tobj);
        } else {
            HSD_MObjSetup(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x968), 7U);
            *arg0 = 1;
        }
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x948), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
    }
    if ((HSD_MObj *) M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x96C) != NULL) {
        if ((u32) *arg0 == 1) {
            fn_800943D8(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x96C)->tobj);
        } else {
            HSD_MObjSetup(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x96C), 7U);
            *arg0 = 1;
        }
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x950), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x94C), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
    }
    if ((HSD_MObj *) M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x970) != NULL) {
        if ((u32) *arg0 == 1) {
            fn_800943D8(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x970)->tobj);
        } else {
            HSD_MObjSetup(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_MObj **, 0x970), 7U);
            *arg0 = 1;
        }
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x954), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x958), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x95C), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
        fn_80095E6C(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x960), (f32 (*)[4]) lbl_8032344C, HSD_TRSP_ALL, 0x04000000U);
    }
    if (var_r30_8 != 0U) {
        *arg0 = 0;
    }
}
