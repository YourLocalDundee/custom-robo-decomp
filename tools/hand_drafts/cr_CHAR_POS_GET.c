/* cr_CHAR_POS_GET: Nov 6, 2026 hand draft (m2c 87%): stores the position of a character (id at PC+2, 0xFF = the first one) into cr_g_Script.char_pos[PC+4], raised by its y offset; the shape of cr_CHAR_COUSE_CLEAR_PS: the chara pointer set to NULL first, the id read into var_r26 before the search loop, 2 dead uses of the PC copy in its `if (0)` block, and no temporary for the slot index */

void cr_CHAR_POS_GET(void) {
    s16 *temp_r31_14;
    u32 *cmd_cnt;
    cr_Chara *var_r30_44;
    s32 var_r29_48;
    cr_Chara *var_r28_68;
    s32 var_r26;
    if (0) {
        temp_r31_14 = temp_r31_14;
        temp_r31_14 = temp_r31_14;
    }

    var_r30_44 = NULL;
    temp_r31_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    if (M2C_FIELD(temp_r31_14, s16 *, 4) < 8) {
        if (M2C_FIELD(temp_r31_14, s16 *, 2) == 0xFF) {
            var_r30_44 = cr_g_FirstChara;
        } else {
            var_r29_48 = 0;
            var_r26 = M2C_FIELD(temp_r31_14, s16 *, 2);
            for (; var_r29_48 < 0x18; var_r29_48 += 1) {
                if ((((void (*)()) cr_g_Charas[var_r29_48].update) != NULL) && (var_r26 == (s32) cr_g_Charas[var_r29_48].id)) {
                    var_r28_68 = &cr_g_Charas[var_r29_48];
                    goto found_8;
                }
            }
            var_r28_68 = NULL;
found_8:
            var_r30_44 = var_r28_68;
        }
        if (var_r30_44 != NULL) {
            cr_g_Script.char_pos[M2C_FIELD(temp_r31_14, s16 *, 4)].x = var_r30_44->pos.x;
            cr_g_Script.char_pos[M2C_FIELD(temp_r31_14, s16 *, 4)].y = var_r30_44->pos.y;
            cr_g_Script.char_pos[M2C_FIELD(temp_r31_14, s16 *, 4)].z = var_r30_44->pos.z;
            cr_g_Script.char_pos[M2C_FIELD(temp_r31_14, s16 *, 4)].y += lbl_804512A4 + var_r30_44->y_offset;
        } else {
            cr_g_Scenario.error_flags |= 0x6000;
        }
    } else {
        cr_g_Scenario.error_flags |= lbl_80451178;
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
