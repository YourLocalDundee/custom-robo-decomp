/* cr_SEQ_TITLE_FrameA: Nov 3, 2026 hand draft (m2c 94%): fn_800EB518 called as void (its result, a coincidental r3 = -1, is not read; the call then frees r3 for the next address), and in case 1 the flag set before the state store */

void cr_SEQ_TITLE_FrameA(void) {
    u8 var_r31;

    switch ((s32) cr_g_SeqCurrent.unk_10) {         /* irregular */
    case 0:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8002)) {
            ((void (*)(s32)) fn_800EB518)(0);
            cr_g_SeqCurrent.unk_10 = 1;
        } else if ((u32) (cr_g_SeqCurrent.unk_40 & 0x8040) == 0x8040) {
            cr_g_SeqCurrent.unk_40 = 2;
        }
        break;
    case 1:
        if (fn_800EB158() == 0) {
            cr_g_SeqCurrent.unk_40 |= 1;
            cr_g_SeqCurrent.unk_10 = 2;
        }
        break;
    case 2:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8000) && (fn_80098F4C() == 0)) {
            cr_g_SeqCurrent.unk_40 |= 2;
            cr_g_SeqCurrent.unk_38 |= 0x20000;
            fn_80016600();
            fn_800972D8(0x2D);
            cr_g_SeqCurrent.unk_2c = 0x708;
            cr_g_SeqCurrent.unk_10 = 3;
            cr_g_SeqCurrent.unk_38 |= 0x200000;
        }
        break;
    case 3:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8000)) {
            cr_g_SeqCurrent.unk_10 = 4;
        }
        break;
    case 4:
        if ((u32) cr_g_SeqCurrent.unk_2c != 0) {
            cr_g_SeqCurrent.unk_2c -= 1;
        } else {
            cr_g_SeqCurrent.next = SEQ_MOVIE;
            cr_g_SeqCurrent.unk_10 = 5;
        }
        var_r31 = 0;
        while (var_r31 < 4U) {
            if ((cr_g_Pads[var_r31].unk_14 & 0x10) && ((enum cr_Seq) cr_g_SeqCurrent.next != SEQ_MODE_SELECTOR)) {
                cr_g_SeqCurrent.next = SEQ_MODE_SELECTOR;
                fn_80098290(2);
            }
            var_r31 += 1;
        }
        break;
    case 5:
        var_r31 = 0;
        while (var_r31 < 4U) {
            if ((cr_g_Pads[var_r31].unk_14 & 0x10) && ((enum cr_Seq) cr_g_SeqCurrent.next != SEQ_MODE_SELECTOR)) {
                cr_g_SeqCurrent.next = SEQ_MODE_SELECTOR;
                fn_80098290(2);
            }
            var_r31 += 1;
        }
        break;
    }
    HSD_JObjAnimAll(lbl_8044F398);
    fn_80075C14();
}
