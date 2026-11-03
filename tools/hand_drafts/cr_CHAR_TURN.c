/* cr_CHAR_TURN: step 110 hand draft (both character searches as plain loops with their IDs read before them, the first result copied into a second local; dead uses in the head rule's if (0) block and the counter declared ninth fix the register order; the int-to-float constant is the draft's own copy, equal with relocations masked) */
void cr_CHAR_TURN(void) {
    cr_Chara *var_r31;
    s16 *temp_r30_14;
    s32 var_r29_11;
    s32 var_r28_137;
    cr_Chara *var_r27_133;
    cr_Chara *var_r25_59;
    cr_Chara *var_r24_157;
    cr_Chara *temp_r23_31;
    u32 *cmd_cnt;
    s16 temp_r0_193;
    s32 var_r21;
    s32 var_r20;

    var_r29_11 = 0;
    temp_r30_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_14, s16 *, 0);
    temp_r23_31 = cr_g_CurrentChara;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 6;
    if (0) {
        (*cmd_cnt)++;
        var_r31 = var_r31;
        temp_r0_193 = temp_r0_193;
    }
    var_r21 = M2C_FIELD(temp_r30_14, s16 *, 2);
    for (; var_r29_11 < 0x18; var_r29_11 += 1) {
        if ((((void (*)()) cr_g_Charas[var_r29_11].update) != NULL) && (var_r21 == (s32) cr_g_Charas[var_r29_11].id)) {
            var_r25_59 = &cr_g_Charas[var_r29_11];
            goto found_5;
        }
    }
    var_r25_59 = NULL;
found_5:
    var_r31 = var_r25_59;
    if (var_r31 != NULL) {
        var_r31->flags2 &= 0xFFFFFFDF;
        var_r31->move_start_angle = var_r31->angle;
        switch ((s16) (M2C_FIELD(temp_r30_14, s16 *, 6))) {                       /* irregular */
        case 0:
            var_r31->target_angle = (u32) (s32) ((f32) (M2C_FIELD(temp_r30_14, s16 *, 4) << 0x10) / lbl_804512A8);
            break;
        case 1:
            var_r31->target_angle = (u32) (s32) ((f32) var_r31->angle + ((f32) (M2C_FIELD(temp_r30_14, s16 *, 4) << 0x10) / lbl_804512A8));
            break;
        case 2:
            if (M2C_FIELD(temp_r30_14, s16 *, 4) == 0xFF) {
                var_r27_133 = cr_g_FirstChara;
                goto block_27;
            }
            var_r28_137 = 0;
            var_r20 = M2C_FIELD(temp_r30_14, s16 *, 4);
            for (; var_r28_137 < 0x18; var_r28_137 += 1) {
                if ((((void (*)()) cr_g_Charas[var_r28_137].update) != NULL) && (var_r20 == (s32) cr_g_Charas[var_r28_137].id)) {
                    var_r24_157 = &cr_g_Charas[var_r28_137];
                    goto found_23;
                }
            }
            var_r24_157 = NULL;
found_23:
            var_r27_133 = var_r24_157;
            if (var_r27_133 == NULL) {
                cr_g_Scenario.error_flags |= 0x6000;
            } else {
block_27:
                var_r31->target_angle = fn_8001F060(var_r31->pos.x - var_r27_133->pos.x, var_r31->pos.z - var_r27_133->pos.z);
            }
            break;
        }
        var_r31->flags2 = var_r31->flags2 | 0x80000000 | 1;
        temp_r0_193 = (s16) var_r31->target_angle - (s16) var_r31->angle;
        M2C_FIELD(var_r31, s16 *, 0x195C) = (s16) M2C_FIELD(temp_r30_14, s16 *, 8);
        var_r31->turn_speed = temp_r0_193 / (s32) M2C_FIELD(var_r31, u16 *, 0x195C);
        cr_g_CurrentChara = var_r31;
        cr_Chara_SetCurrentPointers();
        if (temp_r0_193 & 0x8000) {
            cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->turn_r);
        } else {
            cr_Motion_PlayFromFrame1((u32) var_r31->motion_set->turn_l);
        }
    } else {
        cr_g_Scenario.error_flags |= 0x6000;
    }
    cr_g_CurrentChara = temp_r23_31;
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
