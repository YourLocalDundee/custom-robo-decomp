/* cr_CM_SHAKE: step 110 hand draft (the m2c draft with the head rule, then: the clamped value an s32 local: no extsh before the compares) */
void cr_CM_SHAKE(void) {
    s32 var_r31;
    u8 *var_r30;
    s16 *var_r29;
    u32 *cmd_cnt;

    var_r29 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r29, s16 *, 0);
    var_r30 = cr_g_CurrentCam;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    M2C_FIELD(var_r30, f32 *, 0x140) = (f32) M2C_FIELD(var_r29, f32 *, 4);
    var_r31 = M2C_FIELD(var_r29, s16 *, 2);
    if (var_r31 < 0) {
        var_r31 = 0x12C;
    } else if (var_r31 > 0x12C) {
        var_r31 = 0x12C;
    }
    M2C_FIELD(var_r30, s16 *, 0x13C) = var_r31;
    M2C_FIELD(var_r30, s16 *, 0x12E) = var_r31;
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
