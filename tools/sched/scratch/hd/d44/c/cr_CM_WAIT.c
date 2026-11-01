// m2c draft of cr_CM_WAIT (0x800C7D18, 220 bytes): decompiled output, gitignored, never commit
#define cr_CM_WAIT cr_CM_WAIT__proto
#include "ctx.h"
#undef cr_CM_WAIT

void cr_CM_WAIT(void) {
    u8 *var_r31;
    s16 *var_r30;
    u32 *cmd_cnt;

    var_r30 = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s32) *var_r30;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    var_r31 = cr_g_CurrentCam;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
    if (cr_g_ScenarioSlotPtr->status & 0x10) {
        cr_g_Scenario.wait_timer = M2C_FIELD(var_r31, s32 *, 0xFC);
        cr_g_ScenarioSlotPtr->status |= lbl_80451104;
    }
}
