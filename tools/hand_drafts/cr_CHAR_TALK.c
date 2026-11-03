/* cr_CHAR_TALK: step 110 hand draft (the m2c draft with the head rule, then: the slot index tested without a (u8) cast) */
extern u32 lbl_8044F320;

void cr_CHAR_TALK(void) {
    s16 *var_r31;
    u32 var_r30;
    s32 var_r29;
    s32 var_r28;
    u32 *cmd_cnt;

    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r31, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    cr_g_ScenarioSlotPtr->status |= lbl_80451108;
    cr_g_ScenarioSlotPtr->status &= 0xFFFFFEFF;
    if (cr_g_Scenario.slot_index != 0) {
        if ((u32) var_r31 < (u32) lbl_8044F320) {
            var_r30 = 0;
        } else {
            var_r30 = 1;
        }
    } else {
        var_r30 = 0;
    }
    var_r28 = M2C_FIELD(var_r31, s16 *, 4) & 0x7FFF;
    if (M2C_FIELD(var_r31, s16 *, 4) & 0x8000) {
        var_r30 ^= 1;
    }
    var_r29 = fn_80069D20(var_r30, var_r28);
    if (var_r29 != 0) {
        lbl_8032C2B0[0x4B0] = 0;
        cr_g_Scenario.unk_028 = 0;
        if (!(cr_g_ScenarioSlotPtr->status & 0x40)) {
            fn_800674E4(M2C_FIELD(var_r31, s16 *, 2));
            fn_8006611C(0x18U, 0x120U, var_r29);
        } else {
            fn_800674E4(M2C_FIELD(var_r31, s16 *, 2));
            fn_80065E10(0, var_r29);
        }
    } else {
        cr_g_Script.unk_464 = (u16) M2C_FIELD(var_r31, s16 *, 4);
        cr_g_Scenario.error_flags |= lbl_80451164;
    }
    cr_g_ScenarioSlotPtr->status |= lbl_80451114;
}
