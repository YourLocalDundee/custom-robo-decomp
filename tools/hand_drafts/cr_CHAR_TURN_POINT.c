/* cr_CHAR_TURN_POINT: Nov 6, 2026 hand draft (m2c 89%): the search inline takes the id as an s32 (lha r24 early); the dead-code counter pointer cmd_cnt declared after the chara pointers (its place in the declarations decides r25 / r26; found by permuter queue 7 in cr-linux) */

static inline cr_Chara *cr_CHAR_TURN_POINT_search0(s32 id) {
    s32 var_r29_11;

    for (var_r29_11 = 0; var_r29_11 < 0x18; var_r29_11 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29_11].update) != NULL) && (id == (s32) cr_g_Charas[var_r29_11].id)) {
            return &cr_g_Charas[var_r29_11];
        }
    }
    return NULL;
}

void cr_CHAR_TURN_POINT(void) {
    s16 *temp_r30_14;
    cr_Chara *var_r27_59;
    cr_Chara *temp_r26_31;
    u32 *cmd_cnt;
    s16 temp_r0_92;
    if (0) {
        temp_r30_14 = temp_r30_14;
    }

    temp_r30_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_14, s16 *, 0);
    temp_r26_31 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 8;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r27_59 = cr_CHAR_TURN_POINT_search0(M2C_FIELD(temp_r30_14, s16 *, 2));
    if (var_r27_59 != NULL) {
        var_r27_59->flags2 &= 0xFFFFFFDF;
        var_r27_59->move_start_angle = var_r27_59->angle;
        var_r27_59->flags2 = var_r27_59->flags2 | 0x80000000 | 1;
        var_r27_59->target_angle = fn_8001F060(var_r27_59->pos.x - M2C_FIELD(temp_r30_14, f32 *, 8), var_r27_59->pos.z - M2C_FIELD(temp_r30_14, f32 *, 0xC));
        temp_r0_92 = (s16) var_r27_59->target_angle - (s16) var_r27_59->angle;
        M2C_FIELD(var_r27_59, s16 *, 0x195C) = (s16) M2C_FIELD(temp_r30_14, s16 *, 4);
        var_r27_59->turn_speed = temp_r0_92 / (s32) M2C_FIELD(var_r27_59, u16 *, 0x195C);
        cr_g_CurrentChara = var_r27_59;
        cr_Chara_SetCurrentPointers();
        if (temp_r0_92 & 0x8000) {
            cr_Motion_PlayFromFrame1((u32) var_r27_59->motion_set->turn_r);
        } else {
            cr_Motion_PlayFromFrame1((u32) var_r27_59->motion_set->turn_l);
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = temp_r26_31;
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
