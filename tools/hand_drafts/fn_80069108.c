/* fn_80069108: Nov 13, 2026 hand draft: a local s32 table[8] with an initialiser (the 8 words copied from lbl_801C0A68); the glyph index a local (r31), the width stored unsigned */

void *fn_80069108(void *arg0, void *arg1, u8 *arg2) {
    s32 tbl[8] = { 3, 1, 4, 0, 2, 5, 6, 7 };
    s32 v;

    M2C_FIELD(arg2, s16 *, 0x10) = 0x39;
    v = lbl_801C50D0[tbl[M2C_FIELD(arg0, u8 *, 5)]];
    M2C_FIELD(arg2, u16 *, 8) = v * 0x18;
    M2C_FIELD(arg2, s16 *, 0xA) = 0;
    M2C_FIELD(arg2, u16 *, 0xC) = 0x18U;
    M2C_FIELD(arg2, s16 *, 0xE) = 0x18;
    M2C_FIELD(arg2, u16 *, 0x12) |= 0x8000;
    M2C_FIELD(arg2, f32 *, 0) = (f32) M2C_FIELD(arg1, f32 *, 4);
    M2C_FIELD(arg2, f32 *, 4) = (f32) M2C_FIELD(arg1, f32 *, 8);
    M2C_FIELD(arg1, f32 *, 4) += (f32) M2C_FIELD(arg2, u16 *, 0xC);
    M2C_FIELD(arg2, f32 *, 0x14) = (f32) lbl_80450ABC;
    M2C_FIELD(arg1, u8 *, 0xC) += 1;
    return arg2 + 0x20;
}
