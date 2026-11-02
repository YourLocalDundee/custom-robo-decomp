/* fn_80096374: hand-written from main.dol's asm on its m2c draft (step 109b): the current camera tested against ((void *) 0) (li r0, 0; cmplw) and its view matrix taken into a local first (addi r25, r26, 0x54) */
void fn_80096374(f32 (*arg0)[4], HSD_JObj *arg1, f32 (*arg2)[4]) {
    HSD_JObj *temp_r30_33;
    s32 var_r28_20;
    s32 var_r27_46;
    HSD_CObj *temp_r3_75;
    f32 (*m)[4];

    if (arg1 != NULL) {
        (arg1 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
        var_r28_20 = 0;
        if (!(arg1->flags & 0x800000) && (arg1->flags & 0x40)) {
            var_r28_20 = 1;
        }
        if (var_r28_20 != 0) {
            HSD_JObjSetupMatrixSub(arg1);
        }
    }
    temp_r30_33 = arg1->child;
    if (temp_r30_33 != NULL) {
        (temp_r30_33 != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25DU, lbl_80451097);
        var_r27_46 = 0;
        if (!(temp_r30_33->flags & 0x800000) && (temp_r30_33->flags & 0x40)) {
            var_r27_46 = 1;
        }
        if (var_r27_46 != 0) {
            HSD_JObjSetupMatrixSub(temp_r30_33);
        }
    }
    PSMTXInverse((f32 (*)[4]) arg1->child->mtx[0], arg2);
    PSMTXConcat((f32 (*)[4]) arg1->mtx[0], arg2, arg2);
    if (arg0 != NULL) {
        PSMTXConcat(arg0, arg2, arg2);
        return;
    }
    temp_r3_75 = HSD_CObjGetCurrent();
    if (temp_r3_75 != ((void *) 0)) {
        m = (f32 (*)[4]) temp_r3_75->view_mtx[0];
        PSMTXConcat(m, arg2, arg2);
    }
}
