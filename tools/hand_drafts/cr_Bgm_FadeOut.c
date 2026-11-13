/* cr_Bgm_FadeOut: Nov 13, 2026 hand draft: slot loop; the fade time goes through an s16 local (the saved r29 the code never uses) */

void cr_Bgm_FadeOut(u8 arg0, u16 arg1) {
    cr_BgmSlot *s = cr_g_BgmSlots;
    u16 i;
    s16 d;

    for (i = 0; i < 2U; i++, s++) {
        if (s->flags & 0x80000000) {
            d = (arg1 * 0xC8) / 60;
            s->volume = arg0;
            s->fade_time = d;
            s->flags |= 0x10;
            s->flags |= 0x410;
        }
    }
}
