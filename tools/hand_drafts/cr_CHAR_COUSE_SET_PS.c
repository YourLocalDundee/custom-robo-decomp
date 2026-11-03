/* cr_CHAR_COUSE_SET_PS: step 110 hand draft (the slot count tested without (u8) casts; each course point stored at `(u8 *) p + 0x9FC + i * 0xC`, the constant before the index: the original's stfsx) */
void cr_CHAR_COUSE_SET_PS(void) {
    f32 var_f31;
    cr_Chara *var_r31;
    s16 *var_r30;
    u32 *cmd_cnt;
    s32 var_r29;
    cr_Chara *var_r28;
    s32 var_r26;

    var_r30 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r30, s16 *, 0);
    var_r29 = 0;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 6;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r26 = M2C_FIELD(var_r30, s16 *, 2);
    for (; var_r29 < 0x18; var_r29 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29].update) != NULL) && (var_r26 == (s32) cr_g_Charas[var_r29].id)) {
            var_r28 = &cr_g_Charas[var_r29];
            goto found_5;
        }
    }
    var_r28 = NULL;
found_5:
    var_r31 = var_r28;
    if ((var_r31 != NULL) && (var_r31->unk_09f8 < 0xFU)) {
        if (var_r31->unk_09f8 == 0) {
            var_r31->unk_09f8 = 1;
        }
        *(f32 *) ((u8 *) var_r31 + 0x9FC + var_r31->unk_09f8 * 0xC) = M2C_FIELD(var_r30, f32 *, 4);
        *(f32 *) ((u8 *) var_r31 + 0xA04 + var_r31->unk_09f8 * 0xC) = M2C_FIELD(var_r30, f32 *, 8);
        if ((s8) cr_g_Script.unk_467 != 0) {
            var_f31 = fn_8006207C(M2C_FIELD(var_r30, f32 *, 4), lbl_80451280, M2C_FIELD(var_r30, f32 *, 8), lbl_80451280);
        } else {
            var_f31 = lbl_80451280;
        }
        *(f32 *) ((u8 *) var_r31 + 0xA00 + var_r31->unk_09f8 * 0xC) = var_f31;
        var_r31->unk_09f8 += 1;
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
        /* Duplicate return node #29. Try simplifying control flow for better match */
        return;
    }
}
