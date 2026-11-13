/* fn_80069D20: Nov 13, 2026 hand draft: window text lookup: (u8 *) table[a] + off + 8, left to right (grouping (off + 8) is reassociated) */

s32 fn_80069D20(u8 a, s32 id) {
    u32 off;
    u8 *p;

    cr_g_WindowStyle.unk_2c = (u32) fn_80068978;
    off = ((struct { u32 off; } *) (((u32 *) lbl_80396484)[a] + 0x10))[id].off;
    if (off != 0) {
        p = (u8 *) ((u32 *) lbl_80396478)[a] + off + 8;
        return (s32) p;
    }
    return 0;
}
