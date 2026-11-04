/* cr_SEQ_TITLE_Leave: Nov 4, 2026 hand draft (m2c 97%, permuter 15): the clear colour as a literal initialiser {0, 0, 0, 255} (the pooled .sdata2 bytes lbl_8045025C..F); a copy from the label lets the scheduler pull the four byte loads into the prologue */
void cr_SEQ_TITLE_Leave(void) {
    GXColor c = {0, 0, 0, 255};
    void **var_r31;
    s32 var_r30;
    s32 var_r29;

    fn_80075BBC();
    HSD_JObjUnref(lbl_8044F398);
    var_r31 = lbl_8044F39C;
    if (var_r31 != NULL) {
        var_r30 = M2C_FIELD(var_r31, u16 *, 4) == 0xFFFF;
        if (var_r30 != 0) {
            var_r29 = var_r30;
        } else {
            var_r30 = M2C_FIELD(var_r31, u16 *, 4) == 0;
            M2C_FIELD(var_r31, u16 *, 4) -= 1;
            var_r29 = var_r30;
        }
        if ((var_r29 != 0) && (var_r31 != NULL)) {
            M2C_FIELD(*var_r31, void (**)(void *), 0x30)(var_r31);
            M2C_FIELD(*var_r31, void (**)(void *), 0x34)(var_r31);
        }
    }
    {
        GXColor t = c;
        HSD_VIData.current.vi.clear_clr = t;
    }
}
