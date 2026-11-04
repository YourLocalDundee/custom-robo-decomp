/* cr_SEQ_STAGE_SELECTOR_FrameC: Nov 3, 2026 hand draft (m2c 84%): both cameras typed HSD_CObj * (m2c had the first as u32 and its view matrix as +0x54), which gives the original register order */

void cr_SEQ_STAGE_SELECTOR_FrameC(void) {
    HSD_CObj *temp_r31_14;
    HSD_CObj *temp_r30_49;

    if (HSD_CObjSetCurrent((HSD_CObj *) *lbl_80323270) != 0) {
        temp_r31_14 = (HSD_CObj *) *lbl_80323270;
        (temp_r31_14 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        temp_r31_14->flags = temp_r31_14->flags | 0x80000000 | 2;
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (f32 (*)[4]) temp_r31_14->view_mtx[0]);
        HSD_LObjSetCurrentAll((HSD_LObj *) *lbl_80354154);
        HSD_LObjSetupInit((HSD_CObj *) *lbl_80323270);
        HSD_StateInvalidate(-1);
        fn_8011266C();
        HSD_CObjEndCurrent();
    }
    if (HSD_CObjSetCurrent(lbl_8044F468) != 0) {
        temp_r30_49 = lbl_8044F468;
        (temp_r30_49 != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
        temp_r30_49->flags = temp_r30_49->flags | 0x80000000 | 2;
        PSMTXCopy((f32 (*)[4]) lbl_8032344C, (f32 (*)[4]) temp_r30_49->view_mtx[0]);
    }
}
