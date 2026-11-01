"""Group the schedule-only / register-only drafts of decomp/reports/sched_labels.txt by the block that differs
(docs/drafts.md "Step 92"): the script-command head block, a float block, other. Needs a full batch 12 (decomp/drafts).
usage: python -I tools/sched/group_near.py"""
import sys, re, collections, os
H = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, H); sys.path.insert(0, os.path.dirname(H))
import label_drafts as L, replay as rp, corpus
G = os.path.dirname(os.path.dirname(H))
rows = [l.split() for l in open(os.path.join(G, 'decomp', 'reports', 'sched_labels.txt'))]
rows = [f for f in rows if len(f) >= 3 and f[2].split('+')[0] in ('schedule-only', 'register-only')]
groups = collections.Counter(); out = []
for f in rows:
    n, kind = f[0], f[2]
    ours = L.compile_draft(n); orig = corpus.listing(os.path.join(G, 'decomp', 'drafts', 'asm', n + '.s')).get(n)
    if not ours or not orig:
        groups[(kind.split('+')[0], 'not compiled')] += 1; out.append('%-36s %-20s not compiled' % (n, kind))
        continue
    ob, tb = rp.body_blocks([L.norm(t) for t in ours]), rp.body_blocks([L.norm(t) for t in orig])
    diff = [k for k, ((a, _, _), (b, _, _)) in enumerate(zip(ob, tb)) if [rp.mask(x) for x in a] != [rp.mask(x) for x in b]]
    head = any(any('cr_g_Script@ha' in x for x in tb[k][0]) and any('0x4c(' in x for x in tb[k][0]) for k in diff)
    fl = [k for k in diff if any(x.split()[0] in ('lfs', 'lfd', 'stfs', 'fmuls', 'fadds', 'fsubs') for x in tb[k][0])]
    cls = 'script head block' if head else ('float block' if fl else 'other')
    groups[(kind.split('+')[0], cls)] += 1
    out.append('%-36s %-20s %-18s blocks differing %d of %d' % (n, kind, cls, len(diff), len(tb)))
print('\n'.join(out)); print(groups)
