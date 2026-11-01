#include "ctx.h"
void cr_CM_DUMMY_008(void) {
    s16 *pc;
    u32 *ix;
    pc = cr_g_ScriptPC;
    ix = &cr_g_Script.history_index;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    *ix += 1;
    *ix &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
}
