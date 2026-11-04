/* cr_SEQ_CUSTOMIZE_FrameC: Nov 3, 2026 hand draft (m2c 73%): the saved matrix is a Mtx (m2c: f32[4]), the cameras typed HSD_CObj *, and in the loop the dirty flags or-ed as one constant, `flags | (0x80000000 | 2)` (a macro of the two flags, most likely) */

void cr_SEQ_CUSTOMIZE_FrameC(void) {
    Mtx sp8;
    u8 var_r31_14;
    HSD_CObj *temp_r30_29;
    HSD_CObj *temp_r29_68;

    PSMTXCopy((f32 (*)[4]) lbl_8032344C, sp8);
    HSD_StateInvalidate(-1);
    var_r31_14 = 0;
    while (var_r31_14 < (u32) lbl_8044F504) {
        if (HSD_CObjSetCurrent((struct HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_14 * 4) & 0x3FC)))))) != 0) {
            temp_r30_29 = (HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_14 * 4) & 0x3FC)))));
            (temp_r30_29 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
            temp_r30_29->flags = temp_r30_29->flags | (0x80000000 | 2);
            PSMTXCopy((float (*)[4]) (lbl_8032338C + (var_r31_14 * 0x30)), (f32 (*)[4]) temp_r30_29->view_mtx[0]);
            fn_801010A8(var_r31_14);
            HSD_CObjEndCurrent();
        }
        var_r31_14 += 1;
    }
    PSMTXCopy(sp8, (f32 (*)[4]) lbl_8032344C);
    if (HSD_CObjSetCurrent(lbl_8044F468) != 0) {
        temp_r29_68 = lbl_8044F468;
        (temp_r29_68 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        temp_r29_68->flags = temp_r29_68->flags | 0x80000000 | 2;
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (f32 (*)[4]) temp_r29_68->view_mtx[0]);
    }
}
