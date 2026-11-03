#define cr_CM_DUMMY_008 cr_CM_DUMMY_008__proto
#include "ctx.h"
#undef cr_CM_DUMMY_008

/* step 110: the script-command head at 38 of 38 (decomp-permuter found the order, the dead-code counter found r30) */
void cr_CM_DUMMY_008(void) {
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) *pc;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
}
