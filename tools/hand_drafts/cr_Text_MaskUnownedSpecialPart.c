/* cr_Text_MaskUnownedSpecialPart: Nov 6, 2026 hand draft (m2c 94%): the special part's text slot is (part - first special part) + the count of all parts, `(arg1 - 0x1E) + temp_r28_35` (the compiler adds first and subtracts last; m2c's `(arg1 + temp) - 0x1E` folds the constant into the count instead); the count is the plain sum body + gun + bomb + pod + leg (m2c's grouping loads the five counts in another order) */

void cr_Text_MaskUnownedSpecialPart(u16 arg0, u32 arg1, s16 *arg2) {
    u32 *var_r29_18;
    s32 temp_r28_35;

    if (cr_g_GameMode.flags & 0x800000) {
        var_r29_18 = cr_g_Scenario.part_bits[0];
    } else {
        var_r29_18 = cr_g_SaveData.unlocks.parts[0];
    }
    temp_r28_35 = cr_g_BodyCount + cr_g_GunCount + cr_g_BombCount + cr_g_PodCount + cr_g_LegCount;
    switch ((s32) arg0) {                            /* irregular */
    case 0:
        if (((arg1 == 0x1E) || (arg1 == 0x1F) || (arg1 == 0x20)) && (cr_Bits_Test(var_r29_18, arg1) == 0)) {
            *arg2 = (arg1 - 0x1E) + temp_r28_35;
        }
        return;
    case 1:
        if (((arg1 == 0x31) || (arg1 == 0x32) || (arg1 == 0x33)) && (cr_Bits_Test(var_r29_18 + 8, arg1) == 0)) {
            *arg2 = (arg1 - 0x2E) + temp_r28_35;
            return;
        }
        break;
    case 2:
        if ((arg1 == 0x28) && (cr_Bits_Test(var_r29_18 + 0x10, arg1) == 0)) {
            *arg2 = temp_r28_35 + 6;
            return;
        }
        break;
    case 3:
        if (((arg1 == 0x29) || (arg1 == 0x2A) || (arg1 == 0x2B)) && (cr_Bits_Test(var_r29_18 + 0x18, arg1) == 0)) {
            *arg2 = (arg1 - 0x22) + temp_r28_35;
            return;
        }
        break;
    case 4:
        if ((arg1 == 0xF) && (cr_Bits_Test(var_r29_18 + 0x20, arg1) == 0)) {
            *arg2 = temp_r28_35 + 0xA;
        }
        break;
    }
}
