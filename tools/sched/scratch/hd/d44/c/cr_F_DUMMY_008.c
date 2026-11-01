// m2c draft of cr_F_DUMMY_008 (0x800BEC1C, 152 bytes): decompiled output, gitignored, never commit
#define cr_F_DUMMY_008 cr_F_DUMMY_008__proto
#include "ctx.h"
#undef cr_F_DUMMY_008

void cr_F_DUMMY_008(void) {
    s16 *var_r31;
    u32 *cmd_cnt;

    var_r31 = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s32) *var_r31;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
}
