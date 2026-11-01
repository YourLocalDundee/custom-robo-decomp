"""restore the decompiled\\ lines of docs/expected_checksums.txt to the committed values (no Ghidra export here)"""
import subprocess, os
os.chdir(r'D:\emulators\gameTesting-hsd')
p = 'docs/expected_checksums.txt'
cur = open(p, encoding='utf-8', newline='').read()
old = subprocess.run(['git', 'show', 'HEAD:' + p], capture_output=True).stdout.decode('utf-8')
nl = '\r\n' if '\r\n' in cur else '\n'
key = lambda l: l.split('  ', 1)[1] if '  ' in l else None
oldl = {key(l): l for l in old.replace('\r\n', '\n').split('\n') if key(l) and key(l).startswith('decompiled\\')}
out = [oldl.get(key(l), l) if key(l) and key(l).startswith('decompiled\\') else l for l in cur.replace('\r\n', '\n').split('\n')]
open(p, 'w', encoding='utf-8', newline='').write(nl.join(out))
print('restored', len(oldl))
