/* cr_SYS_PICKUP_BATTLE_END: step 110 hand draft (the byte offset into cr_g_Scenario added to a u8 pointer: m2c added it to the struct pointer, which C scales by the struct size) */
void cr_SYS_PICKUP_BATTLE_END(void) {
    s32 var_r31;
    s32 var_r30;
    s16 *var_r29;
    u32 *cmd_cnt;

    var_r30 = 0;
    var_r29 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) *var_r29;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    while (var_r30 < 4) {
        var_r31 = 0;
        while (var_r31 < 0x40) {
            cr_Bits_Clear((unsigned long *) ((u8 *) &cr_g_Scenario + (var_r30 << 5) + 0x298), (u32) var_r31);
            var_r31 += 1;
        }
        var_r30 += 1;
    }
    cr_g_Scenario.pickup_battle &= 0xFE;
    if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
        if ((s16 *) cr_g_ScriptPC != NULL) {
            if ((s32) cr_g_ScriptPC & 0x80000000) {
                if ((s16) *cr_g_ScriptPC < 0x12C) {
                    if (cr_g_ScriptCharaAct != 0) {
                        if ((s16) *cr_g_ScriptPC >= 0x121) {
                            cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                            return;
                        }
                        cr_g_Scenario.error_flags |= lbl_80451184;
                        return;
                    }
                    if ((s16) *cr_g_ScriptPC < 0x121) {
                        cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                        return;
                    }
                    cr_g_Scenario.error_flags |= lbl_80451180;
                    return;
                }
                cr_g_Scenario.error_flags |= lbl_80451170;
                return;
            }
            cr_g_Scenario.error_flags |= lbl_80451168;
            return;
        }
        cr_g_Scenario.error_flags |= lbl_8045116C;
    }
}
