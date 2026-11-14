/* fn_80096E28: Nov 13, 2026 hand draft: the fade time is stored and assigned in one chained assignment through a u16 field (slot->fade_time = time = time * 200 / 60: the store takes the untruncated r0, the parameter the clrlwi) */

void fn_80096E28(s32 arg0, u8 arg1, u16 arg2) {
    cr_BgmSlot *s = cr_g_BgmSlots;
    u16 i;

    for (i = 0; i < 2U; i++, s++) {
        if (((s32) s->song == arg0) && (s->flags & 0x80000000)) {
            s->volume = arg1;
            *(u16 *) &s->fade_time = arg2 = (arg2 * 0xC8) / 60;
            s->flags |= 0x10;
        }
    }
}
