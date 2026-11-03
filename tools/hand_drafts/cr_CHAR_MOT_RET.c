/* cr_CHAR_MOT_RET: step 110 hand draft (the character search as a plain loop with the ID read before it, as in cr_CHAR_SET_MODEL; the dead-code counter declared fifth and one dead use of the PC give the original's registers) */
void cr_CHAR_MOT_RET(void) {
    s32 var_r31;
    s16 *temp_r30_14;
    cr_Chara *var_r29_59;
    cr_Chara *temp_r28_31;
    u32 *cmd_cnt;
    s32 var_r26;

    var_r31 = 0;
    temp_r30_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_14, s16 *, 0);
    temp_r28_31 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
        temp_r30_14 = temp_r30_14;
    }
    var_r26 = M2C_FIELD(temp_r30_14, s16 *, 2);
    for (; var_r31 < 0x18; var_r31 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r31].update) != NULL) && (var_r26 == (s32) cr_g_Charas[var_r31].id)) {
            var_r29_59 = &cr_g_Charas[var_r31];
            goto found_5;
        }
    }
    var_r29_59 = NULL;
found_5:
    cr_g_CurrentChara = var_r29_59;
    if (var_r29_59 != NULL) {
        cr_Chara_SetCurrentPointers();
        cr_Motion_PlayFromFrame1(cr_g_CurrentChara->unk_194c);
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = temp_r28_31;
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
