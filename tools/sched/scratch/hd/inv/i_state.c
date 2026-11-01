#include "ctx.h"
void cr_CM_DUMMY_008(void) {
    s16 *pc;
    cr_ScriptState *s;
    pc = cr_g_ScriptPC;
    s = &cr_g_Script;
    s->cmd_history[s->history_index] = *pc;
    cr_g_ScriptPC += 2;
    s->history_index += 1;
    s->history_index &= 0x3F;
    s->cmd_count[cr_g_ScriptDepth]++;
}
