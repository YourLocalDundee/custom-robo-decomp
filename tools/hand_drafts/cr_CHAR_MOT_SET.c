/* cr_CHAR_MOT_SET: step 110 hand draft (the head rule plus 1 dead use(s) of var_r31 in its `if (0)` block: dead uses count for the register order, so var_r31 gets the register it has in the original) */
void cr_CHAR_MOT_SET(void) {
    cr_Chara *var_r28;
    cr_Chara *var_r29;
    s16 *var_r31;
    u32 *cmd_cnt;
    s32 var_r26;
    s32 var_r30;

    var_r30 = 0;
    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r31, s16 *, 0);
    var_r28 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
        var_r31 = var_r31;
    }
    var_r26 = M2C_FIELD(var_r31, s16 *, 2);
    for (; var_r30 < 0x18; var_r30 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r30].update) != NULL) && (var_r26 == (s32) cr_g_Charas[var_r30].id)) {
            var_r29 = &cr_g_Charas[var_r30];
            goto found_5;
        }
    }
    var_r29 = NULL;
found_5:
    cr_g_CurrentChara = var_r29;
    if (var_r29 != NULL) {
        cr_g_CurrentChara->unk_194c = (u32) cr_g_CurrentChara->motion;
        cr_Chara_SetCurrentPointers();
        if (M2C_FIELD(var_r31, s16 *, 6) == 0) {
            cr_Motion_PlayFromFrame1((u32) M2C_FIELD(cr_g_CurrentChara->motion_set, u16 *, M2C_FIELD(var_r31, s16 *, 4) * 2));
        } else {
            cr_Motion_PlayFromFrame1((u32) M2C_FIELD(var_r31, s16 *, 4));
        }
        if (cr_g_ScenarioSlotPtr->status & 0x80000) {
            M2C_FIELD(cr_g_CurrentChara, s16 *, 0x9EA) = 0;
            cr_g_CurrentChara->unk_09f1 = 0;
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = var_r28;
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
}
