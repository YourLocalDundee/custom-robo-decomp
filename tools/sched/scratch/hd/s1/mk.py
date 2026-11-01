# structural variants of the pure head
H = '#include "ctx.h"\n'
F = 'void cr_CM_DUMMY_008(void) {\n%s}\n'
V = {}
V['v1_postinc_in_index'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index++] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v2_comma'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc, cr_g_Script.history_index++, cr_g_Script.history_index &= 0x3F;
    cr_g_ScriptPC += 2;
    (*cmd_cnt)++;
'''
V['v3_vol_struct'] = '''    s16 *pc;
    u32 *cmd_cnt;
    volatile cr_ScriptState *S = &cr_g_Script;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v4_vol_pc'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    *(s16 * volatile *) &cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v6_register'] = '''    register s16 *pc;
    register u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v8_cmd_local'] = '''    s16 *pc;
    u32 *cmd_cnt;
    s16 cmd;
    pc = cr_g_ScriptPC;
    cmd = *pc;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = cmd;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v10_pc_long'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC = cr_g_ScriptPC + 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v11_direct_unusedlocal'] = '''    s16 *pc;
    s16 *dummy;
    pc = cr_g_ScriptPC;
    dummy = pc + 1;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
V['v12_ptr_struct_param'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &(&cr_g_Script)->cmd_count[cr_g_ScriptDepth];
    (&cr_g_Script)->cmd_history[(&cr_g_Script)->history_index] = *pc;
    cr_g_ScriptPC += 2;
    (&cr_g_Script)->history_index += 1;
    (&cr_g_Script)->history_index &= 0x3F;
    (*cmd_cnt)++;
'''
V['v13_count_first'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    (*cmd_cnt)++;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
'''
V['v14_idx_local'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    ++cr_g_Script.history_index;
    cr_g_Script.history_index &= 0x3F;
    ++*cmd_cnt;
'''
V['v15_inline_sub'] = None
INL = '''#include "ctx.h"
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
'''
for k, b in V.items():
    if b is None: continue
    open(k + '.c', 'w', newline='\n').write(H + F % b)
open('v15_inline_sub.c', 'w', newline='\n').write(INL)
