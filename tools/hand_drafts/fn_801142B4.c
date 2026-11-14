/* fn_801142B4: Nov 13, 2026 hand draft: the mode is the third parameter (u8); the clear colour a GXColor local with a zero initialiser (the four .sbss2 bytes), copied through a block local into HSD_VIData; the scene pointer a local */

void fn_801142B4(void *arg0, s32 arg1, u8 arg2) {
    GXColor c = { 0, 0, 0, 0 };
    u32 *var_r30;

    if (arg2 == 0) {
        fn_80074FBC(lbl_8044F2E8, lbl_80354110);
        M2C_FIELD(arg0, s8 *, 4) = 4;
        M2C_FIELD(arg0, s8 *, 5) = 1;
    } else if (arg2 == 1) {
        var_r30 = lbl_8037569C;
        fn_80072A58(var_r30, lbl_8034C100);
        M2C_FIELD(arg0, s8 *, 4) = 4;
        {
            GXColor t = c;
            HSD_VIData.current.vi.clear_clr = t;
        }
        fn_80072064();
        fn_8006FB88();
        *lbl_8037569C = (u32) fn_80070130;
        M2C_FIELD(arg0, s8 *, 5) = 1;
    }
}
