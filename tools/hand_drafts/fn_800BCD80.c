/* fn_800BCD80: Nov 6, 2026 hand draft (m2c 87%): the area box's first bound is an f32 (m2c read it as s32 and converted it), and slot_index compared without the (u8) cast */

extern u8 *lbl_8044F710;

void fn_800BCD80(void) {
    s32 var_r31;
    s32 var_r30;

    var_r30 = 0;
    if ((cr_g_Scenario.slot_index == 0) || (((void (*)()) cr_g_FirstChara->update) == ((void *) 0))) {
        M2C_FIELD(lbl_8044F710, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    var_r31 = 0;
    while (var_r31 < 0xA) {
        if ((M2C_FIELD((((u32 *) ((u8 *) lbl_803B92CC + (var_r31 * 8)))), s32 *, 4) != 0) && (**(f32 **) (((u32 *) ((u8 *) lbl_803B92CC + (var_r31 * 8)))) < cr_g_FirstChara->pos.x) && (cr_g_FirstChara->pos.x < M2C_FIELD(*(((u32 *) ((u8 *) lbl_803B92CC + (var_r31 * 8)))), f32 *, 0xC)) && (M2C_FIELD(*(((u32 *) ((u8 *) lbl_803B92CC + (var_r31 * 8)))), f32 *, 8) < cr_g_FirstChara->pos.z) && (cr_g_FirstChara->pos.z < M2C_FIELD(*(((u32 *) ((u8 *) lbl_803B92CC + (var_r31 * 8)))), f32 *, 0x14))) {
            if ((M2C_FIELD(lbl_8044F710, s32 *, 0x10) == 0) || (M2C_FIELD(lbl_8044F710, s32 *, 0x14) != var_r31)) {
                M2C_FIELD(lbl_8044F710, s32 *, 0x10) = 1;
                fn_800BD074(*(((u32 *) ((u8 *) lbl_803B92CC + (var_r31 * 8)))));
            }
            var_r30 = 1;
            M2C_FIELD(lbl_8044F710, s32 *, 0x14) = var_r31;
        }
        var_r31 += 1;
    }
    if (var_r30 == 0) {
        M2C_FIELD(lbl_8044F710, s32 *, 0x10) = 0;
    }
    if (cr_Bits_Test(cr_g_ScenarioSlotPtr->flags, 0x247U) == 0) {
        var_r31 = 1;
        for (; var_r31 < 0x14; var_r31 += 1) {
            if (cr_Bits_Test(&cr_g_Scenario.unk_1c8[2], (u32) var_r31) == 0) {
                break;
            }
        }
        if (var_r31 == 0x14) {
            cr_Bits_Set(cr_g_ScenarioSlotPtr->flags, 0x247U);
            cr_Bits_Set(cr_g_ScenarioSlotPtr->flags, 0x246U);
        }
    }
    if (cr_Bits_Test(cr_g_ScenarioSlotPtr->flags, 0x249U) == 0) {
        var_r31 = 1;
        for (; var_r31 < 0x14; var_r31 += 1) {
            if (cr_Bits_Test(&cr_g_Scenario.unk_1c8[1], (u32) var_r31) == 0) {
                break;
            }
        }
        if (var_r31 == 0x14) {
            cr_Bits_Set(cr_g_ScenarioSlotPtr->flags, 0x249U);
            cr_Bits_Set(cr_g_ScenarioSlotPtr->flags, 0x248U);
        }
    }
    if (cr_Bits_Test(cr_g_ScenarioSlotPtr->flags, 0x24BU) == 0) {
        var_r31 = 1;
        for (; var_r31 < 0x14; var_r31 += 1) {
            if (cr_Bits_Test(cr_g_Scenario.unk_1c8, (u32) var_r31) == 0) {
                break;
            }
        }
        if (var_r31 == 0x14) {
            cr_Bits_Set(cr_g_ScenarioSlotPtr->flags, 0x24BU);
            cr_Bits_Set(cr_g_ScenarioSlotPtr->flags, 0x24AU);
        }
    }
}
