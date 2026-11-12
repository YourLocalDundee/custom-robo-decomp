/* fn_80096C28: Nov 5, 2026 hand draft (m2c 81%): starts the two sound fades (0xFF = all channels; time scaled by 200 / 60). The channel number of the first call is a local (`ch = 0xFF`), which the peephole folds into the li r3 but which keeps its saved register r28 (main.dol saves r28 and never uses it); `r1 = 1; if (r1 == 1)` stays as written (li r29, 1; cmpwi), m2c folded it; the second call takes the time too (r4) */
void fn_80096C28(u16 arg0) {
    u16 t;
    s32 r2;
    s32 r1;
    s32 ch;

    t = arg0;
    lbl_8044F6C0 &= ~1;
    lbl_8044F6C0 |= 6;
    t = (t * 200) / 60;
    if ((lbl_8044F6C0 & 6) && !(lbl_8044F6C0 & 1)) {
        if (lbl_8044F6C0 & 2) {
            ch = 0xFF;
            fn_8017755C(ch, t, 0);
            r1 = 1;
            if (r1 == 1) {
                lbl_8044F6C0 &= ~2;
            }
        }
        if (lbl_8044F6C0 & 4) {
            r2 = ((s32 (*)(u8, u16)) fn_80170D4C)(0xFF, t);
            if (r2 == 1) {
                lbl_8044F6C0 &= ~4;
            }
        }
    }
}
