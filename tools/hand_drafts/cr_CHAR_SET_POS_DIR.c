/* cr_CHAR_SET_POS_DIR: Nov 6, 2026 hand draft (m2c 76%): the generator's draft with the hex PC step head (draft_batch.py fix, 89%) plus 4 dead uses of the PC copy in the head's `if (0)` block (1 to 3 change nothing; 4 lift it above the chara pointer, r31 as in the original) */

void cr_CHAR_SET_POS_DIR(void) {
    s16 *var_r31;
    u32 *cmd_cnt;
    cr_Chara *var_r30;
    s32 var_r29;
    cr_Chara *var_r28;
    s32 var_r26;

    var_r29 = 0;
    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(var_r31, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 0xA;
    if (0) {
        (*cmd_cnt)++;
        var_r31 = var_r31;
        var_r31 = var_r31;
        var_r31 = var_r31;
        var_r31 = var_r31;
    }
    var_r26 = M2C_FIELD(var_r31, s16 *, 2);
    for (; var_r29 < 0x18; var_r29 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29].update) != NULL) && (var_r26 == (s32) cr_g_Charas[var_r29].id)) {
            var_r28 = &cr_g_Charas[var_r29];
            goto found_5;
        }
    }
    var_r28 = NULL;
found_5:
    var_r30 = var_r28;
    if (var_r30 != NULL) {
        var_r30->flags2 &= 0xFFFFFFDF;
        var_r30->pos.x = M2C_FIELD(var_r31, f32 *, 8);
        var_r30->pos.y = M2C_FIELD(var_r31, f32 *, 0xC);
        var_r30->pos.z = M2C_FIELD(var_r31, f32 *, 0x10);
        var_r30->pos_34.x = M2C_FIELD(var_r31, f32 *, 8);
        var_r30->pos_34.y = M2C_FIELD(var_r31, f32 *, 0xC);
        var_r30->pos_34.z = M2C_FIELD(var_r31, f32 *, 0x10);
        var_r30->angle = (s32) ((f32) (M2C_FIELD(var_r31, s16 *, 4) << 0x10) / lbl_804512A8);
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
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
}
