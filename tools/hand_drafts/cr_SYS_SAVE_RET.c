/* cr_SYS_SAVE_RET: Nov 14, 2026 hand draft: the save commands: the scenario flag set first in the body, after the prologue (m2c printed it inside the prologue; 43%) */

void cr_SYS_SAVE_RET(void) {
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    cr_g_Scenario.unk_000 |= 1;
    cr_g_ScenarioSlotPtr->status |= lbl_80451124;
    cr_g_Scenario.unk_02d = 0;
    lbl_8044F790 = (cr_ScriptState *)0xFFFE;
}
