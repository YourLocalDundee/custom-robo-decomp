/* cr_CHAR_MOVE_REL: Nov 6, 2026 hand draft (m2c 89%): the head of the matched script handlers (history, count, then PC += 0xA; m2c had a cmd_cnt pointer and the PC step first), the saved current chara read right after the history store, and dead uses of var_r26 / var_r25 / var_r24 in an `if (0)` block (their register order) */

void cr_CHAR_MOVE_REL(void) {
    cr_Chara *var_r31;
    s16 *var_r30;
    u32 *cmd_cnt;
    s32 var_r29;
    s32 var_r28;
    cr_Chara *var_r27;
    cr_Chara *var_r26;
    cr_Chara *var_r25;
    cr_Chara *var_r24;
    s32 var_r22;
    s32 var_r21;
    if (0) {
        var_r26 = var_r26;
        var_r25 = var_r25;
        var_r24 = var_r24;
    }

    var_r29 = 0;
    var_r30 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r30, s16 *, 0);
    var_r24 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 0xA;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r22 = M2C_FIELD(var_r30, s16 *, 2);
    for (; var_r29 < 0x18; var_r29 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29].update) != NULL) && (var_r22 == (s32) cr_g_Charas[var_r29].id)) {
            var_r26 = &cr_g_Charas[var_r29];
            goto found_5;
        }
    }
    var_r26 = NULL;
found_5:
    var_r31 = var_r26;
    if (var_r31 != NULL) {
        var_r21 = M2C_FIELD(var_r30, s16 *, 4);
        var_r28 = 0;
        for (; var_r28 < 0x18; var_r28 += 1) {
            if ((((void (*)()) cr_g_Charas[var_r28].update) != NULL) && (var_r21 == (s32) cr_g_Charas[var_r28].id)) {
                var_r25 = &cr_g_Charas[var_r28];
                goto found_13;
            }
        }
        var_r25 = NULL;
found_13:
        var_r27 = var_r25;
        if (var_r27 != NULL) {
            var_r31->flags2 |= 2;
            var_r31->flags2 &= 0xFFFFFFDF;
            var_r31->move_target.x = var_r27->pos.x + M2C_FIELD(var_r30, f32 *, 8);
            var_r31->move_target.z = var_r27->pos.z + M2C_FIELD(var_r30, f32 *, 0xC);
            var_r31->countdown_198c = (u16) M2C_FIELD(var_r30, s16 *, 0x10);
            var_r31->unk_0010 = (var_r31->move_target.x - var_r31->pos.x) / (f32) var_r31->countdown_198c;
            var_r31->unk_0018 = (var_r31->move_target.z - var_r31->pos.z) / (f32) var_r31->countdown_198c;
            cr_g_CurrentChara = var_r31;
            cr_Chara_SetCurrentPointers();
            var_r31->flags2 &= 0xFFFFFFFB;
            if (M2C_FIELD(var_r30, s16 *, 0x12) == 1) {
                cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->walk_fa);
            } else if (M2C_FIELD(var_r30, s16 *, 0x12) == 2) {
                cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->run);
            } else {
                var_r31->flags2 |= 4;
            }
        } else {
            cr_g_Scenario.error_flags |= 0x6000;
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = var_r24;
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
