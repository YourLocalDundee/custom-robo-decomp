/* cr_CHAR_TURN_RET: step 110 hand draft (the character search as a plain loop with the ID read before it; its result copied into a second local; dead uses in the head rule's if (0) block and temp_r26_31 declared at position 1 fix the register order) */
void cr_CHAR_TURN_RET(void) {
    cr_Chara *temp_r26_31;
    cr_Chara *var_r31;
    s32 var_r29;
    s16 *temp_r30_14;
    u32 *cmd_cnt;
    cr_Chara *var_r27_59;
    s16 temp_r0_84;
    s32 var_r24;

    var_r29 = 0;
    temp_r30_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_14, s16 *, 0);
    temp_r26_31 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
        temp_r30_14 = temp_r30_14;
        temp_r30_14 = temp_r30_14;
        temp_r0_84 = temp_r0_84;
    }
    var_r24 = M2C_FIELD(temp_r30_14, s16 *, 2);
    for (; var_r29 < 0x18; var_r29 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29].update) != NULL) && (var_r24 == (s32) cr_g_Charas[var_r29].id)) {
            var_r27_59 = &cr_g_Charas[var_r29];
            goto found_5;
        }
    }
    var_r27_59 = NULL;
found_5:
    var_r31 = var_r27_59;
    if (var_r31 != NULL) {
        var_r31->flags2 &= 0xFFFFFFDF;
        var_r31->target_angle = (u32) var_r31->move_start_angle;
        var_r31->flags2 = var_r31->flags2 | 0x80000000 | 1;
        temp_r0_84 = (s16) var_r31->move_start_angle - (s16) var_r31->angle;
        M2C_FIELD(var_r31, s16 *, 0x195C) = (s16) M2C_FIELD(temp_r30_14, s16 *, 4);
        var_r31->turn_speed = temp_r0_84 / (s32) M2C_FIELD(var_r31, u16 *, 0x195C);
        cr_g_CurrentChara = var_r31;
        cr_Chara_SetCurrentPointers();
        if (temp_r0_84 & 0x8000) {
            cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->turn_r);
        } else {
            cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->turn_l);
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = temp_r26_31;
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
