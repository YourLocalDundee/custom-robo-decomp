/* fn_800528B8: Nov 3, 2026 hand draft + decomp-permuter (m2c 96%): plain u16 stack local for +0x32 (never read, kept), a dead s32 after flags as `if (0) { unused = unused; }`, and the permuter's reload of flags (+0x10) after the call, which gives the original's r3 for the frame-count load */

extern u8 *lbl_8044F574;

void fn_800528B8(void) {
    u8 set;
    s16 kind;
    u32 flags;
    s32 unused;
    s32 diff;
    u16 sp8;
    set = 0;
    flags = M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x10);
    if (!(flags & 8)) {
        sp8 = M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x32);
        set |= 1;
        lbl_8044F574 = cr_g_CurrentChara_0e4c;
        fn_80051384(cr_g_CurrentChara_11cc);
        flags = M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x10);
        kind = 7;
        diff = (f32) M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x18) - cr_g_MotionFrame;
        if (diff <= 0x10) {
            kind = 3;
        }
        M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 6) = 0;
        M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 2) = kind;
        if ((enum cr_Seq) cr_g_SeqCurrent.current == SEQ_SCENARIO) {
            M2C_FIELD(cr_g_CurrentChara_09e8, s16 *, 2) = 0xA;
        }
    }
    if (0) { unused = unused; }
    M2C_FIELD(cr_g_CurrentChara_09e8, s8 *, 9) = set;
    M2C_FIELD(cr_g_CurrentChara_09e8, s8 *, 8) = 0;
}
