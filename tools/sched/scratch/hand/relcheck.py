"""relcheck.py OUTDIR FUNC DRAFT.c: the relocation symbol at each instruction of our compile (hd.py -r) against
the symbols in the original's .s (OUTDIR/asm/FUNC.s); prints the differing instructions, `relocations equal` if none.
hd.py masks relocated fields, so a hand draft that reorders statements can match word for word with symbols swapped."""
import re
import subprocess
import sys

out, func, src = sys.argv[1:4]
r = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/hd.py', out, func, src, '-r'], capture_output=True, text=True)
mine = {}
for m in re.finditer(r'^\s*([0-9a-f]+): R_PPC_\w+\s+(\S+)', r.stdout, re.M):
    # a literal keeps its addend (`@715+0x1`): dtk labels each byte of a pooled GXColor on its own (lbl_8045025C..F)
    sym = m.group(2) if m.group(2).startswith('@') else re.sub(r'[+-]0x[0-9a-f]+$', '', m.group(2))
    mine[int(m.group(1), 16) // 4] = sym
orig = {}
k = 0
for line in open('%s/asm/%s.s' % (out, func), encoding='utf-8'):
    if not re.match(r'/\* [0-9A-F]{8} ', line):
        continue
    m = re.search(r'([A-Za-z_][\w.$]*)@(?:ha|h|l|sda21|sda2)', line) or re.search(r'\b(?:bl|b)\s+([A-Za-z_]\w*)\s*$', line)
    if m and not m.group(1).startswith('.L'):
        orig[k] = m.group(1)
    k += 1
# our compiler's literal constants (`@729`) stand for the original's pooled ones (lbl_...): accepted when the mapping
# is one to one over the whole function
pool, back, bad = {}, {}, []
for i in sorted(set(orig) | set(mine)):
    a, b = orig.get(i), mine.get(i)
    if a == b:
        continue
    if a and b and b.startswith('@') and pool.setdefault(b, a) == a and back.setdefault(a, b) == b:
        continue
    bad.append((i, a, b))
for i, a, b in bad:
    print('word %d: original %s, ours %s' % (i, a, b))
print(('relocations equal' + (' (%d literal constants mapped)' % len(pool) if pool else '')) if not bad else '%d differ' % len(bad))
