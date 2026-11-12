/* cr_C_ACT_CALL: Nov 6, 2026 hand draft (m2c 83%): the push onto the script return stack as an array member, stack[n++] = PC (as cr_C_ACT_RET's stack[--n]); m2c's temp for the count took a saved register */


typedef struct { u8 pad[0x1914]; short *stack[9]; } cr_CharaRet;
void cr_C_ACT_CALL(void) {
    s16 *pc;

    pc = cr_g_ScriptPC;
    cr_g_ScriptPC += 4;
    if ((s32) cr_g_CurrentChara->unk_1938 < 7) {
        ((cr_CharaRet *) cr_g_CurrentChara)->stack[cr_g_CurrentChara->unk_1938++] = cr_g_ScriptPC;
        cr_g_ScriptPC = *M2C_FIELD(pc, s16 ***, 4);
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
        cr_g_CurrentChara->unk_19a4 |= lbl_80451160;
    }
}
