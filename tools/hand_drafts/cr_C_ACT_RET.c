/* cr_C_ACT_RET: Nov 6, 2026 hand draft (m2c 86%): the return stack at chara + 0x1914 (cr_types.h has it as u8 unk_1914[0x20]) is an array member, stack[--n]: the index sum (n * 4 + 0x1914) then lwzx off the reloaded chara pointer, as the original; m2c's temp for the decremented count took a saved register */


typedef struct { u8 pad[0x1914]; short *stack[9]; } cr_CharaRet;
void cr_C_ACT_RET(void) {
    s16 *sp8;

    sp8 = cr_g_ScriptPC;
    cr_g_ScriptPC += 2;
    if ((s32) cr_g_CurrentChara->unk_1938 != 0) {
        cr_g_ScriptPC = ((cr_CharaRet *) cr_g_CurrentChara)->stack[--cr_g_CurrentChara->unk_1938];
        if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
            if ((s16 *) cr_g_ScriptPC != NULL) {
                if ((s32) cr_g_ScriptPC & 0x80000000) {
                    if ((s16) *cr_g_ScriptPC < 0x12C) {
                        if (cr_g_ScriptCharaAct != 0) {
                            if ((s16) *cr_g_ScriptPC >= 0x121) {
                                cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                                return;
                            }
                            cr_g_Scenario.error_flags |= lbl_80451184;
                            return;
                        }
                        if ((s16) *cr_g_ScriptPC < 0x121) {
                            cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
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
        cr_g_CurrentChara->unk_19a4 |= lbl_8045115C;
    }
}
