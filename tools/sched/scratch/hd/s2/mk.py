H = '#include "ctx.h"\n'
F = 'void cr_CM_DUMMY_008(void) {\n%s}\n'
V = {}
V['h1_histptr'] = '''    s16 *pc;
    s32 *hist;
    pc = cr_g_ScriptPC;
    hist = &cr_g_Script.cmd_history[cr_g_Script.history_index];
    *hist = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
V['h2_histptr_pcfirst'] = '''    s16 *pc;
    s32 *hist;
    hist = &cr_g_Script.cmd_history[cr_g_Script.history_index];
    pc = cr_g_ScriptPC;
    *hist = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
V['h3_idxptr'] = '''    s16 *pc;
    u32 *idx;
    pc = cr_g_ScriptPC;
    idx = &cr_g_Script.history_index;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    (*idx)++;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
V['h4_idxptr2'] = '''    s16 *pc;
    u32 *idx;
    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    idx = &cr_g_Script.history_index;
    (*idx) &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
V['h5_unused_param'] = None
V['h6_cnt_direct_pp'] = '''    s16 *pc;
    u32 *cmd_cnt;
    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];
    (*cmd_cnt)++;
'''
V['h7_pcptr'] = '''    s16 *pc;
    s16 **ppc;
    ppc = &cr_g_ScriptPC;
    pc = *ppc;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    *ppc += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
V['h8_structptr'] = '''    s16 *pc;
    cr_ScriptState *s;
    s = &cr_g_Script;
    pc = cr_g_ScriptPC;
    s->cmd_history[s->history_index] = *pc;
    cr_g_ScriptPC += 2;
    s->history_index += 1;
    s->history_index &= 0x3F;
    s->cmd_count[cr_g_ScriptDepth]++;
'''
V['h9_histptr_late'] = '''    s16 *pc;
    s32 *hist;
    pc = cr_g_ScriptPC;
    hist = cr_g_Script.cmd_history + cr_g_Script.history_index;
    *hist = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
'''
for k, b in V.items():
    if b is None: continue
    open(k + '.c', 'w', newline='\n').write(H + F % b)
open('h5_unused_param.c', 'w', newline='\n').write('''#include "ctx.h"
#undef cr_CM_DUMMY_008
void cr_CM_DUMMY_008(int unused) {
    s16 *pc;
    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;
    cr_g_ScriptPC += 2;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
}
''')
