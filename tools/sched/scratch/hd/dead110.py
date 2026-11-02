"""Step 110: insert dead-local statements into the head and score. usage: dead110.py OUTDIR"""
import itertools, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
HERE = os.path.dirname(os.path.abspath(__file__))
PR = os.path.join(HERE, 'pr.py')
out = sys.argv[1]; os.makedirs(out, exist_ok=True)
BASE = {'hist': 'S.cmd_history[S.history_index] = (s16) *pc;', 'inc': 'S.history_index += 1;',
        'cnt': 'cmd_cnt = &S.cmd_count[cr_g_ScriptDepth];', 'mask': 'S.history_index &= 0x3F;',
        'count': '(*cmd_cnt)++;', 'adv': 'cr_g_ScriptPC += 2;'}
ORDERS = [('hist', 'inc', 'cnt', 'mask', 'count', 'adv'), ('hist', 'cnt', 'adv', 'inc', 'mask', 'count'),
          ('hist', 'adv', 'inc', 'cnt', 'mask', 'count'), ('hist', 'inc', 'adv', 'cnt', 'mask', 'count'),
          ('hist', 'inc', 'cnt', 'adv', 'mask', 'count'), ('hist', 'inc', 'cnt', 'mask', 'adv', 'count')]
DEAD = ['d = &S.history_index;', 'v = S.history_index;', 's = &S;', 'v = (u32) &S;', 'v = S.history_index += 1;',
        'v = S.history_index++;', 'v = ++S.history_index;', 'v = S.history_index &= 0x3F;', 'd = &S.cmd_count[0];',
        'v = (u32) &S.history_index;', 's = &S; s = &S;', 'd = (u32 *) &S;', 'v = S.unk0;', 'v = S.history_init;',
        'S.history_index;', '&S;', 'v = 0;', 'v = (u32) cr_g_ScriptPC;', 'q = cr_g_ScriptPC;', 'v = *pc;']
REPL = {'inc': ['(s = &S)->history_index += 1;', '*(d = &S.history_index) += 1;', 'v = S.history_index += 1;',
                'v = S.history_index++;', 'v = ++S.history_index;'],
        'mask': ['(s = &S)->history_index &= 0x3F;', '*(d = &S.history_index) &= 0x3F;', 'v = S.history_index &= 0x3F;'],
        'adv': ['q = cr_g_ScriptPC += 2;', 'v = (u32) (cr_g_ScriptPC += 2);'],
        'hist': ['v = S.cmd_history[S.history_index] = (s16) *pc;'],
        'count': ['v = (*cmd_cnt)++;', 'v = ++*cmd_cnt;']}
HDR = ('typedef unsigned int u32; typedef int s32; typedef short s16; typedef unsigned short u16; typedef unsigned char u8;\n'
       'typedef struct { u32 unk0; u32 history_init; u32 history_index; u32 cmd_count[16]; s32 cmd_history[64]; } ST;\n'
       'extern ST cr_g_Script;\nextern s32 cr_g_ScriptDepth;\nextern s16 *cr_g_ScriptPC;\n')
DECLS = ['s16 *pc;', 'u32 *cmd_cnt;', 'u32 *d;', 'u32 v;', 'ST *s;', 's16 *q;']
def src(stmts):
    body = '\n'.join('    ' + x for x in stmts).replace('S.', 'cr_g_Script.').replace('&S', '&cr_g_Script')
    return HDR + 'void cr_CM_DUMMY_008(void) {\n' + '\n'.join('    ' + d for d in DECLS) + '\n    pc = cr_g_ScriptPC;\n' + body + '\n}\n'
variants = []
for o in ORDERS:
    st = [BASE[k] for k in o]
    variants.append(('base', o, st))
    for dd in DEAD:
        for i in range(len(st) + 1):
            variants.append(('dead %s @%d' % (dd, i), o, st[:i] + [dd] + st[i:]))
    for k, alts in REPL.items():
        for a in alts:
            variants.append(('repl %s' % a, o, [a if x == BASE[k] else x for x in st]))
def score(j):
    n, (lab, o, st) = j
    p = os.path.join(out, 'd%05d.c' % n); open(p, 'w').write(src(st))
    r = subprocess.run([sys.executable, '-I', PR, 'cr_CM_DUMMY_008', p, '--quiet'], capture_output=True, text=True)
    m = re.search(r'pos (\d+) / seq (\d+) / ops (\d+) of 38 \(orig 38, ours (\d+)\)', r.stdout)
    for e in ('.o', '.o.s'):
        try: os.remove(p[:-2] + e)
        except OSError: pass
    return (int(m.group(1)), int(m.group(3)), int(m.group(4))) if m else (-1, -1, -1), n, lab, o
with ThreadPoolExecutor(8) as ex:
    res = list(ex.map(score, enumerate(variants)))
res.sort(key=lambda x: (-x[0][0], -x[0][1]))
with open(os.path.join(out, 'scores.txt'), 'w') as f:
    for s, n, lab, o in res: f.write('%s d%05d %s %s\n' % (s, n, lab, o))
for s, n, lab, o in res[:20]: print(s, 'd%05d' % n, lab, o)
