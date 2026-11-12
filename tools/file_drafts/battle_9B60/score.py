"""score.py FUNC [OUTDIR]: hd.py + relcheck.py of tools/file_drafts/battle_9B60/FUNC.c with pool.inc in front."""
import os
import subprocess
import sys
import tempfile

here = os.path.dirname(os.path.abspath(__file__))
func = sys.argv[1]
out = sys.argv[2] if len(sys.argv) > 2 else 'decomp/build/draftscan'
src = open(os.path.join(here, 'pool.inc')).read() + '\n' + open(os.path.join(here, func + '.c')).read()
fd, tmp = tempfile.mkstemp(suffix='.c')
with os.fdopen(fd, 'w') as f:
    f.write(src)
try:
    for tool in ('hd.py', 'relcheck.py'):
        r = subprocess.run([sys.executable, '-I', 'tools/sched/scratch/hand/' + tool, out, func, tmp],
                           capture_output=True, text=True)
        lines = r.stdout.strip().splitlines()
        print('\n'.join(lines[:40] if tool == 'hd.py' else lines[-3:]))
finally:
    os.unlink(tmp)
