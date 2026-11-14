/* cr_E_FIN: Nov 14, 2026 hand draft: the wipe / fade commands (41%): the time argument a u32 local read right before the mode stores (it keeps the original's fourth saved register), the order mode, |= 0x2000, 255 / time (as in fn_800EAC48), then the flag word cleared and set; m2c printed the stores in their scheduled order */

void cr_E_FIN(void) {
    s16 *pc;
    u32 *cmd_cnt;
    u32 time;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    time = pc[1];
    cr_g_SeqCurrent.unk_40 = 2;
    cr_g_SeqCurrent.unk_40 |= 0x2000;
    lbl_8044F488 = 0xFFU / time;
    *lbl_803BA1E8 &= ~*lbl_803BA1E8;
    *lbl_803BA1E8 |= 1;
    cr_g_ScenarioSlotPtr->status |= lbl_80451100;
}
