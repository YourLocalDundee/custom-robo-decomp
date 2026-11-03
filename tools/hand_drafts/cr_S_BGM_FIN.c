/* cr_S_BGM_FIN: step 110 hand draft (fn_80097670 called with its third parameter u8: the prototype m2c inferred has s8, which adds an extsb the original does not have) */
void cr_S_BGM_FIN(void) {
    s16 *temp_r31_14;
    u32 *cmd_cnt;

    temp_r31_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 8;
    if (0) {
        (*cmd_cnt)++;
    }
    if (((u32) M2C_FIELD(temp_r31_14, u32 *, 4) >> 0x10U) == 0) {
        cr_Bgm_Play(M2C_FIELD(temp_r31_14, u32 *, 4), (u8) M2C_FIELD(temp_r31_14, s16 *, 8), (u8) ((f32) M2C_FIELD(temp_r31_14, s16 *, 0xA) * fn_800965EC()), (u16) (u8) M2C_FIELD(temp_r31_14, s16 *, 0xC));
    } else {
        ((void (*)(s32, u8, u8, u16)) fn_80097670)((s32) M2C_FIELD(temp_r31_14, u32 *, 4), (u8) M2C_FIELD(temp_r31_14, s16 *, 8), (u8) M2C_FIELD(temp_r31_14, s16 *, 0xA), (u16) M2C_FIELD(temp_r31_14, s16 *, 0xC));
    }
    cr_g_ScenarioSlotPtr->unk_ba = (u16) M2C_FIELD(temp_r31_14, u32 *, 4);
    lbl_8044F6C8 |= 1;
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
