"""Step 110 sweep of cr_CM_DUMMY_008 shapes (standalone sources, own struct types). usage: gen110.py OUTDIR N SEED"""
import itertools, os, random, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
HERE = os.path.dirname(os.path.abspath(__file__))
PR = os.path.join(HERE, 'pr.py')
out, N, seed = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
os.makedirs(out, exist_ok=True)
rnd = random.Random(seed)
HI = ['(s16) *pc', '*pc', 'pc[0]', '(s32) *pc', '(s16) pc[0]']
INC = ['S.history_index += 1;', 'S.history_index++;', '++S.history_index;', 'S.history_index = S.history_index + 1;',
       'S.history_index = 1 + S.history_index;', 'S.history_index -= -1;', 'S.history_index += 1U;',
       'S.history_index += (s16) 1;', '(&S)->history_index++;', 'S.history_index += (u8) 1;']
MASK = ['S.history_index &= 0x3F;', 'S.history_index %= 64;', 'S.history_index = S.history_index & 0x3F;',
        'S.history_index = S.history_index % 64;', 'S.history_index &= 63U;', 'S.history_index &= (u8) 0x3F;',
        'S.history_index = (u8) (S.history_index << 2) >> 2;']
CNT = ['cmd_cnt = &S.cmd_count[cr_g_ScriptDepth];', 'cmd_cnt = S.cmd_count + cr_g_ScriptDepth;',
       'cmd_cnt = &(&S)->cmd_count[cr_g_ScriptDepth];']
COUNT = ['(*cmd_cnt)++;', '*cmd_cnt += 1;', '++*cmd_cnt;', 'cmd_cnt[0]++;', '*cmd_cnt = *cmd_cnt + 1;']
ADV = ['cr_g_ScriptPC += 2;', 'cr_g_ScriptPC = cr_g_ScriptPC + 2;', 'cr_g_ScriptPC = &cr_g_ScriptPC[2];',
       'cr_g_ScriptPC++; cr_g_ScriptPC++;']
TIDX = ['u32', 's32', 'int', 'unsigned long']
TCNT = ['u32', 's32']
THIS = ['s32', 'u32', 'int']
TDEP = ['s32', 'u32', 'int']
DECL = ['pc_first', 'cnt_first']
def orders():
    items = ['hist', 'inc', 'cnt', 'mask', 'count', 'adv']
    for p in itertools.permutations(items):
        if p.index('mask') < p.index('inc') or p.index('count') < p.index('cnt'):
            continue
        yield p
ORD = list(orders())
def make(i):
    c = dict(hi=rnd.choice(HI), inc=rnd.choice(INC), mask=rnd.choice(MASK), cnt=rnd.choice(CNT), count=rnd.choice(COUNT),
             adv=rnd.choice(ADV), tidx=rnd.choice(TIDX), tcnt=rnd.choice(TCNT), this=rnd.choice(THIS), tdep=rnd.choice(TDEP),
             decl=rnd.choice(DECL), ord=rnd.choice(ORD))
    st = dict(hist='S.cmd_history[S.history_index] = %s;' % c['hi'], inc=c['inc'], cnt=c['cnt'], mask=c['mask'],
              count=c['count'], adv=c['adv'])
    body = '\n'.join('    ' + st[k] for k in c['ord']).replace('S.', 'cr_g_Script.').replace('(&S)', '(&cr_g_Script)')
    decls = ['    s16 *pc;', '    %s *cmd_cnt;' % c['tcnt']]
    if c['decl'] == 'cnt_first':
        decls.reverse()
    src = ('typedef unsigned int u32; typedef int s32; typedef short s16; typedef unsigned short u16; typedef unsigned char u8;\n'
           'typedef struct { u32 unk0; u32 history_init; %s history_index; %s cmd_count[16]; %s cmd_history[64]; } ST;\n'
           'extern ST cr_g_Script;\nextern %s cr_g_ScriptDepth;\nextern s16 *cr_g_ScriptPC;\n'
           'void cr_CM_DUMMY_008(void) {\n%s\n    pc = cr_g_ScriptPC;\n%s\n}\n') % (
        c['tidx'], c['tcnt'], c['this'], c['tdep'], '\n'.join(decls), body)
    return c, src
def score(path):
    r = subprocess.run([sys.executable, '-I', PR, 'cr_CM_DUMMY_008', path, '--quiet'], capture_output=True, text=True)
    m = re.search(r'pos (\d+) / seq (\d+) / ops (\d+) of 38 \(orig 38, ours (\d+)\)', r.stdout)
    for e in ('.o', '.o.s'):
        try: os.remove(os.path.splitext(path)[0] + e)
        except OSError: pass
    return (int(m.group(1)), int(m.group(3)), int(m.group(4))) if m else (-1, -1, -1)
jobs = []
seen = set()
while len(jobs) < N:
    c, src = make(len(jobs))
    if src in seen: continue
    seen.add(src)
    p = os.path.join(out, 'v%05d.c' % len(jobs))
    open(p, 'w').write(src)
    jobs.append((p, c))
with ThreadPoolExecutor(8) as ex:
    res = list(ex.map(lambda j: score(j[0]), jobs))
rows = sorted(zip(res, jobs), key=lambda x: (-x[0][0], -x[0][1]))
with open(os.path.join(out, 'scores.txt'), 'w') as f:
    for (pos, ops, w), (p, c) in rows:
        f.write('%2d %2d %2d %s %s\n' % (pos, ops, w, os.path.basename(p), c))
for (pos, ops, w), (p, c) in rows[:15]:
    print(pos, ops, w, os.path.basename(p), c['ord'], c['tidx'], c['inc'], c['mask'])
