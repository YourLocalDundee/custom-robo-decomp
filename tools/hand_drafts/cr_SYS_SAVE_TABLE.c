/* cr_SYS_SAVE_TABLE: Nov 6, 2026 hand draft (m2c 44%): the m2c draft with the head of the matched script handlers (history store, index, count, then the PC step, with the dead (*cmd_cnt)++ in an `if (0)` block) in place of m2c's cmd_cnt pointer head */

void cr_SYS_SAVE_TABLE(void) {
    s16 *temp_r31_14;
    u32 *cmd_cnt;

    temp_r31_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    cr_g_Scenario.unk_000 |= 1;
    if (0) {
        (*cmd_cnt)++;
    }
    cr_g_ScenarioSlotPtr->status |= lbl_80451124;
    cr_g_Scenario.unk_02d = 0;
    lbl_8044F790 = M2C_FIELD(temp_r31_14, cr_ScriptState **, 4);
}
