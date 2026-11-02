/* fn_80095D7C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the inlined HSD_JObjSetupMatrix as Melee writes it: `if (j == NULL || !dirty(j)) return; HSD_JObjSetupMatrixSub(j);` with jobj.h's HSD_JObjMtxIsDirty as a static inline (bne / b, not beq) */
static inline s32 cr_dirty(HSD_JObj *jobj) {
    s32 result;

    (jobj != NULL) ? (void) 0 : __assert(lbl_80451090, 0x25D, lbl_80451097);
    result = 0;
    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {
        result = 1;
    }
    return result;
}

void fn_80095D7C(HSD_JObj *arg0) {
    if (arg0 == NULL || !cr_dirty(arg0)) {
        return;
    }
    HSD_JObjSetupMatrixSub(arg0);
}
