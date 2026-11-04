/* fn_8003D500: Nov 3, 2026 hand draft (m2c 82%): __fabs (the intrinsic; m2c fabs() kept three saved FP registers), the double constant compared without the (f32) casts, and the matrix source read before the joint in the loop */

void fn_8003D500(void *arg0) {
    s32 var_r30;
    f32 (*var_r28)[4];
    HSD_JObj *var_r29;
    s32 var_r27;

    if (M2C_FIELD(arg0, f32 *, 0xA8) > M2C_FIELD(arg0, f32 *, 0xA4)) {
        M2C_FIELD(arg0, f32 *, 0xA8) -= lbl_80450668;
    }
    if (M2C_FIELD(arg0, f32 *, 0xA8) < M2C_FIELD(arg0, f32 *, 0xA4)) {
        M2C_FIELD(arg0, f32 *, 0xA8) += lbl_80450668;
    }
    if (__fabs(M2C_FIELD(arg0, f32 *, 0xA8) - M2C_FIELD(arg0, f32 *, 0xA4)) < lbl_80450668) {
        M2C_FIELD(arg0, f32 *, 0xA8) = (f32) M2C_FIELD(arg0, f32 *, 0xA4);
    }
    M2C_FIELD(arg0, f32 *, 0xB4) = (f32) M2C_FIELD(arg0, f32 *, 0xA8);
    if (M2C_FIELD(arg0, f32 *, 0xB4) < lbl_80450638) {
        M2C_FIELD(arg0, f32 *, 0xB4) = (f32) lbl_80450638;
    }
    if (M2C_FIELD(arg0, f32 *, 0xB4) > lbl_80450654) {
        M2C_FIELD(arg0, f32 *, 0xB4) = (f32) lbl_80450654;
    }
    if (lbl_80450654 == M2C_FIELD(arg0, f32 *, 0xB4)) {
        fn_80007AE4(1);
    } else {
        fn_80007AE4(0);
    }
    fn_80007AE4(1);
    if (M2C_FIELD(arg0, f32 *, 0xA8) > lbl_804506B8) {
        fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x954));
        fn_80095A30(M2C_FIELD(cr_g_CurrentBattleRobo, HSD_JObj **, 0x95C));
        var_r30 = 0;
        while (var_r30 < 4) {
            if ((*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 3)][0x8F0] != 0U) && (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC] != 0U) && (M2C_FIELD(*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC], u32 *, 0x90) != 0U)) {
                var_r28 = (float (*)[4]) (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 3)][0x8F0] + 0x44);
                var_r29 = M2C_FIELD(*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC], HSD_JObj **, 0x90);
                (var_r29 != NULL) ? (void) 0 : __assert(lbl_80450670, 0x495U, lbl_80450677);
                (var_r28 != NULL) ? (void) 0 : __assert(lbl_80450670, 0x496U, lbl_8045067C);
                PSMTXCopy(var_r28, (float (*)[4]) ((HSD_JObj *) ((u8 *) var_r29 + 0x44)));
                var_r29->flags |= 0x03800000;
                if (var_r29 != NULL) {
                    (var_r29 != NULL) ? (void) 0 : __assert(lbl_80450670, 0x25DU, lbl_80450677);
                    var_r27 = 0;
                    if (!(var_r29->flags & 0x800000) && (var_r29->flags & 0x40)) {
                        var_r27 = 1;
                    }
                    if (var_r27 == 0) {
                        HSD_JObjSetMtxDirtySub(var_r29);
                    }
                }
            }
            var_r30 += 1;
        }
    }
    if (lbl_804506B8 == M2C_FIELD(arg0, f32 *, 0xA8)) {
        var_r30 = 0;
        while (var_r30 < 4) {
            if (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC] != 0U) {
                fn_80078EE8((void *) (*(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC]));
                *(u32 *) &((u8 (*)[0x4]) (cr_g_CurrentBattleRobo))[(var_r30 + 1)][0x333DC] = 0;
            }
            var_r30 += 1;
        }
    }
}
