/* fn_80037FF0: Nov 6, 2026 hand draft (m2c 76%): a field character motion command step (0x7FFF = loop back, 0x8000 = the motion from +0x18, 0 = wait, else that motion ID; length 1 = play once, else ID + 0x2000); an if-chain on a u32 copy of the u16 command (no switch, no clrlwi), the 0x8000 motion read into the same variable, `x != 0` first */

void fn_80037FF0(void) {
    u32 x;

    if (!(M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC) <= (f32) M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x18))) {
        x = M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x1A);
        if (x == 0x7FFF) {
            M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x50);
            M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x46) = M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x44);
            M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC) = M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x48);
            cr_g_MotionFrame = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC);
            for (lbl_8044F590 = M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C); M2C_FIELD(lbl_8044F590, s32 *, 0) != 0; lbl_8044F590 += 8) {
                lbl_8044F58C = *M2C_FIELD(lbl_8044F590, u16 **, 4);
                if (!((f32) lbl_8044F58C < cr_g_MotionFrame)) {
                    break;
                }
            }
            M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = lbl_8044F590;
        } else {
            if (x == 0x8000) {
                x = *M2C_FIELD(cr_g_CurrentChara_06dc, u16 **, 0x18);
                lbl_8044F4E4 = x;
            } else if (x != 0) {
                lbl_8044F4E4 = x;
            } else {
                M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC) -= lbl_80450654;
                return;
            }
            lbl_8044F4E8 = M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x48);
            lbl_8044F4EC = 0;
            if (lbl_8044F4E8 == 1) {
                cr_Motion_Play(lbl_8044F4E4, 1U);
            } else {
                lbl_8044F4E4 += 0x2000;
                cr_Motion_Play(lbl_8044F4E4, lbl_8044F4E8);
            }
        }
    }
    cr_Motion_UpdateRoot();
    lbl_8044F588 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC);
    lbl_8044F590 = M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C);
    fn_8005CCFC();
}
