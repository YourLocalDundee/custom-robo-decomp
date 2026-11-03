/* cr_CHAR_FOLLOW_SET: step 110 hand draft (dead uses of the PC and the follow flag in the head rule's if (0) block; the two searched IDs and the two results declared in the other order) */
void cr_CHAR_FOLLOW_SET(void) {
    cr_Chara *var_r26;
    cr_Chara *var_r25;
    cr_Chara *var_r30;
    cr_Chara *var_r31;
    s16 *var_r29;
    u32 *cmd_cnt;
    s32 var_r23;
    s32 var_r22;
    s32 var_r27;
    s32 var_r28;

    var_r30 = NULL;
    var_r31 = NULL;
    var_r28 = 0;
    var_r29 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r29, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
        var_r29 = var_r29;
        var_r29 = var_r29;
        var_r28 = var_r28;
    }
    var_r23 = M2C_FIELD(var_r29, s16 *, 2);
    for (; var_r28 < 0x18; var_r28 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r28].update) != NULL) && (var_r23 == (s32) cr_g_Charas[var_r28].id)) {
            var_r26 = &cr_g_Charas[var_r28];
            goto found_5;
        }
    }
    var_r26 = NULL;
found_5:
    var_r30 = var_r26;
    if (var_r30 != NULL) {
        var_r30->flags2 |= 0xC0;
        var_r30->unk_0b39 = 0x3C;
        if (M2C_FIELD(var_r29, s16 *, 4) == 0xFF) {
            var_r31 = cr_g_Charas;
        } else {
            var_r22 = M2C_FIELD(var_r29, s16 *, 4);
            var_r27 = 0;
            for (; var_r27 < 0x18; var_r27 += 1) {
                if ((((void (*)()) cr_g_Charas[var_r27].update) != NULL) && (var_r22 == (s32) cr_g_Charas[var_r27].id)) {
                    var_r25 = &cr_g_Charas[var_r27];
                    goto found_15;
                }
            }
            var_r25 = NULL;
found_15:
            var_r31 = var_r25;
        }
        if (var_r31 != NULL) {
            if (M2C_FIELD(var_r29, s16 *, 6) == 0) {
                M2C_FIELD(var_r30, cr_Chara **, 0x199C) = (cr_Chara *) M2C_FIELD(var_r31, cr_Chara **, 0x199C);
                if ((cr_Chara *) M2C_FIELD(var_r31, cr_Chara **, 0x199C) != NULL) {
                    M2C_FIELD(M2C_FIELD(var_r31, cr_Chara **, 0x199C), cr_Chara **, 0x1998) = var_r30;
                }
                M2C_FIELD(var_r30, cr_Chara **, 0x1998) = var_r31;
                M2C_FIELD(var_r31, cr_Chara **, 0x199C) = var_r30;
            } else {
                while ((cr_Chara *) M2C_FIELD(var_r31, cr_Chara **, 0x199C) != NULL) {
                    var_r31 = M2C_FIELD(var_r31, cr_Chara **, 0x199C);
                }
                M2C_FIELD(var_r31, cr_Chara **, 0x199C) = var_r30;
                M2C_FIELD(var_r30, cr_Chara **, 0x1998) = var_r31;
            }
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
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
