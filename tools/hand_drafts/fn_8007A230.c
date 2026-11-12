/* fn_8007A230: Nov 5, 2026 hand draft (m2c 82%): the divisor 10 is a local (li r30, 10; divw), not a literal (which compiles to the multiply-high form) */
void fn_8007A230(void *arg0) {
    u8 temp_r4_21;
    s32 ten;

    if (cr_g_GameMode.flags & 0x04000000) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFDF;
    }
    if ((u32) cr_g_SeqCurrent.unk_10 != 9) {
        ten = 10;
        temp_r4_21 = (s32) M2C_FIELD(arg0, u8 *, 0x57) / ten;
        if ((M2C_FIELD(arg0, u8 *, 0x57) - temp_r4_21) > 0) {
            M2C_FIELD(arg0, u8 *, 0x57) -= temp_r4_21;
        } else {
            M2C_FIELD(arg0, u8 *, 0x57) = 0U;
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFDF;
        }
        M2C_FIELD(arg0, s32 *, 0) |= 1;
    }
}
