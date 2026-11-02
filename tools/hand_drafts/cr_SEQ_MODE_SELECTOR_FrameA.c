/* cr_SEQ_MODE_SELECTOR_FrameA: hand-written from main.dol's asm on its m2c draft (plan.md step 108): `if (state == 3) { (void) 0; }` before the switch: an empty debug macro (the compare stays, the branch goes) */
void cr_SEQ_MODE_SELECTOR_FrameA(void) {
    if ((u32) cr_g_SeqCurrent.unk_24 == 0) {
        fn_800EF2B0();
        cr_Cam_Init();
        fn_80092108(8U);
        fn_80124F5C();
        fn_80114588();
        if (lbl_8044F885[2] == 0) {
            fn_80124808(0);
        }
        lbl_8044F885[2] = 0;
        lbl_8044F885[3] = 0;
        cr_g_SeqCurrent.unk_40 = 2;
        cr_g_SeqCurrent.unk_24 = 1;
    }
    if ((s32) cr_g_SeqCurrent.unk_24 == 3) {
        (void) 0;
    }
    switch ((s32) cr_g_SeqCurrent.unk_24) {         /* irregular */
    case 1:
        if (!(cr_g_SeqCurrent.unk_40 & 0x8003) && !(fn_80089F40() & 1)) {
            fn_8011453C();
        }
        /* fallthrough */
    case 2:
        return;
    }
}
