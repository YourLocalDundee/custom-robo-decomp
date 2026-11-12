/* fn_800B1944: Nov 6, 2026 hand draft (m2c 95%): m2c's draft with one statement moved: the joint pointer read after the base pointer of the next statement (only the statement order differed; found by the adjacent-swap sweep) */

void fn_800B1944(s32 arg0) {
    HSD_JObj *temp_r31_54;
    u8 var_r30_6;
    s32 var_r29_47;
    s32 var_r28_29;
    u8 *temp_r27_55;
    s32 var_r26_92;

    var_r30_6 = 0;
    while (var_r30_6 < 0x18U) {
        if (M2C_FIELD((lbl_803BC028 + (var_r30_6 * 0x174)), u32 *, 0) != 0U) {
            lbl_8044F748 = lbl_803BC028 + (var_r30_6 * 0x174);
            if (!(M2C_FIELD(lbl_8044F748, s32 *, 0x88) & 0x200000)) {
                var_r28_29 = 0;
                if (arg0 != 0) {
                    if (M2C_FIELD(lbl_8044F748, s32 *, 0x15C) & 0x40) {
                        var_r28_29 = 1;
                    }
                } else if (!(M2C_FIELD(lbl_8044F748, s32 *, 0x15C) & 0x40)) {
                    var_r28_29 = 1;
                }
                if (var_r28_29 != 0) {
                    var_r29_47 = 0;
                    while (M2C_FIELD(&lbl_8044F748[(var_r29_47 * 4) & 0x3FC], void **, 0x90) != ((void *) 0)) {
                        temp_r27_55 = lbl_8044F748 + 0xB8;
                        temp_r31_54 = M2C_FIELD(&lbl_8044F748[(var_r29_47 * 4) & 0x3FC], HSD_JObj **, 0x90);
                        (temp_r31_54 != NULL) ? (void) 0 : __assert(lbl_804513BC, 0x495U, lbl_804513C3);
                        (temp_r27_55 != NULL) ? (void) 0 : __assert(lbl_804513BC, 0x496U, lbl_804513C8);
                        PSMTXCopy((f32 (*)[4]) temp_r27_55, (float (*)[4]) ((HSD_JObj *) ((u8 *) temp_r31_54 + 0x44)));
                        temp_r31_54->flags |= 0x03800000;
                        if (temp_r31_54 != NULL) {
                            (temp_r31_54 != NULL) ? (void) 0 : __assert(lbl_804513BC, 0x25DU, lbl_804513C3);
                            var_r26_92 = 0;
                            if (!(temp_r31_54->flags & 0x800000) && (temp_r31_54->flags & 0x40)) {
                                var_r26_92 = 1;
                            }
                            if (var_r26_92 == 0) {
                                HSD_JObjSetMtxDirtySub(temp_r31_54);
                            }
                        }
                        HSD_JObjDispAll(M2C_FIELD(&lbl_8044F748[(var_r29_47 * 4) & 0x3FC], HSD_JObj **, 0x90), (f32 (*)[4]) lbl_8032344C, 5U, 0U);
                        var_r29_47 += 1;
                    }
                }
            }
        }
        var_r30_6 += 1;
    }
}
