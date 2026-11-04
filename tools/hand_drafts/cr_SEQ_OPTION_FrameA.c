/* cr_SEQ_OPTION_FrameA: Nov 3, 2026 hand draft (m2c 86%): the sub-frame table entries take no argument and return u8 (m2c passed the index) */

void cr_SEQ_OPTION_FrameA(void) {
    u8 r;

    if ((u32) cr_g_SeqCurrent.unk_24 == 0) {
        cr_g_SeqCurrent.unk_40 = 2;
        cr_g_SeqCurrent.unk_24 = 1;
        fn_80092108(1U);
        lbl_8044F364 = 0;
    }
    r = (*((u8 (**)(void)) (lbl_801E1588 + (lbl_8044F364 * 4))))();
    if (r == 0xF0) {
        cr_g_SeqCurrent.next = SEQ_MODE_SELECTOR;
        lbl_8044F364 = 0;
        return;
    }
    lbl_8044F364 = r;
}
