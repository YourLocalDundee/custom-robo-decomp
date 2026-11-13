/* fn_80097488: Nov 13, 2026 hand draft: the same slot loop as cr_Bgm_FadeOut (s16 fade-time local), matched by id, returns */

void fn_80097488(s32 arg0, u8 arg1, u16 arg2) {
    u32 *s = lbl_803B76E8;
    u16 i;
    s16 d;

    for (i = 0; i < 0x60U; i++, s += 0xE) {
        if (((s32) s[2] == arg0) && (s[0] & 0x80000000)) {
            d = (arg2 * 0xC8) / 60;
            M2C_FIELD(s, u8 *, 0xD) = arg1;
            M2C_FIELD(s, s16 *, 0x30) = d;
            s[0] |= 0x10;
            s[0] |= 0x410;
            return;
        }
    }
}
