/* cr_CM_WAIT: step 110 hand draft (the head rule plus 1 dead use(s) of var_r31 in its `if (0)` block: dead uses count for the register order, so var_r31 gets the register it has in the original) */
void cr_CM_WAIT(void) {
    u8 *var_r31;
    s16 *var_r30;
    u32 *cmd_cnt;

    var_r30 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) *var_r30;
    cr_g_Script.history_index += 1;
    var_r31 = cr_g_CurrentCam;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
        var_r31 = var_r31;
    }
    if (cr_g_ScenarioSlotPtr->status & 0x10) {
        cr_g_Scenario.wait_timer = M2C_FIELD(var_r31, s32 *, 0xFC);
        cr_g_ScenarioSlotPtr->status |= lbl_80451104;
    }
}
