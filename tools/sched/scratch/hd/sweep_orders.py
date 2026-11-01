"""all statement orders x pointer kind/position. usage: python -I sweep_orders.py OUTDIR [cnt|hist|none]"""
import sys, os, itertools
out, kind = sys.argv[1], sys.argv[2]
os.makedirs(out, exist_ok=True)
PCR = '    pc = cr_g_ScriptPC;\n'
HIST = '    cr_g_Script.cmd_history[cr_g_Script.history_index] = *pc;\n'
HISTP = '    *hist = *pc;\n'
ADV = '    cr_g_ScriptPC += 2;\n'
INC = '    cr_g_Script.history_index += 1;\n'
MASK = '    cr_g_Script.history_index &= 0x3F;\n'
CNT = '    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;\n'
CNTP = '    (*cmd_cnt)++;\n'
SETC = '    cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];\n'
SETH = '    hist = &cr_g_Script.cmd_history[cr_g_Script.history_index];\n'
n = 0
for perm in itertools.permutations(['H', 'A', 'I', 'M', 'C']):
    if kind == 'cnt':
        base = [{'H': HIST, 'A': ADV, 'I': INC, 'M': MASK, 'C': CNTP}[k] for k in perm]
        ci = perm.index('C')
        for pos in range(ci + 1):
            st = base[:]; st.insert(pos, SETC)
            body = '    s16 *pc;\n    u32 *cmd_cnt;\n' + PCR + ''.join(st)
            open(os.path.join(out, 'c_%s_p%d.c' % (''.join(perm), pos)), 'w', newline='\n').write('#include "ctx.h"\nvoid cr_CM_DUMMY_008(void) {\n' + body + '}\n'); n += 1
    elif kind == 'hist':
        base = [{'H': HISTP, 'A': ADV, 'I': INC, 'M': MASK, 'C': CNT}[k] for k in perm]
        hi = perm.index('H')
        for pos in range(hi + 1):
            st = base[:]; st.insert(pos, SETH)
            body = '    s16 *pc;\n    s32 *hist;\n' + PCR + ''.join(st)
            open(os.path.join(out, 'h_%s_p%d.c' % (''.join(perm), pos)), 'w', newline='\n').write('#include "ctx.h"\nvoid cr_CM_DUMMY_008(void) {\n' + body + '}\n'); n += 1
    else:
        base = [{'H': HIST, 'A': ADV, 'I': INC, 'M': MASK, 'C': CNT}[k] for k in perm]
        body = '    s16 *pc;\n' + PCR + ''.join(base)
        open(os.path.join(out, 'n_%s.c' % ''.join(perm)), 'w', newline='\n').write('#include "ctx.h"\nvoid cr_CM_DUMMY_008(void) {\n' + body + '}\n'); n += 1
print(n)
