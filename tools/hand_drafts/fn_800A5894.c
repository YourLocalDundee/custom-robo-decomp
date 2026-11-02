/* fn_800A5894: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the list `((u8 (*)[4]) p)[i][0x90]` (addi + lwzx) tested against ((void *) 0) */
void fn_800A5894(void) {
    s32 var_r31_7;

    fn_800B1FBC();
    var_r31_7 = 0;
    while (*(HSD_JObj * *) &((u8 (*)[4]) lbl_8044F748)[var_r31_7][0x90] != ((void *) 0)) {
        fn_80094DD8(*(HSD_JObj * *) &((u8 (*)[4]) lbl_8044F748)[var_r31_7][0x90], lbl_80451280);
        var_r31_7 += 1;
    }
    M2C_FIELD(lbl_8044F748, f32 *, 0x14) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 8);
    M2C_FIELD(lbl_8044F748, f32 *, 0x18) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0xC);
    M2C_FIELD(lbl_8044F748, f32 *, 0x1C) = (f32) M2C_FIELD(lbl_8044F748, f32 *, 0x10);
    M2C_FIELD(lbl_8044F748, s32 *, 0x15C) |= 0x80;
    if ((lbl_803BFE68[3] == 0xC) || (lbl_803BFE68[3] == 0x1A) || (lbl_803BFE68[3] == 0x1B)) {
        M2C_FIELD(lbl_8044F748, f32 *, 0x170) = (f32) lbl_80451280;
        M2C_FIELD(lbl_8044F748, s32 *, 4) = 5;
        M2C_FIELD(lbl_8044F748, s16 *, 0x16C) = 0xF0;
        fn_80098104(0x40079, 0x7FU);
        fn_8001F6BC(0, 0xF0U);
    } else {
        M2C_FIELD(lbl_8044F748, f32 *, 0x170) = (f32) lbl_8045138C;
    }
    M2C_FIELD(lbl_8044F748, s32 *, 0x88) &= 0xFFDFFFFF;
}
