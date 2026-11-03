/* cr_F_PARTS_GET: step 110 hand draft (the byte offset into cr_g_Scenario added to a u8 pointer: m2c added it to the struct pointer, which C scales by the struct size) */
void cr_F_PARTS_GET(void) {
    s16 *temp_r31_15;
    u32 *cmd_cnt;
    u32 *temp_r30_45;

    temp_r31_15 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_15, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    temp_r30_45 = (unsigned long *) ((u8 *) &cr_g_Scenario + (M2C_FIELD(temp_r31_15, s16 *, 2) << 5) + 0x288);
    if (cr_Bits_Test(temp_r30_45, (u32) M2C_FIELD(temp_r31_15, s16 *, 4)) == 0) {
        cr_Bits_Set(temp_r30_45 + 6, (u32) M2C_FIELD(temp_r31_15, s16 *, 4));
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
