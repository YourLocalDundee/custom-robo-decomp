/* fn_800ABE94: Nov 13, 2026 hand draft: the five labels a local char table[5][6] with an initialiser (30 bytes of lbl_801C3820: Shift-JIS text; m2c shows the byte copy of the tail through words); the position and the size are Vecs (z unused), which gives the stack layout */

extern u8 *lbl_8044F728;

void fn_800ABE94(void *arg0) {
    char t0[5][6] = { { 0x8C, 0x83, 0x91, 0x81, 0, 0 }, { 0x91, 0x81, 0x82, 0xA2, 0, 0 }, { 0x95, 0x81, 0x92, 0xCA, 0, 0 }, { 0x92, 0x78, 0x82, 0xA2, 0, 0 }, { 0x8C, 0x83, 0x92, 0x78, 0, 0 } };
    Vec pos;
    Vec size;
    u8 i;

    i = 0;
    size.x = *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
    size.y = M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 4);
    while (i < 5U) {
        pos.x = (lbl_80451410 * (f32) i) + lbl_80451478;
        pos.y = lbl_8045140C;
        fn_80068358((s32) arg0, (s32) t0[i], (s32) &pos, (s32) &size, 0U);
        if (M2C_FIELD(lbl_8044F728, u8 *, 3) == 8) {
            M2C_FIELD((lbl_8044F728 + ((i * 0x10) & 0xFF0)), s8 *, 0xAC) = cr_Text_CountChars((s32) t0[i]);
        }
        i += 1;
    }
    M2C_FIELD(arg0, s8 *, 0x58) = 1;
}
