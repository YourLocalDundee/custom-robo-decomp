/* cr_E_VIEWOUT: Nov 13, 2026 hand draft: as cr_E_VIEWIN with 0x800 */

static inline void view_set(s32 arg0, s32 arg1, f32 farg0, f32 farg1) {
    f32 rect[4] = { 0.0f, 640.0f, 0.0f, 480.0f };

    fn_800B3640(cr_g_CurrentCam, arg0, arg1, (s32) rect, farg0, farg1);
}

void cr_E_VIEWOUT(void) {
    s16 *var_r31;
    u32 *cmd_cnt;
    s32 var_r29;

    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r31, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r29 = M2C_FIELD(var_r31, s16 *, 2);
    view_set(0x800, var_r29, lbl_804512AC, lbl_804512B0);
    cr_g_ScenarioSlotPtr->status |= lbl_80451100;
}
