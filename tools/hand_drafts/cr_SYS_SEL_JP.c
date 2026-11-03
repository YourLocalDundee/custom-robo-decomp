/* cr_SYS_SEL_JP: step 110 hand draft (three locals used only in dead code: the original saves r28-r31 and uses only r30; the counter pointer with six dead uses ranks first for r31) */
void cr_SYS_SEL_JP(void) {
    u32 *d2;
    u32 *d3;
    s16 *temp_r30_16;
    u32 *cmd_cnt;

    temp_r30_16 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_16, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 8;
    if (0) {
        (*cmd_cnt)++;
        (*cmd_cnt)++;
        (*cmd_cnt)++;
        (*cmd_cnt)++;
        (*cmd_cnt)++;
        (*cmd_cnt)++;
        (*d2)++;
        (*d3)++;
    }
    cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
    switch ((s32) cr_g_Scenario.unk_028) {          /* irregular */
    case 0:
        cr_g_ScriptPC = M2C_FIELD(M2C_FIELD(temp_r30_16, void **, 4), s16 **, 0);
        cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(M2C_FIELD(temp_r30_16, void **, 4), void **, 4);
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
        return;
    case 1:
        cr_g_ScriptPC = M2C_FIELD(M2C_FIELD(temp_r30_16, void **, 8), s16 **, 0);
        cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(M2C_FIELD(temp_r30_16, void **, 8), void **, 4);
        return;
    case 2:
        cr_g_ScriptPC = M2C_FIELD(M2C_FIELD(temp_r30_16, void **, 0xC), s16 **, 0);
        cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(M2C_FIELD(temp_r30_16, void **, 0xC), void **, 4);
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
        break;
    }
}
