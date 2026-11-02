/* cr_SEQ_MEMORY_CARD_Enter: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the fill loop's counter an int (m2c: s8, an extsb per use) */
extern u8 *lbl_8044F7EC;

void cr_SEQ_MEMORY_CARD_Enter(void) {
    s32 var_r31_34;

    lbl_8044F38C = 0;
    lbl_8044F390 = 0;
    fn_80092108(2U);
    fn_80065C68();
    cr_g_SeqCurrent.unk_40 = 2;
    cr_g_SeqCurrent.unk_24 = 1;
    lbl_8044F7D8 = cr_Heap2_AllocAligned(0x1E000);
    fn_800F5EF0(lbl_8044F7D8, &lbl_80400420[0xFC]);
    lbl_8044F7EC = cr_Heap2_AllocAligned(0xE000);
    for (var_r31_34 = 0; var_r31_34 < 0x10000; var_r31_34++) {
        lbl_8044F7EC[var_r31_34] = var_r31_34;
    }
}
