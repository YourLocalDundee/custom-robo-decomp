/* cr_SYS_CALL: step 110 hand draft (the call depth incremented in place, `cr_g_ScriptReturnStack[cr_g_ScriptDepth++] = cr_g_ScriptPC` after the call count: m2c's copy of the depth was a saved local the original does not have) */
void cr_SYS_CALL(void) {
    s16 *temp_r31_14;
    u32 *cmd_cnt;

    temp_r31_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    if ((u32) cr_g_ScriptDepth < 0xFU) {
        cr_g_Script.call_count += 1;
        cr_g_ScriptReturnStack[cr_g_ScriptDepth++] = cr_g_ScriptPC;
        cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
        cr_g_ScriptPC = M2C_FIELD(M2C_FIELD(temp_r31_14, void **, 4), s16 **, 0);
        cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(M2C_FIELD(temp_r31_14, void **, 4), void **, 4);
        if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
            if ((s16 *) cr_g_ScriptPC != NULL) {
                if ((s32) cr_g_ScriptPC & 0x80000000) {
                    if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x12C) {
                        if (cr_g_ScriptCharaAct != 0) {
                            if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) >= 0x121) {
                                cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
                                return;
                            }
                            cr_g_Scenario.error_flags |= lbl_80451184;
                            return;
                        }
                        if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x121) {
                            cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
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
    } else {
        cr_g_Scenario.error_flags |= lbl_80451160;
    }
}
