/* fn_80019828: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the countdown compared as s32 (subic. / blt) */
void fn_80019828(void) {
    u8 var_r31_8;

    var_r31_8 = 0;
    if (!(cr_g_SeqCurrent.unk_40 & 0x8000)) {
        if ((cr_g_Pads->unk_14 & 2) || (cr_g_Pads->unk_14 & 0x02000000)) {
            lbl_8044F350 += 1;
        }
        if ((cr_g_Pads->unk_14 & 8) || (cr_g_Pads->unk_14 & 0x08000000)) {
            lbl_8044F350 -= 1;
        }
        if ((s8) lbl_8044F350 < 0) {
            lbl_8044F350 = 0;
        }
        if ((s32) lbl_8044F350 == 2) {
            lbl_8044F350 = 1;
        }
        if (cr_g_Pads->unk_14 & 0x100) {
            var_r31_8 = 1;
        }
    }
    if (((s32) (lbl_8044C3E0 -= 1) < 0) || (var_r31_8 == 1)) {
        cr_g_SeqCurrent.unk_38 &= 0xFFFFFFBF;
        if ((s8) lbl_8044F350 == 0) {
            OSSetProgressiveMode(1U);
            lbl_8044F354 = 1;
            cr_g_SeqCurrent.unk_40 = 1;
            cr_g_SeqCurrent.unk_10 = 7;
            lbl_8044F3CC = 1;
            return;
        }
        OSSetProgressiveMode(0U);
        lbl_8044F354 = 0;
        lbl_8044C3E0 = 0x96;
        cr_g_SeqCurrent.unk_10 = 6;
    }
}
