/* fn_8012855C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): fn_800AA17C called with its argument (0, then 0 and 1: draft_batch.py PARAM_SET), the u32 globals converted as s32 (xoris), the constants as literals except the last 0.5, which main.dol loads after the conversion constant like an extern */
void fn_8012855C(void *arg0) {
    f32 var_f31;
    s32 var_r30;

    M2C_FIELD(arg0, f32 *, 0x48) = (f32) ((100.0f * (f32) *(s32 *) lbl_8044F508) / 1000.0f);
    M2C_FIELD(arg0, u32 *, 0x50) = (u32) (300.0f * M2C_FIELD(arg0, f32 *, 0x48));
    if ((s32) cr_g_GameMode.battle_mode == 2) {
        if (((f32) M2C_FIELD(lbl_8032A8B4, s32 *, 0) >= 1000.0f) && ((f32) (s32) lbl_8032A8B4[1] >= 1000.0f)) {
            M2C_FIELD(arg0, u32 *, 0x50) += 0x2710;
        }
    } else if ((f32) M2C_FIELD(lbl_8032A8B4, s32 *, 0) >= 1000.0f) {
        M2C_FIELD(arg0, u32 *, 0x50) += 0x1388;
    }
    var_f31 = (f32) cr_g_Scenario.unk_280 / 60.0f;
    M2C_FIELD(arg0, u32 *, 0x64) = (u32) (10.0f * var_f31);
    M2C_FIELD(arg0, s32 *, 0x54) = (s32) (M2C_FIELD(arg0, u32 *, 0x64) * 0xA);
    M2C_FIELD(arg0, s32 *, 0x58) = (s32) (cr_g_Scenario.unk_284 * 0x2710);
    M2C_FIELD(arg0, u32 *, 0x68) = (u32) cr_g_Scenario.slot[1].unk_84[3];
    if (M2C_FIELD(arg0, u32 *, 0x68) > 0x3E7U) {
        M2C_FIELD(arg0, u32 *, 0x68) = 0x3E7U;
    }
    if (M2C_FIELD(arg0, u32 *, 0x68) < 5U) {
        M2C_FIELD(arg0, f32 *, 0x4C) = (f32) -((0.1f * (f32) M2C_FIELD(arg0, u32 *, 0x68)) - 1.0f);
    } else {
        M2C_FIELD(arg0, f32 *, 0x4C) = (f32) 0.5f;
    }
    if ((s32) cr_g_GameMode.battle_mode != 2) {
        M2C_FIELD(arg0, u8 *, 0x6C) = fn_800AA17C(0);
    } else {
        M2C_FIELD(arg0, u8 *, 0x6C) = fn_800AA17C(0);
        M2C_FIELD(arg0, u8 *, 0x6C) |= fn_800AA17C(1);
    }
    M2C_FIELD(arg0, u32 *, 0x5C) = M2C_FIELD(arg0, u32 *, 0x50);
    M2C_FIELD(arg0, u32 *, 0x5C) += M2C_FIELD(arg0, s32 *, 0x54);
    M2C_FIELD(arg0, u32 *, 0x5C) += M2C_FIELD(arg0, s32 *, 0x58);
    M2C_FIELD(arg0, u32 *, 0x5C) = (u32) ((f32) M2C_FIELD(arg0, u32 *, 0x5C) * M2C_FIELD(arg0, f32 *, 0x4C));
    M2C_FIELD(arg0, u32 *, 0x5C) = (u32) ((f32) M2C_FIELD(arg0, u32 *, 0x5C) * lbl_8044F750);
    if (M2C_FIELD(arg0, u8 *, 0x6C) & 1) {
        M2C_FIELD(arg0, u32 *, 0x5C) = (u32) ((f32) M2C_FIELD(arg0, u32 *, 0x5C) * lbl_80451A54);
    }
    var_r30 = 0;
    cr_g_Scenario.unk_258[cr_g_Script.battle_arg] = M2C_FIELD(arg0, u32 *, 0x5C);
    M2C_FIELD(arg0, s32 *, 0x60) = 0;
    while (var_r30 < (s32) cr_g_Script.battle_arg) {
        M2C_FIELD(arg0, s32 *, 0x60) += cr_g_Scenario.unk_258[var_r30];
        var_r30 += 1;
    }
    M2C_FIELD(arg0, s32 *, 0x60) += M2C_FIELD(arg0, u32 *, 0x5C);
}
