"""step 85 docs: fill the counts from the reports and the before/after draft reports, then edit plan.md, docs/drafts.md,
docs/decomp.md (CRLF kept: newline='')"""
import re, os, sys
G = r'D:\emulators\gameTesting-hsd'
S = os.path.dirname(os.path.abspath(__file__))

def rd(p):
    return open(p, encoding='utf-8', newline='').read()

def wr(p, s):
    open(p, 'w', encoding='utf-8', newline='').write(s)

def report(path):
    out = {}
    for l in open(path, encoding='utf-8'):
        m = re.match(r'^([0-9A-F]{8})\s+(\S+)\s+(\d+)\s+(\S+)', l)
        if m:
            out[m.group(2)] = m.group(4)
    return out

def pct(v):
    return int(v.rstrip('%')) if v.endswith('%') else (100 if v == '100%' else -1)

before = report(os.path.join(S, 'draft_report_before.txt'))
after = report(os.path.join(G, 'decomp/drafts/draft_report.txt'))
ups = downs = lost = 0
new = []
for k, v in after.items():
    b = before.get(k)
    if b is None:
        continue
    pa, pb = pct(v), pct(b)
    if pa > pb:
        ups += 1
    elif pa < pb:
        downs += 1
        if pb == 100:
            lost += 1
    if pa == 100 and pb != 100:
        new.append('%s %s -> 100%%' % (k, b))
summ = rd(os.path.join(G, 'decomp/reports/draft_summary.txt'))
def s_int(key, txt):
    m = re.search(r'^%s (\d+)' % key, txt, re.M)
    return int(m.group(1))
summ_before = rd(os.path.join(S, 'draft_summary_before.txt'))
near = lambda t: sum(int(x) for x in re.findall(r'^\s+[89]0-[89]9%\s+(\d+)', t, re.M))
prog = rd(os.path.join(G, 'decomp/reports/progress_report.txt'))
mg = re.search(r'^game\s+(\d+)\s+(\d+)', prog, re.M)
mm = re.search(r'^main\.dol \(all units\)\s+(\d+)\s+(\d+)\s+0x[0-9A-F]+/0x[0-9A-F]+\s+([\d.]+)%\s+0x[0-9A-F]+/0x[0-9A-F]+\s+([\d.]+)%', prog, re.M)
gc = re.search(r'game code .*?: (\d+) objects from C .*?(0x[0-9A-F]+) bytes = ([\d.]+)% of the game code', prog)
promo = rd(os.path.join(G, 'decomp/reports/promote_report.txt'))
mp = re.search(r'promoted: (\d+).*?file objects: (\d+).*?kept from earlier runs[^\n]*: (\d+)', promo, re.S)
runs = [l.rstrip() for l in promo.splitlines() if re.match(r'^[0-9A-F]{8}-[0-9A-F]{8}  ', l)]
vals = dict(
    compile_before=f"{s_int('compiled', summ_before):,}", compile_after=f"{s_int('compiled', summ):,}",
    match_before=s_int('100% MATCH', summ_before), match_after=s_int('100% MATCH', summ),
    new_matches=', '.join(new) if new else 'no new match',
    bytes_before=f"{s_int('bytes matched', summ_before):,}", bytes_after=f"{s_int('bytes matched', summ):,}",
    near_before=near(summ_before), near_after=near(summ), ups=ups, downs=downs, lost=lost,
    objs=mg.group(2), units=mm.group(2), units_all=mm.group(1), code_pct=mm.group(3), data_pct=mm.group(4),
    game_bytes=gc.group(2), game_pct=gc.group(3),
    promoted=mp.group(1) if mp else '?', fileobjs=mp.group(2) if mp else '?', kept=mp.group(3) if mp else '?',
)
print(vals)
print('\n'.join(runs[:6]))
if '--check' in sys.argv:
    sys.exit(0)
vals['run_table'] = sys.argv[sys.argv.index('--table') + 1] if '--table' in sys.argv else '(table)'
txt = open(os.path.join(S, 'docs85_drafts.txt'), encoding='utf-8').read()
for k, v in vals.items():
    txt = txt.replace('{%s}' % k, str(v))
assert '{' not in re.sub(r'\{[a-z_]+\}', '', txt) or True
left = re.findall(r'\{[a-z_]+\}', txt)
assert not left, left
# docs/drafts.md: before "## Using it"
p = os.path.join(G, 'docs/drafts.md'); s = rd(p)
anchor = '\r\n## Using it\r\n'
assert anchor in s and 'Step 85' not in s
s = s.replace(anchor, txt.replace('\n', '\r\n') + anchor, 1)
wr(p, s)
print('drafts.md done')
