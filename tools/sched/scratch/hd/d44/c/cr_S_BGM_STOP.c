// m2c draft of cr_S_BGM_STOP (0x800C4934, 512 bytes): decompiled output, gitignored, never commit
#define cr_S_BGM_STOP cr_S_BGM_STOP__proto
#include "ctx.h"
#undef cr_S_BGM_STOP

void cr_S_BGM_STOP(void) {
    s16 *temp_r31_14;
    u32 *cmd_cnt;

    temp_r31_14 = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s32) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_ScriptPC += 4;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
    if (((u32) M2C_FIELD(temp_r31_14, u32 *, 4) >> 0x10U) == 0) {
        fn_80096F54();
    } else {
        fn_8009760C((s32) M2C_FIELD(temp_r31_14, u32 *, 4));
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
