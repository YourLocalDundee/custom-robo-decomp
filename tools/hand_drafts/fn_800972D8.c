/* fn_800972D8: Nov 13, 2026 hand draft: BGM play at the default volume: fn_80097154 with the volume read first, same inline slot allocator */

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

void fn_800972D8(s32 arg0) {
    cr_BgmSlot *s;
    u8 arg1 = *lbl_8044EAF8;

    if (arg0 < 0x48) {
        s = cr_Bgm_GetFreeSlot();
        if (s == ((void *) 0)) {
            cr_g_BgmRequest.song = (u32) arg0;
            cr_g_BgmRequest.start_volume = (u32) arg1;
            cr_g_BgmRequest.volume = (u32) arg1;
            cr_g_BgmRequest.state = 1;
            return;
        }
        s->song = (u32) arg0;
        s->start_volume = arg1;
        M2C_FIELD(s, f32 *, 0x10) = (f32) cr_g_BgmVolume;
        s->flags |= 0x40;
        s->flags |= 0x80000000;
    }
}
