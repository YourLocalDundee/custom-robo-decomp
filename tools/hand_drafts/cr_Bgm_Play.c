/* cr_Bgm_Play: Nov 13, 2026 hand draft: the inline free-slot search and the s16 fade time as before; one dead use of the fade time puts it above the inline result (r29 saved and unused, the result in r28): by the usage rule of allocate_saved_gprs */

static inline cr_BgmSlot *cr_Bgm_GetFreeSlot(void) {
    u8 i;

    for (i = 0; i < 2U; i++) {
        if (!(cr_g_BgmSlots[i].flags & 0x80000000)) {
            cr_g_BgmSlots[i] = *(cr_BgmSlot *) lbl_8021AC30;
            return &cr_g_BgmSlots[i];
        }
    }
    return NULL;
}


void cr_Bgm_Play(u32 arg0, u8 arg1, u8 arg2, u16 arg3) {
    cr_BgmSlot *s;
    s16 d;

    (void) d;
    s = cr_Bgm_GetFreeSlot();
    if (s != ((void *) 0)) {
        d = (arg3 * 0xC8) / 60;
        s->fade_time = d;
        s->song = arg0;
        s->start_volume = arg1;
        s->volume = arg2;
        s->flags |= 0x40;
        s->flags |= 0x10;
        s->flags |= 0x80000000;
        return;
    }
    cr_g_BgmRequest.song = arg0;
    cr_g_BgmRequest.start_volume = (u32) arg1;
    cr_g_BgmRequest.volume = (u32) arg2;
    cr_g_BgmRequest.fade_frames = (u32) arg3;
    cr_g_BgmRequest.state = 3;
}
