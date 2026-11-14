/* cr_SEQ_STAGE_SELECTOR_Leave: Nov 14, 2026 hand draft: the clear colour saved and replaced: a GXColor local with a zero initialiser (.sbss2), and in the block two GXColor locals copied byte by byte (old from HSD_VIData's clear colour, new from the local), the new one declared first (42%) */

void cr_SEQ_STAGE_SELECTOR_Leave(void) {
    GXColor col = { 0, 0, 0, 0 };

    fn_8011214C();
    if ((cr_g_StageSelector.flags & 0x80) && (cr_g_SeqCurrent.next == SEQ_MODE_SELECTOR)) {
        cr_g_StageSelector.flags &= ~0x80;
        fn_80097530();
    }
    if ((cr_g_SeqCurrent.next == SEQ_CUSTOMIZE) || (cr_g_SeqCurrent.next == SEQ_MODE_SELECTOR) ||
        (cr_g_SeqCurrent.next == SEQ_MENU)) {
        GXColor nw;
        GXColor old;

        old = HSD_VIData.current.vi.clear_clr;
        nw = col;

        lbl_8044F848 = old.r;
        lbl_8044F849 = old.g;
        lbl_8044F84A = old.b;
        lbl_8044F84B[0] = old.a;
        HSD_VIData.current.vi.clear_clr = nw;
    }
    fn_80111E0C(0x5F);
}
