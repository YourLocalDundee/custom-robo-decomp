/* fn_80096D28: Nov 5, 2026 hand draft (m2c 81%): the twin of fn_80096C28 for channel 0 (the same folded `ch` local and the kept `r1 = 1` test), then sets bit 0 */
void fn_80096D28(u16 arg0) {
    u16 t;
    s32 r2;
    s32 r1;
    s32 ch;

    t = arg0;
    lbl_8044F6C0 |= 6;
    t = (t * 200) / 60;
    if ((lbl_8044F6C0 & 6) && !(lbl_8044F6C0 & 1)) {
        if (lbl_8044F6C0 & 2) {
            ch = 0;
            fn_8017755C(ch, t, 0);
            r1 = 1;
            if (r1 == 1) {
                lbl_8044F6C0 &= ~2;
            }
        }
        if (lbl_8044F6C0 & 4) {
            r2 = ((s32 (*)(u8, u16)) fn_80170D4C)(0, t);
            if (r2 == 1) {
                lbl_8044F6C0 &= ~4;
            }
        }
    }
    lbl_8044F6C0 |= 1;
}
