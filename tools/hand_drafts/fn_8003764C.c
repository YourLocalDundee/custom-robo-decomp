/* fn_8003764C: Nov 6, 2026 hand draft (m2c 78%): m2c's draft with the statements of the switch cases reordered (adjacent-swap and block-permutation sweeps, 4 swaps and 4 runs): the stores to lbl_8044F48C / lbl_8044F490 / cr_g_SeqCurrent.unk_40 in the original's order */

void fn_8003764C(void) {

    if ((s32) *lbl_8044F490 == 0) {
        switch ((cr_g_SeqCurrent.unk_40 & 0x3C0)) {                        /* irregular */
        case 0x40:
            lbl_8044F48C += lbl_8044F488;
            if (lbl_8044F48C >= 0xFF) {
                lbl_8044F48C = 0xFF;
                cr_g_SeqCurrent.unk_40 |= 0x1000;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        case 0x80:
            lbl_8044F48C -= lbl_8044F488;
            if (lbl_8044F48C <= 0) {
                lbl_8044F48C = 0;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        case 0x100:
            lbl_8044F48C += lbl_8044F488;
            if (lbl_8044F48C >= 0xFF) {
                lbl_8044F48C = 0xFF;
                cr_g_SeqCurrent.unk_40 |= 0x1000;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        case 0x200:
            lbl_8044F48C -= lbl_8044F488;
            if (lbl_8044F48C <= 0) {
                lbl_8044F48C = 0;
                *lbl_8044F490 = 0;
                cr_g_SeqCurrent.unk_40 |= 0x4000;
                cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
            }
            lbl_8044C458[3] = (u8) lbl_8044F48C;
            return;
        }
    } else {
        *lbl_8044F490 -= 1;
        if ((s32) *lbl_8044F490 == 0) {
            cr_g_SeqCurrent.unk_40 |= 0x4000;
            cr_g_SeqCurrent.unk_40 &= 0xFFFF7FFF;
        }
    }
}
