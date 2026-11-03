/* cr_F_CHK_BIT: step 110 hand draft (the m2c draft with the head rule, then: the bit index an s32 local: no extsh before the shift) */
extern u8 *lbl_8044F788;

void cr_F_CHK_BIT(void) {
    s16 *var_r31;
    u32 *cmd_cnt;
    s32 var_r30;
    s32 var_r28;
    s32 var_r27;

    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r31, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 6;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r30 = M2C_FIELD(var_r31, s16 *, 2);
    var_r27 = var_r30 >> 5;
    var_r28 = 1 << (var_r30 & 0x1F);
    if (cr_g_ScenarioSlotPtr->flags[var_r27] & var_r28) {
        if ((void *) M2C_FIELD(var_r31, void **, 4) == ((void *) 0)) {
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
            if ((u32) (M2C_FIELD(var_r31, u8 **, 4)) == (u32) (0xFFFFFFFF)) {
                cr_g_ScriptPC = M2C_FIELD(lbl_8044F788, s16 **, 0);
                cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(lbl_8044F788, void **, 4);
                cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
                return;
            }
            cr_g_ScriptPC = M2C_FIELD(M2C_FIELD(var_r31, void **, 4), s16 **, 0);
            cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(M2C_FIELD(var_r31, void **, 4), void **, 4);
            cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
        }
    } else if ((void *) M2C_FIELD(var_r31, void **, 8) == ((void *) 0)) {
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
        if ((u32) (M2C_FIELD(var_r31, u8 **, 8)) == (u32) (0xFFFFFFFF)) {
            cr_g_ScriptPC = M2C_FIELD(lbl_8044F788, s16 **, 0);
            cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(lbl_8044F788, void **, 4);
            cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
            return;
        }
        cr_g_ScriptPC = M2C_FIELD(M2C_FIELD(var_r31, void **, 8), s16 **, 0);
        cr_g_Script.script_name[cr_g_ScriptDepth] = M2C_FIELD(M2C_FIELD(var_r31, void **, 8), void **, 4);
        cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
    }
}
