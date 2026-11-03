/* cr_CHAR_MOVE_COURSE_PS: step 110 hand draft (the head rule plus a dead use of var_r30; the int-to-float constant is the draft's own copy, equal with relocations masked) */
void cr_CHAR_MOVE_COURSE_PS(void) {
    cr_Chara *var_r27;
    cr_Chara *var_r28;
    cr_Chara *var_r31;
    s16 *var_r30;
    u32 *cmd_cnt;
    s32 var_r25;
    s32 var_r29;

    var_r29 = 0;
    var_r30 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r30, s16 *, 0);
    var_r27 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
        var_r30 = var_r30;
    }
    var_r25 = M2C_FIELD(var_r30, s16 *, 2);
    for (; var_r29 < 0x18; var_r29 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29].update) != NULL) && (var_r25 == (s32) cr_g_Charas[var_r29].id)) {
            var_r28 = &cr_g_Charas[var_r29];
            goto found_5;
        }
    }
    var_r28 = NULL;
found_5:
    var_r31 = var_r28;
    if (var_r31 != NULL) {
        var_r31->flags2 &= 0xFFFFFFDF;
        fn_800D0E60(var_r31, &var_r31->unk_09f8, (f32) M2C_FIELD(var_r30, s16 *, 4));
        var_r31->flags2 |= 0x200;
        cr_g_CurrentChara = var_r31;
        cr_Chara_SetCurrentPointers();
        var_r31->flags2 &= 0xFFFFFFFB;
        if (M2C_FIELD(var_r30, s16 *, 6) == 1) {
            cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->walk_fa);
        } else if (M2C_FIELD(var_r30, s16 *, 6) == 2) {
            cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->run);
        } else {
            var_r31->flags2 |= 4;
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = var_r27;
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
