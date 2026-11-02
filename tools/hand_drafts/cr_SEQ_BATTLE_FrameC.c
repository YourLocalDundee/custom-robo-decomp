/* cr_SEQ_BATTLE_FrameC: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the camera matrix set through a static inline (cobj.h's assert at 0x1A2, the flags, PSMTXCopy into view_mtx): each inlined parameter takes its own saved register (r30 in the loop, r29 after it; m2c's two temporaries swapped them); fn_80026F58 takes the u8 index (draft_batch.py PARAM_SET) */
static inline void cr_CObjSetMtx(HSD_CObj *cobj, f32 (*mtx)[4]) {
    (cobj != NULL) ? (void) 0 : __assert(lbl_80450328, 0x1A2U, lbl_8045032F);
    cobj->flags = cobj->flags | 0x80000000 | 2;
    PSMTXCopy(mtx, cobj->view_mtx);
}

void cr_SEQ_BATTLE_FrameC(void) {
    u8 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < (u8) cr_g_CamSystem.unk_08) {
        lbl_8044F47C = cr_g_Cams + (var_r31_6 * 0x13D0);
        PSMTXCopy((float (*)[4]) (lbl_8032338C + (var_r31_6 * 0x30)), (float (*)[4]) ((f32 (*)[4]) lbl_8032344C));
        if (HSD_CObjSetCurrent((struct HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_6 * 4) & 0x3FC)))))) != 0) {
            cr_CObjSetMtx((HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_6 * 4) & 0x3FC))))), (f32 (*)[4]) lbl_8032344C);
            HSD_LObjSetCurrentAll((HSD_LObj *) *lbl_80354154);
            HSD_LObjSetupInit((struct HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((var_r31_6 * 4) & 0x3FC))))));
            fn_80026F58(var_r31_6);
            HSD_StateInvalidate(-1);
            fn_80071974(0U);
            fn_80083C78(0U);
            fn_80038BD8();
            fn_80071974(1U);
            fn_800389C8(1);
            fn_800389C8(6);
            fn_80079080();
            fn_80075C54();
            fn_80071974(2U);
            fn_80083C78(1U);
            HSD_CObjEndCurrent();
        }
        var_r31_6 += 1;
    }
    if (HSD_CObjSetCurrent(lbl_8044F468) != 0) {
        cr_CObjSetMtx(lbl_8044F468, (f32 (*)[4]) lbl_8032344C);
    }
}
