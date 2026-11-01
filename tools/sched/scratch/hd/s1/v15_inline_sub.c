#include "ctx.h"
static inline void scr_head(cr_ScriptState *s, s16 **ppc, s32 *pdepth, int n) {
    s16 *pc;
    u32 *cmd_cnt;
    pc = *ppc;
    cmd_cnt = &s->cmd_count[*pdepth];
    s->cmd_history[s->history_index] = *pc;
    *ppc += n;
    s->history_index += 1;
    s->history_index &= 0x3F;
    (*cmd_cnt)++;
}
void cr_CM_DUMMY_008(void) {
    scr_head(&cr_g_Script, &cr_g_ScriptPC, &cr_g_ScriptDepth, 2);
}
