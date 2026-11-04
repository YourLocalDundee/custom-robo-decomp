/* fn_80024848: Nov 3, 2026 hand draft (m2c 82%): the else store written out, `p->f = p->f + 0xFFF7` (the original computes the element address twice), and the u32 counter compared as (u8) */

void fn_80024848(void) {
    f32 var_f31_37;
    u32 var_r31_6;

    var_r31_6 = 0;
    while ((u8) var_r31_6 < cr_g_CamSystem.unk_08) {
        if (M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), u32 *, 0x114) > 9U) {
            M2C_FIELD((M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC)), s32 *, 0x114) -= (cr_g_StageSelector.stage_open[(s8) cr_g_StageSelector.cursor] * 9);
            if (lbl_8044F45A != 0) {
                var_f31_37 = lbl_80450518;
            } else {
                var_f31_37 = lbl_8045051C;
            }
            M2C_FIELD((M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC)), f32 *, 0xA0) += var_f31_37;
            if (M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), f32 *, 0xA0) < lbl_80450520) {
                lbl_8044F45A = 0;
            } else if (M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), f32 *, 0xA0) > lbl_80450524) {
                lbl_8044F45A = 1;
            }
        } else {
            M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), s32 *, 0x114) = M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), s32 *, 0x114) + 0xFFF7;
        }
        var_r31_6 += 1;
    }
}
