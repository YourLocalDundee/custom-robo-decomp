#include "ctx.h"
void cr_CM_DUMMY_008(void) {
    s16 *pc;
    s16 **ppc;
    ppc = &cr_g_ScriptPC;
    pc = *ppc;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    *ppc += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
}
