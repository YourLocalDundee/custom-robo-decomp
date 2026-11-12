/* fn_800EAC48: Nov 6, 2026 hand draft (m2c 93%): at the end the two unk_38 updates come before unk_40 = 1 (m2c's order interleaved them; only the volatile registers differed) */

void fn_800EAC48(void) {
    if (!(cr_g_SeqCurrent.unk_38 & 0x8A)) {
        if (cr_g_SeqCurrent.unk_38 & 4) {
            if (OSGetResetButtonState() == 0) {
                PADRecalibrate(0x80000000U);
                PADRecalibrate(0x40000000U);
                PADRecalibrate(0x20000000U);
                PADRecalibrate(0x10000000U);
                cr_g_SeqCurrent.unk_38 |= 2;
            }
        } else if (OSGetResetButtonState() != 0) {
            cr_g_SeqCurrent.unk_38 |= 4;
        }
        if ((u32) (cr_g_Pads->dir & 0x1210) == 0x1210) {
            *lbl_8044F7C8 += 1;
            if ((s32) *lbl_8044F7C8 > 0x1E) {
                PADRecalibrate(0x80000000U);
                PADRecalibrate(0x40000000U);
                PADRecalibrate(0x20000000U);
                PADRecalibrate(0x10000000U);
                cr_g_SeqCurrent.unk_38 |= 2;
            }
        } else {
            *lbl_8044F7C8 = 0;
        }
    } else if (cr_g_SeqCurrent.unk_38 & 8) {
        if ((u32) cr_g_SeqCurrent.unk_34 != 0) {
            cr_g_SeqCurrent.unk_34 -= 1;
        } else {
            HSD_VISetBlack(1);
            fn_8019D38C();
            fn_8019DB14(HSD_RP_SCREEN);
            fn_8019DC90();
            VIWaitForRetrace();
            if (cr_g_SeqCurrent.unk_38 & 0x40) {
                cr_g_SeqCurrent.unk_38 &= ~0x8A;
                cr_g_SeqCurrent.unk_38 &= 0xFFFFFFFB;
                HSD_VISetBlack(0);
                cr_g_SeqCurrent.unk_40 |= 2;
                fn_800191E8();
                if (DVDCheckDisk() == 0) {
                    OSResetSystem(1, 0U, 1);
                } else {
                    fn_80096C28(0x1EU);
                }
            } else if (cr_g_SeqCurrent.unk_38 & 0x20) {
                OSResetSystem(1, 0U, 1);
            } else {
                OSResetSystem(0, 0U, 0);
            }
        }
    }
    if (!(cr_g_SeqCurrent.unk_38 & 0x90) && (cr_g_SeqCurrent.unk_38 & 2)) {
        cr_g_SeqCurrent.unk_34 = 8;
        cr_g_SeqCurrent.unk_38 &= 0xFFFFFFFD;
        cr_g_SeqCurrent.unk_38 |= 8;
        cr_g_SeqCurrent.unk_40 = 1;
        cr_g_SeqCurrent.unk_40 |= 0x2000;
        lbl_8044F488 = 0xFFU / (u32) cr_g_SeqCurrent.unk_34;
        fn_80096D28((u16) cr_g_SeqCurrent.unk_34);
    }
}
